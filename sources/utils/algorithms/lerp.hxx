#ifndef UTILS_ALGORITHMS_LERP_HXX
#define UTILS_ALGORITHMS_LERP_HXX


#include "../debug/assert.hxx" // ASSERT
#include "../debug/log.hxx" // LOG_EXPR


namespace Utils
{
  /**
   * @brief
   * @tparam TXY
   * @param x
   * @param x_min
   * @param x_max
   * @param y_min
   * @param y_max
   * @return
   */
  template <typename TXY>
  constexpr TXY
  lerp (const TXY & x, const TXY & x_min, const TXY & x_max, const TXY & y_min, const TXY & y_max)
  {
    ASSERT (!(x_max < x_min), "`x_max' should not be less than `x_min'");
    ASSERT (!(y_max < y_min), "`y_max' should not be less than `y_min'");

    return (y_min + (x - x_min) * (y_max - y_min) / (x_max - x_min));
  }
}


#endif // UTILS_ALGORITHMS_LERP_HXX
