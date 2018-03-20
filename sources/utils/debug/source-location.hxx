#ifndef UTILS_LOGGING_SOURCELOCATION_HXX
#define UTILS_LOGGING_SOURCELOCATION_HXX


#include <cstddef> // std::size_t

// #include <string_view> // std::string_view
#include <ostream> // std::ostream

#include "../config/source-location.hxx" // Config::Utils::SourceLocation::{Default_file, Default_function}
#include "../containers/c-string.hxx" // CString


namespace Utils
{
  /**
   * @brief
   */
  struct SourceLocation final
  {
    private:
      /**
       * @brief
       */
      using self_type = SourceLocation;


    public:
      /**
       * @brief
       */
      constexpr SourceLocation (void) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr SourceLocation (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       */
      constexpr SourceLocation (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param function
       * @param file
       * @param line
       */
      explicit constexpr SourceLocation (const CString & function, const CString & file, std::size_t line) noexcept :
        function_ (function),
        file_ (file),
        line_ (line)
      { }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const CString &
      function (void) const noexcept
      {
        return function_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr const CString &
      file (void) const noexcept
      {
        return file_;
      }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr std::size_t
      line (void) const noexcept
      {
        return line_;
      }


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

      /**
       * @brief
       * @param output
       * @param self
       * @return
       */
      friend std::ostream &
      operator << (std::ostream & output, const self_type & self);


    private:
      /**
       * @brief
       */
      CString function_ { Config::Utils::SourceLocation::Default_function };

      /**
       * @brief
       */
      CString file_ { Config::Utils::SourceLocation::Default_file };

      /**
       * @brief
       */
      std::size_t line_ { };
  };
}


/**
 * @brief
 * TODO: [0;0] Use
 *   `Q_FUNC_INFO' (from `QtGlobal') or
 *   `BOOST_CURRENT_FUNCTION' (from `boost/current_function.hpp') or
 *   `__PRETTY_FUNCTION__' or
 *   `__FUNCTION__' or
 *   `__func__'?
 */
#define CURRENT_SOURCE_LOCATION() (::Utils::SourceLocation ((__PRETTY_FUNCTION__), (__FILE__), (__LINE__)))


#endif // UTILS_LOGGING_SOURCELOCATION_HXX
