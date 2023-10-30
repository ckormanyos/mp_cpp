///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_base.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Implementation of mp_base multiple precision base class.
// 
// *****************************************************************************

#include <algorithm>
#include <utility>

#include <mp/mp_base.h>
#include <mp/mp_detail_pown_template.h>

bool mp::mp_base::create_mp_base(const std::int32_t my_digits10, const int n_fft_threads)
{
  static const bool mp_core_instance_is_valid =
    mp_core_instance(my_digits10, n_fft_threads).is_valid();

  return mp_core_instance_is_valid;
}

const mp::mp_core& mp::mp_base::mp_core_instance(const std::int32_t my_digits10, const int n_fft_threads)
{
  static const mp_core mp_core_object(my_digits10, n_fft_threads);

  return mp_core_object;
}

void mp::mp_base::precision(const std::int32_t prec_digits)
{
  static const std::int32_t prec_min =
    static_cast<std::int32_t>(static_cast<std::int32_t>(8) * mp_core::mp_elem_digits10);

  const std::int32_t prec = (std::max)(prec_digits, prec_min);

  const int elems =
        (prec / mp_core::mp_elem_digits10)
    + (((prec % mp_core::mp_elem_digits10) != 0) ? 1 : 0);

  static const std::int32_t elems_significant_max =
      (  mp_digits10_tol() / mp_core::mp_elem_digits10)
    + (((mp_digits10_tol() % mp_core::mp_elem_digits10) != 0) ? 1 : 0);

  prec_elem = ((elems > elems_significant_max) ? static_cast<std::int32_t>(mp_elem_number())
                                               : static_cast<std::int32_t>(elems));
}

std::int32_t mp::mp_base::precision() const
{
  const std::int32_t prec_digits(prec_elem * mp_core::mp_elem_digits10);

  return ((prec_digits >= mp_digits10_tol()) ? mp_digits10_tol() : prec_digits);
}

std::int32_t mp::mp_base::compare_data(const array_type& v_data, const std::int32_t my_digits10) const
{
  // Compare my_data of *this with my_data of v, disregarding the sign.

  // First, find out the number of elements that need to be compared,
  // with a minimum of at least one element.
  const std::size_t number_of_elements_to_compare =
    (std::max)(std::size_t(1U),
               std::size_t(    std::int32_t(my_digits10 / mp::mp_core::mp_elem_digits10)
                           + ((std::int32_t(my_digits10 % mp::mp_core::mp_elem_digits10) != 0) ? 1 : 0)));

  // Set the end of element comparison to include the final element
  // of significance, or (at most) to the actual end of the data array.
  const array_type::const_iterator u_compare_end =
      my_data.cbegin()
    + (std::min)(number_of_elements_to_compare, my_data.size());

  // Compare the element arrays using a standard comparison algorithm.
  const std::pair<array_type::const_iterator,
                  array_type::const_iterator> mismatch_result =
    std::mismatch(my_data.cbegin(),
                  u_compare_end,
                  v_data.cbegin());

  std::int32_t compare_result;

  if(mismatch_result.first == u_compare_end)
  {
    // The arrays are identical.
    compare_result = INT32_C(0);
  }
  else
  {
    // The arrays are not identical.
    // A more detailed comparison is required.
    const std::int32_t actual_number_of_digits_compared =
      std::int32_t(number_of_elements_to_compare * mp::mp_core::mp_elem_digits10);

    if(   (actual_number_of_digits_compared > my_digits10)
       && (mismatch_result.first == (u_compare_end - 1U)))
    {
      // Here, we have actually compared more decimal digits than necessary
      // *and* there is a mismatch in the final element of the comparison.

      // We need to manually analyze the individual digits
      // in the final elements that have taken part in the comparison.

      // Find out how many extra digits should be disregarded
      // in the final elements of comparison.
      const std::int32_t number_of_extra_digits_to_disregard =
          std::int32_t(actual_number_of_digits_compared - my_digits10)
        + std::int32_t(order_of_an_element(my_data[0U]) + 1U);

      // Create the denominator for disregarding the extra digits.
      const value_type denominator_for_disregarding_extra_digits =
        value_type(mp::detail::pown_template(value_type(10U), std::int64_t(number_of_extra_digits_to_disregard)));

      // Disregard the extra digits via truncating the values.
      // There is not any sophisticated rounding here.
      const value_type left  = (*mismatch_result.first)  / denominator_for_disregarding_extra_digits;
      const value_type right = (*mismatch_result.second) / denominator_for_disregarding_extra_digits;

      compare_result = ((left == right) ? INT32_C(0) : ((left > right) ? INT32_C(1) : INT32_C(-1)));
    }
    else
    {
      // There is a mismatch somewhere in the data arrays
      // (but not in the last elements). A simple comparison
      // of the elements in the mismatch is sufficient
      // for comparison.

      const value_type left  = (*mismatch_result.first);
      const value_type right = (*mismatch_result.second);

      compare_result = ((left > right) ? INT32_C(1) : INT32_C(-1));
    }
  }

  return compare_result;
}

