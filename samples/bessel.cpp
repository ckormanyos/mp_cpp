///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : bessel.cpp
// 
// Project     : Samples for multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Bessel function lattice calculation using multiple
//               precision math library.
//
//               Algorithm description as implemented in the
//               subroutine Jn_lattice:
//
//               Multiple arrays of Bessel functions with orders 0...n
//               are computed using recursion methods. The array
//               elements are subsequently normalized and written
//               to various output files to form lattice data.
//               The characteristics of the lattice are defined in
//               the enumerated lattice declared in the subroutine
//               Jn_lattice.
//
//               1) Compute the starting order for recursion using
//                  the asymptotic approximation for large nu.
//                  The starting order m is chosen from the higher order
//                  obtained from the relations:
//                  |Jm(x)| < 10^(-p) or |Jm(x)| < |Jn(z)| * 10^(-p / 2)
//                  where p is the desired number of decimal digits of
//                  precision and n is the highest order of Bessel
//                  function to be computed.
//                  See "Computation of Special Functions", Zhang,
//                  Wiley 1996, ISBN 0-471-11963-6, Chapter 5,
//                  particularly Eq. 5.3.5 and the description thereof.
//               2) Chose two orders above the starting order
//                  (call them Jn_p2 and Jn_p1) and arbitrarily
//                  set Jn_p2 to 0 and Jn_p1 to 1.
//               3) Recur down to order zero, storing the Bessel functions
//                  as required in an array.
//               4) Normalize the Bessel functions in the array using
//                  the Neumann expansion:
//                  1 = J_0(x) + 2 * J_2(x) + 2 * J_4(x) + 2 * J_6(x) + ...
//                  See Abramowitz and Stegun 9.1.46 (also 9.1.87).
//               5) Write array data to output files.
// 
// *****************************************************************************

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <mp/mp_math.h>
#include <samples/samples.h>

/// ---------------------------------------------------------------------------
/// \func     std::uint32_t bisect(const double (*const F)(const double&),
///                                const double& x_lo,
///                                const double& x_hi) const
///
/// \descr    Bisection method as described in Numerical Recipes in C++,
///           chapter 9.1. The program on page 358 was taken directly from
///           the book and slightly modified to improve adherence with
///           standard C++ coding practices. The subroutine is specifically
///           coded for use with the call to fn_mstart, thus it is not a
///           bisection implementation.
///
/// ---------------------------------------------------------------------------
template<typename floating_point_type>
static floating_point_type bisect(floating_point_type (*const func)(const floating_point_type),
                                  const floating_point_type x_lo,
                                  const floating_point_type x_hi)
{
  const floating_point_type f_lo = func(x_lo);
  const floating_point_type f_hi = func(x_hi);

  const bool f_lo_is_neg = (f_lo < floating_point_type(0.0L));
  const bool f_hi_is_neg = (f_hi < floating_point_type(0.0L));

  // Make sure that there is at least one root in the interval.
  if(f_lo_is_neg == f_hi_is_neg)
  {
    std::cerr << "Error: Bisection interval has no root or it has multiple roots!" << std::endl;

    return 0;
  }

  // Orient the search such that f > 0 lies at x + dx.
  floating_point_type dx;
  floating_point_type rtb;

  if(f_lo_is_neg)
  {
    dx  = x_hi - x_lo;
    rtb = x_lo;
  }
  else
  {
    dx  = x_lo - x_hi;
    rtb = x_hi;
  }

  // Bisection iteration loop, maximum 64 times.
  for(std::uint_fast8_t i = 0U; i < 64U; ++i)
  {
    dx /= 2;

    const floating_point_type xmid = rtb + dx;
    const floating_point_type fmid = func(xmid);

    if(fmid <= floating_point_type(0.0L))
    {
      rtb = xmid;
    }

    // Test for convergence.
    using std::fabs;

    if(   (fabs(dx)   < floating_point_type(0.5L))
       || (fabs(fmid) < std::numeric_limits<floating_point_type>::epsilon()))
    {
      // Return root.
      return rtb;
    }
  }

  // Bisection iteration did not converge.
  std::cerr << "Error: Bisection iteration did not converge!" << std::endl;

  return floating_point_type(0.0L);
}

