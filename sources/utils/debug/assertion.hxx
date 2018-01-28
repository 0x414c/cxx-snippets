#ifndef UTILS_DEBUG_ASSERTION_HXX
#define UTILS_DEBUG_ASSERTION_HXX


#include "../config/logger.hxx" // Config::Utils::Logger::{Assert_prefix, Check_prefix, Fatal_prefix}
#include "../containers/c-string.hxx" // CString
#include "../logging/logger.hxx" // Logger::log_Detailed
#include "condition.hxx" // Condition


namespace Utils
{
  /**
   * @brief
   */
  class Assertion final
  {
    private:
      /**
       * @brief
       */
      using self_type = Assertion;


    public:
      /**
       * @brief
       */
      constexpr Assertion (void) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr Assertion (const self_type & that [[maybe_unused]]) noexcept = delete;


      /**
       * @brief
       * @param condition
       */
      constexpr explicit Assertion (const Condition & condition) noexcept :
        condition_ (condition)
      { }


      /**
       * @brief
       * @param message
       * @return
       */
      constexpr void
      require (const CString & message) const
      {
        if (!condition_)
        {
          assertionFailure_ ();

          Logger::log_Detailed (
            condition_.sourceLocation (), Config::Utils::Logger::Assert_prefix,
            "Assertion `{0:s}' failed: `{1:s}'", condition_.source (), message
          );

          crashProgram_ ();
        }
      }


      /**
       * @brief
       * @param message
       * @return
       */
      constexpr void
      check (const CString & message) const
      {
        if (!condition_)
        {
          assertionFailure_ ();

          Logger::log_Detailed (
            condition_.sourceLocation (), Config::Utils::Logger::Check_prefix,
            "Check `{0:s}' failed: `{1:s}'", condition_.source (), message
          );
        }
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = delete;


    private:
      void
      assertionFailure_ (void) const noexcept;

      [[noreturn]] void
      crashProgram_ (void) const noexcept;

      /**
       * @brief
       */
      const Condition condition_;
  };
}


#endif // UTILS_DEBUG_ASSERTION_HXX
