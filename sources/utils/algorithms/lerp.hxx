#ifndef UTILS_ALGORITHMS_LERP_HXX
#define UTILS_ALGORITHMS_LERP_HXX


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
  template <typename TX, typename TY>
  constexpr TY
  lerp (const TX & x, const TX & x_0, const TX & x_1, const TY & y_0, const TY & y_1)
  {
    return (y_0 + (x - x_0) * (y_1 - y_0) / (x_1 - x_0));
  }
}


#endif // UTILS_ALGORITHMS_LERP_HXX
