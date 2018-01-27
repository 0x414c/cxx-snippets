#ifndef UTILS_DEBUG_CHECK_HXX
#define UTILS_DEBUG_CHECK_HXX


#include "../preproc/stringify.hxx" // STRINGIFY_E
#include "assertion.hxx" // Assertion
#include "condition.hxx" // CONDITION


#ifdef WITH_CHECKS
/**
 * @brief
 */
#define CHECK(condition, message) \
  do \
  { \
    ::Utils::Assertion ((CONDITION ((condition)))).check ((message)); \
  } \
  while (false)
#else // WITH_CHECKS
#define CHECK(condition, message) do { } while (false)
#endif // WITH_CHECKS


#endif // UTILS_DEBUG_CHECK_HXX
