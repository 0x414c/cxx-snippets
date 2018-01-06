#ifndef UTILS_MISC_FLAGS_HXX
#define UTILS_MISC_FLAGS_HXX


#include <initializer_list> // std::initializer_list
#include <ios> // std::{hex, showbase}
#include <limits> // std::numeric_limits
#include <ostream> // std::ostream
#include <type_traits> // std::{is_enum_v, underlying_type_t, is_unsigned_v}


namespace Utils
{
  /**
   * @brief
   * @tparam TFlag
   */
  template <typename TFlag>
  class Flags final
  {
    static_assert (std::is_enum_v <TFlag>, "Type `TFlag' should be an enumeration type");

    static_assert (
      std::is_unsigned_v <std::underlying_type_t <TFlag>>,
      "Underlying type of `TFlag' enumeration should be an unsigned one"
    );


    public:
      /**
       * @brief
       */
      using flag_type = TFlag;

      /**
       * @brief
       */
      using underlying_type = std::underlying_type_t <TFlag>;

      /**
       * @brief
       */
      using self_type = Flags;


      /**
       * @brief
       */
      constexpr Flags (void) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr Flags (const self_type & that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param flags
       */
      constexpr Flags (std::initializer_list <flag_type> flags) noexcept
      {
        underlying_type new_flags { };
        for (flag_type flag : flags)
        {
          new_flags |= underlying_type (flag);
        }
        flags_ = flag_type (new_flags);
      }


      /**
       * @brief
       * @param flags
       */
      explicit constexpr Flags (flag_type flags) noexcept :
        flags_ (flags)
      { }


      /**
       * @brief
       * @param flags
       */
      explicit constexpr Flags (underlying_type flags) noexcept :
        flags_ (flags)
      { }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr bool
      has (flag_type flags) const noexcept
      {
        return ((underlying_type (flags) & underlying_type (flags_)) != underlying_zero_);
      }


      /**
       * @brief
       * @param flags
       * @return
       */
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


      /**
       * @brief
       * @return
       */
      constexpr bool
      none (void) const noexcept
      {
        return (underlying_type (flags_) == underlying_zero_);
      }


      /**
       * @brief
       * @return
       */
      constexpr bool
      any (void) const noexcept
      {
        return !none ();
      }


      /**
       * @brief
       * @return
       */
      constexpr bool
      all (void) const noexcept
      {
        return (underlying_type (flags_) == std::numeric_limits <underlying_type>::max ());
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr void
      set (flag_type flags) noexcept
      {
        flags_ = underlying_type (flags_) | underlying_type (flags);
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr void
      unset (flag_type flags) noexcept
      {
        flags_ = underlying_type (flags_) ^ underlying_type (flags);
      }


      /**
       * @brief
       * @return
       */
      constexpr void
      reset (void) noexcept
      {
        flags_ = flag_zero_;
      }


      /**
       * @brief
       * @return
       */
      constexpr explicit operator flag_type (void) const noexcept
      {
        return flags_;
      }


      /**
       * @brief
       * @return
       */
      constexpr explicit operator underlying_type (void) const noexcept
      {
        return underlying_type (flags_);
      }


      /**
       * @brief
       * @return
       */
      constexpr explicit operator bool (void) const noexcept
      {
        return any ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that) noexcept
      {
        if (this != &that)
        {
          flags_ = that.flags_;
        }

        return *this;
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr self_type &
      operator = (flag_type flags) noexcept
      {
        flags_ = flags;

        return *this;
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr self_type &
      operator = (underlying_type flags) noexcept
      {
        flags_ = flags;

        return *this;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr bool
      operator == (const self_type & that) const noexcept
      {
        return ((this == &that) || (flags_ == that.flags_));
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr bool
      operator == (flag_type flags) const noexcept
      {
        return (flags_ == flags);
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr bool
      operator == (underlying_type flags) const noexcept
      {
        return (underlying_type (flags_) == flags);
      }


      /**
       * @brief
       * @param output
       * @param self
       * @return
       */
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
      /**
       * @brief
       */
      flag_type flags_ { };

      /**
       * @brief
       */
      static constexpr flag_type flag_zero_ { };

      /**
       * @brief
       */
      static constexpr underlying_type underlying_zero_ { };
  };
}


#endif // UTILS_MISC_FLAGS_HXX
