#ifndef UTILS_TYPETRAITS_CHOOSE_HXX
#define UTILS_TYPETRAITS_CHOOSE_HXX


#include "if-then-else.hxx" // IfThenElse


namespace Utils
{
  /**
   * @brief
   * @tparam TIfTrue
   * @tparam TIfFalse
   */
  template <typename TIfTrue, typename ... TIfFalse>
  struct Choose final
  {
    /**
     * @brief
     */
    using type = typename IfThenElse <TIfTrue::condition, TIfTrue, TIfFalse ...>::type;
  };
}


#endif // UTILS_TYPETRAITS_CHOOSE_HXX
