#ifndef UTILS_ALGORITHMS_MIN_HXX
#define UTILS_ALGORITHMS_MIN_HXX


//#include <type_traits> // std::common_type
//#include <utility> // std::forward


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
  constexpr typename std::common_type <TFirst, TSecond, TRest ...>::type
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


//  /**
//   * @brief
//   * @tparam TFirst
//   * @param first
//   * @return
//   */
//  template <typename TFirst>
//  constexpr const TFirst &
//  min (const TFirst & first)
//  {
//    return first;
//  }
//
//
//  /**
//   * @brief
//   * NOTE: a <= b -> (a <= b == b >= a) ? a : b === (b < a) ? b : a -> min (a, b) == a <=> a == b.
//   * @tparam TFirstSecond
//   * @tparam TRest
//   * @param first
//   * @param second
//   * @param rest
//   * @return
//   */
//  template <typename TFirstSecond, typename ... TRest>
//  constexpr const TFirstSecond &
//  min (const TFirstSecond & first, const TFirstSecond & second, const TRest & ... rest)
//  {
//    if (second < first)
//    {
//      return min (second, rest ...);
//    }
//    else
//    {
//      return min (first, rest ...);
//    }
//  }
}


#endif // UTILS_ALGORITHMS_MIN_HXX
