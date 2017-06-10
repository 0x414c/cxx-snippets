#ifndef UTILS_DEBUG_CHECK_HXX
#define UTILS_DEBUG_CHECK_HXX


#include "../logging/logger.hxx" // ::Utils::Logger::printLog_Detailed
#include "../logging/source-location.hxx" // ::Utils::SourceLocation
#include "../preproc/stringify.hxx" // STRINGIFY_E


#ifdef WITH_CHECKS
  /**
   * @brief
   */
  #define CHECK(condition, message) \
    do \
    { \
      if (!(condition)) \
      { \
        ::Utils::Logger::printLog_Detailed ( \
          ::Utils::SourceLocation (__PRETTY_FUNCTION__, __FILE__, __LINE__), \
          ::Config::Utils::Logger::Check_prefix, \
          "Check `{0}' failed: `{1}'", (STRINGIFY_E (condition)), (message) \
        ); \
      } \
    } \
    while (false)
#else // WITH_CHECKS
  #define CHECK(condition, message) do { } while (false)
#endif // WITH_CHECKS


#endif // UTILS_DEBUG_CHECK_HXX
