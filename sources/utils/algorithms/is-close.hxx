#ifndef UTILS_ALGORITHMS_ISCLOSE_HXX
#define UTILS_ALGORITHMS_ISCLOSE_HXX


#include <cmath> // std::{fabs, isfinite}

#include <type_traits> // std::is_floating_point

#include "../debug/assert.hxx" // ASSERT


namespace Utils
{
  /**
   * @brief
   * TODO: [1;0] Move `rel_tol' and `abs_tol' defaults to corresponding config header.
   * @tparam TXY
   * @param x
   * @param y
   * @param rel_tol
   * @param abs_tol
   * @return
   */
  template <typename TXY>
  constexpr bool
  isClose (const TXY & x, const TXY & y, TXY rel_tol = 1E-8, TXY abs_tol = 1E-16)
  {
    static_assert (
      std::is_floating_point <TXY>::value,
      "Type `TXY' should be floating-point one"
    );

    ASSERT (!(rel_tol < 0.0), "`rel_tol' should not be less than 0.0");
    ASSERT (!(abs_tol < 0.0), "`abs_tol' should not be less than 0.0");

    if (std::isfinite (x) && std::isfinite (y))
    {
      if (x == y)
      {
        return true;
      }
      else
      {
        const TXY abs_diff (std::fabs (x - y));

        return (
             (abs_diff <= abs_tol)
          || (abs_diff <= (rel_tol * std::fabs (x)))
          || (abs_diff <= (rel_tol * std::fabs (y)))
        );
      }
    }
    else
    {
      return (x == y);
    }
  }
}


#endif // UTILS_ALGORITHMS_ISCLOSE_HXX
