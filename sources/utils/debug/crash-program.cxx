#include "crash-program.hxx"  // crashProgram

#include <cstdlib>  // std::abort

#include "../config/logger.hxx"  // Config::Utils::Logger::Fatal_prefix
#include "../containers/c-string.hxx"  // CString
#include "../logging/logger.hxx"  // Logger::{log, log_Detailed}
#include "source-location.hxx"  // SourceLocation


namespace Utils
{
  [[noreturn]] void
  crashProgram (void) noexcept
  {
    std::abort ();
  }


  [[noreturn]] void
  crashProgram (const CString & message) noexcept
  {
    Logger::log (Config::Utils::Logger::Fatal_prefix, "Fatal error: `{0:s}'", message);

    crashProgram ();
  }


  [[noreturn]] void
  crashProgram (const CString & message, const SourceLocation & source_location) noexcept
  {
    Logger::log_Detailed (source_location, Config::Utils::Logger::Fatal_prefix, "Fatal error: `{0:s}'", message);

    crashProgram ();
  }
}
