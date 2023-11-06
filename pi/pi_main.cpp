///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : pi_main.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Test pi calculation for multiple precision math.
// 
// *****************************************************************************

#include <iomanip>
#include <iostream>

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
  const auto result_pi_is_ok = samples::pi(argc, argv);

  const auto flg = std::cout.flags();

  std::cout << "result_pi_is_ok: " << std::boolalpha << result_pi_is_ok << std::endl;

  std::cout.flags(flg);

  const auto result_of_main = (result_pi_is_ok ? 0 : -1);

  std::cout << "result_of_main : " << result_of_main << std::endl;

  return result_of_main;
}
