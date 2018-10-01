#include "source-location.hxx"  // SourceLocation::*

#include <ostream>  // std::ostream


namespace Utils
{
  std::ostream &
  operator << (std::ostream & output, const SourceLocation::self_type & self)
  {
    output
      << "SourceLocation{"
      << "function:" << self.function_
      << ";file:" << self.file_
      << ";line:" << self.line_
      << '}';

    return output;
  }
}
