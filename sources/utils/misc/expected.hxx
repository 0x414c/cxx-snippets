#ifndef UTILS_MISC_EXPECTED_HXX
#define UTILS_MISC_EXPECTED_HXX


#include <initializer_list>  // std::initializer_list
#include <memory>  // std::addressof
#include <ostream>  // std::ostream
#include <type_traits>  // std::*
#include <utility>  // std::{forward, move}

#include "../containers/c-string.hxx"  // CString
#include "../debug/fatal.hxx"  // FATAL_NL
#include "../meta/enable-copy-move.hxx"  // EnableCopyMove
#include "../meta/remove-c-v-ref.hxx"  // RemoveCVRefT


namespace Utils
{
  /**
   * @brief
   */
  struct ResultTag
  {
    /**
     * @brief
     */
    explicit constexpr ResultTag () noexcept = default;
  };


  /**
   * @brief
   */
  inline constexpr ResultTag Result;


  /**
   * @brief
   */
  struct ErrorTag
  {
    /**
     * @brief
     */
    explicit constexpr ErrorTag () noexcept = default;
  };


  /**
   * @brief
   */
  inline constexpr ErrorTag Error;


  /**
   * @brief
   */
  class UncheckedPolicy
  {
    private:
      /**
       * @brief
       */
      using self_type = UncheckedPolicy;


    public:
      /**
       * @brief
       */
      constexpr UncheckedPolicy () noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr UncheckedPolicy (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr UncheckedPolicy (self_type && that [[maybe_unused]]) noexcept = default;


    protected:
      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr bool
      wasChecked_ () const noexcept
      {
        return true;
      }


      /**
       * @brief
       * @param was_checked
       * @return
       */
      constexpr void
      wasChecked_ (bool was_checked [[maybe_unused]]) const noexcept
      { }


    public:
      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = default;
  };


  /**
   * @brief
   */
  class CheckedNoDiscardPolicy
  {
    private:
      /**
       * @brief
       */
      using self_type = CheckedNoDiscardPolicy;


    public:
      /**
       * @brief
       */
      CheckedNoDiscardPolicy () noexcept = default;


      /**
       * @brief
       * @param that
       */
      CheckedNoDiscardPolicy (const self_type & that) noexcept
      {
        that.wasChecked_ (true);
      }


      /**
       * @brief
       * @param that
       */
      CheckedNoDiscardPolicy (self_type && that) noexcept
      {
        that.wasChecked_ (true);
      }


      /**
       * @brief
       */
      ~ CheckedNoDiscardPolicy () noexcept
      {
        if (! wasChecked_ ())
        {
          FATAL_NL ("No instance of  `Expected'  must be left unchecked");
        }
      }


    protected:
      /**
       * @brief
       * @return
       */
      [[nodiscard]] bool
      wasChecked_ () const noexcept
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


    public:
      /**
       * @brief
       * @param that
       * @return
       */
      self_type &
      operator = (const self_type & that) noexcept
      {
        if (this != & that)
        {
          wasChecked_ (false);
          that.wasChecked_ (true);
        }

        return * this;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      self_type &
      operator = (self_type && that) noexcept
      {
        if (this != & that)
        {
          wasChecked_ (false);
          that.wasChecked_ (true);
        }

        return * this;
      }


    private:
      /**
       * @brief
       */
      mutable bool was_checked_ { };
  };


  namespace ExpectedInternals_
  {
    /**
     * @brief
     * @tparam TType
     */
    template <typename TType>
    inline constexpr bool IsAllowedV_ (
         std::is_object_v <TType>
      && ! std::is_array_v <TType>
      && std::is_destructible_v <TType>
      && ! std::is_same_v <RemoveCVRefT <TType>, ResultTag>
      && ! std::is_same_v <RemoveCVRefT <TType>, ErrorTag>
    );


    /**
     * @brief
     */
    struct Dummy_ final
    {
      private:
        /**
         * @brief
         */
        using self_type = Dummy_;


      public:
        /**
         * @brief
         */
        constexpr Dummy_ () noexcept = default;

        /**
         * @brief
         */
        constexpr Dummy_ (const self_type &) noexcept = delete;

        /**
         * @brief
         */
        constexpr Dummy_ (self_type &&) noexcept = delete;

        /**
         * @brief
         * @return
         */
        constexpr Dummy_ &
        operator = (const self_type &) noexcept = delete;

        /**
         * @brief
         * @return
         */
        constexpr Dummy_ &
        operator = (self_type &&) noexcept = delete;
    };


    /**
     * @brief
     * @tparam TResult
     * @tparam TError
     * @tparam TIsTriviallyDestructible
     */
    template <
      typename TResult, typename TError,
      bool TIsTriviallyDestructible =
           std::is_trivially_destructible_v <TResult>
        && std::is_trivially_destructible_v <TError>
    >
    union ResultOrError_ final
    {
      static_assert (
           IsAllowedV_ <TResult>
        && IsAllowedV_ <TError>
      );


      public:
        /**
         * @brief
         */
        using result_type = std::remove_const_t <TResult>;

        /**
         * @brief
         */
        using error_type = std::remove_const_t <TError>;


      private:
        /**
         * @brief
         */
        using self_type = ResultOrError_;


      public:
        /**
         * @brief
         */
        constexpr ResultOrError_ () noexcept
        { }


        /**
         * @brief
         * @param that
         */
        constexpr ResultOrError_ (const self_type & that [[maybe_unused]]) noexcept = delete;

