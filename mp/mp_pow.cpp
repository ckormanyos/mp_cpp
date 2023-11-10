///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_func.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Power functions for multiple precision math.
// 
// *****************************************************************************

#include <algorithm>
#include <cmath>
#include <cstdlib>

#include <mp/mp_cpp.h>
#include <mp/mp_detail.h>

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::calculate_log()
// 
// Description : Compute the base-e logarithm of *this.
//               Quadratically convergent AGM iteration is used.
//               The main iteration is carried out with full precision.
//               The optional input parameters m and my_pow2 will be used
//               if the caller has supplied pre-computed values of
//               m and 2^(m - 2).
//
//               ln(x) = {pi / [2 * AGM(1, 4 / s)]} - m * ln2
//               where s = x * 2^m
//               and m is chosen such that s > 2^(n / 2)
//               where n is the number of bits of precision required
//               in the result.
//               Chose m > N * 1.661 - ln(x) / ln(2),
//               where N is the required base-10 decimal digits of precision.
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::calculate_log(const std::int32_t my_m, const mp_cpp& p2)
{
  if((iszero)() || (isneg)())
  {
    return operator=(value_nan());
  }
  else if((isone)())
  {
    return operator=(mp::zero());
  }
  else if((my_data[0U] == 2U) && (*this == mp::two()))
  {
    return operator=(mp::ln2());
  }
  else if((my_data[0U] == 5U) && (*this == mp::half()))
  {
    operator=(mp::ln2());

    my_neg = true;

    return *this;
  }

  // For values less than 1 invert the argument and negate the result below.
  const bool b_negate = (*this < mp::one());

  if(b_negate)
  {
    calculate_inv();
  }

  // Use an AGM method.

  // Set a0 = 1.
  // Set b0 = 4 / (*this * 2^m) = (1 / *this) / 2^(m - 2)

  mp_cpp ak(1U);
  ak.prec_elem = prec_elem;

  const bool input_params_m_and_two_pow_m_are_supplied =
    ((my_m > 0) && ((p2.iszero)() == false));

  const int32_t m =
    (input_params_m_and_two_pow_m_are_supplied
      ? my_m
      : mp::detail::compute_pow2_for_agm_log(*this, precision()));

  mp_cpp bk = mp::pow2(static_cast<std::int32_t>(m));
    (input_params_m_and_two_pow_m_are_supplied
      ? p2
      : mp::pow2(m));

  bk.prec_elem = prec_elem;
  bk *= *this;
  bk.calculate_inv();
  bk *= static_cast<std::int32_t>(4);

  mp_cpp ak_tmp(0U);

  ak_tmp.prec_elem = prec_elem;

  // Determine the requested precision of the upcoming iteration in units of digits10.
  const std::int32_t tol_half    = static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);
  const std::int32_t prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
  const std::int32_t target_prec = (std::min)(prec_half, tol_half);

  // Set the tolerance for the upcoming iteration in units of mp-elements (limbs).
  const std::int32_t tol_elems_half =
      ( (target_prec + (mp_core::mp_elem_digits10 / 2)) / mp_core::mp_elem_digits10)
    + (((target_prec %  mp_core::mp_elem_digits10) != 0) ? 1 : 0);

  for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
  {
    bool break_after_this_iteration = false;

    if(   (k > static_cast<std::int32_t>(7))
       && (ak.order() == bk.order()))
    {
      std::int32_t sig_elems;

      // Check for the number of significant digits to be
      // at least half of the requested digits. If at least
      // half of the requested digits have been achieved,
      // then break after the upcoming iteration.

      break_after_this_iteration =
        mp::detail::check_close_representation(ak, bk, tol_elems_half, sig_elems);

      static_cast<void>(sig_elems);
    }

    ak_tmp   = ak;
    ak      += bk;
    ak.div_by_int(2);

    if(break_after_this_iteration)
    {
      break;
    }

    bk      *= ak_tmp;
    bk.calculate_sqrt();
  }

  // We are now finished with the AGM iteration for log(x).

  // Compute log(x) = {pi / [2 * AGM(1, 4 / 2^m)]} - (m * ln2)
  // Note that ak = bk = AGM(...)

  // Retrieve the value of pi.
  operator=(mp::pi());

  // Divide by 2 * a.
  ak.mul_by_int(2);
  operator*=(ak.calculate_inv());

  // Subtract m * ln2.
  operator-=(mp_cpp(mp::ln2()).mul_by_int(m));

  my_neg = b_negate;

  return *this;
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::exp()
// 
// Description : Compute the exponential funciton exp(*this).
//               For precision in the low digit range, a Taylor series
//               expansion with argument translation is used.
//               For hig precision, quadratically convergent Newton-Raphson
//               iteration is used in combination with the logarithm,
//               where the logarithm is computed above using an AGM method.
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::calculate_exp()
{
  if((iszero)())
  {
    return operator=(mp::one());
  }

  const bool b_neg_arg = my_neg;

  my_neg = false;

  // Check the range of the argument.

  if((*this) > mp::detail::maximum_argument_for_exp())
  {
    // Handle overflow / underflow.

    return operator=((!my_neg) ? std::numeric_limits<mp_cpp>::quiet_NaN() : mp::zero());
  }

  using std::log;

  const std::int64_t n2 = static_cast<std::int64_t>(to_double(*this) / log(2.0));

  if(!mp_base::mp_high_digit_range(precision()))
  {
    // Use a Taylor series expansion with a reduced
    // argument for non-high-precision range.

    // Translate the argument using
    //   exp(t) = (1 + r + r^2/2! + r^3/3! ...)^q * 2^n,
    // where
    //   q = 2^nq
    //   r = t_prime/q
    //   t_prime = t - n*ln(2.0),
    // and where n is chosen such that -0.5*ln(2.0) < t_prime <= 0.5*ln(2.0)

    mp_cpp r = (*this) - (n2 * mp::ln2());

    // Argument translation: Divide by a fixed power of 2, q^2.
    const std::int32_t np     =  precision();
    const std::int32_t loop_q =  np > static_cast<std::int32_t>(2000) ? static_cast<std::int32_t>( 4) : np > static_cast<std::int32_t>(500) ? static_cast<std::int32_t>( 3) : static_cast<std::int32_t>(2);
    const std::int32_t nq     =  np > static_cast<std::int32_t>(2000) ? static_cast<std::int32_t>(16) : np > static_cast<std::int32_t>(500) ? static_cast<std::int32_t>(12) : static_cast<std::int32_t>(8);

    for(std::int32_t j = static_cast<std::int32_t>(0); j < loop_q; ++j)
    {
      const std::int32_t two_pow_nq =
        static_cast<std::int32_t>(static_cast<std::uint32_t>(1U) << static_cast<std::uint32_t>(nq));

      r.div_by_int(two_pow_nq);
    }

    // Determine the tolerance for the Taylor series.
    const std::int64_t tol_exp = static_cast<std::int64_t>(static_cast<std::int64_t>(r.my_exp - static_cast<std::int64_t>(precision())) - mp_core::mp_elem_digits10);

    // Compute the terms of the Taylor series up to O[1].
    operator = (mp::one() + r);

    // Perform the series expansion using the translated,
    // small argument r. The series is terminated when the
    // series term is smaller than the preset tolerance.
    mp_cpp term(r);

    for(std::int32_t k = static_cast<std::int32_t>(2); ; ++k)
    {
      term    *= r;
      operator+= (term.div_by_int(k));

      if((term.my_exp < tol_exp) || (term.iszero)())
      {
        break;
      }
    }

    // Translate the result of the series expansion back.
    // This is done by raising the result to the power of q.
    for(std::int32_t j = static_cast<std::int32_t>(0); j < static_cast<std::int32_t>(loop_q * nq); ++j)
    {
      operator*=(*this);
    }

    // We are now finished with the Taylor series expansion for exp(x).
  }
  else
  {
    // Use Newton-Raphson iteration for high-precision range.

    const std::int32_t tol_half    = static_cast<std::int32_t>((mp_digits10_tol() + 1) / 2);
    const std::int32_t prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
    const std::int32_t target_prec = (std::min)(prec_half, tol_half);

    operator-=(n2 * mp::ln2());

    const mp_cpp original_this(*this);

    mp_cpp iterate_term;

    // Generate an initial estimate for exp(x) using built-in
    // floating-point calculations and string manipulation.
    std::string str_exp_value;
    mp::detail::compute_fast_exp_string(*this, str_exp_value);
    operator=(mp_cpp(str_exp_value));

    // Set the precision for next iteration.
    precision             (0);
    iterate_term.precision(0);

    std::int32_t estimated_prec = precision();

    // Set target exponent for the iteration
    const std::int64_t target_exp = static_cast<std::int64_t>(my_exp - static_cast<std::int64_t>(target_prec));

    for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
    {
      // Next iteration
      iterate_term   = *this;
      iterate_term.calculate_log(0, zero());
      iterate_term.negate();
      iterate_term  += original_this;

      const std::int64_t iterate_exp =  iterate_term.my_exp;
      const bool         b_is_zero   = (iterate_term.iszero)();

      iterate_term  += mp::one();
      operator      *= (iterate_term);

      // Test the order of the last iteration change.
      // If it is small enough, then the calculation is finished.
      if(    ((iterate_exp    <= target_exp)  && (b_is_zero == false))
          || ((estimated_prec >= target_prec) &&  b_is_zero)
        )
      {
        // Reset the precision and exit the iteration loop.
        precision(original_this.precision());

        break;
      }

      // Double the estimated precision.
      estimated_prec = static_cast<std::int32_t>(static_cast<std::int64_t>(static_cast<double>(estimated_prec) * 1.9));

      // Determine precision for next iteration
      const std::int32_t x_digits = (b_is_zero ? estimated_prec : static_cast<std::int32_t>(my_exp - iterate_exp));
      const std::int32_t n_digits = (4 * x_digits) + (10 * mp_core::mp_elem_digits10);
      const std::int32_t n_prec   = ((n_digits < original_this.precision()) ? n_digits : original_this.precision());

      // Precision for next iteration
      precision             (n_prec);
      iterate_term.precision(n_prec);
    }

    // We are now finished with the Newton-Raphson iteration for exp(x).
  }

  // Multiplication by 2^n except for 2^0 = 1
  if(n2 > static_cast<std::int64_t>(0))
  {
    operator*=(mp::pow2(n2));
  }

  return (b_neg_arg ? calculate_inv() : *this);
}

