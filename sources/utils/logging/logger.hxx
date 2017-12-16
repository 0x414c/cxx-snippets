#ifndef UTILS_LOGGING_LOGGER_HXX
#define UTILS_LOGGING_LOGGER_HXX


#include <cstddef> // std::size_t

//#include <string_view> // std::string_view
#include <iostream> // std::{ostream, clog}

#include <fmt/format.h> // fmt::print
#include <fmt/ostream.h> // fmt::print[std::ostream]

#include "../containers/c-string.hxx" // CString
#include "source-location.hxx" // SourceLocation


namespace Utils
{
  /**
   * @brief
   */
  class Logger final
  {
    public:
      /**
       * @brief
       * @tparam TArgs
       * @param prefix
       * @param format
       * @param args
       */
      template <typename ... TArgs>
      static void
      log (const CString & prefix, const CString & format, const TArgs & ... args)
      {
        ++message_id_;

        fmt::print (
          output_stream_, "{0:d}/{1:s} {2:s}\n",
          message_id_, prefix, fmt::format (format.data (), args ...)
        );
      }


      /**
       * @brief
       * @tparam TArgs
       * @param source_location
       * @param prefix
       * @param format
       * @param args
       */
      template <typename ... TArgs>
      static void
      log_Detailed (
        const SourceLocation & source_location,
        const CString & prefix, const CString & format, const TArgs & ... args
      )
      {
        ++message_id_;

        fmt::print (
          output_stream_, "{0:d}/{1:s} {2:s} (in `{3:s}' at `{4:s}:{5:d}')\n",
          message_id_, prefix, fmt::format (format.data (), args ...),
          source_location.function (), source_location.file (), source_location.line ()
        );
      }


    private:
      /**
       * @brief
       */
      inline static std::size_t message_id_ { };


      /**
       * @brief
       */
      static constexpr std::ostream & output_stream_ { std::clog };
  };
}


#endif // UTILS_LOGGING_LOGGER_HXX
