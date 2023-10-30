///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//


// *****************************************************************************
// Filename    : gamma_main.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 01-Mar-2004
// 
// Description : Test Lanczos coefficient calculations for multiple precision math.
// 
// *****************************************************************************

#include <mp/mp_math.h>
#include <samples/samples.h>

// *****************************************************************************
// Function    : int main(int argc, const char* argv[])
// 
// Description : Test complicated matrix calculation of Lanczos
//               coefficients for the gamma function.
// 
// *****************************************************************************
int main(int argc, const char* argv[])
{
  // Calculate Lanczos coefficients for the gamma function.
  const bool b_ok = samples::gamma(argc, argv);

  return (b_ok ? 0 : 1);
}

