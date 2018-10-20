#ifndef UTILS_ALGORITHMS_ABS_HXX
#define UTILS_ALGORITHMS_ABS_HXX


#include <type_traits>  // std::is_arithmetic_v


namespace Utils
{
  /**
   * @brief
   * @tparam TArithmetic
   * @param x
   * @return
   */
  template <typename TArithmetic>
  [[nodiscard]] constexpr TArithmetic
  abs (TArithmetic x)
  {
    static_assert (std::is_arithmetic_v <TArithmetic>);

    if (x < TArithmetic (0))
    {
      return - x;
    }

    return x;
  }
}


#endif  // UTILS_ALGORITHMS_ABS_HXX
