#ifndef UTILS_ALGORITHMS_ISCLOSE_HXX
#define UTILS_ALGORITHMS_ISCLOSE_HXX


#include <cmath> // std::{fabs, isfinite}

#include <type_traits> // std::is_floating_point

#include "../config/is-close.hxx" // Config::Utils::IsClose::{Relative, Absolute}_tolerance
#include "../debug/assert.hxx" // ASSERT


namespace Utils
{
  /**
   * @brief
   * @tparam TXY
   * @param x
   * @param y
   * @param rel_tol
   * @param abs_tol
   * @return
   */
  template <typename TXY>
  constexpr bool
  isClose (
    const TXY & x, const TXY & y,
    const TXY & rel_tol = Config::Utils::IsClose::Relative_tolerance <TXY>,
    const TXY & abs_tol = Config::Utils::IsClose::Absolute_tolerance <TXY>
  )
  {
    static_assert (
      std::is_floating_point <TXY>::value,
      "Type `TXY' should be a floating-point one"
    );

    ASSERT (!(rel_tol < 0), "`rel_tol' should not be less than 0");
    ASSERT (!(abs_tol < 0), "`abs_tol' should not be less than 0");

    if (std::isfinite (x) && std::isfinite (y))
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
      if (x == y)
#pragma GCC diagnostic pop
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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
      return (x == y);
#pragma GCC diagnostic pop
    }
  }
}


#endif // UTILS_ALGORITHMS_ISCLOSE_HXX
