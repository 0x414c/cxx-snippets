#ifndef UTILS_DEBUG_ASSERT_HXX
#define UTILS_DEBUG_ASSERT_HXX


#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E
#include "assertion-guard.hxx" // AssertionGuard


#ifdef WITH_ASSERTS
/**
 * @brief
 */
#define ASSERT(condition, message) \
  do \
  { \
    constexpr ::Utils::AssertionGuard assertion_guard ((message), (CURRENT_SOURCE_LOCATION ())); \
    assertion_guard.require ((condition), (STRINGIFY_E (condition))); \
  } \
  while (false)
#else // WITH_ASSERTS
#define ASSERT(condition, message) do { } while (false)
#endif // WITH_ASSERTS


#endif // UTILS_DEBUG_ASSERT_HXX