// *****************************************************************************
// Function    : mp_cpp mp::floor(const mp_cpp& x)
// 
// Description : Return the floor of x.
// 
// *****************************************************************************
mp::mp_cpp mp::floor(const mp::mp_cpp& x)
{
  mp_cpp result = x;

  if((result.isfinite)() == false)
  {
    return result;
  }
  else
  {
    if((result.isneg)())
    {
      result -= mp::one();
    }

    return mp_cpp::integer_part(result);
  }
}

// *****************************************************************************
// Function    : mp_cpp mp::ceil(const mp_cpp& x)
// 
// Description : Return the ceil of x.
// 
// *****************************************************************************
mp::mp_cpp mp::ceil(const mp::mp_cpp& x)
{
  mp_cpp result = x;

  if((result.isfinite)() == false)
  {
    return result;
  }
  else
  {
    if((result.isneg)() == false)
    {
      result += mp::one();
    }

    return mp_cpp::integer_part(result);
  }
}

// *****************************************************************************
// Function    : mp_cpp mp::frexp(const mp_cpp& x, std::int64_t* expptr)
// 
// Description : Split x into mantissa and exponent.
// 
// *****************************************************************************
mp::mp_cpp mp::frexp(const mp::mp_cpp& x, std::int64_t* expptr)
{
  mp_cpp result = x;

  if((result.isneg)())
  {
    result.negate();
  }

  if((result.iszero)())
  {
    *expptr = static_cast<std::int64_t>(0);

    return result;
  }

  std::int64_t t = result.order();

  if(std::abs(t) < static_cast<std::int64_t>((std::numeric_limits<std::int64_t>::max)() / 1000))
  {
    t *= 1000;
    t /=  301;
  }
  else
  {
    t /=  301;
    t *= 1000;
  }

  result *= mp::pow2(-t);

  if((result.iszero)() || (result.isinf)() || (result.isnan)())
  {
    // The calculation of pow2 overflowed, slip the calculation up.
    result = x;

    if((result.isneg)())
    {
      result.negate();
    }

    t /= 2;
    result *= mp::pow2(-t);
  }

  if(std::abs(result.order()) > 5)
  {
    // If our first estimate doesn't get close enough then try recursion until we do:
    std::int64_t e2;

    mp_cpp r2 = mp::frexp(result, &e2);

    // Overflow protection: There is no overflow protection.

    t      += e2;
    result  = r2;
  }

  while(result.compare(mp::one()) >= 0)
  {
    result /= 2;
    ++t;
  }

  while(result.compare(mp::half()) < 0)
  {
    result *= 2;
    --t;
  }

  *expptr = t;

  if((x.isneg)())
  {
    result.negate();
  }

  return result;
}

