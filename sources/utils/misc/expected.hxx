#ifndef UTILS_MISC_EXPECTED_HXX
#define UTILS_MISC_EXPECTED_HXX


#include <initializer_list> // std::initializer_list
#include <ostream> // std::ostream
#include <memory> // std::addressof
#include <type_traits> /* std::{enable_if_t, is_array_v, is_assignable_v, is_constructible_v, is_convertible_v,
  is_copy_assignable_v, is_copy_constructible_v, is_default_constructible_v, is_function_v, is_move_assignable_v,
  is_move_constructible_v, is_reference_v, is_same_v, is_void_v} */
#include <utility> // std::{forward, move}

#include "../containers/c-string.hxx" // CString
#include "../debug/fatal.hxx" // FATAL_NL
#include "../type-traits/aligned-union-storage-manager.hxx" // AlignedUnionStorageManager
#include "../type-traits/enable-copy-move.hxx" // EnableCopyMove
#include "../type-traits/remove-c-v-ref.hxx" // RemoveCVRefT


namespace Utils
{
  /**
    * @brief
    */
  struct ResultTag
  {
    explicit constexpr ResultTag (void) noexcept = default;
  };


  /**
   * @brief
   */
  inline constexpr ResultTag Result { };


  /**
   * @brief
   */
  struct ErrorTag
  {
    explicit constexpr ErrorTag (void) noexcept = default;
  };


  /**
   * @brief
   */
  inline constexpr ErrorTag Error { };


  namespace
  {
    template <typename TType>
    constexpr bool IsAllowedV = !(
         std::is_function_v <TType>
      || std::is_reference_v <TType>
      || std::is_void_v <TType>
      || std::is_array_v <TType>
      || std::is_same_v <RemoveCVRefT <TType>, ResultTag>
      || std::is_same_v <RemoveCVRefT <TType>, ErrorTag>
    );


    template <typename TResult, typename TError>
    class ExpectedBase
    {
      static_assert (IsAllowedV <TResult> && IsAllowedV <TError>);


      public:
        /**
         * @brief
         */
        using result_type = TResult;

        /**
         * @brief
         */
        using error_type = TError;

        /**
         * @brief
         */
        using self_type = ExpectedBase;


        /**
         * @brief
         */
        ExpectedBase (void) noexcept = delete;


        /**
         * @brief
         * @param is_result
         */
        ExpectedBase (bool is_result) noexcept
          : is_result_ (is_result)
        { }


        /**
         * @brief
         * @param that
         */
        ExpectedBase (const self_type & that) :
          is_result_ (that.is_result_)
        {
          static_assert (
               std::is_copy_constructible_v <result_type>
            && std::is_copy_constructible_v <error_type>
          );

          if (that.isResult_ ())
          {
            result_or_error_.template construct <result_type> (that.result_ ());
          }
          else
          {
            result_or_error_.template construct <error_type> (that.error_ ());
          }

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @param that
         */
        ExpectedBase (self_type && that) :
          is_result_ (that.is_result_)
        {
          static_assert (
               std::is_move_constructible_v <result_type>
            && std::is_move_constructible_v <error_type>
          );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
          if (that.isResult_ ())
#pragma GCC diagnostic pop
          {
            result_or_error_.template construct <result_type> (std::move (that.result_ ()));
          }
          else
          {
            result_or_error_.template construct <error_type> (std::move (that.error_ ()));
          }

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (const self_type & that)
        {
          static_assert (
               std::is_copy_constructible_v <result_type>
            && std::is_copy_constructible_v <error_type>
            && std::is_copy_assignable_v <result_type>
            && std::is_copy_assignable_v <error_type>
          );

          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                result_or_error_.template assign <result_type> (that.result_ ());
              }
              else
              {
                result_or_error_.template destroy <result_type> ();
                result_or_error_.template construct <error_type> (that.error_ ());
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                result_or_error_.template destroy <error_type> ();
                result_or_error_.template construct <result_type> (that.result_ ());
              }
              else
              {
                result_or_error_.template assign <error_type> (that.error_ ());
              }
            }

            isResult_ (that.isResult_ ());

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
            that.wasChecked_ (true);
            wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS
          }

