#ifndef UTILS_DATETIME_TIMER_HXX
#define UTILS_DATETIME_TIMER_HXX


#include <chrono> // std::chrono::steady_clock

#include "../config/timer.hxx" // Config::Utils::Timer::Default_description
#include "../containers/c-string.hxx" // CString
#include "../preproc/paste.hxx" // PASTE_E


namespace Utils
{
  /**
   * @brief Measures execution time using high resolution clock.
   * Logs elapsed time on destruction (in automatic mode only).
   * TODO: [0;1] `clock_type' as a template parameter.
   * TODO: [1;1] Split to two classes: manual and automatic.
   * TODO: [1;2] Add file and line to the description.
   */
  class Timer final
  {
    public:
      /**
       * @brief
       */
      using clock_type = std::chrono::steady_clock;

      /**
       * @brief
       */
      using self_type = Timer;


      /**
       * @brief
       * @return
       */
      Timer (void);

      /**
       * @brief
       * @param that
       * @return
       */
      Timer (const self_type & that);

      /**
       * @brief
       * @param description
       * @return
       */
      explicit Timer (const CString & description);

      /**
       * @brief
       * @param description
       * @param is_automatic
       * @return
       */
      explicit Timer (const CString & description, bool is_automatic);

      /**
       * @brief
       */
      ~Timer (void);

      /**
       * @brief
       * @return
       */
      const CString &
      description (void) const noexcept;

      /**
       * @brief
       * @return
       */
      bool
      isAutomatic (void) const noexcept;

      /**
       * @brief
       * @return
       */
      clock_type::duration
      timeElapsed (void) const;

      /**
       * @brief
       */
      void
      logTimeElapsed (void) const;

      /**
       * @brief
       */
      void
      start (void);

      /**
       * @brief
       */
      void
      stop (void);

      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (const self_type & that) = delete;


    private:
      /**
       * @brief
       */
      void
      initialize_ (void);

      /**
       * @brief
       */
      CString description_ { Config::Utils::Timer::Default_description };

      /**
       * @brief
       */
      bool is_automatic_ { true };

      /**
       * @brief
       */
      clock_type::time_point time_started_;

      /**
       * @brief
       */
      clock_type::time_point time_stopped_;
  };
}


#ifdef WITH_TIMERS
  /**
   * @brief
   */
  #define TIMER_AUTO() const ::Utils::Timer (PASTE_E (timer_, __COUNTER__)) (__PRETTY_FUNCTION__)


  /**
   * @brief
   */
  #define TIMER_CREATE(id, description) ::Utils::Timer (PASTE_E (timer_, id)) ((description), false)


  /**
   * @brief
   */
  #define TIMER_START(id) \
    do \
    { \
      (PASTE_E (timer_, id)).start (); \
    } \
    while (false)


  /**
   * @brief
   */
  #define TIMER_STOP(id) \
    do \
    { \
      (PASTE_E (timer_, id)).stop (); \
      (PASTE_E (timer_, id)).logTimeElapsed (); \
    } \
    while (false)
#else // WITH_TIMERS
  #define TIMER_AUTO() (void (0))

  #define TIMER_CREATE(id, description) (void (0))

  #define TIMER_START(id) do { } while (false)

  #define TIMER_STOP(id) do { } while (false)
#endif // WITH_TIMERS


#endif // UTILS_DATETIME_TIMER_HXX
