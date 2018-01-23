#ifndef UTILS_ALGORITHMS_VARIANCE_HXX
#define UTILS_ALGORITHMS_VARIANCE_HXX


#include <iterator> // std::{distance, iterator_traits}
#include <type_traits> // std::is_arithmetic_v

#include "../debug/assert.hxx" // ASSERT


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
  constexpr typename std::iterator_traits <TInputIterator>::value_type
  variance (TInputIterator first, TInputIterator last)
  {
    using difference_type = typename std::iterator_traits <TInputIterator>::difference_type;
    using value_type = typename std::iterator_traits <TInputIterator>::value_type;


    static_assert (std::is_arithmetic_v <value_type>);

//    ASSERT (std::distance (first, last) > 1, "distance between `first' and `last' should be >= 2");

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
