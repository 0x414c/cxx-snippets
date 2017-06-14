#ifndef UTILS_DATETIME_FORMATDDATEISO8601_HXX
#define UTILS_DATETIME_FORMATDDATEISO8601_HXX


#include <ctime> // std::{localtime, gmtime, time_t, tm}

#include <string> // std::string

#include "fmt/format.h" // fmt::print
#include "fmt/time.h" // fmt::print[std::strftime]


namespace Utils
{
  template <typename TClock>
  std::string
  formatDate_Iso8601 (const typename TClock::time_point & now)
  {
    using clock_type = TClock;


    const std::time_t now_as_time_t (clock_type::to_time_t (now));
    const std::tm now_as_local_tm (*std::localtime (&now_as_time_t));
    const std::tm now_as_utc_tm (*std::gmtime (&now_as_time_t));

    std::time_t delta_seconds (
        60 * (
            60 * (now_as_local_tm.tm_hour - now_as_utc_tm.tm_hour)
          +      (now_as_local_tm.tm_min  - now_as_utc_tm.tm_min )
        )
      + (now_as_local_tm.tm_sec - now_as_utc_tm.tm_sec)
    );
    const int delta_days (now_as_local_tm.tm_mday - now_as_utc_tm.tm_mday); // -1, 0 or 1
    if ((delta_days == 1) || (delta_days < -1))
    {
      delta_seconds += 24 * 60 * 60;
    }
    else
    {
      if ((delta_days == -1) || (delta_days > 1))
      {
        delta_seconds -= 24 * 60 * 60;
      }
    }

    fmt::MemoryWriter memory_writer;
    memory_writer.write ("{0:%Y-%m-%dT%H:%M:%S}", now_as_local_tm);
    if (delta_seconds == 0)
    {
      memory_writer.write ("Z");
    }
    else
    {
      const char delta_sign ((delta_seconds > 0) ? '+' : '-');
      const std::time_t delta_hours (delta_seconds / 60 / 60);
      const std::time_t delta_minutes ((delta_seconds - delta_hours * 60 * 60) / 60);

      memory_writer.write ("{0:c}{1:0>2d}:{2:0>2d}", delta_sign, delta_hours, delta_minutes);
    }

    return memory_writer.str ();
  }
}


#endif // UTILS_DATETIME_FORMATDDATEISO8601_HXX
