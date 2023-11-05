///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : boost_main.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 2015 - 2023
// 
// Description : Test Boost.Multiprecision backend wrapper for multiple precision math.
// 
// *****************************************************************************

#include <iomanip>
#include <iostream>

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include <boost/multiprecision/mp_cpp_backend.hpp>

typedef boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<100>,
                                      boost::multiprecision::et_off>
mp_type;

int main()
{
  const mp_type y(char(1));
  const mp_type x(3.0L);
  const mp_type z = y / x;

  const mp_type lg_max = log((std::numeric_limits<mp_type>::max)());
  const mp_type eps    = std::numeric_limits<mp_type>::epsilon();

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << boost::math::constants::pi<mp_type>() << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << mp_type(0.5F)                         << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << z                                     << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << sin  (z)                              << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << floor(mp_type(1.5F))                  << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << ceil (mp_type(1.5F))                  << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << sqrt (mp_type(2))                     << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << boost::math::cbrt(mp_type(2))         << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << exp  (z)                              << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << atan2(y, x)                           << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << atan (z)                              << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << log  (mp_type(2))                     << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << lg_max                                << std::endl;

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << eps                    << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << sqrt(eps)              << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << boost::math::cbrt(eps) << std::endl;

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << boost::math::tgamma(mp_type(0.5F))          << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << sqrt(boost::math::constants::pi<mp_type>()) << std::endl;

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << boost::math::cyl_bessel_j(mp_type(1) / 7, mp_type(2.5F))  << std::endl;

  int nexp;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << frexp(mp_type(2), &nexp) << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << ldexp(mp_type(1), 2)     << std::endl;

  std::cout << std::boolalpha << (x < y) << std::endl;
  std::cout << std::boolalpha << (y > x) << std::endl;

  std::cout << std::boolalpha << (lg_max > (std::numeric_limits<std::int64_t>::max)()) << std::endl;
  std::cout << std::boolalpha << (lg_max > (std::numeric_limits<std::int64_t>::min)()) << std::endl;

  const mp_type a3(("33." + std::string(std::size_t(std::numeric_limits<mp_type>::digits10 - 2), '3') + std::string(16U, '4')).c_str());
  const mp_type b3(mp_type(100U) / 3);

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << a3 << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << b3 << std::endl;

  std::cout << std::boolalpha << (a3 == b3) << std::endl;
  std::cout << std::boolalpha << (a3 >  b3) << std::endl;
  std::cout << std::boolalpha << (a3 <  b3) << std::endl;

  const mp_type c3(("33." + std::string(120U, '3')).c_str());
  const mp_type d3(mp_type(100U) / 3);

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << c3 << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << d3 << std::endl;

  std::cout << std::boolalpha << (c3 == d3) << std::endl;
  std::cout << std::boolalpha << (c3 >  d3) << std::endl;
  std::cout << std::boolalpha << (c3 <  d3) << std::endl;
}
