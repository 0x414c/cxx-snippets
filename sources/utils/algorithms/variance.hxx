#ifndef UTILS_ALGORITHMS_VARIANCE_HXX
#define UTILS_ALGORITHMS_VARIANCE_HXX


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
  variance (TInputIter first, TInputIter last)
  {
//    ASSERT (std::distance (first, last) > 1, "distance between `first' and `last' should be >= 2");


    using difference_type = typename std::iterator_traits <TInputIter>::difference_type;
    using value_type = typename std::iterator_traits <TInputIter>::value_type;


    difference_type n (0);
    value_type mean (0);
    value_type m_2 (0);

    while (first != last)
    {
      ++n;

      const value_type x (*first);

      const value_type delta_1 (x - mean);
      mean += delta_1 / value_type (n);

      const value_type delta_2 (x - mean);
      m_2 += delta_1 * delta_2;

      ++first;
    }

    return (m_2 / value_type (n - 1));
  }
}


#endif // UTILS_ALGORITHMS_VARIANCE_HXX
