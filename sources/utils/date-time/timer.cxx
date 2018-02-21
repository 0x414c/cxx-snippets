#include <utility> // std::move

#include "timer.hxx" // Timer::*

#include "../config/logger.hxx" // Config::Utils::Logger::Timer_prefix
#include "../containers/c-string.hxx" // CString
#include "../logging/logger.hxx" // Logger::log
#include "../misc/bool-flag.hxx" // BoolFlag
#include "format-duration.hxx" // formatDuration


namespace Utils
{
  Timer::Timer (void)
  {
    initialize_ ();
  }


  Timer::Timer (const CString & description) :
    description_ (description)
  {
    initialize_ ();
  }


  Timer::Timer (is_automatic_flag is_automatic) :
    is_automatic_ (is_automatic)
  {
    initialize_ ();
  }


  Timer::Timer (const CString & description, is_automatic_flag is_automatic) :
    description_ (description),
    is_automatic_ (is_automatic)
  {
    initialize_ ();
  }


  Timer::~Timer (void)
  {
    if (isAutomatic ())
    {
      stop ();
    }
  }


  const CString &
  Timer::description (void) const noexcept
  {
    return description_;
  }


  bool
  Timer::isAutomatic (void) const noexcept
  {
    return is_automatic_;
  }


  Timer::clock_type::time_point
  Timer::timeStarted (void) const
  {
    return time_started_;
  }


  void
  Timer::timeStarted_ (Timer::clock_type::time_point time_started)
  {
    time_started_ = time_started;
  }


  Timer::clock_type::time_point
  Timer::timeStopped (void) const
  {
    return time_stopped_;
  }


  void
  Timer::timeStopped_ (Timer::clock_type::time_point time_stopped)
  {
    time_stopped_ = time_stopped;
  }


  Timer::clock_type::duration
  Timer::timeElapsed (void) const
  {
    return (timeStopped () - timeStarted ());
  }


  void
  Timer::logTimeElapsed (void) const
  {
    Logger::log (
      Config::Utils::Logger::Timer_prefix, "Timer `{0}': time elapsed: {1}",
      description_, formatDuration (timeElapsed ())
    );
  }


  void
  Timer::start (void)
  {
    timeStarted_ (clock_type::now ());
  }


  void
  Timer::stop (void)
  {
    timeStopped_ (clock_type::now ());

    if (isAutomatic ())
    {
      logTimeElapsed ();
    }
  }


  void
  Timer::initialize_ (void)
  {
    if (isAutomatic ())
    {
      start ();
    }
  }
}
