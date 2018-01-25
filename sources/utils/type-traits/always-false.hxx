#ifndef UTILS_TYPETRAITS_ALWAYSFALSE_HXX
#define UTILS_TYPETRAITS_ALWAYSFALSE_HXX


#include <type_traits> // std::false_type

#include "common.hxx" // ValueOf


namespace Utils
{
  /**
   * @brief
   */
  template <typename TType, TType ...>
  struct AlwaysFalse :
    public std::false_type
  { };


  /**
   * @brief
   */
  template <typename TType, TType ... TValues>
  inline constexpr bool AlwaysFalseV = ValueOf <AlwaysFalse <TType, TValues ...>>;
}


#endif // UTILS_TYPETRAITS_ALWAYSFALSE_HXX
