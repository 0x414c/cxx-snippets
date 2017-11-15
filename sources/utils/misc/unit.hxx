#ifndef UTILS_MISC_UNIT_HXX
#define UTILS_MISC_UNIT_HXX


#include <ostream> // std::ostream


namespace Utils
{
  /**
   * @brief
   */
  struct Unit final
  {
    /**
     * @brief
     */
    using self_type = Unit;


    /**
     * @brief
     * @param that
     * @return
     */
    constexpr bool
    operator == ([[maybe_unused]] const self_type & that) const
    {
      return true;
    }


    /**
     * @brief
     * @param that
     * @return
     */
    constexpr bool
    operator != (const self_type & that) const
    {
      return !operator == (that);
    }


    /**
     * @brief
     * @param that
     * @return
     */
    constexpr bool
    operator < ([[maybe_unused]] const self_type & that) const
    {
      return false;
    }


    /**
     * @brief
     * @param that
     * @return
     */
    constexpr bool
    operator > ([[maybe_unused]] const self_type & that) const
    {
      return false;
    }


    /**
     * @brief
     * @param that
     * @return
     */
    constexpr bool
    operator <= (const self_type & that) const
    {
      return !operator < (that);
    }


    /**
     * @brief
     * @param that
     * @return
     */
    constexpr bool
    operator >= (const self_type & that) const
    {
      return !operator > (that);
    }


    /**
     * @brief
     * @param output
     * @param unit
     * @return
     */
    friend std::ostream &
    operator << (std::ostream & output, [[maybe_unused]] const self_type & unit)
    {
      output << "Unit{}";

      return output;
    }
  };
}


#endif // UTILS_MISC_UNIT_HXX
