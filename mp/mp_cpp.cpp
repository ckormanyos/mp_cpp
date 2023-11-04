///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_cpp.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Implementation of mp_cpp class for multiple precision math
// 
// *****************************************************************************

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>

#include <mp/mp_cpp.h>

mp::mp_cpp::mp_cpp(const double mantissa,
                   const std::int64_t exponent) noexcept : my_neg(false),
                                                          my_exp(static_cast<std::int64_t>(0))
{
  // Create a mp_cpp from mantissa and exponent.
  // Warning: This ctor *does not* maintain the full precision of double.

  const bool mantissa_is_iszero = (std::fabs(mantissa) < ((std::numeric_limits<double>::min)() * 2.0));

  if(mantissa_is_iszero)
  {
    operator=(mp::zero());
  }
  else
  {
    const bool b_neg = (mantissa < 0.0);

    my_neg = b_neg;

    double       d = ((!b_neg) ? mantissa : -mantissa);
    std::int64_t e = exponent;

    while(d > 1.0)
    {
      d /= 10.0;
      ++e;
    }

    while(d < 1.0)
    {
      d *= 10.0;
      --e;
    }

    std::int32_t shift = static_cast<std::int32_t>(e % static_cast<std::int32_t>(mp_core::mp_elem_digits10));

    while(static_cast<std::int32_t>(shift-- % mp_core::mp_elem_digits10) != static_cast<std::int32_t>(0))
    {
      d *= 10.0;
      --e;
    }

    my_exp = e;
    my_neg = b_neg;

    static const std::int_fast32_t digit_ratio = static_cast<std::int_fast32_t>(static_cast<std::int_fast32_t>(std::numeric_limits<double>::digits10) / static_cast<std::int32_t>(mp_core::mp_elem_digits10));
    static const std::int_fast32_t digit_loops = static_cast<std::int_fast32_t>(digit_ratio + static_cast<std::int_fast32_t>(2));

    for(std::int_fast32_t i = static_cast<std::int_fast32_t>(0); i < digit_loops; ++i)
    {
      std::uint32_t n  = static_cast<std::uint32_t>(static_cast<std::uint64_t>(d));
      my_data[i]       = static_cast<std::uint32_t>(n);
      d               -= static_cast<double>(n);
      d               *= static_cast<double>(mp_core::mp_elem_mask);
    }
  }
}

void mp::mp_cpp::from_uint32(const std::uint32_t u) noexcept
{
  if(u != static_cast<std::uint32_t>(0))
  {
    const std::uint32_t data_med = static_cast<std::uint32_t>(u / static_cast<std::uint32_t>(mp_core::mp_elem_mask));
    const std::uint32_t data_lo  = static_cast<std::uint32_t>(u % static_cast<std::uint32_t>(mp_core::mp_elem_mask));

    if(data_med)
    {
      my_data[0U] = data_med;
      my_data[1U] = data_lo;
      my_exp = (static_cast<std::int64_t>(1) * static_cast<std::int64_t>(mp_core::mp_elem_digits10));
    }
    else
    {
      my_data[0U] = data_lo;
    }
  }
}

void mp::mp_cpp::from_uint64(const std::uint64_t u) noexcept
{
  if(u < static_cast<std::uint64_t>((std::numeric_limits<std::uint32_t>::max)()))
  {
    from_uint32(static_cast<std::uint32_t>(u));
  }
  else
  {
    if(u != static_cast<std::uint64_t>(0))
    {
      const std::uint32_t data_hi  =
        static_cast<std::uint32_t>(static_cast<std::uint64_t>(u / static_cast<std::uint32_t>(mp_core::mp_elem_mask)) / static_cast<std::uint32_t>(mp_core::mp_elem_mask));
      const std::uint32_t data_med =
        static_cast<std::uint32_t>(                          (u / static_cast<std::uint32_t>(mp_core::mp_elem_mask)) % static_cast<std::uint32_t>(mp_core::mp_elem_mask));
      const std::uint32_t data_lo  =
        static_cast<std::uint32_t>(                           u                                                      % static_cast<std::uint32_t>(mp_core::mp_elem_mask));

      if(data_hi)
      {
        my_data[0U] = data_hi;
        my_data[1U] = data_med;
        my_data[2U] = data_lo;

        my_exp = (static_cast<std::int64_t>(2) * static_cast<std::int64_t>(mp_core::mp_elem_digits10));
      }
      else if(data_med)
      {
        my_data[0U] = data_med;
        my_data[1U] = data_lo;

        my_exp = (static_cast<std::int64_t>(1) * static_cast<std::int64_t>(mp_core::mp_elem_digits10));
      }
      else
      {
        my_data[0U] = data_lo;
      }
    }
  }
}

void mp::mp_cpp::from_long_double(const long double ld) noexcept
{
  if(ld == static_cast<long double>(0.0L))
  {
    *this = mp::zero();
  }
  else if(ld == static_cast<long double>(1.0L))
  {
    *this = mp::one();
  }
  else if((std::isinf)(ld))
  {
    *this = value_inf();
  }
  else if((std::isnan)(ld))
  {
    *this = value_nan();
  }
  else
  {
    int e;
    long double f;
    long double term;

    *this = mp::zero();

    f = std::frexp(ld, &e);

    // See https://svn.boost.org/trac/boost/ticket/10924
    // for an example of why this may go wrong.

    static const int shift = std::numeric_limits<int>::digits - 1;

    while(f != static_cast<long double>(0.0L))
    {
      // Extract int sized bits from f.
      f = std::ldexp(f, shift);

      term = std::floor(f);
      e   -= shift;

      *this *= mp::pow2(std::int64_t(shift));

      if(term > 0)
      {
        *this += mp_cpp(static_cast<unsigned long long>(term));
      }
      else
      {
        *this -= mp_cpp(static_cast<unsigned long long>(-term));
      }

      f -= term;
    }

    if(e != 0)
    {
      *this *= mp::pow2(e);
    }
  }
}
