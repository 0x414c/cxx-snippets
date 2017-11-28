#ifndef UTILS_ALGORITHMS_MEAN_HXX
#define UTILS_ALGORITHMS_MEAN_HXX


#include <iterator> // std::{distance, iterator_traits}

#include "../debug/assert.hxx" // ASSERT


namespace Utils
{
  /**
   * @brief
   * @tparam TInputIter
   * @param first
   * @param last
   * @return
   */
  template <typename TInputIter>
  constexpr typename std::iterator_traits <TInputIter>::value_type
  mean (TInputIter first, TInputIter last)
  {
//    ASSERT (std::distance (first, last) > 1, "distance between `first' and `last' should be >= 2");


    using difference_type = typename std::iterator_traits <TInputIter>::difference_type;
    using value_type = typename std::iterator_traits <TInputIter>::value_type;


    difference_type count (0);
    value_type mean (0);

    while (first != last)
    {
      ++count;

      mean += (*first - mean) / value_type (count);

      ++first;
    }

    return mean;
  }
}


#endif // UTILS_ALGORITHMS_MEAN_HXX
