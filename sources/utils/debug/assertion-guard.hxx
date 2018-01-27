#ifndef UTILS_DEBUG_ASSERTIONGUARD_HXX
#define UTILS_DEBUG_ASSERTIONGUARD_HXX


#include "../config/logger.hxx" // Config::Utils::Logger::{Assert_prefix, Check_prefix, Fatal_prefix}
#include "../containers/c-string.hxx" // CString
#include "../logging/logger.hxx" // Logger::log_Detailed
#include "source-location.hxx" // SourceLocation


namespace Utils
{
  /**
   * @brief
   */
  class AssertionGuard final
  {
    private:
      /**
       * @brief
       */
      using self_type = AssertionGuard;


    public:
      /**
       * @brief
       */
      constexpr AssertionGuard (void) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr AssertionGuard (const self_type & that [[maybe_unused]]) noexcept = delete;


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
       * @param message
       * @param source_location
       */
      constexpr explicit AssertionGuard (const CString & message, const SourceLocation & source_location) noexcept :
        message_ (message),
        source_location_ (source_location)
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
          assertionFailure_ ();

          Logger::log_Detailed (
            source_location_, Config::Utils::Logger::Assert_prefix,
            "Assertion `{0:s}' failed: `{1:s}'", condition_as_text, message_
          );

          crashProgram_ ();
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
          assertionFailure_ ();

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

        crashProgram_ ();
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

      [[noreturn]] void crashProgram_ (void) const noexcept;

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
