#ifndef UTILS_ALGORITHMS_SIGN_HXX
#define UTILS_ALGORITHMS_SIGN_HXX


#include <type_traits> // std::is_signed_v


namespace Utils
{
  /**
   * @brief
   * @tparam TSigned
   * @param x
   * @return
   */
  template <typename TSigned>
  constexpr TSigned
  sign (TSigned x)
  {
    static_assert (std::is_signed_v <TSigned>);

    if (TSigned (0) < x)
    {
      return TSigned (1);
    }
    else
    {
      if (x < TSigned (0))
      {
        return TSigned (- 1);
      }
      else
      {
        return TSigned (0);
      }
    }
  }
}


#endif // UTILS_ALGORITHMS_SIGN_HXX
