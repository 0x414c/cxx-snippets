#ifndef UTILS_MISC_EXPECTED_HXX
#define UTILS_MISC_EXPECTED_HXX


#include <initializer_list> // std::initializer_list
#include <ostream> // std::ostream
#include <memory> // std::addressof
#include <type_traits> // std::*
#include <utility> // std::{forward, move}

#include "../containers/c-string.hxx" // CString
#include "../debug/fatal.hxx" // FATAL_NL
#include "../meta/enable-copy-move.hxx" // EnableCopyMove
#include "../meta/remove-c-v-ref.hxx" // RemoveCVRefT


namespace Utils
{
  struct ResultTag
  {
    explicit constexpr ResultTag (void) noexcept = default;
  };


  inline constexpr ResultTag Result;


  struct ErrorTag
  {
    explicit constexpr ErrorTag (void) noexcept = default;
  };


  inline constexpr ErrorTag Error;


  class UncheckedPolicy
  {
    private:
      using self_type = UncheckedPolicy;


    public:
      constexpr UncheckedPolicy (void) noexcept = default;

      constexpr UncheckedPolicy (const self_type & that [[maybe_unused]]) noexcept = default;

      constexpr UncheckedPolicy (self_type && that [[maybe_unused]]) noexcept = default;


    protected:
      constexpr bool
      wasChecked_ (void) const noexcept
      {
        return true;
      }


      constexpr void
      wasChecked_ (bool was_checked [[maybe_unused]]) const noexcept
      { }


    public:
      self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = default;

      self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = default;
  };


  class CheckedNoDiscardPolicy
  {
    private:
      using self_type = CheckedNoDiscardPolicy;


    public:
      CheckedNoDiscardPolicy (void) noexcept = default;


      CheckedNoDiscardPolicy (const self_type & that) noexcept
      {
        that.wasChecked_ (true);
      }


      CheckedNoDiscardPolicy (self_type && that) noexcept
      {
        that.wasChecked_ (true);
      }


      ~CheckedNoDiscardPolicy (void) noexcept
      {
        if (!wasChecked_ ())
        {
          FATAL_NL ("No instance of `Expected' must be left unchecked");
        }
      }


    protected:
      bool
      wasChecked_ (void) const noexcept
      {
        return was_checked_;
      }


      void
      wasChecked_ (bool was_checked) const noexcept
      {
        was_checked_ = was_checked;
      }


    public:
      self_type &
      operator = (const self_type & that) noexcept
      {
        if (this != &that)
        {
          wasChecked_ (false);
          that.wasChecked_ (true);
        }

        return *this;
      }


      self_type &
      operator = (self_type && that) noexcept
      {
        if (this != &that)
        {
          wasChecked_ (false);
          that.wasChecked_ (true);
        }

        return *this;
      }


    private:
      mutable bool was_checked_ { false };
  };


  namespace ExpectedInternals_
  {
    template <typename TType>
    inline constexpr bool IsAllowedV_ (
         std::is_object_v <TType>
      && !std::is_array_v <TType>
      && std::is_destructible_v <TType>
      && !std::is_same_v <RemoveCVRefT <TType>, ResultTag>
      && !std::is_same_v <RemoveCVRefT <TType>, ErrorTag>
    );


    struct Dummy_ final
    {
      private:
        using self_type = Dummy_;


      public:
        constexpr Dummy_ (void) noexcept = default;

        constexpr Dummy_ (const self_type &) noexcept = delete;

        constexpr Dummy_ (self_type &&) noexcept = delete;

        Dummy_ &
        operator = (const self_type &) noexcept = delete;

        Dummy_ &
        operator = (self_type &&) noexcept = delete;
    };


    template <
      typename TResult, typename TError,
      bool TIsTriviallyAssignable =
           std::is_trivially_copy_assignable_v <TResult>
        && std::is_trivially_move_assignable_v <TResult>
        && std::is_trivially_copy_assignable_v <TError>
        && std::is_trivially_move_assignable_v <TError>
    >
    union ResultOrError_ final
    {
      static_assert (IsAllowedV_ <TResult> && IsAllowedV_ <TError>);


      public:
        using result_type = TResult;

        using error_type = TError;


      private:
        using self_type = ResultOrError_;


      public:
        constexpr ResultOrError_ (void) noexcept
        { }


        constexpr ResultOrError_ (const self_type & that [[maybe_unused]]) noexcept = delete;

        constexpr ResultOrError_ (self_type && that [[maybe_unused]]) noexcept = delete;


        template <typename ... TArgs>
        constexpr ResultOrError_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          result (std::forward <TArgs> (args) ...)
        { }


        template <typename ... TArgs>
        constexpr ResultOrError_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          error (std::forward <TArgs> (args) ...)
        { }


        self_type &
        operator = (const self_type & that [[maybe_unused]]) noexcept = delete;

        self_type &
        operator = (self_type && that [[maybe_unused]]) noexcept = delete;


      private:
        Dummy_ dummy_ { };


      public:
        result_type result;

        error_type error;
    };


    template <typename TResult, typename TError>
    union ResultOrError_ <TResult, TError, false>
    {
      static_assert (IsAllowedV_ <TResult> && IsAllowedV_ <TError>);


      public:
        using result_type = TResult;

