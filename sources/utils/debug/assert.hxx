#ifndef UTILS_DEBUG_ASSERT_HXX
#define UTILS_DEBUG_ASSERT_HXX


#include <cstdlib> // ::std::abort

#include "../config/logger.hxx" // ::Config::Utils::Logger::Assert_prefix
#include "../logging/logger.hxx" // ::Utils::Logger::printLog_Detailed
#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E


#ifdef WITH_ASSERTS
/**
 * @brief
 */
#define ASSERT(condition, message) \
  do \
  { \
    if (!(condition)) \
    { \
      ::Utils::Logger::printLog_Detailed ( \
        CURRENT_SOURCE_LOCATION (), \
        ::Config::Utils::Logger::Assert_prefix, \
        "Assertion `{0}' failed: `{1}'", (STRINGIFY_E (condition)), (message) \
      ); \
      ::std::abort (); \
    } \
  } \
  while (false)
#else // WITH_ASSERTS
#define ASSERT(condition, message) do { } while (false)
#endif // WITH_ASSERTS


#endif // UTILS_DEBUG_ASSERT_HXX
