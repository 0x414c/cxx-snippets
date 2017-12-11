#ifndef UTILS_DEBUG_ASSERTIONGUARD_HXX
#define UTILS_DEBUG_ASSERTIONGUARD_HXX


#include <cstdlib> // std::abort

#include "../config/logger.hxx" // Config::Utils::Logger::{Assert, Check}_prefix
#include "../containers/c-string.hxx" // CString
#include "../logging/logger.hxx" // Logger::printLog_Detailed
#include "../logging/source-location.hxx" // SourceLocation


namespace Utils
{
  /**
   * @brief
   */
  class AssertionGuard final
  {
    public:
      /**
       * @brief
       */
      using self_type = AssertionGuard;


      /**
       * @brief
       */
      constexpr AssertionGuard (void) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr AssertionGuard (const self_type & that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       */
      constexpr explicit AssertionGuard (const CString & message, const SourceLocation & source_location) noexcept :
        message_ (message),
        source_location_ (source_location)
      { }


      /**
       * @brief
       */
      constexpr void
      require (bool condition, const CString & condition_as_text) const
      {
        if (!condition)
        {
          Logger::printLog_Detailed (
            source_location_, Config::Utils::Logger::Assert_prefix,
            "Assertion `{0}' failed: `{1}'", condition_as_text, message_
          );

          std::abort ();
        }
      }


      /**
       * @brief
       */
      constexpr void
      check (bool condition, const CString & condition_as_text) const
      {
        if (!condition)
        {
          Logger::printLog_Detailed (
            source_location_, Config::Utils::Logger::Check_prefix,
            "Check `{0}' failed: `{1}'", condition_as_text, message_
          );
        }
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr const self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = delete;


    private:
      /**
       * @brief
       */
      const CString message_;

      /**
       * @brief
       */
      const SourceLocation source_location_;
  };
}


#endif // UTILS_DEBUG_ASSERTIONGUARD_HXX
