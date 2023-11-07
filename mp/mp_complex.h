///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_complex.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Implementation of mp::complex<mp_cpp> for multiple precision math.
// 
// *****************************************************************************

// Implement templates for an extended complex class and
// its associated functions. These are intended to be used
// with both user-defined types as well as built-in float,
// double and long double.

#ifndef MP_COMPLEX_2002_10_09_H
  #define MP_COMPLEX_2002_10_09_H

  #if defined(__GNUC__)
    #pragma GCC system_header
  #endif

  #include <cmath>

  #if !defined(EXTENDED_COMPLEX_DISABLE_IOSTREAM)
  #include <algorithm>
  #include <array>
  #include <istream>
  #include <ostream>
  #include <regex>
  #include <sstream>
  #include <string>
  #endif

  #if defined(_MSC_VER) && (_MSC_VER <= 1800)
    #define EXTENDED_COMPLEX_CONSTEXPR
  #else
    #define EXTENDED_COMPLEX_CONSTEXPR constexpr
  #endif

  #include <mp/mp_cpp.h>
  #include <mp/mp_detail.h>
  #include <mp/mp_detail_pown_template.h>

  // Make a separate complex class for mp::complex<mp::mp_cpp>.

  // Even though there exists a generic std::complex<T>,
  // the C++ specification paragraph 26.2/2 indicates:
  // "The effect of instantiating the template complex
  // for any type other than float, double or long double
  // is unspecified".

  // A separate template class and corresponding template
  // specializations are, therefore, needed for mp::mp_cpp.

  // Also include template specializations for both built-in
  // float, double and long double as well as mp::mp_cpp.

  // The result of this file is:
  //   mp::complex<float>
  //   mp::complex<double>
  //   mp::complex<long double>
  //   mp::complex<mp::mp_cpp>,

  // ... and these are intended to behave like std::complex<T>.

  namespace mp
  {
    // Forward declarations.

    // Class template extended_complex::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.2.

    template<typename T>
    class complex;

    // Class template specializations of extended_complex::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.3.

    template<>
    class complex<float>;

    template<>
    class complex<double>;

    template<>
    class complex<long double>;

    // Non-member operations for extended_complex::complex<T>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.6.

    // Unary +/- operators.
    template<typename T> complex<T> operator+(const complex<T>&);
    template<typename T> complex<T> operator-(const complex<T>&);

    // Global add, sub, mul, div operators.
    template<typename T> complex<T> operator+(const complex<T>&, const complex<T>&);
    template<typename T> complex<T> operator-(const complex<T>&, const complex<T>&);
    template<typename T> complex<T> operator*(const complex<T>&, const complex<T>&);
    template<typename T> complex<T> operator/(const complex<T>&, const complex<T>&);

    template<typename T> complex<T> operator+(const complex<T>&, const T&);
    template<typename T> complex<T> operator-(const complex<T>&, const T&);
    template<typename T> complex<T> operator*(const complex<T>&, const T&);
    template<typename T> complex<T> operator/(const complex<T>&, const T&);

    template<typename T> complex<T> operator+(const T&, const complex<T>&);
    template<typename T> complex<T> operator-(const T&, const complex<T>&);
    template<typename T> complex<T> operator*(const T&, const complex<T>&);
    template<typename T> complex<T> operator/(const T&, const complex<T>&);

    // Equality and inequality operators.
    template<typename T> bool operator==(const complex<T>&, const complex<T>&);
    template<typename T> bool operator==(const complex<T>&, const T&);
    template<typename T> bool operator==(const T&,          const complex<T>&);

    template<typename T> bool operator!=(const complex<T>&, const complex<T>&);
    template<typename T> bool operator!=(const complex<T>&, const T&);
    template<typename T> bool operator!=(const T&,          const complex<T>&);

    #if !defined(EXTENDED_COMPLEX_DISABLE_IOSTREAM)

    // I/O stream operators.
    template<typename T, typename char_type, typename traits_type>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>&, complex<T>&);

    template<typename T, typename char_type, typename traits_type>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>&, const complex<T>&);

    #endif // !EXTENDED_COMPLEX_DISABLE_IOSTREAM

    // Value operations for extended_complex::complex<T>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.7.

    template<typename T> T          real (const complex<T>&);
    template<typename T> T          imag (const complex<T>&);

    template<typename T> T          abs  (const complex<T>&);
    template<typename T> T          arg  (const complex<T>&);
    template<typename T> T          norm (const complex<T>&);

    template<typename T> complex<T> conj (const complex<T>&);
    template<typename T> complex<T> proj (const complex<T>&);
    template<typename T> complex<T> polar(const T&, const T& = T(0U));

    // Elementary transcendental functions for extended_complex::complex<T>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.8.

    template<typename T> complex<T> acos (const complex<T>&);
    template<typename T> complex<T> asin (const complex<T>&);
    template<typename T> complex<T> atan (const complex<T>&);
    template<typename T> complex<T> asinh(const complex<T>&);
    template<typename T> complex<T> acosh(const complex<T>&);
    template<typename T> complex<T> atanh(const complex<T>&);

    template<typename T> complex<T> cos  (const complex<T>&);
    template<typename T> complex<T> cosh (const complex<T>&);
    template<typename T> complex<T> exp  (const complex<T>&);
    template<typename T> complex<T> log  (const complex<T>&);
    template<typename T> complex<T> log10(const complex<T>&);

    template<typename T> complex<T> pow  (const complex<T>&, int);
    template<typename T> complex<T> pow  (const complex<T>&, const T&);
    template<typename T> complex<T> pow  (const complex<T>&, const complex<T>&);
    template<typename T> complex<T> pow  (const T&, const complex<T>&);

    template<typename T> complex<T> sin  (const complex<T>&);
    template<typename T> complex<T> sinh (const complex<T>&);
    template<typename T> complex<T> sqrt (const complex<T>&);
    template<typename T> complex<T> tan  (const complex<T>&);
    template<typename T> complex<T> tanh (const complex<T>&);

    // Class template extended_complex::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.2.

    template<typename T>
    class complex
    {
    public:
      typedef T value_type;

      complex(const value_type& my_x = value_type(),
              const value_type& my_y = value_type()) : my_re(my_x),
                                                       my_im(my_y) { }

      complex(const complex& my_z) : my_re(my_z.real()),
                                     my_im(my_z.imag()) { }

      template<typename X>
      complex(const complex<X>& my_z) : my_re(static_cast<value_type>(my_z.real())),
                                        my_im(static_cast<value_type>(my_z.imag())) { }

      value_type real() const { return my_re; }
      value_type imag() const { return my_im; }

      void real(value_type my_x) { my_re = my_x; }
      void imag(value_type my_y) { my_im = my_y; }

      complex& operator=(const value_type& my_other_x)
      {
        my_re = my_other_x;
        my_im = value_type(0U);

        return *this;
      }

      complex& operator+=(const value_type& my_x)
      {
        my_re += my_x;

        return *this;
      }

      complex& operator-=(const value_type& my_x)
      {
        my_re -= my_x;

        return *this;
      }

      complex& operator*=(const value_type& my_x)
      {
        my_re *= my_x;
        my_im *= my_x;

        return *this;
      }

      complex& operator/=(const value_type& my_x)
      {
        my_re /= my_x;
        my_im /= my_x;

        return *this;
      }

      complex& operator=(const complex& my_other_z)
      {
        if(this != &my_other_z)
        {
          my_re = my_other_z.my_re;
          my_im = my_other_z.my_im;
        }

        return *this;
      }

      template<typename X>
      complex& operator=(const complex<X>& my_other_z)
      {
        my_re = static_cast<value_type>(my_other_z.my_re);
        my_im = static_cast<value_type>(my_other_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& my_z)
      {
        my_re += static_cast<value_type>(my_z.my_re);
        my_im += static_cast<value_type>(my_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& my_z)
      {
        if(this == &my_z)
        {
          my_re = value_type(0);
          my_im = value_type(0);
        }
        else
        {
          my_re -= static_cast<value_type>(my_z.my_re);
          my_im -= static_cast<value_type>(my_z.my_im);
        }

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& my_z)
      {
        const value_type tmp_re(my_re);

        if(this == &my_z)
        {
          my_re = (tmp_re * tmp_re) - (my_im * my_im);
          my_im = (tmp_re * my_im) * 2U;
        }
        else
        {
          my_re = (tmp_re * my_z.my_re) - (my_im * my_z.my_im);
          my_im = (tmp_re * my_z.my_im) + (my_im * my_z.my_re);
        }

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& my_z)
      {
        if(this == &my_z)
        {
          my_re = value_type(1);
          my_im = value_type(0);
        }
        else
        {
          if(fabs(my_z.real()) < fabs(my_z.imag()))
          {
            const value_type my_c_over_d = my_z.real() / my_z.imag();

            const value_type my_denominator = (my_z.real() * my_c_over_d) + my_z.imag();

            const value_type my_tmp_re(my_re);

            my_re = ((my_tmp_re * my_c_over_d) + my_im)     / my_denominator;
            my_im = ((my_im     * my_c_over_d) - my_tmp_re) / my_denominator;
          }
          else
          {
            const value_type my_d_over_c = my_z.imag() / my_z.real();

            const value_type my_denominator = (my_z.imag() * my_d_over_c) + my_z.real();

            const value_type my_tmp_re(my_re);

            my_re = (( my_im     * my_d_over_c) + my_tmp_re) / my_denominator;
            my_im = ((-my_tmp_re * my_d_over_c) + my_im)     / my_denominator;
          }
        }

        return *this;
      }

    private:
      value_type my_re;
      value_type my_im;
    };


    // Specialization of class template extended_complex::complex<float>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.3.

    template<>
    class complex<float>
    {
    public:
      typedef float value_type;

      EXTENDED_COMPLEX_CONSTEXPR complex(float my_x = float(),
                                         float my_y = float()) : my_re(my_x),
                                                                 my_im(my_y) { }

      explicit EXTENDED_COMPLEX_CONSTEXPR complex(const complex<double>&);
      explicit EXTENDED_COMPLEX_CONSTEXPR complex(const complex<long double>&);

      EXTENDED_COMPLEX_CONSTEXPR float real() const { return my_re; }
      EXTENDED_COMPLEX_CONSTEXPR float imag() const { return my_im; }

      void real(float my_x) { my_re = my_x; }
      void imag(float my_y) { my_im = my_y; }

      complex& operator=(float my_other_x)
      {
        my_re = my_other_x;
        my_im = 0.0F;

        return *this;
      }

      complex& operator+=(float my_x)
      {
        my_re += my_x;

        return *this;
      }

      complex& operator-=(float my_x)
      {
        my_re -= my_x;

        return *this;
      }

      complex& operator*=(float my_x)
      {
        my_re *= my_x;
        my_im *= my_x;

        return *this;
      }

      complex& operator/=(float my_x)
      {
        my_re /= my_x;
        my_im /= my_x;

        return *this;
      }

      complex& operator=(const complex& my_other_z)
      {
        if(this != &my_other_z)
        {
          my_re = my_other_z.my_re;
          my_im = my_other_z.my_im;
        }

        return *this;
      }

      template<typename X>
      complex& operator=(const complex<X>& my_other_z)
      {
        my_re = static_cast<float>(my_other_z.my_re);
        my_im = static_cast<float>(my_other_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& my_z)
      {
        my_re += static_cast<float>(my_z.my_re);
        my_im += static_cast<float>(my_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& my_z)
      {
        my_re -= static_cast<float>(my_z.my_re);
        my_im -= static_cast<float>(my_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& my_z)
      {
        const float tmp_re(my_re);

        if(this == &my_z)
        {
          my_re = (tmp_re * tmp_re) - (my_im * my_im);
          my_im = (tmp_re * my_im) * 2.0F;
        }
        else
        {
          my_re = (tmp_re * my_z.my_re) - (my_im * my_z.my_im);
          my_im = (tmp_re * my_z.my_im) + (my_im * my_z.my_re);
        }

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& my_z)
      {
        if(this == &my_z)
        {
          my_re = 1.0F;
          my_im = 0.0F;
        }
        else
        {
          using std::fabs;

          if(fabs(my_z.real()) < fabs(my_z.imag()))
          {
            const float my_c_over_d = my_z.real() / my_z.imag();

            const float my_denominator = (my_z.real() * my_c_over_d) + my_z.imag();

            const float my_tmp_re(my_re);

            real(((my_tmp_re * my_c_over_d) + my_im)     / my_denominator);
            imag(((my_im     * my_c_over_d) - my_tmp_re) / my_denominator);
          }
          else
          {
            const float my_d_over_c = my_z.imag() / my_z.real();

            const float my_denominator = (my_z.imag() * my_d_over_c) + my_z.real();

            const float my_tmp_re(my_re);

            real((( my_im     * my_d_over_c) + my_tmp_re) / my_denominator);
            imag(((-my_tmp_re * my_d_over_c) + my_im)     / my_denominator);
          }
        }

        return *this;
      }

    private:
      float my_re;
      float my_im;
    };


    // Specialization of class template extended_complex::complex<double>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.3.

    template<>
    class complex<double>
    {
    public:
      typedef double value_type;

      EXTENDED_COMPLEX_CONSTEXPR complex(double my_x = double(),
                                         double my_y = double()) : my_re(my_x),
                                                                   my_im(my_y) { }

      EXTENDED_COMPLEX_CONSTEXPR complex(const complex<float>& my_f) : my_re(double(my_f.real())),
                                                                       my_im(double(my_f.imag())) { }

      explicit EXTENDED_COMPLEX_CONSTEXPR complex(const complex<long double>&);

      EXTENDED_COMPLEX_CONSTEXPR double real() const { return my_re; }
      EXTENDED_COMPLEX_CONSTEXPR double imag() const { return my_im; }

      void real(double my_x) { my_re = my_x; }
      void imag(double my_y) { my_im = my_y; }

      complex& operator=(double my_other_x)
      {
        my_re = my_other_x;
        my_im = 0.0;

        return *this;
      }

      complex& operator+=(double my_x)
      {
        my_re += my_x;

        return *this;
      }

      complex& operator-=(double my_x)
      {
        my_re -= my_x;

        return *this;
      }

      complex& operator*=(double my_x)
      {
        my_re *= my_x;
        my_im *= my_x;

        return *this;
      }

      complex& operator/=(double my_x)
      {
        my_re /= my_x;
        my_im /= my_x;

        return *this;
      }

      complex& operator=(const complex& my_other_z)
      {
        if(this != &my_other_z)
        {
          my_re = my_other_z.my_re;
          my_im = my_other_z.my_im;
        }

        return *this;
      }

      template<typename X>
      complex& operator=(const complex<X>& my_other_z)
      {
        my_re = static_cast<double>(my_other_z.my_re);
        my_im = static_cast<double>(my_other_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& my_z)
      {
        my_re += static_cast<double>(my_z.my_re);
        my_im += static_cast<double>(my_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& my_z)
      {
        my_re -= static_cast<double>(my_z.my_re);
        my_im -= static_cast<double>(my_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& my_z)
      {
        const double tmp_re(my_re);

        if(this == &my_z)
        {
          my_re = (tmp_re * tmp_re) - (my_im * my_im);
          my_im = (tmp_re * my_im) * 2.0;
        }
        else
        {
          my_re = (tmp_re * my_z.my_re) - (my_im * my_z.my_im);
          my_im = (tmp_re * my_z.my_im) + (my_im * my_z.my_re);
        }

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& my_z)
      {
        if(this == &my_z)
        {
          my_re = 1.0;
          my_im = 0.0;
        }
        else
        {
          using std::fabs;

          if(fabs(my_z.real()) < fabs(my_z.imag()))
          {
            const double my_c_over_d = my_z.real() / my_z.imag();

            const double my_denominator = (my_z.real() * my_c_over_d) + my_z.imag();

            const double my_tmp_re(my_re);

            real(((my_tmp_re * my_c_over_d) + my_im)     / my_denominator);
            imag(((my_im     * my_c_over_d) - my_tmp_re) / my_denominator);
          }
          else
          {
            const double my_d_over_c = my_z.imag() / my_z.real();

            const double my_denominator = (my_z.imag() * my_d_over_c) + my_z.real();

            const double my_tmp_re(my_re);

            real((( my_im     * my_d_over_c) + my_tmp_re) / my_denominator);
            imag(((-my_tmp_re * my_d_over_c) + my_im)     / my_denominator);
          }
        }

        return *this;
      }

    private:
      double my_re;
      double my_im;
    };

    // Specialization of class template extended_complex::complex<long double>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.3.

    template<>
    class complex<long double>
    {
    public:
      typedef long double value_type;

      EXTENDED_COMPLEX_CONSTEXPR complex(value_type my_x = (value_type()),
                                         value_type my_y = (value_type())) : my_re(my_x),
                                                                             my_im(my_y) { }

      EXTENDED_COMPLEX_CONSTEXPR complex(const complex<float>& my_f) : my_re(static_cast<long double>(my_f.real())),
                                                                       my_im(static_cast<long double>(my_f.imag())) { }

      EXTENDED_COMPLEX_CONSTEXPR complex(const complex<double>& my_d) : my_re(static_cast<long double>(my_d.real())),
                                                                        my_im(static_cast<long double>(my_d.imag())) { }

      EXTENDED_COMPLEX_CONSTEXPR long double real() const { return my_re; }
      EXTENDED_COMPLEX_CONSTEXPR long double imag() const { return my_im; }

      void real(long double my_x) { my_re = my_x; }
      void imag(long double my_y) { my_im = my_y; }

      complex& operator=(long double my_other_x)
      {
        my_re = my_other_x;
        my_im = 0.0L;

        return *this;
      }

      complex& operator+=(long double my_x)
      {
        my_re += my_x;

        return *this;
      }

      complex& operator-=(long double my_x)
      {
        my_re -= my_x;

        return *this;
      }

      complex& operator*=(long double my_x)
      {
        my_re *= my_x;
        my_im *= my_x;

        return *this;
      }

      complex& operator/=(long double my_x)
      {
        my_re /= my_x;
        my_im /= my_x;

        return *this;
      }

      complex& operator=(const complex& my_other_z)
      {
        if(this != &my_other_z)
        {
          my_re = my_other_z.my_re;
          my_im = my_other_z.my_im;
        }

        return *this;
      }

      template<typename X>
      complex& operator=(const complex<X>& my_other_z)
      {
        my_re = static_cast<long double>(my_other_z.my_re);
        my_im = static_cast<long double>(my_other_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& my_z)
      {
        my_re += static_cast<long double>(my_z.my_re);
        my_im += static_cast<long double>(my_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& my_z)
      {
        my_re -= static_cast<long double>(my_z.my_re);
        my_im -= static_cast<long double>(my_z.my_im);

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& my_z)
      {
        const long double tmp_re(my_re);

        if(this == &my_z)
        {
          my_re = (tmp_re * tmp_re) - (my_im * my_im);
          my_im = (tmp_re * my_im) * 2.0L;
        }
        else
        {
          my_re = (tmp_re * my_z.my_re) - (my_im * my_z.my_im);
          my_im = (tmp_re * my_z.my_im) + (my_im * my_z.my_re);
        }

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& my_z)
      {
        if(this == &my_z)
        {
          my_re = 1.0L;
          my_im = 0.0L;
        }
        else
        {
          using std::fabs;

          if(fabs(my_z.real()) < fabs(my_z.imag()))
          {
            const long double my_c_over_d = my_z.real() / my_z.imag();

            const long double my_denominator = (my_z.real() * my_c_over_d) + my_z.imag();

            const long double my_tmp_re(my_re);

            real(((my_tmp_re * my_c_over_d) + my_im)     / my_denominator);
            imag(((my_im     * my_c_over_d) - my_tmp_re) / my_denominator);
          }
          else
          {
            const long double my_d_over_c = my_z.imag() / my_z.real();

            const long double my_denominator = (my_z.imag() * my_d_over_c) + my_z.real();

            const long double my_tmp_re(my_re);

            real((( my_im     * my_d_over_c) + my_tmp_re) / my_denominator);
            imag(((-my_tmp_re * my_d_over_c) + my_im)     / my_denominator);
          }
        }

        return *this;
      }

    private:
      long double my_re;
      long double my_im;
    };

    // These constructors are located here because they need to be
    // implemented after the template specializations have been declared.

    EXTENDED_COMPLEX_CONSTEXPR complex<float >::complex(const complex<double>&      my_d) : my_re(float (my_d.real())), my_im(float (my_d.imag())) { }
    EXTENDED_COMPLEX_CONSTEXPR complex<float >::complex(const complex<long double>& my_l) : my_re(float (my_l.real())), my_im(float (my_l.imag())) { }
    EXTENDED_COMPLEX_CONSTEXPR complex<double>::complex(const complex<long double>& my_l) : my_re(double(my_l.real())), my_im(double(my_l.imag())) { }

    // Non-member operations for extended_complex::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.6.

    // Unary +/- operators.
    template<typename T> complex<T> operator+(const complex<T>& my_u) { return my_u; }
    template<typename T> complex<T> operator-(const complex<T>& my_u) { return complex<T>(-my_u.real(), -my_u.imag()); }

    // Global add, sub, mul, div operators.
    template<typename T> complex<T> operator+(const complex<T>& my_u, const complex<T>& my_v) { return complex<T>(my_u.real() + my_v.real(), my_u.imag() + my_v.imag()); }
    template<typename T> complex<T> operator-(const complex<T>& my_u, const complex<T>& my_v) { return complex<T>(my_u.real() - my_v.real(), my_u.imag() - my_v.imag()); }

    template<typename T> complex<T> operator*(const complex<T>& my_u, const complex<T>& my_v)
    {
      return complex<T>((my_u.real() * my_v.real()) - (my_u.imag() * my_v.imag()),
                        (my_u.real() * my_v.imag()) + (my_u.imag() * my_v.real()));
    }

    template<typename T> complex<T> operator/(const complex<T>& my_u, const complex<T>& my_v)
    {
      using std::fabs;

      complex<T> my_result;

      if(fabs(my_v.real()) < fabs(my_v.imag()))
      {
        const T my_c_over_d = my_v.real() / my_v.imag();

        const T my_denominator = (my_v.real() * my_c_over_d) + my_v.imag();

        my_result = complex<T>(((my_u.real() * my_c_over_d) + my_u.imag()) / my_denominator,
                               ((my_u.imag() * my_c_over_d) - my_u.real()) / my_denominator);
      }
      else
      {
        const T my_d_over_c = my_v.imag() / my_v.real();

        const T my_denominator = (my_v.imag() * my_d_over_c) + my_v.real();

        my_result = complex<T>((( my_u.imag() * my_d_over_c) + my_u.real()) / my_denominator,
                               ((-my_u.real() * my_d_over_c) + my_u.imag()) / my_denominator);
      }

      return my_result;
    }

    template<typename T> complex<T> operator+(const complex<T>& my_u, const T& my_v) { return complex<T>(my_u.real() + my_v, my_u.imag()); }
    template<typename T> complex<T> operator-(const complex<T>& my_u, const T& my_v) { return complex<T>(my_u.real() - my_v, my_u.imag()); }
    template<typename T> complex<T> operator*(const complex<T>& my_u, const T& my_v) { return complex<T>(my_u.real() * my_v, my_u.imag() * my_v); }
    template<typename T> complex<T> operator/(const complex<T>& my_u, const T& my_v) { return complex<T>(my_u.real() / my_v, my_u.imag() / my_v); }

    template<typename T> complex<T> operator+(const T& my_u, const complex<T>& my_v) { return complex<T>(my_u + my_v.real(), my_v.imag()); }
    template<typename T> complex<T> operator-(const T& my_u, const complex<T>& my_v) { return complex<T>(my_u - my_v.real(), -my_v.imag()); }
    template<typename T> complex<T> operator*(const T& my_u, const complex<T>& my_v) { return complex<T>(my_u * my_v.real(), my_u * my_v.imag()); }

    template<typename T> complex<T> operator/(const T& my_u, const complex<T>& my_v)
    {
      using std::fabs;

      complex<T> my_result;

      if(fabs(my_v.real()) < fabs(my_v.imag()))
      {
        const T my_c_over_d = my_v.real() / my_v.imag();

        const T my_denominator = (my_v.real() * my_c_over_d) + my_v.imag();

        my_result = complex<T>(( my_u * my_c_over_d) / my_denominator,
                                -my_u                  / my_denominator);
      }
      else
      {
        const T my_d_over_c = my_v.imag() / my_v.real();

        const T my_denominator = (my_v.imag() * my_d_over_c) + my_v.real();

        my_result = complex<T>(  my_u                  / my_denominator,
                               (-my_u * my_d_over_c) / my_denominator);
      }

      return my_result;
    }

    // Equality and inequality operators.
    template<typename T> bool operator==(const complex<T>& my_u, const complex<T>& my_v) { return ((my_u.real() == my_v.real()) && (my_u.imag() == my_v.imag())); }
    template<typename T> bool operator==(const complex<T>& my_u, const T&          my_v) { return ((my_u.real() == my_v)        && (my_u.imag() == T(0))); }
    template<typename T> bool operator==(const T&          my_u, const complex<T>& my_v) { return ((my_u == my_v.real())        && (my_v.imag() == T(0))); }

    template<typename T> bool operator!=(const complex<T>& my_u, const complex<T>& my_v) { return ((my_u.real() != my_v.real()) || (my_u.imag() != my_v.imag())); }
    template<typename T> bool operator!=(const complex<T>& my_u, const T&          my_v) { return ((my_u.real() != my_v)        || (my_u.imag() != T(0))); }
    template<typename T> bool operator!=(const T&          my_u, const complex<T>& my_v) { return ((my_u != my_v.real())        || (my_v.imag() != T(0))); }

    #if !defined(EXTENDED_COMPLEX_DISABLE_IOSTREAM)

    // I/O stream operators.
    template<typename T, typename char_type, typename traits_type>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& my_istream, complex<T>& my_z)
    {
      // Parse an (extended) complex number of any of the forms u, (u) or (u,v).

      const std::array<std::regex, 3U> my_regexes =
      {{
        // A regular expression for an (extended) complex number of the form (u,v).
        std::regex(  std::string("\\({1}")                             // One open parentheses.
                   + std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                   + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)")     // Possible exponent field.
                   + std::string("\\,{1}")                             // One comma character.
                   + std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                   + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)")     // Possible exponent field.
                   + std::string("\\){1}")),                           // One close parentheses.

        // A regular expression for an (extended) complex number of the form (u).
        std::regex(  std::string("\\({1}")                             // One open parentheses.
                   + std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                   + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)")     // Possible exponent field.
                   + std::string("\\){1}")),                           // One close parentheses.

        // A regular expression for an (extended) complex number of the form u.
        std::regex(  std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                   + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)"))    // Possible exponent field.
      }};

      T my_real_input(0U);
      T my_imag_input(0U);

      bool my_input_has_error = false;

      std::string my_input_str;

      my_istream >> my_input_str;

      std::match_results<std::string::const_iterator> my_mr;

      auto my_it_regex_match =
        std::find_if(my_regexes.cbegin(),
                     my_regexes.cend(),
                     [&my_input_str, &my_mr](const std::regex& my_rx) -> bool
                     {
                       return std::regex_match(my_input_str, my_mr, my_rx);
                     });

      if(my_it_regex_match == my_regexes.cend())
      {
        // The input does not match any of the regular expressions.
        // Set the error flag and take no other action.

        my_input_has_error = true;
      }
      else if(my_it_regex_match == my_regexes.cbegin())
      {
        // The input matches my_regexes[0U], corresponding to the form (u,v).
        // This represents a complex number real and imaginary parts.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string my_str1(my_mr[1U]);
        const std::string my_str2(my_mr[2U]);
        const std::string my_str3(my_mr[3U]);
        const std::string my_str4(my_mr[4U]);

        const std::string my_decimal_digits("0123456789");

        const bool my_str1_has_error = ((my_str1.length() != std::size_t(0U)) && (my_str1.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str2_has_error = ((my_str2.length() != std::size_t(0U)) && (my_str2.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str3_has_error = ((my_str3.length() != std::size_t(0U)) && (my_str3.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str4_has_error = ((my_str4.length() != std::size_t(0U)) && (my_str4.find_first_of(my_decimal_digits) == std::string::npos));

        if(my_str1_has_error || my_str2_has_error || my_str3_has_error || my_str4_has_error)
        {
          my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          {
            std::stringstream my_stringstream;

            my_stringstream << my_mr[1U] << my_mr[2U];
            my_stringstream >> my_real_input;
          }

          // Extract the imaginary part of the complex number.
          {
            std::stringstream my_stringstream;

            my_stringstream << my_mr[3U] << my_mr[4U];
            my_stringstream >> my_imag_input;
          }
        }
      }
      else if(   (my_it_regex_match == (my_regexes.cbegin() + 1U))
              || (my_it_regex_match == (my_regexes.cbegin() + 2U)))
      {
        // The input matches the_regexes[1U] or the_regexes[2U],
        // corresponding to either of the forms (u) or u.
        // This represents a pure real complex number.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string my_str1(my_mr[1U]);
        const std::string my_str2(my_mr[2U]);

        const std::string my_decimal_digits("0123456789");

        if(   ((my_str1.length() != std::size_t(0U)) && (my_str1.find_first_of(my_decimal_digits) == std::string::npos))
           || ((my_str2.length() != std::size_t(0U)) && (my_str2.find_first_of(my_decimal_digits) == std::string::npos)))
        {
          my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream my_stringstream;

          my_stringstream << my_mr[1U] << my_mr[2U];
          my_stringstream >> my_real_input;

          my_imag_input = T(0U);
        }
      }

      if(my_input_has_error)
      {
        // Error case: Restore the characters in the input stream
        // and set the fail bit in the input stream.
        std::for_each(my_input_str.cbegin(),
                      my_input_str.cend(),
                      [&my_istream](const char& my_c)
                      {
                        my_istream.putback(my_c);
                      });

        my_istream.setstate(std::ios_base::failbit);
      }
      else
      {
        my_z = complex<T>(my_real_input, my_imag_input);
      }

      return my_istream;
    }

    template<class T, class char_type, class traits_type>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& my_ostream, const complex<T>& my_z)
    {
      std::basic_ostringstream<char_type, traits_type> my_tmp_ostream;

      my_tmp_ostream.flags    (my_ostream.flags());
      my_tmp_ostream.imbue    (my_ostream.getloc());
      my_tmp_ostream.precision(my_ostream.precision());

      my_tmp_ostream << '('
                     << my_z.real()
                     << ','
                     << my_z.imag()
                     << ')';

      return (my_ostream << my_tmp_ostream.str());
    }

    #endif // !EXTENDED_COMPLEX_DISABLE_IOSTREAM

    // Value operations for extended_complex::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.7.

    template<typename T> T real(const complex<T>& my_z) { return my_z.real(); }
    template<typename T> T imag(const complex<T>& my_z) { return my_z.imag(); }

    template<typename T> T abs (const complex<T>& my_z) { using std::sqrt;  return sqrt(norm(my_z)); }
    template<typename T> T arg (const complex<T>& my_z) { using std::atan2; return atan2(my_z.imag(), my_z.real()); }

    template<typename T> T norm(const complex<T>& my_z)
    {
      using std::fabs;

      T my_result;

      if(fabs(my_z.real()) < fabs(my_z.imag()))
      {
        const T my_a_over_b = my_z.real() / my_z.imag();

        my_result = (my_z.imag() * my_z.imag()) * (T(1U) + (my_a_over_b * my_a_over_b));
      }
      else
      {
        const T my_b_over_a = my_z.imag() / my_z.real();

        my_result = (my_z.real() * my_z.real()) * (T(1U) + (my_b_over_a * my_b_over_a));
      }

      return my_result;
    }

    template<typename T> complex<T> conj(const complex<T>& my_z)
    {
      return complex<T>(-my_z.imag(), my_z.real());
    }

    template<typename T> complex<T> proj(const complex<T>& my_z)
    {
      const T denominator_half((norm(my_z) + T(1U)) / 2U);

      return complex<T>(my_z.real() / denominator_half,
                        my_z.imag() / denominator_half);
    }

    template<typename T> complex<T> polar(const T& my_rho, const T& my_theta)
    {
      using std::cos;
      using std::sin;

      return complex<T>(my_rho * cos(my_theta), my_rho * sin(my_theta));
    }


    // Elementary transcendental functions for extended_complex::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.8.

    template<typename T> complex<T> acos(const complex<T>& my_z)
    {
      using std::asin;

      return T(asin(T(1U))) - asin(my_z);
    }

    template<typename T> complex<T> asin(const complex<T>& my_z)
    {
      return -conj(log(conj(my_z) + sqrt(T(1U) - (my_z * my_z))));
    }

    template<typename T> complex<T> atan(const complex<T>& my_z)
    {
      const complex<T> z_conj = conj(my_z);

      complex<T> result = conj(log(T(1) - z_conj) - log(T(1) + z_conj));

      result.real(result.real() / 2U);
      result.imag(result.imag() / 2U);

      return result;
    }

    template<typename T> complex<T> acosh(const complex<T>& my_z)
    {
      const complex<T> zp(my_z.real() + T(1U), my_z.imag());
      const complex<T> zm(my_z.real() - T(1U), my_z.imag());

      return log(my_z + (zp * sqrt(zm / zp)));
    }

    template<typename T> complex<T> asinh(const complex<T>& my_z)
    {
      return log(my_z + sqrt(T(1U) + (my_z * my_z)));
    }

    template<typename T> complex<T> atanh(const complex<T>& my_z)
    {
      complex<T> result = (log(T(1U) + my_z) - log(T(1U) - my_z));

      result.real(result.real() / 2U);
      result.imag(result.imag() / 2U);

      return result;
    }

    template<typename T> complex<T> cos(const complex<T>& my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(   cos(my_z.real()) * cosh(my_z.imag()),
                        - (sin(my_z.real()) * sinh(my_z.imag())));
    }

    template<typename T> complex<T> cosh(const complex<T>& my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(cos(my_z.imag()) * cosh(my_z.real()),
                        sin(my_z.imag()) * sinh(my_z.real()));
    }

    template<typename T> complex<T> exp(const complex<T>& my_z)
    {
      using std::cos;
      using std::exp;
      using std::sin;

      return complex<T>(cos(my_z.imag()), sin(my_z.imag())) * T(exp(my_z.real()));
    }

    template<typename T> complex<T> log(const complex<T>& my_z)
    {
      using std::atan2;
      using std::log;

      const T my_real_part(log(norm(my_z)) / 2U);
      const T my_imag_part(atan2(my_z.imag(), my_z.real()));

      return complex<T>(my_real_part, my_imag_part);
    }

    template<typename T> complex<T> log10(const complex<T>& my_z)
    {
      using std::log;

      return log(my_z) / T(log(T(10)));
    }

    template<typename T> complex<T> pow(const complex<T>& my_z, int my_pn)
    {
      if     (my_pn <  0) { return  T(1U) / pow(my_z, -my_pn); }
      else if(my_pn == 0) { return  complex<T>(T(1U)); }
      else if(my_pn == 1) { return  my_z; }
      else if(my_pn == 2) { return  my_z * my_z; }
      else if(my_pn == 3) { return (my_z * my_z) * my_z; }
      else if(my_pn == 4) { const complex<T> my_z2(my_z * my_z); return (my_z2 * my_z2); }
      else
      {
        complex<T> my_result = T(static_cast<unsigned>(UINT8_C(1)));

        complex<T> y(my_z);

        auto p_local = static_cast<std::uint64_t>(my_pn);

        // Use the so-called ladder method for the power calculation.
        for(;;)
        {
          const auto do_power_multiply =
            (static_cast<std::uint_fast8_t>(p_local & static_cast<unsigned>(UINT8_C(1))) != static_cast<std::uint_fast8_t>(UINT8_C(0)));

          if(do_power_multiply)
          {
            my_result *= y;
          }

          p_local >>= static_cast<unsigned>(UINT8_C(1));

          if(p_local == static_cast<std::uint64_t>(UINT8_C(0)))
          {
            break;
          }

          y *= y;
        }

        return my_result;
      }
    }

    template<typename T> complex<T> pow(const complex<T>& my_z, const T& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<typename T> complex<T> pow(const complex<T>& my_z, const complex<T>& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<typename T> complex<T> pow(const T& my_z, const complex<T>& my_a)
    {
      using std::log;

      return exp(my_a * T(log(my_z)));
    }

    template<typename T> complex<T> sin(const complex<T>& my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(sin(my_z.real()) * cosh(my_z.imag()),
                        cos(my_z.real()) * sinh(my_z.imag()));
    }

    template<typename T> complex<T> sinh(const complex<T>& my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(cos (my_z.imag()) * sinh(my_z.real()),
                        cosh(my_z.real()) * sin (my_z.imag()));
    }

    template<typename T> complex<T> sqrt(const complex<T>& my_z)
    {
      using std::fabs;
      using std::sqrt;

      // Use the following:
      // sqrt(z) = (s, zi / 2s)       for zr >= 0
      //           (|zi| / 2s, +-s)   for zr <  0
      // where s = sqrt{ [ |zr| + sqrt(zr^2 + zi^2) ] / 2 },
      // and the +- sign is the same as the sign of zi.

      const bool real_part_is_neg(my_z.real() < T(0U));
      const T    real_part_fabs  ((real_part_is_neg == false) ? my_z.real() : -my_z.real());
      const T    s_part          (sqrt((real_part_fabs + abs(my_z)) / 2U));

      if(real_part_is_neg == false)
      {
        return complex<T>(s_part,
                          my_z.imag() / (s_part * 2U));
      }
      else
      {
        const bool imag_part_is_neg(my_z.imag() < T(0U));
        const T    imag_part_fabs  ((imag_part_is_neg == false) ? my_z.imag() : -my_z.imag());

        return complex<T>( imag_part_fabs / (s_part * 2U),
                         ((imag_part_is_neg == false) ? s_part : -s_part));
      }
    }

    template<typename T> complex<T> tan(const complex<T>& my_z)
    {
      return sin(my_z) / cos(my_z);
    }

    template<typename T> complex<T> tanh(const complex<T>& my_z)
    {
      return sinh(my_z) / cosh(my_z);
    }

    // Now we begin the specialization of mp::complex<mp:cpp>

    // Template specializations of unary +/- operators
    // for mp::complex<mp::mp_cpp>.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>&);

    // Template specializations of global add, sub, mul, div operators
    // for mp::complex<mp::mp_cpp>.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>&, const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>&, const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> operator*(const complex<mp_cpp>&, const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> operator/(const complex<mp_cpp>&, const complex<mp_cpp>&);

    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>&, const mp_cpp&);
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>&, const mp_cpp&);
    template<> inline complex<mp_cpp> operator*(const complex<mp_cpp>&, const mp_cpp&);
    template<> inline complex<mp_cpp> operator/(const complex<mp_cpp>&, const mp_cpp&);

    template<> inline complex<mp_cpp> operator+(const mp_cpp&, const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> operator-(const mp_cpp&, const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> operator*(const mp_cpp&, const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> operator/(const mp_cpp&, const complex<mp_cpp>&);

    // Template specializations of equality and inequality operators
    // for mp::complex<mp::mp_cpp>.
    template<> inline bool operator==(const complex<mp_cpp>&, const complex<mp_cpp>&);
    template<> inline bool operator==(const complex<mp_cpp>&, const mp_cpp&);
    template<> inline bool operator==(const mp_cpp&,          const complex<mp_cpp>&);
    template<> inline bool operator!=(const complex<mp_cpp>&, const complex<mp_cpp>&);
    template<> inline bool operator!=(const complex<mp_cpp>&, const mp_cpp&);
    template<> inline bool operator!=(const mp_cpp&,          const complex<mp_cpp>&);

    // Template specializations of elementary and transcendental functions
    // for mp::complex<mp::mp_cpp>.
    template<> inline mp_cpp          real(const complex<mp_cpp>&);
    template<> inline mp_cpp          imag(const complex<mp_cpp>&);

    template<> inline mp_cpp          abs (const complex<mp_cpp>&);
    template<> inline mp_cpp          arg (const complex<mp_cpp>&);
    template<> inline mp_cpp          norm(const complex<mp_cpp>&);

    template<> inline complex<mp_cpp> conj (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> proj (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> polar(const mp_cpp&, const mp_cpp&);

    template<> inline complex<mp_cpp> acos (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> asin (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> atan (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> asinh(const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> acosh(const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> atanh(const complex<mp_cpp>&);

    template<> inline complex<mp_cpp> cos  (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> cosh (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> exp  (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> log  (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> log10(const complex<mp_cpp>&);

    template<> inline complex<mp_cpp> pow  (const complex<mp_cpp>&, int);
    template<> inline complex<mp_cpp> pow  (const complex<mp_cpp>&, const mp_cpp&);
    template<> inline complex<mp_cpp> pow  (const complex<mp_cpp>&, const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> pow  (const mp_cpp&, const complex<mp_cpp>&);

    template<> inline complex<mp_cpp> sin  (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> sinh (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> sqrt (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> tan  (const complex<mp_cpp>&);
    template<> inline complex<mp_cpp> tanh (const complex<mp_cpp>&);

    // Template specializations of I/O stream operators for mp::complex<mp_cpp>.
    template<typename char_type, typename traits_type>
    inline std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>&, complex<mp_cpp>&);

    template<typename char_type, typename traits_type>
    inline std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>&, const complex<mp_cpp>&);

    // Specialization of class template mp::complex<mp::mp_cpp>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.3.

    template<>
    class complex<mp_cpp>
    {
    public:
      typedef mp_cpp value_type;

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      explicit complex(const IntegralType n) : my_re(n),
                                               my_im(0U) { }

      template<typename FloatingPointType,
               typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
      explicit complex(const FloatingPointType f) : my_re(f),
                                                    my_im(0U) { }

      complex(const value_type& my_x = value_type(),
              const value_type& my_y = value_type()) : my_re(my_x),
                                                         my_im(my_y) { }

      complex(const complex& my_z) : my_re(my_z.real()),
                                       my_im(my_z.imag()) { }

      value_type real() const { return my_re; }
      value_type imag() const { return my_im; }

      void real(value_type my_x) { my_re = my_x; }
      void imag(value_type my_y) { my_im = my_y; }

      complex& operator=(const value_type& my_other_x)
      {
        my_re  = my_other_x;
        my_im  = value_type(0U);

        return *this;
      }

      complex& operator+=(const value_type& v) { my_re += v;             return *this; }
      complex& operator-=(const value_type& v) { my_re -= v;             return *this; }
      complex& operator*=(const value_type& v) { my_re *= v; my_im *= v; return *this; }
      complex& operator/=(const value_type& v) { my_re /= v; my_im /= v; return *this; }

      complex& operator=(const complex& my_other_z)
      {
        if(this != &my_other_z)
        {
          my_re = my_other_z.my_re;
          my_im = my_other_z.my_im;
        }

        return *this;
      }

      complex& operator+=(const complex& my_z)
      {
        my_re += my_z.my_re;
        my_im += my_z.my_im;

        return *this;
      }

      complex& operator-=(const complex& my_z)
      {
        if(this == &my_z)
        {
          real(mp_cpp(0U));
          imag(mp_cpp(0U));
        }
        else
        {
          my_re -= static_cast<value_type>(my_z.my_re);
          my_im -= static_cast<value_type>(my_z.my_im);
        }

        return *this;
      }

      complex& operator*=(const complex& my_z)
      {
        const value_type tmp_re(my_re);

        if(this == &my_z)
        {
          my_re = (tmp_re * tmp_re) - (my_im * my_im);
          my_im = (tmp_re * my_im) * 2U;
        }
        else
        {
          my_re = (tmp_re * my_z.my_re) - (my_im * my_z.my_im);
          my_im = (tmp_re * my_z.my_im) + (my_im * my_z.my_re);
        }

        return *this;
      }

      complex& operator/=(const complex& my_z)
      {
        if(this == &my_z)
        {
          real(mp_cpp(1U));
          imag(mp_cpp(0U));
        }
        else
        {
          const value_type one_over_denom(norm(my_z).calculate_inv());

          const value_type tmp_re(my_re);

          my_re = ((tmp_re  * my_z.my_re) + (my_im * my_z.my_im)) * one_over_denom;
          my_im = ((my_im * my_z.my_re) - (tmp_re  * my_z.my_im)) * one_over_denom;
        }

        return *this;
      }

      complex& calculate_inv ();
      complex& calculate_sqrt();
      complex& calculate_log ();
      complex& calculate_exp ();

      void negate()
      {
        my_re.negate();
        my_im.negate();
      }

      // Operators with integer.
      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator+=(const IntegralType n)
      {
        my_re += n;

        return *this;
      }

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator-=(const IntegralType n)
      {
        my_re -= n;

        return *this;
      }

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator*=(const IntegralType n)
      {
        my_re *= n;
        my_im *= n;

        return *this;
      }

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator/=(const IntegralType n)
      {
        my_re /= n;
        my_im /= n;

        return *this;
      }

      std::int32_t precision() const
      {
        const std::int32_t my_prec_real = my_re.precision();
        const std::int32_t my_prec_imag = my_im.precision();

        return (std::min)(my_prec_real, my_prec_imag);
      }

      void precision(const std::int32_t my_prec)
      {
        my_re.precision(my_prec);
        my_im.precision(my_prec);
      }

      std::int64_t order() const
      {
        const std::int64_t my_order_real = my_re.order();
        const std::int64_t my_order_imag = my_im.order();

        return (std::max)(my_order_real, my_order_imag);
      }

    private:
      value_type my_re;
      value_type my_im;
    };

    inline complex<mp_cpp>& complex<mp_cpp>::calculate_inv()
    {
      // Compute inverse 1 / (x + iy) = (x - iy) / (x^2 + y^2)
      const mp_cpp one_over_denom(norm(*this).calculate_inv());

      my_re *= one_over_denom;
      my_im *= one_over_denom;

      my_im.negate();

      return *this;
    }

    inline complex<mp_cpp>& complex<mp_cpp>::calculate_sqrt()
    {
      // Handle pure real and also pure zero arguments.
      if((my_im.iszero)())
      {
        if((my_re.iszero)() == false)
        {
          my_re.calculate_sqrt();
        }
        else
        {
          // Here we have sqrt(0 + 0i) = 0. Do nothing.
          ;
        }
      }
      else
      {
        // sqrt(*this) = (s, I / 2s)     for R >= 0
        //               (|I| / 2s, +-s) for R <  0
        // where s = sqrt{ [ |R| + sqrt(R^2 + I^2) ] / 2 },
        // and the +- sign is the same as the sign of I.

        // Equation from MPFUN documentation page 12.
        // See: http://www.nersc.gov/~dhb/mpdist/mpdist.html

        const mp_cpp sqrt_term = sqrt((fabs(my_re) + abs(*this)).div_by_int(2));

        if((my_re.iszero)() || ((my_re.isneg)() == false))
        {
          my_re  = sqrt_term;

          my_im /= sqrt_term;

          my_im.div_by_int(2);
        }
        else
        {
          const bool imag_is_pos = ((my_im.isneg)() == false);

          my_re  = fabs(my_im);

          my_re /= sqrt_term;

          my_re.div_by_int(2);

          my_im  = (imag_is_pos ? sqrt_term : -sqrt_term);
        }
      }

      return *this;
    }

    inline complex<mp_cpp>& complex<mp_cpp>::calculate_log()
    {
      const bool real_argument_is_zero = (real().iszero)();
      const bool imag_argument_is_zero = (imag().iszero)();

      // Handle zero argument.
      if(real_argument_is_zero && imag_argument_is_zero)
      {
        my_im = std::numeric_limits<mp_cpp>::infinity();
      }

      // Handle pure real argument.
      if(imag_argument_is_zero)
      {
        my_re = ((real().isone()) ? zero() : log(my_re));

        return *this;
      }

      // Handle pure imaginary argument.
      if(real_argument_is_zero)
      {
        my_re = ((imag().isone() ? zero() : log(imag())));

        my_im = pi_half();

        return *this;
      }

      // Use an AGM iteration to calculate the logarithm.

      // Set a0 = 1.
      // Set b0 = 4 / (*this * 2^m).
      complex ak(one());
      ak.precision(precision());

      const std::int32_t m = detail::compute_pow2_for_agm_log(abs(*this), static_cast<std::int32_t>(precision()));

      complex bk = complex(pow2(static_cast<std::int32_t>(m)));

      bk.precision(precision());
      bk *= *this;
      bk.calculate_inv();
      bk *= static_cast<std::int32_t>(4);

      complex ak_tmp;

      ak_tmp.precision(precision());

      // Determine the requested precision of the upcoming iteration in units of digits10.
      const std::int32_t tol_half    = static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);
      const std::int32_t prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
      const std::int32_t target_prec = (std::min)(prec_half, tol_half);

      // Set the tolerance for the upcoming iteration in units of mp-elements (limbs).
      const std::int32_t tol_elems_half =
          ( (target_prec + (mp_core::mp_elem_digits10 / 2)) / mp_core::mp_elem_digits10)
        + (((target_prec %  mp_core::mp_elem_digits10) != 0) ? 1 : 0);

      for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
      {
        bool break_after_this_iteration = false;

        if(   (k > static_cast<std::int32_t>(7))
           && (ak.order() == bk.order()))
        {
          const mp_cpp local_ak_real(ak.real());
          const mp_cpp local_bk_real(bk.real());

          const mp_cpp local_ak_imag(ak.imag());
          const mp_cpp local_bk_imag(bk.imag());

          if(   (local_ak_real.order() == local_bk_real.order())
             && (local_ak_imag.order() == local_bk_imag.order()))
          {
            std::int32_t sig_elems_real;
            std::int32_t sig_elems_imag;

            // Check for the number of significant digits to be
            // at least half of the requested digits. If at least
            // half of the requested digits have been achieved,
            // then break after the upcoming iteration.

            break_after_this_iteration =
              (   mp::detail::check_close_representation(local_ak_real, local_bk_real, tol_elems_half, sig_elems_real)
               && mp::detail::check_close_representation(local_ak_imag, local_bk_imag, tol_elems_half, sig_elems_imag));

            static_cast<void>(sig_elems_real);
            static_cast<void>(sig_elems_imag);
          }
        }

        ak_tmp  = ak;
        ak     += bk;
        ak     /= static_cast<std::int32_t>(2);

        if(break_after_this_iteration)
        {
          break;
        }

        bk *= ak_tmp;
        bk.calculate_sqrt();
      }

      // The iteration loop is finished.
      // Compute log(x) = {pi / [2 * AGM(1, 4 / 2^m)]} - (m * ln2)
      // Note that ak = bk = AGM(...)

      // Get pi/2.
      operator=(pi_half());

      // Divide by 2 * ak.
      operator*=(ak.calculate_inv());

      // Subtract m * ln2.
      operator-=(ln2() * static_cast<std::int32_t>(m));

      return *this;
    }

    inline complex<mp_cpp>& complex<mp_cpp>::calculate_exp()
    {
      if((my_im.iszero)())
      {
        // Handle various cases for zero imaginary part.
        if((my_re.iszero)())
        {
          my_re = one();
        }
        else
        {
          my_re.calculate_exp();
        }

        return *this;
      }
      else
      {
        // Analyze and prepare the phase of the imaginary part.
        // Make argument (I) positive.
        bool b_negate_sin = false;
        bool b_negate_cos = false;

        if((my_im.isneg)())
        {
          my_im = -my_im;
          b_negate_sin = (!b_negate_sin);
        }

        // Remove even multiples of pi and possibly adjust signs.
        bool b_even_mult_pi = true;

        if(my_im > pi())
        {
          const mp_cpp n_pi = mp_cpp::integer_part(my_im / pi());

          b_even_mult_pi = n_pi.iseven();

          my_im -= (n_pi * pi());
        }

        if(!b_even_mult_pi)
        {
          b_negate_sin = !b_negate_sin;
          b_negate_cos = !b_negate_cos;
        }

        // Compute the trigonometric part alternatively using
        // a Taylor series (for non-high-digit range) or Newton-Raphson
        // in combination with the complex-logarithmic AGM iteration
        // for high-digit range.
        if(!mp_base::mp_high_digit_range(precision()))
        {
          // Perform a Taylor series expansion for cosine
          // and use a simple identity to compute the sine.

          // Extract the pure-real exponent part.
          // Note that R = 0 is a valid input to exp()
          // and exp should return 1 quickly.
          mp_cpp exp_part = exp(my_re);
          my_re = zero();

          // Small argument: Use a standard divide by five identity
          // a certain number of times (9765625 = 5^10).
          std::int32_t n_angle_identity = 10;

          my_im.div_by_int(9765625);

          if(my_im.precision() > 5000)
          {
            n_angle_identity += 3;

            my_im.div_by_int(125);
          }

          // Establish the tolerance for the Taylor series expansion.
          double       dd;
          std::int64_t ne;
          mp_cpp::to_parts(my_im, dd, ne);

          const std::int64_t tol_exp = static_cast<std::int64_t>(ne - static_cast<std::int64_t>(my_im.precision() + 10));

          // Now with small arguments, we are ready for a series expansion.
          // Compute the first two terms of the cosine series: 1 - (y^2) / 2
          mp_cpp y_squared(my_im);
          y_squared *= y_squared;

          mp_cpp term(y_squared);
          term.div_by_int(2);

          mp_cpp val_cos;
          val_cos.precision(precision());
          val_cos = mp_cpp(one()) - term;

          // Taylor expansion for cosine.
          bool b_neg_term = false;

          for(std::int32_t k = static_cast<std::int32_t>(4); ; k += static_cast<std::int32_t>(2))
          {
            term *= y_squared;
            term.div_by_int(k * (k - 1));

            b_neg_term ? val_cos -= term : val_cos += term;
            b_neg_term = !b_neg_term;

            mp_cpp::to_parts(term, dd, ne);

            if((term.iszero)() || (ne < tol_exp))
            {
              break;
            }
          }

          // Convert back using multiple angle identity.
          mp_cpp sum1;
          mp_cpp sum2;
          mp_cpp sum3;
          mp_cpp sum5;

          for(std::int32_t k = static_cast<std::int32_t>(0); k < n_angle_identity; ++k)
          {
            sum1.precision(precision());
            sum2.precision(precision());
            sum3.precision(precision());
            sum5.precision(precision());

            val_cos.precision(precision());

            // Cosine part
            sum1 = val_cos;
            sum2 = val_cos * val_cos;
            sum3 = val_cos * sum2;
            sum5 = sum2    * sum3;

            val_cos  = sum1.mul_by_int(5);
            val_cos -= sum3.mul_by_int(20);
            val_cos += sum5.mul_by_int(16);
          }

          // Set sine and cosine parts
          my_re = val_cos;
          my_im = sqrt(one() - (val_cos * val_cos));

          if(b_negate_sin) my_im = -my_im;
          if(b_negate_cos) my_re = -my_re;

          // Multiply by the pure-real exponent part.
          if((!exp_part.isone()))
          {
            my_re *= exp_part;
            my_im *= exp_part;
          }
        }
        else
        {
          // Implement Newton-Raphson iteration for complex exp using
          // the complex logarithmic AGM iteration.

          // Determine the requested precision of the upcoming iteration in units of digits10.
          const std::int32_t tol_half    = static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);
          const std::int32_t prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
          const std::int32_t target_prec = (std::min)(prec_half, tol_half);

          // Create local copy of original input (*this)
          const complex original_this(*this);

          // Reduce precision for upcoming estimations...
          precision(0);

          std::int32_t estimated_prec = precision();

          complex iterate_term;

          // Precision for next iteration
          iterate_term.precision(0);

          // Generate initial estimate for exp using a low precision calculation.
          // This will use the series expansion above.
          calculate_exp();

          // Set target exponent for the iteration
          const std::int64_t target_exp = static_cast<std::int64_t>(order() - static_cast<std::int64_t>(target_prec));

          for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
          {
            // Perform the next iteration.
            iterate_term           = *this;
            iterate_term.calculate_log();
            iterate_term.negate();
            iterate_term          += original_this;

            const std::int64_t iterate_exp = iterate_term.order();
            const bool  b_is_zero          = ((my_re.iszero)() && (my_im.iszero)());

            iterate_term          += one();
            operator              *= (iterate_term);

            // Test the order of the last iteration change.
            // If it is small enough, then the calculation is finished.
            if(   ((iterate_exp    <= target_exp)  && (b_is_zero == false))
               || ((estimated_prec >= target_prec) &&  b_is_zero))
            {
              // Reset precision and exit iteration loop.
              precision(original_this.precision());

              break;
            }

            // Double the estimated precision.
            estimated_prec = static_cast<std::int32_t>(static_cast<std::int64_t>(static_cast<double>(estimated_prec) * static_cast<double>(1.9)));

            // Determine precision for next iteration
            const std::int32_t x_digits = b_is_zero ? estimated_prec : static_cast<std::int32_t>(order() - iterate_exp);
            const std::int32_t n_digits = (4 * x_digits) + (10 * mp_core::mp_elem_digits10);
            const std::int32_t n_prec   = ((n_digits < original_this.precision()) ? n_digits : original_this.precision());

            // Set the precisions for next iteration
            precision             (n_prec);
            iterate_term.precision(n_prec);
          }
        }

        if(b_negate_sin) { my_im = -my_im; }
        if(b_negate_cos) { my_re = -my_re; }

        return *this;
      }
    }

    // Non-member operations for mp::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.6.

    // Unary +/- operators.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>& my_u) { return complex<mp_cpp>(my_u); }
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>& my_u) { return complex<mp_cpp>(-my_u.real(), -my_u.imag()); }

    // Global binary add, sub, mul, div of complex op complex.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_u) += my_v; }
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_u) -= my_v; }
    template<> inline complex<mp_cpp> operator*(const complex<mp_cpp>& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_u) *= my_v; }
    template<> inline complex<mp_cpp> operator/(const complex<mp_cpp>& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_u) /= my_v; }

    // Global binary add, sub, mul, div of complex op T.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>& my_u, const mp_cpp& my_v) { return complex<mp_cpp>(my_u.real() + my_v, my_u.imag()); }
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>& my_u, const mp_cpp& my_v) { return complex<mp_cpp>(my_u.real() - my_v, my_u.imag()); }
    template<> inline complex<mp_cpp> operator*(const complex<mp_cpp>& my_u, const mp_cpp& my_v) { return complex<mp_cpp>(my_u.real() * my_v, my_u.imag() * my_v); }
    template<> inline complex<mp_cpp> operator/(const complex<mp_cpp>& my_u, const mp_cpp& my_v) { return complex<mp_cpp>(my_u.real() / my_v, my_u.imag() / my_v); }

    // Global binary add, sub, mul, div of T op complex.
    template<> inline complex<mp_cpp> operator+(const mp_cpp& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_u) += my_v; }
    template<> inline complex<mp_cpp> operator-(const mp_cpp& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_u) -= my_v; }
    template<> inline complex<mp_cpp> operator*(const mp_cpp& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_v.real() * my_u, my_v.imag() * my_u); }
    template<> inline complex<mp_cpp> operator/(const mp_cpp& my_u, const complex<mp_cpp>& my_v) { return complex<mp_cpp>(my_v).calculate_inv() * my_u; }

    template<> inline bool operator==(const complex<mp_cpp>& my_u, const complex<mp_cpp>& my_v) { return ((my_u.real() == my_v.real()) && (my_u.imag() == my_v.imag())); }
    template<> inline bool operator==(const complex<mp_cpp>& my_u, const mp_cpp         & my_v) { return ((my_u.real() == my_v)        && (my_u.imag() == mp_cpp(0U))); }
    template<> inline bool operator==(const mp_cpp&          my_u, const complex<mp_cpp>& my_v) { return ((my_u == my_v.real())        && (my_v.imag() == mp_cpp(0U))); }
    template<> inline bool operator!=(const complex<mp_cpp>& my_u, const complex<mp_cpp>& my_v) { return ((my_u.real() != my_v.real()) || (my_u.imag() != my_v.imag())); }
    template<> inline bool operator!=(const complex<mp_cpp>& my_u, const mp_cpp         & my_v) { return ((my_u.real() != my_v)        || (my_u.imag() != mp_cpp(0U))); }
    template<> inline bool operator!=(const mp_cpp&          my_u, const complex<mp_cpp>& my_v) { return ((my_u != my_v.real())        || (my_v.imag() != mp_cpp(0U))); }

    template<> inline mp_cpp real(const complex<mp_cpp>& z) { return z.real(); }
    template<> inline mp_cpp imag(const complex<mp_cpp>& z) { return z.imag(); }

    template<> inline mp_cpp abs (const complex<mp_cpp>& z) { return sqrt((z.real() * z.real()) + (z.imag() * z.imag())); }
    template<> inline mp_cpp arg (const complex<mp_cpp>& z) { return atan2(z.imag(), z.real()); }
    template<> inline mp_cpp norm(const complex<mp_cpp>& z) { return (z.real() * z.real()) + (z.imag() * z.imag()); }

    template<> inline complex<mp_cpp> conj(const complex<mp_cpp>& z)
    {
      return complex<mp_cpp>(z.real(), -z.imag());
    }

    template<> inline complex<mp_cpp> proj(const complex<mp_cpp>& z)
    {
      const mp_cpp one_over_denom((norm(z) + one()).calculate_inv());

      return complex<mp_cpp>((z.real() * 2) * one_over_denom,
                             (z.imag() * 2) * one_over_denom);
    }

    template<> inline complex<mp_cpp> polar(const mp_cpp& my_rho, const mp_cpp& my_theta)
    {
      mp_cpp my_s;
      mp_cpp my_c;

      sincos(my_theta, my_s, my_c);

      return complex<mp_cpp>(my_rho * my_c, my_rho * my_s);
    }

    template<> inline complex<mp_cpp> acos(const complex<mp_cpp>& my_z)
    {
      return complex<mp_cpp>(pi_half()) - asin(my_z);
    }

    template<> inline complex<mp_cpp> asin(const complex<mp_cpp>& my_z)
    {
      const complex<mp_cpp> my_z_times_i(-my_z.imag(), my_z.real());

      const complex<mp_cpp> my_pre_result(log(my_z_times_i + sqrt(one() - (my_z * my_z))));

      return complex<mp_cpp>(my_pre_result.imag(), -my_pre_result.real());
    }

    template<> inline complex<mp_cpp> atan(const complex<mp_cpp>& my_z)
    {
      const complex<mp_cpp> my_z_times_i(-my_z.imag(), my_z.real());

      const complex<mp_cpp> my_pre_result(log(one() - my_z_times_i) - log(one() + my_z_times_i));

      return complex<mp_cpp>(-my_pre_result.imag() / 2,
                              my_pre_result.real() / 2);
    }

    template<> inline complex<mp_cpp> acosh(const complex<mp_cpp>& my_z)
    {
      const complex<mp_cpp> my_zp = my_z + one();
      const complex<mp_cpp> my_zm = my_z - one();

      return log(my_z + (my_zp * sqrt(my_zm / my_zp)));
    }

    template<> inline complex<mp_cpp> asinh(const complex<mp_cpp>& my_z)
    {
      return log(my_z + sqrt((my_z * my_z) + one()));
    }

    template<> inline complex<mp_cpp> atanh(const complex<mp_cpp>& my_z)
    {
      const complex<mp_cpp> my_pre_result(  log(one() + my_z)
                                            - log(one() - my_z));

      return complex<mp_cpp>(my_pre_result.real() / 2,
                             my_pre_result.imag() / 2);
    }

    template<> inline complex<mp_cpp> cos(const complex<mp_cpp>& my_z)
    {
      mp_cpp my_sin_x;
      mp_cpp my_cos_x;
      mp_cpp my_sinh_y;
      mp_cpp my_cosh_y;

      sincos  (my_z.real(), my_sin_x,  my_cos_x);
      sinhcosh(my_z.imag(), my_sinh_y, my_cosh_y);

      return complex<mp_cpp>(my_cos_x * my_cosh_y, -(my_sin_x * my_sinh_y));
    }

    template<> inline complex<mp_cpp> cosh(const complex<mp_cpp>& my_z)
    {
      mp_cpp my_sin_y;
      mp_cpp my_cos_y;
      mp_cpp my_sinh_x;
      mp_cpp my_cosh_x;

      sincos  (my_z.imag(), my_sin_y,  my_cos_y);
      sinhcosh(my_z.real(), my_sinh_x, my_cosh_x);

      return complex<mp_cpp>(my_cos_y * my_cosh_x, my_sin_y * my_sinh_x);
    }

    template<> inline complex<mp_cpp> exp(const complex<mp_cpp>& my_z)
    {
      return complex<mp_cpp>(my_z).calculate_exp();
    }

    template<> inline complex<mp_cpp> log(const complex<mp_cpp>& my_z)
    {
      return complex<mp_cpp>(my_z).calculate_log();
    }

    template<> inline complex<mp_cpp> log10(const complex<mp_cpp>& my_z)
    {
      return log(my_z) / ln10();
    }

    template<> inline complex<mp_cpp> pow(const complex<mp_cpp>& my_z, int my_pn)
    {
      if     (my_pn <  0) { return  mp_cpp(1U) / pow(my_z, -my_pn); }
      else if(my_pn == 0) { return  complex<mp_cpp>(mp_cpp(1U)); }
      else if(my_pn == 1) { return  my_z; }
      else if(my_pn == 2) { return  my_z * my_z; }
      else if(my_pn == 3) { return (my_z * my_z) * my_z; }
      else if(my_pn == 4) { const complex<mp_cpp> my_z2(my_z * my_z); return (my_z2 * my_z2); }
      else
      {
        complex<mp_cpp> my_result = mp_cpp(static_cast<unsigned>(UINT8_C(1)));

        complex<mp_cpp> y(my_z);

        auto p_local = static_cast<std::uint64_t>(my_pn);

        // Use the so-called ladder method for the power calculation.
        for(;;)
        {
          const auto do_power_multiply =
            (static_cast<std::uint_fast8_t>(p_local & static_cast<unsigned>(UINT8_C(1))) != static_cast<std::uint_fast8_t>(UINT8_C(0)));

          if(do_power_multiply)
          {
            my_result *= y;
          }

          p_local >>= static_cast<unsigned>(UINT8_C(1));

          if(p_local == static_cast<std::uint64_t>(UINT8_C(0)))
          {
            break;
          }

          y *= y;
        }

        return my_result;
      }
    }

    template<> inline complex<mp_cpp> pow(const complex<mp_cpp>& my_z, const mp_cpp& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<> inline complex<mp_cpp> pow(const complex<mp_cpp>& my_z, const complex<mp_cpp>& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<> inline complex<mp_cpp> pow(const mp_cpp& my_z, const complex<mp_cpp>& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<> inline complex<mp_cpp> sin(const complex<mp_cpp>& my_z)
    {
      mp_cpp my_sin_x;
      mp_cpp my_cos_x;
      mp_cpp my_sinh_y;
      mp_cpp my_cosh_y;

      sincos  (my_z.real(), my_sin_x,  my_cos_x);
      sinhcosh(my_z.imag(), my_sinh_y, my_cosh_y);

      return complex<mp_cpp>(my_sin_x * my_cosh_y, my_cos_x * my_sinh_y);
    }

    template<> inline complex<mp_cpp> sinh(const complex<mp_cpp>& my_z)
    {
      mp_cpp my_sin_y;
      mp_cpp my_cos_y;
      mp_cpp my_sinh_x;
      mp_cpp my_cosh_x;

      sincos  (my_z.imag(), my_sin_y,  my_cos_y);
      sinhcosh(my_z.real(), my_sinh_x, my_cosh_x);

      return complex<mp_cpp>(my_cos_y * my_sinh_x, my_cosh_x * my_sin_y);
    }

    template<> inline complex<mp_cpp> sqrt(const complex<mp_cpp>& my_z)
    {
      return complex<mp_cpp>(my_z).calculate_sqrt();
    }

    template<> inline complex<mp_cpp> tan(const complex<mp_cpp>& my_z)
    {
      mp_cpp my_sin_x;
      mp_cpp my_cos_x;
      mp_cpp my_sinh_y;
      mp_cpp my_cosh_y;

      sincos  (my_z.real(), my_sin_x,  my_cos_x);
      sinhcosh(my_z.imag(), my_sinh_y, my_cosh_y);

      const complex<mp_cpp> my_s(my_sin_x * my_cosh_y,   my_cos_x * my_sinh_y);
      const complex<mp_cpp> my_c(my_cos_x * my_cosh_y, -(my_sin_x * my_sinh_y));

      return my_s / my_c;
    }

    template<> inline complex<mp_cpp> tanh(const complex<mp_cpp>& my_z)
    {
      mp_cpp my_sin_y;
      mp_cpp my_cos_y;
      mp_cpp my_sinh_x;
      mp_cpp my_cosh_x;

      sincos  (my_z.imag(), my_sin_y,  my_cos_y);
      sinhcosh(my_z.real(), my_sinh_x, my_cosh_x);

      const complex<mp_cpp> my_sh(my_cos_y * my_sinh_x, my_cosh_x * my_sin_y);
      const complex<mp_cpp> my_ch(my_cos_y * my_cosh_x, my_sin_y  * my_sinh_x);

      return my_sh / my_ch;
    }

    // I/O stream operators.
    template<typename char_type, typename traits_type>
    inline std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& my_istream, complex<mp_cpp>& my_z)
    {
      // Parse an (extended) complex number of any of the forms u, (u) or (u,v).

      const std::array<std::regex, 3U> my_regexes =
      {{
        // A regular expression for an (extended) complex number of the form (u,v).
        std::regex(  std::string("\\({1}")                             // One open parentheses.
                   + std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                   + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)")     // Possible exponent field.
                   + std::string("\\,{1}")                             // One comma character.
                   + std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                   + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)")     // Possible exponent field.
                   + std::string("\\){1}")),                           // One close parentheses.

        // A regular expression for an (extended) complex number of the form (u).
        std::regex(  std::string("\\({1}")                             // One open parentheses.
                    + std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                    + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)")     // Possible exponent field.
                    + std::string("\\){1}")),                           // One close parentheses.

        // A regular expression for an (extended) complex number of the form u.
        std::regex(  std::string("([\\+\\-]{0,1}[0-9]*\\.*[0-9]*)")    // Possible +-, decimal point and digits 0-9.
                    + std::string("([eE]{0,1}[\\+\\-]{0,1}[0-9]*)"))    // Possible exponent field.
      }};

      mp_cpp my_real_input(0U);
      mp_cpp my_imag_input(0U);

      bool my_input_has_error = false;

      std::string my_input_str;

      my_istream >> my_input_str;

      std::match_results<std::string::const_iterator> my_mr;

      auto my_it_regex_match =
        std::find_if(my_regexes.cbegin(),
                     my_regexes.cend(),
                     [&my_input_str, &my_mr](const std::regex& my_rx) -> bool
                     {
                       return std::regex_match(my_input_str, my_mr, my_rx);
                     });

      if(my_it_regex_match == my_regexes.cend())
      {
        // The input does not match any of the regular expressions.
        // Set the error flag and take no other action.

        my_input_has_error = true;
      }
      else if(my_it_regex_match == my_regexes.cbegin())
      {
        // The input matches my_regexes[0U], corresponding to the form (u,v).
        // This represents a complex number real and imaginary parts.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string my_str1(my_mr[1U]);
        const std::string my_str2(my_mr[2U]);
        const std::string my_str3(my_mr[3U]);
        const std::string my_str4(my_mr[4U]);

        const std::string my_decimal_digits("0123456789");

        const bool my_str1_has_error = ((my_str1.length() != std::size_t(0U)) && (my_str1.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str2_has_error = ((my_str2.length() != std::size_t(0U)) && (my_str2.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str3_has_error = ((my_str3.length() != std::size_t(0U)) && (my_str3.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str4_has_error = ((my_str4.length() != std::size_t(0U)) && (my_str4.find_first_of(my_decimal_digits) == std::string::npos));

        if(my_str1_has_error || my_str2_has_error || my_str3_has_error || my_str4_has_error)
        {
          my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream my_stringstream;

          my_stringstream << my_mr[1U] << my_mr[2U];
          my_stringstream >> my_real_input;

          my_stringstream.str(std::string());
          my_stringstream.clear();

          // Extract the imaginary part of the complex number.

          my_stringstream << my_mr[3U] << my_mr[4U];
          my_stringstream >> my_imag_input;
        }
      }
      else if(   (my_it_regex_match == (my_regexes.cbegin() + 1U))
              || (my_it_regex_match == (my_regexes.cbegin() + 2U)))
      {
        // The input matches the_regexes[1U] or the_regexes[2U],
        // corresponding to either of the forms (u) or u.
        // This represents a complex number having only
        // a pure real part.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string my_str1(my_mr[1U]);
        const std::string my_str2(my_mr[2U]);

        const std::string my_decimal_digits("0123456789");

        if(   ((my_str1.length() != std::size_t(0U)) && (my_str1.find_first_of(my_decimal_digits) == std::string::npos))
           || ((my_str2.length() != std::size_t(0U)) && (my_str2.find_first_of(my_decimal_digits) == std::string::npos)))
        {
          my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream my_stringstream;

          my_stringstream << my_mr[1U] << my_mr[2U];
          my_stringstream >> my_real_input;

          my_imag_input = mp_cpp(0U);
        }
      }

      if(my_input_has_error)
      {
        // Error case: Restore the characters in the input stream
        // and set the fail bit in the input stream.
        std::for_each(my_input_str.cbegin(),
                      my_input_str.cend(),
                      [&my_istream](const char& my_c)
                      {
                        my_istream.putback(my_c);
                      });

        my_istream.setstate(std::ios_base::failbit);
      }
      else
      {
        my_z = complex<mp_cpp>(my_real_input, my_imag_input);
      }

      return my_istream;
    }

    template<typename char_type, typename traits_type>
    inline std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& my_ostream, const complex<mp_cpp>& my_z)
    {
      std::basic_ostringstream<char_type, traits_type> my_tmp_ostream;

      my_tmp_ostream.flags    (my_ostream.flags());
      my_tmp_ostream.imbue    (my_ostream.getloc());
      my_tmp_ostream.precision(my_ostream.precision());

      my_tmp_ostream << '('
                       << my_z.real()
                       << ','
                       << my_z.imag()
                       << ')';

      return (my_ostream << my_tmp_ostream.str());
    }
  }

#endif // MP_COMPLEX_2002_10_09_H
