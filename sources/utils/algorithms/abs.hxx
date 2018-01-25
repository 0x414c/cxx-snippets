#ifndef UTILS_ALGORITHMS_ABS_HXX
#define UTILS_ALGORITHMS_ABS_HXX


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
  template <typename TArithmetic>
  constexpr TArithmetic
  abs (TArithmetic x)
  {
    static_assert (std::is_arithmetic_v <TArithmetic>);

    if (x < TArithmetic (0))
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
