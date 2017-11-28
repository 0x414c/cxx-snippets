#ifndef UTILS_TYPETRAITS_ALIGNEDSTORAGE_HXX
#define UTILS_TYPETRAITS_ALIGNEDSTORAGE_HXX


#include <cstddef> // std::{byte, size_t}


namespace Utils
{
  /**
   * @brief
   * @tparam TSize
   * @tparam TAlignment
   */
  template <std::size_t TSize, std::size_t TAlignment>
  struct AlignedStorage final
  {
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
        alignas (alignment) std::byte data_[size];
    };
  };
}


#endif // UTILS_TYPETRAITS_ALIGNEDSTORAGE_HXX
