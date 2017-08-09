#include <cstddef> // std::size_t

#include <string> // std::string
#include <utility> // std::move
#include <ostream> // std::ostream

#include "source-location.hxx" // Utils::SourceLocation::*


namespace Utils
{
  SourceLocation::SourceLocation (const std::string & function, const std::string & file, std::size_t line) :
    function_ (function),
    file_ (file),
    line_ (line)
  { }


  SourceLocation::SourceLocation (const std::string & function, std::string && file, std::size_t line) :
    function_ (function),
    file_ (std::move (file)),
    line_ (line)
  { }


  SourceLocation::SourceLocation (std::string && function, const std::string & file, std::size_t line) :
    function_ (std::move (function)),
    file_ (file),
    line_ (line)
  { }


  SourceLocation::SourceLocation (std::string && function, std::string && file, std::size_t line) :
    function_ (std::move (function)),
    file_ (std::move (file)),
    line_ (line)
  { }


  const std::string &
  SourceLocation::function (void) const
  {
    return function_;
  }


  const std::string &
  SourceLocation::file (void) const
  {
    return file_;
  }


  std::size_t
  SourceLocation::line (void) const
  {
    return line_;
  }


  const SourceLocation::self_type &
  SourceLocation::operator = (const SourceLocation::self_type & that)
  {
    if (this != &that)
    {
      function_ = that.function_;
      file_ = that.file_;
      line_ = that.line_;
    }

    return *this;
  }


  const SourceLocation::self_type &
  SourceLocation::operator = (SourceLocation::self_type && that)
  {
    if (this != &that)
    {
      function_ = std::move (that.function_);
      file_ = std::move (that.file_);
      line_ = std::move (that.line_);
    }

    return *this;
  }


  std::ostream &
  operator << (std::ostream & output, const SourceLocation::self_type & self)
  {
    output
      << "SourceLocation{function:" << self.function_
      << ";file:" << self.file_
      << ";line:" << self.line_
      << '}';

    return output;
  }
}
