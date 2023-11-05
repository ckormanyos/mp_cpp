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
#include <array>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include <mp/mp_math.h>
#include <samples/samples.h>

namespace local
{
  constexpr auto str_len_to_check = static_cast<std::string::size_type>(UINT8_C(32));

  float test_real(bool& result_is_ok);
  float test_imag();
}

// *****************************************************************************
// Function    : void local::test_real()
// 
// Description : Test various real-valued functions.
// 
// *****************************************************************************
float local::test_real(bool& result_is_ok)
{
  using mp_values_array_type        = std::array<mp::mp_cpp, static_cast<std::size_t>(UINT8_C(6))>;
  using mp_values_ctrl_strings_type = std::array<std::string, std::tuple_size<mp_values_array_type>::value>;

  const auto mp_values_ctrl_strings =
    mp_values_ctrl_strings_type
    {
      std::string("087890625000000000000000000e+223"),
      std::string("6423093500696066591266110387e-01"),
      std::string("1476916049200907195743034256e-01"),
      std::string("5728219208235453391899827400e-01"),
      std::string("87918311751840086748643e+1505150"),
      std::string("8593839501629585750517154736e+17")
    };

  const mp::mp_cpp x = mp::mp_cpp(12345U) / static_cast<std::int32_t>(10000U);

  // Execute the real-valued test functions and measure the timing.
  const std::clock_t start = std::clock();

  const mp_values_array_type
    mp_values
    {
      pow(x, static_cast<std::int32_t>(INT16_C(2440))),                      // N[(12345 / 10000)^2440,   10002]
      sin(x),                                                                // N[Sin[12345 / 10000],     10002]
      log(x),                                                                // N[Log[12345 / 10000],     10002]
      acosh(x),                                                              // N[ArcCosh[12345 / 10000], 10002]
      mp::pow2(5000001),                                                     // N[2^5000001,              10002]
      exp(mp::pi() * sqrt(mp::mp_cpp(static_cast<unsigned>(UINT8_C(163)))))  // N[Exp[Pi Sqrt[163]],      10002]
    };

  const auto elapsed_time = static_cast<float>(static_cast<float>(std::clock() - start) / CLOCKS_PER_SEC);

  result_is_ok = true;

  auto it_str_ctrl = mp_values_ctrl_strings.cbegin();

  for(const auto& val : mp_values)
  {
    std::stringstream strm;

    strm << std::scientific << std::setprecision(std::numeric_limits<mp::mp_cpp>::digits10) << val;

    const std::string str_val = std::move(strm.str());

    const auto str_check = std::string(str_val.cend() - str_len_to_check, str_val.cend());

    const auto result_val_is_ok = (str_check == *it_str_ctrl++);

    result_is_ok = (result_val_is_ok && result_is_ok);
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
#if 0
double local::test_imag()
{
  std::vector<mp::complex<mp::mp_cpp>> values;

  values.reserve(10U);

  const mp::mp_cpp seven_point_two = mp::mp_cpp(72U) / static_cast<std::int32_t>(10);
  const mp::mp_cpp three_point_one = mp::mp_cpp(31U) / static_cast<std::int32_t>(10);

  const mp::complex<mp::mp_cpp> z(seven_point_two, three_point_one);

  // Execute the complex-valued test functions and measure the timing.
  const std::clock_t start = std::clock();

  values.push_back(mp::one() / z);  // N[1 / ((72/10) + ((31 I)/10))],   10002]
  values.push_back(sin(z));         // N[Sin[(72/10) + ((31 I)/10)],     10002]
  values.push_back(exp(z));         // N[Exp[(72/10) + ((31 I)/10)],     10002]
  values.push_back(log(z));         // N[Log[(72/10) + ((31 I)/10)],     10002]
  values.push_back(acosh(z));       // N[ArcCosh[(72/10) + ((31 I)/10)], 10002]
  values.push_back(sinh(z));        // N[Sinh[(72/10) + ((31 I)/10)],    10002]

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
#endif

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
  const std::int32_t my_digits10    = 10001;
  const int          my_fft_threads =     4;

  // Create the multiple precision base.
  const auto create_mp_base_is_ok = mp::mp_base::create_mp_base(my_digits10, my_fft_threads);

  auto result_is_ok = create_mp_base_is_ok;

  float elapsed_time_both { };
  float elapsed_time_real { };
  //float elapsed_time_imag { };

  if(create_mp_base_is_ok)
  {
    // Test various real-valued functions.
    {
      auto result_real_is_ok = false;

      elapsed_time_real = local::test_real(result_real_is_ok);

      result_is_ok = (result_real_is_ok && result_is_ok);

      elapsed_time_both += elapsed_time_real;
    }

    // Test various complex-valued functions.
    //const double elapsed_time_imag = local::test_imag();

    // Sum up the total test time.

    // Report the test timing.
    std::cout << "Elapsed time for test_real: " << elapsed_time_real << std::endl;
    //std::cout << "Elapsed time for test_imag: " << elapsed_time_imag << std::endl;
    //std::cout << "Elapsed time for both:      " << elapsed_time_both << std::endl;
  }

  return result_is_ok;
}
