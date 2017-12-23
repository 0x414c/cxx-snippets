#ifndef UTILS_TYPETRAITS_COMMON_HXX
#define UTILS_TYPETRAITS_COMMON_HXX


namespace Utils
{
  /**
   * @brief
   */
  template <typename TType>
  using TypeOf = typename TType::type;


  /**
   * @brief
   */
  template <typename TType>
  constexpr auto ValueOf = TType::value;
}


#endif // UTILS_TYPETRAITS_COMMON_HXX
