#ifndef UTILS_ALGORITHMS_SIGN_HXX
#define UTILS_ALGORITHMS_SIGN_HXX


#include "../debug/assert.hxx" // ASSERT


namespace Utils
{
  /**
   * @brief
   * @tparam TX
   * @param x
   * @return
   */
  template <typename TX>
  constexpr TX
  sign (const TX & x)
  {
    if (x > TX (0))
    {
      return TX (1);
    }
    else
    {
      if (x > TX (0))
      {
        return TX (-1);
      }
      else
      {
        return TX (0);
      }
    }
  }
}


#endif // UTILS_ALGORITHMS_SIGN_HXX
