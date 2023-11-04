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
  FloatingPointType pown_template(const FloatingPointType&         b,
                                  const OtherUnsignedIntegralType& p)
  {
    // Calculate (b ^ p).

    using local_floating_point_type    = FloatingPointType;
    using local_unsigned_integral_type = OtherUnsignedIntegralType;

    local_floating_point_type result;

    if     (p == local_unsigned_integral_type(0U)) { result = local_floating_point_type(1U); }
    else if(p == local_unsigned_integral_type(1U)) { result = b; }
    else if(p == local_unsigned_integral_type(2U)) { result = b; result *= b; }
    else
    {
      result = local_floating_point_type(1U);

      local_floating_point_type y(b);

      // TBD: Use the ladder method.
      for(local_unsigned_integral_type p_local(p); p_local != local_unsigned_integral_type(0U); p_local >>= 1U)
      {
        if((static_cast<unsigned>(p_local) & 1U) != 0U)
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
