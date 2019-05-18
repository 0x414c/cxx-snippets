#ifndef UTILS_ALGORITHMS_VARIANCE_HXX
#define UTILS_ALGORITHMS_VARIANCE_HXX


#include <iterator>  // std::{distance, iterator_traits}
#include <type_traits>  // std::is_arithmetic_v

//#include "../debug/assert.hxx"  // ASSERT
#include "summator.hxx"  // Summator


namespace Utils
{
  /**
   * @brief
   * @tparam TInputIterator
   * @param first
   * @param last
   * @return
   */
  template <typename TInputIterator>
  [[nodiscard]] constexpr typename std::iterator_traits <TInputIterator>::value_type
  variance (TInputIterator first, TInputIterator last)
  {
    using difference_type = typename std::iterator_traits <TInputIterator>::difference_type;
    using value_type = typename std::iterator_traits <TInputIterator>::value_type;


    static_assert (std::is_arithmetic_v <value_type>);

    // TODO:  `std::distance'  isn't usable as a constexpr function yet.
    //ASSERT (std::distance (first, last) > 1, "Distance between  `first'  &  `last'  must be greater than  `1'");

    difference_type count (0);
    Summator <value_type> mean (0);
    Summator <value_type> m_2 (0);
    while (first != last)
    {
      const value_type x (* first);
      const value_type delta_1 (x - value_type (mean));
      ++ count;
      mean += delta_1 / value_type (count);
      const value_type delta_2 (x - value_type (mean));
      m_2 += delta_1 * delta_2;

      ++ first;
    }

    return value_type (m_2) / value_type (count - 1);
  }
}


#endif  // UTILS_ALGORITHMS_VARIANCE_HXX
