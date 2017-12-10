#ifndef UTILS_CONTAINERS_CSTRING_HXX
#define UTILS_CONTAINERS_CSTRING_HXX


#include <cstddef> // std::size_t


namespace Utils
{
  /**
   * @brief
   */
  template <typename TChar>
  class BasicCString final
  {
    public:
      /**
       * @brief
       */
      using char_type = TChar;

      /**
       * @brief
       */
      using self_type = BasicCString;


      /**
       * @brief
       * @tparam TSize
       * @param data
       */
      template <std::size_t TSize>
      constexpr BasicCString (const char_type (& data)[TSize]) :
        data_ (data),
        size_ (TSize - 1)
      { }


      /**
       * @brief
       * @return
       */
      constexpr const char_type *
      data (void) const noexcept
      {
        return data_;
      }


      /**
       * @brief
       * @return
       */
      constexpr std::size_t
      size (void) const noexcept
      {
        return size_;
      }


      /**
       * @brief
       * @param i
       * @return
       */
      constexpr char_type
      operator [] (std::size_t i) const noexcept
      {
        return data_[i];
      }


      /**
       * @brief
       * @return
       */
      constexpr operator const char_type * (void) const noexcept
      {
        return data ();
      }


  private:
    /**
     * @brief
     */
    const char_type * const data_;

    /**
     * @brief
     */
    const std::size_t size_;
  };


  using CString = BasicCString <char>;
}


#endif // UTILS_CONTAINERS_CSTRING_HXX
