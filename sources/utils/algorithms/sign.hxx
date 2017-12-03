#ifndef UTILS_ALGORITHMS_SIGN_HXX
#define UTILS_ALGORITHMS_SIGN_HXX


#include <functional> // std::less
#include <type_traits> // std::is_signed

#include "../debug/assert.hxx" // ASSERT


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
  sign (const TX & x, const TCompare & compare = TCompare ())
  {
    static_assert (
      std::is_signed <typename std::underlying_type <TX>::type>::value,
      "Underlying type of `TX' enumeration should be a signed one"
    );

    if (compare (TX (0), x))
    {
      return TX (1);
    }
    else
    {
      if (compare (x, TX (0)))
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
