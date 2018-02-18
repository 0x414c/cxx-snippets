#ifndef UTILS_META_COMMON_HXX
#define UTILS_META_COMMON_HXX


namespace Utils
{
  namespace TypeOfInternals_
  {
    /**
     * @brief
     * @tparam TMetafunction
     * @tparam TArgs
     */
    template <typename TMetafunction, typename ... TArgs>
    struct TypeOfImpl_
    {
      /**
       * @brief
       */
      using type = typename TMetafunction::template type <TArgs ...>;
    };


    /**
     * @brief
     * @tparam TMetafunction
     */
    template <typename TMetafunction>
    struct TypeOfImpl_ <TMetafunction>
    {
      /**
       * @brief
       */
      using type = typename TMetafunction::type;
    };
  }


  /**
   * @brief
   */
  template <typename TMetafunction, typename ... TArgs>
  using TypeOf = typename TypeOfInternals_::TypeOfImpl_ <TMetafunction, TArgs ...>::type;


  namespace ValueOfInternals_
  {
    /**
     * @brief
     * @tparam TMetafunction
     * @tparam TArgs
     */
    template <typename TMetafunction, typename ... TArgs>
    struct ValueOfImpl_
    {
      /**
       * @brief
       */
      static constexpr auto value { TMetafunction::template value <TArgs ...> };
    };


    /**
     * @brief
     * @tparam TMetafunction
     */
    template <typename TMetafunction>
    struct ValueOfImpl_ <TMetafunction>
    {
      /**
       * @brief
       */
      static constexpr auto value { TMetafunction::value };
    };
  }


  /**
   * @brief
   */
  template <typename TMetafunction, typename ... TArgs>
  inline constexpr auto ValueOf { ValueOfInternals_::ValueOfImpl_ <TMetafunction, TArgs ...>::value };
}


#endif // UTILS_META_COMMON_HXX
