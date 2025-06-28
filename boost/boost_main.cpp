///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2025.
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
// Date        : 2015 - 2025
// 
// Description : Test Boost.Multiprecision backend wrapper for multiple precision math.
// 
// *****************************************************************************

#if !defined(BOOST_MATH_STANDALONE)
#define BOOST_MATH_STANDALONE
#endif

#include <boost/multiprecision/mp_cpp_backend.hpp>

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/math/special_functions/gamma.hpp>

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>

namespace local {

template<typename NumericType>
auto is_close_fraction(const NumericType& a,
                       const NumericType& b,
                       const NumericType& tol = ::std::numeric_limits<NumericType>::epsilon() * 100) -> bool
{
  using numeric_type = NumericType;

  numeric_type closeness { };

  using ::std::fabs;

  if(b == numeric_type(0))
  {
    closeness = fabs(a - b);
  }
  else
  {
    const numeric_type delta = (a / b);

    closeness = fabs(1 - delta);
  }

  const bool result_is_ok = (closeness < tol);

  return result_is_ok;
}

using mp_type = boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<100>, boost::multiprecision::et_off>;

} // namespace local

int main()
{
  using local::mp_type;

  const mp_type y(char(1));
  const mp_type x(3.0L);
  const mp_type z = y / x;

  const mp_type lg_max = log((std::numeric_limits<mp_type>::max)());
  const mp_type eps    = std::numeric_limits<mp_type>::epsilon();

  const mp_type result_sin_z        { sin(z) };
  const mp_type result_sqrt_two     { sqrt(mp_type(2)) };
  const mp_type result_cbrt_two     { boost::math::cbrt(mp_type(2)) };
  const mp_type result_exp_z        { exp(z) };
  const mp_type result_atan_z       { atan(z) };
  const mp_type result_log_two      { log(mp_type(2)) };
  const mp_type result_tgamma_half  { boost::math::tgamma(mp_type(0.5F)) };
  const mp_type result_sqrt_pi      { sqrt(boost::math::constants::pi<mp_type>()) };

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << boost::math::constants::pi<mp_type>() << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << mp_type(0.5F)                         << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << z                                     << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_sin_z                          << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << floor(mp_type(1.5F))                  << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << ceil (mp_type(1.5F))                  << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_sqrt_two                       << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_cbrt_two                       << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_exp_z                          << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_atan_z                         << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_log_two                        << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << lg_max                                << std::endl;

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << eps                    << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << sqrt(eps)              << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << boost::math::cbrt(eps) << std::endl;

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_tgamma_half     << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_sqrt_pi         << std::endl;

  bool result_is_ok { true };

  {
    // N[BesselJ[1/7, 25/10], 100]
    const std::string str_bessel_ctrl { "0.06180131488538539958704981305517437529130987774612456203864079593497766843694902194335563990646502023" };
    const mp_type result_bessel = boost::math::cyl_bessel_j(mp_type(1) / 7, mp_type(2.5F));
    std::cout << "\nBessel check\n" << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_bessel  << std::endl;
    std::cout << "Expected:\n" << str_bessel_ctrl << '\n' << std::endl;

    const bool result_bessel_is_ok = local::is_close_fraction(result_bessel, mp_type(str_bessel_ctrl));

    result_is_ok = (result_bessel_is_ok && result_is_ok);
  }

  {
    // N[BesselJ[11/7, 123/10], 100]
    const std::string str_bessel_ctrl { "-0.2188976146993564413179947235166800405355277034535824651310860062540136035942626397519480169927808891" };
    const mp_type result_bessel = boost::math::cyl_bessel_j(mp_type(11) / 7, mp_type(123) / 10);
    std::cout << "\nBessel check\n" << std::setprecision(std::numeric_limits<mp_type>::digits10) << result_bessel  << std::endl;
    std::cout << "Expected:\n" << str_bessel_ctrl << '\n' << std::endl;

    const bool result_bessel_is_ok = local::is_close_fraction(result_bessel, mp_type(str_bessel_ctrl));

    result_is_ok = (result_bessel_is_ok && result_is_ok);
  }

  int nexp;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << frexp(mp_type(2), &nexp) << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10) << ldexp(mp_type(1), 2)     << std::endl;

  std::cout << std::boolalpha << (x < y) << ", Expect: false" << std::endl; // Expect false
  std::cout << std::boolalpha << (y > x) << ", Expect: false" << std::endl; // Expect false

  std::cout << std::boolalpha << (lg_max > (std::numeric_limits<std::int64_t>::max)()) <<  ", Expect: false" << std::endl; // Expect false
  std::cout << std::boolalpha << (lg_max > (std::numeric_limits<std::int64_t>::min)()) << " , Expect: true"  << std::endl; // Expect true

  const mp_type a3(("33." + std::string(std::size_t(std::numeric_limits<mp_type>::digits10 - 2), '3') + std::string(16U, '4')).c_str());
  const mp_type b3(mp_type(100U) / 3);

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << a3 << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << b3 << std::endl;

  std::cout << std::boolalpha << (a3 == b3) <<  ", Expect: false" << std::endl; // Expect false
  std::cout << std::boolalpha << (a3 >  b3) << " , Expect: true" << std::endl; // Expect true
  std::cout << std::boolalpha << (a3 <  b3) <<  ", Expect: false" << std::endl; // Expect false

  const mp_type c3(("33." + std::string(120U, '3')).c_str());
  const mp_type d3(mp_type(100U) / 3);

  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << c3 << std::endl;
  std::cout << std::setprecision(std::numeric_limits<mp_type>::digits10 + 4) << d3 << std::endl;

  std::cout << std::boolalpha << (c3 == d3) <<  ", Expect: false" << std::endl;
  std::cout << std::boolalpha << (c3 >  d3) <<  ", Expect: false" << std::endl;
  std::cout << std::boolalpha << (c3 <  d3) << " , Expect: true"  << std::endl;

  return (result_is_ok ? 0 : -1);
}
