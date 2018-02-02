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
  struct IfThenElse;


  /**
   * @brief
   * @tparam TCondition
   * @tparam TIfFirst
   */
  template <bool TCondition, typename TIfFirst>
  struct IfThenElse <TCondition, TIfFirst>
  {
    /**
     * @brief
     */
    using type = TypeOf <TIfFirst>;
  };


  /**
   * @brief
   * @tparam TCondition
   * @tparam TIfFirst
   */
  template <typename TIfFirst>
  struct IfThenElse <false, TIfFirst>
  { };


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
    using type = TypeOf <IfThenElse <ValueOf <TIfFalseFirst>, TIfFalseFirst, TIfFalseRest ...>>;
  };


  /**
   * @brief
   */
  template <bool TCondition, typename TIfFirst, typename ... TIfRest>
  using IfThenElseT = TypeOf <IfThenElse <TCondition, TIfFirst, TIfRest ...>>;
}


#endif // UTILS_META_IFTHENELSE_HXX
