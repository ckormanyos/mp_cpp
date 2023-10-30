///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_cpp_limits.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Specialization of std::numeric_limits<mp_cpp> for multiple precision math
// 
// *****************************************************************************

#include <mp/mp_cpp.h>

const mp::mp_cpp& (std::numeric_limits<mp::mp_cpp>::min)         () { return mp::mp_cpp::value_min(); }
const mp::mp_cpp& (std::numeric_limits<mp::mp_cpp>::max)         () { return mp::mp_cpp::value_max(); }
const mp::mp_cpp&  std::numeric_limits<mp::mp_cpp>::lowest       () { return mp::zero(); }
const mp::mp_cpp&  std::numeric_limits<mp::mp_cpp>::epsilon      () { return mp::mp_cpp::value_eps(); }
const mp::mp_cpp&  std::numeric_limits<mp::mp_cpp>::round_error  () { return mp::half(); }
const mp::mp_cpp&  std::numeric_limits<mp::mp_cpp>::infinity     () { return mp::mp_cpp::value_inf(); }
const mp::mp_cpp&  std::numeric_limits<mp::mp_cpp>::quiet_NaN    () { return mp::mp_cpp::value_nan(); }
const mp::mp_cpp&  std::numeric_limits<mp::mp_cpp>::signaling_NaN() { return mp::zero(); }
const mp::mp_cpp&  std::numeric_limits<mp::mp_cpp>::denorm_min   () { return mp::zero(); }

namespace std
{
  std::int64_t numeric_limits<mp::mp_cpp>::digits;
  std::int64_t numeric_limits<mp::mp_cpp>::digits10;
  std::int64_t numeric_limits<mp::mp_cpp>::max_digits10;
}
