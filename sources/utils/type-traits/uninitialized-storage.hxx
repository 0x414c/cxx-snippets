#ifndef UTILS_TYPETRAITS_UNINITIALIZEDSTORAGE_HXX
#define UTILS_TYPETRAITS_UNINITIALIZEDSTORAGE_HXX


#include <cstddef> // std::size_t

#include "../algorithms/max.hxx" // max


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
      alignas (alignment) unsigned char data_[size];
    };
  };


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


#endif // UTILS_TYPETRAITS_UNINITIALIZEDSTORAGE_HXX
