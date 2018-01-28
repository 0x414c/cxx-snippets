#ifndef UTILS_META_COMMON_HXX
#define UTILS_META_COMMON_HXX


namespace Utils
{
  /**
   * @brief
   */
  template <typename TMetafunction>
  using TypeOf = typename TMetafunction::type;


  /**
   * @brief
   */
  template <typename TMetafunction>
  inline constexpr auto ValueOf = TMetafunction::value;
}


#endif // UTILS_META_COMMON_HXX
