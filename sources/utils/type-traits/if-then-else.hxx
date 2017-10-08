#ifndef UTILS_TYPETRAITS_IFTHENELSE_HXX
#define UTILS_TYPETRAITS_IFTHENELSE_HXX


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
    using type = typename TIfFirst::type;
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
    using type = typename TIfTrue::type;
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
    using type = typename TIfFalse::type;
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
    using type = typename TIfTrue::type;
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
    using type = typename IfThenElse <TIfFalseFirst::condition, TIfFalseFirst, TIfFalseRest ...>::type;
  };
}


#endif // UTILS_TYPETRAITS_IFTHENELSE_HXX
