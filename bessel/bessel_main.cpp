///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : bessel_main.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Test bessel function lattice calculations for multiple precision math.
// 
// *****************************************************************************

#include <mp/mp_math.h>
#include <samples/samples.h>

// *****************************************************************************
// Function    : int main(int argc, const char* argv[])
// 
// Description : Test multiple precision calculation of Bessel function values.
// 
// *****************************************************************************
int main(int argc, const char* argv[])
{
  // Calculate a lattice of Bessel functions.
  const bool b_ok = samples::bessel(argc, argv);

  return (b_ok ? 0 : 1);
}
