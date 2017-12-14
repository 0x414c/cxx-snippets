#ifndef UTILS_MISC_RESULT_HXX
#define UTILS_MISC_RESULT_HXX


#include <ostream> // std::ostream
#include <utility> // std::{forward, move}

#include "../debug/fatal.hxx" // FATAL
#include "../type-traits/aligned-union-storage-manager.hxx" // AlignedUnionStorageManager
#include "operation-status.hxx" // OperationStatus


namespace Utils
{
  /**
   * @brief
   * @tparam TResult
   * @tparam TError
   */
  template <typename TResult, typename TError = OperationStatus>
  class [[nodiscard]] Result final
  {
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
      using self_type = Result;


      /**
       * @brief
       */
      Result (void)
      {
        initialize_ ();
      }


      /**
       * @brief
       * @param that
       */
      Result (const self_type & that) :
#ifdef RESULT_WITH_RUNTIME_CHECKS
        is_error_ (that.is_error_),
        error_was_checked_ (that.error_was_checked_)
#else // RESULT_WITH_RUNTIME_CHECKS
        is_error_ (that.is_error_)
#endif // RESULT_WITH_RUNTIME_CHECKS
      {
        if (that.isError_Unchecked_ ())
        {
          result_or_error_.template construct <error_type> (that.result_or_error_.template get <error_type> ());
        }
        else
        {
          result_or_error_.template construct <result_type> (that.result_or_error_.template get <result_type> ());
        }
      }


      /**
       * @brief
       * @param that
       */
      Result (self_type && that) :
#ifdef RESULT_WITH_RUNTIME_CHECKS
        is_error_ (that.is_error_),
        error_was_checked_ (that.error_was_checked_)
#else // RESULT_WITH_RUNTIME_CHECKS
        is_error_ (that.is_error_)
#endif // RESULT_WITH_RUNTIME_CHECKS
      {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
        if (that.isError_Unchecked_ ())
#pragma GCC diagnostic pop
        {
          result_or_error_.template construct <error_type> (
            std::move (that.result_or_error_.template get <error_type> ())
          );
        }
        else
        {
          result_or_error_.template construct <result_type> (
            std::move (that.result_or_error_.template get <result_type> ())
          );
        }
      }


      /**
       * @brief
       */
      ~Result (void)
      {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
        if (isError_Unchecked_ ())
#pragma GCC diagnostic pop
        {
          result_or_error_.template destroy <error_type> ();
        }
        else
        {
          result_or_error_.template destroy <result_type> ();
        }
      }


