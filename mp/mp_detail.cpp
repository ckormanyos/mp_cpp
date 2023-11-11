///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_detail.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Various utilities for multiple precision float
// 
// *****************************************************************************

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iterator>
#include <limits>
#include <sstream>

#include <mp/mp_cpp.h>
#include <mp/mp_detail.h>

std::int32_t mp::detail::compute_pow2_for_agm_log(const mp::mp_cpp& x, const std::int32_t my_digits10)
{
  // Choose m > [(my_digits10 * 1.661) - (log(x) / log(2))] > m_min
  const double n_times_factor = static_cast<double>(static_cast<double>(my_digits10) * 1.67);
  const double lgx_over_lg2   = static_cast<double>(mp::detail::compute_fast_log_value(x)) / std::log(2.0);

  const std::int32_t m = static_cast<std::int32_t>(n_times_factor - lgx_over_lg2);

  // Ensure that the resulting power is non-negative.
  // Also enforce that m >= 8.
  return (std::max)(m, static_cast<std::int32_t>(8));
}

void mp::detail::compute_fast_exp_string(const mp::mp_cpp& x, std::string& str_exp_value)
{
  // Calculate the string representation of e^x,
  // where the range of double might be inadequate.
  // Here we assume that x can be properly stored
  // in double. But e^x might be much larger or
  // much smaller and not fit within the range of double.

  // Consider e^x = 10^B * y,
  //   where B =    int_part(x log10(e))
  //   and   y = 10^dec_part(x log10(e)).

  // Compute B and y using standard <cmath> functions.

  const double x_as_double = mp::mp_cpp::to_double(x);
  const bool   b_neg       = (x_as_double < 0.0);
  const double xx          = ((!b_neg) ? x_as_double : -x_as_double);

  const long double  x_log10_e = static_cast<long double>(xx) * std::log10(2.7182818284590452353602874713526624977572L);
  const std::int64_t int_part  = static_cast<std::int64_t>(static_cast<std::int64_t>(x_log10_e));
  const long double  dec_part  = static_cast<long double>(x_log10_e - static_cast<long double>(int_part));

  const long double y = std::pow(10.0L, dec_part);

  std::stringstream ss;

  if(b_neg == false)
  {
    ss << std::setprecision(std::numeric_limits<long double>::digits10)
       << y
       << "E+"
       << int_part;
  }
  else
  {
    ss << std::setprecision(std::numeric_limits<long double>::digits10)
       << (1.0L / y)
       << "E-"
       << int_part;
  }

  str_exp_value = ss.str();
}

double mp::detail::compute_fast_log_value(const mp::mp_cpp& x)
{
  double fast_log_result;

  if((x.iszero)() || (x.isone)())
  {
    fast_log_result = 0.0;
  }
  else
  {
    double       dd;
    std::int64_t ne;
    mp::mp_cpp::to_parts(x, dd, ne);

    fast_log_result =
      static_cast<double>(std::log(dd) + (static_cast<double>(ne) * std::log(10.0)));
  }

  return fast_log_result;
}

const mp::mp_cpp& mp::detail::maximum_argument_for_exp()
{
  // Compute the maximum range for the exponential function.
  // The maximum range is given by log((std::numeric_limits<mp_cpp>::max)()).

  static const long double max_arg_as_double =
  []() -> long double
  {
    double       dd;
    std::int64_t ne;
    mp::mp_cpp::to_parts((std::numeric_limits<mp::mp_cpp>::max)(), dd, ne);

    return static_cast<long double>(static_cast<long double>(std::log(dd)) * static_cast<long double>(ne));
  }();

  static const mp::mp_cpp max_arg(max_arg_as_double);

  return max_arg;
}

