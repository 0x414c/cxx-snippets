#ifndef UTILS_MISC_ALIGNEDUNION_HXX
#define UTILS_MISC_ALIGNEDUNION_HXX


#include <memory>  // std::addressof
#include <type_traits>  // std::*
#include <utility>  // std::{forward, launder}

#include "../meta/aligned-union-storage.hxx"  // AlignedUnionStorageT


namespace Utils
{
  namespace AlignedUnionInternals_
  {
    /**
     * @brief
     * @tparam TType
     */
    template <typename TType>
    inline constexpr bool IsAllowedV_ (
         std::is_object_v <TType>
      && (! std::is_array_v <TType>)
      && std::is_destructible_v <TType>
    );
  }


  /**
   * @brief Unsafe union-like class that holds static uninitialized storage and provides methods for constructing,
   *   accessing  &  destroying objects of different types in that storage.
   * NOTE:  [2]  Doesn't manage memory by itself  &  doesn't track types of active objects!
   * @tparam TTypes
   */
  template <typename ... TTypes>
  class AlignedUnion final
  {
    static_assert (sizeof ... (TTypes) > 0);
    static_assert ((AlignedUnionInternals_::IsAllowedV_ <TTypes> && ...));


    private:
      /**
       * @brief
       */
      using self_type = AlignedUnion;


    public:
      /**
       * @brief
       */
      AlignedUnion () noexcept = default;

      /**
       * @brief
       * @param that
       */
      AlignedUnion (const self_type & that [[maybe_unused]]) noexcept = delete;

      /**
       * @brief
       * @param that
       */
      AlignedUnion (self_type && that [[maybe_unused]]) noexcept = delete;


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      const TType &
      get () const noexcept
      {
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);

        return * get_ <TType> ();
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      TType &
      get () noexcept
      {
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);

        return * get_ <TType> ();
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
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);
        static_assert (std::is_constructible_v <TType, TArgs && ...>);

        return * construct_ <TType, TArgs ...> (get_ (), std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TType
       * @tparam TArgs
       * @param arg
       * @return
       */
      template <typename TType, typename TArg, std::enable_if_t <std::is_assignable_v <TType &, TArg &&>> ...>
      TType &
      assign (TArg && arg)
      noexcept (std::is_nothrow_assignable_v <TType &, TArg &&>)
      {
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);
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
      destroy ()
      noexcept (std::is_nothrow_destructible_v <TType>)
      {
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);

        destroy_ <TType> ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = delete;

      /**
       * @brief
       * @param that
       * @return
       */
      self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = delete;


    private:
      /**
       * @brief
       * @return
       */
      const void *
      get_ () const noexcept
      {
        return std::launder (std::addressof (storage_));
      }


      /**
       * @brief
       * @return
       */
      void *
      get_ () noexcept
      {
        return std::launder (std::addressof (storage_));
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      const TType *
      get_ () const noexcept
      {
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
        return (const TType *) get_ ();
#pragma GCC diagnostic pop
      }


      /**
       * @brief
       * @tparam TType
       * @return
       */
      template <typename TType>
      TType *
      get_ () noexcept
      {
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
        return (TType *) get_ ();
#pragma GCC diagnostic pop
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
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);
        static_assert (std::is_constructible_v <TType, TArgs && ...>);

        return ::new (address) (TType) (std::forward <TArgs> (args) ...);
      }


      /**
       * @brief
       * @tparam TType
       */
      template <typename TType>
      void
      destroy_ ()
      noexcept (std::is_nothrow_destructible_v <TType>)
      {
        static_assert (AlignedUnionInternals_::IsAllowedV_ <TType>);

        get_ <TType> ()->TType::~ TType ();
      }


      /**
       * @brief
       */
      AlignedUnionStorageT <TTypes ...> storage_;
  };
}


#endif // UTILS_MISC_ALIGNEDUNION_HXX