      /**
       * @brief
       * @return
       */
      const result_type &
      result (void) const
      {
#ifdef RESULT_WITH_RUNTIME_CHECKS
        if (errorWasChecked_ ())
        {
          if (!isError_Unchecked_ ())
          {
            return result_Unchecked_ ();
          }
          else
          {
            FATAL ("Cannot access result because this instance of Result was initialized as an error.");
          }
        }
        else
        {
          FATAL ("Result should be checked for being an error before calling `result ()'.");
        }
#else // RESULT_WITH_RUNTIME_CHECKS
        return result_Unchecked_ ();
#endif // RESULT_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      error (void) const
      {
#ifdef RESULT_WITH_RUNTIME_CHECKS
        if (errorWasChecked_ ())
        {
          if (isError_Unchecked_ ())
          {
            return error_Unchecked_ ();
          }
          else
          {
            FATAL ("Cannot access error because this instance of Result was initialized as a result.");
          }
        }
        else
        {
          FATAL ("Result should be checked for being an error before calling `error ()'.");
        }
#else // RESULT_WITH_RUNTIME_CHECKS
        return error_Unchecked_ ();
#endif // RESULT_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] bool
      isError (void) const
      {
#ifdef RESULT_WITH_RUNTIME_CHECKS
        errorWasChecked_ (true);
#endif // RESULT_WITH_RUNTIME_CHECKS

        return isError_Unchecked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] explicit operator bool (void) const
      {
        return !isError ();
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
          if (isError_Unchecked_ ())
          {
            if (that.isError_Unchecked_ ())
            {
              result_or_error_.template assign <error_type> (that.result_or_error_.template get <error_type> ());
            }
            else
            {
              result_or_error_.template destroy <error_type> ();
              result_or_error_.template construct <result_type> (that.result_or_error_.template get <result_type> ());
            }
          }
          else
          {
            if (that.isError_Unchecked_ ())
            {
              result_or_error_.template destroy <result_type> ();
              result_or_error_.template construct <error_type> (that.result_or_error_.template get <error_type> ());
            }
            else
            {
              result_or_error_.template assign <result_type> (that.result_or_error_.template get <result_type> ());
            }
          }

          is_error_ = that.is_error_;
#ifdef RESULT_WITH_RUNTIME_CHECKS
          error_was_checked_ = that.error_was_checked_;
#endif // RESULT_WITH_RUNTIME_CHECKS
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
          if (isError_Unchecked_ ())
          {
            if (that.isError_Unchecked_ ())
            {
              result_or_error_.template assign <error_type> (
                std::move (that.result_or_error_.template get <error_type> ())
              );
            }
            else
            {
              result_or_error_.template destroy <error_type> ();
              result_or_error_.template construct <result_type> (
                std::move (that.result_or_error_.template get <result_type> ())
              );
            }
          }
          else
          {
            if (that.isError_Unchecked_ ())
            {
              result_or_error_.template destroy <result_type> ();
              result_or_error_.template construct <error_type> (
                std::move (that.result_or_error_.template get <error_type> ())
              );
            }
            else
            {
              result_or_error_.template assign <result_type> (
                std::move (that.result_or_error_.template get <result_type> ())
              );
            }
          }

          is_error_ = that.is_error_;
#ifdef RESULT_WITH_RUNTIME_CHECKS
          error_was_checked_ = that.error_was_checked_;
#endif // RESULT_WITH_RUNTIME_CHECKS
        }

        return *this;
      }


      /**
       * @brief
       * @return
       */
      const result_type &
      operator * (void) const
      {
        return result ();
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
        output << "Result{";
        if (self.isError_Unchecked_ ())
        {
          output << "error:" << self.error_Unchecked_ ();
        }
        else
        {
          output << "result:" << self.result_Unchecked_ ();
        }
        output << '}';

        return output;
      }


      /*
       * NOTE: [1] We can make `make*' functions either:
       * templated in-class static functions with template parameters like `typename ... TArgs',
       * or freestanding friend functions with template parameters like
       *   `typename result_type, typename error_type, typename ... TArgs'.
       * The `make*' functions were made static methods, because otherwise we would have
       * to specify `result_type' and `error_type' template parameters manually each time we call a `make*' function.
       */
      template <typename ... TArgs>
      static self_type
      makeResult (TArgs && ... args)
      {
        self_type new_result;
        new_result.result_or_error_.template construct <result_type> (std::forward <TArgs> (args) ...);
        new_result.isError_ (false);

        return new_result;
      }


      /**
       * @brief
       * @tparam TArgs
       * @param args
       * @return
       */
      template <typename ... TArgs>
      static self_type
      makeError (TArgs && ... args)
      {
        self_type new_error;
        new_error.result_or_error_.template construct <error_type> (std::forward <TArgs> (args) ...);
        new_error.isError_ (true);

        return new_error;
      }


    private:
      /**
       * @brief
       * @return
       */
      const result_type &
      result_Unchecked_ (void) const
      {
        return result_or_error_.template get <result_type> ();
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      error_Unchecked_ (void) const
      {
        return result_or_error_.template get <error_type> ();
      }


      /**
       * @brief
       * @param is_error
       */
      void
      isError_ (bool is_error)
      {
        is_error_ = is_error;
      }


      /**
       * @brief
       * @return
       */
      bool
      isError_Unchecked_ (void) const
      {
        return is_error_;
      }


#ifdef RESULT_WITH_RUNTIME_CHECKS
      /**
       * @brief
       * @return
       */
      bool
      errorWasChecked_ (void) const
      {
        return error_was_checked_;
      }


      /**
       * @brief
       * @param error_was_checked
       */
      void
      errorWasChecked_ (bool error_was_checked) const
      {
        error_was_checked_ = error_was_checked;
      }
#endif // RESULT_WITH_RUNTIME_CHECKS


      void
      initialize_ (void)
      {
#ifdef RESULT_WITH_RUNTIME_CHECKS
        errorWasChecked_ (false);
#endif // RESULT_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       */
      AlignedUnionStorageManager <result_type, error_type> result_or_error_;

      /**
       * @brief
       */
      bool is_error_;

#ifdef RESULT_WITH_RUNTIME_CHECKS
      /**
       * @brief
       */
      mutable bool error_was_checked_;
#endif // RESULT_WITH_RUNTIME_CHECKS
  };
}


#endif // UTILS_MISC_RESULT_HXX
