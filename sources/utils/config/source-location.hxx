#ifndef UTILS_CONFIG_SOURCELOCATION_HXX
#define UTILS_CONFIG_SOURCELOCATION_HXX


#include "../containers/c-string.hxx" // CString


namespace Config::Utils::SourceLocation
{
  inline constexpr ::Utils::CString Default_function ("(unknown)");
  inline constexpr ::Utils::CString Default_file ("(unknown)");
}


#endif // UTILS_CONFIG_SOURCELOCATION_HXX
