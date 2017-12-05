#ifndef UTILS_CONFIG_ISCLOSE_HXX
#define UTILS_CONFIG_ISCLOSE_HXX


namespace Config::Utils::IsClose
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
  template <typename TType>
  constexpr TType Relative_tolerance { 1e-8l };

  template <typename TType>
  constexpr TType Absolute_tolerance { 1e-16l };
#pragma GCC diagnostic pop
}


#endif // UTILS_CONFIG_ISCLOSE_HXX
