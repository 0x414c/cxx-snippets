#include "source-location.hxx"  // SourceLocation::*

#include <ostream>  // std::ostream


namespace Utils
{
  std::ostream &
  operator << (std::ostream & output, const SourceLocation::self_type & self)
  {
    output
      << "SourceLocation{function:" << self.function ()
      << ";file:" << self.file ()
      << ";line:" << self.line () << '}';

    return output;
  }
}
