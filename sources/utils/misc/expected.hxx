#ifndef UTILS_MISC_EXPECTED_HXX
#define UTILS_MISC_EXPECTED_HXX


#include <ostream> // std::ostream
#include <memory> // std::addressof
#include <type_traits> // std::{enable_if, is_assignable, is_convertible, is_constructible}
#include <utility> // std::{forward, move}

#include "../debug/fatal.hxx" // FATAL_NL
#include "../type-traits/aligned-union-storage-manager.hxx" // AlignedUnionStorageManager
#include "operation-status.hxx" // OperationStatus


namespace Utils
{
  /**
    * @brief
    */
  struct ResultTag
  { };


  /**
   * @brief
   */
  constexpr ResultTag Result { };


  /**
   * @brief
   */
  struct ErrorTag
  { };


  /**
   * @brief
   */
  constexpr ErrorTag Error { };


  /**
   * @brief
   * @tparam TResult
   * @tparam TError
   */
  template <typename TResult, typename TError = OperationStatus>
  class [[nodiscard]] Expected final
  {
    static_assert (
         !(std::is_void <TResult>::value || std::is_void <TError>::value)
      && !(std::is_reference <TResult>::value || std::is_reference <TError>::value)
    );


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


      template <typename TResult2, typename TError2>
      friend class Expected;


      /**
       * @brief
       */
      Expected (void) noexcept = delete;


