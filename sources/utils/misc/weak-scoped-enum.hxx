#ifndef UTILS_MISC_WEAKSCOPEDENUM_HXX
#define UTILS_MISC_WEAKSCOPEDENUM_HXX


#include <type_traits> // std::{enable_if_t, is_enum_v, underlying_type_t}


namespace Utils
{
  /**
   * @brief
   * @tparam TEnum
   * @param enum
   * @return
   */
  template <typename TEnum, std::enable_if_t <std::is_enum_v <TEnum>> ...>
  constexpr std::underlying_type_t <TEnum>
  operator + (TEnum x) noexcept
  {
    return std::underlying_type_t <TEnum> (x);
  }
}


#endif // UTILS_MISC_WEAKSCOPEDENUM_HXX
