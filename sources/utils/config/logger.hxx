#ifndef UTILS_CONFIG_LOGGER_HXX
#define UTILS_CONFIG_LOGGER_HXX


namespace Config::Utils::Logger
{
  inline constexpr const char Detailed_prefix[] ("D");
  inline constexpr const char Value_prefix[] ("V");
  inline constexpr const char Func_prefix[] ("I");

  inline constexpr const char Default_prefix[] ("L");

  inline constexpr const char Check_prefix[] ("C");
  inline constexpr const char Assert_prefix[] ("A");
  inline constexpr const char Fatal_prefix[] ("F");

  inline constexpr const char Timer_prefix[] ("T");
}


#endif // UTILS_CONFIG_LOGGER_HXX
