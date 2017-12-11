#ifndef UTILS_MISC_FLAGS_HXX
#define UTILS_MISC_FLAGS_HXX


#include <initializer_list> // std::initializer_list
#include <ios> // std::{hex, showbase}
#include <limits> // std::limits
#include <ostream> // std::ostream
#include <type_traits> // std::{is_enum, is_unsigned, underlying_type}


namespace Utils
{
  template <typename TFlag>
  class Flags final
  {
    static_assert (std::is_enum <TFlag>::value, "Type `TFlag' should be an enumeration type");

    static_assert (
      std::is_unsigned <typename std::underlying_type <TFlag>::type>::value,
      "Underlying type of `TFlag' enumeration should be an unsigned one"
    );


    public:
      using self_type = Flags;

      using flag_type = TFlag;

      using underlying_type = typename std::underlying_type <TFlag>::type;


      constexpr Flags (void) noexcept = default;

      constexpr Flags (const self_type & that) noexcept = default;


      constexpr Flags (std::initializer_list <flag_type> flags) noexcept
      {
        underlying_type new_flags { };
        for (flag_type flag : flags)
        {
          new_flags |= underlying_type (flag);
        }
        flags_ = flag_type (new_flags);
      }


      explicit constexpr Flags (flag_type flags) noexcept :
        flags_ (flags)
      { }


      explicit constexpr Flags (underlying_type flags) noexcept :
        flags_ (flags)
      { }


      constexpr bool
      has (flag_type flags) const noexcept
      {
        return (underlying_type (flags) == (underlying_type (flags) & underlying_type (flags_)));
      }


      constexpr bool
      has (std::initializer_list <flag_type> flags) const noexcept
      {
        for (flag_type flag : flags)
        {
          if (!has (flag))
          {
            return false;
          }
        }

        return true;
      }


      constexpr bool
      none (void) const noexcept
      {
        return (underlying_type (flags_) == underlying_zero);
      }


      constexpr bool
      any (void) const noexcept
      {
        return (underlying_type (flags_) > underlying_zero);
      }


      constexpr bool
      all (void) const noexcept
      {
        return (underlying_type (flags_) == std::numeric_limits <underlying_type>::max ());
      }


      constexpr void
      set (flag_type flags) noexcept
      {
        flags_ = underlying_type (flags_) | underlying_type (flags);
      }


      constexpr void
      unset (flag_type flags) noexcept
      {
        flags_ = underlying_type (flags_) ^ underlying_type (flags);
      }


      constexpr void
      reset (void) noexcept
      {
        flags_ = flag_zero;
      }


      constexpr explicit operator flag_type (void) const noexcept
      {
        return flags_;
      }


      constexpr explicit operator underlying_type (void) const noexcept
      {
        return underlying_type (flags_);
      }


      constexpr explicit operator bool (void) const noexcept
      {
        return any ();
      }


      constexpr const self_type &
      operator = (const self_type & that) noexcept
      {
        if (this != &that)
        {
          flags_ = that.flags_;
        }

        return *this;
      }


      constexpr const self_type &
      operator = (flag_type flags) noexcept
      {
        flags_ = flags;

        return *this;
      }


      constexpr const self_type &
      operator = (underlying_type flags) noexcept
      {
        flags_ = flags;

        return *this;
      }


      constexpr bool
      operator == (const self_type & that) const noexcept
      {
        return (flags_ == that.flags_);
      }


      constexpr bool
      operator == (flag_type flags) const noexcept
      {
        return (flags_ == flags);
      }


      constexpr bool
      operator == (underlying_type flags) const noexcept
      {
        return (underlying_type (flags_) == flags);
      }


      friend std::ostream &
      operator << (std::ostream & output, const self_type & self)
      {
        output
          << "Flags{"
          << std::hex << std::showbase << underlying_type (self)
          << '}';

        return output;
      }


    private:
      flag_type flags_ { };

      inline static constexpr flag_type flag_zero { };

      inline static constexpr underlying_type underlying_zero { };
  };
}


#endif // UTILS_MISC_FLAGS_HXX
