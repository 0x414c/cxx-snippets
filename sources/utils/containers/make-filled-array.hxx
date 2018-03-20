#ifndef UTILS_CONTAINERS_MAKEFILLEDARRAY_HXX
#define UTILS_CONTAINERS_MAKEFILLEDARRAY_HXX


#include <cstddef> // std::size_t

#include <array> // std::array


namespace Utils
{
  /**
   * @brief
   * @param value
   * @return
   */
  template <typename TValue, std::size_t TSize>
  [[nodiscard]] std::array <TValue, TSize>
  makeFilledArray (const TValue & value)
  {
    std::array <TValue, TSize> new_array;

    new_array.fill (value);

    return new_array;
  }
}


#endif // UTILS_CONTAINERS_MAKEFILLEDARRAY_HXX
