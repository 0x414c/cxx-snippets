#ifndef UTILS_DEBUG_FATAL_HXX
#define UTILS_DEBUG_FATAL_HXX


#include "crash-program.hxx"  // ::Utils::crashProgram
#include "source-location.hxx"  // CURRENT_SOURCE_LOCATION


#define FATAL_L(message) \
  do \
  { \
    ::Utils::crashProgram ((message), (CURRENT_SOURCE_LOCATION ())); \
  } \
  while (false)


#define FATAL_NL(message) \
  do \
  { \
    ::Utils::crashProgram ((message)); \
  } \
  while (false)


#endif  // UTILS_DEBUG_FATAL_HXX
