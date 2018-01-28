#ifndef UTILS_META_ALIGNEDUNIONSTORAGE_HXX
#define UTILS_META_ALIGNEDUNIONSTORAGE_HXX


#include <cstddef> // std::size_t

#include "../algorithms/max.hxx" // max
#include "aligned-storage.hxx" // AlignedStorageT
#include "common.hxx" // TypeOf


namespace Utils
{
  /**
   * @brief
   * @tparam TTypes
   */
  template <typename ... TTypes>
  struct AlignedUnionStorage
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
    using type = AlignedStorageT <size, alignment>;
  };


  /**
   * @brief
   */
  template <typename ... TTypes>
  using AlignedUnionStorageT = TypeOf <AlignedUnionStorage <TTypes ...>>;
}


#endif // UTILS_META_ALIGNEDUNIONSTORAGE_HXX
