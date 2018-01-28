#ifndef UTILS_ALGORITHMS_CLAMP_HXX
#define UTILS_ALGORITHMS_CLAMP_HXX


#include <functional> // std::less

#include "../debug/assert.hxx" // ASSERT


namespace Utils
{
  /**
   * @brief
   * NOTE: The result is undefined if `x_min' is greater than `x_max'.
   * NOTE: If `x' is equivalent to either bound, returns a reference to `x', not the bound.
   * NOTE: Can be implemented
   *   as `return std::max (x_min, std::min (x_max, x));'
   *   or `return std::min (std::max (x, x_min), x_max);'
   *   or `return ((x_max < x) ? x_max : ((x < x_min) ? x_min : x));'
   *   or `return ((x < x_min) ? x_min : ((x_max < x) ? x_max : x));'.
   * @tparam TX
   * @param x
   * @param x_min
   * @param x_max
   * @param compare
   * @return
   */
  template <typename TX, typename TCompare = std::less <TX>>
  constexpr const TX &
  clamp (const TX & x, const TX & x_min, const TX & x_max, TCompare compare = TCompare ())
  {
    ASSERT (!compare (x_max, x_min), "`x_max' must not be less than `x_min'");

    if (compare (x, x_min))
    {
      return x_min;
    }
    else
    {
      if (compare (x_max, x))
      {
        return x_max;
      }
      else
      {
        return x;
      }
    }
  }


  /**
   * @brief
   * @tparam TX
   * @param x
   * @param x_min
   * @param x_max
   * @param y_min
   * @param y_max
   * @param compare
   * @return
   */
  template <typename TX, typename TY = TX, typename TCompare = std::less <TX>>
  constexpr const TY &
  clamp (
    const TX & x, const TX & x_min, const TX & x_max, const TY & y_min, const TY & y_max,
    TCompare compare = TCompare ()
  )
  {
    ASSERT (!compare (x_max, x_min), "`x_max' must not be less than `x_min'");

    if (compare (x, x_min))
    {
      return y_min;
    }
    else
    {
      if (compare (x_max, x))
      {
        return y_max;
      }
      else
      {
        return x;
      }
    }
  }
}


#endif // UTILS_ALGORITHMS_CLAMP_HXX