// *****************************************************************************
// Function    : struct Jn_algo
// 
// Description : A utility structure with subroutines used primarily
//               for estimating the proper starting order for recursive
//               calculations of Bessel functions (Jn).
// 
// *****************************************************************************
struct Jn_algo
{
  static const double e_half;
  static const double two_pi;

  static double m_z;
  static double m_n;
  static std::int32_t  m_p;

  /// ---------------------------------------------------------------------------
  /// \func     double fn_mstart1(const double& m)
  ///
  /// \descr    This equation corresponds to | Jm(z) | = 10^(-p) for
  ///           the large-m asymptotic behavior of Jm. It should be
  ///           solved using some sort of interval method in order
  ///           to find the proper starting order for recursive Bessel
  ///           function calculations requiring p digits of precision.
  ///
  /// ---------------------------------------------------------------------------
  static double fn_mstart1(const double m)
  {
    //                          1             e * z
    // Jm asymptotic --> ----------------  * [-----]^m = 10^-p
    //                   sqrt(2 * pi * m)     2 * m
    //
    // The resulting root equation is:
    // p + log10(Jnu asymptotic) = 0
    //
    return   static_cast<double>(m_p)
           - (0.5 * std::log10(two_pi * m))
           + (  m * std::log10(e_half * m_z / m));
  }

  /// ---------------------------------------------------------------------------
  /// \func     static double fn_mstart2(const double& m)
  ///
  /// \descr    This equation corresponds to | Jm(z) | = 10^(-p / 2) * | Jn(z)|
  ///           for the large-m, large-n asymptotic behavior of Jm, Jn. It should
  ///           be solved using some sort of interval method in order to find the
  ///           proper starting order for recursive Bessel function calculations
  ///           requiring p digits of precision.
  ///
  /// ---------------------------------------------------------------------------
  static double fn_mstart2(const double m)
  {
    //                          1             e * z                         1             e * z
    // Jm asymptotic --> ----------------  * [-----]^m = 10^(-p/2) * ----------------  * [-----]^n
    //                   sqrt(2 * pi * m)     2 * m                  sqrt(2 * pi * n)     2 * n
    //
    // The resulting root equation is:
    // p + log10(Jnu asymptotic) = 0
    //
    return   (static_cast<double>(m_p) / 2.0)
           - (0.5 * std::log10(two_pi * m))
           + (  m * std::log10(e_half * m_z / m))
           + (0.5 * std::log10(two_pi * m_n))
           - (m_n * std::log10(e_half * m_z / m_n));
  }

  /// ---------------------------------------------------------------------------
  /// \func     static std::uint32_t mstart1(const double& x, const std::uint32_t digits)
  ///
  /// \descr    Returns the starting order for Bessel function recursive
  ///           calculations of argument x requiring the given digits of
  ///           precision.
  ///
  /// ---------------------------------------------------------------------------
  static std::uint32_t mstart1(const double x, const std::uint32_t digits)
  {
    m_z = ((x < 0.1) ? 0.1 : x);
    m_p = digits;

    // Get the starting order for recursion.
    const double        d_order = bisect(fn_mstart1, 0.1, x + 10000.0);
    const std::uint32_t n_order = static_cast<std::uint32_t>(d_order);

    // Make sure the return value is an odd integer.
    return ((((n_order % 2) == 0U) && (n_order > 0U)) ? n_order - 1U : n_order);
  }

