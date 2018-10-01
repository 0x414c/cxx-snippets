#ifndef UTILS_CONFIG_LOGGER_HXX
#define UTILS_CONFIG_LOGGER_HXX


#include "../containers/c-string.hxx"  // CString


namespace Config::Utils::Logger
{
  inline constexpr ::Utils::CString Default_prefix ("L");

  inline constexpr ::Utils::CString Check_prefix ("C");

  inline constexpr ::Utils::CString Assert_prefix ("A");

  inline constexpr ::Utils::CString Fatal_prefix ("F");

  inline constexpr ::Utils::CString Detailed_prefix ("D");

  inline constexpr ::Utils::CString Value_prefix ("V");

  inline constexpr ::Utils::CString Func_prefix ("I");

  inline constexpr ::Utils::CString Timer_prefix ("T");
}


#endif  // UTILS_CONFIG_LOGGER_HXX
