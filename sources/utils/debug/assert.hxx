#ifndef UTILS_DEBUG_ASSERT_HXX
#define UTILS_DEBUG_ASSERT_HXX


#include <cstdlib> // ::std::exit, EXIT_FAILURE

#include "../logging/logger.hxx" // ::Utils::Logger::printLog_Detailed
#include "../logging/source-location.hxx" // ::Utils::SourceLocation
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
          ::Utils::SourceLocation (__PRETTY_FUNCTION__, __FILE__, __LINE__), \
          ::Config::Utils::Logger::Assert_prefix, \
          "Assertion `{0}' failed: `{1}'", (STRINGIFY_E (condition)), (message) \
        ); \
        ::std::exit (EXIT_FAILURE); \
      } \
    } \
    while (false)
#else // WITH_ASSERTS
  #define ASSERT(condition, message) do { } while (false)
#endif // WITH_ASSERTS


#endif // UTILS_DEBUG_ASSERT_HXX
