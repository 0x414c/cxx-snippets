#ifndef UTILS_ALGORITHMS_MEAN_HXX
#define UTILS_ALGORITHMS_MEAN_HXX


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
  mean (TInputIterator first, TInputIterator last)
  {
    using difference_type = typename std::iterator_traits <TInputIterator>::difference_type;
    using value_type = typename std::iterator_traits <TInputIterator>::value_type;


    static_assert (std::is_arithmetic_v <value_type>);

    // TODO:  `std::distance'  isn't usable as a constexpr function yet.
    //ASSERT (std::distance (first, last) > 1, "Distance between  `first'  &  `last' must be greater than  `1'");

    difference_type count (0);
    Summator <value_type> mean (0);
    while (first != last)
    {
      ++ count;
      mean += ((* first) - value_type (mean)) / value_type (count);

      ++ first;
    }

    return value_type (mean);
  }
}


#endif  // UTILS_ALGORITHMS_MEAN_HXX
