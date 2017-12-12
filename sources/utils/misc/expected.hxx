#ifndef UTILS_MISC_EXPECTED_HXX
#define UTILS_MISC_EXPECTED_HXX


#include <ostream> // std::ostream
#include <utility> // std::{forward, move}

#include "../debug/fatal.hxx" // FATAL
#include "../type-traits/aligned-union-storage-manager.hxx" // AlignedUnionStorageManager
#include "operation-status.hxx" // OperationStatus


namespace Utils
{
  /**
   * @brief
   */
  struct ErrorTag
  { };


  /**
   * @brief
   */
  constexpr ErrorTag Error_tag { };


  /**
   * @brief
   * @tparam TResult
   * @tparam TError
   */
  template <typename TResult, typename TError = OperationStatus>
  class [[nodiscard]] Expected final
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
      using self_type = Expected;


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
      Expected (TArgs && ... args) :
        is_result_ (true)
      {
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
        result_or_error_.template construct <error_type> (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @param that
       */
      Expected (const self_type & that) :
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        is_result_ (that.is_result_),
        was_checked_ (that.was_checked_)
#else // EXPECTED_WITH_RUNTIME_CHECKS
        is_error_ (that.is_error_)
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      {
        if (that.isResult_Unchecked_ ())
        {
          result_or_error_.template construct <result_type> (that.result_or_error_.template get <result_type> ());
        }
        else
        {
          result_or_error_.template construct <error_type> (that.result_or_error_.template get <error_type> ());
        }
      }


      /**
       * @brief
       * @param that
       */
      Expected (self_type && that) :
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        is_result_ (that.is_result_),
        was_checked_ (that.was_checked_)
#else // EXPECTED_WITH_RUNTIME_CHECKS
        is_error_ (that.is_error_)
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
        if (that.isResult_Unchecked_ ())
#pragma GCC diagnostic pop
        {
          result_or_error_.template construct <result_type> (
            std::move (that.result_or_error_.template get <result_type> ())
          );
        }
        else
        {
          result_or_error_.template construct <error_type> (
            std::move (that.result_or_error_.template get <error_type> ())
          );
        }
      }


      /**
       * @brief
       */
      ~Expected (void)
      {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wduplicated-branches"
        if (isResult_Unchecked_ ())
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
      const result_type &
      result (void) const
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        if (wasChecked_ ())
        {
          if (isResult_Unchecked_ ())
          {
            return result_Unchecked_ ();
          }
          else
          {
            FATAL ("Cannot access result because this instance of `Expected' was initialized as an error.");
          }
        }
        else
        {
          FATAL ("`Expected' should be checked for being an error before calling `result ()'.");
        }
#else // EXPECTED_WITH_RUNTIME_CHECKS
        return result_Unchecked_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @return
       */
      const error_type &
      error (void) const
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        if (wasChecked_ ())
        {
          if (!isResult_Unchecked_ ())
          {
            return error_Unchecked_ ();
          }
          else
          {
            FATAL ("Cannot access error because this instance of `Expected' was initialized as a result.");
          }
        }
        else
        {
          FATAL ("`Expected' should be checked for being an error before calling `error ()'.");
        }
#else // EXPECTED_WITH_RUNTIME_CHECKS
        return error_Unchecked_ ();
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] explicit operator bool (void) const
      {
        return isResult_ ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (const self_type & that [[maybe_unused]]) = delete;

      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (self_type && that [[maybe_unused]]) = delete;


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
       * @return
       */
      const result_type *
      operator -> (void) const
      {
        return &(operator * ());
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
        if (self.isResult_Unchecked_ ())
        {
          output << "result:" << self.result_Unchecked_ ();
        }
        else
        {
          output << "error:" << self.error_Unchecked_ ();
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
       * @return
       */
      [[nodiscard]] bool
      isResult_ (void) const
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        return isResult_Unchecked_ ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] bool
      isResult_Unchecked_ (void) const
      {
        return is_result_;
      }


#ifdef EXPECTED_WITH_RUNTIME_CHECKS
      /**
       * @brief
       * @return
       */
      bool
      wasChecked_ (void) const
      {
        return was_checked_;
      }


      /**
       * @brief
       * @param was_checked
       */
      void
      wasChecked_ (bool was_checked) const
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
