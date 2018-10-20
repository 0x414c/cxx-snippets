#include "assertion.hxx"  // Assertion::*

#include <cstdlib>  // std::abort

#include "crash-program.hxx"  // crashProgram


namespace Utils
{
  [[noreturn]] void
  Assertion::crashProgram_ () const noexcept
  {
    crashProgram ();
  }


  void
  Assertion::assertionFailure_ () const noexcept
  { }
}
