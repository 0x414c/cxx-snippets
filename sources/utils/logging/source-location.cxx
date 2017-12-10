#include <ostream> // std::ostream

#include "source-location.hxx" // SourceLocation


namespace Utils
{
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
