#ifndef UTILS_MISC_EXPECTED_HXX
#define UTILS_MISC_EXPECTED_HXX


#include <ostream> // std::ostream
#include <memory> // std::addressof
#include <utility> // std::{forward, move}

#include "../debug/fatal.hxx" // FATAL_NL
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
  constexpr ErrorTag Error { };


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
        was_checked_ (false)
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

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @param that
       */
      Expected (self_type && that) :
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        is_result_ (that.is_result_),
        was_checked_ (false)
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
      const error_type &
      error (void) const
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        if (wasChecked_ ())
        {
          if (isResult_Unchecked_ ())
          {
            FATAL_NL ("Cannot access error because this instance of `Expected' was initialized as a result");
          }
          else
          {
            return error_Unchecked_ ();
          }
        }
        else
        {
          FATAL_NL ("`Expected' should be checked for being an error before accessing error");
        }
#else // EXPECTED_WITH_RUNTIME_CHECKS
        if (isResult_Unchecked_ ())
        {
          FATAL_NL ("Cannot access error because this instance of `Expected' was initialized as a result");
        }
        else
        {
          return error_Unchecked_ ();
        }
#endif // EXPECTED_WITH_RUNTIME_CHECKS
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] explicit operator bool (void) const noexcept
      {
        return isResult_ ();
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
          if (isResult_Unchecked_ ())
          {
            if (that.isResult_Unchecked_ ())
            {
              result_or_error_.template assign <result_type> (that.result_or_error_.template get <result_type> ());
            }
            else
            {
              result_or_error_.template destroy <result_type> ();
              result_or_error_.template construct <error_type> (that.result_or_error_.template get <error_type> ());
            }
          }
          else
          {
            if (that.isResult_Unchecked_ ())
            {
              result_or_error_.template destroy <error_type> ();
              result_or_error_.template construct <result_type> (that.result_or_error_.template get <result_type> ());
            }
            else
            {
              result_or_error_.template assign <error_type> (that.result_or_error_.template get <error_type> ());
            }
          }

          is_result_ = that.is_result_;

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.was_checked_ = true;
          was_checked_ = false;
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
          if (isResult_Unchecked_ ())
          {
            if (that.isResult_Unchecked_ ())
            {
              result_or_error_.template assign <result_type> (
                std::move (that.result_or_error_.template get <result_type> ())
              );
            }
            else
            {
              result_or_error_.template destroy <result_type> ();
              result_or_error_.template construct <error_type> (
                std::move (that.result_or_error_.template get <error_type> ())
              );
            }
          }
          else
          {
            if (that.isResult_Unchecked_ ())
            {
              result_or_error_.template destroy <error_type> ();
              result_or_error_.template construct <result_type> (
                std::move (that.result_or_error_.template get <result_type> ())
              );
            }
            else
            {
              result_or_error_.template assign <error_type> (
                std::move (that.result_or_error_.template get <error_type> ())
              );
            }
          }

          is_result_ = that.is_result_;

#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.was_checked_ = true;
          was_checked_ = false;
#endif // EXPECTED_WITH_RUNTIME_CHECKS
        }

        return *this;
      }


      /**
       * @brief
       * @param that
       * @return Earliest error or latest result
       */
      template <typename TThat>
      self_type
      operator & (TThat && that) const &
      {
        if (isResult_Unchecked_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::forward <TThat> (that);
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return *this;
        }
      }


      /**
       * @brief
       * @param that
       * @return Earliest error or latest result
       */
      template <typename TThat>
      self_type &&
      operator & (TThat && that) &&
      {
        if (isResult_Unchecked_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::forward <TThat> (that);
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::move (*this);
        }
      }


      /**
       * @brief
       * @param that
       * @return Latest error or earliest result
       */
      template <typename TThat>
      self_type
      operator | (TThat && that) const &
      {
        if (that.isResult_Unchecked_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return *this;
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::forward <TThat> (that);
        }
      }


      /**
       * @brief
       * @param that
       * @return Latest error or earliest result
       */
      template <typename TThat>
      self_type &&
      operator | (TThat && that) &&
      {
        if (that.isResult_Unchecked_ ())
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          that.wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::move (*this);
        }
        else
        {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
          wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

          return std::forward <TThat> (that);
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
      result_type
      operator || (TFallback && fallback) const &
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        if (isResult_Unchecked_ ())
        {
          return result_Unchecked_ ();
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
      result_type &&
      operator || (TFallback && fallback) &&
      {
#ifdef EXPECTED_WITH_RUNTIME_CHECKS
        wasChecked_ (true);
#endif // EXPECTED_WITH_RUNTIME_CHECKS

        if (isResult_Unchecked_ ())
        {
          return std::move (result_Unchecked_ ());
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
        return result_ ();
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
      result_ (void) const
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
            FATAL_NL ("Cannot access result because this instance of `Expected' was initialized as an error");
          }
        }
        else
        {
          FATAL_NL ("`Expected' should be checked for being an error before accessing result");
        }
#else // EXPECTED_WITH_RUNTIME_CHECKS
        if (isResult_Unchecked_ ())
        {
          return result_Unchecked_ ();
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
      const result_type &
      result_Unchecked_ (void) const
      {
        return result_or_error_.template get <result_type> ();
      }


      /**
       * @brief
       * @return
       */
      result_type &
      result_Unchecked_ (void)
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
      isResult_ (void) const noexcept
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
      isResult_Unchecked_ (void) const noexcept
      {
        return is_result_;
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
      mutable bool was_checked_ { };
#endif // EXPECTED_WITH_RUNTIME_CHECKS
  };
}


#endif // UTILS_MISC_EXPECTED_HXX
