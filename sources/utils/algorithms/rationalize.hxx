#ifndef UTILS_ALGORITHMS_RATIONALIZE_HXX
#define UTILS_ALGORITHMS_RATIONALIZE_HXX


#include <cstddef> // std::size_t
#include <cmath> // std::{isfinite, trunc, fabs}

#include <limits> // std::numeric_limits
#include <tuple> // std::tuple
#include <type_traits> // std::is_{integral, floating_point}

#include "../config/is-close.hxx" // Config::Utils::IsClose::{Relative, Absolute}_tolerance
#include "../debug/assert.hxx" // ASSERT
#include "abs.hxx" // abs
#include "is-close.hxx" // isClose


namespace Utils
{
  template <typename TInt, typename TFloat>
  constexpr std::tuple <TInt, TInt, TFloat>
  rationalize (
    const TFloat & x,
    const TInt & max_denominator = 1'000'000, const std::size_t max_iterations = 23,
    const TFloat & rel_tol = Config::Utils::IsClose::Relative_tolerance <TFloat>,
    const TFloat & abs_tol = Config::Utils::IsClose::Absolute_tolerance <TFloat>
  )
  {
    static_assert (
      std::is_integral <TInt>::value,
      "Type `TInt' should be an integral one"
    );

    static_assert (
      std::is_floating_point <TFloat>::value,
      "Type `TFloat' should be a floating-point one"
    );

    ASSERT (!(rel_tol < TFloat (0)), "`rel_tol' should not be less than 0");
    ASSERT (!(abs_tol < TFloat (0)), "`abs_tol' should not be less than 0");
    ASSERT (!(max_denominator <= TInt (0)), "`max_denominator' should not be less than or equal to 0");

    if (!std::isfinite (x))
    {
      return { 0, 0, std::numeric_limits <TFloat>::quiet_NaN () };
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    if (x == TFloat (0))
#pragma GCC diagnostic pop
    {
      return { 0, 1, 0 };
    }

    TFloat r_1 (x);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
    TInt a_1 (std::trunc (r_1));
#pragma GCC diagnostic pop

    if (isClose (TFloat (a_1), x, rel_tol, abs_tol) || (max_denominator == 1) || (max_iterations == 1))
    {
      return { a_1, 1, TFloat (a_1) - x };
    }

    std::size_t iteration (1);

    TInt p_0 (1);
    TInt q_0 (0);
    TInt p_1 (a_1);
    TInt q_1 (1);
    TInt p_2 (0);
    TInt q_2 (1);

    while (true)
    {
      ++iteration;

      const TFloat r_2 (TFloat (1) / (r_1 - TFloat (a_1)));
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
      const TInt a_2 (std::trunc (r_2));
#pragma GCC diagnostic pop
      p_2 = a_2 * p_1 + p_0;
      q_2 = a_2 * q_1 + q_0;
      const TFloat c_2 (TFloat (p_2) / TFloat (q_2));

      if (!isClose (c_2, x, rel_tol, abs_tol) && (abs (q_2) <= max_denominator) && (iteration < max_iterations))
      {
        p_0 = p_1;
        q_0 = q_1;
        p_1 = p_2;
        q_1 = q_2;
        r_1 = r_2;
        a_1 = a_2;
      }
      else
      {
        break;
      }
    }

    if (abs (q_2) <= max_denominator)
    {
      return { p_2, q_2, TFloat (p_2) / TFloat (q_2) - x };
    }
    else
    {
      return { p_1, q_1, TFloat (p_1) / TFloat (q_1) - x };
    }
  }
}


#endif // UTILS_ALGORITHMS_RATIONALIZE_HXX
