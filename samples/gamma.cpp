///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : gamma.cpp
// 
// Project     : Samples for multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Gamma function calculation using multiple
//               precision math library.
//               The Lanczos approximation is used.
//               This approximation gives results with precision
//               equal to about half of the number of decimal
//               digits in the multiple precision numbers.
//
//               The Lanczos coefficients are determined using
//               the methods which are described in:
//               http://www.rskey.org/lanczos.htm
//               http://www.rskey.org/gamma.htm
//               Considerable effort has been taken to improve
//               the run-time characteristics of the algorithms.
//               In trade-off, the memory usage of the matrix
//               calculations is significant.
//
//               To facilitate the algorithms, rudimentary template
//               libraries for 1D-vectors and 2D-matrices have been
//               implemented. Functionalities for basic mathematical
//               operations and inner products have been implemented.
// 
// *****************************************************************************

#include <array>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

#include <samples/gamma.h>
#include <samples/samples.h>

#define MP_CPP_SAMPLES_GAMMA_USE_MP_CPP
//#define MP_CPP_SAMPLES_GAMMA_USE_BOOST_MULTIPRECISION

#define MP_CPP_SAMPLES_GAMMA_DIGITS10 500

#if defined(MP_CPP_SAMPLES_GAMMA_USE_MP_CPP)

#include <mp/mp_math.h>

using local_mp_type = mp::mp_cpp;

template<typename IntegralType>
local_mp_type local_factorial(const IntegralType i)
{
  return mp::factorial(static_cast<std::int32_t>(i));
}

#endif

#if defined(MP_CPP_SAMPLES_GAMMA_USE_BOOST_MULTIPRECISION)

#include <boost/math/special_functions/factorials.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>

using local_mp_type =
  boost::multiprecision::number<boost::multiprecision::cpp_dec_float<MP_CPP_SAMPLES_GAMMA_DIGITS10>,
                                boost::multiprecision::et_off>;

template<typename IntegralType>
local_mp_type local_factorial(const IntegralType i)
{
  return boost::math::factorial<local_mp_type>(i);
}

namespace mp { struct mp_base { static bool create_mp_base(int) { return true; } }; }

#endif

// *****************************************************************************
// Function    : bool gamma(int argc, const char* argv[])
// 
// Description : Compute Lanczos coefficients and write them to an output file.
// 
// *****************************************************************************
bool samples::gamma(const int argc, const char* argv[])
{
  static_cast<void>(argc);

  std::string::size_type p;

  // Get the output file path relative to path of executable program.
  std::string str_outfile = std::string(argv[0U]);

  if((p = str_outfile.rfind(".exe")) != std::string::npos)
  {
    str_outfile = str_outfile.substr(0U, p + 1U) + "out";
  }
  else if(   (p = str_outfile.rfind('\\'))   != std::string::npos
          || (p = str_outfile.rfind('/' ))   != std::string::npos)
  {
    str_outfile = str_outfile.substr(0U, p + 1U) + "gamma.out";
  }
  else
  {
    str_outfile += ".out";
  }

  bool b_ok = false;

  // Open the output data file.
  std::ofstream outfile(str_outfile.c_str(),
                        std::ios_base::out | std::ios_base::trunc);

  // Set the precision and calculate the Lanczos coefficients.
  if(mp::mp_base::create_mp_base(MP_CPP_SAMPLES_GAMMA_DIGITS10) && outfile.is_open())
  {
    using gamma_lanczos_type = my_gamma::lanczos::lanczos<local_mp_type>;

    // Compute the Lanczos coefficients.
    static_cast<void>(gamma_lanczos_type::get_coefficients(true));

    // Write file information using comments in C++ style.
    outfile << '/' << '/' << " Lanczos coefficients:\n";
    outfile << '/' << '/' << "   n = " << gamma_lanczos_type::get_n() << '\n';
    outfile << '/' << '/' << "   g = " << gamma_lanczos_type::get_g()  << '\n';
    outfile << '/' << '/' << "   std::numeric_limits<local_mp_type>::digits10 = " << std::numeric_limits<local_mp_type>::digits10  << '\n';
    outfile << "static const local_mp_type lanczos_coefficients[] =" << '\n';
    outfile << "{\n";

    // Write string representation of the coefficients
    // to the output file using C++ array syntax.
    for(std::int32_t i = INT32_C(0); i < static_cast<std::int32_t>(gamma_lanczos_type::get_coefficients().size()); ++i)
    {
      outfile << "  local_mp_type(\""
              << std::setprecision(std::numeric_limits<local_mp_type>::digits10)
              << gamma_lanczos_type::get_coefficients()[i]
              << (i < gamma_lanczos_type::get_n() - 1 ? "\")," : "\")")
              << '\n';
    }

    outfile << "};" << std::endl;

    outfile.close();

    // Perform some some spot tests.

    std::int32_t i;

    // Test gamma for integer values.
    std::cout << "Test gamma function for some integers:" << std::endl << std::endl;

    for(i = INT32_C(0); i <= INT32_C(101); ++i)
    {
      std::cout << std::setprecision(std::numeric_limits<local_mp_type>::digits10 / 2)
                << gamma_lanczos_type::gamma(local_mp_type(i) + local_mp_type(1U))
                << std::endl;
    }

    --i;

    std::cout << std::endl;

    // Test last result using factorial
    std::cout << "Test last result using factorial of " << i << std::endl << std::endl;
    std::cout << std::setprecision(std::numeric_limits<local_mp_type>::digits10 / 2)
              << local_factorial(i)
              << std::endl
              << std::endl;

    // Test gamma for positive fractions.
    std::cout << "Test gamma function for positive fractions:" << std::endl << std::endl;

    for(i = INT32_C(2); i <= INT32_C(10); ++i)
    {
      std::cout << std::setprecision(std::numeric_limits<local_mp_type>::digits10 / 2)
                << gamma_lanczos_type::gamma(local_mp_type(1U) / i)
                << std::endl;
    }

    std::cout << std::endl;

    // Test gamma for negative fractions.
    std::cout << "Test gamma function for negative fractions:" << std::endl << std::endl;

    const std::array<local_mp_type, 5U> fractions =
    {{
      -local_mp_type(0.25F),
      -local_mp_type(local_mp_type(1U)) / 3,
      -local_mp_type(0.50F),
      -local_mp_type(local_mp_type(2U)) / 3,
      -local_mp_type(0.75F)
    }};

    for(i = INT32_C(0); i < static_cast<std::int32_t>(fractions.size()); ++i)
    {
      std::cout << std::setprecision(std::numeric_limits<local_mp_type>::digits10 / 2)
                << gamma_lanczos_type::gamma(fractions[i])
                << std::endl;
    }

    std::cout << std::endl;

    b_ok = true;
  }

  return b_ok;
}