  /// ---------------------------------------------------------------------------
  /// \func     static std::uint32_t mstart2(const double& x, const double& dn, const std::uint32_t digits)
  ///
  /// \descr    Returns the starting order for Bessel function recursive
  ///           calculations of argument x requiring the given digits of
  ///           precision for each recursive Bessel function in the
  ///           downward recursion sequence starting with n.
  ///
  /// ---------------------------------------------------------------------------
  static std::uint32_t mstart2(const double x, const double dn, const std::uint32_t digits)
  {
    m_z = ((x < 0.1) ? 0.1 : x);
    m_n = dn;
    m_p = digits;

    // Get the starting order for recursion.
    const double        d_order = bisect(fn_mstart2, 0.1, x + 10000.0);
    const std::uint32_t n_order = static_cast<std::uint32_t>(d_order);

    // Make sure the return value is an odd integer.
    return ((((n_order % 2U) == 0U) && (n_order > 0U)) ? n_order - 1U : n_order);
  }
};

const double Jn_algo::e_half = 1.359140914229522617680143735676331248879;
const double Jn_algo::two_pi = 6.283185307179586476925286766559005768394;

double Jn_algo::m_z = 0.0;
double Jn_algo::m_n = 0.0;
std::int32_t  Jn_algo::m_p = 0;

