#ifndef UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX
#define UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX


#include <memory> // std::addressof
#include <utility> // std::forward

#include "aligned-union-storage.hxx" // AlignedUnionStorage


namespace Utils
{
  /**
   * @brief Unsafe union-like class that holds static uninitialized storage and provides methods for constructing,
   * accessing and destroying objects of different types in that storage.
   * NOTE: [2] Does not manage memory by itself and does not track types of active objects!
   * TODO: [1] Shouldn't we use `std::allocator::{construct, destroy}' (or std::destroy_at) and `std::aligned_union'
   *   for this purposes?
   * @tparam TTypes
   */
  template <typename ... TTypes>
  class AlignedUnionStorageManager final
  {
    public:
      /**
       * @brief
       */
      using self_type = AlignedUnionStorageManager;


      /**
       * @brief
       */
      constexpr AlignedUnionStorageManager (void) noexcept = default;

      /**
       * @brief
       * @param that
       */
      AlignedUnionStorageManager (const self_type & that [[maybe_unused]]) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      AlignedUnionStorageManager (self_type && that [[maybe_unused]]) noexcept = delete;


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      const TType &
      get (void) const
      {
        return *get_ <TType> ();
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      TType &
      get (void)
      {
        return *get_ <TType> ();
      }


      /**
       * @brief
       * @tparam TType
       * @tparam TArgs
       * @param args
       * @return
       */
      template <typename TType, typename ... TArgs>
      const TType &
      construct (TArgs && ... args)
      {
        return *construct_ <TType, TArgs ...> (get_ <TType> (), std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * TODO: [2] Use `is_assignable'?
       * TODO: [2] Remove this method as it is not 'basic'?
       * @tparam TType
       * @tparam TArgs
       * @param args
       * @return
       */
      template <typename TType, typename TArg>
      const TType &
      assign (TArg && arg)
      {
        get <TType> () = std::forward <TArg> (arg);

        return get <TType> ();
      }


      /**
       * @brief
       * @tparam TType
       */
      template <typename TType>
      void
      destroy (void)
      {
        destroy_ <TType> ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr const self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = delete;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr const self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = delete;


    private:
      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      const TType *
      get_ (void) const noexcept
      {
        return reinterpret_cast <const TType *> (std::addressof (storage_));
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      TType *
      get_ (void) noexcept
      {
        return reinterpret_cast <TType *> (std::addressof (storage_));
      }


      /**
       * @brief
       * @tparam TType
       * @tparam TArgs
       * @param address
       * @param args
       * @return
       */
      template <typename TType, typename ... TArgs>
      const TType *
      construct_ (TType * address, TArgs && ... args)
      {
        return ::new (address) (TType) (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TType
       */
      template <typename TType>
      void
      destroy_ (void)
      {
        get_ <TType> ()->~TType ();
      }


      /**
       * @brief
       */
      typename AlignedUnionStorage <TTypes ...>::type storage_;
  };
}


#endif // UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX
