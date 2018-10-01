#ifndef UTILS_META_CHOOSE_HXX
#define UTILS_META_CHOOSE_HXX


#include "common.hxx"  // TypeOf, ValueOf
#include "if-then-else.hxx"  // IfThenElseT


namespace Utils
{
  /**
   * @brief
   * @tparam TIfTrue
   * @tparam TIfFalse
   */
  template <typename TIfTrue, typename ... TIfFalse>
  struct Choose
  {
    /**
     * @brief
     */
    using type = IfThenElseT <ValueOf <TIfTrue>, TIfTrue, TIfFalse ...>;
  };


  /**
   * @brief
   */
  template <typename TIfTrue, typename ... TIfFalse>
  using ChooseT = TypeOf <Choose <TIfTrue, TIfFalse ...>>;
}


#endif  // UTILS_META_CHOOSE_HXX
