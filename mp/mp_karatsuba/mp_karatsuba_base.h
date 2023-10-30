///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _MP_KARATSUBA_BASE_2011_06_27_H_
  #define _MP_KARATSUBA_BASE_2011_06_27_H_

  #include <algorithm>
  #include <cstdint>

  #include "../mp_core.h"

  struct mul_loop_karatsuba_base
  {
  private:
    static const std::int32_t my_base = mp_core::mp_elem_mask;

  public:
    template<const std::size_t N>
    static void mul_loop_uv_n_by_n_to_2n(const std::uint32_t* const u, const std::uint32_t* const v, std::uint32_t* const w, std::uint64_t* scratch64)
    {
      // Use scratch memory for the intermediate results.
      // The caller has already cleared the scratch memory to zero.
      // The caller will also increment the scratch memory for the next call.
      std::uint64_t* w2 = scratch64;

      for(std::int32_t j = static_cast<std::int32_t>(N - 1u); j >= static_cast<std::int32_t>(0); j--)
      {
        for(std::int32_t i = static_cast<std::int32_t>(N - 1u); i >= static_cast<std::int32_t>(0); i--)
        {
          w2[i + j + 1] += static_cast<std::uint64_t>(u[j] * static_cast<std::uint64_t>(v[i]));
        }
      }

      // Release the carries.
      std::uint64_t carry = static_cast<std::uint64_t>(0u);

      for(std::int32_t j = static_cast<std::int32_t>(static_cast<std::size_t>(2u * N) - 1u); j >= static_cast<std::int32_t>(0); j--)
      {
        const std::uint64_t t = w2[j] + carry;

        w[j]  = static_cast<std::uint32_t>(t % my_base);
        carry = static_cast<std::uint64_t>(t / my_base);
      }
    }

    template<>
    static void mul_loop_uv_n_by_n_to_2n<std::size_t(32u)>(const std::uint32_t* const u, const std::uint32_t* const v, std::uint32_t* const w, std::uint64_t* scratch64)
    {
      // TBD: Implement a template specialization for 32*32 --> 64 word multiply.
      static_cast<void>(u);
      static_cast<void>(v);
      static_cast<void>(w);
      static_cast<void>(scratch64);
    }

  protected:
    static std::uint32_t add_loop_uv(const std::size_t N, std::uint32_t* const u, const std::uint32_t* const v)
    {
      // Implement the addition algorithm u -= v.
      // Return the carry flag.
      std::uint32_t carry = static_cast<std::uint32_t>(0u);

      for(std::int32_t j = static_cast<std::int32_t>(N - 1); j >= static_cast<std::int32_t>(0); j--)
      {
        const std::uint32_t t = static_cast<std::uint32_t>(static_cast<std::uint32_t>(u[j] + v[j]) + carry);
        carry          = static_cast<std::uint32_t>(t / static_cast<std::uint32_t>(my_base));
        u[j]           = static_cast<std::uint32_t>(t % static_cast<std::uint32_t>(my_base));
      }

      return carry;
    }

    static std::int32_t sub_loop_uv(const std::size_t N, std::uint32_t* const u, const std::uint32_t* const v)
    {
      // Implement the subtraction algorithm u -= v.
      // Return the borrow flag.
      std::int32_t borrow = static_cast<std::int32_t>(0);

      for(std::uint32_t j = static_cast<std::uint32_t>(N - 1); static_cast<std::int32_t>(j) >= static_cast<std::int32_t>(0); j--)
      {
        std::int32_t t = static_cast<std::int32_t>(static_cast<std::int32_t>(u[j]) - static_cast<std::int32_t>(v[j])) - borrow;

        // Underflow? Borrow?
        if(t < static_cast<std::int32_t>(0))
        {
          // Yes, underflow and borrow
          t     += my_base;
          borrow = static_cast<std::int32_t>(1);
        }
        else
        {
          borrow = static_cast<std::int32_t>(0);
        }

        u[j] = static_cast<std::uint32_t>(static_cast<std::uint32_t>(t) % static_cast<std::uint32_t>(my_base));
      }

      return borrow;
    }

    static void propogate_carry(std::uint32_t* p_carry, const std::uint32_t* p_end)
    {
      while(p_carry != p_end)
      {
        if(*p_carry >= static_cast<std::uint32_t>(my_base - 1u))
        {
          *p_carry = static_cast<std::uint32_t>(0u);
        }
        else
        {
          *p_carry = static_cast<std::uint32_t>((*p_carry) + 1u);
          break;
        }

        --p_carry;
      }
    }

    static void propogate_borrow(std::uint32_t* p_borrow, const std::uint32_t* p_end)
    {
      while(p_borrow != p_end)
      {
        if(*p_borrow == static_cast<std::uint32_t>(0u))
        {
          *p_borrow = static_cast<std::uint32_t>(my_base - 1u);
        }
        else
        {
          *p_borrow = static_cast<std::uint32_t>((*p_borrow) - 1u);
          break;
        }

        --p_borrow;
      }
    }

    static std::int32_t cmp_data(const std::uint32_t* begin1, const std::uint32_t* end1, const std::uint32_t* begin2)
    {
      const std::pair<const std::uint32_t*, const std::uint32_t*> result = std::mismatch(begin1, end1, begin2);

      return ((result.first == end1) ? static_cast<std::int32_t>(0)
                                     : ((*result.first > *result.second) ? static_cast<std::int32_t>(1) : static_cast<std::int32_t>(-1)));
    }
  };

#endif // _MP_KARATSUBA_BASE_2011_06_27_H_
