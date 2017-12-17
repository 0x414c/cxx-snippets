#ifndef UTILS_DEBUG_ASSERT_HXX
#define UTILS_DEBUG_ASSERT_HXX


#include "../logging/source-location.hxx" // CURRENT_SOURCE_LOCATION
#include "../preproc/stringify.hxx" // STRINGIFY_E
#include "assertion-guard.hxx" // AssertionGuard
#include "../type-traits/always-false.hxx" // ::Utils::AlwaysFalse


#ifdef WITH_ASSERTS
#ifdef __GNUG__
/**
 * @brief
 */
#define ASSERT_CX(condition, message) \
  do \
  { \
    if (__builtin_constant_p ((condition))) \
    { \
      static_assert (::Utils::AlwaysFalse <bool, __builtin_constant_p ((condition))>::value, message); \
    } \
  } while (false)
#else // __GNUG__
#define ASSERT_CX(condition, message) do { } while (false)
#endif // __GNUG__


/**
 * @brief
 */
#define ASSERT(condition, message) \
  do \
  { \
    ASSERT_CX ((condition), message); \
    constexpr ::Utils::AssertionGuard assertion_guard ((message), (CURRENT_SOURCE_LOCATION ())); \
    assertion_guard.require ((condition), (STRINGIFY_E (condition))); \
  } \
  while (false)
#else // WITH_ASSERTS
#define ASSERT(condition, message) do { } while (false)
#endif // WITH_ASSERTS


#endif // UTILS_DEBUG_ASSERT_HXX
