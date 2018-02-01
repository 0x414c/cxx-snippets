#ifndef UTILS_CONTAINERS_CSTRING_HXX
#define UTILS_CONTAINERS_CSTRING_HXX


#include <cstddef> // std::size_t

#include <type_traits> // std::is_same_v


namespace Utils
{
  /**
   * @brief
   */
  template <typename TChar>
  class BasicCString final
  {
    static_assert (
         std::is_same_v <TChar, char>
      || std::is_same_v <TChar, wchar_t>
      || std::is_same_v <TChar, char16_t>
      || std::is_same_v <TChar, char32_t>
    );


    public:
      /**
       * @brief
       */
      using char_type = TChar;


    private:
      /**
       * @brief
       */
      using self_type = BasicCString;


    public:
      /**
       * @brief
       */
      constexpr BasicCString (void) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr BasicCString (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr BasicCString (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @tparam TSize
       * @param data
       */
      template <std::size_t TSize>
      constexpr BasicCString (const char_type (& data)[TSize]) noexcept :
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
       * @return
       */
      constexpr operator const char_type * (void) const noexcept
      {
        return data ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param pos
       * @return
       */
      constexpr char_type
      operator [] (std::size_t pos) const noexcept
      {
        return data ()[pos];
      }


  private:
    /**
     * @brief
     */
    const char_type * data_;

    /**
     * @brief
     */
    std::size_t size_;
  };


  /**
   * @brief
   */
  using CString = BasicCString <char>;

  /**
   * @brief
   */
  using WCString = BasicCString <wchar_t>;

  /**
   * @brief
   */
  using U16CString = BasicCString <char16_t>;

  /**
   * @brief
   */
  using U32CString = BasicCString <char32_t>;
}


#endif // UTILS_CONTAINERS_CSTRING_HXX
