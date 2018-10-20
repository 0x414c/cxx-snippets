#ifndef UTILS_LANG_WEAKSCOPEDENUM_HXX
#define UTILS_LANG_WEAKSCOPEDENUM_HXX


#include <type_traits>  // std::{enable_if_t, is_enum_v, underlying_type_t}


namespace Utils::Lang::WeakScopedEnum
{
  /**
   * @brief
   * @tparam TEnum
   * @param x
   * @return
   */
  template <typename TEnum, std::enable_if_t <std::is_enum_v <TEnum>> ...>
  [[nodiscard]] constexpr std::underlying_type_t <TEnum>
  operator + (TEnum x) noexcept
  {
    return std::underlying_type_t <TEnum> (x);
  }
}


#endif  // UTILS_LANG_WEAKSCOPEDENUM_HXX
