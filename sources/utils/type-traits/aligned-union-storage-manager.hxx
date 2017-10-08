#ifndef UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX
#define UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX


#include <iostream> // std::{clog, endl}
#include <memory> // std::addressof
#include <type_traits> // std::{enable_if, is_assignable}
#include <utility> // std::forward

#include "uninitialized-storage.hxx" // AlignedUnionStorage


namespace Utils
{
  /**
   * @brief Unsafe union-like class that holds uninitialized storage and provides
   *   methods for constructing, accessing and destroying objects in that storage.
   * NOTE: [2] Does not manage memory by itself and does not track types of active objects!
   * TODO: [1] Shouldn't we use `std::allocator::{construct, destroy}' and `std::aligned_union' for this?
   * @tparam TTypes
   */
  template <typename ... TTypes>
  class AlignedUnionStorageManager
  {
    public:
      /**
       * @brief
       */
      using self_type = AlignedUnionStorageManager;


      /**
       * @brief
       */
      AlignedUnionStorageManager (void) = default;

      /**
       * @brief
       * @param that
       */
      AlignedUnionStorageManager (const self_type & that) = delete;

      /**
       * @brief
       * @param that
       */
      AlignedUnionStorageManager (self_type && that) = delete;

      /**
       * @brief
       */
      ~AlignedUnionStorageManager (void) = default;


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      const TType *
      get (void) const
      {
        return (reinterpret_cast <const TType *> (std::addressof (storage_)));
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      TType *
      get (void)
      {
        return (reinterpret_cast <TType *> (std::addressof (storage_)));
      }


      /**
       * @brief
       * @tparam TType
       * @tparam TArgs
       * @param args
       * @return
       */
      template <typename TType, typename ... TArgs>
      TType *
      construct (TArgs && ... args)
      {
        return (
          static_cast <TType *> (
            ::new (static_cast <void *> (std::addressof (storage_))) (TType) (std::forward <TArgs> (args) ...)
          )
        );
      }


      /**
       * @brief
       * TODO: [2] Use `is_assignable'.
       * TODO: [2] Remove this method as it is not 'basic'?
       * @tparam TType
       * @tparam TArgs
       * @param args
       * @return
       */
      template <typename TType, typename TArg>
      TType *
      assign (TArg && arg)
      {
        *get <TType> () = std::forward <TArg> (arg);

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
        get <TType> ()->~TType ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (const self_type & that) = delete;

      /**
       * @brief
       * @param that
       * @return
       */
      const self_type &
      operator = (self_type && that) = delete;


    private:
      /**
       * @brief
       */
      typename AlignedUnionStorage <TTypes ...>::type storage_;
  };
}


#endif // UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX
