#include <cstdlib> // std::abort

#include "assertion.hxx" // Assertion::*
#include "crash-program.hxx" // crashProgram


namespace Utils
{
  [[noreturn]] void
  Assertion::crashProgram_ (void) const noexcept
  {
    crashProgram ();
  }


  void
  Assertion::assertionFailure_ (void) const noexcept
  { }
}
