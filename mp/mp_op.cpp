///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_op.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Mathematical operations for multiple precision math.
// 
// *****************************************************************************

#include <algorithm>

#include <mp/mp_cpp.h>
#include <mp/mp_fft/mp_fft_base.h>
#include <mp/mp_fft/mp_fft_multiply.h>

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::operator+=(const mp_cpp& v)
// 
// Description : Addition of mp_cpp *this += v
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::operator+=(const mp::mp_cpp& v)
{
  if(this == &v)
  {
    return operator+=(mp_cpp(v));
  }
  else
  {
    if((v.isinf)())
    {
      return operator=(std::numeric_limits<mp_cpp>::infinity());
    }
    else if((v.isnan)())
    {
      return operator=(std::numeric_limits<mp_cpp>::quiet_NaN());
    }
    else if((isnan)() || (isinf)())
    {
      return *this;
    }
    else
    {
      if((iszero)())
      {
        operator=(v);
      }
      else
      {
        // Get the offset for the add/sub operation.
        const std::int64_t ofs = static_cast<std::int64_t>(static_cast<std::int64_t>(my_exp - v.my_exp) / mp_core::mp_elem_digits10);

        // Do the add/sub operation.
        (my_neg == v.my_neg) ? add(v, ofs) : sub(v, ofs);
      }

      return *this;
    }
  }
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::operator-=(const mp_cpp& v)
// 
// Description : Subtraction of mp_cpp *this -= v
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::operator-=(const mp::mp_cpp& v)
{
  if(this == &v)
  {
    return ((isfinite)() ? operator=(mp::zero()) : operator-=(mp_cpp(v)));
  }
  else
  {
    if((v.isinf)())
    {
      return operator=(std::numeric_limits<mp_cpp>::infinity());
    }
    else if((v.isnan)())
    {
      return operator=(std::numeric_limits<mp_cpp>::quiet_NaN());
    }
    else if((isnan)() || (isinf)())
    {
      return *this;
    }
    else
    {
      // Use *this - v = -(-*this + v)

      my_neg = (!my_neg);

      static_cast<void>(operator+=(v));

      my_neg = (!my_neg);

      return *this;
    }
  }
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::operator*=(const mp_cpp& v)
// 
// Description : Multiplication of mp_cpp *this *= v
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::operator*=(const mp::mp_cpp& v)
{
  if(this == &v)
  {
    return operator*=(mp_cpp(v));
  }
  else
  {
    if((v.isinf)())
    {
      return operator=(std::numeric_limits<mp_cpp>::infinity());
    }
    else if((v.isnan)())
    {
      return operator=(std::numeric_limits<mp_cpp>::quiet_NaN());
    }
    else if((isnan)() || (isinf)())
    {
      return *this;
    }
    else
    {
      if((iszero)() || (v.iszero)())
      {
        // Multiplication by zero.
        my_neg = false;
        my_exp = static_cast<std::int64_t>(0);

        std::fill(my_data.begin(),
                  my_data.begin() + static_cast<std::size_t>(mp_elem_number()),
                  static_cast<std::uint32_t>(0U));
      }
      else
      {
        // Adjust the exponent.
        my_exp += v.my_exp;

        // Set the sign.
        my_neg = (my_neg != v.my_neg);

        const std::size_t prec_mul =
          static_cast<std::size_t>((std::min)(prec_elem, v.prec_elem));

        // For low precision use traditional school multiplication.
        // For high precision use FFT-based multiplication.

        // TBD: For medium precision, find out if Karatsuba or Toom-Cook algorithms might be beneficial?

        if(prec_mul < static_cast<std::size_t>(mp_core::mp_elem_fft_min))
        {
          // Traditional brute-force multiplication.
          std::uint32_t* w = new (std::nothrow) std::uint32_t[prec_mul + 1U];

          std::fill(w,
                    w + static_cast<std::size_t>(prec_mul + 1U),
                    static_cast<std::uint32_t>(0U));

          // Use a (half-sized) traditional school multiplication loop.
          mul_loop_uv(  &my_data[0U],
                      &v.my_data[0U],
                      &w[0U], static_cast<std::int32_t>(prec_mul));

          // Copy the multiplication my_data into the result.
          if(w[0U] != static_cast<std::uint32_t>(0U))
          {
            // Adjust the exponent if necessary.
            my_exp += static_cast<std::int64_t>(mp_core::mp_elem_digits10);

            std::copy(w,
                      w + prec_mul,
                      my_data.begin());
          }
          else
          {
            std::copy(w + static_cast<std::size_t>(1U),
                      w + static_cast<std::size_t>(prec_mul + 1U),
                      my_data.begin());
          }

          delete [] w;
        }
        else
        {
          // Use an FFT-based multiplication.
          mp_fft_multiply(  &my_data[0U],
                          &v.my_data[0U],
                          static_cast<std::int32_t>(prec_mul));

          // Adjust the exponent because of the internal scaling of the FFT multiplication.
          my_exp += static_cast<std::int64_t>(mp_core::mp_elem_digits10);

          // Check for justify
          if(my_data.front() == static_cast<std::uint32_t>(0U))
          {
            // Justify my_data.
            std::copy(my_data.cbegin() + static_cast<std::size_t>(1U),
                      my_data.cbegin() + static_cast<std::size_t>(prec_elem),
                      my_data.begin());

            my_data.back() = static_cast<std::uint32_t>(0U);

            // Adjust the exponent accordingly.
            my_exp -= static_cast<std::int64_t>(mp_core::mp_elem_digits10);
          }
        }
      }
  
      return *this;
    }
  }
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::operator++()
// 
// Description : Increment of mp_cpp *this.
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::operator++()
{
  if((isfinite)() == true)
  {
    if((isneg)() == true)
    {
      negate();
      --(*this);
      negate();
    }
    else
    {
      if(my_exp == 0)
      {
        if((iszero)())
        {
          operator=(one());
        }
        else if(my_data[0U] == (mp_core::mp_elem_mask - 1U))
        {
          operator+=(one());
        }
        else
        {
          ++my_data[0U];
        }
      }
      else
      {
        operator+=(one());
      }
    }
  }

  return *this;
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::operator--()
// 
// Description : Decrement of mp_cpp *this.
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::operator--()
{
  if((isfinite)() == true)
  {
    if((isneg)() == true)
    {
      negate();
      ++(*this);
      negate();
    }
    else
    {
      if((iszero)())
      {
        operator=(one());
        negate();
      }
      else if((isone)())
      {
        operator=(zero());
      }
      else if(my_exp != 0)
      {
        operator-=(one());
      }
      else
      {
        --my_data[0U];
      }
    }
  }

  return *this;
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::operator/=(const mp_cpp& v)
// 
// Description : Division of mp_cpp *this /= v
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::operator/=(const mp_cpp& v)
{
  if((isfinite)())
  {
    if(this == &v)
    {
      return (((iszero)() == false) ? operator=(mp::one()) : operator/=(mp_cpp(v)));
    }
    else
    {
      return operator*=(mp_cpp(v).calculate_inv());
    }
  }
  else
  {
    return *this;
  }
}

// *****************************************************************************
// Function    : void mp_cpp::add(const mp_cpp& v, const std::int64_t v_ofs)
// 
// Description : Addition algorithm
// 
// *****************************************************************************
void mp::mp_cpp::add(const mp::mp_cpp& v, const std::int64_t v_ofs)
{
  // Check if the operation is out of range, requiring special handling.
  if((v.iszero)() || (v_ofs >= static_cast<std::int64_t>(prec_elem)))
  {
    // Return *this unchanged since v is negligible compared to *this.
    return;
  }
  else if(v_ofs <= -static_cast<std::int64_t>(prec_elem))
  {
    // Return *this = v since *this is negligible compared to v.
    operator=(v);

    return;
  }

  // Add v to *this, where my_data of either *this or v might have
  // to be treated with a positive, negative or zero offset.
  // The result is stored in *this. To my_data, add one element
  // at a time with carry.
        std::uint32_t*       my_p_u    = static_cast<      std::uint32_t*>(  &my_data[0]);
  const std::uint32_t*       my_p_v    = static_cast<const std::uint32_t*>(&v.my_data[0]);
        std::uint32_t* const my_mem_n  = mp_core_instance().mp_core_memory->mem_n();

  bool b_copy = false;

  if(v_ofs > static_cast<std::int64_t>(0))
  {
    std::copy(v.my_data.cbegin(),
              v.my_data.cbegin() + static_cast<std::size_t>(static_cast<std::int64_t>(prec_elem) - v_ofs),
              my_mem_n + static_cast<std::size_t>(v_ofs));

    std::fill(my_mem_n,
              my_mem_n + static_cast<std::size_t>(v_ofs),
              static_cast<std::uint32_t>(0U));

    my_p_v = my_mem_n;
  }
  else if(v_ofs < static_cast<std::int64_t>(0))
  {
    std::copy(my_data.cbegin(),
              my_data.cbegin() + static_cast<std::size_t>(prec_elem + v_ofs),
              my_mem_n + static_cast<std::size_t>(-v_ofs));

    std::fill(my_mem_n,
              my_mem_n + static_cast<std::size_t>(-v_ofs),
              static_cast<std::uint32_t>(0U));

    my_p_u = my_mem_n;

    b_copy = true;
  }

  // Addition algorithm
  const std::uint32_t carry = add_loop_uv(my_p_u, my_p_v, prec_elem);

  if(b_copy)
  {
    std::copy(my_mem_n,
              my_mem_n + static_cast<std::size_t>(prec_elem),
              my_data.begin());

    my_exp = v.my_exp;
  }

  // There needs to be a backward carry -1 position in my_data.
  if((carry != static_cast<std::uint32_t>(0U)) && (v_ofs == static_cast<std::int64_t>(0)))
  {
    std::copy_backward(my_data.cbegin(),
                       my_data.cend() - static_cast<std::size_t>(1U),
                       my_data.end());

    my_data[0U] = carry;

    my_exp += static_cast<std::int64_t>(mp_core::mp_elem_digits10);
  }
}

// *****************************************************************************
// Function    : void mp_cpp::sub(const mp_cpp& v, const std::int64_t v_ofs)
// 
// Description : Subtraction algorithm
// 
// *****************************************************************************
void mp::mp_cpp::sub(const mp::mp_cpp& v, const std::int64_t v_ofs)
{
  // Check if the operation is out of range, requiring special handling.
  if((v.iszero)() || v_ofs >= static_cast<std::int64_t>(prec_elem))
  {
    // Return *this unchanged since v is negligible compared to *this.
    return;
  }
  else if(v_ofs <= -static_cast<std::int64_t>(prec_elem))
  {
    // Return *this = v since *this is negligible compared to v.
    operator=(v);

    return;
  }

  // Add v to *this, where the data array of either *this or v
  // might have to be treated with a positive, negative or zero offset.
        std::uint32_t*       my_p_u    = static_cast<      std::uint32_t*>(  &my_data[0]);
  const std::uint32_t*       my_p_v    = static_cast<const std::uint32_t*>(&v.my_data[0]);
        std::uint32_t* const my_mem_n  = mp_core_instance().mp_core_memory->mem_n();

  bool b_copy = false;

  if(       (v_ofs  > static_cast<std::int64_t>(0))
     || (   (v_ofs == static_cast<std::int64_t>(0))
         && (compare_data(v.crepresentation(), prec_elem * mp::mp_core::mp_elem_digits10) > static_cast<std::int32_t>(0))))
  {
    if(v_ofs != static_cast<std::int64_t>(0))
    {
      // In this case, |u| > |v| and ofs is positive.
      // Copy my_data of v, shifted down to a lower value
      // into the array my_mem_n. Set the operand pointer my_p_v
      // to point to the copied and shifted data in my_mem_n.
      std::copy(v.my_data.cbegin(),
                v.my_data.cbegin() + static_cast<std::size_t>(prec_elem - v_ofs),
                my_mem_n + static_cast<std::size_t>(v_ofs));

      std::fill(my_mem_n,
                my_mem_n + static_cast<std::size_t>(v_ofs),
                static_cast<std::uint32_t>(0U));

      my_p_v = static_cast<const std::uint32_t*>(my_mem_n);
    }
  }
  else
  {
    if(v_ofs)
    {
      // In this case, |u| < |v| and ofs is negative.
      // Shift the data of u down to a lower value.
      std::copy_backward(my_data.cbegin(),
                         my_data.cbegin() + static_cast<std::size_t>(prec_elem + v_ofs),
                         my_data.begin()  + static_cast<std::size_t>(prec_elem));

      std::fill(my_data.begin(),
                my_data.begin() + static_cast<std::size_t>(-v_ofs),
                static_cast<std::uint32_t>(0U));
    }

    // Copy my_data of v into the array my_mem_n.
    // Set the u-pointer my_p_u to point to my_mem_n and the
    // operand pointer my_p_v to point to the shifted data in my_mem_n.
    std::copy(v.my_data.cbegin(),
              v.my_data.cbegin() + static_cast<std::size_t>(prec_elem),
              my_mem_n);

    my_p_u    = static_cast<      std::uint32_t*>(my_mem_n);
    my_p_v    = static_cast<const std::uint32_t*>(&my_data[0]);
    b_copy = true;
  }

  // Subtraction algorithm
  const std::int32_t borrow = sub_loop_uv(my_p_u, my_p_v, prec_elem);

  static_cast<void>(borrow);

  if(b_copy)
  {
    std::copy(my_mem_n,
              my_mem_n + static_cast<std::size_t>(prec_elem),
              my_data.begin());

    // Adjust the exponent.
    my_exp = v.my_exp;

    // Adjust the sign.
    my_neg = v.my_neg;
  }

  // Check for justify

  const auto first_nonzero_elem =
    std::find_if(my_data.cbegin(),
                 my_data.cbegin() + static_cast<std::size_t>(prec_elem),
                 [](const std::uint32_t& u) -> bool
                 {
                   return (u != static_cast<std::uint32_t>(0));
                 });

  if(first_nonzero_elem != my_data.cbegin())
  {
    const bool result_of_subtraction_is_zero =
      (   (first_nonzero_elem == my_data.cend())
       || (static_cast<std::size_t>(first_nonzero_elem - my_data.cbegin()) >= static_cast<std::size_t>(prec_elem)));

    if(result_of_subtraction_is_zero)
    {
      // The result of the subtraction is exactly zero
      // to within the precision of the subtraction.

      std::fill(my_data.begin(),
                my_data.end(),
                static_cast<std::uint32_t>(0U));

      my_neg = false;

      my_exp = static_cast<std::int64_t>(0);
    }
    else
    {
      // Justify my_data.

      const std::size_t n_shift = static_cast<std::size_t>(first_nonzero_elem - my_data.cbegin());

      std::copy(my_data.cbegin() + n_shift,
                my_data.cbegin() + static_cast<std::size_t>(prec_elem),
                my_data.begin());

      std::fill(my_data.begin() + static_cast<std::size_t>(static_cast<std::size_t>(prec_elem) - n_shift),
                my_data.end(),
                static_cast<std::uint32_t>(0U));

      my_exp -= static_cast<std::int64_t>(static_cast<std::int64_t>(n_shift) * static_cast<std::int64_t>(mp_core::mp_elem_digits10));
    }
  }
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::mul_by_int(const std::int64_t n)
// 
// Description : Multiplication of mp_cpp with an integer *this *= n
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::mul_by_int(const std::int64_t n)
{
  if(n == 0)
  {
    return ((isfinite)() ? (*this = mp::zero()) : *this);
  }

  const std::uint64_t nn = static_cast<std::uint64_t>(n > static_cast<std::int64_t>(0) ? n : -n);

  if(nn >= static_cast<std::uint64_t>(mp_core::mp_elem_mask))
  {
    operator*=(mp_cpp(n));

    return *this;
  }
  else
  {
    // Set up the range of the multiplication loop.
    const auto highest_nonzero_element_index =
      std::find_if(my_data.crbegin(),
                   my_data.crend(),
                   [](const std::uint32_t& u) -> bool
                   {
                     return (u != static_cast<std::uint32_t>(0));
                   });

    const std::int32_t jmax = static_cast<std::int32_t>(my_data.crend() - highest_nonzero_element_index);
    const std::int32_t jm1  = static_cast<std::int32_t>(jmax + static_cast<std::int32_t>(1));
    const std::int32_t jm   = (std::min)(jm1, prec_elem);

    // Perform the multiplication loop.
    const std::uint32_t carry = mul_loop_n(&my_data[0U], static_cast<std::uint32_t>(nn), jm);

    // Handle the carry and adjust the exponent.
    if(carry != static_cast<std::uint32_t>(0U))
    {
      my_exp += static_cast<std::int64_t>(mp_core::mp_elem_digits10);

      // Shift result of the multiplication one element to the right.
      std::copy_backward(my_data.cbegin(),
                         my_data.cbegin() + static_cast<std::size_t>(jm - 1),
                         my_data.begin()  + static_cast<std::size_t>(jm));

      my_data[static_cast<std::size_t>(0U)] = static_cast<std::uint32_t>(carry);
    }

    my_neg = (my_neg != (n < static_cast<std::int32_t>(0)));

    return *this;
  }
}

// *****************************************************************************
// Function    : mp_cpp& mp_cpp::div_by_int(const std::int64_t n)
// 
// Description : Division of mp_cpp by an integer *this /= n
// 
// *****************************************************************************
mp::mp_cpp& mp::mp_cpp::div_by_int(const std::int64_t n)
{
  if(n == 0)
  {
    return ((isfinite)() ? (*this = value_inf()) : (*this = value_nan()));
  }

  const std::uint64_t nn = static_cast<std::uint64_t>(n > static_cast<std::int64_t>(0) ? n : -n);

  if(nn > static_cast<std::uint64_t>(mp_core::mp_elem_mask))
  {
    // Here, n is larger than the element mask.
    // Multiply by the inverse.
    *this *= mp_cpp(nn).calculate_inv();
  }
  else if(nn == static_cast<std::uint64_t>(mp_core::mp_elem_mask))
  {
    // Adjust exponent for n == +/- mp_core::mp_elem_mask
    my_exp -= static_cast<std::int64_t>(mp_core::mp_elem_digits10);
  }
  else
  {
    // Use a school algorithm for division.
    const std::int32_t jm  = static_cast<std::int32_t>(prec_elem);

    const std::uint32_t prev = div_loop_n(&my_data[0U], static_cast<std::uint32_t>(nn), jm);

    // Determine if one leading zero is in the result (the result is my_data).
    if(my_data[0] == static_cast<std::uint32_t>(0U))
    {
      // Adjust the exponent
      my_exp -= static_cast<std::int64_t>(mp_core::mp_elem_digits10);

      // Shift the result of the division one element to the left.
      std::copy(my_data.cbegin() + static_cast<std::size_t>(1U),
                my_data.cbegin() + static_cast<std::size_t>(jm),
                my_data.begin());

      my_data[static_cast<std::size_t>(jm - 1)] = static_cast<std::uint32_t>(static_cast<std::uint64_t>(prev * static_cast<std::uint64_t>(mp_core::mp_elem_mask)) / nn);
    }
  }

  my_neg = (my_neg != (n < static_cast<std::int32_t>(0)));

  return *this;
}

// *****************************************************************************
// Function    : std::int32_t mp_cpp::compare(const mp_cpp& v) const
// 
// Description : Comparison of *this with v.
//               The subroutine returns:
//                 -1, *this < v
//                  0, *this = v
//                 +1, *this > v
// 
// *****************************************************************************
std::int32_t mp::mp_cpp::compare(const mp::mp_cpp& v) const
{
  // Compare *this with v.
  // Return +1 for *this >  v
  //         0 for *this == v
  //        -1 for *this <  v

  // Handle all non-finite cases.
  if((!(isfinite)()) || (!(v.isfinite)()))
  {
    // NaN can never equal NaN. Return an implementation-dependent
    // signed result. Also note that comparison of NaN with NaN
    // using operators greater-than or less-than is undefined.
    if((isnan)() || (v.isnan)())
    {
      return ((isnan)() ? 1 : -1);
    }

    if((isinf)() && (v.isinf)())
    {
      // Both *this and v are infinite. They are equal if they have the same sign.
      // Otherwise, *this is less than v if (and only if) *this is negative.
      return ((my_neg == v.my_neg) ? 0 : (my_neg ? -1 : 1));
    }

    if((isinf)())
    {
      // *this is infinite, but v is finite.
      // So negative infinite *this is less than any finite v.
      // Whereas positive infinite *this is greater than any finite v.
      return ((isneg)() ? -1 : 1);
    }
    else
    {
      // *this is finite, and v is infinite.
      // So any finite *this is greater than negative infinite v.
      // Whereas any finite *this is less than positive infinite v.
      return (v.my_neg ? 1 : -1);
    }
  }

  // And now handle all *finite* cases.
  if((iszero)())
  {
    // The value of *this is zero and v is either zero or non-zero.
    return ((v.iszero)() ? 0 : (v.my_neg ? 1 : -1));
  }
  else if((v.iszero)())
  {
    // The value of v is zero and *this is non-zero.
    return (my_neg ? -1 : 1);
  }
  else
  {
    // Both *this and v are non-zero.

    if(my_neg != v.my_neg)
    {
      // The signs are different.
      return (my_neg ? -1 : 1);
    }
    else if(my_exp != v.my_exp)
    {
      // The signs are the same and the exponents are different.
      const int val_exp_compare = ((my_exp < v.my_exp) ? 1 : -1);

      return (my_neg ? val_exp_compare : -val_exp_compare);
    }
    else
    {
      // The signs are the same and the exponents are the same.
      // We need to manually compare the data.

      const std::int32_t number_of_digits_to_compare =
        (std::min)(std::int32_t(prec_elem * mp::mp_core::mp_elem_digits10), mp_digits10());

      const int32_t val_cmp_data = compare_data(v.crepresentation(), number_of_digits_to_compare);

      return ((my_neg == false) ? val_cmp_data : -val_cmp_data);
    }
  }
}

// *****************************************************************************
// Function    : bool mp_cpp::iseven() const
// 
// Description : Check if this mp_cpp is even.
// 
// *****************************************************************************
bool mp::mp_cpp::iseven() const
{
  // By design a mp_cpp value that can not be represented as an integer
  // is even. This can be either because the value is too large or
  // smaller than zero in magnitude.

  const bool non_integral_range =
    (   (my_exp <  static_cast<std::int64_t>(0))
     || (my_exp >= static_cast<std::int64_t>((static_cast<std::int64_t>(mp_elem_number()) * mp_core::mp_elem_digits10) - 1)));

  bool this_is_even;

  if(non_integral_range)
  {
    this_is_even = true;
  }
  else
  {
    // Check the least significant limb for even-ness.
    const value_type least_significant_limb =
      my_data[static_cast<std::size_t>(my_exp / static_cast<std::int64_t>(mp_core::mp_elem_digits10))];

    this_is_even = (value_type(least_significant_limb % 2U) == value_type(0U));
  }

  return this_is_even;
}

std::int64_t mp::mp_cpp::order() const
{
  const std::uint_fast32_t prefix = ((((iszero)() == false) && (isfinite)())
                                      ? order_of_an_element(my_data[0U])
                                      : 0U);

  return std::int64_t(my_exp + std::int64_t(prefix));
}
