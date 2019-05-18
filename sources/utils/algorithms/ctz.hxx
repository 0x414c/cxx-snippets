#ifndef UTILS_ALGORITHMS_CTZ_HXX
#define UTILS_ALGORITHMS_CTZ_HXX


#include <cstddef>  // std::size_t
#include <cstdint>  // std::*

#include "../meta/always-false.hxx"  // AlwaysFalseV


namespace Utils
{
  /**
   * @brief
   * @tparam TIntegral
   * @param x
   * @return
   */
  template <typename TIntegral>
  [[nodiscard]] constexpr std::uint8_t
  ctz (TIntegral x) noexcept
  {
    static_assert (
      AlwaysFalseV <TIntegral>, "ctz <TIntegral>:  There's no suitable template specialization available"
    );

    return 0;
  }


  namespace CtzInternals_
  {
    /**
     * @brief
     * @tparam TSize
     */
    template <std::size_t TSize>
    struct CtzLut_ final
    {
      static_assert (
        AlwaysFalseV <std::size_t, TSize>, "CtzLut_ <TSize>:  There's no suitable template specialization available"
      );
    };


    /**
     * @brief
     */
    template <>
    struct CtzLut_ <8> final
    {
      /**
       * @brief
       */
      static constexpr auto width { 8 };

      /**
       * @brief
       */
      static constexpr std::uint8_t lut [width] { 0, 1, 2, 4, 7, 3, 6, 5 };

      /**
       * @brief
       */
      static constexpr auto factor { 23 };

      /**
       * @brief
       */
      static constexpr auto shift { 5 };
    };


    /**
     * @brief
     */
    template <>
    struct CtzLut_ <16> final
    {
      /**
       * @brief
       */
      static constexpr auto width { 16 };

      /**
       * @brief
       */
      static constexpr std::uint8_t lut [width] { 0, 1, 2, 5, 3, 9, 6, 11, 15, 4, 8, 10, 14, 7, 13, 12 };

      /**
       * @brief
       */
      static constexpr auto factor { 2'479 };

      /**
       * @brief
       */
      static constexpr auto shift { 12 };
    };


    /**
     * @brief
     */
    template <>
    struct CtzLut_ <32> final
    {
      /**
       * @brief
       */
      static constexpr auto width { 32 };

      /**
       * @brief
       */
      static constexpr std::uint8_t lut [width] {
         0,  1,  2,  6,  3, 11,  7, 16,  4, 14, 12, 21,  8, 23, 17, 26,
        31,  5, 10, 15, 13, 20, 22, 25, 30,  9, 19, 24, 29, 18, 28, 27,
      };

      /**
       * @brief
       */
      static constexpr auto factor { 73'743'071 };

      /**
       * @brief
       */
      static constexpr auto shift { 27 };
    };


    /**
     * @brief
     */
    template <>
    struct CtzLut_ <64> final
    {
      /**
       * @brief
       */
      static constexpr auto width { 64 };

      /**
       * @brief
       */
      static constexpr std::uint8_t lut [width] {
         0,  1,  2,  7,  3, 13,  8, 19,  4, 25, 14, 28,  9, 34, 20, 40,
         5, 17, 26, 38, 15, 46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57,
        63,  6, 12, 18, 24, 27, 33, 39, 16, 37, 45, 47, 30, 53, 49, 56,
        62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43, 51, 60, 42, 59, 58,
      };

      /**
       * @brief
       */
      static constexpr auto factor { 151'050'438'420'815'295 };

      /**
       * @brief
       */
      static constexpr auto shift { 58 };
    };
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  [[nodiscard]] constexpr std::uint8_t
  ctz (std::uint8_t x) noexcept
  {
    using lut_type = CtzInternals_::CtzLut_ <8>;


    if (x != 0)
    {
      return lut_type::lut [((x & (- x)) * lut_type::factor) >> lut_type::shift];
    }

    return lut_type::width;
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  [[nodiscard]] constexpr std::uint8_t
  ctz (std::int8_t x) noexcept
  {
    return ctz (std::uint8_t (x));
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  [[nodiscard]] constexpr std::uint8_t
  ctz (std::uint16_t x) noexcept
  {
    using lut_type = CtzInternals_::CtzLut_ <16>;


    if (x != 0)
    {
      return lut_type::lut [((x & (- x)) * lut_type::factor) >> lut_type::shift];
    }

    return lut_type::width;
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  [[nodiscard]] constexpr std::uint8_t
  ctz (std::int16_t x) noexcept
  {
    return ctz (std::uint16_t (x));
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  constexpr std::uint8_t
  ctz (std::uint32_t x) noexcept
  {
    using lut_type = CtzInternals_::CtzLut_ <32>;


    if (x != 0)
    {
      return lut_type::lut [((x & (- x)) * lut_type::factor) >> lut_type::shift];
    }

    return lut_type::width;
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  constexpr std::uint8_t
  ctz (std::int32_t x) noexcept
  {
    return ctz (std::uint32_t (x));
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  constexpr std::uint8_t
  ctz (std::uint64_t x) noexcept
  {
    using lut_type = CtzInternals_::CtzLut_ <64>;


    if (x != 0)
    {
      return lut_type::lut [((x & (- x)) * lut_type::factor) >> lut_type::shift];
    }

    return lut_type::width;
  }


  /**
   * @brief
   * @param x
   * @return
   */
  template <>
  constexpr std::uint8_t
  ctz (std::int64_t x) noexcept
  {
    return ctz (std::uint64_t (x));
  }
}


#endif  // UTILS_ALGORITHMS_CTZ_HXX
