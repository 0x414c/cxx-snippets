#ifndef UTILS_META_ALWAYSFALSE_HXX
#define UTILS_META_ALWAYSFALSE_HXX


#include "common.hxx" // ValueOf


namespace Utils
{
  /**
   * @brief
   */
  template <typename TType, TType ... TValues>
  struct AlwaysFalse
  {
    /**
     * @brief
     */
    static constexpr bool value { false };
  };


  /**
   * @brief
   */
  template <typename TType, TType ... TValues>
  inline constexpr bool AlwaysFalseV (ValueOf <AlwaysFalse <TType, TValues ...>>);
}


#endif // UTILS_META_ALWAYSFALSE_HXX
