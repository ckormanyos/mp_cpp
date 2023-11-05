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
  float test_imag(bool& result_is_ok);

  struct complex_strings_pair
  {
    std::string str_real { };
    std::string str_imag { };
  };
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
float local::test_imag(bool& result_is_ok)
{
  using mp_complex_values_array_type        = std::array<mp::complex<mp::mp_cpp>, static_cast<std::size_t>(UINT8_C(6))>;
  using mp_complex_values_ctrl_strings_type = std::array<complex_strings_pair, std::tuple_size<mp_complex_values_array_type>::value>;

  const auto mp_complex_values_ctrl_strings =
    mp_complex_values_ctrl_strings_type
    {
      complex_strings_pair { std::string("7062652563059397884458909683e-01"), std::string("5964198535394629780309194467e-02") },
      complex_strings_pair { std::string("4048577319642457086365375268e+00"), std::string("5639809266248545375073278611e+00") },
      complex_strings_pair { std::string("2715964707859525004249772254e+03"), std::string("7657027474486443190971299319e+01") },
      complex_strings_pair { std::string("7855237294569533539406160330e+00"), std::string("8569705518774451157628892749e-01") },
      complex_strings_pair { std::string("7740602553207513524736354554e+00"), std::string("0883886635819581538293412637e-01") },
      complex_strings_pair { std::string("2084717042651553190640739533e+02"), std::string("3112485247365323186414254167e+01") },
    };

  const mp::mp_cpp seven_point_two = mp::mp_cpp(static_cast<unsigned>(UINT8_C(72))) / static_cast<unsigned>(UINT8_C(10));
  const mp::mp_cpp three_point_one = mp::mp_cpp(static_cast<unsigned>(UINT8_C(31))) / static_cast<unsigned>(UINT8_C(10));

  const mp::complex<mp::mp_cpp> z(seven_point_two, three_point_one);

  result_is_ok = true;

  // Execute the complex-valued test functions and measure the timing.
  const std::clock_t start = std::clock();

  const mp_complex_values_array_type
    mp_complex_values
    {
      mp::one() / z,                             // N[Re[1/((72/10) + ((31 I)/10))],     10002] (Use Im instead or Re to obtain the imaginary part).
      sin(z),                                    // N[Re[Sin[(72/10) + ((31 I)/10)],     10002] (Use Im instead or Re to obtain the imaginary part).
      exp(z),                                    // N[Re[Exp[(72/10) + ((31 I)/10)],     10002] (Use Im instead or Re to obtain the imaginary part).
      log(z),                                    // N[Re[Log[(72/10) + ((31 I)/10)],     10002] (Use Im instead or Re to obtain the imaginary part).
      acosh(z),                                  // N[Re[ArcCosh[(72/10) + ((31 I)/10)], 10002] (Use Im instead or Re to obtain the imaginary part).
      sinh(z)                                    // N[Re[Sinh[(72/10) + ((31 I)/10)],    10002] (Use Im instead or Re to obtain the imaginary part).
    };

  const auto elapsed_time = static_cast<float>(static_cast<float>(std::clock() - start) / CLOCKS_PER_SEC);

  result_is_ok = true;

  auto it_complex_str_ctrl = mp_complex_values_ctrl_strings.cbegin();

  for(const auto& complex_val : mp_complex_values)
  {
    std::string str_real_check { };
    std::string str_imag_check { };

    {
      std::stringstream strm;

      strm << std::scientific << std::setprecision(std::numeric_limits<mp::mp_cpp>::digits10) << complex_val.real();

      const std::string str_real_val = std::move(strm.str());

      str_real_check = std::string(str_real_val.cend() - str_len_to_check, str_real_val.cend());
    }

    {
      std::stringstream strm;

      strm << std::scientific << std::setprecision(std::numeric_limits<mp::mp_cpp>::digits10) << complex_val.imag();

      const std::string str_imag_val = std::move(strm.str());

      str_imag_check = std::string(str_imag_val.cend() - str_len_to_check, str_imag_val.cend());
    }

    const auto result_complex_val_is_ok =
    (
         (str_real_check == it_complex_str_ctrl->str_real)
      && (str_imag_check == it_complex_str_ctrl->str_imag)
    );

    ++it_complex_str_ctrl;

    result_is_ok = (result_complex_val_is_ok && result_is_ok);
  }

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
  constexpr auto my_digits10    = static_cast<std::int32_t>(INT16_C(10001));
  constexpr auto my_fft_threads = static_cast<int>(INT8_C(4));

  // Create the multiple precision base.
  const auto create_mp_base_is_ok = mp::mp_base::create_mp_base(my_digits10, my_fft_threads);

  auto result_is_ok = create_mp_base_is_ok;

  float elapsed_time_both { };
  float elapsed_time_real { };

  if(create_mp_base_is_ok)
  {
    {
      auto result_real_is_ok = false;

      // Test various real-valued functions.
      elapsed_time_real = local::test_real(result_real_is_ok);

      result_is_ok = (result_real_is_ok && result_is_ok);

      elapsed_time_both += elapsed_time_real;
    }

    float elapsed_time_imag { };

    {
      auto result_imag_is_ok = false;

      // Test various complex-valued functions.
      elapsed_time_imag = local::test_imag(result_imag_is_ok);

      result_is_ok = (result_imag_is_ok && result_is_ok);

      elapsed_time_both += result_imag_is_ok;
    }

    // Report the test timing.
    std::cout << "Elapsed time for test_real: " << elapsed_time_real << std::endl;
    std::cout << "Elapsed time for test_imag: " << elapsed_time_imag << std::endl;
    std::cout << "Elapsed time for both:      " << elapsed_time_both << std::endl;
  }

  return result_is_ok;
}
