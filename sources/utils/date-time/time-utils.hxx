#ifndef UTILS_DATETIME_TIMEUTILS_HXX
#define UTILS_DATETIME_TIMEUTILS_HXX


#include <chrono> // std::chrono::nanoseconds::rep


namespace Utils
{
  /**
   * @brief
   * @return
   */
  std::chrono::nanoseconds::rep
  unixTime (void);
}


#endif // UTILS_DATETIME_TIMEUTILS_HXX
