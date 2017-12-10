#ifndef UTILS_DEBUG_CHECK_HXX
#define UTILS_DEBUG_CHECK_HXX


//#include <string_view> // std::string_view

#include "../config/logger.hxx" // Config::Utils::Logger::Check_prefix
#include "../containers/cstring.hxx" // CString
#include "../logging/logger.hxx" // Logger::printLog_Detailed
#include "../logging/source-location.hxx" // SourceLocation, CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E


namespace Utils
{
  /**
   * @brief
   */
  class CheckGuard final
  {
    public:
      /**
       * @brief
       */
      using self_type = CheckGuard;


      /**
       * @brief
       */
      constexpr CheckGuard (void) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr CheckGuard (const self_type & that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       */
      constexpr explicit CheckGuard (const CString & message, const SourceLocation & source_location) noexcept :
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


#ifdef WITH_CHECKS
/**
 * @brief
 */
#define CHECK(condition, message) \
  do \
  { \
    constexpr ::Utils::CheckGuard check_guard ((message), (CURRENT_SOURCE_LOCATION ())); \
    check_guard.test ((condition), (STRINGIFY_E (condition))); \
  } \
  while (false)
#else // WITH_CHECKS
#define CHECK(condition, message) do { } while (false)
#endif // WITH_CHECKS


#endif // UTILS_DEBUG_CHECK_HXX
