#ifndef UTILS_DATETIME_FORMATDDATEISO8601_HXX
#define UTILS_DATETIME_FORMATDDATEISO8601_HXX


#include <ctime> // std::{localtime, gmtime, time_t, tm}

#include <string> // std::string

#include <fmt/format.h> // fmt::print
#include <fmt/time.h> // fmt::print[std::tm]


namespace Utils
{
  /**
   * @brief
   * @tparam TClock
   * @param now
   * @return
   */
  template <typename TClock>
  std::string
  formatDate_Iso8601 (const typename TClock::time_point & now)
  {
    using clock_type = TClock;


    const std::time_t now_as_time_t (clock_type::to_time_t (now));
    const std::tm now_as_local_tm (*std::localtime (&now_as_time_t));
    const std::tm now_as_utc_tm (*std::gmtime (&now_as_time_t));

    int time_diff_in_seconds (
        60 * (
            60 * (now_as_local_tm.tm_hour - now_as_utc_tm.tm_hour)
          +      (now_as_local_tm.tm_min  - now_as_utc_tm.tm_min )
        )
      + (now_as_local_tm.tm_sec - now_as_utc_tm.tm_sec)
    );
    const int date_diff_in_days (now_as_local_tm.tm_mday - now_as_utc_tm.tm_mday); // -1, 0 or 1
    if ((date_diff_in_days == 1) || (date_diff_in_days < -1))
    {
      time_diff_in_seconds += 24 * 60 * 60;
    }
    else
    {
      if ((date_diff_in_days == -1) || (date_diff_in_days > 1))
      {
        time_diff_in_seconds -= 24 * 60 * 60;
      }
    }

    if (now_as_local_tm.tm_isdst == 1) {
      if (time_diff_in_seconds >= 0)
      {
        time_diff_in_seconds += 60 * 60;
      }
      else
      {
        time_diff_in_seconds -= 60 * 60;
      }
    }

    fmt::MemoryWriter memory_writer;
    memory_writer.write ("{0:%Y-%m-%dT%H:%M:%S}", now_as_local_tm);
    if (time_diff_in_seconds == 0)
    {
      memory_writer.write ("Z");
    }
    else
    {
      const int time_diff_hours (time_diff_in_seconds / 60 / 60);
      const int time_diff_minutes ((time_diff_in_seconds - time_diff_hours * 60 * 60) / 60);

      memory_writer.write ("{0:0=+3d}:{1:0>2d}", time_diff_hours, time_diff_minutes);
    }

    return memory_writer.str ();
  }
}


#endif // UTILS_DATETIME_FORMATDDATEISO8601_HXX
