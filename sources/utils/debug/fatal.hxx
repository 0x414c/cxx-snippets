#ifndef UTILS_DEBUG_FATAL_HXX
#define UTILS_DEBUG_FATAL_HXX


#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION
#include "assertion-guard.hxx" // AssertionGuard


#define FATAL_L(message) \
  do \
  { \
    constexpr ::Utils::AssertionGuard assertion_guard ((message), (CURRENT_SOURCE_LOCATION ())); \
    assertion_guard.crash (true); \
  } \
  while (false)


#define FATAL_NL(message) \
  do \
  { \
    constexpr ::Utils::AssertionGuard assertion_guard ((message)); \
    assertion_guard.crash (false); \
  } \
  while (false)


#endif // UTILS_DEBUG_FATAL_HXX
