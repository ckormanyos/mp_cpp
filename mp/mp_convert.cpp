///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_convert.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Conversion functions for multiple precision math.
// 
// *****************************************************************************

#include <algorithm>
#include <ostream>

#include <mp/mp_cpp.h>
#include <mp/mp_detail.h>

// Implement various conversion functions.

bool mp::mp_cpp::read_string(const char* const s) noexcept
{
  // Clear any existing value that might be present as a relic
  // left over from an existing static instance of mp_cpp.
  *this = mp::zero();

  // Copy the input string to a local std::string object.
  std::string str(s);

  // Remove spaces and tabs.
  str.erase(std::remove(str.begin(), str.end(), static_cast<char>(' ')),  str.end());
  str.erase(std::remove(str.begin(), str.end(), static_cast<char>('\t')), str.end());

  my_exp = static_cast<std::int64_t>(0);

  std::string::size_type pos;

  std::stringstream ss;

  // Get a possible exponent and remove it.
  if(   ((pos = str.find('e')) != std::string::npos)
     || ((pos = str.find('E')) != std::string::npos))
  {
    ss << (str.c_str() + static_cast<std::size_t>(pos + 1U));

    ss >> my_exp;

    ss.str(std::string());
    ss.clear();

    str = str.substr(static_cast<std::string::size_type>(0U), pos);
  }

  // Get a possible +/- sign and remove it.
  my_neg = false;

  if((pos = str.find(static_cast<char>('-'))) != std::string::npos)
  {
    my_neg = true;

    str.erase(pos, static_cast<std::string::size_type>(1U));
  }

  if((pos = str.find(static_cast<char>('+'))) != std::string::npos)
  {
    str.erase(pos, static_cast<std::string::size_type>(1U));
  }

  // Remove the leading zeros in the input string.
  while(str.length() && str.at(static_cast<std::string::size_type>(0U)) == static_cast<char>('0'))
  {
    str.erase(static_cast<std::string::size_type>(0U),
              static_cast<std::string::size_type>(1U));
  }

  // Put the input string into the standard mp_cpp input
  // form aaa.bbbbE+/-n, where aa has 1...unit digits, bbbb
  // has an even multiple of unit digits which are possibly
  // zero padded on the right-end, and n is a signed 32-bit integer
  // which is an even multiple of unit.

  // Find a possible decimal point.
  pos = str.find(static_cast<char>('.'));

  if(pos != std::string::npos)
  {
    // Remove all trailing insignificant zeros.
    while(str.at(static_cast<std::string::size_type>(str.length() - 1U)) == static_cast<char>('0'))
    {
      str.erase(static_cast<std::string::size_type>(str.length() - 1U),
                static_cast<std::string::size_type>(1U));
    }

    // Check if input is identically zero.
    if(str == std::string("."))
    {
      operator=(mp::zero());

      return true;
    }

    // Remove the leading significant zeros just after
    // the decimal point and adjust the exponent accordingly.
    // Note that the while-loop operates only on strings
    // of the form ".000abcd...". Zeros just after the
    // decimal point are removed.
    if(   ((str.at(static_cast<std::string::size_type>(1U)) == static_cast<char>('0')) && (pos == 0U))
       ||  (str.at(static_cast<std::string::size_type>(0U)) == static_cast<char>('.'))
      )
    {
      while(str.at(static_cast<std::string::size_type>(1U)) == static_cast<char>('0'))
      {
        str.erase(static_cast<std::string::size_type>(1U),
                  static_cast<std::string::size_type>(1U));

        --my_exp;
      }

      // Bring one single digit into the mantissa and adjust exponent accordingly.
      str.insert(static_cast<std::string::size_type>(2U), ".");

      str.erase(static_cast<std::string::size_type>(0U),
                static_cast<std::string::size_type>(1U));

      --my_exp;
    }
  }
  else
  {
    // The input string has no decimal point. Append a decimal point.
    str.append(1U, static_cast<char>('.'));
  }

  // Shift the decimal point such that the exponent is an even
  // multiple of mp_core::mp_elem_digits10.
        std::string::size_type n_shift   = static_cast<std::string::size_type>(0U);
  const std::string::size_type n_exp_rem = static_cast<std::string::size_type>(my_exp % static_cast<std::int64_t>(mp_core::mp_elem_digits10));

  if(my_exp % static_cast<std::int64_t>(mp_core::mp_elem_digits10))
  {
    n_shift = ((my_exp < static_cast<std::int64_t>(0))
                ? static_cast<std::string::size_type>(n_exp_rem + static_cast<std::string::size_type>(mp_core::mp_elem_digits10))
                : static_cast<std::string::size_type>(n_exp_rem));
  }

  // Make sure that there are enough digits for the shift.
  pos = str.find(static_cast<char>('.'));

  while(static_cast<std::string::size_type>(str.length() - static_cast<std::string::size_type>(pos + 1U)) < n_shift)
  {
    str.append(1U, static_cast<char>('0'));
  }

  // Perform the decimal point shift.
  if(n_shift)
  {
    str.insert(static_cast<std::string::size_type>(static_cast<std::string::size_type>(pos + 1U) + n_shift), ".");

    str.erase(pos, static_cast<std::string::size_type>(1U));

    my_exp -= static_cast<std::int64_t>(n_shift);
  }

  // Cut the size of the mantissa to <= mp_core::mp_elem_digits10.
  pos = str.find(static_cast<char>('.'));

  if(pos > static_cast<std::string::size_type>(mp_core::mp_elem_digits10))
  {
    const std::int32_t n_pos = static_cast<std::int32_t>(pos);
    const std::int32_t n     = static_cast<std::int32_t>(static_cast<std::int32_t>(n_pos / mp_core::mp_elem_digits10) - static_cast<std::int32_t>(!(n_pos % mp_core::mp_elem_digits10)));

    str.insert(static_cast<std::string::size_type>(n_pos - (n * mp_core::mp_elem_digits10)), ".");

    str.erase(static_cast<std::string::size_type>(pos + 1U),
              static_cast<std::string::size_type>(1U));

    my_exp += static_cast<std::int64_t>(static_cast<std::int64_t>(n) * static_cast<std::int64_t>(mp_core::mp_elem_digits10));
  }

  if(my_exp % static_cast<std::int64_t>(mp_core::mp_elem_digits10))
  {
    return false;
  }

  // Pad the decimal part such that its value
  // is an even multiple of mp_core::mp_elem_digits10.
  pos = str.find(static_cast<char>('.'));

  const std::int32_t n_dec =
    static_cast<std::int32_t>(str.length() - 1U) - static_cast<std::int32_t>(pos);

  std::int32_t n_cnt =
    (((n_dec % mp_core::mp_elem_digits10) != 0)
      ? (mp_core::mp_elem_digits10 - (n_dec % mp_core::mp_elem_digits10))
      : 0);

  str.append(static_cast<std::string::size_type>(n_cnt), static_cast<char>('0'));

  // Truncate decimal part if it is too long.
  const std::string::size_type max_dec = static_cast<std::string::size_type>((mp_elem_number() - 1) * mp_core::mp_elem_digits10);

  if(static_cast<std::string::size_type>(str.length() - pos) > max_dec)
  {
    str = str.substr(static_cast<std::string::size_type>(0U),
                     static_cast<std::string::size_type>(static_cast<std::string::size_type>(pos + 1U) + max_dec));
  }

  // Now the input string has the standard mp_cpp input form (see comment above).

  // Extract the data.

  // First get the digits to the left of the decimal point.
  ss << str.substr(static_cast<std::string::size_type>(0U), pos);

  ss >> my_data[0U];

  ss.str(std::string());
  ss.clear();

  str = str.substr(static_cast<std::string::size_type>(pos + 1U),
                   static_cast<std::string::size_type>(str.length() - static_cast<std::string::size_type>(pos + 1U)));

  // Get the remaining digits to the right of the decimal point.
  n_cnt = 1;

  if(str.length())
  {
    do
    {
      ss << str.substr(static_cast<std::string::size_type>(0U),
                       static_cast<std::string::size_type>(mp_core::mp_elem_digits10));

      ss >> my_data[n_cnt];

      ss.str(std::string());
      ss.clear();

      ++n_cnt;

      str = str.substr(static_cast<std::string::size_type>(mp_core::mp_elem_digits10),
                       static_cast<std::string::size_type>(str.length() - static_cast<std::string::size_type>(mp_core::mp_elem_digits10)));
    }
    while(str.length() != 0U);
  }

  return true;
}

