#ifndef UTILS_DATETIME_TIMEUTILS_HXX
#define UTILS_DATETIME_TIMEUTILS_HXX


#include <ctime> // std::tm

#include <chrono> // std::chrono::nanoseconds::rep


namespace Utils
{
  /**
   * @brief
   * @return
   */
  std::chrono::nanoseconds::rep
  unixTime (void);

  /**
   * @brief
   * @param x
   * @param y
   * @return
   */
  constexpr int
  tmDiff (const std::tm & x, const std::tm & y) noexcept
  {
    /*
     * Compute intervening leap days correctly even if year is negative.
     * Take care to avoid integer overflow in leap day calculations,
     *   but it's OK to assume that `x' and `y' are close to each other.
     */
    const int x4 ((x.tm_year >> 2) + (1900 >> 2) - !(x.tm_year & 3));
    const int y4 ((y.tm_year >> 2) + (1900 >> 2) - !(y.tm_year & 3));
    const int x100 (x4 / 25 - (x4 % 25 < 0));
    const int y100 (y4 / 25 - (y4 % 25 < 0));
    const int x400 (x100 >> 2);
    const int y400 (y100 >> 2);
    const int intervening_leap_days ((x4 - y4) - (x100 - y100) + (x400 - y400));

    const int years (x.tm_year - y.tm_year);
    const int days (365 * years + intervening_leap_days + (x.tm_yday - y.tm_yday));
    const int hours (24 * days + (x.tm_hour - y.tm_hour));
    const int minutes (60 * hours + (x.tm_min - y.tm_min));
    const int seconds (60 * minutes + (x.tm_sec - y.tm_sec));

    return seconds;
  }
}


#endif // UTILS_DATETIME_TIMEUTILS_HXX
