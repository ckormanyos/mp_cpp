///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
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
// Date        : 1999 - 2019
// 
// Description : Implementation of mp::complex<mp_cpp> for multiple precision math.
// 
// *****************************************************************************

#ifndef MP_COMPLEX_2002_10_09_H_
  #define MP_COMPLEX_2002_10_09_H_

  #include <algorithm>
  #include <complex>
  #include <ostream>
  #include <istream>
  #include <regex>
  #include <sstream>

  #include <mp/mp_cpp.h>
  #include <mp/mp_detail.h>

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
    // Forward declaration of class template mp::complex<T>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.2.

    template<typename T>
    class complex;

    // Forward declarations of class template specializations
    // of mp::complex<float>, mp::complex<double>, and
    // mp::complex<long double>.

    template<>
    class complex<float>;

    template<>
    class complex<double>;

    template<>
    class complex<long double>;

    // Forward declaration of class template specialization
    // of mp::complex<mp_cpp>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.3.

    template<>
    class complex<mp_cpp>;

    // Non-member operations for mp::complex<T>.
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

    // Value operations for mp::complex<T>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.7.

    template<typename T> T          real (const complex<T>&);
    template<typename T> T          imag (const complex<T>&);

    template<typename T> T          abs  (const complex<T>&);
    template<typename T> T          arg  (const complex<T>&);
    template<typename T> T          norm (const complex<T>&);

    template<typename T> complex<T> conj (const complex<T>&);
    template<typename T> complex<T> proj (const complex<T>&);
    template<typename T> complex<T> polar(const T&, const T& = T(0U));

    // Elementary transcendental functions for mp::complex<T>.
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

    template<typename T> complex<T> pow  (const complex<T>&, std::int64_t);
    template<typename T> complex<T> pow  (const complex<T>&, const T&);
    template<typename T> complex<T> pow  (const complex<T>&, const complex<T>&);
    template<typename T> complex<T> pow  (const T&, const complex<T>&);

    template<typename T> complex<T> sin  (const complex<T>&);
    template<typename T> complex<T> sinh (const complex<T>&);
    template<typename T> complex<T> sqrt (const complex<T>&);
    template<typename T> complex<T> tan  (const complex<T>&);
    template<typename T> complex<T> tanh (const complex<T>&);

    // I/O stream operators.
    template<typename T, typename char_type, typename traits_type>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>&, complex<T>&);

    template<typename T, typename char_type, typename traits_type>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>&, const complex<T>&);

    // Class template mp::complex<T>.
    // Similat to ISO/IEC 14882:2011 Sect. 26.4.2.

    template<typename T>
    class complex
    {
    public:
      typedef T value_type;

      complex(const value_type& __my_x = value_type(),
              const value_type& __my_y = value_type()) : __my_re(__my_x),
                                                         __my_im(__my_y) { }

      complex(const complex& __my_z) : __my_re(__my_z.real()),
                                       __my_im(__my_z.imag()) { }

      template<typename X>
      complex(const complex<X>& __my_z) : __my_re(static_cast<value_type>(__my_z.real())),
                                          __my_im(static_cast<value_type>(__my_z.imag())) { }

      value_type real() const { return __my_re; }
      value_type imag() const { return __my_im; }

      void real(value_type __my_x) { __my_re = __my_x; }
      void imag(value_type __my_y) { __my_im = __my_y; }

      complex& operator=(const value_type& __my_other_x)
      {
        __my_re = __my_other_x;
        __my_im = value_type(0U);

        return *this;
      }

      complex& operator+=(const value_type& __my_x)
      {
        __my_re += __my_x;

        return *this;
      }

      complex& operator-=(const value_type& __my_x)
      {
        __my_re -= __my_x;

        return *this;
      }

      complex& operator*=(const value_type& __my_x)
      {
        __my_re *= __my_x;
        __my_im *= __my_x;

        return *this;
      }

      complex& operator/=(const value_type& __my_x)
      {
        __my_re /= __my_x;
        __my_im /= __my_x;

        return *this;
      }

      complex& operator=(const complex& __my_other_z)
      {
        if(this != &__my_other_z)
        {
          __my_re = __my_other_z.__my_re;
          __my_im = __my_other_z.__my_im;
        }

        return *this;
      }

      template<typename X>
      complex& operator=(const complex<X>& __my_other_z)
      {
        __my_re = static_cast<value_type>(__my_other_z.__my_re);
        __my_im = static_cast<value_type>(__my_other_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& __my_z)
      {
        __my_re += static_cast<value_type>(__my_z.__my_re);
        __my_im += static_cast<value_type>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& __my_z)
      {
        if(this == &__my_z)
        {
          real(mp_cpp(0U));
          imag(mp_cpp(0U));
        }
        else
        {
          __my_re -= static_cast<value_type>(__my_z.__my_re);
          __my_im -= static_cast<value_type>(__my_z.__my_im);
        }

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& __my_z)
      {
        if(this == &__my_z)
        {
          real((__my_z.real() * __my_z.real()) - (__my_z.imag() * __my_z.imag()));
          imag((__my_z.real() * __my_z.imag()) * 2U);
        }
        else
        {
          const value_type __my_tmp_re(__my_re);

          __my_re = (__my_tmp_re * __my_z.real()) - (imag() * __my_z.imag());
          __my_im = (__my_tmp_re * __my_z.imag()) + (imag() * __my_z.real());
        }

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& __my_z)
      {
        if(this == &__my_z)
        {
          real(1U);
          imag(0U);
        }
        else
        {
          using std::fabs;

          if(fabs(__my_z.real()) < fabs(__my_z.imag()))
          {
            const value_type __my_c_over_d = __my_z.real() / __my_z.imag();

            const value_type __my_denominator = (__my_z.real() * __my_c_over_d) + __my_z.imag();

            const value_type __my_tmp_re(__my_re);
            __my_re = ((__my_tmp_re * __my_c_over_d) + __my_im)     / __my_denominator;
            __my_im = ((__my_im     * __my_c_over_d) - __my_tmp_re) / __my_denominator;
          }
          else
          {
            const value_type __my_d_over_c = __my_z.imag() / __my_z.real();

            const value_type __my_denominator = (__my_z.imag() * __my_d_over_c) + __my_z.real();

            const value_type __my_tmp_re(__my_re);
            __my_re = (( __my_im     * __my_d_over_c) + __my_tmp_re) / __my_denominator;
            __my_im = ((-__my_tmp_re * __my_d_over_c) + __my_im)     / __my_denominator;
          }
        }

        return *this;
      }

    private:
      value_type __my_re;
      value_type __my_im;
    };

    // mp::complex<> specializations.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.3.
    template<>
    class complex<float>
    {
    public:
      typedef float value_type;

      complex(float __my_x = 0.0F,
              float __my_y = 0.0F) : __my_re(__my_x),
                                     __my_im(__my_y) { }

      explicit complex(const complex<double>&);
      explicit complex(const complex<long double>&);

      float real() const { return __my_re; }
      float imag() const { return __my_im; }

      void real(float __my_x) { __my_re = __my_x; }
      void imag(float __my_y) { __my_im = __my_y; }

      complex& operator=(float __my_x)
      {
        __my_re = __my_x;
        __my_im = 0.0F;

        return *this;
      }

      complex& operator+=(float __my_x)
      {
        __my_re += __my_x;

        return *this;
      }

      complex& operator-=(float __my_x)
      {
        __my_re -= __my_x;

        return *this;
      }

      complex& operator*=(float __my_x)
      {
        __my_re *= __my_x;
        __my_im *= __my_x;

        return *this;
      }

      complex& operator/=(float __my_x)
      {
        __my_re /= __my_x;
        __my_im /= __my_x;

        return *this;
      }

      complex& operator=(const complex& __my_z)
      {
        if(this != &__my_z)
        {
          __my_re = __my_z.__my_re;
          __my_im = __my_z.__my_im;
        }

        return *this;
      }

      template<typename X>
      complex<float>& operator=(const complex<X>& __my_z)
      {
        __my_re = static_cast<float>(__my_z.__my_re);
        __my_im = static_cast<float>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& __my_z)
      {
        __my_re += static_cast<float>(__my_z.__my_re);
        __my_im += static_cast<float>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& __my_z)
      {
        __my_re -= static_cast<float>(__my_z.__my_re);
        __my_im -= static_cast<float>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& __my_z)
      {
        const float tmp_im = static_cast<float>((__my_re * __my_z.__my_im) + (__my_im * __my_z.__my_re));

        __my_re = (__my_re * __my_z.__my_re) - (__my_im * __my_z.__my_im);
        __my_im = tmp_im;

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& __my_z)
      {
        const float __denominator = static_cast<float>(norm(__my_z));

        const float __tmp_im = static_cast<float>((__my_im * __my_z.__my_re) - (__my_re * __my_z.__my_im));

        __my_re = ((__my_re * __my_z.__my_re) + (__my_im * __my_z.__my_im)) / __denominator;
        __my_im = __tmp_im / __denominator;

        return *this;
      }

    private:
      float __my_re;
      float __my_im;
    };

    template<>
    class complex<double>
    {
    public:
      typedef double value_type;

      complex(double __my_x = 0.0,
              double __my_y = 0.0) : __my_re(__my_x),
                                     __my_im(__my_y) { }

      complex(const complex<float>& __my_f) : __my_re(double(__my_f.real())),
                                              __my_im(double(__my_f.imag())) { }

      explicit complex(const complex<long double>&);

      double real() const { return __my_re; }
      double imag() const { return __my_im; }

      void real(double __my_x) { __my_re = __my_x; }
      void imag(double __my_y) { __my_im = __my_y; }

      complex& operator=(double __my_x)
      {
        __my_re = __my_x;
        __my_im = 0.0;

        return *this;
      }

      complex& operator+=(double __my_x)
      {
        __my_re += __my_x;

        return *this;
      }

      complex& operator-=(double __my_x)
      {
        __my_re -= __my_x;

        return *this;
      }

      complex& operator*=(double __my_x)
      {
        __my_re *= __my_x;
        __my_im *= __my_x;

        return *this;
      }

      complex& operator/=(double __my_x)
      {
        __my_re /= __my_x;
        __my_im /= __my_x;

        return *this;
      }

      complex& operator=(const complex& __my_z)
      {
        if(this != &__my_z)
        {
          __my_re = __my_z.__my_re;
          __my_im = __my_z.__my_im;
        }

        return *this;
      }

      template<typename X>
      complex& operator=(const complex<X>& __my_z)
      {
        __my_re = static_cast<double>(__my_z.__my_re);
        __my_im = static_cast<double>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& __my_z)
      {
        __my_re += static_cast<double>(__my_z.__my_re);
        __my_im += static_cast<double>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& __my_z)
      {
        __my_re -= static_cast<double>(__my_z.__my_re);
        __my_im -= static_cast<double>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& __my_z)
      {
        const double __tmp_im = static_cast<double>((__my_re * __my_z.__my_im) + (__my_im * __my_z.__my_re));

        __my_re = (__my_re * __my_z.__my_re) - (__my_im * __my_z.__my_im);
        __my_im = __tmp_im;

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& __my_z)
      {
        const double __denominator = static_cast<double>(norm(__my_z));

        const double __tmp_im = static_cast<double>((__my_im * __my_z.__my_re) - (__my_re * __my_z.__my_im));

        __my_re = ((__my_re * __my_z.__my_re) + (__my_im * __my_z.__my_im)) / __denominator;
        __my_im = __tmp_im / __denominator;

        return *this;
      }

    private:
      double __my_re;
      double __my_im;
    };

    template<>
    class complex<long double>
    {
    public:
      typedef long double value_type;

      complex(long double __my_x = 0.0L,
              long double __my_y = 0.0L) : __my_re(__my_x),
                                          __my_im(__my_y) { }

      complex(const complex<float>& __my_f) : __my_re(static_cast<long double>(__my_f.real())),
                                              __my_im(static_cast<long double>(__my_f.imag())) { }

      complex(const complex<double>& __my_d) : __my_re(static_cast<long double>(__my_d.real())),
                                               __my_im(static_cast<long double>(__my_d.imag())) { }

      long double real() const { return __my_re; }
      long double imag() const { return __my_im; }

      void real(long double __my_x) { __my_re = __my_x; }
      void imag(long double __my_y) { __my_im = __my_y; }

      complex& operator=(long double __my_x)
      {
        __my_re = __my_x;
        __my_im = 0.0L;

        return *this;
      }

      complex& operator+=(long double __my_x)
      {
        __my_re += __my_x;

        return *this;
      }

      complex& operator-=(long double __my_x)
      {
        __my_re -= __my_x;

        return *this;
      }

      complex& operator*=(long double __my_x)
      {
        __my_re *= __my_x;
        __my_im *= __my_x;

        return *this;
      }

      complex& operator/=(long double __my_x)
      {
        __my_re /= __my_x;
        __my_im /= __my_x;

        return *this;
      }

      complex& operator=(const complex& __my_z)
      {
        if(this != &__my_z)
        {
          __my_re = __my_z.__my_re;
          __my_im = __my_z.__my_im;
        }

        return *this;
      }

      template<typename X>
      complex& operator=(const complex<X>& __my_z)
      {
        __my_re = static_cast<long double>(__my_z.__my_re);
        __my_im = static_cast<long double>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator+=(const complex<X>& __my_z)
      {
        __my_re += static_cast<long double>(__my_z.__my_re);
        __my_im += static_cast<long double>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator-=(const complex<X>& __my_z)
      {
        __my_re -= static_cast<long double>(__my_z.__my_re);
        __my_im -= static_cast<long double>(__my_z.__my_im);

        return *this;
      }

      template<typename X>
      complex& operator*=(const complex<X>& __my_z)
      {
        const long double __tmp_im = static_cast<long double>((__my_re * __my_z.__my_im) + (__my_im * __my_z.__my_re));

        __my_re = (__my_re * __my_z.__my_re) - (__my_im * __my_z.__my_im);
        __my_im = __tmp_im;

        return *this;
      }

      template<typename X>
      complex& operator/=(const complex<X>& __my_z)
      {
        const long double __denominator = static_cast<long double>(norm(__my_z));

        const long double __tmp_im = static_cast<long double>((__my_im * __my_z.__my_re) - (__my_re * __my_z.__my_im));

        __my_re = ((__my_re * __my_z.__my_re) + (__my_im * __my_z.__my_im)) / __denominator;
        __my_im = __tmp_im / __denominator;

        return *this;
      }

    private:
      long double __my_re;
      long double __my_im;
    };

    // These constructors are located here because they need to be
    // implemented after the template specializations have been declared.

    inline complex<float >::complex(const complex<double>&      __my_d)  : __my_re(float ( __my_d.real())), __my_im(float ( __my_d.imag())) { }
    inline complex<float >::complex(const complex<long double>& __my_ld) : __my_re(float (__my_ld.real())), __my_im(float (__my_ld.imag())) { }
    inline complex<double>::complex(const complex<long double>& __my_ld) : __my_re(double(__my_ld.real())), __my_im(double(__my_ld.imag())) { }

    // mp::complex<T> non-member operations
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.6.

    // Unary +/- operators.
    template<typename T> complex<T> operator+(const complex<T>& __my_u) { return __my_u; }
    template<typename T> complex<T> operator-(const complex<T>& __my_u) { return complex<T>(-__my_u.real(), -__my_u.imag()); }

    // Global add, sub, mul, div operators.
    template<typename T> complex<T> operator+(const complex<T>& __my_u, const complex<T>& __my_v) { return complex<T>(__my_u.real() + __my_v.real(), __my_u.imag() + __my_v.imag()); }
    template<typename T> complex<T> operator-(const complex<T>& __my_u, const complex<T>& __my_v) { return complex<T>(__my_u.real() - __my_v.real(), __my_u.imag() - __my_v.imag()); }

    template<typename T> complex<T> operator*(const complex<T>& __my_u, const complex<T>& __my_v)
    {
      return complex<T>((__my_u.real() * __my_v.real()) - (__my_u.imag() * __my_v.imag()),
                        (__my_u.real() * __my_v.imag()) + (__my_u.imag() * __my_v.real()));
    }

    template<typename T> complex<T> operator/(const complex<T>& __my_u, const complex<T>& __my_v)
    {
      const T __denominator(norm(__my_v));

      return complex<T>(((__my_u.real() * __my_v.real()) + (__my_u.imag() * __my_v.imag())) / __denominator,
                        ((__my_u.imag() * __my_v.real()) - (__my_u.real() * __my_v.imag())) / __denominator);
    }

    template<typename T> complex<T> operator+(const complex<T>& __my_u, const T& __my_v)  { return complex<T>(__my_u.real() + __my_v, __my_u.imag()); }
    template<typename T> complex<T> operator-(const complex<T>& __my_u, const T& __my_v)  { return complex<T>(__my_u.real() - __my_v, __my_u.imag()); }
    template<typename T> complex<T> operator*(const complex<T>& __my_u, const T& __my_v)  { return complex<T>(__my_u.real() * __my_v, __my_u.imag() * __my_v); }
    template<typename T> complex<T> operator/(const complex<T>& __my_u, const T& __my_v)  { return complex<T>(__my_u.real() / __my_v, __my_u.imag() / __my_v); }

    template<typename T> complex<T> operator+(const T& __my_u, const complex<T>& __my_v) { return complex<T>(__my_u + __my_v.real(), __my_v.imag()); }
    template<typename T> complex<T> operator-(const T& __my_u, const complex<T>& __my_v) { return complex<T>(__my_u - __my_v.real(), -__my_v.imag()); }
    template<typename T> complex<T> operator*(const T& __my_u, const complex<T>& __my_v) { return complex<T>(__my_u * __my_v.real(), __my_u * __my_v.imag()); }
    template<typename T> complex<T> operator/(const T& __my_u, const complex<T>& __my_v) { const T __denominator(norm(__my_v));
                                                                                           return complex<T>(( __my_u * __my_v.real()) / __denominator,
                                                                                                             (-__my_u * __my_v.imag()) / __denominator); }

    // Equality and inequality operators.
    template<typename T> bool operator==(const complex<T>& __my_u, const complex<T>& __my_v) { return ((__my_u.real() == __my_v.real()) && (__my_u.imag() == __my_v.imag())); }
    template<typename T> bool operator==(const complex<T>& __my_u, const T&          __my_v) { return ((__my_u.real() == __my_v)        && (__my_u.imag() == T(0))); }
    template<typename T> bool operator==(const T&          __my_u, const complex<T>& __my_v) { return ((__my_u == __my_v.real())        && (__my_v.imag() == T(0))); }

    template<typename T> bool operator!=(const complex<T>& __my_u, const complex<T>& __my_v) { return ((__my_u.real() != __my_v.real()) || (__my_u.imag() != __my_v.imag())); }
    template<typename T> bool operator!=(const complex<T>& __my_u, const T&          __my_v) { return ((__my_u.real() != __my_v)        || (__my_u.imag() != T(0))); }
    template<typename T> bool operator!=(const T&          __my_u, const complex<T>& __my_v) { return ((__my_u != __my_v.real())        || (__my_v.imag() != T(0))); }

    // I/O stream operators.
    template<typename T, typename char_type, typename traits_type>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& __my_istream, complex<T>& __my_z)
    {
      // Parse an (extended) complex number of any of the forms u, (u) or (u,v).

      const std::array<std::regex, 3U> __the_regexes =
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

      T __my_re_z(0U);
      T __my_im_z(0U);

      bool __input_has_error = false;

      std::string __input_str;
      __my_istream >> __input_str;

      std::match_results<std::string::const_iterator> __mr;

      auto __it_regex_match =
        std::find_if(__the_regexes.cbegin(),
                      __the_regexes.cend(),
                      [&__input_str, &__mr](const std::regex& __rx) -> bool
                      {
                        return std::regex_match(__input_str, __mr, __rx);
                      });

      if(__it_regex_match == __the_regexes.cend())
      {
        // The input does not match any of the regular expressions.
        // Set the error flag and take no other action.

        __input_has_error = true;
      }
      else if(__it_regex_match == __the_regexes.cbegin())
      {
        // The input matches __the_regexes[0U], corresponding to the form (u,v).
        // This represents a complex number real and imaginary parts.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string __str1(__mr[1U]);
        const std::string __str2(__mr[2U]);
        const std::string __str3(__mr[3U]);
        const std::string __str4(__mr[4U]);

        const std::string __digits("0123456789");

        const bool __str1_has_error = ((__str1.length() != std::size_t(0U)) && (__str1.find_first_of(__digits) == std::string::npos));
        const bool __str2_has_error = ((__str2.length() != std::size_t(0U)) && (__str2.find_first_of(__digits) == std::string::npos));
        const bool __str3_has_error = ((__str3.length() != std::size_t(0U)) && (__str3.find_first_of(__digits) == std::string::npos));
        const bool __str4_has_error = ((__str4.length() != std::size_t(0U)) && (__str4.find_first_of(__digits) == std::string::npos));

        if(__str1_has_error || __str2_has_error || __str3_has_error || __str4_has_error)
        {
          __input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          {
            std::stringstream __my_stringstream;

            __my_stringstream << __mr[1U] << __mr[2U];
            __my_stringstream >> __my_re_z;
          }

          // Extract the imaginary part of the complex number.
          {
            std::stringstream __my_stringstream;

            __my_stringstream << __mr[3U] << __mr[4U];
            __my_stringstream >> __my_im_z;
          }
        }
      }
      else if(   (__it_regex_match == (__the_regexes.cbegin() + 1U))
              || (__it_regex_match == (__the_regexes.cbegin() + 2U)))
      {
        // The input matches the_regexes[1U] or the_regexes[2U],
        // corresponding to either of the forms (u) or u.
        // This represents a pure real complex number.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string __str1(__mr[1U]);
        const std::string __str2(__mr[2U]);

        const std::string __digits("0123456789");

        if(   ((__str1.length() != std::size_t(0U)) && (__str1.find_first_of(__digits) == std::string::npos))
            || ((__str2.length() != std::size_t(0U)) && (__str2.find_first_of(__digits) == std::string::npos)))
        {
          __input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream __my_stringstream;

          __my_stringstream << __mr[1U] << __mr[2U];
          __my_stringstream >> __my_re_z;

          __my_im_z = T(0U);
        }
      }

      if(__input_has_error)
      {
        // Error case: Restore the characters in the input stream
        // and set the fail bit in the input stream.
        std::for_each(__input_str.cbegin(),
                      __input_str.cend(),
                      [&__my_istream](const char& __my_c)
                      {
                        __my_istream.putback(__my_c);
                      });

        __my_istream.setstate(std::ios_base::failbit);
      }
      else
      {
        __my_z = complex<T>(__my_re_z, __my_im_z);
      }

      return __my_istream;
    }

    template<class T, class char_type, class traits_type>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& __my_ostream, const complex<T>& __my_z)
    {
      std::basic_ostringstream<char_type, traits_type> __tmp_ostream;

      __tmp_ostream.flags    (__my_ostream.flags());
      __tmp_ostream.imbue    (__my_ostream.getloc());
      __tmp_ostream.precision(__my_ostream.precision());

      __tmp_ostream << '('
                    << __my_z.real()
                    << ','
                    << __my_z.imag()
                    << ')';

      return (__my_ostream << __tmp_ostream.str());
    }

    // mp::complex<mp_cpp> value operations.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.7.
    template<typename T> T real(const complex<T>& __my_z) { return __my_z.real(); }
    template<typename T> T imag(const complex<T>& __my_z) { return __my_z.imag(); }

    template<typename T> T abs (const complex<T>& __my_z) { using std::sqrt;  return sqrt(norm(__my_z)); }
    template<typename T> T arg (const complex<T>& __my_z) { using std::atan2; return atan2(__my_z.imag(), __my_z.real()); }
    template<typename T> T norm(const complex<T>& __my_z) { return (__my_z.real() * __my_z.real()) + (__my_z.imag() * __my_z.imag()); }

    template<typename T> complex<T> conj(const complex<T>& __my_z) { return complex<T>(-__my_z.imag(), __my_z.real()); }

    template<typename T> complex<T> proj(const complex<T>& __my_z)
    {
      const T __denominator_half((norm(__my_z) + T(1U)) / 2U);

      return std::complex<T>(__my_z.real() / __denominator_half,
                             __my_z.imag() / __denominator_half);
    }

    template<typename T> complex<T> polar(const T& __my_rho, const T& __my_theta)
    {
      using std::cos;
      using std::sin;

      return complex<T>(__my_rho * cos(__my_theta), __my_rho * sin(__my_theta));
    }

    // mp::complex<mp_cpp> transcendentals.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.8.
    template<typename T> complex<T> acos(const complex<T>& __my_z)
    {
      using std::asin;

      return T(asin(T(1U))) - asin(__my_z);
    }

    template<typename T> complex<T> asin(const complex<T>& __my_z)
    {
      return -conj(log(conj(__my_z) + sqrt(T(1U) - (__my_z * __my_z))));
    }

    template<typename T> complex<T> atan(const complex<T>& __my_z)
    {
      const complex<T> __z_conj = conj(__my_z);

      complex<T> __result = conj(log(T(1) - __z_conj) - log(T(1) + __z_conj));

      __result.real(__result.real() / 2U);
      __result.imag(__result.imag() / 2U);

      return __result;
    }

    template<typename T> complex<T> asinh(const complex<T>& __my_z)
    {
      return log(__my_z + sqrt((__my_z * __my_z) + T(1U)));
    }

    template<typename T> complex<T> acosh(const complex<T>& __my_z)
    {
      const complex<T> __zp(__my_z.real() + T(1U), __my_z.imag());
      const complex<T> __zm(__my_z.real() - T(1U), __my_z.imag());

      return log(__my_z + (__zp * sqrt(__zm / __zp)));
    }

    template<typename T> complex<T> atanh(const complex<T>& __my_z)
    {
      complex<T> __result = (log(T(1U) + __my_z) - log(T(1U) - __my_z));

      __result.real(__result.real() / 2U);
      __result.imag(__result.imag() / 2U);

      return __result;
    }

    template<typename T> complex<T> cos(const complex<T>& __my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(  cos(__my_z.real()) * cosh(__my_z.imag()),
                        -(sin(__my_z.real()) * sinh(__my_z.imag())));
    }

    template<typename T> complex<T> cosh(const complex<T>& __my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(cos(__my_z.imag()) * cosh(__my_z.real()),
                        sin(__my_z.imag()) * sinh(__my_z.real()));
    }

    template<typename T> complex<T> exp(const complex<T>& __my_z)
    {
      using std::cos;
      using std::exp;
      using std::sin;

      return complex<T>(cos(__my_z.imag()), sin(__my_z.imag())) * T(exp(__my_z.real()));
    }

    template<typename T> complex<T> log(const complex<T>& __my_z)
    {
      using std::atan2;
      using std::log;

      return complex<T>(log(norm(__my_z)) / T(2U),
                        atan2(__my_z.imag(), __my_z.real()));
    }

    template<typename T> complex<T> log10(const complex<T>& __my_z)
    {
      using std::log;

      return log(__my_z) / T(log(T(10)));
    }

    template<typename T> complex<T> pow(const complex<T>& __my_z, int __my_pn)
    {
      if     (__my_pn <  0) { return  T(1U) / pow(__my_z, -__my_pn); }
      else if(__my_pn == 0) { return  complex<T>(T(1U)); }
      else if(__my_pn == 1) { return  __my_z; }
      else if(__my_pn == 2) { return  __my_z * __my_z; }
      else if(__my_pn == 3) { return (__my_z * __my_z) * __my_z; }
      else if(__my_pn == 4) { const complex<T> __x2(__my_z * __my_z); return (__x2 * __x2); }
      else
      {
        // The variable xn stores the binary powers of __my_z.
        complex<T> __result(((__my_pn % 2) != 0) ? __my_z : complex<T>(T(1U)));
        complex<T> __xn      (__my_z);

        int __p2 = __my_pn;

        while((__p2 /= 2) != 0)
        {
          // Square xn for each binary power.
          __xn *= __xn;

          const bool __has_binary_power = ((__p2 % 2) != 0);

          if(__has_binary_power)
          {
            // Multiply the result with each binary power contained in the exponent.
            __result *= __xn;
          }
        }

        return __result;
      }
    }

    template<typename T> complex<T> pow(const complex<T>& __my_z, const T& __my_a)
    {
      return exp(__my_a * log(__my_z));
    }

    template<typename T> complex<T> pow(const complex<T>& __my_z, const complex<T>& __my_a)
    {
      return exp(__my_a * log(__my_z));
    }

    template<typename T> complex<T> pow(const T& __my_z, const complex<T>& __my_a)
    {
      using std::log;

      return exp(__my_a * T(log(__my_z)));
    }

    template<typename T> complex<T> sin(const complex<T>& __my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(sin(__my_z.real()) * cosh(__my_z.imag()),
                        cos(__my_z.real()) * sinh(__my_z.imag()));
    }

    template<typename T> complex<T> sinh(const complex<T>& __my_z)
    {
      using std::cos;
      using std::cosh;
      using std::sin;
      using std::sinh;

      return complex<T>(cos (__my_z.imag()) * sinh(__my_z.real()),
                        cosh(__my_z.real()) * sin (__my_z.imag()));
    }

    template<typename T> complex<T> sqrt(const complex<T>& __my_z)
    {
      using std::fabs;
      using std::sqrt;

      // Use the following:
      // sqrt(z) = (s, zi / 2s)       for zr >= 0
      //           (|zi| / 2s, +-s)   for zr <  0
      // where s = sqrt{ [ |zr| + sqrt(zr^2 + zi^2) ] / 2 },
      // and the +- sign is the same as the sign of zi.

      const bool __real_part_is_neg(__my_z.real() < T(0U));
      const T    __real_part_fabs  ((__real_part_is_neg == false) ? __my_z.real() : -__my_z.real());
      const T    __s_part          (sqrt((__real_part_fabs + abs(__my_z)) / 2U));

      if(__real_part_is_neg == false)
      {
        return complex<T>(__s_part,
                          __my_z.imag() / (__s_part * 2U));
      }
      else
      {
        const bool __imag_part_is_neg(__my_z.imag() < T(0U));
        const T    __imag_part_fabs  ((__imag_part_is_neg == false) ? __my_z.imag() : -__my_z.imag());

        return complex<T>( __imag_part_fabs / (__s_part * 2U),
                         ((__imag_part_is_neg == false) ? __s_part : -__s_part));
      }
    }

    template<typename T> complex<T> tan(const complex<T>& __my_z)
    {
      return sin(__my_z) / cos(__my_z);
    }

    template<typename T> complex<T> tanh(const complex<T>& __my_z)
    {
      return sinh(__my_z) / cosh(__my_z);
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

    template<> inline complex<mp_cpp> pow  (const complex<mp_cpp>&, std::int64_t);
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
      explicit complex(const IntegralType n) : __my_re(n),
                                               __my_im(0U) { }

      template<typename FloatingPointType,
               typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
      explicit complex(const FloatingPointType f) : __my_re(f),
                                                    __my_im(0U) { }

      complex(const value_type& __my_x = value_type(),
              const value_type& __my_y = value_type()) : __my_re(__my_x),
                                                         __my_im(__my_y) { }

      complex(const complex& __my_z) : __my_re(__my_z.real()),
                                       __my_im(__my_z.imag()) { }

      value_type real() const { return __my_re; }
      value_type imag() const { return __my_im; }

      void real(value_type __my_x) { __my_re = __my_x; }
      void imag(value_type __my_y) { __my_im = __my_y; }

      complex& operator=(const value_type& __my_other_x)
      {
        __my_re  = __my_other_x;
        __my_im  = value_type(0U);

        return *this;
      }

      complex& operator+=(const value_type& v) { __my_re += v;               return *this; }
      complex& operator-=(const value_type& v) { __my_re -= v;               return *this; }
      complex& operator*=(const value_type& v) { __my_re *= v; __my_im *= v; return *this; }
      complex& operator/=(const value_type& v) { __my_re /= v; __my_im /= v; return *this; }

      complex& operator=(const complex& __my_other_z)
      {
        if(this != &__my_other_z)
        {
          __my_re = __my_other_z.__my_re;
          __my_im = __my_other_z.__my_im;
        }

        return *this;
      }

      complex& operator+=(const complex& __my_z)
      {
        __my_re += __my_z.__my_re;
        __my_im += __my_z.__my_im;

        return *this;
      }

      complex& operator-=(const complex& __my_z)
      {
        if(this == &__my_z)
        {
          real(mp_cpp(0U));
          imag(mp_cpp(0U));
        }
        else
        {
          __my_re -= static_cast<value_type>(__my_z.__my_re);
          __my_im -= static_cast<value_type>(__my_z.__my_im);
        }

        return *this;
      }

      complex& operator*=(const complex& __my_z)
      {
        if(this == &__my_z)
        {
          real((__my_z.real() * __my_z.real()) - (__my_z.imag() * __my_z.imag()));
          imag((__my_z.real() * __my_z.imag()) * 2U);
        }
        else
        {
          const value_type tmp_re(__my_re);

          __my_re = (tmp_re * __my_z.__my_re) - (__my_im * __my_z.__my_im);
          __my_im = (tmp_re * __my_z.__my_im) + (__my_im * __my_z.__my_re);
        }

        return *this;
      }

      complex& operator/=(const complex& __my_z)
      {
        if(this == &__my_z)
        {
          real(mp_cpp(1U));
          imag(mp_cpp(0U));
        }
        else
        {
          const value_type one_over_denom(norm(__my_z).calculate_inv());

          const value_type tmp_re(__my_re);

          __my_re = ((tmp_re  * __my_z.__my_re) + (__my_im * __my_z.__my_im)) * one_over_denom;
          __my_im = ((__my_im * __my_z.__my_re) - (tmp_re  * __my_z.__my_im)) * one_over_denom;
        }

        return *this;
      }

      complex& calculate_inv ();
      complex& calculate_sqrt();
      complex& calculate_log ();
      complex& calculate_exp ();

      void negate()
      {
        __my_re.negate();
        __my_im.negate();
      }

      // Operators with integer.
      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator+=(const IntegralType n)
      {
        __my_re += n;

        return *this;
      }

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator-=(const IntegralType n)
      {
        __my_re -= n;

        return *this;
      }

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator*=(const IntegralType n)
      {
        __my_re *= n;
        __my_im *= n;

        return *this;
      }

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      complex& operator/=(const IntegralType n)
      {
        __my_re /= n;
        __my_im /= n;

        return *this;
      }

      std::int32_t precision() const
      {
        const std::int32_t __my_prec_real = __my_re.precision();
        const std::int32_t __my_prec_imag = __my_im.precision();

        return (std::min)(__my_prec_real, __my_prec_imag);
      }

      void precision(const std::int32_t __my_prec)
      {
        __my_re.precision(__my_prec);
        __my_im.precision(__my_prec);
      }

      std::int64_t order() const
      {
        const std::int64_t __my_order_real = __my_re.order();
        const std::int64_t __my_order_imag = __my_im.order();

        return (std::max)(__my_order_real, __my_order_imag);
      }

    private:
      value_type __my_re;
      value_type __my_im;
    };

    inline complex<mp_cpp>& complex<mp_cpp>::calculate_inv()
    {
      // Compute inverse 1 / (x + iy) = (x - iy) / (x^2 + y^2)
      const mp_cpp one_over_denom(norm(*this).calculate_inv());

      __my_re *= one_over_denom;
      __my_im *= one_over_denom;

      __my_im.negate();

      return *this;
    }

    inline complex<mp_cpp>& complex<mp_cpp>::calculate_sqrt()
    {
      // Handle pure real and also pure zero arguments.
      if((__my_im.iszero)())
      {
        if((__my_re.iszero)() == false)
        {
          __my_re.calculate_sqrt();
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

        const mp_cpp sqrt_term = sqrt((fabs(__my_re) + abs(*this)).div_by_int(2));

        if((__my_re.iszero)() || ((__my_re.isneg)() == false))
        {
          __my_re  = sqrt_term;

          __my_im /= sqrt_term;

          __my_im.div_by_int(2);
        }
        else
        {
          const bool imag_is_pos = ((__my_im.isneg)() == false);

          __my_re  = fabs(__my_im);

          __my_re /= sqrt_term;

          __my_re.div_by_int(2);

          __my_im  = (imag_is_pos ? sqrt_term : -sqrt_term);
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
        __my_im = std::numeric_limits<mp_cpp>::infinity();
      }

      // Handle pure real argument.
      if(imag_argument_is_zero)
      {
        __my_re = ((real().isone()) ? zero() : log(__my_re));

        return *this;
      }

      // Handle pure imaginary argument.
      if(real_argument_is_zero)
      {
        __my_re = ((imag().isone() ? zero() : log(imag())));

        __my_im = pi_half();

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
      if((__my_im.iszero)())
      {
        // Handle various cases for zero imaginary part.
        if((__my_re.iszero)())
        {
          __my_re = one();
        }
        else
        {
          __my_re.calculate_exp();
        }

        return *this;
      }
      else
      {
        // Analyze and prepare the phase of the imaginary part.
        // Make argument (I) positive.
        bool b_negate_sin = false;
        bool b_negate_cos = false;

        if((__my_im.isneg)())
        {
          __my_im = -__my_im;
          b_negate_sin = (!b_negate_sin);
        }

        // Remove even multiples of pi and possibly adjust signs.
        bool b_even_mult_pi = true;

        if(__my_im > pi())
        {
          const mp_cpp n_pi = mp_cpp::integer_part(__my_im / pi());

          b_even_mult_pi = n_pi.iseven();

          __my_im -= (n_pi * pi());
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
          mp_cpp exp_part = exp(__my_re);
          __my_re = zero();

          // Small argument: Use a standard divide by five identity
          // a certain number of times (9765625 = 5^10).
          std::int32_t n_angle_identity = 10;

          __my_im.div_by_int(9765625);

          if(__my_im.precision() > 5000)
          {
            n_angle_identity += 3;

            __my_im.div_by_int(125);
          }

          // Establish the tolerance for the Taylor series expansion.
          double       dd;
          std::int64_t ne;
          mp_cpp::to_parts(__my_im, dd, ne);

          const std::int64_t tol_exp = static_cast<std::int64_t>(ne - static_cast<std::int64_t>(__my_im.precision() + 10));

          // Now with small arguments, we are ready for a series expansion.
          // Compute the first two terms of the cosine series: 1 - (y^2) / 2
          mp_cpp y_squared(__my_im);
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
          __my_re = val_cos;
          __my_im = sqrt(one() - (val_cos * val_cos));

          if(b_negate_sin) __my_im = -__my_im;
          if(b_negate_cos) __my_re = -__my_re;

          // Multiply by the pure-real exponent part.
          if((!exp_part.isone()))
          {
            __my_re *= exp_part;
            __my_im *= exp_part;
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
            const bool  b_is_zero          = ((__my_re.iszero)() && (__my_im.iszero)());

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

        if(b_negate_sin) { __my_im = -__my_im; }
        if(b_negate_cos) { __my_re = -__my_re; }

        return *this;
      }
    }

    // Non-member operations for mp::complex<T>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.6.

    // Unary +/- operators.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>& __my_u) { return complex<mp_cpp>(__my_u); }
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>& __my_u) { return complex<mp_cpp>(-__my_u.real(), -__my_u.imag()); }

    // Global binary add, sub, mul, div of complex op complex.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_u) += __my_v; }
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_u) -= __my_v; }
    template<> inline complex<mp_cpp> operator*(const complex<mp_cpp>& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_u) *= __my_v; }
    template<> inline complex<mp_cpp> operator/(const complex<mp_cpp>& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_u) /= __my_v; }

    // Global binary add, sub, mul, div of complex op T.
    template<> inline complex<mp_cpp> operator+(const complex<mp_cpp>& __my_u, const mp_cpp& __my_v) { return complex<mp_cpp>(__my_u.real() + __my_v, __my_u.imag()); }
    template<> inline complex<mp_cpp> operator-(const complex<mp_cpp>& __my_u, const mp_cpp& __my_v) { return complex<mp_cpp>(__my_u.real() - __my_v, __my_u.imag()); }
    template<> inline complex<mp_cpp> operator*(const complex<mp_cpp>& __my_u, const mp_cpp& __my_v) { return complex<mp_cpp>(__my_u.real() * __my_v, __my_u.imag() * __my_v); }
    template<> inline complex<mp_cpp> operator/(const complex<mp_cpp>& __my_u, const mp_cpp& __my_v) { return complex<mp_cpp>(__my_u.real() / __my_v, __my_u.imag() / __my_v); }

    // Global binary add, sub, mul, div of T op complex.
    template<> inline complex<mp_cpp> operator+(const mp_cpp& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_u) += __my_v; }
    template<> inline complex<mp_cpp> operator-(const mp_cpp& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_u) -= __my_v; }
    template<> inline complex<mp_cpp> operator*(const mp_cpp& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_v.real() * __my_u, __my_v.imag() * __my_u); }
    template<> inline complex<mp_cpp> operator/(const mp_cpp& __my_u, const complex<mp_cpp>& __my_v) { return complex<mp_cpp>(__my_v).calculate_inv() * __my_u; }

    template<> inline bool operator==(const complex<mp_cpp>& __my_u, const complex<mp_cpp>& __my_v) { return ((__my_u.real() == __my_v.real()) && (__my_u.imag() == __my_v.imag())); }
    template<> inline bool operator==(const complex<mp_cpp>& __my_u, const mp_cpp         & __my_v) { return ((__my_u.real() == __my_v)        && (__my_u.imag() == mp_cpp(0U))); }
    template<> inline bool operator==(const mp_cpp&          __my_u, const complex<mp_cpp>& __my_v) { return ((__my_u == __my_v.real())        && (__my_v.imag() == mp_cpp(0U))); }
    template<> inline bool operator!=(const complex<mp_cpp>& __my_u, const complex<mp_cpp>& __my_v) { return ((__my_u.real() != __my_v.real()) || (__my_u.imag() != __my_v.imag())); }
    template<> inline bool operator!=(const complex<mp_cpp>& __my_u, const mp_cpp         & __my_v) { return ((__my_u.real() != __my_v)        || (__my_u.imag() != mp_cpp(0U))); }
    template<> inline bool operator!=(const mp_cpp&          __my_u, const complex<mp_cpp>& __my_v) { return ((__my_u != __my_v.real())        || (__my_v.imag() != mp_cpp(0U))); }

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

    template<> inline complex<mp_cpp> polar(const mp_cpp& __my_rho, const mp_cpp& __my_theta)
    {
      mp_cpp __my_s;
      mp_cpp __my_c;

      sincos(__my_theta, __my_s, __my_c);

      return complex<mp_cpp>(__my_rho * __my_c, __my_rho * __my_s);
    }

    template<> inline complex<mp_cpp> acos(const complex<mp_cpp>& __my_z)
    {
      return complex<mp_cpp>(pi_half()) - asin(__my_z);
    }

    template<> inline complex<mp_cpp> asin(const complex<mp_cpp>& __my_z)
    {
      const complex<mp_cpp> __my_z_times_i(-__my_z.imag(), __my_z.real());

      const complex<mp_cpp> __my_pre_result(log(__my_z_times_i + sqrt(one() - (__my_z * __my_z))));

      return complex<mp_cpp>(__my_pre_result.imag(), -__my_pre_result.real());
    }

    template<> inline complex<mp_cpp> atan(const complex<mp_cpp>& __my_z)
    {
      const complex<mp_cpp> __my_z_times_i(-__my_z.imag(), __my_z.real());

      const complex<mp_cpp> __my_pre_result(log(one() - __my_z_times_i) - log(one() + __my_z_times_i));

      return complex<mp_cpp>(-__my_pre_result.imag() / 2,
                              __my_pre_result.real() / 2);
    }

    template<> inline complex<mp_cpp> acosh(const complex<mp_cpp>& __my_z)
    {
      const complex<mp_cpp> __my_zp = __my_z + one();
      const complex<mp_cpp> __my_zm = __my_z - one();

      return log(__my_z + (__my_zp * sqrt(__my_zm / __my_zp)));
    }

    template<> inline complex<mp_cpp> asinh(const complex<mp_cpp>& __my_z)
    {
      return log(__my_z + sqrt((__my_z * __my_z) + one()));
    }

    template<> inline complex<mp_cpp> atanh(const complex<mp_cpp>& __my_z)
    {
      const complex<mp_cpp> __my_pre_result(  log(one() + __my_z)
                                            - log(one() - __my_z));

      return complex<mp_cpp>(__my_pre_result.real() / 2,
                             __my_pre_result.imag() / 2);
    }

    template<> inline complex<mp_cpp> cos(const complex<mp_cpp>& __my_z)
    {
      mp_cpp __my_sin_x;
      mp_cpp __my_cos_x;
      mp_cpp __my_sinh_y;
      mp_cpp __my_cosh_y;

      sincos  (__my_z.real(), __my_sin_x,  __my_cos_x);
      sinhcosh(__my_z.imag(), __my_sinh_y, __my_cosh_y);

      return complex<mp_cpp>(__my_cos_x * __my_cosh_y, -(__my_sin_x * __my_sinh_y));
    }

    template<> inline complex<mp_cpp> cosh(const complex<mp_cpp>& __my_z)
    {
      mp_cpp __my_sin_y;
      mp_cpp __my_cos_y;
      mp_cpp __my_sinh_x;
      mp_cpp __my_cosh_x;

      sincos  (__my_z.imag(), __my_sin_y,  __my_cos_y);
      sinhcosh(__my_z.real(), __my_sinh_x, __my_cosh_x);

      return complex<mp_cpp>(__my_cos_y * __my_cosh_x, __my_sin_y * __my_sinh_x);
    }

    template<> inline complex<mp_cpp> exp(const complex<mp_cpp>& __my_z)
    {
      return complex<mp_cpp>(__my_z).calculate_exp();
    }

    template<> inline complex<mp_cpp> log(const complex<mp_cpp>& __my_z)
    {
      return complex<mp_cpp>(__my_z).calculate_log();
    }

    template<> inline complex<mp_cpp> log10(const complex<mp_cpp>& __my_z)
    {
      return log(__my_z) / ln10();
    }

    template<> inline complex<mp_cpp> pow(const complex<mp_cpp>& __my_z, std::int64_t __my_pn)
    {
      return detail::pown_template<complex<mp_cpp>>(__my_z, __my_pn);
    }

    template<> inline complex<mp_cpp> pow(const complex<mp_cpp>& __my_z, const mp_cpp& __my_a)
    {
      return exp(__my_a * log(__my_z));
    }

    template<> inline complex<mp_cpp> pow(const complex<mp_cpp>& __my_z, const complex<mp_cpp>& __my_a)
    {
      return exp(__my_a * log(__my_z));
    }

    template<> inline complex<mp_cpp> pow(const mp_cpp& __my_z, const complex<mp_cpp>& __my_a)
    {
      return exp(__my_a * log(__my_z));
    }

    template<> inline complex<mp_cpp> sin(const complex<mp_cpp>& __my_z)
    {
      mp_cpp __my_sin_x;
      mp_cpp __my_cos_x;
      mp_cpp __my_sinh_y;
      mp_cpp __my_cosh_y;

      sincos  (__my_z.real(), __my_sin_x,  __my_cos_x);
      sinhcosh(__my_z.imag(), __my_sinh_y, __my_cosh_y);

      return complex<mp_cpp>(__my_sin_x * __my_cosh_y, __my_cos_x * __my_sinh_y);
    }

    template<> inline complex<mp_cpp> sinh(const complex<mp_cpp>& __my_z)
    {
      mp_cpp __my_sin_y;
      mp_cpp __my_cos_y;
      mp_cpp __my_sinh_x;
      mp_cpp __my_cosh_x;

      sincos  (__my_z.imag(), __my_sin_y,  __my_cos_y);
      sinhcosh(__my_z.real(), __my_sinh_x, __my_cosh_x);

      return complex<mp_cpp>(__my_cos_y * __my_sinh_x, __my_cosh_x * __my_sin_y);
    }

    template<> inline complex<mp_cpp> sqrt(const complex<mp_cpp>& __my_z)
    {
      return complex<mp_cpp>(__my_z).calculate_sqrt();
    }

    template<> inline complex<mp_cpp> tan(const complex<mp_cpp>& __my_z)
    {
      mp_cpp __my_sin_x;
      mp_cpp __my_cos_x;
      mp_cpp __my_sinh_y;
      mp_cpp __my_cosh_y;

      sincos  (__my_z.real(), __my_sin_x,  __my_cos_x);
      sinhcosh(__my_z.imag(), __my_sinh_y, __my_cosh_y);

      const complex<mp_cpp> __my_s(__my_sin_x * __my_cosh_y,   __my_cos_x * __my_sinh_y);
      const complex<mp_cpp> __my_c(__my_cos_x * __my_cosh_y, -(__my_sin_x * __my_sinh_y));

      return __my_s / __my_c;
    }

    template<> inline complex<mp_cpp> tanh(const complex<mp_cpp>& __my_z)
    {
      mp_cpp __my_sin_y;
      mp_cpp __my_cos_y;
      mp_cpp __my_sinh_x;
      mp_cpp __my_cosh_x;

      sincos  (__my_z.imag(), __my_sin_y,  __my_cos_y);
      sinhcosh(__my_z.real(), __my_sinh_x, __my_cosh_x);

      const complex<mp_cpp> __my_sh(__my_cos_y * __my_sinh_x, __my_cosh_x * __my_sin_y);
      const complex<mp_cpp> __my_ch(__my_cos_y * __my_cosh_x, __my_sin_y  * __my_sinh_x);

      return __my_sh / __my_ch;
    }

    // I/O stream operators.
    template<typename char_type, typename traits_type>
    inline std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& __my_istream, complex<mp_cpp>& __my_z)
    {
      // Parse an (extended) complex number of any of the forms u, (u) or (u,v).

      const std::array<std::regex, 3U> __my_regexes =
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

      mp_cpp __my_real_input(0U);
      mp_cpp __my_imag_input(0U);

      bool __my_input_has_error = false;

      std::string __my_input_str;

      __my_istream >> __my_input_str;

      std::match_results<std::string::const_iterator> __my_mr;

      auto __my_it_regex_match =
        std::find_if(__my_regexes.cbegin(),
                     __my_regexes.cend(),
                     [&__my_input_str, &__my_mr](const std::regex& __my_rx) -> bool
                     {
                       return std::regex_match(__my_input_str, __my_mr, __my_rx);
                     });

      if(__my_it_regex_match == __my_regexes.cend())
      {
        // The input does not match any of the regular expressions.
        // Set the error flag and take no other action.

        __my_input_has_error = true;
      }
      else if(__my_it_regex_match == __my_regexes.cbegin())
      {
        // The input matches __the_regexes[0U], corresponding to the form (u,v).
        // This represents a complex number real and imaginary parts.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string __my_str1(__my_mr[1U]);
        const std::string __my_str2(__my_mr[2U]);
        const std::string __my_str3(__my_mr[3U]);
        const std::string __my_str4(__my_mr[4U]);

        const std::string __my_decimal_digits("0123456789");

        const bool __my_str1_has_error = ((__my_str1.length() != std::size_t(0U)) && (__my_str1.find_first_of(__my_decimal_digits) == std::string::npos));
        const bool __my_str2_has_error = ((__my_str2.length() != std::size_t(0U)) && (__my_str2.find_first_of(__my_decimal_digits) == std::string::npos));
        const bool __my_str3_has_error = ((__my_str3.length() != std::size_t(0U)) && (__my_str3.find_first_of(__my_decimal_digits) == std::string::npos));
        const bool __my_str4_has_error = ((__my_str4.length() != std::size_t(0U)) && (__my_str4.find_first_of(__my_decimal_digits) == std::string::npos));

        if(__my_str1_has_error || __my_str2_has_error || __my_str3_has_error || __my_str4_has_error)
        {
          __my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream __my_stringstream;

          __my_stringstream << __my_mr[1U] << __my_mr[2U];
          __my_stringstream >> __my_real_input;

          __my_stringstream.str(std::string());
          __my_stringstream.clear();

          // Extract the imaginary part of the complex number.

          __my_stringstream << __my_mr[3U] << __my_mr[4U];
          __my_stringstream >> __my_imag_input;
        }
      }
      else if(   (__my_it_regex_match == (__my_regexes.cbegin() + 1U))
              || (__my_it_regex_match == (__my_regexes.cbegin() + 2U)))
      {
        // The input matches the_regexes[1U] or the_regexes[2U],
        // corresponding to either of the forms (u) or u.
        // This represents a complex number having only
        // a pure real part.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string __my_str1(__my_mr[1U]);
        const std::string __my_str2(__my_mr[2U]);

        const std::string __my_decimal_digits("0123456789");

        if(   ((__my_str1.length() != std::size_t(0U)) && (__my_str1.find_first_of(__my_decimal_digits) == std::string::npos))
           || ((__my_str2.length() != std::size_t(0U)) && (__my_str2.find_first_of(__my_decimal_digits) == std::string::npos)))
        {
          __my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream __my_stringstream;

          __my_stringstream << __my_mr[1U] << __my_mr[2U];
          __my_stringstream >> __my_real_input;

          __my_imag_input = mp_cpp(0U);
        }
      }

      if(__my_input_has_error)
      {
        // Error case: Restore the characters in the input stream
        // and set the fail bit in the input stream.
        std::for_each(__my_input_str.cbegin(),
                      __my_input_str.cend(),
                      [&__my_istream](const char& __my_c)
                      {
                        __my_istream.putback(__my_c);
                      });

        __my_istream.setstate(std::ios_base::failbit);
      }
      else
      {
        __my_z = complex<mp_cpp>(__my_real_input, __my_imag_input);
      }

      return __my_istream;
    }

    template<typename char_type, typename traits_type>
    inline std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& __my_ostream, const complex<mp_cpp>& __my_z)
    {
      std::basic_ostringstream<char_type, traits_type> __my_tmp_ostream;

      __my_tmp_ostream.flags    (__my_ostream.flags());
      __my_tmp_ostream.imbue    (__my_ostream.getloc());
      __my_tmp_ostream.precision(__my_ostream.precision());

      __my_tmp_ostream << '('
                       << __my_z.real()
                       << ','
                       << __my_z.imag()
                       << ')';

      return (__my_ostream << __my_tmp_ostream.str());
    }
  }

#endif // MP_COMPLEX_2002_10_09_H_