#ifndef UTILS_META_ENABLECOPYMOVE_HXX
#define UTILS_META_ENABLECOPYMOVE_HXX


#include "always-false.hxx"  // AlwaysFalseV


namespace Utils
{
  template <
    bool TCopyConstructible, bool TCopyAssignable, bool TMoveConstructible, bool TMoveAssignable, typename TTag
  >
  struct EnableCopyMove
  {
    static_assert (
         AlwaysFalseV <bool, TCopyConstructible, TCopyAssignable, TMoveConstructible, TMoveAssignable>
      && AlwaysFalseV <TTag>,
      "EnableCopyMove <TCopyConstructible, TCopyAssignable, TMoveConstructible, TMoveAssignable, TTag>:  "
      "There's no suitable template specialization available"
    );
  };


  template <typename TTag>
  struct EnableCopyMove <false, false, false, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <false, false, false, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };


  template <typename TTag>
  struct EnableCopyMove <false, false, true, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <false, false, true, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };


  template <typename TTag>
  struct EnableCopyMove <false, true, false, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <false, true, false, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };


  template <typename TTag>
  struct EnableCopyMove <false, true, true, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <false, true, true, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = delete;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };


  template <typename TTag>
  struct EnableCopyMove <true, false, false, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <true, false, false, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };


  template <typename TTag>
  struct EnableCopyMove <true, false, true, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <true, false, true, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };


  template <typename TTag>
  struct EnableCopyMove <true, true, false, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <true, true, false, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = delete;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };


  template <typename TTag>
  struct EnableCopyMove <true, true, true, false, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = delete;
  };


  template <typename TTag>
  struct EnableCopyMove <true, true, true, true, TTag>
  {
    private:
      using self_type = EnableCopyMove;


    protected:
      constexpr EnableCopyMove () noexcept = default;

      constexpr EnableCopyMove (const self_type &) noexcept = default;

      constexpr EnableCopyMove (self_type &&) noexcept = default;

      constexpr EnableCopyMove &
      operator = (const self_type &) noexcept = default;

      constexpr EnableCopyMove &
      operator = (self_type &&) noexcept = default;
  };
}


#endif  // UTILS_META_ENABLECOPYMOVE_HXX
