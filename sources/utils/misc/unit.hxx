#ifndef UTILS_MISC_UNIT_HXX
#define UTILS_MISC_UNIT_HXX


#include <ostream> // std::ostream


namespace Utils
{
  struct Unit
  {
    using self_type = Unit;


    constexpr bool
    operator == (const self_type & that) const
    {
      return true;
    }


    constexpr bool
    operator != (const self_type & that) const
    {
      return !operator == (that);
    }


    constexpr bool
    operator < (const self_type & that) const
    {
      return false;
    }


    constexpr bool
    operator > (const self_type & that) const
    {
      return false;
    }


    constexpr bool
    operator <= (const self_type & that) const
    {
      return !operator < (that);
    }


    constexpr bool
    operator >= (const self_type & that) const
    {
      return !operator > (that);
    }


    friend std::ostream &
    operator << (std::ostream & output, const self_type & unit)
    {
      output << "Unit{}";

      return output;
    }
  };
}


#endif // UTILS_MISC_UNIT_HXX
