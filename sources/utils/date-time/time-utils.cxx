#include <chrono> // std::chrono::{system_clock::{time_point, now, duration}, duration_cast, nanoseconds}

#include "time-utils.hxx" // Utils::*


namespace Utils
{
  std::chrono::nanoseconds::rep
  unixTime (void)
  {
    using clock_type = std::chrono::system_clock;


    const clock_type::time_point now (clock_type::now ());
    const clock_type::duration time_since_epoch (now.time_since_epoch ());
    const std::chrono::nanoseconds nanoseconds (
      std::chrono::duration_cast <std::chrono::nanoseconds> (time_since_epoch)
    );

    return nanoseconds.count ();
  }
}
