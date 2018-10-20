#ifndef UTILS_MATH_CONSTANTS_HXX
#define UTILS_MATH_CONSTANTS_HXX


// TODO: Rename => `Constant'
namespace Utils::Math::Constants
{
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wfloat-conversion"
  /**
   * @brief e.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic E { 2.718281828459045235360287471352662498l };

  /**
   * @brief Binary logarithm of e.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Lb_e { 1.442695040888963407359924681001892137l };

  /**
   * @brief Base 10 logarithm of e.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Lg_e { 0.434294481903251827651128918916605082l };

  /**
   * @brief Natural logarithm of 2.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Ln_2 { 0.693147180559945309417232121458176568l };

  /**
   * @brief Natural logarithm of 10.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Ln_10 { 2.302585092994045684017991454684364208l };

  /**
   * @brief π.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Pi { 3.141592653589793238462643383279502884l };

  /**
   * @brief π / 2.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Pi_div_2 { 1.570796326794896619231321691639751442l };

  /**
   * @brief π / 4.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Pi_div_4 { 0.785398163397448309615660845819875721l };

  /**
   * @brief 1 / π.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic One_div_pi { 0.318309886183790671537767526745028724l };

  /**
   * @brief 2 / π.
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Two_div_pi { 0.636619772367581343075535053490057448l };

  /**
   * @brief 2 / Sqrt[π].
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Two_div_sqrt_pi { 1.128379167095512573896158903121545172l };

  /**
   * @brief Sqrt[2].
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic Sqrt_2 { 1.414213562373095048801688724209698079l };

  /**
   * @brief 1 / Sqrt[2].
   * @tparam TArithmetic
   */
  template <typename TArithmetic>
  inline constexpr TArithmetic One_div_sqrt_2 { 0.707106781186547524400844362104849039l };
#pragma GCC diagnostic pop
}


#endif  // UTILS_MATH_CONSTANTS_HXX
