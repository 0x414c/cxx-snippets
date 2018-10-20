#ifndef UTILS_ALGORITHMS_SUMMATOR_HXX
#define UTILS_ALGORITHMS_SUMMATOR_HXX


#include <ostream>  // std::ostream
#include <type_traits>  // std::{is_arithmetic_v, is_floating_point_v, is_integral_v}

#include "../meta/choose.hxx"  // ChooseT
#include "../meta/if-then.hxx"  // IfThen


namespace Utils
{
  /**
   * @brief
   * @tparam TTerm
   */
  template <typename TTerm>
  class NaiveSummationPolicy final
  {
    static_assert (std::is_arithmetic_v <TTerm>);


    public:
      /**
       * @brief
       */
      using term_type = TTerm;


    private:
      /**
       * @brief
       */
      using self_type = NaiveSummationPolicy;


    public:
      /**
       * @brief
       * @return
       */
      constexpr NaiveSummationPolicy () noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr NaiveSummationPolicy (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr NaiveSummationPolicy (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * TODO: [1;0] Explicit?
       * @param initial_value
       * @return
       */
      constexpr explicit NaiveSummationPolicy (term_type initial_value) noexcept :
        sum_ (initial_value)
      { }


      /**
       * @brief
       * @return
       */
      [[nodiscard]] constexpr term_type
      total () const noexcept
      {
        return sum_;
      }


      /**
       * @brief
       * @param term
       */
      constexpr void
      add (term_type term) noexcept
      {
        sum_ += term;
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param new_initial_value
       * @return
       */
      constexpr self_type &
      operator = (term_type new_initial_value) noexcept
      {
        sum_ = new_initial_value;

        return * this;
      }


    private:
      /**
       * @brief
       */
      term_type sum_ { };
  };


  /**
   * @brief
   * @tparam TTerm
   */
  template <typename TTerm>
  class CompensatingSummationPolicy final
  {
    static_assert (std::is_arithmetic_v <TTerm>);


    public:
      /**
       * @brief
       */
      using term_type = TTerm;


    private:
      /**
       * @brief
       */
      using self_type = CompensatingSummationPolicy;


    public:
      /**
       * @brief
       * @return
       */
      constexpr CompensatingSummationPolicy () noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr CompensatingSummationPolicy (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr CompensatingSummationPolicy (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * TODO: [1;0] Explicit?
       * @param initial_value
       * @return
       */
      constexpr explicit CompensatingSummationPolicy (term_type initial_value) noexcept :
        sum_ (initial_value)
      { }


      /**
       * @brief A variation of compensated summation in which the final sum is also corrected.
       * For the reference see: [5], [8].
       * @return
       */
      [[nodiscard]] constexpr term_type
      total () const noexcept
      {
        return sum_ + correction_;
      }


      /**
       * @brief Kahan's compensated summation method employs the correction `e' on every step of a recursive summation.
       * After each partial sum is formed, the correction is computed and immediately added to the next term `x[i]'
       *   before that term is added to the partial sum.  Thus the idea is to capture the rounding errors and feed them
       *   back into the summation [5, p. 9].
       *
       * Each term is first corrected for the error that has accumulated so far.
       * Then the new sum is calculated by adding this corrected term to the running total.
       * Finally, a new correction term is calculated as the difference between the change in the sums and the
       *   corrected term [1, p. 1].
       *
       * For the reference see:
       * [1] `http://www.drdobbs.com/floating-point-summation';
       * [2] `https://en.wikipedia.org/wiki/Kahan_summation_algorithm';
       * [3] `http://www.phys.uconn.edu/~rozman/Courses/P2200_11F/downloads/sum-howto.pdf';
       * [4] Kahan, William (January 1965). Further remarks on reducing truncation errors, doi:10.1145/363707.363723;
       * [5] Higham, Nicholas J. (1993). The accuracy of floating point summation, doi:10.1137/0914050;
       * [6] Kahan, William (1972). A Survey of Error Analysis. pp. 1214-1239;
       * [7] Higham, Nicholas J. (2002). Accuracy and Stability of Numerical Algorithms (2 ed). SIAM. pp. 110–123;
       * [8] Kahan, William (1973). Implementation of algorithms (lecture notes by W.S. Haugeland and D. Hough). p. 107.
       * @param term
       */
      constexpr void
      add (term_type term) noexcept
      {
        // Algorithm from [1], [2]:
        // So far, so good: `correction_' is zero:
        const term_type corrected_term (term - correction_);
        // Alas, `sum_' is big, `corrected_term' is small, so low-order digits of `corrected_term' are lost:
        const term_type new_sum (sum_ + corrected_term);
        // (new_sum - sum_) cancels the high-order part of `corrected_term';
        // subtracting `corrected_term' recovers negative (low part of `corrected_term'):
        correction_ = (new_sum - sum_) - corrected_term;
        // Algebraically, `correction_' should always be zero.  Beware overly-aggressive optimizing compilers!
        sum_ = new_sum;
        // Next time around, the lost low part will be added to `corrected_term' in a fresh attempt.

        // Algorithm from [3], [5] (ditto):
/*        const term_type old_sum (sum_);
        const term_type corrected_term (term + correction_);
        sum_ = old_sum + corrected_term;
        correction_ = (old_sum - sum_) + corrected_term;*/

        // Algorithm from [8] (ditto):
/*        const term_type corrected_term (correction_ + term);
        const term_type new_sum (sum_ + corrected_term);
        correction_ = (sum_ - new_sum) + corrected_term;
        sum_ = new_sum;*/
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param new_initial_value
       * @return
       */
      constexpr self_type &
      operator = (term_type new_initial_value) noexcept
      {
        sum_ = new_initial_value;
        correction_ = term_zero_;

        return * this;
      }


    private:
      /**
       * @brief
       */
      term_type sum_ { };

      /**
       * @brief A running compensation for lost low-order bits
       */
      term_type correction_ { };

      /**
       * @brief
       */
      static constexpr term_type term_zero_ { };
  };


  /**
   * @brief
   * @tparam TTerm
   * @tparam TSummationPolicy
   */
  template <
    typename TTerm,
    typename TSummationPolicy = ChooseT <
      IfThen <std::is_integral_v <TTerm>, NaiveSummationPolicy <TTerm>>,
      IfThen <std::is_floating_point_v <TTerm>, CompensatingSummationPolicy <TTerm>>
    >
  >
  class Summator final
  {
    static_assert (std::is_arithmetic_v <TTerm>);


    public:
      /**
       * @brief
       */
      using term_type = TTerm;

      /**
       * @brief
       */
      using summation_policy_type = TSummationPolicy;


    private:
      /**
       * @brief
       */
      using self_type = Summator;


    public:
      /**
       * @brief
       * @return
       */
      constexpr Summator () noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr Summator (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr Summator (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * TODO: [1;0] Explicit?
       * @param initial_value
       * @return
       */
      constexpr explicit Summator (term_type initial_value) noexcept :
        summation_policy_ (initial_value)
      { }


      /**
       * @brief
       * TODO: [1;0] Explicit?
       * @return
       */
      [[nodiscard]] constexpr explicit operator term_type () const noexcept
      {
        return summation_policy_.total ();
      }


      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (const self_type & that [[maybe_unused]]) noexcept = default;

      /**
       * @brief
       * @param that
       * @return
       */
      constexpr self_type &
      operator = (self_type && that [[maybe_unused]]) noexcept = default;


      /**
       * @brief
       * @param new_initial_value
       * @return
       */
      constexpr self_type &
      operator = (term_type new_initial_value) noexcept
      {
        summation_policy_ = new_initial_value;

        return * this;
      }


      /**
       * @brief
       * @param term
       * @return
       */
      constexpr self_type &
      operator += (term_type term) noexcept
      {
        summation_policy_.add (term);

        return * this;
      }


      /**
       * @brief
       * @param term
       * @return
       */
      constexpr self_type &
      operator -= (term_type term) noexcept
      {
        summation_policy_.add (- term);

        return * this;
      }


      /**
       * @brief
       * @param output
       * @param self
       * @return
       */
      friend std::ostream &
      operator << (std::ostream & output, const self_type & self)
      {
        output
          << "Summator{"
          << "total:" << self.summation_policy_.total ()
          << '}';

        return output;
      }


    private:
      /**
       * @brief
       */
      summation_policy_type summation_policy_ { };
  };
}


#endif  // UTILS_ALGORITHMS_SUMMATOR_HXX
