#ifndef UTILS_CONFIG_RATIONALIZE_HXX
#define UTILS_CONFIG_RATIONALIZE_HXX


#include <cstddef> // std::size_t


namespace Config::Utils::Rationalize
{
  template <typename TType>
  constexpr TType Max_denominator { 1'000'000 };

  constexpr std::size_t Max_iterations (23);
}


#endif // UTILS_CONFIG_RATIONALIZE_HXX
