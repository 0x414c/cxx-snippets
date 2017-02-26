#ifndef UTILS_MISC_RESULT_HXX
#define UTILS_MISC_RESULT_HXX


#include <cstddef> // std::size_t

#include <algorithm> // std::max
#include <iostream> // std::{clog, endl}
#include <ostream> // std::ostream
#ifdef RESULT_WITH_EXCEPTIONS
#include <stdexcept> // std::runtime_error
#endif // RESULT_WITH_EXCEPTIONS
#include <utility> // std::{forward, move}

#include "../type-traits/aligned-union-storage-manager.hxx" // AlignedUnionStorageManager


namespace Utils
{
  /**
   * @brief
   */
  enum struct BasicError
  {
    Success, Fail, Nothing
  };


  /**
   * @brief
   * @tparam TResult
   * @tparam TError
   */
  template <typename TResult, typename TError = BasicError>
  class Result
  {
    public:
      /**
       * @brief
       */
      using self_type = Result;

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
      Result (void)
      {
        std::clog << "Result (void)" << std::endl;

        init_ ();
      }


      /**
       * @brief
       * @param that
       */
      Result (const self_type & that) :
#ifdef RESULT_WITH_EXCEPTIONS
      is_error_ (that.is_error_),
      error_was_checked_ (that.error_was_checked_)
#else // RESULT_WITH_EXCEPTIONS
      is_error_ (that.is_error_)
#endif // RESULT_WITH_EXCEPTIONS
      {
        std::clog << "Result (const self_type & that)" << std::endl;

        if (that.isErrorNoCheck_ ())
        {
          result_or_error_.template construct <error_type> (*that.result_or_error_.template get <error_type> ());
        }
        else
        {
          result_or_error_.template construct <result_type> (*that.result_or_error_.template get <result_type> ());
        }
      }


      /**
       * @brief
       * @param that
       */
      Result (self_type && that) :
#ifdef RESULT_WITH_EXCEPTIONS
        is_error_ (that.is_error_),
        error_was_checked_ (that.error_was_checked_)
#else // RESULT_WITH_EXCEPTIONS
      is_error_ (that.is_error_)
#endif // RESULT_WITH_EXCEPTIONS
      {
        std::clog << "Result (self_type && that)" << std::endl;

        if (that.isErrorNoCheck_ ())
        {
          result_or_error_.template construct <error_type> (
            std::move (*that.result_or_error_.template get <error_type> ())
          );
        }
        else
        {
          result_or_error_.template construct <result_type> (
            std::move (*that.result_or_error_.template get <result_type> ())
          );
        }
      }


