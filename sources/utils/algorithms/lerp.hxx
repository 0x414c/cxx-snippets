#ifndef UTILS_ALGORITHMS_LERP_HXX
#define UTILS_ALGORITHMS_LERP_HXX


#include "../debug/assert.hxx" // ASSERT
#include "../debug/log.hxx" // LOG_EXPR


namespace Utils
{
  /**
   * @brief
   * @tparam TX
   * @param x
   * @param x_min
   * @param x_max
   * @param y_min
   * @param y_max
   * @return
   */
  template <typename TX>
  constexpr TX
  lerp (const TX & x, const TX & x_min, const TX & x_max, const TX & y_min, const TX & y_max)
  {
    ASSERT (!(x_max < x_min), "`x_max' should not be less than `x_min'");
    ASSERT (!(y_max < y_min), "`y_max' should not be less than `y_min'");

    return (y_min + (y_max - y_min) * ((x - x_min) / (x_max - x_min)));
  }
}


#endif // UTILS_ALGORITHMS_LERP_HXX
