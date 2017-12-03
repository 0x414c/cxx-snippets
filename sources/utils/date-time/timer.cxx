#include <string> // std::string
#include <utility> // std::move

#include "timer.hxx" // Utils::Timer::*

#include "../config/logger.hxx" // Config::Utils::Logger::Timer_log_prefix
#include "../logging/logger.hxx" // Logger::printLog_Short
#include "format-duration.hxx" // formatDuration


namespace Utils
{
  Timer::Timer (void)
  {
    initialize_ ();
  }


  Timer::Timer (const self_type & that) :
    description_ (that.description_),
    is_automatic_ (that.is_automatic_),
    time_started_ (that.time_started_)
  {
    initialize_ ();
  }


  Timer::Timer (self_type && that) :
    description_ (std::move (that.description_)),
    is_automatic_ (std::move (that.is_automatic_)),
    time_started_ (std::move (that.time_started_))
  {
    initialize_ ();
  }


//  Timer::Timer (bool is_automatic) :
//    is_automatic_ (is_automatic)
//  {
//    initialize_ ();
//  }


  Timer::Timer (const std::string & description) :
    description_ (description)
  {
    initialize_ ();
  }


  Timer::Timer (std::string && description) :
    description_ (std::move (description))
  {
    initialize_ ();
  }


  Timer::Timer (const std::string & description, bool is_automatic) :
    description_ (description),
    is_automatic_ (is_automatic)
  {
    initialize_ ();
  }


  Timer::Timer (std::string && description, bool is_automatic) :
    description_ (std::move (description)),
    is_automatic_ (is_automatic)
  {
    initialize_ ();
  }


  Timer::~Timer (void)
  {
    if (is_automatic_)
    {
      stop ();

      logTimeElapsed ();
    }
  }


  const std::string &
  Timer::description (void) const
  {
    return description_;
  }


  bool
  Timer::isAutomatic (void) const noexcept
  {
    return is_automatic_;
  }


  Timer::clock_type::duration
  Timer::timeElapsed (void) const
  {
    return (time_stopped_ - time_started_);
  }


  void
  Timer::logTimeElapsed (void) const
  {
    const clock_type::duration elapsed (time_stopped_ - time_started_);

    Logger::printLog_Short (
      Config::Utils::Logger::Timer_log_prefix,
      "Timer `{0}': time elapsed: {1}", description_, formatDuration (elapsed)
    );
  }


  void
  Timer::start (void)
  {
    time_started_ = clock_type::now ();
  }


  void
  Timer::stop (void)
  {
    time_stopped_ = clock_type::now ();
  }


  const Timer::self_type &
  Timer::operator = (const self_type & that)
  {
    if (this != &that)
    {
      description_ = that.description_;
      is_automatic_ = that.is_automatic_;
      time_started_ = that.time_started_;
    }

    return *this;
  }


  const Timer::self_type &
  Timer::operator = (self_type && that)
  {
    if (this != &that)
    {
      description_ = std::move (that.description_);
      is_automatic_ = std::move (that.is_automatic_);
      time_started_ = std::move (that.time_started_);
    }

    return *this;
  }


  void
  Timer::initialize_ (void)
  {
    if (is_automatic_)
    {
      start ();
    }
  }
}
