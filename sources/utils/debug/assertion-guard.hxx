#ifndef UTILS_DEBUG_ASSERTIONGUARD_HXX
#define UTILS_DEBUG_ASSERTIONGUARD_HXX


#include <cstdlib> // std::abort

#include "../config/logger.hxx" // Config::Utils::Logger::{Assert, Check, Fatal}_prefix
#include "../containers/c-string.hxx" // CString
#include "../logging/logger.hxx" // Logger::log_Detailed
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
       * @param message
       * @param source_location
       */
      constexpr explicit AssertionGuard (const CString & message, const SourceLocation & source_location) noexcept :
        message_ (message),
        source_location_ (source_location)
      { }


      /**
       * @brief
       * @param message
       */
      constexpr explicit AssertionGuard (const CString & message) noexcept :
        message_ (message),
        source_location_ ()
      { }


      /**
       * @brief
       * @param condition
       * @param condition_as_text
       * @return
       */
      constexpr void
      require (bool condition, const CString & condition_as_text) const
      {
        if (!condition)
        {
          Logger::log_Detailed (
            source_location_, Config::Utils::Logger::Assert_prefix,
            "Assertion `{0:s}' failed: `{1:s}'", condition_as_text, message_
          );

          std::abort ();
        }
      }


      /**
       * @brief
       * @param condition
       * @param condition_as_text
       * @return
       */
      constexpr void
      check (bool condition, const CString & condition_as_text) const
      {
        if (!condition)
        {
          Logger::log_Detailed (
            source_location_, Config::Utils::Logger::Check_prefix,
            "Check `{0:s}' failed: `{1:s}'", condition_as_text, message_
          );
        }
      }


      /**
       * @brief
       * @param show_location
       */
      [[noreturn]] void
      crash (bool show_location) const
      {
        if (show_location)
        {
          Logger::log_Detailed (
            source_location_, Config::Utils::Logger::Fatal_prefix, "Fatal error: `{0:s}'", message_
          );
        }
        else
        {
          Logger::log (Config::Utils::Logger::Fatal_prefix, "Fatal error: `{0:s}'", message_);
        }

        std::abort ();
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
