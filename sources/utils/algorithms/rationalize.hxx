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
  /**
   * @brief Finds the "best" rational approximation (p / q) of the real number `x' that satisfies given restrictions
   *   (error, maximal denominator value and maximal iterations count) by using Continued Fractions method.
   * For the reference see `http://mathworld.wolfram.com/ContinuedFraction.html'.
   * The absolute error value will be |p / q - x|.
   * @tparam TInt
   * @tparam TFloat
   * @param x
   * @param max_denominator
   * @param max_iterations
   * @param rel_tol
   * @param abs_tol
   * @return
   */
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
      return { TInt (0), TInt (0), std::numeric_limits <TFloat>::quiet_NaN () };
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-equal"
    if (x == TFloat (0))
#pragma GCC diagnostic pop
    {
      return { TInt (0), TInt (1), TFloat (0) };
    }

    TFloat r_1 (x); // See eq. (8): r[0] := x
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
    TInt a_1 (std::trunc (r_1)); // See eq. (5): a[0] := ⌊x⌋
#pragma GCC diagnostic pop

    /*
     * If we got "almost integer" `x' or one of the trivial cases, we should return pair 〈a0; 1, e〉,
     *   or the loop will stuck at division by 0 later.
     */
    if (isClose (TFloat (a_1), x, rel_tol, abs_tol) || (max_denominator == 1) || (max_iterations == 1))
    {
      return { a_1, TInt (1), TFloat (a_1) - x };
    }

    std::size_t iteration (1);

    TInt p_0 (1); // See eq. (25): p[-1] := 1
    TInt q_0 (0); // See eq. (25): q[-1] := 0
    TInt p_1 (a_1); // See eq. (26): p[0] := a[0]
    TInt q_1 (1); // See eq. (26): q[0] := 1
    TInt p_2 (0); // p[1] := 0
    TInt q_2 (1); // q[1] := 1
    /*
     * NOTE: Indices in variables' names is shifted by 1.
     * We use only p[-1], q[-1] and p[0], q[0] as starting coefficients.
     * Rather than keeping entire sequence of p's and q's we will keep and reuse coefficients computed on current
     *   iteration (p[1] and q[1]) to compute new coefficients on the next iteration.
     */

    while (true)
    {
      ++iteration;

      const TFloat r_2 (TFloat (1) / (r_1 - TFloat (a_1))); // See eq. (9): r[n] := 1 / (r[n-1] - a[n-1])
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
      const TInt a_2 (std::trunc (r_2)); // See eq. (10): a[n] := ⌊r[n]⌋
#pragma GCC diagnostic pop
      p_2 = a_2 * p_1 + p_0; // See eq. (27): p[n] == a[n] * p[n-1] + p[n-2]
      q_2 = a_2 * q_1 + q_0; // See eq. (28): q[n] == a[n] * q[n-1] + q[n-2]
      // Look at the n-th convergent (see eq. (11)): c[n] := p[n] / q[n]:
      const TFloat c_2 (TFloat (p_2) / TFloat (q_2));

      if (!isClose (c_2, x, rel_tol, abs_tol) && (abs (q_2) <= max_denominator) && (iteration < max_iterations))
      {
        /* Now "shift" all the coefficients to the left:
         *   - p[-1] := p[0], q[-1] := q[0];
         *   - p[ 0] := p[1], q[ 0] := q[1];
         *   - r[ 0] := r[1], a[ 0] := a[1].
         */
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