bool mp::mp_cpp::write_string(std::string& result_str,
                              std::streamsize number_of_digits,
                              const std::ios::fmtflags format_flags) const
{
  if((isfinite)() == false)
  {
    if((isinf)())
    {
      result_str = "inf";

      if((isneg)())
      {
        result_str.insert(result_str.cbegin(), char('-'));
      }
      else if(static_cast<std::ios::fmtflags>(format_flags & std::ios::showpos) != static_cast<std::ios::fmtflags>(0))
      {
        result_str.insert(result_str.cbegin(), char('+'));
      }
    }
    else if((isnan)())
    {
      result_str = "nan";
    }
  }
  else
  {
    std::intmax_t org_digits = number_of_digits;
    std::int64_t  exp_value  = order();

    if(number_of_digits == static_cast<std::streamsize>(0))
    {
      number_of_digits = mp_digits10();
    }

    const bool is_fixed =
      (static_cast<std::ios::fmtflags>(format_flags & std::ios::fixed) != static_cast<std::ios::fmtflags>(0));

    const bool is_scientific =
      (static_cast<std::ios::fmtflags>(format_flags & std::ios::scientific) != static_cast<std::ios::fmtflags>(0));

    if(is_fixed)
    {
      number_of_digits += static_cast<std::streamsize>(exp_value + 1);
    }
    else if(is_scientific)
    {
      ++number_of_digits;
    }

    // Determine the number of elements needed to provide
    // the requested digits from mp_cpp.
    const std::size_t number_of_elements =
      (std::min)(static_cast<std::size_t>((number_of_digits / mp::mp_core::mp_elem_digits10) + 2),
                 static_cast<std::size_t>(my_data.size()));

    // Extract the digits the in front of the decimal point.
    std::stringstream ss;

    ss << my_data[0U];

    // Extract all of the remaining digits after the decimal point
    // beginning with the first data element.
    for(std::size_t i = static_cast<std::size_t>(1U); i < number_of_elements; ++i)
    {
      ss << std::setw(static_cast<std::streamsize>(mp::mp_core::mp_elem_digits10))
         << std::setfill(static_cast<char>('0'))
         << my_data[i];
    }

    result_str = ss.str();

    bool have_leading_zeros = false;

    if(number_of_digits == static_cast<std::streamsize>(0))
    {
      // We only get here if the output format is "fixed"
      // and we just need to round the first non-zero digit.

      // Reset to the original value.
      number_of_digits -= (exp_value + 1);

      result_str.insert(std::string::size_type(0U),
                        std::string::size_type(number_of_digits),
                        static_cast<char>('0'));

      have_leading_zeros = true;
    }

    if(number_of_digits < static_cast<std::streamsize>(0))
    {
      result_str = "0";

      mp::detail::format_float_string(result_str,
                                      static_cast<std::int64_t>(0),
                                      static_cast<std::streamsize>(number_of_digits - static_cast<std::streamsize>(exp_value + 1)),
                                      format_flags,
                                      (isneg) (),
                                      (iszero)());
    }
    else
    {
      // Truncate the output string to the size of the precision.
      if(result_str.length() > static_cast<std::string::size_type>(number_of_digits))
      {
        // Get the digit after the last needed digit for rounding.
        const char rounding_char = result_str[static_cast<std::string::size_type>(number_of_digits)];

        const std::uint32_t rounding_value =
          static_cast<std::uint32_t>(  static_cast<std::uint32_t>(rounding_char)
                                     - static_cast<std::uint32_t>('0'));

        bool needs_round_up = (rounding_value >= 5U);

        if(rounding_value == 5U)
        {
          const std::uint32_t ix = static_cast<std::uint32_t>(static_cast<std::uint32_t>(result_str[static_cast<std::string::size_type>(number_of_digits - 1)]) - static_cast<std::uint32_t>('0'));

          if(static_cast<std::uint32_t>(ix & 1U) == static_cast<std::uint32_t>(0U))
          {
            // We have an even digit followed by a 5, so we might not actually
            // need to round up if all the remaining digits are zero.

            if(result_str.find_first_not_of(static_cast<char>('0'), static_cast<std::string::size_type>(number_of_digits + 1)) == std::string::npos)
            {
              bool all_zeros = true;

              // There aren't any non-zero trailing digits in the string.
              // Check whatever parts we didn't convert to the string.
              for(std::size_t i = number_of_elements; i < my_data.size(); ++i)
              {
                if(my_data[i])
                {
                  all_zeros = false;

                  break;
                }
              }

              if(all_zeros)
              {
                // There is a tie break. Round to even.
                needs_round_up = false;
              }
            }
          }
        }

        // Truncate the string.
        result_str.erase(static_cast<std::string::size_type>(number_of_digits));

        if(needs_round_up)
        {
          std::size_t ix = static_cast<std::size_t>(result_str.length() - 1U);

          // Every trailing 9 must be rounded up.
          while(ix && (static_cast<std::int32_t>(result_str[ix]) - static_cast<std::int32_t>('0') == static_cast<std::int32_t>(9)))
          {
            result_str[ix] = static_cast<char>('0');

            --ix;
          }

          if(ix == 0U)
          {
            // There were nothing but trailing nines.
            if(static_cast<std::int32_t>(static_cast<std::int32_t>(result_str[ix]) - static_cast<std::int32_t>(0x30)) == static_cast<std::int32_t>(9))
            {
              // Increment up to the next order and adjust the exponent.
              result_str[ix] = static_cast<char>('1');

              ++exp_value;
            }
            else
            {
              // Round up this digit.
              ++result_str[ix];
            }
          }
          else
          {
            // Round up the last digit.
            ++result_str[ix];
          }
        }
      }

      if(have_leading_zeros)
      {
        // We need to take the zeros back out again,
        // and correct the exponent if we rounded up.

        if(result_str[std::string::size_type(number_of_digits - 1)] != static_cast<char>('0'))
        {
          ++exp_value;

          result_str.erase(static_cast<std::string::size_type>(0U),
                           std::string::size_type(number_of_digits - 1));
        }
        else
        {
          result_str.erase(static_cast<std::string::size_type>(0U),
                           std::string::size_type(number_of_digits));
        }
      }

      mp::detail::format_float_string(result_str,
                                      exp_value,
                                      org_digits,
                                      format_flags,
                                      (isneg) (),
                                      (iszero)());
    }
  }
  return true;
}

