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
// Description : Trigonometric functions for multiple precision math.
// 
// *****************************************************************************

#include <algorithm>
#include <cmath>

#include <mp/mp_complex.h>

mp::mp_cpp mp::sin(const mp::mp_cpp& x)
{
  // The complex exponent function is used.
  return (mp::complex<mp::mp_cpp>(mp::zero(), x).calculate_exp()).imag();
}

mp::mp_cpp mp::cos(const mp::mp_cpp& x)
{
  // The complex exponent function is used.
  return (mp::complex<mp::mp_cpp>(mp::zero(), x).calculate_exp()).real();
}

mp::mp_cpp mp::tan(const mp::mp_cpp& x)
{
  mp_cpp s;
  mp_cpp c;

  mp::sincos(x, s, c);

  return s / c;
}

void mp::sincos(const mp::mp_cpp& x, mp::mp_cpp& s, mp::mp_cpp& c)
{
  mp::complex<mp::mp_cpp> z(mp::zero(), x);

  z.calculate_exp();

  s = z.imag();
  c = z.real();
}

mp::mp_cpp mp::asin(const mp::mp_cpp& x)
{
  if((x.iszero)())
  {
    return mp::zero();
  }
  else if(mp::fabs(x) > mp::one())
  {
    return std::numeric_limits<mp_cpp>::quiet_NaN();
  }

  if(!mp_base::mp_high_digit_range(x.precision()))
  {
    // Implement Newton-Raphson for asin.

    bool b_neg = (x.isneg)();

    // Create a local copy xx which is always positive.
    // The copy xx should have the same precision as the input x.
    mp_cpp xx((b_neg == false) ? x : -x);

    // Get initial estimate using standard math function asin.
    const double d = std::asin(mp_cpp::to_double(xx));

    // Determine the requested precision of the upcoming iteration in units of digits10.
    const std::int32_t tol_half    = static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);
    const std::int32_t prec_half   = static_cast<std::int32_t>((xx.precision() + 1) / 2);
    const std::int32_t target_prec = (std::min)(prec_half, tol_half);

    std::int32_t estimated_prec    = static_cast<std::int32_t>(std::numeric_limits<double>::digits10 - 1);

    mp_cpp yn(d);

    double       dd;
    std::int64_t ne;
    mp_cpp::to_parts(yn, dd, ne);

    const std::int64_t target_exp = ne - target_prec;

    mp_cpp iterate_term;
    mp_cpp sin_term;
    mp_cpp cos_term;

    iterate_term.precision(0);
    sin_term.precision    (0);
    cos_term.precision    (0);
    yn.precision          (0);

    for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
    {
      mp::sincos(yn, sin_term, cos_term);

      iterate_term  = sin_term;
      iterate_term -= xx;
      iterate_term /= cos_term;

      yn -= iterate_term;

      std::int64_t iterate_exp;
      mp_cpp::to_parts(iterate_term, dd, iterate_exp);

      // Test the order of the last iteration change.
      // If it is small enough, then the calculation is finished.
      if(   ((iterate_exp    <= target_exp)  && ((iterate_term.iszero)() == false))
         || ((estimated_prec >= target_prec) && ((iterate_term.iszero)() == true )))
      {
        break;
      }

      // Double the estimated precision.
      estimated_prec = static_cast<std::int32_t>(static_cast<std::int64_t>(static_cast<double>(estimated_prec) * 1.9));

      // Determine precision for next iteration.
      std::int64_t yn_exp;
      mp_cpp::to_parts(yn, dd, yn_exp);

      const std::int32_t x_digits = ((iterate_term.iszero)() ? estimated_prec : static_cast<std::int32_t>(yn_exp - iterate_exp));
      const std::int32_t n_digits = (4 * x_digits) + (10 * mp_core::mp_elem_digits10);
      const std::int32_t n_prec   = ((n_digits < xx.precision()) ? n_digits : xx.precision());

      // Set the precisions for next iteration.
      iterate_term.precision(n_prec);
      sin_term.precision    (n_prec);
      cos_term.precision    (n_prec);
      yn.precision          (n_prec);
    }

    yn.precision(xx.precision());

    return ((b_neg == false) ? yn : -yn);
  }
  else
  {
    mp::complex<mp::mp_cpp> z(mp::sqrt(mp_cpp(mp::one()) - mp_cpp(x) * x), x);

    mp_cpp value((z.calculate_log()).imag());

    return value;
  }
}

