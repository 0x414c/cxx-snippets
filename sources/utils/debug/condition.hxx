#ifndef UTILS_DEBUG_CONDITION_HXX
#define UTILS_DEBUG_CONDITION_HXX


#include <ostream>  // std::ostream

#include "../containers/c-string.hxx"  // CString
#include "source-location.hxx"  // CURRENT_SOURCE_LOCATION, SourceLocation


namespace Utils
{
  /**
   * @brief
   */
  struct Condition final
  {
    private:
      /**
       * @brief
       */
      using self_type = Condition;


    public:
      /**
       * @brief
       */
      constexpr Condition () noexcept = delete;

      /**
       * @brief
       * @param that
       */
      constexpr Condition (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr Condition (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param value
       * @param as_text
       * @param source_location
       */
      constexpr explicit Condition (
        bool value, const CString & as_text, const SourceLocation & source_location
      ) noexcept :
        value_ (value),
        source_ (as_text),
        source_location_ (source_location)
      { }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr bool
      value () const noexcept
      {
        return value_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const CString &
      source () const noexcept
      {
        return source_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const SourceLocation &
      sourceLocation () const noexcept
      {
        return source_location_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr explicit operator bool () const noexcept
      {
        return value ();
      }


      /**
       * @brief
       * @param output
       * @param self
       * @return
       */
      friend std::ostream &
      operator << (std::ostream & output, const self_type & self);

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = default;


    private:
      /**
       * @brief
       */
      bool value_;

      /**
       * @brief
       */
      CString source_;

      /**
       * @brief
       */
      SourceLocation source_location_;
  };
}


/**
 * @brief
 */
#define CONDITION(condition) (::Utils::Condition ((condition), (STRINGIFY_E (condition)), (CURRENT_SOURCE_LOCATION ())))


#endif  // UTILS_DEBUG_CONDITION_HXX
