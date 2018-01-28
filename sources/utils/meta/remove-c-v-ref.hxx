#ifndef UTILS_META_REMOVECVREF_HXX
#define UTILS_META_REMOVECVREF_HXX


#include <type_traits> // std::{remove_cv, remove_reference_t}

#include "common.hxx" // TypeOf


namespace Utils
{
  /**
   * @brief
   */
  template <typename TType>
  using RemoveCVRef = std::remove_cv <std::remove_reference_t <TType>>;


  /**
   * @brief
   */
  template <typename TType>
  using RemoveCVRefT = TypeOf <RemoveCVRef <TType>>;
}


#endif // UTILS_META_REMOVECVREF_HXX
