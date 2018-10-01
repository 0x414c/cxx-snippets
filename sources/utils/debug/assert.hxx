#ifndef UTILS_DEBUG_ASSERT_HXX
#define UTILS_DEBUG_ASSERT_HXX


#include "../preproc/stringify.hxx"  // STRINGIFY_E
#include "assertion.hxx"  // ::Utils::Assertion
#include "condition.hxx"  // CONDITION


#ifdef WITH_ASSERTS
/**
 * @brief
 */
#define ASSERT(condition, message) \
  do \
  { \
    ::Utils::Assertion ((CONDITION ((condition)))).require ((message)); \
  } while (false)
#else  // WITH_ASSERTS
#define ASSERT(condition, message) do { } while (false)
#endif  // WITH_ASSERTS


#endif  // UTILS_DEBUG_ASSERT_HXX
