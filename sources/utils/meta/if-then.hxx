#ifndef UTILS_META_IFTHEN_HXX
#define UTILS_META_IFTHEN_HXX


#include "common.hxx" // TypeOf


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
    /**
     * @brief
     */
    using type = TType;


    /**
     * @brief
     */
    static constexpr bool value { TCondition };
  };


  /**
   * @brief
   * @tparam TType
   */
  template <typename TType>
  struct IfThen <false, TType>
  {
    /**
     * @brief
     */
    static constexpr bool value { false };
  };


  /**
   * @brief
   */
  template <bool TCondition, typename TType>
  using IfThenT = TypeOf <IfThen <TCondition, TType>>;
}


#endif // UTILS_META_IFTHEN_HXX
