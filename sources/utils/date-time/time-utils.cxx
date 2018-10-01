#include "time-utils.hxx"  // unixTime

#include <ctime>  // std::tm

#include <chrono>  // std::chrono::{duration_cast, nanoseconds, system_clock}


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

    return (nanoseconds.count ());
  }
}