bool mp::detail::format_float_string(std::string& result_str,
                                     const std::int64_t my_exp_value,
                                     const std::streamsize my_digits10,
                                     const std::ios::fmtflags format_flags,
                                     const bool is_neg,
                                     const bool string_value_is_zero)
{
  using string_size_type = std::string::size_type;

  const bool has_scientific = ((format_flags & std::ios::scientific) == std::ios::scientific);
  const bool has_fixed      = ((format_flags & std::ios::fixed)      == std::ios::fixed);
  const bool has_showpoint  = ((format_flags & std::ios::showpoint)  == std::ios::showpoint);
  const bool has_showpos    = ((format_flags & std::ios::showpos)    == std::ios::showpos);

  const auto result_size =
    static_cast<std::streamsize>
    (
      (std::max)(result_str.size(), static_cast<string_size_type>(UINT8_C(16)))
    );

  const std::streamsize local_digits10 =
    static_cast<std::streamsize>
    (
      (my_digits10 == static_cast<std::streamsize>(INT8_C(0))) ? result_size : my_digits10
    );

  if(string_value_is_zero || result_str.empty() || (result_str.find_first_not_of('0') == std::string::npos))
  {
    // We will be printing zero, even though the value might not
    // actually be zero (it just may have been rounded to zero).
    result_str = "0";

    if(has_scientific || has_fixed)
    {
      result_str.append(1U, '.');

      result_str.append(string_size_type(local_digits10), '0');

      if(has_scientific)
      {
        result_str.append("e+00");
      }
    }
    else
    {
      if(has_showpoint)
      {
        result_str.append(1U, '.');

        if(local_digits10 > 1)
        {
          result_str.append(string_size_type(local_digits10 - 1), '0');
        }
      }
    }

    if(is_neg)
    {
      result_str.insert(0U, 1U, '-');
    }
    else if(has_showpos)
    {
      result_str.insert(0U, 1U, '+');
    }

    return true;
  }

  if(((!has_fixed) && (!has_scientific)) && (!has_showpoint))
  {
    // Suppress trailing zeros:
    std::string::iterator pos = result_str.end();

    while(pos != result_str.begin() && *--pos == '0')
    {
      ;
    }

    if(pos != result_str.end())
    {
      ++pos;
    }

    result_str.erase(pos, result_str.end());

    if(result_str.empty())
    {
      result_str = '0';
    }
  }
  else if((!has_fixed) || (my_exp_value >= 0))
  {
    // Pad out the end with zero's if we need to:

    auto number_of_chars = static_cast<std::streamsize>(result_str.size());

    number_of_chars = local_digits10 - number_of_chars;

    if(has_scientific)
    {
      ++number_of_chars;
    }

    if(number_of_chars > 0)
    {
      result_str.append(string_size_type(number_of_chars), '0');
    }
  }

  if(has_fixed || (!has_scientific && (my_exp_value >= -4) && (my_exp_value < local_digits10)))
  {
    if(1 + my_exp_value > static_cast<std::streamsize>(result_str.size()))
    {
      // Just pad out the end with zeros:
      result_str.append(string_size_type(1 + my_exp_value) - result_str.size(), '0');

      if(has_showpoint || has_fixed)
      {
        result_str.append(".");
      }
    }
    else if(std::streamsize(my_exp_value + 1) < std::streamsize(result_str.size()))
    {
      if(my_exp_value < 0)
      {
        result_str.insert(0U, string_size_type(-1 - my_exp_value), '0');
        result_str.insert(0U, "0.");
      }
      else
      {
        // Insert the decimal point:
        result_str.insert(string_size_type(my_exp_value + 1), 1U, '.');
      }
    }
    else if(has_showpoint || has_fixed)
    {
      // we have exactly the digits we require to left of the point
      result_str += ".";
    }

    if(has_fixed)
    {
      // We may need to add trailing zeros:
      auto len = static_cast<std::streamsize>(result_str.find('.') + static_cast<std::size_t>(UINT8_C(1)));

      len = local_digits10 - (static_cast<std::streamsize>(result_str.size()) - len);

      if(len > static_cast<std::streamsize>(INT8_C(0)))
      {
        result_str.append(string_size_type(len), '0');
      }
    }
  }
  else
  {
    // Scientific format:
    if(has_showpoint || (result_str.size() > 1U))
    {
      result_str.insert(1U, 1U, '.');
    }

    result_str.append(1U, 'e');

    std::string str_exp;

    {
      std::stringstream ss;

      ss << std::abs(my_exp_value);

      ss >> str_exp;
    }

    const string_size_type minimum_exponent_digits10 = 2U;

    if(str_exp.size() < minimum_exponent_digits10)
    {
      str_exp.insert(0U, minimum_exponent_digits10 - str_exp.size(), '0');
    }

    if(my_exp_value < 0)
    {
      str_exp.insert(0U, 1U, '-');
    }
    else
    {
      str_exp.insert(0U, 1U, '+');
    }

    result_str.append(str_exp);
  }

  if(is_neg)
  {
    result_str.insert(0U, 1U, '-');
  }
  else if(has_showpos)
  {
    result_str.insert(0U, 1U, '+');
  }

  return true;
}

bool mp::detail::check_close_representation(const mp::mp_cpp& a,
                                            const mp::mp_cpp& b,
                                            const std::int32_t tol_elems,
                                                  std::int32_t& sig_elems)
{
  using pair_of_array_iterator_type = std::pair<mp_cpp::array_type::const_iterator, mp_cpp::array_type::const_iterator>;

  const pair_of_array_iterator_type it =
    std::mismatch(a.crepresentation().cbegin(),
                  a.crepresentation().cend(),
                  b.crepresentation().cbegin());

  sig_elems = static_cast<std::int32_t>(std::distance(a.crepresentation().cbegin(), it.first));

  // Check for the number of significant digits to be
  // at least half of the requested digits. If at least
  // half of the requested digits have been achieved,
  // then break after the upcoming iteration.

  return (sig_elems >= tol_elems);
}
