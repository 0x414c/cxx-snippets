#ifndef UTILS_ALGORITHMS_LERP_HXX
#define UTILS_ALGORITHMS_LERP_HXX


#include <type_traits> // std::is_arithmetic_v


namespace Utils
{
  /**
   * @brief
   * @tparam TX
   * @tparam TY
   * @param x
   * @param x_0
   * @param x_1
   * @param y_0
   * @param y_1
   * @return
   */
  template <typename TX, typename TY = TX>
  constexpr TY
  lerp (TX x, TX x_0, TX x_1, TY y_0, TY y_1)
  {
    static_assert (std::is_arithmetic_v <TX>);
    static_assert (std::is_arithmetic_v <TY>);

    return (y_0 + (x - x_0) * (y_1 - y_0) / (x_1 - x_0));
  }
}


#endif // UTILS_ALGORITHMS_LERP_HXX
