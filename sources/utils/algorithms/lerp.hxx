#ifndef UTILS_ALGORITHMS_LERP_HXX
#define UTILS_ALGORITHMS_LERP_HXX


#include "../debug/assert.hxx" // ASSERT


namespace Utils
{
  /**
   * @brief
   * @tparam TX
   * @tparam TY
   * @param x
   * @param x_min
   * @param x_max
   * @param y_min
   * @param y_max
   * @return
   */
  template <typename TX, typename TY>
  constexpr TY
  lerp (const TX & x, const TX & x_min, const TX & x_max, const TY & y_min, const TY & y_max)
  {
    ASSERT (!(x_max < x_min), "`x_max' should not be less than `x_min'");
    ASSERT (!(y_max < y_min), "`y_max' should not be less than `y_min'");

    return (y_min + (x - x_min) * (y_max - y_min) / (x_max - x_min));
  }
}


#endif // UTILS_ALGORITHMS_LERP_HXX