mp::mp_cpp mp::acos(const mp::mp_cpp& x)
{
  mp_cpp acos_result;

  if((x.iszero)())
  {
    acos_result = mp::pi_half();
  }
  else
  {
    const bool b_neg = (x.isneg)();

    const mp_cpp xx((b_neg == false) ? x : -x);

    if(xx > mp::one())
    {
      acos_result = std::numeric_limits<mp_cpp>::quiet_NaN();
    }
    else
    {
      if(xx.isone())
      {
        acos_result = ((b_neg == false) ? mp::zero() : mp::pi());
      }
      else
      {
        acos_result = mp_cpp(mp::pi_half()) - mp::asin(x);
      }
    }
  }

  return acos_result;
}

mp::mp_cpp mp::atan(const mp::mp_cpp& x)
{
  if((x.iszero)())
  {
    return mp::zero();
  }

  if(!mp_base::mp_high_digit_range(x.precision()))
  {
    // Implement Newton-Raphson for atan.

    bool b_neg = (x.isneg)();

    // Create a local copy xx which is always positive.
    // The copy xx should have the same precision as the input x.
    mp_cpp xx((b_neg == false) ? x : -x);

    // Get initial estimate using standard math function asin.
    const double d = std::atan(mp_cpp::to_double(xx));

    // Determine the requested precision of the upcoming iteration in units of digits10.
    const std::int32_t tol_half    = static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);
    const std::int32_t prec_half   = static_cast<std::int32_t>((xx.precision() + 1) / 2);
    const std::int32_t target_prec = (std::min)(prec_half, tol_half);

    mp_cpp yn(d);

    double       dd;
    std::int64_t ne;
    mp_cpp::to_parts(yn, dd, ne);

    const std::int64_t target_exp = static_cast<std::int64_t>(ne - static_cast<std::int64_t>(target_prec));

    mp_cpp iterate_term;
    mp_cpp sin_term;
    mp_cpp cos_term;
      
    iterate_term.precision(0);
    sin_term.precision    (0);
    cos_term.precision    (0);
    yn.precision          (0);

    std::int32_t estimated_prec = yn.precision();

    for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
    {
      mp::sincos(yn, sin_term, cos_term);

      iterate_term  = xx;
      iterate_term *= cos_term;
      iterate_term -= sin_term;
      iterate_term *= cos_term;

      yn += iterate_term;

      std::int64_t iterate_exp;
      mp_cpp::to_parts(iterate_term, dd, iterate_exp);

      // Test the order of the last iteration change.
      // If it is small enough, then the calculation is finished.
      if(    ((iterate_exp    <= target_exp)  && ((iterate_term.iszero)() == false))
          || ((estimated_prec >= target_prec) &&  (iterate_term.iszero)())
        )
      {
        break;
      }

      // Double the estimated precision.
      estimated_prec = static_cast<std::int32_t>(static_cast<std::int64_t>(static_cast<double>(estimated_prec) * 1.9));

      // Determine precision for next iteration.
      std::int64_t yn_exp;
      mp_cpp::to_parts(yn, dd, yn_exp);

      const std::int32_t x_digits = ((iterate_term.iszero)() ? estimated_prec : static_cast<std::int32_t>(yn_exp - iterate_exp));
      const std::int32_t n_digits = (4 * x_digits) + (10 * mp_core::mp_elem_digits10);
      const std::int32_t n_prec   = ((n_digits < xx.precision()) ? n_digits : xx.precision());

      // Set the precisions for next iteration.
      iterate_term.precision(n_prec);
      sin_term.precision    (n_prec);
      cos_term.precision    (n_prec);
      yn.precision          (n_prec);
    }

    yn.precision(xx.precision());

    return ((b_neg == false) ? yn : -yn);
  }
  else
  {
    mp::complex<mp::mp_cpp> zp(mp::one(),  x);

    return zp.calculate_log().imag();
  }
}

mp::mp_cpp mp::atan2(const mp::mp_cpp& y, const mp::mp_cpp& x)
{
  mp_cpp atan2_result;

  if((y.iszero)())
  {
    atan2_result = ((x.isneg)() ? mp::pi() : mp::zero());
  }
  else if((x.iszero)())
  {
    const int sign_result = mp::sgn(y);

    switch(sign_result)
    {
      case -1:
        atan2_result = -mp::pi_half();
        break;

      case +1:
        atan2_result =  mp::pi_half();
        break;

      case 0:
      default:
        atan2_result =  mp::zero();
        break;
    }
  }
  else
  {
    // Compute atan(y / x), ignoring sign for the moment.
    mp_cpp atan_term(mp::atan(y / x));

    // Determine the quadrant based on signs of x and y.
    bool y_neg = (y.isneg)();
    bool x_neg = (x.isneg)();

    if(y_neg == x_neg)
    {
      // Both negative or both positive
      atan2_result = (x_neg ? (atan_term - mp::pi()) : atan_term);
    }
    else
    {
      // Different signs of x, y
      atan2_result = (x_neg ? (atan_term + mp::pi()) : atan_term);
    }
  }

  return atan2_result;
}
