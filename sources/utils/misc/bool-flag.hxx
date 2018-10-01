#ifndef UTILS_MISC_BOOLFLAG_HXX
#define UTILS_MISC_BOOLFLAG_HXX


namespace Utils
{
  /**
   * @brief
   * @tparam TTag
   */
  template <typename TTag>
  class BoolFlag final
  {
    private:
      /**
       * @brief
       */
      using self_type = BoolFlag;


    public:
      /**
       * @brief
       */
      constexpr BoolFlag (void) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr BoolFlag (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr BoolFlag (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param value
       */
      constexpr explicit BoolFlag (bool value) noexcept :
        value_ (value)
      { }


      /**
       * @brief
       * @tparam TThatFlag
       * @param that
       */
      template <typename TThatFlag>
      constexpr explicit BoolFlag (BoolFlag <TThatFlag> that) noexcept :
        value_ (that.value_)
      { }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr explicit operator bool (void) const noexcept
      {
        return value_;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = delete;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = delete;


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator == (const self_type & that) const noexcept
      {
        return (value_ == that.value_);
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator != (const self_type & that) const noexcept
      {
        return (! operator == (that));
      }


    private:
      /**
       * @brief
       */
      const bool value_ { };
  };
}


#endif // UTILS_MISC_BOOLFLAG_HXX
