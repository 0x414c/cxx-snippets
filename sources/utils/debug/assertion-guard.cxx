#include <cstdlib> // std::abort

#include "assertion-guard.hxx" // AssertionGuard::*

namespace Utils
{
  void
  AssertionGuard::assertionFailure_ (void) const noexcept
  { }


  void
  AssertionGuard::crashProgram_ (void) const noexcept
  {
    std::abort ();
  }
}
