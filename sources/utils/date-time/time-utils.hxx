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
  int
  tmDiff (const std::tm & x, const std::tm & y);
}


#endif // UTILS_DATETIME_TIMEUTILS_HXX