        using error_type = TError;


      private:
        using self_type = ResultOrError_;


      public:
        ResultOrError_ (void) noexcept
        { };


        ResultOrError_ (const self_type & that [[maybe_unused]]) noexcept = delete;

        ResultOrError_ (self_type && that [[maybe_unused]]) noexcept = delete;


        ~ResultOrError_ (void) noexcept
        { }


        template <typename ... TArgs>
        ResultOrError_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          result (std::forward <TArgs> (args) ...)
        { }


        template <typename ... TArgs>
        ResultOrError_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          error (std::forward <TArgs> (args) ...)
        { }


        self_type &
        operator = (const self_type & that [[maybe_unused]]) noexcept = delete;

        self_type &
        operator = (self_type && that [[maybe_unused]]) noexcept = delete;


      private:
        Dummy_ dummy_ { };


      public:
        result_type result;

        error_type error;
    };


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
    class ExpectedStorage_
    {
      static_assert (IsAllowedV_ <TResult> && IsAllowedV_ <TError>);


      public:
        using result_type = TResult;

        using error_type = TError;


      private:
        using self_type = ExpectedStorage_;


      public:
        constexpr ExpectedStorage_ (void) noexcept = default;


        constexpr ExpectedStorage_ (const self_type & that)
        noexcept (
             std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            construct_ (Result, that.get_ (Result));
          }
          else
          {
            construct_ (Error, that.get_ (Error));
          }
        }


        constexpr ExpectedStorage_ (self_type && that)
        noexcept (
             std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            construct_ (Result, std::move (that.get_ (Result)));
          }
          else
          {
            construct_ (Error, std::move (that.get_ (Error)));
          }
        }


        template <typename ... TArgs>
        constexpr ExpectedStorage_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          is_result_ (true),
          result_or_error_ (Result, std::forward <TArgs> (args) ...)
        { }


        template <typename ... TArgs>
        constexpr ExpectedStorage_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          is_result_ (false),
          result_or_error_ (Error, std::forward <TArgs> (args) ...)
        { }


      protected:
        [[nodiscard]] constexpr bool
        isResult_ (void) const noexcept
        {
          return is_result_;
        }


        constexpr void
        isResult_ (bool is_result) noexcept
        {
          is_result_ = is_result;
        }


        constexpr const result_type &
        get_ (ResultTag) const noexcept
        {
          return result_or_error_.result;
        }


        constexpr result_type &
        get_ (ResultTag) noexcept
        {
          return result_or_error_.result;
        }


        constexpr const error_type &
        get_ (ErrorTag) const noexcept
        {
          return result_or_error_.error;
        }


        constexpr error_type &
        get_ (ErrorTag) noexcept
        {
          return result_or_error_.error;
        }


