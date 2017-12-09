#ifndef UTILS_ALGORITHMS_LERP2D_HXX
#define UTILS_ALGORITHMS_LERP2D_HXX


#include "lerp.hxx" // lerp


namespace Utils
{
  /**
   * @brief
   * @tparam TX
   * @tparam TY
   * @tparam TZ
   * @param x
   * @param y
   * @param x_0
   * @param x_1
   * @param y_0
   * @param y_1
   * @param z_0_0
   * @param z_0_1
   * @param z_1_1
   * @param z_1_0
   * @return
   */
  template <typename TX, typename TY, typename TZ>
  constexpr TZ
  lerp_2d (
    const TX & x, const TY & y,
    const TX & x_0, const TX & x_1, const TY & y_0, const TY & y_1,
    const TZ & z_0_0, const TZ & z_0_1, const TZ & z_1_1, const TZ & z_1_0
  )
  {
    const TZ z_0 (lerp (x, x_0, x_1, z_0_0, z_1_0));
    const TZ z_1 (lerp (x, x_0, x_1, z_0_1, z_1_1));

    return lerp (y, y_0, y_1, z_0, z_1);
  }
}


#endif // UTILS_ALGORITHMS_LERP2D_HXX