        /**
         * @brief
         * @param that
         */
        constexpr ResultOrError_ (self_type && that [[maybe_unused]]) noexcept = delete;


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ResultOrError_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          result (std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ResultOrError_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          error (std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (const self_type & that [[maybe_unused]]) noexcept = delete;

        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (self_type && that [[maybe_unused]]) noexcept = delete;


      private:
        /**
         * @brief
         */
        Dummy_ dummy_;


      public:
        /**
         * @brief
         */
        result_type result;

        /**
         * @brief
         */
        error_type error;
    };


    /**
     * @brief
     * @tparam TResult
     * @tparam TError
     */
    template <typename TResult, typename TError>
    union ResultOrError_ <TResult, TError, false>
    {
      static_assert (
           IsAllowedV_ <TResult>
        && IsAllowedV_ <TError>
      );


      public:
        /**
         * @brief
         */
        using result_type = std::remove_const_t <TResult>;

        /**
         * @brief
         */
        using error_type = std::remove_const_t <TError>;


      private:
        /**
         * @brief
         */
        using self_type = ResultOrError_;


      public:
        /**
         * @brief
         */
        ResultOrError_ () noexcept
        { };


        /**
         * @brief
         * @param that
         */
        ResultOrError_ (const self_type & that [[maybe_unused]]) noexcept = delete;

        /**
         * @brief
         * @param that
         */
        ResultOrError_ (self_type && that [[maybe_unused]]) noexcept = delete;


        /**
         * @brief
         */
        ~ ResultOrError_ () noexcept
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        ResultOrError_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          result (std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        ResultOrError_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          error (std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (const self_type & that [[maybe_unused]]) noexcept = delete;

        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (self_type && that [[maybe_unused]]) noexcept = delete;


      private:
        /**
         * @brief
         */
        Dummy_ dummy_;


      public:
        /**
         * @brief
         */
        result_type result;

        /**
         * @brief
         */
        error_type error;
    };


    /**
     * @brief
     * @tparam TResult
     * @tparam TError
     * @tparam TDerived
     */
    template <typename TResult, typename TError, typename TDerived>
    class ExpectedStorageBase_
    {
      static_assert (
           IsAllowedV_ <TResult>
        && IsAllowedV_ <TError>
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
        using derived_type = TDerived;


      private:
        /**
         * @brief
         */
        using self_type = ExpectedStorageBase_;


      public:
        /**
         * @brief
         */
        constexpr ExpectedStorageBase_ () noexcept = default;


        /**
         * @brief
         * @param that
         */
        constexpr ExpectedStorageBase_ (const self_type & that)
        noexcept (
             std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            derived_ ()->construct_ (Result, that.get_ (Result));
          }
          else
          {
            derived_ ()->construct_ (Error, that.get_ (Error));
          }
        }


        /**
         * @brief
         * @param that
         */
        constexpr ExpectedStorageBase_ (self_type && that)
        noexcept (
             std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            derived_ ()->construct_ (Result, std::move (that.get_ (Result)));
          }
          else
          {
            derived_ ()->construct_ (Error, std::move (that.get_ (Error)));
          }
        }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ExpectedStorageBase_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          is_result_ (true),
          result_or_error_ (Result, std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ExpectedStorageBase_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          is_result_ (false),
          result_or_error_ (Error, std::forward <TArgs> (args) ...)
        { }


      protected:
        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr bool
        isResult_ () const noexcept
        {
          return is_result_;
        }


        /**
         * @brief
         * @param is_result
         * @return
         */
        constexpr void
        isResult_ (bool is_result) noexcept
        {
          is_result_ = is_result;
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const result_type &
        get_ (ResultTag) const noexcept
        {
          return result_or_error_.result;
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr result_type &
        get_ (ResultTag) noexcept
        {
          return result_or_error_.result;
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const error_type &
        get_ (ErrorTag) const noexcept
        {
          return result_or_error_.error;
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr error_type &
        get_ (ErrorTag) noexcept
        {
          return result_or_error_.error;
        }


        /**
         * @brief
         * @tparam TThatResult
         * @param that_result
         * @return
         */
        template <typename TThatResult>
        constexpr void
        construct_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_constructible_v <result_type, TThatResult &&>)
        {
          get_ (Result) = std::forward <TThatResult> (that_result);
        }


        /**
         * @brief
         * @tparam TThatError
         * @param that_error
         * @return
         */
        template <typename TThatError>
        constexpr void
        construct_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>)
        {
          get_ (Error) = std::forward <TThatError> (that_error);
        }


        /**
         * @brief
         * @tparam TThatResult
         * @param that_result
         * @return
         */
        template <typename TThatResult>
        constexpr void
        assign_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_assignable_v <result_type &, TThatResult &&>)
        {
          get_ (Result) = std::forward <TThatResult> (that_result);
        }


        /**
         * @brief
         * @tparam TThatError
         * @param that_error
         * @return
         */
        template <typename TThatError>
        constexpr void
        assign_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_assignable_v <error_type &, TThatError &&>)
        {
          get_ (Error) = std::forward <TThatError> (that_error);
        }


        /**
         * @brief
         * @return
         */
        constexpr void
        destroy_ (ResultTag) noexcept
        { }


        /**
         * @brief
         * @return
         */
        constexpr void
        destroy_ (ErrorTag) noexcept
        { }


        /**
         * @brief
         * @return
         */
        constexpr void
        clear_ () noexcept
        { }


      public:
        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (const self_type & that) noexcept (
             std::is_nothrow_copy_assignable_v <result_type>
          && std::is_nothrow_destructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_assignable_v <error_type>
          && std::is_nothrow_destructible_v <error_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        )
        {
          if (this != & that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                derived_ ()->assign_ (Result, that.get_ (Result));
              }
              else
              {
                derived_ ()->destroy_ (Result);
                derived_ ()->construct_ (Error, that.get_ (Error));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                derived_ ()->destroy_ (Error);
                derived_ ()->construct_ (Result, that.get_ (Result));
              }
              else
              {
                derived_ ()->assign_ (Error, that.get_ (Error));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return * this;
        }


        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (self_type && that) noexcept (
             std::is_nothrow_move_assignable_v <result_type>
          && std::is_nothrow_destructible_v <result_type>
          && std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_assignable_v <error_type>
          && std::is_nothrow_destructible_v <error_type>
          && std::is_nothrow_move_constructible_v <error_type>
        )
        {
          if (this != & that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                derived_ ()->assign_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                derived_ ()->destroy_ (Result);
                derived_ ()->construct_ (Error, std::move (that.get_ (Error)));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                derived_ ()->destroy_ (Error);
                derived_ ()->construct_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                derived_ ()->assign_ (Error, std::move (that.get_ (Error)));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return * this;
        }


      private:
        /**
         * @brief
         */
        bool is_result_;

        /**
         * @brief
         */
        ResultOrError_ <result_type, error_type> result_or_error_;


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const derived_type *
        derived_ () const noexcept
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
          return (const derived_type *) this;
#pragma GCC diagnostic pop
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr derived_type *
        derived_ () noexcept
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
          return (derived_type *) this;
#pragma GCC diagnostic pop
        }
    };


    /**
     * @brief
     * @tparam TResult
     * @tparam TError
     * @tparam TIsTriviallyAssignable
     * @tparam TIsTriviallyDestructible
     */
    template <
      typename TResult, typename TError,
      bool TIsTriviallyAssignable =
           std::is_trivially_copy_assignable_v <TResult>
        && std::is_trivially_move_assignable_v <TResult>
        && std::is_trivially_copy_assignable_v <TError>
        && std::is_trivially_move_assignable_v <TError>,
      bool TIsTriviallyDestructible =
           std::is_trivially_destructible_v <TResult>
        && std::is_trivially_destructible_v <TError>
    >
    class ExpectedStorage_ :
      private ExpectedStorageBase_ <
        TResult, TError, ExpectedStorage_ <TResult, TError, TIsTriviallyAssignable, TIsTriviallyDestructible>
      >
    {
      static_assert (
           IsAllowedV_ <TResult>
        && IsAllowedV_ <TError>
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


      private:
        /**
         * @brief
         */
        using self_type = ExpectedStorage_;

        /**
         * @brief
         */
        using base_type = ExpectedStorageBase_ <result_type, error_type, self_type>;

        /**
         * @brief
         */
        friend base_type;


      public:
        /**
         * @brief
         */
        constexpr ExpectedStorage_ () noexcept = default;

        /**
         * @brief
         * @param that
         */
        constexpr ExpectedStorage_ (const self_type & that [[maybe_unused]])
        noexcept (
             std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        )
        = default;

        /**
         * @brief
         * @param that
         */
        constexpr ExpectedStorage_ (self_type && that [[maybe_unused]])
        noexcept (
             std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_constructible_v <error_type>
        )
        = default;


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ExpectedStorage_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          base_type (Result, std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ExpectedStorage_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          base_type (Error, std::forward <TArgs> (args) ...)
        { }


      protected:
        /**
         * @brief
         */
        using base_type::isResult_;

        /**
         * @brief
         */
        using base_type::get_;

        /**
         * @brief
         */
        using base_type::construct_;

        /**
         * @brief
         */
        using base_type::assign_;

        /**
         * @brief
         */
        using base_type::destroy_;

        /**
         * @brief
         */
        using base_type::clear_;


      public:
        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (const self_type & that)
        noexcept (
             std::is_nothrow_copy_assignable_v <result_type>
          && std::is_nothrow_copy_assignable_v <error_type>
        )
        = default;

        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (self_type && that)
        noexcept (
             std::is_nothrow_move_assignable_v <result_type>
          && std::is_nothrow_move_assignable_v <error_type>
        )
        = default;
    };


    /**
     * @brief
     * @tparam TResult
     * @tparam TError
     */
    template <typename TResult, typename TError>
    class ExpectedStorage_ <TResult, TError, false, true> :
      private ExpectedStorageBase_ <TResult, TError, ExpectedStorage_ <TResult, TError, false, true>>
    {
      static_assert (
           IsAllowedV_ <TResult>
        && IsAllowedV_ <TError>
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


      private:
        /**
         * @brief
         */
        using self_type = ExpectedStorage_;

        /**
         * @brief
         */
        using base_type = ExpectedStorageBase_ <result_type, error_type, self_type>;

        /**
         * @brief
         */
        friend base_type;


      public:
        /**
         * @brief
         */
        ExpectedStorage_ () noexcept = default;

        /**
         * @brief
         * @param that
         */
        ExpectedStorage_ (const self_type & that [[maybe_unused]])
        noexcept (
             std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        )
        = default;

        /**
         * @brief
         * @param that
         */
        ExpectedStorage_ (self_type && that [[maybe_unused]])
        noexcept (
             std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_constructible_v <error_type>
        )
        = default;


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        ExpectedStorage_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          base_type (Result, std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        ExpectedStorage_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          base_type (Error, std::forward <TArgs> (args) ...)
        { }


      protected:
        /**
         * @brief
         */
        using base_type::isResult_;

        /**
         * @brief
         */
        using base_type::get_;

        /**
         * @brief
         */
        using base_type::assign_;

        /**
         * @brief
         */
        using base_type::destroy_;

        /**
         * @brief
         */
        using base_type::clear_;


        /**
         * @brief
         * @tparam TThatResult
         * @param that_result
         */
        template <typename TThatResult>
        void
        construct_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_constructible_v <result_type, TThatResult &&>)
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
          ::new ((void *) std::addressof (get_ (Result))) (result_type) (std::forward <TThatResult> (that_result));
#pragma GCC diagnostic pop
        }


        /**
         * @brief
         * @tparam TThatError
         * @param that_error
         */
        template <typename TThatError>
        void
        construct_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>)
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
          ::new ((void *) std::addressof (get_ (Error))) (error_type) (std::forward <TThatError> (that_error));
#pragma GCC diagnostic pop
        }


      public:
        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (const self_type & that)
        noexcept (
             std::is_nothrow_copy_assignable_v <result_type>
          && std::is_nothrow_copy_assignable_v <error_type>
        )
        = default;

        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (self_type && that)
        noexcept (
             std::is_nothrow_move_assignable_v <result_type>
          && std::is_nothrow_move_assignable_v <error_type>
        )
        = default;
    };


    /**
     * @brief
     * @tparam TResult
     * @tparam TError
     */
    template <typename TResult, typename TError>
    class ExpectedStorage_ <TResult, TError, false, false> :
      private ExpectedStorageBase_ <TResult, TError, ExpectedStorage_ <TResult, TError, false, false>>
    {
      static_assert (
           IsAllowedV_ <TResult>
        && IsAllowedV_ <TError>
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


      private:
        /**
         * @brief
         */
        using self_type = ExpectedStorage_;

        /**
         * @brief
         */
        using base_type = ExpectedStorageBase_ <result_type, error_type, self_type>;

        /**
         * @brief
         */
        friend base_type;


      public:
        /**
         * @brief
         */
        ExpectedStorage_ () noexcept = default;

        /**
         * @brief
         * @param that
         */
        ExpectedStorage_ (const self_type & that [[maybe_unused]])
        noexcept (
             std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        )
        = default;

        /**
         * @brief
         * @param that
         */
        ExpectedStorage_ (self_type && that [[maybe_unused]])
        noexcept (
             std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_constructible_v <error_type>
        )
        = default;


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        ExpectedStorage_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          base_type (Result, std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        ExpectedStorage_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          base_type (Error, std::forward <TArgs> (args) ...)
        { }


        /**
         * @brief
         */
        ~ ExpectedStorage_ ()
        noexcept (
             std::is_nothrow_destructible_v <result_type>
          && std::is_nothrow_destructible_v <error_type>
        )
        {
          clear_ ();
        }


      protected:
        /**
         * @brief
         */
        using base_type::isResult_;

        /**
         * @brief
         */
        using base_type::get_;

        /**
         * @brief
         */
        using base_type::assign_;


        /**
         * @brief
         * @tparam TThatResult
         * @param that_result
         */
        template <typename TThatResult>
        void
        construct_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_constructible_v <result_type, TThatResult &&>)
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
          ::new ((void *) std::addressof (get_ (Result))) (result_type) (std::forward <TThatResult> (that_result));
#pragma GCC diagnostic pop
        }


        /**
         * @brief
         * @tparam TThatError
         * @param that_error
         */
        template <typename TThatError>
        void
        construct_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>)
        {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
          ::new ((void *) std::addressof (get_ (Error))) (error_type) (std::forward <TThatError> (that_error));
#pragma GCC diagnostic pop
        }


        /**
         * @brief
         */
        void
        destroy_ (ResultTag)
        noexcept (std::is_nothrow_destructible_v <result_type>)
        {
          get_ (Result).~ result_type ();
        }


        /**
         * @brief
         */
        void
        destroy_ (ErrorTag)
        noexcept (std::is_nothrow_destructible_v <error_type>)
        {
          get_ (Error).~ error_type ();
        }


        /**
         * @brief
         */
        void
        clear_ ()
        noexcept (
             std::is_nothrow_destructible_v <result_type>
          && std::is_nothrow_destructible_v <error_type>
        )
        {
          if (isResult_ ())
          {
            destroy_ (Result);
          }
          else
          {
            destroy_ (Error);
          }
        }


      public:
        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (const self_type & that)
        noexcept (
             std::is_nothrow_copy_assignable_v <result_type>
          && std::is_nothrow_copy_assignable_v <error_type>
        )
        = default;

        /**
         * @brief
         * @param that
         * @return
         */
        self_type &
        operator = (self_type && that)
        noexcept (
             std::is_nothrow_move_assignable_v <result_type>
          && std::is_nothrow_move_assignable_v <error_type>
        )
        = default;
    };


    /**
     * @brief
     * @tparam TResult
     * @tparam TError
     * @tparam TSafetyPolicy
     */
    template <typename TResult, typename TError, typename TSafetyPolicy>
    class ExpectedBase_ :
      private ExpectedStorage_ <TResult, TError>,
      private TSafetyPolicy
    {
      static_assert (
           IsAllowedV_ <TResult>
        && IsAllowedV_ <TError>
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
        using storage_type = ExpectedStorage_ <result_type, error_type>;

        /**
         * @brief
         */
        using safety_policy_type = TSafetyPolicy;


      protected:
        /**
         * @brief
         */
        using storage_type::isResult_;

        /**
         * @brief
         */
        using storage_type::get_;

        /**
         * @brief
         */
        using storage_type::construct_;

        /**
         * @brief
         */
        using storage_type::assign_;

        /**
         * @brief
         */
        using storage_type::destroy_;

        /**
         * @brief
         */
        using storage_type::clear_;

        /**
         * @brief
         */
        using safety_policy_type::wasChecked_;


      private:
        /**
         * @brief
         */
        using self_type = ExpectedBase_;


      public:
        /**
         * @brief
         */
        constexpr ExpectedBase_ ()
        noexcept (std::is_nothrow_default_constructible_v <safety_policy_type>)
        = default;

        /**
         * @brief
         * @param that
         */
        constexpr ExpectedBase_ (const self_type & that [[maybe_unused]])
        noexcept (
             std::is_nothrow_copy_constructible_v <storage_type>
          && std::is_nothrow_copy_constructible_v <safety_policy_type>
        )
        = default;

        /**
         * @brief
         * @param that
         */
        constexpr ExpectedBase_ (self_type && that [[maybe_unused]])
        noexcept (
             std::is_nothrow_move_constructible_v <storage_type>
          && std::is_nothrow_move_constructible_v <safety_policy_type>
        )
        = default;


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ExpectedBase_ (ResultTag, TArgs && ... args)
        noexcept (
             std::is_nothrow_constructible_v <storage_type, ResultTag, TArgs && ...>
          && std::is_nothrow_default_constructible_v <safety_policy_type>
        ) :
          storage_type (Result, std::forward <TArgs> (args) ...),
          safety_policy_type ()
        { }


        /**
         * @brief
         * @tparam TArgs
         * @param args
         */
        template <typename ... TArgs>
        constexpr ExpectedBase_ (ErrorTag, TArgs && ... args)
        noexcept (
             std::is_nothrow_constructible_v <storage_type, ErrorTag, TArgs && ...>
          && std::is_nothrow_default_constructible_v <safety_policy_type>
        ) :
          storage_type (Error, std::forward <TArgs> (args) ...),
          safety_policy_type ()
        { }


      protected:
        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const result_type &
        result_ () const noexcept
        {
          return storage_type::get_ (Result);
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr result_type &
        result_ () noexcept
        {
          return storage_type::get_ (Result);
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const result_type &
        result_Safe_ () const noexcept
        {
          if (isResult_ ())
          {
            return result_ ();
          }

          FATAL_NL ("Cannot access result because this instance of  `Expected'  was initialized as an error");
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr result_type &
        result_Safe_ () noexcept
        {
          if (isResult_ ())
          {
            return result_ ();
          }

          FATAL_NL ("Cannot access result because this instance of  `Expected'  was initialized as an error");
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const result_type &
        result_Checked_ () const noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return result_Safe_ ();
          }

          FATAL_NL ("Instance of  `Expected'  must be checked for being an error before accessing result");
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr result_type &
        result_Checked_ () noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return result_Safe_ ();
          }

          FATAL_NL ("Instance of  `Expected'  must be checked for being an error before accessing result");
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const error_type &
        error_ () const noexcept
        {
          return storage_type::get_ (Error);
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr error_type &
        error_ () noexcept
        {
          return storage_type::get_ (Error);
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const error_type &
        error_Safe_ () const noexcept
        {
          if (isResult_ ())
          {
            FATAL_NL ("Cannot access error because this instance of  `Expected'  was initialized as a result");
          }

          return error_ ();
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr error_type &
        error_Safe_ () noexcept
        {
          if (isResult_ ())
          {
            FATAL_NL ("Cannot access error because this instance of  `Expected'  was initialized as a result");
          }

          return error_ ();
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr const error_type &
        error_Checked_ () const noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return error_Safe_ ();
          }

          FATAL_NL ("Instance of  `Expected'  must be checked for being an error before accessing error");
        }


        /**
         * @brief
         * @return
         */
        [[nodiscard]] constexpr error_type &
        error_Checked_ () noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return error_Safe_ ();
          }

          FATAL_NL ("Instance of  `Expected'  must be checked for being an error before accessing error");
        }


      public:
        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (const self_type & that [[maybe_unused]])
        noexcept (
             std::is_nothrow_copy_assignable_v <storage_type>
          && std::is_nothrow_copy_assignable_v <safety_policy_type>
        )
        = default;

        /**
         * @brief
         * @param that
         * @return
         */
        constexpr self_type &
        operator = (self_type && that [[maybe_unused]])
        noexcept (
             std::is_nothrow_move_assignable_v <storage_type>
          && std::is_nothrow_move_assignable_v <safety_policy_type>
        )
        = default;
    };
  }


  /**
   * @brief
   * @tparam TError
   */
  template <typename TError>
  class Unexpected
  {
    static_assert (ExpectedInternals_::IsAllowedV_ <TError>);


    public:
      /**
       * @brief
       */
      using error_type = TError;


    private:
      /**
       * @brief
       */
      using self_type = Unexpected;


    public:
      /**
       * @brief
       */
      constexpr Unexpected ()
      noexcept (std::is_nothrow_default_constructible_v <error_type>)
      = default;

      /**
       * @brief
       * @param that
       */
      constexpr Unexpected (const self_type & that [[maybe_unused]])
      noexcept (std::is_nothrow_copy_constructible_v <error_type>)
      = default;

      /**
       * @brief
       * @param that
       */
      constexpr Unexpected (self_type && that [[maybe_unused]])
      noexcept (std::is_nothrow_move_constructible_v <error_type>)
      = default;


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param that_error
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      explicit constexpr Unexpected (TThatError && that_error)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::forward <TThatError> (that_error))
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param that_error
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && ! std::is_convertible_v <TThatError &&, error_type>
        > ...
      >
      explicit constexpr Unexpected (TThatError && that_error)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::forward <TThatError> (that_error))
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && ! std::is_convertible_v <const TThatError &, error_type>
        > ...
      >
      explicit constexpr Unexpected (const Unexpected <TThatError> & that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError>) :
        error_ (that.error ())
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && std::is_convertible_v <const TThatError &, error_type>
        > ...
      >
      constexpr Unexpected (const Unexpected <TThatError> & that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError>) :
        error_ (that.error ())
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && ! std::is_convertible_v <TThatError &&, error_type>
        > ...
      >
      explicit constexpr Unexpected (Unexpected <TThatError> && that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::move (that.error ()))
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      constexpr Unexpected (Unexpected <TThatError> && that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::move (that.error ()))
      { }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const error_type &
      error () const & noexcept
      {
        return error_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr error_type &
      error () & noexcept
      {
        return error_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const error_type &&
      error () const && noexcept
      {
        return error_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr error_type &&
      error () && noexcept
      {
        return error_;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]])
      noexcept (std::is_nothrow_copy_assignable_v <error_type>)
      = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]])
      noexcept (std::is_nothrow_move_assignable_v <error_type>)
      = default;


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator == (const self_type & that) const
      {
        if (this == & that)
        {
          return true;
        }

        return error () == that.error ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator != (const self_type & that) const
      {
        return ! operator == (that);
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
        output << "Unexpected{error:" << self.error () << '}';

        return output;
      }


    private:
      /**
       * @brief
       */
      error_type error_;
  };


  /**
   * @brief
   * @tparam TType
   */
  template <typename TType>
  Unexpected (TType) -> Unexpected <TType>;


  /**
   * @brief
   * @tparam TResult
   * @tparam TError
   * @tparam TSafetyPolicy
   */
  template <typename TResult, typename TError, typename TSafetyPolicy = UncheckedPolicy>
  class [[nodiscard]] Expected final :
    private ExpectedInternals_::ExpectedBase_ <TResult, TError, TSafetyPolicy>,
    private EnableCopyMove <
      std::is_copy_constructible_v <TResult> && std::is_copy_constructible_v <TError>,
         std::is_copy_constructible_v <TResult> && std::is_copy_constructible_v <TError>
      && std::is_copy_assignable_v <TResult> && std::is_copy_assignable_v <TError>,
      std::is_move_constructible_v <TResult> && std::is_move_constructible_v <TError>,
         std::is_move_constructible_v <TResult> && std::is_move_constructible_v <TError>
      && std::is_move_assignable_v <TResult> && std::is_move_assignable_v <TError>,
      Expected <TResult, TError, TSafetyPolicy>
    >
  {
    static_assert (
         ExpectedInternals_::IsAllowedV_ <TResult>
      && ExpectedInternals_::IsAllowedV_ <TError>
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
      using safety_policy_type = TSafetyPolicy;

      /**
       * @brief
       */
      using unexpected_type = Unexpected <error_type>;


    private:
      /**
       * @brief
       */
      using self_type = Expected;

      /**
       * @brief
       */
      using base_type = ExpectedInternals_::ExpectedBase_ <result_type, error_type, safety_policy_type>;


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam TThatSafetyPolicy
       */
      template <typename TThatResult, typename TThatError, typename TThatSafetyPolicy>
      friend class Expected;


    public:
      /**
       * @brief
       * @tparam TDummy
       * @tparam ...
       */
      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <result_type>, TDummy> ...>
      constexpr Expected ()
      noexcept (
           std::is_nothrow_default_constructible_v <result_type>
        && std::is_nothrow_constructible_v <base_type, ResultTag, result_type &&>
      ) :
        base_type (Result, result_type ())
      { }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam ...
       * @param that_result
       */
      template <
        typename TThatResult = result_type,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && ! std::is_convertible_v <TThatResult &&, result_type>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, self_type>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, unexpected_type>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, ResultTag>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, ErrorTag>
        > ...
      >
      explicit constexpr Expected (TThatResult && that_result)
      noexcept (std::is_nothrow_constructible_v <base_type, ResultTag, TThatResult &&>) :
        base_type (Result, std::forward <TThatResult> (that_result))
      { }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam ...
       * @param that_result
       */
      template <
        typename TThatResult = result_type,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, self_type>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, unexpected_type>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, ResultTag>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, ErrorTag>
        > ...
      >
      constexpr Expected (TThatResult && that_result)
      noexcept (std::is_nothrow_constructible_v <base_type, ResultTag, TThatResult &&>) :
        base_type (Result, std::forward <TThatResult> (that_result))
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param unexpected
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, const TThatError &>
          && ! std::is_convertible_v <const TThatError &, error_type>
        > ...
      >
      explicit constexpr Expected (const Unexpected <TThatError> & unexpected)
      noexcept (std::is_nothrow_constructible_v <base_type, ErrorTag, const TThatError &>) :
        base_type (Error, unexpected.error ())
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param unexpected
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, const TThatError &>> ...
      >
      constexpr Expected (const Unexpected <TThatError> & unexpected)
      noexcept (std::is_nothrow_constructible_v <base_type, ErrorTag, const TThatError &>) :
        base_type (Error, unexpected.error ())
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param unexpected
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && ! std::is_convertible_v <TThatError &&, error_type>
        > ...
      >
      explicit constexpr Expected (Unexpected <TThatError> && unexpected)
      noexcept (std::is_nothrow_constructible_v <base_type, ErrorTag, TThatError &&>) :
        base_type (Error, std::move (unexpected.error ()))
      { }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param unexpected
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      constexpr Expected (Unexpected <TThatError> && unexpected)
      noexcept (std::is_nothrow_constructible_v <base_type, ErrorTag, TThatError &&>) :
        base_type (Error, std::move (unexpected.error ()))
      { }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, const TThatResult &>
          && ! std::is_convertible_v <const TThatResult &, result_type>
          && std::is_constructible_v <error_type, const TThatError &>
          && ! std::is_convertible_v <const TThatError &, error_type>
          && ! std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      explicit constexpr Expected (const Expected <TThatResult, TThatError> & that)
      noexcept (
           std::is_nothrow_default_constructible_v <base_type>
        && std::is_nothrow_copy_constructible_v <result_type>
        && std::is_nothrow_copy_constructible_v <error_type>
      ) :
        base_type ()
      {
        base_type::isResult_ (that.isResult_ ());

        if (that.isResult_ ())
        {
          base_type::construct_ (Result, that.result_ ());
        }
        else
        {
          base_type::construct_ (Error, that.error_ ());
        }

        that.wasChecked_ (true);
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, const TThatResult &>
          && std::is_constructible_v <error_type, const TThatError &>
          && ! std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      constexpr Expected (const Expected <TThatResult, TThatError> & that)
      noexcept (
           std::is_nothrow_default_constructible_v <base_type>
        && std::is_nothrow_copy_constructible_v <result_type>
        && std::is_nothrow_copy_constructible_v <error_type>
      ) :
        base_type ()
      {
        base_type::isResult_ (that.isResult_ ());

        if (that.isResult_ ())
        {
          base_type::construct_ (Result, that.result_ ());
        }
        else
        {
          base_type::construct_ (Error, that.error_ ());
        }

        that.wasChecked_ (true);
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && ! std::is_convertible_v <TThatResult &&, result_type>
          && std::is_constructible_v <error_type, TThatError &&>
          && ! std::is_convertible_v <TThatError &&, error_type>
          && ! std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      explicit constexpr Expected (Expected <TThatResult, TThatError> && that)
      noexcept (
           std::is_nothrow_default_constructible_v <base_type>
        && std::is_nothrow_move_constructible_v <result_type>
        && std::is_nothrow_move_constructible_v <error_type>
      ) :
        base_type ()
      {
        base_type::isResult_ (that.isResult_ ());

        if (that.isResult_ ())
        {
          base_type::construct_ (Result, std::move (that.result_ ()));
        }
        else
        {
          base_type::construct_ (Error, std::move (that.error_ ()));
        }

        that.wasChecked_ (true);
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && std::is_constructible_v <error_type, TThatError &&>
          && ! std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      constexpr Expected (Expected <TThatResult, TThatError> && that)
      noexcept (
           std::is_nothrow_default_constructible_v <base_type>
        && std::is_nothrow_move_constructible_v <result_type>
        && std::is_nothrow_move_constructible_v <error_type>
      ) :
        base_type ()
      {
        base_type::isResult_ (that.isResult_ ());

        if (that.isResult_ ())
        {
          base_type::construct_ (Result, std::move (that.result_ ()));
        }
        else
        {
          base_type::construct_ (Error, std::move (that.error_ ()));
        }

        that.wasChecked_ (true);
      }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @tparam ...
       * @param init
       * @param args
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <
          std::is_constructible_v <result_type, const std::initializer_list <TInit> &, TArgs && ...>
        > ...
      >
      constexpr Expected (ResultTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
        std::is_nothrow_constructible_v <base_type, ResultTag, const std::initializer_list <TInit> &, TArgs && ...>
      ) :
        base_type (Result, init, std::forward <TArgs> (args) ...)
      { }


      /**
       * @brief
       * @tparam TArgs
       * @tparam ...
       * @param args
       */
      template <
        typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <result_type, TArgs && ...>> ...
      >
      constexpr Expected (ResultTag, TArgs && ... args)
      noexcept (std::is_nothrow_constructible_v <base_type, ResultTag, TArgs && ...>) :
        base_type (Result, std::forward <TArgs> (args) ...)
      { }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @tparam ...
       * @param init
       * @param args
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <
          std::is_constructible_v <error_type, const std::initializer_list <TInit> &, TArgs && ...>
        > ...
      >
      constexpr Expected (ErrorTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
        std::is_nothrow_constructible_v <base_type, ErrorTag, const std::initializer_list <TInit> &, TArgs && ...>
      ) :
        base_type (Error, init, std::forward <TArgs> (args) ...)
      { }


      /**
       * @brief
       * @tparam TArgs
       * @tparam ...
       * @param args
       */
      template <
        typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, TArgs && ...>> ...
      >
      constexpr Expected (ErrorTag, TArgs && ... args)
      noexcept (std::is_nothrow_constructible_v <base_type, ErrorTag, TArgs && ...>) :
        base_type (Error, std::forward <TArgs> (args) ...)
      { }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const result_type &
      result () const & noexcept
      {
        return base_type::result_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr result_type &
      result () & noexcept
      {
        return base_type::result_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const result_type &&
      result () const && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr result_type &&
      result () && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const error_type &
      error () const & noexcept
      {
        return base_type::error_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr error_type &
      error () & noexcept
      {
        return base_type::error_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const error_type &&
      error () const && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::error_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr error_type &&
      error () && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::error_Safe_ ());
      }


      /**
       * @brief
       * @param message
       * @return
       */
      constexpr const result_type &
      expect (const CString & message) const & noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return base_type::result_ ();
        }

        FATAL_NL (message);
      }


      /**
       * @brief
       * @param message
       * @return
       */
      constexpr result_type &
      expect (const CString & message) & noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return base_type::result_ ();
        }

        FATAL_NL (message);
      }


      /**
       * @brief
       * @param message
       * @return
       */
      constexpr const result_type &&
      expect (const CString & message) const && noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return std::move (base_type::result_ ());
        }

        FATAL_NL (message);
      }


      /**
       * @brief
       * @param message
       * @return
       */
      constexpr result_type &&
      expect (const CString & message) && noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return std::move (base_type::result_ ());
        }

        FATAL_NL (message);
      }


      /**
       * @brief
       * @tparam TDummy
       * @tparam ...
       * @return
       */
      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <result_type>, TDummy> ...>
      constexpr result_type &
      reset (ResultTag)
      noexcept (
           std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_default_constructible_v <result_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (true);
        base_type::template construct_ (Result, result_type ());

        base_type::wasChecked_ (false);

        return base_type::result_ ();
      }


      /**
       * @brief
       * @tparam TDummy
       * @tparam ...
       * @return
       */
      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <error_type>, TDummy> ...>
      constexpr error_type &
      reset (ErrorTag)
      noexcept (
           std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_default_constructible_v <error_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (false);
        base_type::template construct_ (Error, error_type ());

        base_type::wasChecked_ (false);

        return base_type::error_ ();
      }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @tparam ...
       * @param init
       * @param args
       * @return
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <
          std::is_constructible_v <result_type, const std::initializer_list <TInit> &, TArgs && ...>
        > ...
      >
      constexpr result_type &
      emplace (ResultTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
           std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <result_type, const std::initializer_list <TInit> &, TArgs && ...>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (true);
        base_type::template construct_ (Result, init, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::result_ ();
      }


      /**
       * @brief
       * @tparam TArgs
       * @tparam ...
       * @param args
       * @return
       */
      template <typename ... TArgs, std::enable_if_t <std::is_constructible_v <result_type, TArgs && ...>> ...>
      constexpr result_type &
      emplace (ResultTag, TArgs && ... args)
      noexcept (
           std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <result_type, TArgs && ...>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (true);
        base_type::template construct_ (Result, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::result_ ();
      }


      /**
       * @brief
       * @tparam TInit
       * @tparam TArgs
       * @tparam ...
       * @param init
       * @param args
       * @return
       */
      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <
          std::is_constructible_v <error_type, const std::initializer_list <TInit> &, TArgs && ...>
        > ...
      >
      constexpr error_type &
      emplace (ErrorTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
           std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <error_type, const std::initializer_list <TInit> &, TArgs && ...>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (false);
        base_type::template construct_ (Error, init, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::error_ ();
      }


      /**
       * @brief
       * @tparam TArgs
       * @tparam ...
       * @param args
       * @return
       */
      template <
        typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, TArgs && ...>> ...
      >
      constexpr error_type &
      emplace (ErrorTag, TArgs && ... args)
      noexcept (
           std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <error_type, TArgs && ...>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (false);
        base_type::template construct_ (Error, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::error_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] explicit constexpr operator bool () const noexcept
      {
        base_type::wasChecked_ (true);

        return base_type::isResult_ ();
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_assignable_v <result_type &, const TThatResult &>
          && std::is_constructible_v <result_type &, const TThatResult &>
          && std::is_assignable_v <error_type &, const TThatError &>
          && std::is_constructible_v <error_type &, const TThatError &>
          && ! std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      constexpr self_type &
      operator = (const Expected <TThatResult, TThatError> & that)
      noexcept (
           std::is_nothrow_assignable_v <result_type, const TThatResult &>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_constructible_v <result_type, const TThatResult &>
        && std::is_nothrow_assignable_v <error_type, const TThatError &>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <error_type, const TThatError &>
      )
      {
        if (base_type::isResult_ ())
        {
          if (that.isResult_ ())
          {
            base_type::assign_ (Result, that.get_ (Result));
          }
          else
          {
            base_type::destroy_ (Result);
            base_type::construct_ (Error, that.get_ (Error));
          }
        }
        else
        {
          if (that.isResult_ ())
          {
            base_type::destroy_ (Error);
            base_type::construct_ (Result, that.get_ (Result));
          }
          else
          {
            base_type::assign_ (Error, that.get_ (Error));
          }
        }

        base_type::isResult_ (that.isResult_ ());

        base_type::wasChecked_ (false);
        that.wasChecked_ (true);

        return * this;
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_assignable_v <result_type &, TThatResult &&>
          && std::is_constructible_v <result_type &, TThatResult &&>
          && std::is_assignable_v <error_type &, TThatError &&>
          && std::is_constructible_v <error_type &, TThatError &&>
          && ! std::is_same_v <self_type, Expected <TThatResult, TThatError>>
        > ...
      >
      constexpr self_type &
      operator = (Expected <TThatResult, TThatError> && that)
      noexcept (
           std::is_nothrow_assignable_v <result_type, TThatResult &&>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_constructible_v <result_type, TThatResult &&>
        && std::is_nothrow_assignable_v <error_type, TThatError &&>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <error_type, TThatError &&>
      )
      {
        if (base_type::isResult_ ())
        {
          if (that.isResult_ ())
          {
            base_type::assign_ (Result, std::move (that.get_ (Result)));
          }
          else
          {
            base_type::destroy_ (Result);
            base_type::construct_ (Error, std::move (that.get_ (Error)));
          }
        }
        else
        {
          if (that.isResult_ ())
          {
            base_type::destroy_ (Error);
            base_type::construct_ (Result, std::move (that.get_ (Result)));
          }
          else
          {
            base_type::assign_ (Error, std::move (that.get_ (Error)));
          }
        }

        base_type::isResult_ (that.isResult_ ());

        base_type::wasChecked (false);
        that.wasChecked_ (true);

        return * this;
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam ...
       * @param that_result
       * @return
       */
      template <
        typename TThatResult = result_type,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && std::is_assignable_v <result_type &, TThatResult &&>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, self_type>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, unexpected_type>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, ResultTag>
          && ! std::is_same_v <RemoveCVRefT <TThatResult &&>, ErrorTag>
        > ...
      >
      constexpr self_type &
      operator = (TThatResult && that_result)
      noexcept (
           std::is_nothrow_assignable_v <result_type, TThatResult &&>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <result_type, TThatResult &&>
      )
      {
        using that_result_type = TThatResult;


        if (base_type::isResult_ ())
        {
          base_type::assign_ (Result, std::forward <that_result_type> (that_result));
        }
        else
        {
          base_type::destroy_ (Error);
          base_type::isResult_ (true);
          base_type::construct_ (Result, std::forward <that_result_type> (that_result));
        }

        base_type::wasChecked_ (false);

        return * this;
      }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param unexpected
       * @return
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, const TThatError &>> ...
      >
      constexpr self_type &
      operator = (const Unexpected <TThatError> & unexpected)
      noexcept (
           std::is_nothrow_assignable_v <error_type, const TThatError &>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_constructible_v <error_type, const TThatError &>
      )
      {
        if (base_type::isResult_ ())
        {
          base_type::destroy_ (Result);
          base_type::isResult_ (false);
          base_type::construct_ (Error, unexpected.error ());
        }
        else
        {
          base_type::assign_ (Error, unexpected.error ());
        }

        base_type::wasChecked_ (false);

        return * this;
      }


      /**
       * @brief
       * @tparam TThatError
       * @tparam ...
       * @param unexpected
       * @return
       */
      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      constexpr self_type &
      operator = (Unexpected <TThatError> && unexpected)
      noexcept (
           std::is_nothrow_assignable_v <error_type, TThatError &&>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_constructible_v <error_type, TThatError &&>
      )
      {
        if (base_type::isResult_ ())
        {
          base_type::destroy_ (Result);
          base_type::isResult_ (false);
          base_type::construct_ (Error, std::move (unexpected.error ()));
        }
        else
        {
          base_type::assign_ (Error, std::move (unexpected.error ()));
        }

        base_type::wasChecked_ (false);

        return * this;
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <std::is_constructible_v <Expected <TThatResult, TThatError>, const self_type &>> ...
      >
      Expected <TThatResult, TThatError>
      constexpr operator & (const Expected <TThatResult, TThatError> & that) const &
      noexcept (std::is_nothrow_constructible_v <Expected <TThatResult, TThatError>, const self_type &>)
      {
        using that_type = Expected <TThatResult, TThatError>;


        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return that;
        }

        that.wasChecked_ (true);

        return that_type (* this);
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <std::is_constructible_v <Expected <TThatResult, TThatError>, self_type &&>> ...
      >
      Expected <TThatResult, TThatError>
      constexpr operator & (const Expected <TThatResult, TThatError> & that) &&
      noexcept (std::is_nothrow_constructible_v <Expected <TThatResult, TThatError>, self_type &&>)
      {
        using that_type = Expected <TThatResult, TThatError>;


        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return that;
        }

        that.wasChecked_ (true);

        return that_type (std::move (* this));
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <std::is_constructible_v <Expected <TThatResult, TThatError>, const self_type &>> ...
      >
      Expected <TThatResult, TThatError>
      constexpr operator | (const Expected <TThatResult, TThatError> & that) const &
      noexcept (std::is_nothrow_constructible_v <Expected <TThatResult, TThatError>, const self_type &>)
      {
        using that_type = Expected <TThatResult, TThatError>;


        if (base_type::isResult_ ())
        {
          that.wasChecked_ (true);

          return that_type (* this);
        }

        base_type::wasChecked_ (true);

        return that;
      }


      /**
       * @brief
       * @tparam TThatResult
       * @tparam TThatError
       * @tparam ...
       * @param that
       * @return
       */
      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <std::is_constructible_v <Expected <TThatResult, TThatError>, self_type &&>> ...
      >
      Expected <TThatResult, TThatError>
      constexpr operator | (const Expected <TThatResult, TThatError> & that) &&
      noexcept (std::is_nothrow_constructible_v <Expected <TThatResult, TThatError>, self_type &&>)
      {
        using that_type = Expected <TThatResult, TThatError>;


        if (base_type::isResult_ ())
        {
          that.wasChecked_ (true);

          return that_type (std::move (* this));
        }

        base_type::wasChecked_ (true);

        return that;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator == (const self_type & that) const
      {
        if (this == & that)
        {
          return true;
        }

        if (base_type::isResult_ ())
        {
          if (that.isResult_ ())
          {
            return base_type::result_ () == that.result_ ();
          }

          return false;
        }

        if (that.isResult_ ())
        {
          return false;
        }

        return base_type::error_ () == that.error_ ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator != (const self_type & that) const
      {
        return ! operator == (that);
      }


      /**
       * @brief
       * @tparam TFallback
       * @tparam ...
       * @param fallback
       * @return
       */
      template <
        typename TFallback = result_type,
        std::enable_if_t <std::is_constructible_v <TFallback, const result_type &>> ...
      >
      [[nodiscard]] constexpr TFallback
      operator || (TFallback && fallback) const &
      noexcept (std::is_nothrow_constructible_v <TFallback, const result_type &>)
      {
        using fallback_type = TFallback;


        base_type::wasChecked_ (true);

        if (base_type::isResult_ ())
        {
          return fallback_type (base_type::result_ ());
        }

        return std::forward <fallback_type> (fallback);
      }


      /**
       * @brief
       * @tparam TFallback
       * @tparam ...
       * @param fallback
       * @return
       */
      template <
        typename TFallback = result_type,
        std::enable_if_t <std::is_constructible_v <TFallback, result_type &&>> ...
      >
      [[nodiscard]] constexpr TFallback
      operator || (TFallback && fallback) &&
      noexcept (std::is_nothrow_constructible_v <TFallback, result_type &&>)
      {
        using fallback_type = TFallback;


        base_type::wasChecked_ (true);

        if (base_type::isResult_ ())
        {
          return fallback_type (std::move (base_type::result_ ()));
        }

        return std::forward <fallback_type> (fallback);
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const result_type &
      operator * () const & noexcept
      {
        return base_type::result_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr result_type &
      operator * () & noexcept
      {
        return base_type::result_Checked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const result_type &&
      operator * () const && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr result_type &&
      operator * () && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::result_Safe_ ());
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const result_type *
      operator -> () const noexcept
      {
        return std::addressof (operator * ());
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr result_type *
      operator -> () noexcept
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


#endif  // UTILS_MISC_EXPECTED_HXX