        template <typename TThatResult>
        constexpr void
        construct_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_assignable_v <result_type &, TThatResult &&>)
        {
          get_ (Result) = std::forward <TThatResult> (that_result);
        }


        template <typename TThatError>
        constexpr void
        construct_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_assignable_v <error_type &, TThatError &&>)
        {
          get_ (Error) = std::forward <TThatError> (that_error);
        }


        template <typename TThatResult>
        constexpr void
        assign_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_assignable_v <result_type &, TThatResult &&>)
        {
          get_ (Result) = std::forward <TThatResult> (that_result);
        }


        template <typename TThatError>
        constexpr void
        assign_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_assignable_v <error_type &, TThatError &&>)
        {
          get_ (Error) = std::forward <TThatError> (that_error);
        }


        constexpr void
        destroy_ (ResultTag) noexcept
        { }


        constexpr void
        destroy_ (ErrorTag) noexcept
        { }


        constexpr void
        clear_ (void) noexcept
        { }


      public:
        self_type &
        operator = (const self_type & that) noexcept (
             std::is_nothrow_copy_assignable_v <result_type>
          && std::is_nothrow_copy_assignable_v <error_type>
        )
        {
          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                assign_ (Result, that.get_ (Result));
              }
              else
              {
                destroy_ (Result);
                construct_ (Error, that.get_ (Error));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                destroy_ (Error);
                construct_ (Result, that.get_ (Result));
              }
              else
              {
                assign_ (Error, that.get_ (Error));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return *this;
        }


        self_type &
        operator = (self_type && that) noexcept (
             std::is_nothrow_move_assignable_v <result_type>
          && std::is_nothrow_move_assignable_v <error_type>
        )
        {
          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                assign_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                destroy_ (Result);
                construct_ (Error, std::move (that.get_ (Error)));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                destroy_ (Error);
                construct_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                assign_ (Error, std::move (that.get_ (Error)));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return *this;
        }


      private:
        bool is_result_ { };

        ResultOrError_ <result_type, error_type> result_or_error_ { };
    };


    template <typename TResult, typename TError>
    class ExpectedStorage_ <TResult, TError, false, true>
    {
      static_assert (IsAllowedV_ <TResult> && IsAllowedV_ <TError>);


      public:
        using result_type = TResult;

        using error_type = TError;


      private:
        using self_type = ExpectedStorage_;


      public:
        ExpectedStorage_ (void) noexcept = default;


        ExpectedStorage_ (const self_type & that)
        noexcept (
             std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            construct_ (Result, that.get_ (Result));
          }
          else
          {
            construct_ (Error, that.get_ (Error));
          }
        }


        ExpectedStorage_ (self_type && that)
        noexcept (
             std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            construct_ (Result, std::move (that.get_ (Result)));
          }
          else
          {
            construct_ (Error, std::move (that.get_ (Error)));
          }
        }


        template <typename ... TArgs>
        ExpectedStorage_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          is_result_ (true),
          result_or_error_ (Result, std::forward <TArgs> (args) ...)
        { }


        template <typename ... TArgs>
        ExpectedStorage_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          is_result_ (false),
          result_or_error_ (Error, std::forward <TArgs> (args) ...)
        { }


      protected:
        [[nodiscard]] bool
        isResult_ (void) const noexcept
        {
          return is_result_;
        }


        void
        isResult_ (bool is_result) noexcept
        {
          is_result_ = is_result;
        }


        const result_type &
        get_ (ResultTag) const noexcept
        {
          return result_or_error_.result;
        }


        result_type &
        get_ (ResultTag) noexcept
        {
          return result_or_error_.result;
        }


        const error_type &
        get_ (ErrorTag) const noexcept
        {
          return result_or_error_.error;
        }


        error_type &
        get_ (ErrorTag) noexcept
        {
          return result_or_error_.error;
        }


        template <typename TThatResult>
        void
        construct_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_constructible_v <result_type &, TThatResult &&>)
        {
          ::new ((void *) (std::addressof (get_ (Result)))) (result_type) (std::forward <TThatResult> (that_result));
        }


        template <typename TThatError>
        void
        construct_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_constructible_v <error_type &, TThatError &&>)
        {
          ::new ((void *) (std::addressof (get_ (Error)))) (error_type) (std::forward <TThatError> (that_error));
        }


        template <typename TThatResult>
        void
        assign_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_assignable_v <result_type &, TThatResult &&>)
        {
          get_ (Result) = std::forward <TThatResult> (that_result);
        }


        template <typename TThatError>
        void
        assign_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_assignable_v <error_type &, TThatError &&>)
        {
          get_ (Error) = std::forward <TThatError> (that_error);
        }


        void
        destroy_ (ResultTag) noexcept
        { }


        void
        destroy_ (ErrorTag) noexcept
        { }


        void
        clear_ (void) noexcept
        { }


      public:
        self_type &
        operator = (const self_type & that) noexcept (
             std::is_nothrow_copy_assignable_v <result_type>
          && std::is_nothrow_copy_assignable_v <error_type>
        )
        {
          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                assign_ (Result, that.get_ (Result));
              }
              else
              {
                destroy_ (Result);
                construct_ (Error, that.get_ (Error));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                destroy_ (Error);
                construct_ (Result, that.get_ (Result));
              }
              else
              {
                assign_ (Error, that.get_ (Error));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return *this;
        }


        self_type &
        operator = (self_type && that) noexcept (
             std::is_nothrow_move_assignable_v <result_type>
          && std::is_nothrow_move_assignable_v <error_type>
        )
        {
          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                assign_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                destroy_ (Result);
                construct_ (Error, std::move (that.get_ (Error)));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                destroy_ (Error);
                construct_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                assign_ (Error, std::move (that.get_ (Error)));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return *this;
        }


      private:
        bool is_result_ { };

        ResultOrError_ <result_type, error_type> result_or_error_ { };
    };


    template <typename TResult, typename TError>
    class ExpectedStorage_ <TResult, TError, false, false>
    {
      static_assert (IsAllowedV_ <TResult> && IsAllowedV_ <TError>);


      public:
        using result_type = TResult;

        using error_type = TError;


      private:
        using self_type = ExpectedStorage_;


      public:
        ExpectedStorage_ (void) noexcept = default;


        ExpectedStorage_ (const self_type & that)
        noexcept (
             std::is_nothrow_copy_constructible_v <result_type>
          && std::is_nothrow_copy_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            construct_ (Result, that.get_ (Result));
          }
          else
          {
            construct_ (Error, that.get_ (Error));
          }
        }


        ExpectedStorage_ (self_type && that)
        noexcept (
             std::is_nothrow_move_constructible_v <result_type>
          && std::is_nothrow_move_constructible_v <error_type>
        ) :
          is_result_ (that.isResult_ ())
        {
          if (that.isResult_ ())
          {
            construct_ (Result, std::move (that.get_ (Result)));
          }
          else
          {
            construct_ (Error, std::move (that.get_ (Error)));
          }
        }


        template <typename ... TArgs>
        ExpectedStorage_ (ResultTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <result_type, TArgs && ...>) :
          is_result_ (true),
          result_or_error_ (Result, std::forward <TArgs> (args) ...)
        { }


        template <typename ... TArgs>
        ExpectedStorage_ (ErrorTag, TArgs && ... args)
        noexcept (std::is_nothrow_constructible_v <error_type, TArgs && ...>) :
          is_result_ (false),
          result_or_error_ (Error, std::forward <TArgs> (args) ...)
        { }


        ~ExpectedStorage_ (void)
        noexcept (
             std::is_nothrow_destructible_v <result_type>
          && std::is_nothrow_destructible_v <error_type>
        )
        {
          clear_ ();
        }


      protected:
        [[nodiscard]] bool
        isResult_ (void) const noexcept
        {
          return is_result_;
        }


        void
        isResult_ (bool is_result) noexcept
        {
          is_result_ = is_result;
        }


        const result_type &
        get_ (ResultTag) const noexcept
        {
          return result_or_error_.result;
        }


        result_type &
        get_ (ResultTag) noexcept
        {
          return result_or_error_.result;
        }


        const error_type &
        get_ (ErrorTag) const noexcept
        {
          return result_or_error_.error;
        }


        error_type &
        get_ (ErrorTag) noexcept
        {
          return result_or_error_.error;
        }


        template <typename TThatResult>
        void
        construct_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_constructible_v <result_type &, TThatResult &&>)
        {
          ::new ((void *) (std::addressof (get_ (Result)))) (result_type) (std::forward <TThatResult> (that_result));
        }


        template <typename TThatError>
        void
        construct_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_constructible_v <error_type &, TThatError &&>)
        {
          ::new ((void *) (std::addressof (get_ (Error)))) (error_type) (std::forward <TThatError> (that_error));
        }


        template <typename TThatResult>
        void
        assign_ (ResultTag, TThatResult && that_result)
        noexcept (std::is_nothrow_assignable_v <result_type &, TThatResult &&>)
        {
          get_ (Result) = std::forward <TThatResult> (that_result);
        }


        template <typename TThatError>
        void
        assign_ (ErrorTag, TThatError && that_error)
        noexcept (std::is_nothrow_assignable_v <error_type &, TThatError &&>)
        {
          get_ (Error) = std::forward <TThatError> (that_error);
        }


        void
        destroy_ (ResultTag)
        noexcept (std::is_nothrow_destructible_v <result_type>)
        {
          get_ (Result).~result_type ();
        }


        void
        destroy_ (ErrorTag)
        noexcept (std::is_nothrow_destructible_v <error_type>)
        {
          get_ (Error).~error_type ();
        }


        void
        clear_ (void)
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
        self_type &
        operator = (const self_type & that) noexcept (
             std::is_nothrow_copy_assignable_v <result_type>
          && std::is_nothrow_copy_assignable_v <error_type>
        )
        {
          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                assign_ (Result, that.get_ (Result));
              }
              else
              {
                destroy_ (Result);
                construct_ (Error, that.get_ (Error));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                destroy_ (Error);
                construct_ (Result, that.get_ (Result));
              }
              else
              {
                assign_ (Error, that.get_ (Error));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return *this;
        }


        self_type &
        operator = (self_type && that) noexcept (
             std::is_nothrow_move_assignable_v <result_type>
          && std::is_nothrow_move_assignable_v <error_type>
        )
        {
          if (this != &that)
          {
            if (isResult_ ())
            {
              if (that.isResult_ ())
              {
                assign_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                destroy_ (Result);
                construct_ (Error, std::move (that.get_ (Error)));
              }
            }
            else
            {
              if (that.isResult_ ())
              {
                destroy_ (Error);
                construct_ (Result, std::move (that.get_ (Result)));
              }
              else
              {
                assign_ (Error, std::move (that.get_ (Error)));
              }
            }

            isResult_ (that.isResult_ ());
          }

          return *this;
        }


      private:
        bool is_result_ { };

        ResultOrError_ <result_type, error_type> result_or_error_ { };
    };


    template <typename TResult, typename TError, typename TSafetyPolicy>
    class ExpectedBase_ :
      private ExpectedStorage_ <TResult, TError>,
      private TSafetyPolicy
    {
      static_assert (IsAllowedV_ <TResult> && IsAllowedV_ <TError>);


      public:
        using result_type = TResult;

        using error_type = TError;

        using storage_type = ExpectedStorage_ <result_type, error_type>;

        using safety_policy_type = TSafetyPolicy;


      protected:
        using storage_type::isResult_;

        using storage_type::get_;

        using storage_type::construct_;

        using storage_type::assign_;

        using storage_type::destroy_;

        using safety_policy_type::wasChecked_;


      private:
        using self_type = ExpectedBase_;


      public:
        constexpr ExpectedBase_ (void)
        noexcept (std::is_nothrow_default_constructible_v <safety_policy_type>) =
        default;

        constexpr ExpectedBase_ (const self_type & that [[maybe_unused]])
        noexcept (
             std::is_nothrow_copy_constructible_v <storage_type>
          && std::is_nothrow_copy_constructible_v <safety_policy_type>
        ) =
        default;

        constexpr ExpectedBase_ (self_type && that [[maybe_unused]])
        noexcept (
             std::is_nothrow_move_constructible_v <storage_type>
          && std::is_nothrow_move_constructible_v <safety_policy_type>
        ) =
        default;


        template <typename ... TArgs>
        constexpr ExpectedBase_ (ResultTag, TArgs && ... args)
        noexcept (
             std::is_nothrow_constructible_v <result_type, TArgs && ...>
          && std::is_nothrow_constructible_v <storage_type, ResultTag, TArgs && ...>
          && std::is_nothrow_default_constructible_v <safety_policy_type>
        ) :
          storage_type (Result, std::forward <TArgs> (args) ...),
          safety_policy_type ()
        { }


        template <typename ... TArgs>
        constexpr ExpectedBase_ (ErrorTag, TArgs && ... args)
        noexcept (
             std::is_nothrow_constructible_v <error_type, TArgs && ...>
          && std::is_nothrow_constructible_v <storage_type, ErrorTag, TArgs && ...>
          && std::is_nothrow_default_constructible_v <safety_policy_type>
        ) :
          storage_type (Error, std::forward <TArgs> (args) ...),
          safety_policy_type ()
        { }


      protected:
        constexpr const result_type &
        result_ (void) const noexcept
        {
          return storage_type::get_ (Result);
        }


        constexpr result_type &
        result_ (void) noexcept
        {
          return storage_type::get_ (Result);
        }


        constexpr const result_type &
        result_Safe_ (void) const noexcept
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


        constexpr result_type &
        result_Safe_ (void) noexcept
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


        constexpr const result_type &
        result_Checked_ (void) const noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return result_Safe_ ();
          }
          else
          {
            FATAL_NL ("Instance of `Expected' must be checked for being an error before accessing result");
          }
        }


        constexpr result_type &
        result_Checked_ (void) noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return result_Safe_ ();
          }
          else
          {
            FATAL_NL ("Instance of `Expected' must be checked for being an error before accessing result");
          }
        }


        constexpr const error_type &
        error_ (void) const noexcept
        {
          return storage_type::get_ (Error);
        }


        constexpr error_type &
        error_ (void) noexcept
        {
          return storage_type::get_ (Error);
        }


        constexpr const error_type &
        error_Safe_ (void) const noexcept
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


        constexpr error_type &
        error_Safe_ (void) noexcept
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


        constexpr const error_type &
        error_Checked_ (void) const noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return error_Safe_ ();
          }
          else
          {
            FATAL_NL ("Instance of `Expected' must be checked for being an error before accessing error");
          }
        }


        constexpr error_type &
        error_Checked_ (void) noexcept
        {
          if (safety_policy_type::wasChecked_ ())
          {
            return error_Safe_ ();
          }
          else
          {
            FATAL_NL ("Instance of `Expected' must be checked for being an error before accessing error");
          }
        }


      public:
        self_type &
        operator = (const self_type & that [[maybe_unused]])
        noexcept (
             std::is_nothrow_copy_assignable_v <storage_type>
          && std::is_nothrow_copy_assignable_v <safety_policy_type>
        ) =
        default;

        self_type &
        operator = (self_type && that [[maybe_unused]])
        noexcept (
             std::is_nothrow_move_assignable_v <storage_type>
          && std::is_nothrow_move_assignable_v <safety_policy_type>
        ) =
        default;
    };
  }


  template <typename TError>
  class Unexpected
  {
    static_assert (ExpectedInternals_::IsAllowedV_ <TError>);


    public:
      using error_type = TError;


    private:
      using self_type = Unexpected;


    public:
      constexpr Unexpected (void)
      noexcept (std::is_nothrow_default_constructible_v <error_type>) =
      default;

      constexpr Unexpected (const self_type & that [[maybe_unused]])
      noexcept (std::is_nothrow_copy_constructible_v <error_type>) =
      default;

      constexpr Unexpected (self_type && that [[maybe_unused]])
      noexcept (std::is_nothrow_move_constructible_v <error_type>) =
      default;


      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      explicit constexpr Unexpected (TThatError && that_error)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::forward <TThatError> (that_error))
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && !std::is_convertible_v <TThatError &&, error_type>
        > ...
      >
      explicit constexpr Unexpected (TThatError && that_error)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::forward <TThatError> (that_error))
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <
               std::is_constructible_v <error_type, TThatError &&>
            && !std::is_convertible_v <const TThatError &, error_type>
        > ...
      >
      explicit constexpr Unexpected (const Unexpected <TThatError> & that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError>) :
        error_ (that.error ())
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&> && std::is_convertible_v <const TThatError &, error_type>> ...
      >
      constexpr Unexpected (const Unexpected <TThatError> & that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError>) :
        error_ (that.error ())
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && !std::is_convertible_v <TThatError &&, error_type>
        > ...
      >
      explicit constexpr Unexpected (Unexpected <TThatError> && that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::move (that.error ()))
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      constexpr Unexpected (Unexpected <TThatError> && that)
      noexcept (std::is_nothrow_constructible_v <error_type, TThatError &&>) :
        error_ (std::move (that.error ()))
      { }


      constexpr const error_type &
      error (void) const & noexcept
      {
        return error_;
      }


      constexpr error_type &
      error (void) & noexcept
      {
        return error_;
      }


      constexpr const error_type &&
      error (void) const && noexcept
      {
        return error_;
      }


      constexpr error_type &&
      error (void) && noexcept
      {
        return error_;
      }


      self_type &
      operator = (const self_type & that [[maybe_unused]])
      noexcept (std::is_nothrow_copy_assignable_v <error_type>) =
      default;

      self_type &
      operator = (self_type && that [[maybe_unused]])
      noexcept (std::is_nothrow_move_assignable_v <error_type>) =
      default;


      constexpr bool
      operator == (const self_type & that) const
      {
        if (this == &that)
        {
          return true;
        }
        else
        {
          return (error () == that.error ());
        }
      }


      constexpr bool
      operator != (const self_type & that) const
      {
        return !operator == (that);
      }


      friend std::ostream &
      operator << (std::ostream & output, const self_type & self)
      {
        output << "Unexpected{" << "error:" << self.error () << '}';

        return output;
      }


    private:
      error_type error_;
  };


  template <typename TType>
  Unexpected (TType) -> Unexpected <TType>;


  template <typename TResult, typename TError, typename TSafetyPolicy = CheckedNoDiscardPolicy>
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
    static_assert (ExpectedInternals_::IsAllowedV_ <TResult> && ExpectedInternals_::IsAllowedV_ <TError>);


    public:
      using result_type = TResult;

      using error_type = TError;

      using safety_policy_type = TSafetyPolicy;

      using unexpected_type = Unexpected <error_type>;


    private:
      using self_type = Expected;

      using base_type = ExpectedInternals_::ExpectedBase_ <result_type, error_type, safety_policy_type>;


      template <typename TThatResult, typename TThatError, typename TThatSafetyPolicy>
      friend class Expected;


    public:
      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <result_type>, TDummy> ...>
      constexpr Expected (void)
      noexcept (
           std::is_nothrow_default_constructible_v <result_type>
        && std::is_nothrow_constructible_v <base_type, ResultTag, result_type &&>
      ) :
        base_type (Result, result_type ())
      { }


      template <
        typename TThatResult = result_type,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && !std::is_convertible_v <TThatResult &&, result_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, error_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, self_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, unexpected_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, ResultTag>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, ErrorTag>
        > ...
      >
      explicit constexpr Expected (TThatResult && that_result)
      noexcept (
           std::is_nothrow_constructible_v <result_type, TThatResult &&>
        && std::is_nothrow_constructible_v <base_type, ResultTag, TThatResult &&>
      ) :
        base_type (Result, std::forward <TThatResult> (that_result))
      { }


      template <
        typename TThatResult = result_type,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, error_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, self_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, unexpected_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, ResultTag>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, ErrorTag>
        > ...
      >
      constexpr Expected (TThatResult && that_result)
      noexcept (
           std::is_nothrow_constructible_v <result_type, TThatResult &&>
        && std::is_nothrow_constructible_v <base_type, ResultTag, TThatResult &&>
      ) :
        base_type (Result, std::forward <TThatResult> (that_result))
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, const TThatError &>
          && !std::is_convertible_v <const TThatError &, error_type>
        > ...
      >
      explicit constexpr Expected (const Unexpected <TThatError> & unexpected)
      noexcept (
           std::is_nothrow_constructible_v <error_type, const TThatError &>
        && std::is_nothrow_constructible_v <base_type, ErrorTag, const TThatError &>
      ) :
        base_type (Error, unexpected.error ())
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, const TThatError &>> ...
      >
      constexpr Expected (const Unexpected <TThatError> & unexpected)
      noexcept (
           std::is_nothrow_constructible_v <error_type, const TThatError &>
        && std::is_nothrow_constructible_v <base_type, ErrorTag, const TThatError &>
      ) :
        base_type (Error, unexpected.error ())
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <
             std::is_constructible_v <error_type, TThatError &&>
          && !std::is_convertible_v <TThatError &&, error_type>
        > ...
      >
      explicit constexpr Expected (Unexpected <TThatError> && unexpected)
      noexcept (
           std::is_nothrow_constructible_v <error_type, TThatError &&>
        && std::is_nothrow_constructible_v <base_type, ErrorTag, TThatError &&>
      ) :
        base_type (Error, std::move (unexpected.error ()))
      { }


      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      constexpr Expected (Unexpected <TThatError> && unexpected)
      noexcept (
           std::is_nothrow_constructible_v <error_type, TThatError &&>
        && std::is_nothrow_constructible_v <base_type, ErrorTag, TThatError &&>
      ) :
        base_type (Error, std::move (unexpected.error ()))
      { }


      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, const TThatResult &>
          && !std::is_convertible_v <const TThatResult &, result_type>
          && std::is_constructible_v <error_type, const TThatError &>
          && !std::is_convertible_v <const TThatError &, error_type>
          && !std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      explicit constexpr Expected (const Expected <TThatResult, TThatError> & that)
      noexcept (
           std::is_nothrow_copy_constructible_v <result_type>
        && std::is_nothrow_copy_constructible_v <error_type>
        && std::is_nothrow_default_constructible_v <base_type>
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


      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, const TThatResult &>
          && std::is_constructible_v <error_type, const TThatError &>
          && !std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      constexpr Expected (const Expected <TThatResult, TThatError> & that)
      noexcept (
           std::is_nothrow_copy_constructible_v <result_type>
        && std::is_nothrow_copy_constructible_v <error_type>
        && std::is_nothrow_default_constructible_v <base_type>
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


      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && !std::is_convertible_v <TThatResult &&, result_type>
          && std::is_constructible_v <error_type, TThatError &&>
          && !std::is_convertible_v <TThatError &&, error_type>
          && !std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      explicit constexpr Expected (Expected <TThatResult, TThatError> && that)
      noexcept (
           std::is_nothrow_move_constructible_v <result_type>
        && std::is_nothrow_move_constructible_v <error_type>
        && std::is_nothrow_default_constructible_v <base_type>
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


      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && std::is_constructible_v <error_type, TThatError &&>
          && !std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      constexpr Expected (Expected <TThatResult, TThatError> && that)
      noexcept (
           std::is_nothrow_move_constructible_v <result_type>
        && std::is_nothrow_move_constructible_v <error_type>
        && std::is_nothrow_default_constructible_v <base_type>
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


      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <
          std::is_constructible_v <result_type, const std::initializer_list <TInit> &, TArgs && ...>
        > ...
      >
      constexpr Expected (ResultTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <result_type, const std::initializer_list <TInit> &, TArgs && ...>
        && std::is_nothrow_constructible_v <base_type, ResultTag, const std::initializer_list <TInit> &, TArgs && ...>
      ) :
        base_type (Result, init, std::forward <TArgs> (args) ...)
      { }


      template <
        typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <result_type, TArgs && ...>> ...
      >
      constexpr Expected (ResultTag, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <result_type, TArgs && ...>
        && std::is_nothrow_constructible_v <base_type, ResultTag, TArgs && ...>
      ) :
        base_type (Result, std::forward <TArgs> (args) ...)
      { }


      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, const std::initializer_list <TInit> &, TArgs && ...>> ...
      >
      constexpr Expected (ErrorTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <error_type, const std::initializer_list <TInit> &, TArgs && ...>
        && std::is_nothrow_constructible_v <base_type, ErrorTag, const std::initializer_list <TInit> &, TArgs && ...>
      ) :
        base_type (Error, init, std::forward <TArgs> (args) ...)
      { }


      template <
        typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, TArgs && ...>> ...
      >
      constexpr Expected (ErrorTag, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <error_type, TArgs && ...>
        && std::is_nothrow_constructible_v <base_type, ErrorTag, TArgs && ...>
      ) :
        base_type (Error, std::forward <TArgs> (args) ...)
      { }


      constexpr const result_type &
      result (void) const & noexcept
      {
        return base_type::result_Checked_ ();
      }


      constexpr result_type &
      result (void) & noexcept
      {
        return base_type::result_Checked_ ();
      }


      constexpr const result_type &&
      result (void) const && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::result_Safe_ ());
      }


      constexpr result_type &&
      result (void) && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::result_Safe_ ());
      }


      constexpr const error_type &
      error (void) const & noexcept
      {
        return base_type::error_Checked_ ();
      }


      constexpr error_type &
      error (void) & noexcept
      {
        return base_type::error_Checked_ ();
      }


      constexpr const error_type &&
      error (void) const && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::error_Safe_ ());
      }


      constexpr error_type &&
      error (void) && noexcept
      {
        base_type::wasChecked_ (true);

        return std::move (base_type::error_Safe_ ());
      }


      constexpr const result_type &
      expect (const CString & message) const & noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return base_type::result_ ();
        }
        else
        {
          FATAL_NL (message);
        }
      }


      constexpr result_type &
      expect (const CString & message) & noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return base_type::result_ ();
        }
        else
        {
          FATAL_NL (message);
        }
      }


      constexpr const result_type &&
      expect (const CString & message) const && noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return std::move (base_type::result_ ());
        }
        else
        {
          FATAL_NL (message);
        }
      }


      constexpr result_type &&
      expect (const CString & message) && noexcept
      {
        if (base_type::isResult_ ())
        {
          base_type::wasChecked_ (true);

          return std::move (base_type::result_ ());
        }
        else
        {
          FATAL_NL (message);
        }
      }


      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <result_type>, TDummy> ...>
      constexpr result_type &
      reset (ResultTag)
      noexcept (
           std::is_nothrow_default_constructible_v <result_type>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (true);
        base_type::template construct_ (Result, result_type ());

        base_type::wasChecked_ (false);

        return base_type::result_ ();
      }


      template <typename TDummy = void, std::enable_if_t <std::is_default_constructible_v <error_type>, TDummy> ...>
      constexpr error_type &
      reset (ErrorTag)
      noexcept (
           std::is_nothrow_default_constructible_v <error_type>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (false);
        base_type::template construct_ (Error, error_type ());

        base_type::wasChecked_ (false);

        return base_type::error_ ();
      }


      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <result_type, const std::initializer_list <TInit> &, TArgs && ...>> ...
      >
      constexpr result_type &
      emplace (ResultTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <result_type, const std::initializer_list <TInit> &, TArgs && ...>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (true);
        base_type::template construct_ (Result, init, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::result_ ();
      }


      template <typename ... TArgs, std::enable_if_t <std::is_constructible_v <result_type, TArgs && ...>> ...>
      constexpr result_type &
      emplace (ResultTag, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <result_type, TArgs && ...>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (true);
        base_type::template construct_ (Result, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::result_ ();
      }


      template <
        typename TInit, typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, const std::initializer_list <TInit> &, TArgs && ...>> ...
      >
      constexpr error_type &
      emplace (ErrorTag, const std::initializer_list <TInit> & init, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <error_type, const std::initializer_list <TInit> &, TArgs && ...>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (false);
        base_type::template construct_ (Error, init, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::error_ ();
      }


      template <
        typename ... TArgs,
        std::enable_if_t <std::is_constructible_v <error_type, TArgs && ...>> ...
      >
      constexpr error_type &
      emplace (ErrorTag, TArgs && ... args)
      noexcept (
           std::is_nothrow_constructible_v <error_type, TArgs && ...>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
      )
      {
        base_type::clear_ ();
        base_type::isResult_ (false);
        base_type::template construct_ (Error, std::forward <TArgs> (args) ...);

        base_type::wasChecked_ (false);

        return base_type::error_ ();
      }


      [[nodiscard]] explicit constexpr operator bool (void) const noexcept
      {
        base_type::wasChecked_ (true);

        return base_type::isResult_ ();
      }


      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_assignable_v <result_type &, const TThatResult &>
          && std::is_assignable_v <error_type &, const TThatError &>
          && std::is_constructible_v <result_type &, const TThatResult &>
          && std::is_constructible_v <error_type &, const TThatError &>
          && !std::is_same_v <Expected <TThatResult, TThatError>, self_type>
        > ...
      >
      self_type &
      operator = (const Expected <TThatResult, TThatError> & that)
      noexcept (
           std::is_nothrow_assignable_v <result_type, const TThatResult &>
        && std::is_nothrow_assignable_v <error_type, const TThatError &>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <result_type, const TThatResult &>
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

        return *this;
      }


      template <
        typename TThatResult, typename TThatError,
        std::enable_if_t <
             std::is_assignable_v <result_type &, TThatResult &&>
          && std::is_assignable_v <error_type &, TThatError &&>
          && std::is_constructible_v <result_type &, TThatResult &&>
          && std::is_constructible_v <error_type &, TThatError &&>
          && !std::is_same_v <self_type, Expected <TThatResult, TThatError>>
        > ...
      >
      self_type &
      operator = (Expected <TThatResult, TThatError> && that)
      noexcept (
           std::is_nothrow_assignable_v <result_type, TThatResult &&>
        && std::is_nothrow_assignable_v <error_type, TThatError &&>
        && std::is_nothrow_destructible_v <result_type>
        && std::is_nothrow_destructible_v <error_type>
        && std::is_nothrow_constructible_v <result_type, TThatResult &&>
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

        return *this;
      }


      template <
        typename TThatResult = result_type,
        std::enable_if_t <
             std::is_constructible_v <result_type, TThatResult &&>
          && std::is_assignable_v <result_type &, TThatResult &&>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, error_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, self_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, unexpected_type>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, ResultTag>
          && !std::is_same_v <RemoveCVRefT <TThatResult &&>, ErrorTag>
        > ...
      >
      self_type &
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

        return *this;
      }


      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, const TThatError &>> ...
      >
      self_type &
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

        return *this;
      }


      template <
        typename TThatError = error_type,
        std::enable_if_t <std::is_constructible_v <error_type, TThatError &&>> ...
      >
      self_type &
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

        return *this;
      }


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
        else
        {
          that.wasChecked_ (true);

          return that_type (*this);
        }
      }


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
        else
        {
          that.wasChecked_ (true);

          return that_type (std::move (*this));
        }
      }


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

          return that_type (*this);
        }
        else
        {
          base_type::wasChecked_ (true);

          return that;
        }
      }


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

          return that_type (std::move (*this));
        }
        else
        {
          base_type::wasChecked_ (true);

          return that;
        }
      }


      constexpr bool
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


      constexpr bool
      operator != (const self_type & that) const
      {
        return !operator == (that);
      }


      template <
        typename TFallback = result_type,
        std::enable_if_t <std::is_constructible_v <TFallback, const result_type &>> ...
      >
      constexpr TFallback
      operator || (TFallback && fallback) const &
      noexcept (std::is_nothrow_constructible_v <TFallback, const result_type &>)
      {
        using fallback_type = TFallback;


        base_type::wasChecked_ (true);

        if (base_type::isResult_ ())
        {
          return fallback_type (base_type::result_ ());
        }
        else
        {
          return std::forward <fallback_type> (fallback);
        }
      }


      template <
        typename TFallback = result_type,
        std::enable_if_t <std::is_constructible_v <TFallback, result_type &&>> ...
      >
      constexpr TFallback
      operator || (TFallback && fallback) &&
      noexcept (std::is_nothrow_constructible_v <TFallback, result_type &&>)
      {
        using fallback_type = TFallback;


        base_type::wasChecked_ (true);

        if (base_type::isResult_ ())
        {
          return fallback_type (std::move (base_type::result_ ()));
        }
        else
        {
          return std::forward <fallback_type> (fallback);
        }
      }


      constexpr const result_type &
      operator * (void) const & noexcept
      {
        return base_type::result_Safe_ ();
      }


      constexpr result_type &
      operator * (void) & noexcept
      {
        return base_type::result_Safe_ ();
      }


      constexpr const result_type &&
      operator * (void) const && noexcept
      {
        return std::move (base_type::result_Safe_ ());
      }


      constexpr result_type &&
      operator * (void) && noexcept
      {
        return std::move (base_type::result_Safe_ ());
      }


      constexpr const result_type *
      operator -> (void) const noexcept
      {
        return std::addressof (operator * ());
      }


      constexpr result_type *
      operator -> (void) noexcept
      {
        return std::addressof (operator * ());
      }


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
