#ifndef UTILS_ALGORITHMS_ISCLOSE_HXX
#define UTILS_ALGORITHMS_ISCLOSE_HXX


#include <cmath> // std::{fabs, isfinite}

#include <type_traits> // std::is_floating_point_v

#include "../config/is-close.hxx" // Config::Utils::IsClose::{Absolute_tolerance, Relative_tolerance}
#include "../debug/assert.hxx" // ASSERT
#include "abs.hxx" // abs


namespace Utils
{
  /**
   * @brief
   * @tparam TFloatingPoint
   * @param x
   * @param y
   * @param rel_tol
   * @param abs_tol
   * @return
   */
  template <typename TFloatingPoint>
  constexpr bool
  isClose (
    TFloatingPoint x, TFloatingPoint y,
    TFloatingPoint rel_tol = Config::Utils::IsClose::Relative_tolerance <TFloatingPoint>,
    TFloatingPoint abs_tol = Config::Utils::IsClose::Absolute_tolerance <TFloatingPoint>
  )
  {
    static_assert (std::is_floating_point_v <TFloatingPoint>);

    ASSERT (!(rel_tol < TFloatingPoint (0)), "`rel_tol' should not be less than 0");
    ASSERT (!(abs_tol < TFloatingPoint (0)), "`abs_tol' should not be less than 0");

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
        const TFloatingPoint abs_diff (abs (x - y));

        return (
             (abs_diff <= abs_tol)
          || (abs_diff <= (rel_tol * abs (x)))
          || (abs_diff <= (rel_tol * abs (y)))
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