// *****************************************************************************
// Function    : static bool DoJn(const int argc, const char* argv[])
// 
// Description : Compute a lattice of Bessel functions Jn(x)
//               of integer order and real argument using multiple
//               precision floating point numbers.
//               The results are stored in an array of files.
//
// *****************************************************************************
static bool DoJn(const int argc, const char* argv[])
{
  static_cast<void>(argc);

  // Analyze argv[0] to establish the output file path.
  std::string str_of = std::string(argv[0]);

  std::string::size_type p;
  if(   ((p = str_of.rfind('\\')) != std::string::npos)
     || ((p = str_of.rfind('/' )) != std::string::npos))
  {
    str_of = str_of.substr(0U, p + 1U);
  }

  // Characteristics of the lattice
  enum enumLattice
  {
    Jn_orders = 101, // Compute array for orders 0...Jn_orders - 1.
    Jn_range  = 100, // The range of the lattice is from 0 <= x <= Jn_range.
    Jn_grain  =  10  // The lattice granularity in points per mp_elem_digits10 x.
  };

  // Create lattice objects
  std::vector<mp::mp_cpp>     jn_array(static_cast<std::vector<mp::mp_cpp>::size_type>(Jn_orders)); // Vector of multiple precision floats
  std::vector<std::ofstream*> outfiles(static_cast<std::vector<mp::mp_cpp>::size_type>(Jn_orders)); // Vector of output file pointers for the data output

  // Loop over all lattice orders and initialize the lattice objects.
  for(std::int32_t n = 0; n < Jn_orders; ++n)
  {
    jn_array[n] = mp::mp_cpp(0U);

    // Create the output filename and open the associated file.
    // The filename has the form str_Jn = "J00n.dat" where the
    // numerical part of the filename always has three digits.
    std::stringstream  ss;
    std::string str_Jn;

    ss << "J"
       << std::setw(3) << std::setfill('0') << n
       << ".dat"
       << '\n';

    ss >> str_Jn;

    outfiles[n] = new std::ofstream(std::string(str_of + str_Jn).data(), std::ios_base::out | std::ios_base::trunc);

    if(outfiles[n]->is_open() == false)
    {
      for(n = 0; n < static_cast<std::int32_t>(outfiles.size()); ++n)
      {
        outfiles[n]->close();

        delete outfiles[n];
      }
      
      std::cerr << "Could not open all output files for Jn lattice" << std::endl;
      return false;
    }
  }

  // Perform the lattice calculations
  std::int32_t nx;

  for(nx = 0; nx <= Jn_range * Jn_grain; ++nx)
  {
    std::stringstream ss;
    std::string str;

    std::cout << "Calculating Jn lattice point: "
                << std::setprecision(1)
                << std::fixed
                << std::showpoint
                << static_cast<double>(nx) / static_cast<double>(static_cast<int>(Jn_grain))
                << std::endl;

    mp::mp_cpp x;

    std::int32_t m;
    
    if(nx == 0)
    {
      for(m = 0; m < Jn_orders; ++m)
      {
        // Print Jn(0.0) to output files
        *outfiles[m] << std::showpos << std::scientific << std::setprecision(6)
                     << double(0.0)
                     << "\t"
                     << std::setprecision(std::numeric_limits<mp::mp_cpp>::digits10)
                     << (m == 0 ? mp::one() : mp::zero())
                     << std::endl;
      }
    }
    else
    {
      // Downward recursion and simultaneous summation of
      // the normalization factor.
      x = mp::mp_cpp(nx) / static_cast<int>(Jn_grain);

      const mp::mp_cpp one_over_x = mp::mp_cpp(1U) / x;

      // Start recursion using two J's, Jn+2 and Jn+1.
      // Arbitrarily set Jn+2 = 0 and Jn+1 = 1.
      mp::mp_cpp Jn_p2        (0U);
      mp::mp_cpp Jn_p1        (1U);
      mp::mp_cpp normalization(2U);

      const std::uint32_t d10 = static_cast<std::uint32_t>(std::numeric_limits<mp::mp_cpp>::digits10);

      // Get the starting order for recursive calculations.
      const std::int32_t n_start1 = Jn_algo::mstart1(static_cast<double>(nx) / static_cast<double>(static_cast<std::int32_t>(Jn_grain)), d10);
      const std::int32_t n_start2 = Jn_algo::mstart2(static_cast<double>(nx) / static_cast<double>(static_cast<std::int32_t>(Jn_grain)), static_cast<std::int32_t>(Jn_orders) - 1, d10);

      const std::int32_t n_start = ((n_start2 > n_start1) ? n_start2 : n_start1);

      // Do the recursion. The direction of the recursion is downward.
      for(m = n_start; m >= 0; --m)
      {
        //                                 Jn+1(x)
        // Downward recursion is:  Jn(x) = ------- * [2 * (m + 1)] - Jn+2(x)
        //                                    x 
        const mp::mp_cpp Jn = ((Jn_p1 * one_over_x) * (2 * (m + 1))) - Jn_p2;

        Jn_p2 = Jn_p1;
        Jn_p1 = Jn;

        // Normalization.
        // Here we are using the Neumann expansion which is:
        // 1 = J_0(x) + 2 * J_2(x) + 2 * J_4(x) + 2 * J_6(x) + ...
        if((m % 2) == 0)
        {
          normalization += ((m == 0) ? Jn : Jn * 2);
        }

        // Store in the array.
        if(m < Jn_orders)
        {
          jn_array[m] = Jn;
        }
      }

      // Invert the normalization in order to use multiplication.
      normalization = mp::one() / normalization;

      // Normalize the Jn array and print to file.
      for(m = 0; m < Jn_orders; ++m)
      {
        jn_array[m] *= normalization;

        // Print Jn(x) to output files.
        *outfiles[m] << std::showpos << std::scientific << std::setprecision(6)
                     << x
                     << "\t"
                     << std::setprecision(std::numeric_limits<mp::mp_cpp>::digits10)
                     << jn_array[m]
                     << std::endl;
      }
    }
  }

  // Close all of the output files and delete them.
  for(std::size_t i = 0U; i < outfiles.size(); ++i)
  {
    outfiles[i]->close();

    delete outfiles.at(i);
  }

  std::cout << std::endl;

  return true;
}

bool samples::bessel(const int argc, const char* argv[])
{
  bool b_ok;

  // Set 200 decimal digits of precision.
  if(mp::mp_base::create_mp_base(200))
  {
    b_ok = DoJn(argc, argv);
  }
  else
  {
    b_ok = false;
  }

  return b_ok;
}
