#ifndef UTILS_META_ALIGNEDSTORAGE_HXX
#define UTILS_META_ALIGNEDSTORAGE_HXX


#include <cstddef>  // std::{byte, size_t}

#include "common.hxx"  // TypeOf


namespace Utils
{
  /**
   * @brief
   * @tparam TSize
   * @tparam TAlignment
   */
  template <std::size_t TSize, std::size_t TAlignment>
  struct AlignedStorage
  {
    static_assert (TSize > 0);


    /**
     * @brief
     */
    static constexpr std::size_t alignment { TAlignment };

    /**
     * @brief
     */
    static constexpr std::size_t size { TSize };


    /**
     * @brief
     */
    struct type final
    {
      private:
        /**
         * @brief
         */
        alignas (alignment) std::byte data_ [size];
    };
  };


  /**
   * @brief
   */
  template <std::size_t TSize, std::size_t TAlignment>
  using AlignedStorageT = TypeOf <AlignedStorage <TSize, TAlignment>>;
}


#endif  // UTILS_META_ALIGNEDSTORAGE_HXX
