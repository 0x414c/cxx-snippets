#ifndef UTILS_ALGORITHMS_ABS_HXX
#define UTILS_ALGORITHMS_ABS_HXX


#include <functional> // std::less
#include <type_traits> // std::is_arithmetic_v


namespace Utils
{
  /**
   * @brief
   * @tparam TArithmetic
   * @param x
   * @param compare
   * @return
   */
  template <typename TArithmetic, typename TCompare = std::less <TArithmetic>>
  constexpr TArithmetic
  abs (TArithmetic x, TCompare compare = TCompare ())
  {
    static_assert (
      std::is_arithmetic_v <TArithmetic>,
      "Type `TArithmetic' should be an arithmetic one"
    );

    if (compare (x, TArithmetic (0)))
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
