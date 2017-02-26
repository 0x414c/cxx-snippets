#ifndef UTILS_DATETIME_FORMATDURATION_HXX
#define UTILS_DATETIME_FORMATDURATION_HXX


#include <chrono> // std::chrono::{minutes, seconds, milliseconds, duration_cast}
#include <string> // std::string

#include "fmt/format.h" // fmt::MemoryWriter


namespace Utils
{
  template <typename TDuration>
  /**
   * @brief
   * TODO: [0;1] Make code as short & clean as it's possible.
   * @param duration
   * @return
   */
  std::string
  formatDuration (const TDuration & duration)
  {
    using namespace std::chrono;
    using namespace std::chrono_literals;


    fmt::MemoryWriter memory_writer;

    bool is_zero (true);

    const hours as_hours (duration_cast <hours> (duration));
    if (as_hours != 0h)
    {
      memory_writer.write ("{0:d}h", as_hours.count ());

      is_zero = false;
    }

    const minutes as_minutes (duration_cast <minutes> (duration - as_hours));
    if (as_minutes != 0min)
    {
      memory_writer.write ("{0:s}{1:d}min", ((is_zero) ? "" : " "), as_minutes.count ());

      is_zero = false;
    }

    const seconds as_seconds (duration_cast <seconds> (duration - as_hours - as_minutes));
    if (as_seconds != 0s)
    {
      memory_writer.write ("{0:s}{1:d}s", ((is_zero) ? "" : " "), as_seconds.count ());

      is_zero = false;
    }

    const milliseconds as_milliseconds (duration_cast <milliseconds> (duration - as_hours - as_minutes - as_seconds));
    if (as_milliseconds != 0ms)
    {
      memory_writer.write ("{0:s}{1:d}ms", ((is_zero) ? "" : " "), as_milliseconds.count ());

      is_zero = false;
    }

    const microseconds as_microseconds (
      duration_cast <microseconds> (duration - as_hours - as_minutes - as_seconds - as_milliseconds)
    );
    if (as_microseconds != 0us)
    {
      memory_writer.write ("{0:s}{1:d}us", ((is_zero) ? "" : " "), as_microseconds.count ());

      is_zero = false;
    }

    const nanoseconds as_nanoseconds (
      duration_cast <nanoseconds> (duration - as_hours - as_minutes - as_seconds - as_milliseconds - as_microseconds)
    );
    if (as_nanoseconds != 0ns)
    {
      memory_writer.write ("{0:s}{1:d}ns", ((is_zero) ? "" : " "), as_nanoseconds.count ());
    }

    return memory_writer.str ();
  }
}


#endif // UTILS_DATETIME_FORMATDURATION_HXX
