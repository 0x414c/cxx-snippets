#include "condition.hxx"  // Condition::*

#include <ios>  // std::boolalpha
#include <ostream>  // std::ostream


namespace Utils
{
  std::ostream &
  operator << (std::ostream & output, const Condition::self_type & self)
  {
    output
      << "Condition{value:" << std::boolalpha << self.value ()
      << ";source:" << self.source ()
      << ";source_location:" << self.sourceLocation () << '}';

    return output;
  }
}