std::uint64_t mp::mp_cpp::to_uint64(const mp_cpp& x)
{
  const mp_cpp xn   = integer_part(x);

  const bool is_neg = (x.isneg)();

  const mp_cpp xx = ((is_neg == false) ?  xn : -xn);

  std::uint64_t val;

  static const std::uint64_t value_uint64_max_as_uint64((std::numeric_limits<std::uint64_t>::max)());
  static const mp_cpp        value_uint64_max_as_mp_cpp(value_uint64_max_as_uint64);

  if(xx > value_uint64_max_as_mp_cpp)
  {
    val = value_uint64_max_as_uint64;
  }
  else if(xx.my_exp < static_cast<std::int64_t>(0))
  {
    val = static_cast<std::int64_t>(0);
  }
  else
  {
    // Extract my_data into the int64 value.
    val = static_cast<std::int64_t>(xx.my_data[0U]);

    for(std::int32_t i = static_cast<std::int32_t>(1); i <= static_cast<std::int32_t>(static_cast<std::int32_t>(xx.my_exp) / mp_core::mp_elem_digits10); ++i)
    {
      val *= static_cast<std::uint64_t>(mp_core::mp_elem_mask);
      val += static_cast<std::uint64_t>(xx.my_data[i]);
    }
  }

  if(is_neg)
  {
    val = static_cast<std::uint64_t>(-static_cast<std::int64_t>(val));
  }

  return val;
}