// *****************************************************************************
// Function    : mp_cpp mp::frexp(const mp_cpp& x, std::int64_t* expptr)
// 
// Description : Create the result from mantissa x and exponent exp_value.
// 
// *****************************************************************************
mp::mp_cpp mp::ldexp(const mp::mp_cpp& x, std::int64_t  exp_value)
{
  mp_cpp result = x;

  if(   (exp_value > static_cast<std::int64_t>(-std::numeric_limits<std::int32_t>::digits))
     && (exp_value < static_cast<std::int64_t>(0)))
  {
    result /= std::int32_t(static_cast<std::uint64_t>(1) << -exp_value);
  }
  else if(   (exp_value < static_cast<std::int64_t>(+std::numeric_limits<std::int32_t>::digits))
          && (exp_value > static_cast<std::int64_t>(0)))
  {
    result *= std::int32_t(static_cast<std::uint64_t>(1U) << exp_value);
  }
  else if(exp_value != static_cast<std::int64_t>(0))
  {
    result *= mp::pow2(exp_value);
  }

  return result;
}

// *****************************************************************************
// Function    : mp_cpp mp::pow(const mp_cpp& x, const mp_cpp& a)
// 
// Description : Compute the *this to the power a.
//               Use the identity x^a = exp(a * log(x)).
// 
// *****************************************************************************
mp::mp_cpp mp::pow(const mp::mp_cpp& x, const mp::mp_cpp& a)
{
  return mp::exp(a * mp::log(x));
}
