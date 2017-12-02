#ifndef UTILS_CONFIG_ISCLOSE_HXX
#define UTILS_CONFIG_ISCLOSE_HXX


namespace Config::Utils::IsClose
{
  template <typename TType>
  constexpr TType Relative_tolerance { 1e-8l };

  template <typename TType>
  constexpr TType Absolute_tolerance { 1e-16l };
}


#endif // UTILS_CONFIG_ISCLOSE_HXX
