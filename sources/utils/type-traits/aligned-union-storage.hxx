#ifndef UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGE_HXX
#define UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGE_HXX


#include <cstddef> // std::size_t

#include "aligned-storage.hxx" // AlignedStorage

#include "../algorithms/max.hxx" // max


namespace Utils
{
  /**
   * @brief
   * @tparam TTypes
   */
  template <typename ... TTypes>
  struct AlignedUnionStorage final
  {
    /**
     * @brief
     */
    static constexpr std::size_t alignment { max (alignof (TTypes) ...) };

    /**
     * @brief
     */
    static constexpr std::size_t size { max (sizeof (TTypes) ...) };


    /**
     * @brief
     */
    using type = typename AlignedStorage <size, alignment>::type;
  };
}


#endif // UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGE_HXX
