#ifndef UTILS_MISC_NULL_HXX
#define UTILS_MISC_NULL_HXX


namespace Utils
{
  /**
   * @brief
   */
  struct Null final
  {
    private:
      /**
       * @brief
       */
      using self_type = Null;


    public:
      /**
       * @brief
       */
      constexpr Null (void) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr Null (const self_type & that [[maybe_unused]]) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr Null (self_type && that [[maybe_unused]]) noexcept = delete;

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
  };
}


#endif // UTILS_MISC_NULL_HXX