          return *this;
        }


        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (self_type && that)
        {
          static_assert (
               std::is_move_constructible_v <result_type>
            && std::is_move_constructible_v <error_type>
            && std::is_move_assignable_v <result_type>
            && std::is_move_assignable_v <error_type>
          );

          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                result_or_error_.template assign <result_type> (std::move (that.result_ ()));
              }
              else
              {
                result_or_error_.template destroy <result_type> ();
                result_or_error_.template construct <error_type> (std::move (that.error_ ()));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                result_or_error_.template destroy <error_type> ();
                result_or_error_.template construct <result_type> (std::move (that.result_ ()));
              }
              else
              {
                result_or_error_.template assign <error_type> (std::move (that.error_ ()));
              }
            }

            isResult_ (that.isResult_ ());

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
            that.wasChecked_ (true);
            wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS
          }

          return *this;
        }


      protected:
        /**
         * @brief
         * @return
         */
        const result_type &
        result_ (void) const &
        {
          return result_or_error_.template get <result_type> ();
        }


        /**
         * @brief
         * @return
         */
        result_type &
        result_ (void) &
        {
          return result_or_error_.template get <result_type> ();
        }


        /**
         * @brief
         * @return
         */
        const result_type &&
        result_ (void) const &&
        {
          return std::move (result_or_error_.template get <result_type> ());
        }


        /**
         * @brief
         * @return
         */
        result_type &&
        result_ (void) &&
        {
          return std::move (result_or_error_.template get <result_type> ());
        }


        /**
         * @brief
         * @return
         */
        const result_type &
        result_Safe_ (void) const &
        {
          if (isResult_ ())
          {
            return result_ ();
          }
          else
          {
            FATAL_NL ("Cannot access result because this instance of `Expected' was initialized as an error");
          }
        }


        /**
         * @brief
         * @return
         */
        result_type &
        result_Safe_ (void) &
        {
          if (isResult_ ())
          {
            return result_ ();
          }
          else
          {
            FATAL_NL ("Cannot access result because this instance of `Expected' was initialized as an error");
          }
        }


        /**
         * @brief
         * @return
         */
        const result_type &&
        result_Safe_ (void) const &&
        {
          if (isResult_ ())
          {
            return result_ ();
          }
          else
          {
            FATAL_NL ("Cannot access result because this instance of `Expected' was initialized as an error");
          }
        }


        /**
         * @brief
         * @return
         */
        result_type &&
        result_Safe_ (void) &&
        {
          if (isResult_ ())
          {
            return result_ ();
          }
          else
          {
            FATAL_NL ("Cannot access result because this instance of `Expected' was initialized as an error");
          }
        }


        /**
         * @brief
         * @return
         */
        const result_type &
        result_Checked_ (void) const &
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return result_Safe_ ();
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing result");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return result_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        result_type &
        result_Checked_ (void) &
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return result_Safe_ ();
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing result");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return result_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        const result_type &&
        result_Checked_ (void) const &&
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return result_Safe_ ();
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing result");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return result_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        result_type &&
        result_Checked_ (void) &&
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return result_Safe_ ();
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing result");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return result_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        const error_type &
        error_ (void) const &
        {
          return result_or_error_.template get <error_type> ();
        }


        /**
         * @brief
         * @return
         */
        error_type &
        error_ (void) &
        {
          return result_or_error_.template get <error_type> ();
        }


        /**
         * @brief
         * @return
         */
        const error_type &&
        error_ (void) const &&
        {
          return std::move (result_or_error_.template get <error_type> ());
        }


        /**
         * @brief
         * @return
         */
        error_type &&
        error_ (void) &&
        {
          return std::move (result_or_error_.template get <error_type> ());
        }


        /**
         * @brief
         * @return
         */
        const error_type &
        error_Safe_ (void) const &
        {
          if (isResult_ ())
          {
            FATAL_NL ("Cannot access error because this instance of `Expected' was initialized as a result");
          }
          else
          {
            return error_ ();
          }
        }


        /**
         * @brief
         * @return
         */
        error_type &
        error_Safe_ (void) &
        {
          if (isResult_ ())
          {
            FATAL_NL ("Cannot access error because this instance of `Expected' was initialized as a result");
          }
          else
          {
            return error_ ();
          }
        }


        /**
         * @brief
         * @return
         */
        const error_type &&
        error_Safe_ (void) const &&
        {
          if (isResult_ ())
          {
            FATAL_NL ("Cannot access error because this instance of `Expected' was initialized as a result");
          }
          else
          {
            return std::move (error_ ());
          }
        }


        /**
         * @brief
         * @return
         */
        error_type &&
        error_Safe_ (void) &&
        {
          if (isResult_ ())
          {
            FATAL_NL ("Cannot access error because this instance of `Expected' was initialized as a result");
          }
          else
          {
            return std::move (error_ ());
          }
        }


        /**
         * @brief
         * @return
         */
        const error_type &
        error_Checked_ (void) const &
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return error_Safe_ ();
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing error");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return error_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        error_type &
        error_Checked_ (void) &
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return error_Safe_ ();
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing error");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return error_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        const error_type &&
        error_Checked_ (void) const &&
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return std::move (error_Safe_ ());
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing error");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return error_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        error_type &&
        error_Checked_ (void) &&
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          if (wasChecked_ ())
          {
            return std::move (error_Safe_ ());
          }
          else
          {
            FATAL_NL ("`Expected' should be checked for being an error before accessing error");
          }
