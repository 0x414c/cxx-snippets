#ifndef UTILS_ALGORITHMS_MAX_HXX
#define UTILS_ALGORITHMS_MAX_HXX


#include <type_traits> // std::common_type
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
  constexpr typename std::common_type <TFirst>::type
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
  constexpr typename std::common_type <TFirst, TSecond, TRest ...>::type
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


//  /**
//   * @brief
//   * @tparam TFirst
//   * @param first
//   * @return
//   */
//  template <typename TFirst>
//  constexpr const TFirst &
//  max (const TFirst & first)
//  {
//    return first;
//  }
//
//
//  /**
//   * @brief
//   * NOTE: a <= b -> (b >= a == a <= b) ? b : a === (b < a) ? a : b -> max (a, b) == b <=> a == b.
//   * @tparam TFirstSecond
//   * @tparam TRest
//   * @param first
//   * @param second
//   * @param rest
//   * @return
//   */
//  template <typename TFirstSecond, typename ... TRest>
//  constexpr const TFirstSecond &
//  max (const TFirstSecond & first, const TFirstSecond & second, const TRest & ... rest)
//  {
//    if (second < first)
//    {
//      return max (first, rest ...);
//    }
//    else
//    {
//      return max (second, rest ...);
//    }
//  }
}


#endif // UTILS_ALGORITHMS_MAX_HXX