      /**
       * @brief
       */
      ~Result (void)
      {
        std::clog << "~Result (void)" << std::endl;

        if (isErrorNoCheck_ ())
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
#ifdef RESULT_WITH_EXCEPTIONS
        if (errorWasChecked_ ())
        {
          if (!isErrorNoCheck_ ())
          {
            return resultNoCheck_ ();
          }
          else
          {
            throw std::runtime_error ("Cannot access result because Result was initialized as an error.");
          }
        }
        else
        {
          throw std::runtime_error ("Result should be checked for an error before calling `result ()'.");
        }
#else // RESULT_WITH_EXCEPTIONS
        return resultNoCheck_ ();
#endif // RESULT_WITH_EXCEPTIONS
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      error (void) const
      {
#ifdef RESULT_WITH_EXCEPTIONS
        if (errorWasChecked_ ())
        {
          if (isErrorNoCheck_ ())
          {
            return errorNoCheck_ ();
          }
          else
          {
            throw std::runtime_error ("Cannot access error because Result was initialized as a result.");
          }
        }
        else
        {
          throw std::runtime_error ("Result should be checked for an error before calling `error ()'.");
        }
#else // RESULT_WITH_EXCEPTIONS
        return errorNoCheck_ ();
#endif // RESULT_WITH_EXCEPTIONS
      }


      /**
       * @brief
       * @return
       */
      bool
      isError (void) const
      {
#ifdef RESULT_WITH_EXCEPTIONS
        errorWasChecked_ (true);
#endif // RESULT_WITH_EXCEPTIONS

        return isErrorNoCheck_ ();
      }


      /**
       * @brief
       * @return
       */
      explicit operator bool (void) const
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
        std::clog << "operator = (const self_type & that)" << std::endl;

        if (this != &that)
        {
          if (that.isErrorNoCheck_ ())
          {
            result_or_error_.template assign <error_type> (*that.result_or_error_.template get <error_type> ());
          }
          else
          {
            result_or_error_.template assign <result_type> (*that.result_or_error_.template get <result_type> ());
          }

          is_error_ = that.is_error_;
#ifdef RESULT_WITH_EXCEPTIONS
          error_was_checked_ = that.error_was_checked_;
#endif // RESULT_WITH_EXCEPTIONS
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
        std::clog << "operator = (self_type && that)" << std::endl;

        if (this != &that)
        {
          if (that.isErrorNoCheck_ ())
          {
            result_or_error_.template assign <error_type> (
              std::move (*that.result_or_error_.template get <error_type> ())
            );
          }
          else
          {
            result_or_error_.template assign <result_type> (
              std::move (*that.result_or_error_.template get <result_type> ())
            );
          }

          is_error_ = that.is_error_;
#ifdef RESULT_WITH_EXCEPTIONS
          error_was_checked_ = that.error_was_checked_;
#endif // RESULT_WITH_EXCEPTIONS
        }

        return *this;
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
        if (self.isErrorNoCheck_ ())
        {
          output << "error: " << self.errorNoCheck_ ();
        }
        else
        {
          output << "result: " << self.resultNoCheck_ ();
        }

        return output;
      }


      /*
       * NOTE: [1] We can make `make*' functions either:
       * templated in-class static functions with template parameters like `typename ... TArgs',
       * or freestanding friend functions with template parameters like
       *   `typename TResult, typename TError, typename ... TArgs'.
       * The `make*' functions were made static methods, because otherwise we would have
       * to specify `TResult' and `TError' template parameters manually each time we call a `make*' function.
       */
      template <typename ... TArgs>
      static self_type
      makeResult (TArgs && ... args)
      {
        std::clog << "makeResult (TArgs && ... args)" << std::endl;

        Result <TResult, TError> new_result;

        new_result.result_or_error_.template construct <TResult> (std::forward <TArgs> (args) ...);
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
        std::clog << "makeError (TArgs && ... args)" << std::endl;

        Result <TResult, TError> new_error;

        new_error.result_or_error_.template construct <TError> (std::forward <TArgs> (args) ...);
        new_error.isError_ (true);

        return new_error;
      }


    private:
      /**
       * @brief
       * @return
       */
      const result_type &
      resultNoCheck_ (void) const
      {
        return *result_or_error_.template get <result_type> ();
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      errorNoCheck_ (void) const
      {
        return *result_or_error_.template get <error_type> ();
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
      isErrorNoCheck_ (void) const
      {
        return is_error_;
      }


#ifdef RESULT_WITH_EXCEPTIONS
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
#endif // RESULT_WITH_EXCEPTIONS


      void
      init_ (void)
      {
#ifdef RESULT_WITH_EXCEPTIONS
        errorWasChecked_ (false);
#endif // RESULT_WITH_EXCEPTIONS
      }


      /**
       * @brief
       */
      AlignedUnionStorageManager <result_type, error_type> result_or_error_;

      /**
       * @brief
       */
      bool is_error_;

#ifdef RESULT_WITH_EXCEPTIONS
      /**
       * @brief
       */
      mutable bool error_was_checked_;
#endif // RESULT_WITH_EXCEPTIONS
  };
}


#endif // UTILS_MISC_RESULT_HXX
