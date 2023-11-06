///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//


// *****************************************************************************
// Filename    : ln2_main.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Test ln2 calculation for multiple precision math.
// 
// *****************************************************************************

#include <mp/mp_math.h>
#include <samples/samples.h>

// *****************************************************************************
// Function    : int main(int argc, const char* argv[])
// 
// Description : Test multiple precision calculation of pi.
// 
// *****************************************************************************
int main(int argc, const char* argv[])
{
  // Calculate pi.
  const bool b_ok = samples::ln2(argc, argv);

  return (b_ok ? 0 : 1);
}