      /**
       * @brief
       * @tparam TArgs
       * @param args
       */
      template <typename ... TArgs>
      Expected (ResultTag, TArgs && ... args) :
        is_result_ (true)
      {
        static_assert (std::is_constructible <result_type, TArgs ...>::value);

        result_or_error_.template construct <result_type> (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TArgs
       * @param args
       */
      template <typename ... TArgs>
      Expected (ErrorTag, TArgs && ... args) :
        is_result_ (false)
      {
        static_assert (std::is_constructible <error_type, TArgs ...>::value);

        result_or_error_.template construct <error_type> (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @param that
       */
      template <typename TThatResult, typename TThatError>
      Expected (const Expected <TThatResult, TThatError> & that) :
        is_result_ (that.is_result_)
      {
        static_assert (
             std::is_constructible <result_type, typename Expected <TThatResult, TThatError>::result_type>::value
          && std::is_constructible <error_type, typename Expected <TThatResult, TThatError>::error_type>::value
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
      template <typename TThatResult, typename TThatError>
      Expected (Expected <TThatResult, TThatError> && that) :
        is_result_ (that.is_result_)
      {
        static_assert (
             std::is_constructible <result_type, typename Expected <TThatResult, TThatError>::result_type>::value
          && std::is_constructible <error_type, typename Expected <TThatResult, TThatError>::error_type>::value
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
       */
      Expected (const self_type & that) :
        is_result_ (that.is_result_)
      {
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
      Expected (self_type && that) :
        is_result_ (that.is_result_)
      {
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
       */
      ~Expected (void)
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        if (!wasChecked_ ())
        {
          FATAL_NL ("No instance of `Expected' should be left unchecked");
        }
#endif // EXPECTED_WITH_RUNTIME_CHECKS

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
        if (isResult_ ())
#pragma GCC diagnostic pop
        {
          result_or_error_.template destroy <result_type> ();
        }
        else
        {
          result_or_error_.template destroy <error_type> ();
        }
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      error (void) const
      {
        return error_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] explicit operator bool (void) const noexcept
      {
        return isResult_Checked_ ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (const self_type & that)
      {
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
      const self_type &
      operator = (self_type && that)
      {
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


      /**
       * @brief
       * @param that
       * @return
       */
      template <typename TThatResult, typename TThatError>
      const self_type &
      operator = (const Expected <TThatResult, TThatError> & that)
      {
        static_assert (
             std::is_constructible <result_type, typename Expected <TThatResult, TThatError>::result_type>::value
          && std::is_constructible <error_type, typename Expected <TThatResult, TThatError>::error_type>::value
          && std::is_assignable <result_type &, typename Expected <TThatResult, TThatError>::result_type>::value
          && std::is_assignable <error_type &, typename Expected <TThatResult, TThatError>::error_type>::value
        );

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

        return *this;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      template <typename TThatResult, typename TThatError>
      const self_type &
      operator = (Expected <TThatResult, TThatError> && that)
      {
        static_assert (
             std::is_constructible <result_type, typename Expected <TThatResult, TThatError>::result_type>::value
          && std::is_constructible <error_type, typename Expected <TThatResult, TThatError>::error_type>::value
          && std::is_assignable <result_type &, typename Expected <TThatResult, TThatError>::result_type>::value
          && std::is_assignable <error_type &, typename Expected <TThatResult, TThatError>::error_type>::value
        );


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

        return *this;
      }


      /**
       * @brief
       * @param that
       * @return Earliest error or latest result
       */
      template <typename TThatResult, typename TThatError>
      Expected <TThatResult, TThatError>
      operator & (const Expected <TThatResult, TThatError> & that) const &
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible <that_type, self_type>::value
          || std::is_convertible <self_type, that_type>::value
        );

        if (isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
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
       * @brief
       * @param that
       * @return Earliest error or latest result
       */
      template <typename TThatResult, typename TThatError>
      Expected <TThatResult, TThatError>
      operator & (const Expected <TThatResult, TThatError> & that) &&
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible <that_type, self_type>::value
          || std::is_convertible <self_type, that_type>::value
        );
        if (isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
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
       * @param that
       * @return Latest error or earliest result
       */
      template <typename TThatResult, typename TThatError>
      Expected <TThatResult, TThatError>
      operator | (const Expected <TThatResult, TThatError> & that) const &
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible <that_type, self_type>::value
          || std::is_convertible <self_type, that_type>::value
        );

        if (isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that_type (*this);
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that;
        }
      }


      /**
       * @brief
       * @param that
       * @return Latest error or earliest result
       */
      template <typename TThatResult, typename TThatError>
      Expected <TThatResult, TThatError>
      operator | (const Expected <TThatResult, TThatError> & that) &&
      {
        using that_type = Expected <TThatResult, TThatError>;


        static_assert (
             std::is_constructible <that_type, self_type>::value
          || std::is_convertible <self_type, that_type>::value
        );

        if (isResult_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return that_type (std::move (*this));
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
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
      operator == (const self_type & that [[maybe_unused]]) const = delete;

      /**
       * @brief
       * @param that
       * @return
       */
      bool
      operator != (const self_type & that [[maybe_unused]]) const = delete;



      /**
       * @brief
       * @param fallback
       * @return
       */
      template <typename TFallback>
      TFallback
      operator || (TFallback && fallback) const &
      {
        static_assert (
             std::is_constructible <TFallback, result_type>::value
          || std::is_convertible <result_type, TFallback>::value
        );

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        if (isResult_ ())
        {
          return TFallback (result_ ());
        }
        else
        {
          return std::forward <TFallback> (fallback);
        }
      }


      /**
       * @brief
       * @param fallback
       * @return
       */
      template <typename TFallback>
      TFallback
      operator || (TFallback && fallback) &&
      {
        static_assert (
             std::is_constructible <TFallback, result_type>::value
          || std::is_convertible <result_type, TFallback>::value
        );

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        if (isResult_ ())
        {
          return TFallback (std::move (result_ ()));
        }
        else
        {
          return std::forward <TFallback> (fallback);
        }
      }


      /**
       * @brief
       * @return
       */
      const result_type &
      operator * (void) const
      {
        return result_Checked_ ();
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


    private:
      /**
       * @brief
       * @return
       */
      const result_type &
      result_ (void) const
      {
        return result_or_error_.template get <result_type> ();
      }


      /**
       * @brief
       * @return
       */
      result_type &
      result_ (void)
      {
        return result_or_error_.template get <result_type> ();
      }


      /**
       * @brief
       * @return
       */
      const result_type &
      result_Checked_ (void) const
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        if (wasChecked_ ())
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
        else
        {
          FATAL_NL ("`Expected' should be checked for being an error before accessing result");
        }
#else // EXPECTED_WITH_RUNTIME_CHECKS
        if (isResult_ ())
        {
          return result_ ();
        }
        else
        {
          FATAL_NL ("Cannot access result because this instance of `Expected' was initialized as an error");
        }
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      error_ (void) const
      {
        return result_or_error_.template get <error_type> ();
      }


      /**
       * @brief
       * @return
       */
      error_type &
      error_ (void)
      {
        return result_or_error_.template get <error_type> ();
      }


      const error_type &
      error_Checked_ (void) const
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        if (wasChecked_ ())
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
        else
        {
          FATAL_NL ("`Expected' should be checked for being an error before accessing error");
        }
#else // EXPECTED_WITH_RUNTIME_CHECKS
        if (isResult_ ())
        {
          FATAL_NL ("Cannot access error because this instance of `Expected' was initialized as a result");
        }
        else
        {
          return error_ ();
        }
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
       * @return
       */
      [[nodiscard]] bool
      isResult_Checked_ (void) const noexcept
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return isResult_ ();
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


#endif // UTILS_MISC_EXPECTED_HXX
