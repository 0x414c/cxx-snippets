#ifndef UTILS_META_IFTHENELSE_HXX
#define UTILS_META_IFTHENELSE_HXX


#include "common.hxx" // TypeOf


namespace Utils
{
  /**
   * @brief
   * @tparam TCondition
   * @tparam TIfFirst
   * @tparam TIfRest
   */
  template <bool TCondition, typename TIfFirst, typename ... TIfRest>
  struct IfThenElse
  {
    /**
     * @brief
     */
    using type = TypeOf <TIfFirst>;
  };


  /**
   * @brief
   * @tparam TCondition
   * @tparam TIfTrue
   * @tparam TIfFalse
   */
  template <bool TCondition, typename TIfTrue, typename TIfFalse>
  struct IfThenElse <TCondition, TIfTrue, TIfFalse>
  {
    /**
     * @brief
     */
    using type = TypeOf <TIfTrue>;
  };


  /**
   * @brief
   * @tparam TIfTrue
   * @tparam TIfFalse
   */
  template <typename TIfTrue, typename TIfFalse>
  struct IfThenElse <false, TIfTrue, TIfFalse>
  {
    /**
     * @brief
     */
    using type = TypeOf <TIfFalse>;
  };


  /**
   * @brief
   * @tparam TCondition
   * @tparam TIfTrue
   * @tparam TIfFalseFirst
   * @tparam TIfFalseRest
   */
  template <bool TCondition, typename TIfTrue, typename TIfFalseFirst, typename ... TIfFalseRest>
  struct IfThenElse <TCondition, TIfTrue, TIfFalseFirst, TIfFalseRest ...>
  {
    /**
     * @brief
     */
    using type = TypeOf <TIfTrue>;
  };


  /**
   * @brief
   */
  template <bool TCondition, typename TIfFirst, typename ... TIfRest>
  using IfThenElseT = TypeOf <IfThenElse <TCondition, TIfFirst, TIfRest ...>>;


  /**
   * @brief
   * @tparam TIfTrue
   * @tparam TIfFalseFirst
   * @tparam TIfFalseRest
   */
  template <typename TIfTrue, typename TIfFalseFirst, typename ... TIfFalseRest>
  struct IfThenElse <false, TIfTrue, TIfFalseFirst, TIfFalseRest ...>
  {
    /**
     * @brief
     */
    using type = IfThenElseT <ValueOf <TIfFalseFirst>, TIfFalseFirst, TIfFalseRest ...>;
  };
}


#endif // UTILS_META_IFTHENELSE_HXX
