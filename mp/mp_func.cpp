///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
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
// Date        : 1999 - 2019
// 
// Description : Mathematical functions for multiple precision class.
// 
// *****************************************************************************

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>

#include <mp/mp_cpp.h>
#include <mp/mp_detail.h>

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::inv()
// 
// Description : Compute the inverse of *this. Quadriatically convergent
//               Newton-Raphson iteration is used.
//               During the iterative steps, the precision of the
//               calculation is limited to the minimum required in
//               order to minimize the run-time.
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::calculate_inv()
{
  if((iszero)())
  {
    return operator=(std::numeric_limits<mp_cpp>::infinity());
  }

  const bool b_negate = my_neg;

  my_neg = false;

  // Return +/- 1 accordingly if *this == one.
  if(isone())
  {
    my_neg = b_negate;

    return *this;
  }

  // Generate the initial estimate using division.
  // Extract the mantissa and exponent for a manual
  // computation of the estimate using built-in double.

  double       dd;
  std::int64_t ne;

  to_parts(*this, dd, ne);

  const std::int32_t tol_half    = static_cast<std::int32_t>((mp_digits10_tol() + 1) / 2);
  const std::int32_t prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
  const std::int32_t target_prec = (std::min)(prec_half, tol_half);

  const mp_cpp original_this(*this);

  // Perform the calculation of the estimate of the inverse
  // using built-in double.
  operator=(mp_cpp(1.0 / dd, static_cast<std::int64_t>(-ne)));

  const std::int64_t target_exp = static_cast<std::int64_t>(my_exp - static_cast<std::int64_t>(target_prec));

  mp_cpp iterate_term;

  // Set the precisions for the next iteration.
  precision             (0);
  iterate_term.precision(0);

  std::int32_t estimated_prec = precision();

  for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
  {
    // Perform the next iteration.
    iterate_term        = original_this;
    iterate_term.my_neg = (!iterate_term.my_neg);
    iterate_term       *= *this;
    iterate_term       += mp::one();
    iterate_term       *= *this;
    operator           += (iterate_term);

    // Test the order of the last iteration change.
    // If it is small enough, then the calculation is finished.
    if(   ((iterate_term.my_exp <= target_exp)  && ((iterate_term.iszero)() == false))
       || ((estimated_prec      >= target_prec) && ((iterate_term.iszero)() == true )))
    {
      break;
    }

    // Double the estimated precision.
    estimated_prec = static_cast<std::int32_t>(static_cast<std::int64_t>(static_cast<double>(estimated_prec) * 1.9));

    // Determine precision for next iteration.
    const std::int32_t x_digits = ((iterate_term.iszero)() ? estimated_prec : static_cast<std::int32_t>(my_exp - iterate_term.my_exp));
    const std::int32_t n_digits = (4 * x_digits) + (10 * mp_core::mp_elem_digits10);
    const std::int32_t n_prec   = ((n_digits < original_this.precision()) ? n_digits : original_this.precision());

    // Set the precisions for the next iteration.
    precision             (n_prec);
    iterate_term.precision(n_prec);
  }

  my_neg = b_negate;

  prec_elem = original_this.prec_elem;

  return *this;
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::calculate_sqrt()
// 
// Description : Compute the square root of *this.
//               The coupled Newton iteration as described in "Pi Unleashed" is used.
//               During the iterative steps, the precision of the
//               calculation is limited to the minimum required in
//               order to minimize the run-time.
//
//               Book reference:
//               http://www.jjj.de/pibook/pibook.html
//               http://www.amazon.com/exec/obidos/tg/detail/-/3540665722/qid=1035535482/sr=8-7/ref=sr_8_7/104-3357872-6059916?v=glance&n=507846
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::calculate_sqrt()
{
  // Use coupled Newton iteration as described in Pi Unleashed.

  if((isneg)())
  {
    return (*this = std::numeric_limits<mp_cpp>::quiet_NaN());
  }

  if((iszero)())
  {
    return *this;
  }

  // Generate the initial estimate using the sqare root.

  // Extract the mantissa and exponent for a manual
  // computation of the estimate of the square root
  // using built-in double.

  double       dd;
  std::int64_t ne;

  to_parts(*this, dd, ne);

  // Force the exponent to be an even multiple of two.
  if(static_cast<std::int64_t>(ne % static_cast<std::int32_t>(2)) != static_cast<std::int64_t>(0U))
  {
    ++ne;
    dd /= 10.0;
  }

  const double sqd = std::sqrt(dd);

  const std::int32_t tol_half    = static_cast<std::int32_t>((mp_digits10_tol() + 1) / 2);
  const std::int32_t prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
  const std::int32_t target_prec = (std::min)(prec_half, tol_half);

  const mp_cpp original_this(*this);

  // Estimate the square root using double.
  operator=(mp_cpp(sqd, static_cast<std::int64_t>(ne / 2)));

  const std::int64_t target_exp = static_cast<std::int64_t>(my_exp - static_cast<std::int64_t>(target_prec));

  // Perform the calculation of 1.0 / (2.0 * x0)
  // using built-in double.

  mp_cpp vi(0.5 / sqd, static_cast<std::int64_t>(-ne / 2));

  mp_cpp x_iterate_term;
  mp_cpp v_iterate_term;

  // Set the precisions for the next iteration.
  precision               (0);
  vi.precision            (0);
  x_iterate_term.precision(0);
  v_iterate_term.precision(0);

  std::int32_t estimated_prec = precision();

  for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
  {
    // Perform the next iteration of vi.
    v_iterate_term        = *this;
    v_iterate_term       *= vi;
    v_iterate_term       += v_iterate_term;
    v_iterate_term.my_neg = (!v_iterate_term.my_neg);
    v_iterate_term       += mp::one();
    v_iterate_term       *= vi;
    vi                   += v_iterate_term;

    // Next iteration of xi
    x_iterate_term        = *this;
    x_iterate_term       *= x_iterate_term;
    x_iterate_term.my_neg = (!x_iterate_term.my_neg);
    x_iterate_term       += original_this;
    x_iterate_term       *= vi;
    operator             += (x_iterate_term);

    // Test the order of the last iteration change.
    // If it is small enough, then the calculation is finished.
    if(   ((x_iterate_term.my_exp <= target_exp)  && ((x_iterate_term.iszero)() == false))
       || ((estimated_prec        >= target_prec) && ((x_iterate_term.iszero)() == true )))
    {
      break;
    }

    // Increase the estimated precision (nearly double it).
    estimated_prec = static_cast<std::int32_t>(static_cast<std::int64_t>(static_cast<double>(estimated_prec) * 1.9));

    // Determine the precision for next iteration.
    const std::int32_t x_digits = ((x_iterate_term.iszero)() ? estimated_prec : static_cast<std::int32_t>(my_exp - x_iterate_term.my_exp));
    const std::int32_t n_digits = (4 * x_digits) + (10 * mp_core::mp_elem_digits10);
    const std::int32_t n_prec   = ((n_digits < original_this.precision()) ? n_digits : original_this.precision());

    // Set the precisions for the next iteration.
    precision               (n_prec);
    vi.precision            (n_prec);
    x_iterate_term.precision(n_prec);
    v_iterate_term.precision(n_prec);
  }

  prec_elem = original_this.prec_elem;

  return *this;
}

// *****************************************************************************
// Function    : mp_cpp pow2(const std::int64_t p)
// 
// Description : Compute multiple precision result of two raised
//               to the power of std::int64_t p, for p positive
//               or negative.
// 
// *****************************************************************************
mp::mp_cpp mp::pow2(const std::int64_t p)
{
  return mp_cpp::calculate_pow2(p);
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::rootn(const mp_cpp& x, const std::int32_t p)
// 
// Description : Compute the of the p'th root of x.
//
// *****************************************************************************
mp::mp_cpp mp::rootn(const mp_cpp& x, const std::int32_t p)
{
  mp_cpp rootn_result;

  if(p == static_cast<std::int32_t>(-1))
  {
    rootn_result = mp_cpp(x).calculate_inv();
  }
  else if(p < static_cast<std::int32_t>(0))
  {
    rootn_result = mp_cpp(x).calculate_rootn_inv(static_cast<std::int64_t>(-p));
  }
  else if(p == static_cast<std::int32_t>(0))
  {
    rootn_result = std::numeric_limits<mp_cpp>::quiet_NaN();
  }
  else if(p == static_cast<std::int32_t>(1))
  {
    rootn_result = x;
  }
  else
  {
    rootn_result = (mp_cpp(x).calculate_rootn_inv(p)).calculate_inv();
  }

  return rootn_result;
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::rootn_inv(const std::int32_t p)
// 
// Description : Compute the inverse of the p'th root of *this.
//               Quadratically convergent Newton-Raphson iteration is used.
//               During the iterative steps, the precision of the
//               calculation is limited to the minimum required in
//               order to minimize the run-time.
//
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::calculate_rootn_inv(const std::int32_t p)
{
  if(p == static_cast<std::int32_t>(-1))
  {
    return *this;
  }
  else if(p < static_cast<std::int32_t>(0))
  {
    return (calculate_rootn_inv(static_cast<std::int64_t>(-p))).calculate_inv();
  }
  else if(p == static_cast<std::int32_t>(0))
  {
    return (*this = std::numeric_limits<mp_cpp>::quiet_NaN());
  }
  else if(p == static_cast<std::int32_t>(1))
  {
    return *this;
  }

  // Generate the initial estimate using 1 / rootn.
  // Extract the mantissa and exponent for a manual
  // computation of the estimate of the n'th root
  // using built-in double.

  double       dd;
  std::int64_t ne;

  to_parts(*this, dd, ne);

  // Adjust exponent and mantissa such that ne is an even power of p.
  while(static_cast<std::int64_t>(ne % p) != static_cast<std::int64_t>(0))
  {
    ++ne;
    dd /= 10.0;
  }

  const std::int32_t tol_half    = static_cast<std::int32_t>((mp_digits10_tol() + 1) / 2);
  const std::int32_t prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
  const std::int32_t target_prec = (std::min)(prec_half, tol_half);

  const mp_cpp original_this(*this);

  // Perform the calculation of the estimate of 1 / rootn
  // using built-in double.

  const double pwd = 1.0 / std::pow(dd, 1.0 / static_cast<double>(p));

  operator=(mp_cpp(pwd, static_cast<std::int64_t>(-ne / p)));

  const std::int64_t target_exp = static_cast<std::int64_t>(my_exp - static_cast<std::int64_t>(target_prec));

  mp_cpp iterate_term;

  // Set the precisions for the next iteration.
  precision             (0);
  iterate_term.precision(0);

  std::int32_t estimated_prec = precision();

  for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
  {
    // Perform the next iteration.
    iterate_term            = mp::pow(*this, static_cast<std::int64_t>(p));
    iterate_term           *= original_this;
    iterate_term.my_neg     = (!iterate_term.my_neg);
    iterate_term           += mp::one();

    const std::int64_t iterate_exp =  iterate_term.my_exp;
    const bool         b_is_zero   = (iterate_term.iszero)();

    iterate_term.div_by_int(p);
    iterate_term           += mp::one();
    operator               *= (iterate_term);

    // Test the order of the last iteration change.
    // If it is small enough, then the calculation is finished.
    if(   ((iterate_exp    <= target_exp)  && (b_is_zero == false))
       || ((estimated_prec >= target_prec) && (b_is_zero == true )))
    {
      break;
    }

    // Increase the estimated precision (nearly double it).
    estimated_prec = static_cast<std::int32_t>(static_cast<std::int64_t>(static_cast<double>(estimated_prec) * 1.9));

    // Determine the precision for next iteration.
    const std::int32_t x_digits = (b_is_zero ? estimated_prec : static_cast<std::int32_t>(my_exp - iterate_exp));
    const std::int32_t n_digits = (4 * x_digits) + (10 * mp_core::mp_elem_digits10);
    const std::int32_t n_prec   = ((n_digits < original_this.precision()) ? n_digits : original_this.precision());

    // Set the precisions for next iteration.
    precision             (n_prec);
    iterate_term.precision(n_prec);
  }

  prec_elem = original_this.prec_elem;

  return *this;
}
