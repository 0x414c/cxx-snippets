#ifndef UTILS_MISC_COUNTER_HXX
#define UTILS_MISC_COUNTER_HXX


#include <stdexcept> // std::runtime_error


namespace Utils
{
  namespace Counter
  {
    namespace
    {
      template <int N>
      struct Flag
      {
        friend constexpr int adlFlag (Flag <N>);
      };


      template <int N>
      struct Writer
      {
        friend constexpr int adlFlag (Flag <N>)
        {
          return N;
        }

        static constexpr int value { N };
      };


      template <int N, int = adlFlag (Flag <N> { })>
      constexpr int read (int, Flag <N>)
      {
        return N;
      }


      template <int N>
      constexpr int read (float, Flag <N>, int R = read (0, Flag <N - 1> { }))
      {
        return R;
      }


      constexpr int read (float, Flag <0>)
      {
        return 0;
      }
    }


    template <int N = 1>
    constexpr int next (int R = Writer <read (0, Flag <32> { }) + N>::value)
    {
      return R;
    }
  }
}


#endif // UTILS_MISC_COUNTER_HXX
