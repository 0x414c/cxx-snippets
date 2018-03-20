#ifndef UTILS_ALGORITHMS_MAX_HXX
#define UTILS_ALGORITHMS_MAX_HXX


#include <type_traits> // std::common_type_t
#include <utility> // std::forward


namespace Utils
{
  /**
   * @brief
   * @tparam TFirst
   * @param first
   * @return
   */
  template <typename TFirst>
  [[nodiscard]] constexpr std::common_type_t <TFirst>
  max (TFirst && first)
  {
    return std::forward <TFirst> (first);
  }


  /**
   * @brief
   * NOTE: a <= b -> (b >= a == a <= b) ? b : a === (b < a) ? a : b -> max (a, b) == b <=> a == b.
   * @tparam TFirst
   * @tparam TSecond
   * @tparam TRest
   * @param first
   * @param second
   * @param rest
   * @return
   */
  template <typename TFirst, typename TSecond, typename ... TRest>
  [[nodiscard]] constexpr std::common_type_t <TFirst, TSecond, TRest ...>
  max (TFirst && first, TSecond && second, TRest && ... rest)
  {
    if (second < first)
    {
      return max (std::forward <TFirst> (first), std::forward <TRest> (rest) ...);
    }
    else
    {
      return max (std::forward <TSecond> (second), std::forward <TRest> (rest) ...);
    }
  }
}


#endif // UTILS_ALGORITHMS_MAX_HXX
