#ifndef UTILS_ALGORITHMS_ABS_HXX
#define UTILS_ALGORITHMS_ABS_HXX


#include <functional> // std::less
#include <type_traits> // std::is_arithmetic


namespace Utils
{
  /**
   * @brief
   * @tparam TX
   * @param x
   * @return
   */
  template <typename TX, class TCompare = std::less <TX>>
  constexpr TX
  abs (const TX & x, const TCompare & compare = TCompare ())
  {
    static_assert (
      std::is_arithmetic <TX>::value,
      "Type `TInt' should be an arithmetic one"
    );

    if (compare (x, TX (0)))
    {
      return -x;
    }
    else
    {
      return x;
    }
  }
}


#endif // UTILS_ALGORITHMS_ABS_HXX
