#include <string> // std::string
#include <utility> // std::move

#include "timer.hxx" // Utils::Timer

#include "format-duration.hxx" // formatDuration
#include "../logging/logger.hxx" // Logger::printShortLog


namespace Utils
{
  Timer::Timer (void)
  {
    init_ ();
  }


  Timer::Timer (const Timer & that) :
    description_ (that.description_),
    is_automatic_ (that.is_automatic_),
    time_started_ (that.time_started_)
  {
    init_ ();
  }


  Timer::Timer (Timer && that) :
    description_ (std::move (that.description_)),
    is_automatic_ (that.is_automatic_),
    time_started_ (that.time_started_)
  {
    init_ ();
  }


//  Timer::Timer (bool is_automatic) :
//    is_automatic_ (is_automatic)
//  {
//    init_ ();
//  }


  Timer::Timer (const std::string & description) :
    description_ (description)
  {
    init_ ();
  }


  Timer::Timer (std::string && description) :
    description_ (std::move (description))
  {
    init_ ();
  }


  Timer::Timer (const std::string & description, bool is_automatic) :
    description_ (description),
    is_automatic_ (is_automatic)
  {
    init_ ();
  }


  Timer::Timer (std::string && description, bool is_automatic) :
    description_ (std::move (description)),
    is_automatic_ (is_automatic)
  {
    init_ ();
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
  Timer::isAutomatic (void) const
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

    Logger::printShortLog (
      Config::Utils::Logger::timer_log_prefix,
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
      is_automatic_ = that.is_automatic_;
      time_started_ = that.time_started_;
    }

    return *this;
  }


  void
  Timer::init_ (void)
  {
    if (is_automatic_)
    {
      start ();
    }
  }
}
