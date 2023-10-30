///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : test.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Basic test routines for multiple precision math.
// 
// *****************************************************************************

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

#include <mp/mp_math.h>
#include <samples/samples.h>

namespace local
{
  double test_real();
  double test_imag();
}

// *****************************************************************************
// Function    : void local::test_real()
// 
// Description : Test various real-valued functions.
// 
// *****************************************************************************
double local::test_real()
{
  std::vector<mp::mp_cpp> values;

  values.reserve(10U);

  const mp::mp_cpp x = mp::mp_cpp(12345U) / static_cast<std::int32_t>(10000U);

  // Execute the real-valued test functions and measure the timing.
  const std::clock_t start = std::clock();

  values.push_back(rootn(x, static_cast<std::int32_t>(5)));      // N[(12345 / 10000)^5, 10000]
  values.push_back(sin(x));                                      // N[Sin[12345 / 10000], 10000]
  values.push_back(log(x));                                      // N[Log[12345 / 10000], 10000]
  values.push_back(acosh(x));                                    // N[ArcCosh[12345 / 10000], 10000]
  values.push_back(mp::pow2(5000001));                           // N[2^5000001, 10000]
  values.push_back(exp(mp::pi() * mp::sqrt(mp::mp_cpp(163U))));  // N[Exp[Pi Sqrt[163]], 10000]

  const double elapsed_time = double(std::clock() - start) / double(CLOCKS_PER_SEC);

  // Store the original output streamsize.
  const std::streamsize original_stream_size = std::cout.precision();

  // Store an indication if the original output stream has scientific.
  const bool original_stream_has_scientific =
    (static_cast<std::ios::fmtflags>(std::cout.flags() & std::ios::scientific) == std::ios::scientific);

  // Set the output stream precision to that of mp_cpp.
  std::cout.precision(std::numeric_limits<mp::mp_cpp>::digits10);

  // Set the output stream flags to include scientific.
  std::cout.setf(std::ios::scientific);

  // Print the test values.
  std::copy(values.cbegin(),
            values.cend(),
            std::ostream_iterator<mp::mp_cpp>(std::cout, "\n"));

  // Reset the output stream precision to its original streamsize.
  std::cout.precision(original_stream_size);

  // Remove the scientific flag if necessary.
  if((!original_stream_has_scientific))
  {
    std::cout.unsetf(std::ios::scientific);
  }

  // Return the elapsed time.
  return elapsed_time;
}

// *****************************************************************************
// Function    : double local::test_imag()
// 
// Description : Test various complex-valued functions.
// 
// *****************************************************************************
double local::test_imag()
{
  std::vector<mp::complex<mp::mp_cpp>> values;

  values.reserve(10U);

  const mp::mp_cpp seven_point_two = mp::mp_cpp(72U) / static_cast<std::int32_t>(10);
  const mp::mp_cpp three_point_one = mp::mp_cpp(31U) / static_cast<std::int32_t>(10);

  const mp::complex<mp::mp_cpp> z(seven_point_two, three_point_one);

  // Execute the complex-valued test functions and measure the timing.
  const std::clock_t start = std::clock();

  values.push_back(mp::one() / z);  // N[1 / ((72/10) + ((31 I)/10))], 10000]
  values.push_back(sin(z));         // N[Sin[(72/10) + ((31 I)/10)], 10000]
  values.push_back(exp(z));         // N[Exp[(72/10) + ((31 I)/10)], 10000]
  values.push_back(log(z));         // N[Log[(72/10) + ((31 I)/10)], 10000]
  values.push_back(acosh(z));       // N[ArcCosh[(72/10) + ((31 I)/10)], 10000]
  values.push_back(sinh(z));        // N[Sinh[(72/10) + ((31 I)/10)], 10000]

  const double elapsed_time = double(std::clock() - start) / double(CLOCKS_PER_SEC);

  // Store the original output streamsize.
  const std::streamsize original_stream_size = std::cout.precision();

  // Store an indication if the original output stream has scientific.
  const bool original_stream_has_scientific =
    (static_cast<std::ios::fmtflags>(std::cout.flags() & std::ios::scientific) == std::ios::scientific);

  // Set the output stream precision to that of mp_cpp.
  std::cout.precision(std::numeric_limits<mp::mp_cpp>::digits10);

  // Set the output stream flags to include scientific.
  std::cout.setf(std::ios::scientific);

  // Print the test values.
  std::copy(values.cbegin(),
            values.cend(),
            std::ostream_iterator<mp::complex<mp::mp_cpp>>(std::cout, "\n"));

  // Reset the output stream precision to its original streamsize.
  std::cout.precision(original_stream_size);

  // Remove the scientific flag if necessary.
  if((!original_stream_has_scientific))
  {
    std::cout.unsetf(std::ios::scientific);
  }

  // Return the elapsed time.
  return elapsed_time;
}

// *****************************************************************************
// Function    : bool samples::test(const int, const char*[])
// 
// Description : Test various real and complex multiple precision functions.
// 
// *****************************************************************************
bool samples::test(const int, const char*[])
{
  // Set the desired number of decimal digits
  // and the requested count of FFT threads.
  const std::int32_t my_digits10    = 10000;
  const int          my_fft_threads =     4;

  // Create the multiple precision base.
  const bool create_mp_base_is_ok = mp::mp_base::create_mp_base(my_digits10, my_fft_threads);

  if(create_mp_base_is_ok)
  {
    // Test various real-valued functions.
    const double elapsed_time_real = local::test_real();

    // Test various complex-valued functions.
    const double elapsed_time_imag = local::test_imag();

    // Sum up the total test time.
    const double elapsed_time_both = elapsed_time_real + elapsed_time_imag;

    // Report the test timing.
    std::cout << "Elapsed time for test_real: " << elapsed_time_real << std::endl;
    std::cout << "Elapsed time for test_imag: " << elapsed_time_imag << std::endl;
    std::cout << "Elapsed time for both:      " << elapsed_time_both << std::endl;
  }

  return create_mp_base_is_ok;
}
