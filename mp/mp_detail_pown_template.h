///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_detail_pown_template.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Integer power template function pown_template().
// 
// *****************************************************************************

#ifndef MP_DETAIL_POWN_TEMPLATE_2019_03_02_H
  #define MP_DETAIL_POWN_TEMPLATE_2019_03_02_H

  #include <type_traits>

  namespace mp { namespace detail {

  // *****************************************************************************
  // Function    : template<typename FloatingPointType,
  //                        typename OtherUnsignedIntegralType>
  //               FloatingPointType pown_template(const FloatingPointType&         b,
  //                                               const OtherUnsignedIntegralType& p)
  // 
  // Description : Template function for computing (b ^ p). A ladder method
  //               is used. Computational complexity scales with O(log2(p)).
  // 
  // *****************************************************************************
  template<typename FloatingPointType,
           typename OtherUnsignedIntegralType>
  auto pown_template(const FloatingPointType&         b,
                     const OtherUnsignedIntegralType& p) -> typename std::enable_if<std::is_unsigned<OtherUnsignedIntegralType>::value, FloatingPointType>::type
  {
    // Calculate (b ^ p).

    using local_floating_point_type    = FloatingPointType;
    using local_unsigned_integral_type = OtherUnsignedIntegralType;

    local_floating_point_type result { };

    if     (p == static_cast<local_unsigned_integral_type>(UINT8_C(0))) { result = local_floating_point_type(static_cast<unsigned>(UINT8_C(1))); }
    else if(p == static_cast<local_unsigned_integral_type>(UINT8_C(1))) { result = b; }
    else if(p == static_cast<local_unsigned_integral_type>(UINT8_C(2))) { result = b; result *= b; }
    else if(p == static_cast<local_unsigned_integral_type>(UINT8_C(3))) { result = b; result *= b; result *= b; }
    else if(p == static_cast<local_unsigned_integral_type>(UINT8_C(4))) { result = b; result *= b; result *= result; }
    else
    {
      // Use the ladder method for calculating the integral power.

      result = local_floating_point_type(static_cast<unsigned>(UINT8_C(1)));

      auto y(b);

      for(local_unsigned_integral_type p_local(p); p_local != local_unsigned_integral_type(0U); p_local >>= static_cast<unsigned>(UINT8_C(1)))
      {
        const auto lo_bit =
          static_cast<unsigned>
          (
            static_cast<unsigned>(p_local) & static_cast<unsigned>(UINT8_C(1))
          );

        if(lo_bit != static_cast<unsigned>(UINT8_C(0)))
        {
          result *= y;
        }

        y *= y;
      }
    }

    return result;
  }

  } } // namespace mp::detail

#endif // MP_DETAIL_POWN_TEMPLATE_2019_03_02_H
