#ifndef UTILS_LOGGING_SOURCELOCATION_HXX
#define UTILS_LOGGING_SOURCELOCATION_HXX


#include <cstddef> // std::size_t

#include <string> // std::string
#include <ostream> // std::ostream


namespace Utils
{
  /**
   * @brief
   */
  class SourceLocation final
  {
    public:
      /**
       * @brief
       */
      using self_type = SourceLocation;


      /**
       * @brief
       */
      SourceLocation (void);

      /**
       * @brief
       * @param that
       */
      SourceLocation (const self_type & that) = default;

      /**
       * @brief
       * @param that
       */
      SourceLocation (self_type && that) = default;

      /**
       * @brief
       * @param function
       * @param file
       * @param line
       */
      explicit SourceLocation (const std::string & function, const std::string & file, std::size_t line);

      /**
       * @brief
       * @param function
       * @param file
       * @param line
       */
      explicit SourceLocation (const std::string & function, std::string && file, std::size_t line);

      /**
       * @brief
       * @param function
       * @param file
       * @param line
       */
      explicit SourceLocation (std::string && function, const std::string & file, std::size_t line);

      /**
       * @brief
       * @param function
       * @param file
       * @param line
       */
      explicit SourceLocation (std::string && function, std::string && file, std::size_t line);

      /**
       * @brief
       * @return
       */
      const std::string &
      function (void) const;

      /**
       * @brief
       * @return
       */
      const std::string &
      file (void) const;

      /**
       * @brief
       * @return
       */
      std::size_t
      line (void) const noexcept;

      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (const self_type & that);

      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (self_type && that);

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
      std::string function_;

      /**
       * @brief
       */
      std::string file_;

      /**
       * @brief
       */
      std::size_t line_;
  };
}


#define CURRENT_SOURCE_LOCATION() (::Utils::SourceLocation (__PRETTY_FUNCTION__, __FILE__, __LINE__))


#endif // UTILS_LOGGING_SOURCELOCATION_HXX
