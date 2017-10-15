#ifndef UTILS_LOGGING_LOGGER_HXX
#define UTILS_LOGGING_LOGGER_HXX


#include <cstddef> // std::size_t

#include <string> // std::string
#include <iostream> // std::{ostream, clog}

#include <fmt/format.h> // fmt::print
#include <fmt/ostream.h> // fmt::print[std::ostream]

#include "../config/logger.hxx" // Config::Utils::Logger
#include "source-location.hxx" // SourceLocation


namespace Utils
{
  /**
   * @brief
   */
  class Logger
  {
    public:
      /**
       * @brief
       * TODO: [0;0] Use
       *   `Q_FUNC_INFO' (from `QtGlobal') or
       *   `BOOST_CURRENT_FUNCTION' (from `boost/current_function.hpp') or
       *   `__PRETTY_FUNCTION__' or
       *   `__FUNCTION__' or
       *   `__func__'?
       * @param function
       * @param file
       * @param line
       * @param prefix
       * @param format
       * @param args
       */
      template <typename ... TArgs>
      static void
      printLog_Detailed (
        const SourceLocation & source_location,
        const std::string & prefix, const std::string & format, const TArgs & ... args
      )
      {
        ++message_id_;

        fmt::print (
          output_stream_, "{0:d}/{1:s} {2:s}\n{{in `{3:s}' at `{4:s}:{5:d}'}}\n",
          message_id_, prefix, fmt::format (format, args ...),
          source_location.function (), source_location.file (), source_location.line ()
        );
      }

      /**
       * @brief
       * @param description
       * @param prefix
       * @param format
       * @param args
       */
      template <typename ... TArgs>
      static void
      printLog_Short (const std::string & prefix, const std::string & format, const TArgs & ... args)
      {
        ++message_id_;

        fmt::print (
          output_stream_, "{0:d}/{1:s} {2:s}\n",
          message_id_, prefix, fmt::format (format, args ...)
        );
      }


    private:
      /**
       * @brief
       */
      inline static std::size_t message_id_ { 0 };


      /**
       * @brief
       */
      static constexpr std::ostream & output_stream_ { std::clog };
  };
}


#endif // UTILS_LOGGING_LOGGER_HXX
