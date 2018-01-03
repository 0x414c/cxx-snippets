#ifndef UTILS_MISC_COUNTER_HXX
#define UTILS_MISC_COUNTER_HXX


namespace Utils::Counter
{
  inline constexpr int Max_value { 32 };


  namespace
  {
    template <int TValue>
    struct Flag final
    {
      friend constexpr int
      adlFlag (Flag <TValue>);
    };


    template <int TValue>
    struct Writer final
    {
      friend constexpr int
      adlFlag (Flag <TValue>)
      {
        return TValue;
      }


      static constexpr int value { TValue };
    };


    template <int TValue, int = adlFlag (Flag <TValue> { })>
    constexpr int
    read (int, Flag <TValue>)
    {
      return TValue;
    }


    template <int TValue>
    constexpr int
    read (float, Flag <TValue>, int R = read (0, Flag <TValue - 1> { }))
    {
      return R;
    }


    constexpr int
    read (float, Flag <0>)
    {
      return 0;
    }
  }


  template <int TValue = 1>
  constexpr int
  next (int R = Writer <read (0, Flag <Max_value> { }) + TValue>::value)
  {
    return R;
  }
}


#endif // UTILS_MISC_COUNTER_HXX
