#ifndef UTILS_DEBUG_ASSERT_HXX
#define UTILS_DEBUG_ASSERT_HXX


#include <cstdlib> // std::abort

//#include <string_view> // std::string_view

#include "../config/logger.hxx" // Config::Utils::Logger::Assert_prefix
#include "../containers/c-string.hxx" // CString
#include "../logging/logger.hxx" // Logger::printLog_Detailed
#include "../logging/source-location.hxx" // SourceLocation, CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E


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
      test (bool condition, const CString & condition_as_text) const
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


#ifdef WITH_ASSERTS
/**
 * @brief
 */
#define ASSERT(condition, message) \
  do \
  { \
    constexpr ::Utils::AssertionGuard assertion_guard ((message), (CURRENT_SOURCE_LOCATION ())); \
    assertion_guard.test ((condition), (STRINGIFY_E (condition))); \
  } \
  while (false)
#else // WITH_ASSERTS
#define ASSERT(condition, message) do { } while (false)
#endif // WITH_ASSERTS


#endif // UTILS_DEBUG_ASSERT_HXX
