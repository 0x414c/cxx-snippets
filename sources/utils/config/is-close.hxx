#ifndef UTILS_CONFIG_ISCLOSE_HXX
#define UTILS_CONFIG_ISCLOSE_HXX


namespace Config::Utils::IsClose
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
  template <typename TFloatingPoint>
  inline constexpr TFloatingPoint Absolute_tolerance { 1e-16l };

  template <typename TFloatingPoint>
  inline constexpr TFloatingPoint Relative_tolerance { 1e-8l };
#pragma GCC diagnostic pop
}


#endif  // UTILS_CONFIG_ISCLOSE_HXX
