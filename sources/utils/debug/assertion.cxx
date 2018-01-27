#include <cstdlib> // std::abort

#include "assertion.hxx" // Assertion::*


namespace Utils
{
  void
  Assertion::crashProgram_ (void) const noexcept
  {
    std::abort ();
  }


  void
  Assertion::assertionFailure_ (void) const noexcept
  { }
}
