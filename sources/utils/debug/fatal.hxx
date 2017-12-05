#ifndef UTILS_DEBUG_FATAL_HXX
#define UTILS_DEBUG_FATAL_HXX


#include <cstdlib> // ::std::abort

#include "../config/logger.hxx" // ::Config::Utils::Logger::Fatal_prefix
#include "../logging/logger.hxx" // ::Utils::Logger::printLog_Detailed
#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION


#define FATAL(message) \
  do \
  { \
    ::Utils::Logger::printLog_Detailed ( \
      CURRENT_SOURCE_LOCATION (), \
      ::Config::Utils::Logger::Fatal_prefix, \
      "Fatal error: `{0}'", (message) \
    ); \
    ::std::abort (); \
  } \
  while (false)


#endif // UTILS_DEBUG_FATAL_HXX