std::int64_t mp::mp_cpp::to_int64(const mp_cpp& x)
{
  const mp_cpp xn   = integer_part(x);

  const bool b_neg = (x.isneg)();


  const mp_cpp xx = ((b_neg == false) ?  xn : -xn);

  const std::uint64_t u = to_uint64(xx);

  std::int64_t val = static_cast<std::int64_t>(u);

  if(b_neg)
  {
    val = -val;
  }

  return val;
}

void mp::mp_cpp::to_parts(const mp::mp_cpp& x, double& mantissa, std::int64_t& exponent)
{
  // Extract from the input mp_cpp& value x the
  // approximate parts of mantissa and base-10 exponent.

  // Extracts the mantissa and exponent.
  exponent = x.my_exp;
  
  std::uint32_t p10  = static_cast<std::uint32_t>(1U);
  std::uint32_t test = x.my_data[0];

  while(test /= static_cast<std::uint32_t>(10U))
  {
    p10 *= static_cast<std::uint32_t>(10U);

    ++exponent;
  }

  mantissa =   static_cast<double>(x.my_data[0U])
             + static_cast<double>(x.my_data[1U]) / mp_core::mp_elem_mask
             + static_cast<double>(x.my_data[2U]) / (static_cast<double>(mp_core::mp_elem_mask) * mp_core::mp_elem_mask);

  mantissa /= static_cast<double>(p10);

  if(x.my_neg)
  {
    mantissa = -mantissa;
  }
}

