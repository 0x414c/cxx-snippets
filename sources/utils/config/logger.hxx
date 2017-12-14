#ifndef UTILS_CONFIG_LOGGER_HXX
#define UTILS_CONFIG_LOGGER_HXX


namespace Config::Utils::Logger
{
  constexpr const char Detailed_log_prefix[] ("D");
  constexpr const char Value_log_prefix[] ("V");
  constexpr const char Func_log_prefix[] ("I");

  constexpr const char Short_log_prefix[] ("L");

  constexpr const char Check_prefix[] ("C");
  constexpr const char Assert_prefix[] ("A");
  constexpr const char Fatal_prefix[] ("F");

  constexpr const char Timer_log_prefix[] ("T");
}


#endif // UTILS_CONFIG_LOGGER_HXX
