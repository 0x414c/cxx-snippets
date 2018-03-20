#ifndef UTILS_DATETIME_FORMATDURATION_HXX
#define UTILS_DATETIME_FORMATDURATION_HXX


#include <chrono> // std::chrono::{minutes, seconds, milliseconds, duration_cast}
#include <string> // std::string

#include <fmt/format.h> // fmt::MemoryWriter


namespace Utils
{
  /**
   * @brief
   * TODO: [0;1] Make code as short & clean as it's possible.
   * @param duration
   * @return
   */
  template <typename TDuration>
  [[nodiscard]] std::string
  formatDuration (TDuration duration)
  {
    using namespace std::chrono;
    using namespace std::chrono_literals;


    fmt::MemoryWriter memory_writer;

    bool is_zero (true);

    const hours duration_hours (duration_cast <hours> (duration));
    if (duration_hours != 0h)
    {
      memory_writer.write ("{0:d}h", duration_hours.count ());

      is_zero = false;
    }

    const minutes duration_minutes (duration_cast <minutes> (duration - duration_hours));
    if (duration_minutes != 0min)
    {
      memory_writer.write ("{0:s}{1:d}min", ((is_zero) ? "" : " "), duration_minutes.count ());

      is_zero = false;
    }

    const seconds duration_seconds (duration_cast <seconds> (duration - duration_hours - duration_minutes));
    if (duration_seconds != 0s)
    {
      memory_writer.write ("{0:s}{1:d}s", ((is_zero) ? "" : " "), duration_seconds.count ());

      is_zero = false;
    }

    const milliseconds duration_milliseconds (
      duration_cast <milliseconds> (duration - duration_hours - duration_minutes - duration_seconds)
    );
    if (duration_milliseconds != 0ms)
    {
      memory_writer.write ("{0:s}{1:d}ms", ((is_zero) ? "" : " "), duration_milliseconds.count ());

      is_zero = false;
    }

    const microseconds duration_microseconds (
      duration_cast <microseconds> (
        duration - duration_hours - duration_minutes - duration_seconds - duration_milliseconds
      )
    );
    if (duration_microseconds != 0us)
    {
      memory_writer.write ("{0:s}{1:d}us", ((is_zero) ? "" : " "), duration_microseconds.count ());

      is_zero = false;
    }

    const nanoseconds duration_nanoseconds (
      duration_cast <nanoseconds> (
        duration - duration_hours - duration_minutes - duration_seconds - duration_milliseconds - duration_microseconds
      )
    );
    if (duration_nanoseconds != 0ns)
    {
      memory_writer.write ("{0:s}{1:d}ns", ((is_zero) ? "" : " "), duration_nanoseconds.count ());
    }

    return memory_writer.str ();
  }
}


#endif // UTILS_DATETIME_FORMATDURATION_HXX
