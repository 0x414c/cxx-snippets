#ifndef UTILS_DATETIME_TIMEUTILS_HXX
#define UTILS_DATETIME_TIMEUTILS_HXX


#include <ctime>  // std::tm

#include <chrono>  // std::chrono::nanoseconds::rep


namespace Utils
{
  /**
   * @brief
   * @return
   */
  [[nodiscard]] std::chrono::nanoseconds::rep
  unixTime ();


  /**
   * @brief
   * @param x
   * @param y
   * @return
   */
  [[nodiscard]] constexpr int
  tmDiff (const std::tm & x, const std::tm & y) noexcept
  {
    /*
     * Compute intervening leap days correctly even if year is negative.
     * Take care to avoid integer overflow in leap day calculations,
     *   but it's OK to assume that `x' and `y' are close to each other.
     */
    const int x_4 ((x.tm_year >> 2) + (1'900 >> 2) - (! (x.tm_year & 3)));
    const int y_4 ((y.tm_year >> 2) + (1'900 >> 2) - (! (y.tm_year & 3)));
    const int x_100 (x_4 / 25 - (x_4 % 25 < 0));
    const int y_100 (y_4 / 25 - (y_4 % 25 < 0));
    const int x_400 (x_100 >> 2);
    const int y_400 (y_100 >> 2);
    const int intervening_leap_days ((x_4 - y_4) - (x_100 - y_100) + (x_400 - y_400));

    const int years (x.tm_year - y.tm_year);
    const int days (365 * years + intervening_leap_days + (x.tm_yday - y.tm_yday));
    const int hours (24 * days + (x.tm_hour - y.tm_hour));
    const int minutes (60 * hours + (x.tm_min - y.tm_min));
    const int seconds (60 * minutes + (x.tm_sec - y.tm_sec));

    return seconds;
  }
}


#endif  // UTILS_DATETIME_TIMEUTILS_HXX
