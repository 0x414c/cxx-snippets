#ifndef UTILS_TYPETRAITS_IFTHEN_HXX
#define UTILS_TYPETRAITS_IFTHEN_HXX


namespace Utils
{
  /**
   * @brief
   * @tparam TCondition
   * @tparam TType
   */
  template <bool TCondition, typename TType>
  struct IfThen
  {
    static constexpr bool condition { TCondition };
  };


  /**
   * @brief
   * @tparam TType
   */
  template <typename TType>
  struct IfThen <true, TType>
  {
    using type = TType;


    static constexpr bool condition { true };
  };
}


#endif // UTILS_TYPETRAITS_IFTHEN_HXX