mp::mp_cpp mp::mp_cpp::integer_part(const mp::mp_cpp& x)
{
  if((x.isnan)() || (x.isinf)())
  {
    return mp_cpp(x);
  }

  // Compute the signed integer part of x.

  if(x.my_exp < static_cast<std::int64_t>(0))
  {
    // The absolute value of the number is smaller than 1.
    // Thus the integer part is zero.
    return mp::zero();
  }
  else if(x.my_exp >= static_cast<std::int64_t>(std::numeric_limits<mp_cpp>::digits10 - 1))
  {
    // The number is too large to resolve the integer part.
    // Thus it is already a pure integer part.
    return mp_cpp(x);
  }

  mp_cpp xn(x);

  // Clear out the decimal portion
  const size_t first_clear = (static_cast<size_t>(xn.my_exp) / static_cast<size_t>(mp_core::mp_elem_digits10)) + 1U;
  const size_t last_clear  =  static_cast<size_t>(mp_elem_number());

  std::fill(xn.my_data.begin() + first_clear,
            xn.my_data.begin() + last_clear,
            static_cast<std::uint32_t>(0U));

  return xn;
}

mp::mp_cpp mp::mp_cpp::decimal_part(const mp::mp_cpp& x)
{
  if((x.isnan)() || (x.isinf)())
  {
    return x;
  }

  if((x.iszero)())
  {
    return mp::zero();
  }

  // Compute the signed decimal part of x.

  if(x.my_exp < static_cast<std::int64_t>(0))
  {
    // The absolute value of the number is smaller than 1.
    // Thus it is already a pure decimal part.
    return x;
  }
  else if(x.my_exp >= static_cast<std::int64_t>(std::numeric_limits<mp_cpp>::digits10 - 1))
  {
    // The number is too large to have a decimal part.
    // Thus the decimal part is zero.
    return mp::zero();
  }

  mp_cpp xd(x);

  const std::size_t first_copy = static_cast<size_t>((static_cast<size_t>(xd.my_exp) / static_cast<size_t>(mp_core::mp_elem_digits10)) + 1U);
  const std::size_t last_copy  = static_cast<size_t>(mp_elem_number());

  std::copy(xd.my_data.cbegin() + first_copy,
            xd.my_data.cbegin() + last_copy,
            xd.my_data.begin());

  const size_t first_clear = static_cast<size_t>(mp_elem_number() - first_copy);
  const size_t last_clear  = static_cast<size_t>(mp_elem_number());

  std::fill(xd.my_data.begin() + first_clear,
            xd.my_data.begin() + last_clear,
            static_cast<std::uint32_t>(0U));

  // Is it necessary to justify the data?
  const array_type::const_iterator first_nonzero_elem =
    std::find_if(xd.my_data.cbegin(),
                 xd.my_data.cend(),
                 [](const std::uint32_t& u) -> bool
                 {
                   return (u != static_cast<std::uint32_t>(0U));
                 });

  std::size_t sj = static_cast<std::size_t>(0U);

  if(first_nonzero_elem != xd.my_data.cbegin())
  {
    if(first_nonzero_elem == xd.my_data.cend())
    {
      // The decimal part is exactly zero.
      // Reset the sign and the exponent.
      xd.my_neg = false;
      xd.my_exp = static_cast<std::int64_t>(0);
    }
    else
    {
      // Justify my_data
      sj = static_cast<std::size_t>(first_nonzero_elem - xd.my_data.cbegin());

      std::copy(xd.my_data.cbegin() + sj,
                xd.my_data.cend(),
                xd.my_data.begin());

      std::fill(xd.my_data.begin() + static_cast<std::size_t>(static_cast<std::size_t>(mp_elem_number()) - sj),
                xd.my_data.end(),
                static_cast<std::uint32_t>(0U));
    }
  }

  xd.my_exp -= static_cast<std::int64_t>((first_copy + sj) * static_cast<size_t>(mp_core::mp_elem_digits10));
  
  return xd;
}
