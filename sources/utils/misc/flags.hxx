#ifndef UTILS_MISC_FLAGS_HXX
#define UTILS_MISC_FLAGS_HXX


#include <initializer_list> // std::initializer_list
#include <ios>  // std::{hex, showbase}
#include <limits>  // std::numeric_limits
#include <ostream>  // std::ostream
#include <type_traits>  // std::{is_enum_v, is_unsigned_v, underlying_type_t}


namespace Utils
{
  /**
   * @brief
   * @tparam TFlag
   */
  template <typename TFlag>
  class Flags final
  {
    static_assert (std::is_enum_v <TFlag>);
    static_assert (std::is_unsigned_v <std::underlying_type_t <TFlag>>);


    public:
      /**
       * @brief
       */
      using flag_type = TFlag;


    private:
      /**
       * @brief
       */
      using underlying_type = std::underlying_type_t <flag_type>;

      /**
       * @brief
       */
      using self_type = Flags;


    public:
      /**
       * @brief
       */
      constexpr Flags () noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr Flags (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr Flags (self_type && that [[maybe_unused]]) noexcept = default;


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
      constexpr Flags (const std::initializer_list <flag_type> & flags) noexcept :
        flags_ (combine_ (flags))
      { }


      /**
       * @brief
       * @param flags
       * @return
       */
      [[nodiscard]] constexpr bool
      has (flag_type flags) const noexcept
      {
        return (underlying_type (flags_) & underlying_type (flags)) != underlying_zero_;
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      [[nodiscard]] constexpr bool
      has (const std::initializer_list <flag_type> & flags) const noexcept
      {
        for (const flag_type flag : flags)
        {
          if (! has (flag))
          {
            return false;
          }
        }

        return true;
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      [[nodiscard]] constexpr bool
      is (flag_type flags) const noexcept
      {
        return flags_ == flags;
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      [[nodiscard]] constexpr bool
      is (const std::initializer_list <flag_type> & flags) const noexcept
      {
        return is (combine_ (flags));
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr bool
      none () const noexcept
      {
        return underlying_type (flags_) == underlying_zero_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr bool
      any () const noexcept
      {
        return ! none ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr bool
      all () const noexcept
      {
        return underlying_type (flags_) == std::numeric_limits <underlying_type>::max ();
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr void
      set (flag_type flags) noexcept
      {
        flags_ = flag_type (underlying_type (flags_) | underlying_type (flags));
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr void
      set (const std::initializer_list <flag_type> & flags) noexcept
      {
        set (combine_ (flags));
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr void
      unset (flag_type flags) noexcept
      {
        flags_ = flag_type (underlying_type (flags_) ^ underlying_type (flags));
      }


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr void
      unset (const std::initializer_list <flag_type> & flags) noexcept
      {
        unset (combine_ (flags));
      }


      /**
       * @brief
       * @return
       */
      constexpr void
      reset () noexcept
      {
        flags_ = flag_type (underlying_zero_);
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr explicit operator bool () const noexcept
      {
        return any ();
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr explicit operator flag_type () const noexcept
      {
        return flags_;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that) noexcept = default;


      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr self_type &
      operator = (flag_type flags) noexcept
      {
        flags_ = flags;

        return * this;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator == (const self_type & that) const noexcept
      {
        if (this == & that)
        {
          return true;
        }

        return flags_ == that.flags_;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator != (const self_type & that) const
      {
        return ! operator == (that);
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator == (flag_type flags) const noexcept
      {
        return flags_ == flags;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      [[nodiscard]] constexpr bool
      operator != (flag_type flags) const
      {
        return ! operator == (flags);
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
        output << "Flags{" << std::hex << std::showbase << underlying_type (self.flags_) << '}';

        return output;
      }


    private:
      /**
       * @brief
       * @param flags
       * @return
       */
      constexpr flag_type
      combine_ (const std::initializer_list <flag_type> & flags) const noexcept
      {
        underlying_type all_flags (underlying_zero_);
        for (const flag_type flag : flags)
        {
          all_flags |= underlying_type (flag);
        }

        return flag_type (all_flags);
      }


      /**
       * @brief
       */
      flag_type flags_ { };


      /**
       * @brief
       */
      static constexpr underlying_type underlying_zero_ { };
  };
}


#endif  // UTILS_MISC_FLAGS_HXX
