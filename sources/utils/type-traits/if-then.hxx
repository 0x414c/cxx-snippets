#ifndef UTILS_TYPETRAITS_IFTHEN_HXX
#define UTILS_TYPETRAITS_IFTHEN_HXX


#include "common.hxx" // ValueOf


namespace Utils
{
  /**
   * @brief
   * @tparam TValue
   * @tparam TType
   */
  template <bool TValue, typename TType>
  struct IfThen
  {
    /**
     * @brief
     */
    static constexpr bool value { TValue };
  };


  /**
   * @brief
   * @tparam TType
   */
  template <typename TType>
  struct IfThen <true, TType>
  {
    /**
     * @brief
     */
    using type = TType;


    /**
     * @brief
     */
    static constexpr bool value { true };
  };


  /**
   * @brief
   */
  template <bool TCondition, typename TType>
  using IfThenT = TypeOf <IfThen <TCondition, TType>>;
}


#endif // UTILS_TYPETRAITS_IFTHEN_HXX
