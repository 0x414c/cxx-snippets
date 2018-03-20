#ifndef UTILS_ALGORITHMS_MIN_HXX
#define UTILS_ALGORITHMS_MIN_HXX


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
  min (TFirst && first)
  {
    return std::forward <TFirst> (first);
  }


  /**
   * @brief
   * NOTE: a <= b -> (a <= b == b >= a) ? a : b === (b < a) ? b : a -> min (a, b) == a <=> a == b.
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
  min (TFirst && first, TSecond && second, TRest && ... rest)
  {
    if (second < first)
    {
      return min (std::forward <TSecond> (second), std::forward <TRest> (rest) ...);
    }
    else
    {
      return min (std::forward <TFirst> (first), std::forward <TRest> (rest) ...);
    }
  }
}


#endif // UTILS_ALGORITHMS_MIN_HXX
