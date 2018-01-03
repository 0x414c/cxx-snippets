#ifndef UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX
#define UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX


#include <memory> // std::addressof
#include <type_traits> /* std::{conjunction_v, disjunction, enable_if_t, is_array, is_assignable_v, is_constructible_v,
  is_nothrow_constructible_v, is_nothrow_assignable_v, is_reference, is_void, negation} */
#include <utility> // std::forward

#include "aligned-union-storage.hxx" // AlignedUnionStorageT


namespace Utils
{
  /**
   * @brief Unsafe union-like class that holds static uninitialized storage and provides methods for constructing,
   * accessing and destroying objects of different types in that storage.
   * NOTE: [2] Does not manage memory by itself and does not track types of active objects!
   * @tparam TTypes
   */
  template <typename ... TTypes>
  class AlignedUnionStorageManager final
  {
    static_assert (
      std::conjunction_v <
        std::negation <
          std::disjunction <
            std::is_function <TTypes>, std::is_reference <TTypes>, std::is_void <TTypes>, std::is_array <TTypes>
          >
        > ...
      >
    );


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
      constexpr AlignedUnionStorageManager (const self_type & that [[maybe_unused]]) noexcept = delete;


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      const TType &
      get (void) const noexcept
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
      get (void) noexcept
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
      template <
        typename TType, typename ... TArgs, std::enable_if_t <std::is_constructible_v <TType, TArgs && ...>> ...
      >
      TType &
      construct (TArgs && ... args)
      noexcept (std::is_nothrow_constructible_v <TType, TArgs && ...>)
      {
        static_assert (std::is_constructible_v <TType, TArgs && ...>);

        return *construct_ <TType, TArgs ...> (get_ (), std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TType
       * @tparam TArgs
       * @param args
       * @return
       */
      template <typename TType, typename TArg, std::enable_if_t <std::is_assignable_v <TType &, TArg &&>> ...>
      TType &
      assign (TArg && arg)
      noexcept (std::is_nothrow_assignable_v <TType &, TArg &&>)
      {
        static_assert (std::is_assignable_v <TType &, TArg &&>);

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
      noexcept (std::is_nothrow_destructible_v <TType>)
      {
        destroy_ <TType> ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = delete;


    private:
      /**
       * @brief
       * @return
       */
      constexpr const void *
      get_ (void) const noexcept
      {
        return std::addressof (storage_);
      }


      /**
       * @brief
       * @return
       */
      constexpr void *
      get_ (void) noexcept
      {
        return std::addressof (storage_);
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      constexpr const TType *
      get_ (void) const noexcept
      {
        return static_cast <const TType *> (get_ ());
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      constexpr TType *
      get_ (void) noexcept
      {
        return static_cast <TType *> (get_ ());
      }


      /**
       * @brief
       * @tparam TType
       * @tparam TArgs
       * @param address
       * @param args
       * @return
       */
      template <
        typename TType, typename ... TArgs, std::enable_if_t <std::is_constructible_v <TType, TArgs && ...>> ...
      >
      TType *
      construct_ (void * address, TArgs && ... args)
      noexcept (std::is_nothrow_constructible_v <TType, TArgs && ...>)
      {
        static_assert (std::is_constructible_v <TType, TArgs && ...>);

        return ::new (address) (TType) (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TType
       */
      template <typename TType>
      void
      destroy_ (void)
      noexcept (std::is_nothrow_destructible_v <TType>)
      {
        get_ <TType> ()->~TType ();
      }


      /**
       * @brief
       */
      AlignedUnionStorageT <TTypes ...> storage_;
  };
}


#endif // UTILS_TYPETRAITS_ALIGNEDUNIONSTORAGEMANAGER_HXX
