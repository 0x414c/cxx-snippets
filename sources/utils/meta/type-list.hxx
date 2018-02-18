#ifndef UTILS_META_TYPELIST_HXX
#define UTILS_META_TYPELIST_HXX


#include <type_traits> // std::{is_same, negation_v}

#include "common.hxx" // TypeOf, ValueOf
#include "if-then-else.hxx" // IfThenElseT
#include "../misc/null.hxx" // Null


namespace Utils
{
  /**
   * @brief
   * @tparam TType
   * @tparam TTail
   */
  template <typename TType, typename TTail>
  struct TypeList
  {
    /**
     * @brief
     */
    using type = TType;

    /**
     * @brief
     */
    using tail_type = TTail;
  };


  /**
   * @brief
   * @tparam TTypes
   */
  template <typename ... TTypes>
  struct MakeTypeList;


  /**
   * @brief
   */
  template <>
  struct MakeTypeList <>
  {
    /**
     * @brief
     */
    using type = Null;
  };


  /**
   * @brief
   * @tparam TFirst
   * @tparam TRest
   */
  template <typename TFirst, typename ... TRest>
  struct MakeTypeList <TFirst, TRest ...>
  {
    /**
     * @brief
     */
    using type = TypeList <TFirst, TypeOf <MakeTypeList <TRest ...>>>;
  };


  /**
   * @brief
   */
  template <typename ... TTypes>
  using MakeTypeListT = TypeOf <MakeTypeList <TTypes ...>>;


  /**
   * @brief
   * @tparam TTypeList
   */
  template <typename TTypeList>
  struct Tail
  {
    /**
     * @brief
     */
    using type = typename TTypeList::tail_type;
  };


  /**
   * @brief
   */
  template <typename TTypeList>
  using TailT = TypeOf <Tail <TTypeList>>;


  /**
   * @brief
   * @tparam TTypeList
   * @tparam TCompare
   */
  template <typename TTypeList, typename TCompare>
  struct FindIf;


  namespace FindIfInternals_
  {
    /**
     * @brief
     * @tparam TCondition
     * @tparam TTypeList
     * @tparam TCompare
     */
    template <bool TCondition, typename TTypeList, typename TCompare>
    struct FindIfImpl_
    {
      /**
       * @brief
       */
      using type = TTypeList;
    };


    /**
     * @brief
     * @tparam TTypeList
     * @tparam TCompare
     */
    template <typename TTypeList, typename TCompare>
    struct FindIfImpl_ <false, TTypeList, TCompare>
    {
      /**
       * @brief
       */
      using type = TypeOf <FindIf <TailT <TTypeList>, TCompare>>;
    };
  }


  /**
   * @brief
   * @tparam TTypeList
   * @tparam TCompare
   */
  template <typename TTypeList, typename TCompare>
  struct FindIf
  {
    /**
     * @brief
     */
    using type = TypeOf <FindIfInternals_::FindIfImpl_ <ValueOf <TCompare, TypeOf <TTypeList>>, TTypeList, TCompare>>;
  };


  /**
   * @brief
   * @tparam TCompare
   */
  template <typename TCompare>
  struct FindIf <Null, TCompare>
  {
    /**
     * @brief
     */
    using type = Null;
  };


  /**
   * @brief
   */
  template <typename TTypeList, typename TCompare>
  using FindIfT = TypeOf <FindIf <TTypeList, TCompare>>;


  /**
   * @brief
   * @tparam TTypeList
   * @tparam TCompare
   */
  template <typename TTypeList, typename TCompare>
  struct Has
  {
    /**
     * @brief
     */
    static constexpr bool value { std::negation_v <std::is_same <FindIfT <TTypeList, TCompare>, Null>> };
  };


  /**
   * @brief
   * @tparam TTypeList
   * @tparam TCompare
   * @return
   */
  template <typename TTypeList, typename TCompare>
  inline constexpr bool HasV (ValueOf <Has <TTypeList, TCompare>>);
}


#endif // UTILS_META_TYPELIST_HXX
