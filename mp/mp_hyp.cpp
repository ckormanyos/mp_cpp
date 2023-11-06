///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_hyp.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Hyperbolic functions for multiple precision math.
// 
// *****************************************************************************

#include <mp/mp_cpp.h>

// *****************************************************************************
// Function    : mp::mp_cpp mp::sinh(const mp_cpp& x)
// 
// Description : Compute sinh(x). The real exponent function is used.
// 
// *****************************************************************************
mp::mp_cpp mp::sinh(const mp_cpp& x)
{
  mp_cpp sinh_result;

  if((x.iszero)())
  {
    sinh_result = mp::zero();
  }
  else
  {
    const mp_cpp ep = mp::exp(x);
    const mp_cpp em = mp_cpp(ep).calculate_inv();

    sinh_result = (ep - em).div_by_int(2);
  }

  return sinh_result;
}

// *****************************************************************************
// Function    : mp::mp_cpp mp::cosh(const mp_cpp& x)
// 
// Description : Compute cosh(x). The real exponent function is used.
// 
// *****************************************************************************
mp::mp_cpp mp::cosh(const mp_cpp& x)
{
  mp_cpp cosh_result;

  if((x.iszero)())
  {
    cosh_result = mp::one();
  }
  else
  {
    const mp_cpp ep = mp::exp(x);
    const mp_cpp em = mp_cpp(ep).calculate_inv();

    cosh_result = (ep + em).div_by_int(2);
  }

  return cosh_result;
}

// *****************************************************************************
// Function    : mp::mp_cpp mp::tanh(const mp_cpp& x)
// 
// Description : Compute tanh(x). The real exponent function is used.
// 
// *****************************************************************************
mp::mp_cpp mp::tanh(const mp_cpp& x)
{
  mp_cpp tanh_result;

  if((x.iszero)())
  {
    tanh_result = mp::zero();
  }
  else
  {
    const mp_cpp ep = mp::exp(x);
    const mp_cpp em = mp_cpp(ep).calculate_inv();

    tanh_result = (ep - em) / (ep + em);
  }

  return tanh_result;
}

// *****************************************************************************
// Function    : void mp::sinhcosh(const mp_cpp& x, mp_cpp& sh, mp_cpp& ch)
// 
// Description : Compute sinh(x) and cosh(x) storing the results in sh and ch.
//               The complex exponent function is used. This subroutine takes
//               about the same amount of time as sinh or cosh and it should be
//               used whenever both hyperbolic trig values are needed.
// 
// *****************************************************************************
void mp::sinhcosh(const mp_cpp& x, mp_cpp& sh, mp_cpp& ch)
{
  const mp_cpp ep = mp::exp(x);
  const mp_cpp em = mp_cpp(ep).calculate_inv();

  sh = (ep - em).div_by_int(2);
  ch = (ep + em).div_by_int(2);
}

// *****************************************************************************
// Function    : mp::mp_cpp mp::asinh(const mp_cpp& x)
// 
// Description : Compute asinh(x). The real logarithm function is used.
// 
// *****************************************************************************
mp::mp_cpp mp::asinh(const mp_cpp& x)
{
  mp_cpp asinh_result;

  if((x.iszero)())
  {
    asinh_result = mp::zero();
  }
  else
  {
    asinh_result = mp::log(mp::fabs(x) + mp::sqrt(mp_cpp(x) * x + mp::one()));

    if((x.isneg)())
    {
      asinh_result.negate();
    }
  }

  return asinh_result;
}

// *****************************************************************************
// Function    : mp::mp_cpp mp::acosh(const mp_cpp& x)
// 
// Description : Compute acosh(x). The real logarithm function is used.
//               Negative arguments: Using the symmetry of cosh(x)
//               the function returns acosh(-x) = acosh(x) for x < 0.
//               For -1 <= x <= 1 the function returns 0.
// 
// *****************************************************************************
mp::mp_cpp mp::acosh(const mp_cpp& x)
{
  mp_cpp acosh_result;

  mp_cpp xx = mp::fabs(x);

  if(xx < mp::one())
  {
    acosh_result = std::numeric_limits<mp_cpp>::quiet_NaN();
  }
  else if(xx.isone())
  {
    acosh_result = mp::zero();
  }
  else
  {
    acosh_result = mp::log(xx + mp::sqrt((xx * xx) - mp::one()));
  }

  return acosh_result;
}

// *****************************************************************************
// Function    : mp::mp_cpp mp::atanh(const mp_cpp& x)
// 
// Description : Compute atanh(x). The real logarithm function is used.
//               Negative arguments: Using the asymmetry of sinh(x)
//               the function returns atanh(-x) = -atanh(x) for x < 0.
//               For x < -1, x > 1 the function returns 0.
// 
// *****************************************************************************
mp::mp_cpp mp::atanh(const mp_cpp& x)
{
  mp_cpp atanh_result;

  const bool b_neg = (x.isneg)();

  mp_cpp xx = ((b_neg == false) ? x : -x);

  if((xx.isone)())
  {
    atanh_result = mp_cpp::value_inf();

    if(b_neg)
    {
      atanh_result.negate();
    }
  }
  else if(xx > mp::one())
  {
    atanh_result = mp_cpp::value_nan();
  }
  else
  {
    atanh_result = mp::log((xx + mp::one()) / (-xx + mp::one())).div_by_int(2);

    if(b_neg)
    {
      atanh_result.negate();
    }
  }

  return atanh_result;
}
