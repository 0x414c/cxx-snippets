#ifndef UTILS_TYPETRAITS_ALWAYSFALSE_HXX
#define UTILS_TYPETRAITS_ALWAYSFALSE_HXX


#include <cstddef> // std::int64_t

#include <type_traits> // std::false_type


namespace Utils
{
  /**
   * @brief
   */
  template <typename T, T ...>
  struct AlwaysFalse final :
    std::false_type
  { };
}


#endif // UTILS_TYPETRAITS_ALWAYSFALSE_HXX
