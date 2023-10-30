///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//


#ifndef _MP_KARATSUBA_MULTIPLY_2011_06_27_H_
  #define _MP_KARATSUBA_MULTIPLY_2011_06_27_H_

  #include "mp_karatsuba_base.h"

  struct mul_loop_karatsuba : public mul_loop_karatsuba_base
  {
    static void loop(const std::size_t N, const std::uint32_t* const u, const std::uint32_t* const v, std::uint32_t* const r, std::uint32_t* scratch32, std::uint64_t* scratch64)
    {
      #define KARATSUBA_BASECASE_MULTIPLY_LOWER_LIMIT static_cast<std::size_t>(64u)

      if(N == KARATSUBA_BASECASE_MULTIPLY_LOWER_LIMIT)
      {
        static_cast<void>(scratch32);
        mul_loop_uv_n_by_n_to_2n<KARATSUBA_BASECASE_MULTIPLY_LOWER_LIMIT>(u, v, r, scratch64);
      }
      else
      {
        const std::size_t N_half = static_cast<std::size_t>(N / 2u);
        const std::size_t two_N  = static_cast<std::size_t>(N * 2u);

        std::uint32_t* t  = scratch32;
        scratch32 += two_N;

        std::fill(t, t + two_N, static_cast<std::uint32_t>(0u));

        const std::uint32_t* const u1 = u;
        const std::uint32_t* const v1 = v;

        const std::uint32_t* const u0 = u + N_half;
        const std::uint32_t* const v0 = v + N_half;

        // Calculate u1*v1 and store the result in the left-hand half of r.
        mul_loop_karatsuba::loop(N_half, u1, v1, r, scratch32, scratch64);
        scratch64 += N;

        // Calculate u0*v0 and store the result in the right-hand half of r.
        mul_loop_karatsuba::loop(N_half, u0, v0, r + N, scratch32, scratch64);
        scratch64 += N;

        // Copy r to t.
        std::copy(r, r + two_N, t);

        // Add u1*v1 (stored in t + 0) and u0*v0 (or t + N) to the middle two quarters of r.
        const std::uint32_t carry_u1_v1 = add_loop_uv(N, r + N_half, t);

        if(carry_u1_v1 != static_cast<std::uint32_t>(0u))
        {
          // Handle the carry of u1*v1 and propagate it to the left if needed.
          propogate_carry(r + (N_half - 1u), r - 1u);
        }

        // Add u0*v0 (stored in t + N) to the middle two quarters of r.
        const std::uint32_t carry_u0_v0 = add_loop_uv(N, r + N_half, t + N);
        if(carry_u0_v0 != static_cast<std::uint32_t>(0u))
        {
          // Handle the carry of u0*v0 and propagate it to the left if needed.
          propogate_carry(r + (N_half - 1u), r - 1u);
        }

        // Calculate |u1 - u0| and store the result in t + 0.
        // Calculate |v0 - v1| and store the result in t + N/2.

        // Take note of the signs of (u1 - u0) and (v0 - v1).
        const bool u1_is_gtr_u0 = (cmp_data(u1, u1 + N_half, u0) == static_cast<std::int32_t>(1));
        const bool v0_is_gtr_v1 = (cmp_data(v0, v0 + N_half, v1) == static_cast<std::int32_t>(1));

        if(u1_is_gtr_u0)
        {
          std::copy(u1, u1 + N_half, t);
          sub_loop_uv(N_half, t, u0);  // Now (u1 - u0) > 0 is stored in (t + 0).
        }
        else
        {
          std::copy(u0, u0 + N_half, t);
          sub_loop_uv(N_half, t, u1);  // Now (u0 - u1) > 0 is stored in (t + 0).
        }

        if(v0_is_gtr_v1)
        {
          std::copy(v0, v0 + N_half, t + N_half);
          sub_loop_uv(N_half, t + N_half, v1);  // Now (v0 - v1) > 0 is stored in (t + N/2).
        }
        else
        {
          std::copy(v1, v1 + N_half, t + N_half);
          sub_loop_uv(N_half, t + N_half, v0);  // Now (v1 - v0) > 0 is stored in (t + N/2).
        }

        // Now multiply |u1 - u0| * |v0 - v1| and store the result in (t + N).
        std::fill(t + N, t + two_N, static_cast<std::uint32_t>(0u));
        mul_loop_karatsuba::loop(N_half, t, t + N_half, t + N, scratch32, scratch64);
        scratch64 += N;

        // If (u1 - u0) * (v0 - v1) is positive, add it to the result at (r + N/2).
        // Otherwise, subtract it from the result at (r + N/2).
        const bool b_neg = (u1_is_gtr_u0 != v0_is_gtr_v1);

        if(b_neg)
        {
          const std::int32_t borrow = sub_loop_uv(N, r + N_half, t + N);

          if(borrow != static_cast<std::int32_t>(0u))
          {
            // Handle the borrow and propagate it to the left if needed.
            propogate_borrow(r + (N_half - 1u), r - 1u);
          }
        }
        else
        {
          const std::uint32_t carry = add_loop_uv(N, r + N_half, t + N);

          if(carry != static_cast<std::uint32_t>(0u))
          {
            // Handle the carry and propagate it to the left if needed.
            propogate_carry(r + (N_half - 1u), r - 1u);
          }
        }
      }
    }
  };

#endif // _MP_KARATSUBA_MULTIPLY_2011_06_27_H_