#else // EXPECTED_WITH_RUNTIME_CHECKS
          return error_Safe_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] bool
        isResult_ (void) const noexcept
        {
          return is_result_;
        }


        /**
         * @brief
         * @param is_result
         */
        void
        isResult_ (bool is_result) noexcept
        {
          is_result_ = is_result;
        }


#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        /**
         * @brief
         * @return
         */
        bool
        wasChecked_ (void) const noexcept
        {
          return was_checked_;
        }


        /**
         * @brief
         * @param was_checked
         */
        void
        wasChecked_ (bool was_checked) const noexcept
        {
          was_checked_ = was_checked;
        }
#endif // EXPECTED_WITH_RUNTIME_CHECKS


        /**
         * @brief
         */
        AlignedUnionStorageManager <result_type, error_type> result_or_error_;

        /**
         * @brief
         */
        bool is_result_;

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        /**
         * @brief
         */
        mutable bool was_checked_ { false };
#endif // EXPECTED_WITH_RUNTIME_CHECKS
    };
  }


  /**
   * @brief
   * @tparam TResult
   * @tparam TError
   */
  template <typename TResult, typename TError>
  class [[nodiscard]] Expected final :
    private ExpectedBase <TResult, TError>,
    private EnableCopyMove <
      std::is_copy_constructible_v <TResult> && std::is_copy_constructible_v <TError>,
         std::is_copy_constructible_v <TResult> && std::is_copy_constructible_v <TError>
      && std::is_copy_assignable_v <TResult> && std::is_copy_assignable_v <TError>,
      std::is_move_constructible_v <TResult> && std::is_move_constructible_v <TError>,
         std::is_move_constructible_v <TResult> && std::is_move_constructible_v <TError>
      && std::is_move_assignable_v <TResult> && std::is_move_assignable_v <TError>,
      Expected <TResult, TError>
    >
  {
    static_assert (IsAllowedV <TResult> && IsAllowedV <TError>);


    public:
      /**
       * @brief
       */
      using result_type = TResult;

      /**
       * @brief
       */
      using error_type = TError;

      /**
       * @brief
       */
      using self_type = Expected;


    private:
      /**
       * @brief
       */
      using base_type = ExpectedBase <result_type, error_type>;


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       */
      template <typename TThatResult, typename TThatError>
      friend class Expected;


    public:
      /**
       * @brief
       */
      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <result_type>, TDummy> ...>
      Expected (void) :
        base_type (true)
      {
        static_assert (std::is_default_constructible_v <result_type>);

        base_type::result_or_error_.template construct <result_type> ();
      }


      /**
       * @brief
       * @tparam TThatResult
       * @param that_result
       */
      template <
        typename TThatResult,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && !std::is_same_v <self_type, RemoveCVRefT <TThatResult>>
        > ...
      >
      Expected (TThatResult && that_result) :
        base_type (true)
      {
        using that_result_type = TThatResult;


        static_assert (
             std::is_constructible_v <result_type, that_result_type &&>
          && !std::is_same_v <self_type, RemoveCVRefT <TThatResult>>
        );

        base_type::result_or_error_.template construct <result_type> (std::forward <that_result_type> (that_result));
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, const TThatResult &>
          && std::is_constructible_v <error_type, const TThatError &>
          && !std::is_same_v <self_type, Expected <TThatResult, TThatError>>
        > ...
      >
      Expected (const Expected <TThatResult, TThatError> & that) :
        base_type (that.is_result_)
      {
        using that_result_type = TThatResult;
        using that_error_type = TThatError;


        static_assert (
             std::is_constructible_v <result_type, const that_result_type &>
          && std::is_constructible_v <error_type, const that_error_type &>
          && !std::is_same_v <self_type, Expected <that_result_type, that_error_type>>
        );

        if (that.isResult_ ())
        {
          base_type::result_or_error_.template construct <result_type> (that.result_ ());
        }
        else
        {
          base_type::result_or_error_.template construct <error_type> (that.error_ ());
        }

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && std::is_constructible_v <error_type, TThatError &&>
          && !std::is_same_v <self_type, Expected <TThatResult, TThatError>>
        > ...
      >
      Expected (Expected <TThatResult, TThatError> && that) :
        base_type (that.is_result_)
      {
        using that_result_type = TThatResult;
        using that_error_type = TThatError;


        static_assert (
             std::is_constructible_v <result_type, that_result_type &&>
          && std::is_constructible_v <error_type, that_error_type &&>
          && !std::is_same_v <self_type, Expected <that_result_type, that_error_type>>
        );

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
        if (that.isResult_ ())
#pragma GCC diagnostic pop
        {
          base_type::result_or_error_.template construct <result_type> (std::move (that.result_ ()));
        }
        else
        {
          base_type::result_or_error_.template construct <error_type> (std::move (that.error_ ()));
        }

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @param init
       * @param args
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <result_type, std::initializer_list <TInit> &, TArgs && ...>> ...
      >
      Expected (ResultTag, std::initializer_list <TInit> init, TArgs && ... args) :
        base_type (true)
      {
        static_assert (std::is_constructible_v <result_type, std::initializer_list <TInit> &, TArgs && ...>);

        base_type::result_or_error_.template construct <result_type> (init, std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TArgs
       * @param args
       */
      template <typename ... TArgs, std::enable_if_t <std::is_constructible_v <result_type, TArgs && ...>> ...>
      Expected (ResultTag, TArgs && ... args) :
        base_type (true)
      {
        static_assert (std::is_constructible_v <result_type, TArgs && ...>);

        base_type::result_or_error_.template construct <result_type> (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @param init
       * @param args
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, std::initializer_list <TInit> &, TArgs && ...>> ...
      >
      Expected (ErrorTag, std::initializer_list <TInit> init, TArgs && ... args) :
        base_type (false)
      {
        static_assert (std::is_constructible_v <error_type, std::initializer_list <TInit> &, TArgs && ...>);

        base_type::result_or_error_.template construct <error_type> (init, std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TArgs
       * @param args
       */
      template <typename ... TArgs, std::enable_if_t <std::is_constructible_v <error_type, TArgs && ...>> ...>
      Expected (ErrorTag, TArgs && ... args) :
        base_type (false)
      {
        static_assert (std::is_constructible_v <error_type, TArgs && ...>);

        base_type::result_or_error_.template construct <error_type> (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       */
      ~Expected (void)
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        if (!base_type::wasChecked_ ())
        {
          FATAL_NL ("No instance of `Expected' should be left unchecked");
        }
#endif // EXPECTED_WITH_RUNTIME_CHECKS

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
        if (base_type::isResult_ ())
#pragma GCC diagnostic pop
        {
          base_type::result_or_error_.template destroy <result_type> ();
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
        }
      }


      /**
       * @brief
       * @return
       */
      const result_type &
      result (void) const &
      {
        return base_type::result_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      result_type &
      result (void) &
      {
        return base_type::result_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      const result_type &&
      result (void) const &&
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      result_type &&
      result (void) &&
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      error (void) const &
      {
        return base_type::error_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      error_type &
      error (void) &
      {
        return base_type::error_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      const error_type &&
      error (void) const &&
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return std::move (base_type::error_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      error_type &&
      error (void) &&
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return std::move (base_type::error_Safe_ ());
      }


      const result_type &
      expect (const CString & message) const &
      {
        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return base_type::result_ ();
        }
        else
        {
          FATAL_NL (message);
        }
      }


      result_type &
      expect (const CString & message) &
      {
        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return base_type::result_ ();
        }
        else
        {
          FATAL_NL (message);
        }
      }


      const result_type &&
      expect (const CString & message) const &&
      {
        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::move (base_type::result_ ());
        }
        else
        {
          FATAL_NL (message);
        }
      }


      result_type &&
      expect (const CString & message) &&
      {
        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::move (base_type::result_ ());
        }
        else
        {
          FATAL_NL (message);
        }
      }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @param init
       * @param args
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <result_type, std::initializer_list <TInit> &, TArgs && ...>> ...
      >
      result_type &
      emplace (ResultTag, std::initializer_list <TInit> init, TArgs && ... args)
      {
        static_assert (std::is_constructible_v <result_type, std::initializer_list <TInit> &, TArgs && ...>);

        if (base_type::isResult_ ())
        {
          base_type::result_or_error_.template destroy <result_type> ();
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
        }

        base_type::isResult_ (true);

        base_type::result_or_error_.template construct <result_type> (init, std::forward <TArgs> (args) ...);

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return base_type::result_ ();
      }


      /**
       * @brief
       * @tparam TArgs
       * @param args
       */
      template <typename ... TArgs, std::enable_if_t <std::is_constructible_v <result_type, TArgs && ...>> ...>
      result_type &
      emplace (ResultTag, TArgs && ... args)
      {
        static_assert (std::is_constructible_v <result_type, TArgs && ...>);

        if (base_type::isResult_ ())
        {
          base_type::result_or_error_.template destroy <result_type> ();
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
        }

        base_type::isResult_ (true);

        base_type::result_or_error_.template construct <result_type> (std::forward <TArgs> (args) ...);

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return base_type::result_ ();
      }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @param init
       * @param args
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, std::initializer_list <TInit> &, TArgs && ...>> ...
      >
      error_type &
      emplace (ErrorTag, std::initializer_list <TInit> init, TArgs && ... args)
      {
        static_assert (std::is_constructible_v <error_type, std::initializer_list <TInit> &, TArgs && ...>);

        if (base_type::isResult_ ())
        {
          base_type::result_or_error_.template destroy <result_type> ();
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
        }

        base_type::result_or_error_.template construct <error_type> (init, std::forward <TArgs> (args) ...);

        base_type::isResult_ (false);

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return base_type::error_ ();
      }


      /**
       * @brief
       * @tparam TArgs
       * @param args
       */
      template <
        typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, TArgs && ...>> ...
      >
      error_type &
      emplace (ErrorTag, TArgs && ... args)
      {
        static_assert (std::is_constructible_v <error_type, TArgs && ...>);

        if (base_type::isResult_ ())
        {
          base_type::result_or_error_.template destroy <result_type> ();
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
        }

        base_type::result_or_error_.template construct <error_type> (std::forward <TArgs> (args) ...);

        base_type::isResult_ (false);

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return base_type::error_ ();
      }


      /**
       * @brief
       */
      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <result_type>, TDummy> ...>
      result_type &
      reset (ResultTag)
      {
        static_assert (std::is_default_constructible_v <result_type>);

        if (base_type::isResult_ ())
        {
          base_type::result_or_error_.template destroy <result_type> ();
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
        }

        base_type::isResult_ (true);

        base_type::result_or_error_.template construct <result_type> ();

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return base_type::result_ ();
      }


      /**
       * @brief
       */
      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <error_type>, TDummy> ...>
      error_type &
      reset (ErrorTag)
      {
        static_assert (std::is_default_constructible_v <error_type>);

        if (base_type::isResult_ ())
        {
          base_type::result_or_error_.template destroy <result_type> ();
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
        }

        base_type::isResult_ (false);

        base_type::result_or_error_.template construct <error_type> ();

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return base_type::error_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] explicit operator bool (void) const noexcept
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return base_type::isResult_ ();
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, const TThatResult &>
          && std::is_constructible_v <error_type, const TThatError &>
          && std::is_assignable_v <result_type &, const TThatResult &>
          && std::is_assignable_v <error_type &, const TThatError &>
          && !std::is_same_v <self_type, Expected <TThatResult, TThatError>>
        > ...
      >
      self_type &
      operator = (const Expected <TThatResult, TThatError> & that)
      {
        using that_result_type = TThatResult;
        using that_error_type = TThatError;


        static_assert (
             std::is_constructible_v <result_type, const that_result_type &>
          && std::is_constructible_v <error_type, const that_error_type &>
          && std::is_assignable_v <result_type &, const that_result_type &>
          && std::is_assignable_v <error_type &, const that_error_type &>
          && !std::is_same_v <self_type, Expected <that_result_type, that_error_type>>
        );

        if (base_type::isResult_ ())
        {
          if (that.isResult_ ())
          {
            base_type::result_or_error_.template assign <result_type> (that.result_ ());
          }
          else
          {
            base_type::result_or_error_.template destroy <result_type> ();
            base_type::result_or_error_.template construct <error_type> (that.error_ ());
          }
        }
        else
        {
          if (that.isResult_ ())
          {
            base_type::result_or_error_.template destroy <error_type> ();
            base_type::result_or_error_.template construct <result_type> (that.result_ ());
          }
          else
          {
            base_type::result_or_error_.template assign <error_type> (that.error_ ());
          }
        }

        base_type::isResult_ (that.isResult_ ());

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        that.wasChecked_ (true);
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return *this;
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && std::is_constructible_v <error_type, TThatError &&>
          && std::is_assignable_v <result_type &, TThatResult &&>
          && std::is_assignable_v <error_type &, TThatError &&>
          && !std::is_same_v <self_type, Expected <TThatResult, TThatError>>
        > ...
      >
      self_type &
      operator = (Expected <TThatResult, TThatError> && that)
      {
        using that_result_type = TThatResult;
        using that_error_type = TThatError;


        static_assert (
             std::is_constructible_v <result_type, that_result_type &&>
          && std::is_constructible_v <error_type, that_error_type &&>
          && std::is_assignable_v <result_type &, that_result_type &&>
          && std::is_assignable_v <error_type &, that_error_type &&>
          && !std::is_same_v <self_type, Expected <that_result_type, that_error_type>>
        );

        if (base_type::isResult_ ())
        {
          if (that.isResult_ ())
          {
            base_type::result_or_error_.template assign <result_type> (std::move (that.result_ ()));
          }
          else
          {
            base_type::result_or_error_.template destroy <result_type> ();
            base_type::result_or_error_.template construct <error_type> (std::move (that.error_ ()));
          }
        }
        else
        {
          if (that.isResult_ ())
          {
            base_type::result_or_error_.template destroy <error_type> ();
            base_type::result_or_error_.template construct <result_type> (std::move (that.result_ ()));
          }
          else
          {
            base_type::result_or_error_.template assign <error_type> (std::move (that.error_ ()));
          }
        }

        base_type::isResult_ (that.isResult_ ());

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        that.wasChecked_ (true);
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return *this;
      }


      /**
       * @brief
       * @tparam TThatResult
       * @param that_result
       * @return
       */
      template <
        typename TThatResult,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && std::is_assignable_v <result_type &, TThatResult &&>
          && !std::is_same_v <self_type, RemoveCVRefT <TThatResult>>
        > ...
      >
      self_type &
      operator = (TThatResult && that_result)
      {
        using that_result_type = TThatResult;


        static_assert (
             std::is_constructible_v <result_type, that_result_type &&>
          && std::is_assignable_v <result_type &, that_result_type &&>
          && !std::is_same_v <self_type, RemoveCVRefT <that_result_type>>
        );

        if (base_type::isResult_ ())
        {
          base_type::result_or_error_.template assign <result_type> (std::forward <that_result_type> (that_result));
        }
        else
        {
          base_type::result_or_error_.template destroy <error_type> ();
          base_type::result_or_error_.template construct <result_type> (std::forward <that_result_type> (that_result));
        }

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (false);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return *this;
      }


      /**
       * @brief Earliest error or latest result
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <Expected <TThatResult, TThatError>, const self_type &>
          || std::is_convertible_v <const self_type &, Expected <TThatResult, TThatError>>
        > ...
      >
      Expected <TThatResult, TThatError>
      operator & (const Expected <TThatResult, TThatError> & that) const &
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible_v <that_type, const self_type &>
          || std::is_convertible_v <const self_type &, that_type>
        );

        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that;
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that_type (*this);
        }
      }


      /**
       * @brief Earliest error or latest result
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <Expected <TThatResult, TThatError>, self_type &&>
          || std::is_convertible_v <self_type &&, Expected <TThatResult, TThatError>>
        > ...
      >
      Expected <TThatResult, TThatError>
      operator & (const Expected <TThatResult, TThatError> & that) &&
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible_v <that_type, self_type &&>
          || std::is_convertible_v <self_type &&, that_type>
        );

        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that;
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that_type (std::move (*this));
        }
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <Expected <TThatResult, TThatError>, const self_type &>
          || std::is_convertible_v <const self_type &, Expected <TThatResult, TThatError>>
        > ...
      >
      Expected <TThatResult, TThatError>
      operator | (const Expected <TThatResult, TThatError> & that) const &
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible_v <that_type, const self_type &>
          || std::is_convertible_v <const self_type &, that_type>
        );

        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that_type (*this);
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that;
        }
      }


      /**
       * @brief Latest error or earliest result
       * @tparam TThatResult
       * @tparam TThatError
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <Expected <TThatResult, TThatError>, self_type &&>
          || std::is_convertible_v <self_type &&, Expected <TThatResult, TThatError>>
        > ...
      >
      Expected <TThatResult, TThatError>
      operator | (const Expected <TThatResult, TThatError> & that) &&
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible_v <that_type, self_type &&>
          || std::is_convertible_v <self_type &&, that_type>
        );

        if (base_type::isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that_type (std::move (*this));
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that;
        }
      }


      /**
       * @brief
       * @param that
       * @return
       */
      bool
      operator == (const self_type & that) const
      {
        if (this == &that)
        {
          return true;
        }
        else
        {
          if (base_type::isResult_ ())
          {
            if (that.isResult_ ())
            {
              return (base_type::result_ () == that.result_ ());
            }
            else
            {
              return false;
            }
          }
          else
          {
            if (that.isResult_ ())
            {
              return false;
            }
            else
            {
              return (base_type::error_ () == that.error_ ());
            }
          }
        }
      }


      /**
       * @brief
       * @param that
       * @return
       */
      bool
      operator != (const self_type & that) const
      {
        return !operator == (that);
      }


      /**
       * @brief
       * @tparam TFallback
       * @param fallback
       * @return
       */
      template <
        typename TFallback,
        std::enable_if_t <
             std::is_constructible_v <TFallback, const result_type &>
          || std::is_convertible_v <const result_type &, TFallback &&>
        > ...
      >
      TFallback
      operator || (TFallback && fallback) const &
      {
        using fallback_type = TFallback;


        static_assert (
             std::is_constructible_v <fallback_type, const result_type &>
          || std::is_convertible_v <const result_type &, fallback_type &&>
        );

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        if (base_type::isResult_ ())
        {
          return fallback_type (base_type::result_ ());
        }
        else
        {
          return std::forward <fallback_type> (fallback);
        }
      }


      /**
       * @brief
       * @tparam TFallback
       * @param fallback
       * @return
       */
      template <
        typename TFallback,
        std::enable_if_t <
             std::is_constructible_v <TFallback, result_type &&>
          || std::is_convertible_v <result_type &&, TFallback &&>
        > ...
      >
      TFallback
      operator || (TFallback && fallback) &&
      {
        using fallback_type = TFallback;


        static_assert (
             std::is_constructible_v <fallback_type, result_type &&>
          || std::is_convertible_v <result_type &&, fallback_type &&>
        );

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        base_type::wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        if (base_type::isResult_ ())
        {
          return fallback_type (std::move (base_type::result_ ()));
        }
        else
        {
          return std::forward <fallback_type> (fallback);
        }
      }


      /**
       * @brief
       * @return
       */
      const result_type &
      operator * (void) const &
      {
        return base_type::result_Safe_ ();
      }


      /**
       * @brief
       * @return
       */
      result_type &
      operator * (void) &
      {
        return base_type::result_Safe_ ();
      }


      /**
       * @brief
       * @return
       */
      const result_type &&
      operator * (void) const &&
      {
        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      result_type &&
      operator * (void) &&
      {
        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      const result_type *
      operator -> (void) const
      {
        return std::addressof (operator * ());
      }


      /**
       * @brief
       * @return
       */
      result_type *
      operator -> (void)
      {
        return std::addressof (operator * ());
      }


      /**
       * @brief
       * @param output
       * @param self
       * @return
       */
      friend std::ostream &
      operator << (std::ostream & output, const self_type & self)
      {
        output << "Expected{";
        if (self.isResult_ ())
        {
          output << "result:" << self.result_ ();
        }
        else
        {
          output << "error:" << self.error_ ();
        }
        output << '}';

        return output;
      }
  };
}


#endif // UTILS_MISC_EXPECTED_HXX
