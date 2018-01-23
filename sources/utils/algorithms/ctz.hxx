#ifndef UTILS_ALGORITHMS_CTZ_HXX
#define UTILS_ALGORITHMS_CTZ_HXX


#include <cstddef> // std::size_t
#include <cstdint> // std::{int8_t, uint8_t, int32_t, uint32_t, int64_t, uint64_t}

#include "../type-traits/always-false.hxx" // AlwaysFalseV


namespace Utils
{
  template <typename TIntegral>
  constexpr std::uint8_t ctz (TIntegral x) noexcept
  {
    static_assert (
      AlwaysFalseV <TIntegral>, "ctz <TIntegral>: There is no suitable template specialization available"
    );

    return 0;
  }


  namespace
  {
    template <std::size_t TSize>
    struct CtzLut final
    {
      static_assert (
        AlwaysFalseV <std::size_t, TSize>,
        "CtzLut <TSize>: There is no suitable template specialization available"
      );
    };


    template <>
    struct CtzLut <8> final
    {
      static constexpr auto width { 8 };

      static constexpr std::uint8_t lut[width] { 0, 1, 2, 4, 7, 3, 6, 5 };

      static constexpr auto factor { 23 };

      static constexpr auto shift { 5 };
    };


    template <>
    struct CtzLut <16> final
    {
      static constexpr auto width { 16 };

      static constexpr std::uint8_t lut[width] { 0, 1, 2, 5, 3, 9, 6, 11, 15, 4, 8, 10, 14, 7, 13, 12 };

      static constexpr auto factor { 2'479 };

      static constexpr auto shift { 12 };
    };


    template <>
    struct CtzLut <32> final
    {
      static constexpr auto width { 32 };

      static constexpr std::uint8_t lut[width] {
         0,  1,  2,  6,  3, 11,  7, 16,  4, 14, 12, 21,  8, 23, 17, 26,
        31,  5, 10, 15, 13, 20, 22, 25, 30,  9, 19, 24, 29, 18, 28, 27
      };

      static constexpr auto factor { 73'743'071 };

      static constexpr auto shift { 27 };
    };


    template <>
    struct CtzLut <64> final
    {
      static constexpr auto width { 64 };

      static constexpr std::uint8_t lut[width] {
         0,  1,  2,  7,  3, 13,  8, 19,  4, 25, 14, 28,  9, 34, 20, 40,
         5, 17, 26, 38, 15, 46, 29, 48, 10, 31, 35, 54, 21, 50, 41, 57,
        63,  6, 12, 18, 24, 27, 33, 39, 16, 37, 45, 47, 30, 53, 49, 56,
        62, 11, 23, 32, 36, 44, 52, 55, 61, 22, 43, 51, 60, 42, 59, 58
      };

      static constexpr auto factor { 151'050'438'420'815'295 };

      static constexpr auto shift { 58 };
    };
  }


  template <>
  constexpr std::uint8_t
  ctz (std::uint8_t x) noexcept
  {
    using lut_type = CtzLut <8>;


    if (x != 0)
    {
      return lut_type::lut[((x & - x) * lut_type::factor) >> lut_type::shift];
    }
    else
    {
      return lut_type::width;
    }
  }


  template <>
  constexpr std::uint8_t
  ctz (std::int8_t x) noexcept
  {
    return ctz (std::uint8_t (x));
  }


  template <>
  constexpr std::uint8_t
  ctz (std::uint16_t x) noexcept
  {
    using lut_type = CtzLut <16>;


    if (x != 0)
    {
      return lut_type::lut[((x & - x) * lut_type::factor) >> lut_type::shift];
    }
    else
    {
      return lut_type::width;
    }
  }


  template <>
  constexpr std::uint8_t
  ctz (std::int16_t x) noexcept
  {
    return ctz (std::uint16_t (x));
  }


  template <>
  constexpr std::uint8_t
  ctz (std::uint32_t x) noexcept
  {
    using lut_type = CtzLut <32>;


    if (x != 0)
    {
      return lut_type::lut[((x & - x) * lut_type::factor) >> lut_type::shift];
    }
    else
    {
      return lut_type::width;
    }
  }


  template <>
  constexpr std::uint8_t
  ctz (std::int32_t x) noexcept
  {
    return ctz (std::uint32_t (x));
  }


  template <>
  constexpr std::uint8_t
  ctz (std::uint64_t x) noexcept
  {
    using lut_type = CtzLut <64>;


    if (x != 0)
    {
      return lut_type::lut[((x & - x) * lut_type::factor) >> lut_type::shift];
    }
    else
    {
      return lut_type::width;
    }
  }


  template <>
  constexpr std::uint8_t
  ctz (std::int64_t x) noexcept
  {
    return ctz (std::uint64_t (x));
  }
}


#endif // UTILS_ALGORITHMS_CTZ_HXX