void mp::mp_base::mul_loop_uv(const std::uint32_t* const u, const std::uint32_t* const v, std::uint32_t* const w, const std::int32_t p)
{
  std::uint64_t carry(0U);

  for(std::int_fast32_t j = static_cast<std::int_fast32_t>(p - 1); j >= static_cast<std::int_fast32_t>(0); --j)
  {
    std::uint64_t sum(carry);

    for(std::uint_fast32_t i = static_cast<std::uint_fast32_t>(0U); i <= static_cast<std::uint_fast32_t>(j); ++i)
    {
      sum +=
        std::uint64_t(  u[std::size_t(i)]
                      * std::uint64_t(v[std::size_t(std::uint_fast32_t(j) - i)]));
    }

    carry =
      std::uint64_t(sum / static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask));

    w[std::size_t(j + 1)] =
      std::uint32_t(sum % static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask));
  }

  w[0U] = static_cast<std::uint32_t>(carry);
}

std::uint32_t mp::mp_base::mul_loop_n(std::uint32_t* const u, std::uint32_t n, const std::int32_t p)
{
  std::uint64_t carry = static_cast<std::uint64_t>(0U);

  // Multiplication loop.
  for(std::int_fast32_t j = static_cast<std::int_fast32_t>(p - 1); j >= static_cast<std::int_fast32_t>(0); --j)
  {
    const std::uint64_t t = static_cast<std::uint64_t>(carry + static_cast<std::uint64_t>(u[std::size_t(j)] * static_cast<std::uint64_t>(n)));
    carry                 = static_cast<std::uint64_t>(t / static_cast<std::uint32_t>(mp_core::mp_elem_mask));
    u[std::size_t(j)]     = static_cast<std::uint32_t>(t % static_cast<std::uint32_t>(mp_core::mp_elem_mask));
  }
  
  return static_cast<std::uint32_t>(carry);
}

std::uint32_t mp::mp_base::div_loop_n(std::uint32_t* const u, std::uint32_t n, const std::int32_t p)
{
  std::uint32_t prev = static_cast<std::uint32_t>(0U);

  if(n > 1U)
  {
    for(std::int_fast32_t j = static_cast<std::int_fast32_t>(0); j < static_cast<std::int_fast32_t>(p); ++j)
    {
      const std::uint64_t t = static_cast<std::uint64_t>(u[std::size_t(j)] + static_cast<std::uint64_t>(prev * static_cast<std::uint64_t>(mp_core::mp_elem_mask)));
      u[std::size_t(j)]     = static_cast<std::uint32_t>(t / n);
      prev                  = static_cast<std::uint32_t>(t % n);
    }
  }

  return static_cast<std::uint32_t>(prev);
}

std::uint32_t mp::mp_base::add_loop_uv(std::uint32_t* const u, const std::uint32_t* const v, const std::int32_t p)
{
  // Implement the addition algorithm u += v.

  std::uint32_t carry = static_cast<std::uint32_t>(0U);

  for(std::int_fast32_t j = static_cast<std::int_fast32_t>(p - 1); j >= static_cast<std::int_fast32_t>(0); --j)
  {
    const std::uint32_t t = static_cast<std::uint32_t>(static_cast<std::uint32_t>(u[std::size_t(j)] + v[std::size_t(j)]) + carry);
    carry                 = static_cast<std::uint32_t>(t / static_cast<std::uint32_t>(mp_core::mp_elem_mask));
    u[std::size_t(j)]     = static_cast<std::uint32_t>(t % static_cast<std::uint32_t>(mp_core::mp_elem_mask));
  }

  // Return the carry flag.

  return carry;
}

std::int32_t mp::mp_base::sub_loop_uv(std::uint32_t* const u, const std::uint32_t* const v, const std::int32_t p)
{
  // Implement the subtraction algorithm u -= v.

  std::int32_t borrow = static_cast<std::int32_t>(0);

  for(std::uint32_t j = static_cast<std::uint32_t>(p - 1); static_cast<std::int32_t>(j) >= static_cast<std::int32_t>(0); --j)
  {
    std::int32_t t =   static_cast<std::int32_t>(static_cast<std::int32_t>(u[std::size_t(j)])
                     - static_cast<std::int32_t>(v[std::size_t(j)])) - borrow;

    // Is there underflow needing borrow?
    if(t < static_cast<std::int32_t>(0))
    {
      // Yes, there is underflow and a borrow is needed.
      t     += mp_core::mp_elem_mask;
      borrow = static_cast<std::int32_t>(1);
    }
    else
    {
      borrow = static_cast<std::int32_t>(0);
    }

    u[std::size_t(j)] =
      static_cast<std::uint32_t>(static_cast<std::uint32_t>(t) % static_cast<std::uint32_t>(mp_core::mp_elem_mask));
  }

  // Return the borrow flag.

  return borrow;
}
