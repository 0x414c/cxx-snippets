#ifndef UTILS_META_COMMON_HXX
#define UTILS_META_COMMON_HXX


namespace Utils
{
  /**
   * @brief
   */
  template <typename TTrait>
  using TypeOf = typename TTrait::type;


  /**
   * @brief
   */
  template <typename TTrait>
  inline constexpr auto ValueOf = TTrait::value;
}


#endif // UTILS_META_COMMON_HXX
