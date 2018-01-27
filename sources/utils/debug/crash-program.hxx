#ifndef UTILS_DEBUG_CRASHPROGRAM_HXX
#define UTILS_DEBUG_CRASHPROGRAM_HXX


#include "../containers/c-string.hxx" // CString
#include "source-location.hxx" // SourceLocation


namespace Utils
{
  [[noreturn]] void
  crashProgram (void) noexcept;

  [[noreturn]] void
  crashProgram (const CString & message) noexcept;

  [[noreturn]] void
  crashProgram (const CString & message, const SourceLocation & source_location) noexcept;
}


#endif // UTILS_DEBUG_CRASHPROGRAM_HXX
