#ifndef UTILS_DEBUG_CHECK_HXX
#define UTILS_DEBUG_CHECK_HXX


#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E
#include "assertion-guard.hxx" // AssertionGuard


#ifdef WITH_CHECKS
/**
 * @brief
 */
#define CHECK(condition, message) \
  do \
  { \
    constexpr ::Utils::AssertionGuard assertion_guard ((message), (CURRENT_SOURCE_LOCATION ())); \
    assertion_guard.check ((condition), (STRINGIFY_E (condition))); \
  } \
  while (false)
#else // WITH_CHECKS
#define CHECK(condition, message) do { } while (false)
#endif // WITH_CHECKS


#endif // UTILS_DEBUG_CHECK_HXX
