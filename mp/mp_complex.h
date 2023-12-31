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

  #if !defined(EXTENDED_COMPLEX_DISABLE_IOSTREAM)
  #include <algorithm>
  #include <array>
  #endif
  #include <cmath>
  #include <cstddef>
  #include <cstdint>
  #if !defined(EXTENDED_COMPLEX_DISABLE_IOSTREAM)
  #include <istream>
  #include <ostream>
  #include <regex>
  #include <sstream>
  #include <string>
  #endif
  #include <type_traits>

  #if defined(_MSC_VER) && (_MSC_VER <= 1800)
    #define EXTENDED_COMPLEX_CONSTEXPR
  #else
    #define EXTENDED_COMPLEX_CONSTEXPR constexpr
  #endif

  #include <mp/mp_cpp.h>
  #include <mp/mp_detail.h>

  namespace mp
  {
    // Forward declarations.

    // Class template extended_complex::complex<T, EnableType>.
    // See also ISO/IEC 14882:2011 Sect. 26.4.2.

    template<typename T, typename EnableType = void>
    class complex;

    // Non-member operations for extended_complex::complex<T, EnableType>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.6.

    // Unary +/- operators.
    template<typename T, typename EnableType = void> complex<T, EnableType> operator+(const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator-(const complex<T, EnableType>&);

    // Global add, sub, mul, div operators.
    template<typename T, typename EnableType = void> complex<T, EnableType> operator+(const complex<T, EnableType>&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator-(const complex<T, EnableType>&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator*(const complex<T, EnableType>&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator/(const complex<T, EnableType>&, const complex<T, EnableType>&);

    template<typename T, typename EnableType = void> complex<T, EnableType> operator+(const complex<T, EnableType>&, const T&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator-(const complex<T, EnableType>&, const T&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator*(const complex<T, EnableType>&, const T&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator/(const complex<T, EnableType>&, const T&);

    template<typename T, typename EnableType = void> complex<T, EnableType> operator+(const T&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator-(const T&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator*(const T&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> operator/(const T&, const complex<T, EnableType>&);

    // Equality and inequality operators.
    template<typename T, typename EnableType = void> bool operator==(const complex<T, EnableType>&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> bool operator==(const complex<T, EnableType>&, const T&);
    template<typename T, typename EnableType = void> bool operator==(const T&,          const complex<T, EnableType>&);

    template<typename T, typename EnableType = void> bool operator!=(const complex<T, EnableType>&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> bool operator!=(const complex<T, EnableType>&, const T&);
    template<typename T, typename EnableType = void> bool operator!=(const T&,                      const complex<T, EnableType>&);

    #if !defined(EXTENDED_COMPLEX_DISABLE_IOSTREAM)

    // I/O stream operators.
    template<typename T, typename char_type, typename traits_type, typename EnableType = void>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>&, complex<T, EnableType>&);

    template<typename T, typename char_type, typename traits_type, typename EnableType = void>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>&, const complex<T, EnableType>&);

    #endif // !EXTENDED_COMPLEX_DISABLE_IOSTREAM

    // Value operations for extended_complex::complex<T, EnableType>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.7.

    template<typename T, typename EnableType = void> T real (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> T imag (const complex<T, EnableType>&);

    template<typename T, typename EnableType = void> T abs  (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> T arg  (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> T norm (const complex<T, EnableType>&);

    template<typename T, typename EnableType = void> complex<T, EnableType> conj (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> proj (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> polar(const T&, const T& = T(static_cast<unsigned>(UINT8_C(0))));

    // Elementary transcendental functions for extended_complex::complex<T, EnableType>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.8.

    template<typename T, typename EnableType = void> complex<T, EnableType> acos (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> asin (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> atan (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> asinh(const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> acosh(const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> atanh(const complex<T, EnableType>&);

    template<typename T, typename EnableType = void> complex<T, EnableType> cos  (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> cosh (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> exp  (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> log  (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> log10(const complex<T, EnableType>&);

    template<typename T, typename EnableType = void> complex<T, EnableType> pow  (const complex<T, EnableType>&, int);
    template<typename T, typename EnableType = void> complex<T, EnableType> pow  (const complex<T, EnableType>&, const T&);
    template<typename T, typename EnableType = void> complex<T, EnableType> pow  (const complex<T, EnableType>&, const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> pow  (const T&, const complex<T, EnableType>&);

    template<typename T, typename EnableType = void> complex<T, EnableType> sin  (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> sinh (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> sqrt (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> tan  (const complex<T, EnableType>&);
    template<typename T, typename EnableType = void> complex<T, EnableType> tanh (const complex<T, EnableType>&);

    // Now we begin the specialization of mp::complex<mp:cpp>

    // Template specializations of unary +/- operators
    // for mp::complex<mp::mp_cpp>.
    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const complex<mp_cpp, EnableType>&);

    // Template specializations of global add, sub, mul, div operators
    // for mp::complex<mp::mp_cpp>.
    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const complex<mp_cpp, EnableType>&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const complex<mp_cpp, EnableType>&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator*(const complex<mp_cpp, EnableType>&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator/(const complex<mp_cpp, EnableType>&, const complex<mp_cpp, EnableType>&);

    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const complex<mp_cpp, EnableType>&, const mp_cpp&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const complex<mp_cpp, EnableType>&, const mp_cpp&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator*(const complex<mp_cpp, EnableType>&, const mp_cpp&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator/(const complex<mp_cpp, EnableType>&, const mp_cpp&);

    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const mp_cpp&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const mp_cpp&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator*(const mp_cpp&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> operator/(const mp_cpp&, const complex<mp_cpp, EnableType>&);

    // Template specializations of equality and inequality operators
    // for mp::complex<mp::mp_cpp>.
    template<typename EnableType> bool operator==(const complex<mp_cpp, EnableType>&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> bool operator==(const complex<mp_cpp, EnableType>&, const mp_cpp&);
    template<typename EnableType> bool operator==(const mp_cpp&,          const complex<mp_cpp, EnableType>&);
    template<typename EnableType> bool operator!=(const complex<mp_cpp, EnableType>&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> bool operator!=(const complex<mp_cpp, EnableType>&, const mp_cpp&);
    template<typename EnableType> bool operator!=(const mp_cpp&,          const complex<mp_cpp, EnableType>&);

    // Template specializations of elementary and transcendental functions
    // for mp::complex<mp::mp_cpp>.
    template<typename EnableType> mp_cpp real(const complex<mp_cpp, EnableType>&);
    template<typename EnableType> mp_cpp imag(const complex<mp_cpp, EnableType>&);

    template<typename EnableType> mp_cpp abs (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> mp_cpp arg (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> mp_cpp norm(const complex<mp_cpp, EnableType>&);

    template<typename EnableType> complex<mp_cpp, EnableType> conj (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> proj (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> polar(const mp_cpp&, const mp_cpp&);

    template<typename EnableType> complex<mp_cpp, EnableType> acos (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> asin (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> atan (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> asinh(const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> acosh(const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> atanh(const complex<mp_cpp, EnableType>&);

    template<typename EnableType> complex<mp_cpp, EnableType> cos  (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> cosh (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> exp  (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> log  (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> log10(const complex<mp_cpp, EnableType>&);

    template<typename EnableType> complex<mp_cpp, EnableType> pow  (const complex<mp_cpp, EnableType>&, int);
    template<typename EnableType> complex<mp_cpp, EnableType> pow  (const complex<mp_cpp, EnableType>&, const mp_cpp&);
    template<typename EnableType> complex<mp_cpp, EnableType> pow  (const complex<mp_cpp, EnableType>&, const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> pow  (const mp_cpp&, const complex<mp_cpp, EnableType>&);

    template<typename EnableType> complex<mp_cpp, EnableType> sin  (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> sinh (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> sqrt (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> tan  (const complex<mp_cpp, EnableType>&);
    template<typename EnableType> complex<mp_cpp, EnableType> tanh (const complex<mp_cpp, EnableType>&);

    // Template specializations of I/O stream operators for mp::complex<mp_cpp>.
    template<typename char_type, typename traits_type, typename EnableType>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>&, complex<mp_cpp, EnableType>&);

    template<typename char_type, typename traits_type, typename EnableType>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>&, const complex<mp_cpp, EnableType>&);

    // Specialization of class template mp::complex<mp::mp_cpp>.
    // Similar to ISO/IEC 14882:2011 Sect. 26.4.3.

    template<typename EnableType>
    class complex<mp_cpp, EnableType>
    {
    public:
      typedef mp_cpp value_type;

      template<typename IntegralType,
               typename std::enable_if<std::is_integral<IntegralType>::value>::type const* = nullptr>
      explicit complex(const IntegralType n) : my_re(n) { }

      template<typename FloatingPointType,
               typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
      explicit complex(const FloatingPointType f) : my_re(f) { }

      complex(const value_type& my_x = value_type(),
              const value_type& my_y = value_type()) : my_re(my_x),
                                                       my_im(my_y) { }

      complex(const complex& my_z) : my_re(my_z.real()),
                                     my_im(my_z.imag()) { }

      complex(complex&& my_z) noexcept : my_re(std::move(static_cast<value_type&&>(my_z.real()))),
                                         my_im(std::move(static_cast<value_type&&>(my_z.imag()))) { }

      complex& operator=(const complex& other)
      {
        if(this != &other)
        {
          my_re = other.my_re;
          my_im = other.my_im;
        }

        return *this;
      }

      complex& operator=(complex&& other) noexcept
      {
        my_re = std::move(static_cast<value_type&&>(other.my_re));
        my_im = std::move(static_cast<value_type&&>(other.my_im));

        return *this;
      }

      complex& operator=(const value_type& other_x)
      {
        my_re = other_x;
        my_im = value_type(static_cast<unsigned>(UINT8_C(0)));

        return *this;
      }

      complex& operator=(value_type&& other_x) noexcept
      {
        my_re  = std::move(static_cast<value_type&&>(other_x));
        imag(mp_cpp(static_cast<unsigned>(UINT8_C(0))));

        return *this;
      }

      value_type real() const { return my_re; }
      value_type imag() const { return my_im; }

      void real(const value_type& my_x) { my_re = my_x; }
      void imag(const value_type& my_y) { my_im = my_y; }

      complex& operator+=(const value_type& v) { my_re += v;             return *this; }
      complex& operator-=(const value_type& v) { my_re -= v;             return *this; }
      complex& operator*=(const value_type& v) { my_re *= v; my_im *= v; return *this; }
      complex& operator/=(const value_type& v) { my_re /= v; my_im /= v; return *this; }

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
          const value_type zero(static_cast<unsigned>(UINT8_C(0)));

          real(zero);
          imag(zero);
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
          my_im = (tmp_re * my_im) * static_cast<unsigned>(UINT8_C(2));
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
          real(mp_cpp(static_cast<unsigned>(UINT8_C(1))));
          imag(mp_cpp(static_cast<unsigned>(UINT8_C(0))));
        }
        else
        {
          const value_type one_over_denom(norm(my_z).calculate_inv());

          const value_type tmp_re(my_re);

          my_re = ((tmp_re * my_z.my_re) + (my_im  * my_z.my_im)) * one_over_denom;
          my_im = ((my_im  * my_z.my_re) - (tmp_re * my_z.my_im)) * one_over_denom;
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
        const auto my_prec_real = my_re.precision();
        const auto my_prec_imag = my_im.precision();

        return (std::min)(my_prec_real, my_prec_imag);
      }

      void precision(const std::int32_t my_prec)
      {
        my_re.precision(my_prec);
        my_im.precision(my_prec);
      }

      std::int64_t order() const
      {
        const auto my_order_real = my_re.order();
        const auto my_order_imag = my_im.order();

        return (std::max)(my_order_real, my_order_imag);
      }

    private:
      value_type my_re { };
      value_type my_im { };
    };

    template<typename EnableType> complex<mp_cpp, EnableType>& complex<mp_cpp, EnableType>::calculate_inv()
    {
      // Compute inverse 1 / (x + iy) = (x - iy) / (x^2 + y^2)
      const mp_cpp one_over_denom(norm(*this).calculate_inv());

      my_re *= one_over_denom;
      my_im *= one_over_denom;

      my_im.negate();

      return *this;
    }

    template<typename EnableType> complex<mp_cpp, EnableType>& complex<mp_cpp, EnableType>::calculate_sqrt()
    {
      // Handle pure real and also pure zero arguments.
      if((my_im.iszero)())
      {
        if(!(my_re.iszero)())
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

        const mp_cpp sqrt_term = sqrt((fabs(my_re) + abs(*this)).div_by_int(static_cast<int>(INT8_C(2))));

        if((my_re.iszero)() || (!(my_re.isneg)()))
        {
          my_re  = sqrt_term;

          my_im /= sqrt_term;

          my_im.div_by_int(static_cast<int>(INT8_C(2)));
        }
        else
        {
          const auto imag_is_pos = (!(my_im.isneg)());

          my_re  = fabs(my_im);

          my_re /= sqrt_term;

          my_re.div_by_int(static_cast<int>(INT8_C(2)));

          my_im  = (imag_is_pos ? sqrt_term : -sqrt_term);
        }
      }

      return *this;
    }

    template<typename EnableType> complex<mp_cpp, EnableType>& complex<mp_cpp, EnableType>::calculate_log()
    {
      const auto real_argument_is_zero = (real().iszero)();
      const auto imag_argument_is_zero = (imag().iszero)();

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

      const auto m = detail::compute_pow2_for_agm_log(abs(*this), static_cast<std::int32_t>(precision()));

      complex bk = complex(pow2(static_cast<std::int32_t>(m)));

      bk.precision(precision());
      bk *= *this;
      bk.calculate_inv();
      bk *= static_cast<std::int32_t>(INT8_C(4));

      complex ak_tmp;

      ak_tmp.precision(precision());

      // Determine the requested precision of the upcoming iteration in units of digits10.
      const auto tol_half    = static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);
      const auto prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
      const auto target_prec = (std::min)(prec_half, tol_half);

      // Set the tolerance for the upcoming iteration in units of mp-elements (limbs).
      const std::int32_t tol_elems_half =
          ( (target_prec + (mp_core::mp_elem_digits10 / 2)) / mp_core::mp_elem_digits10)
        + (((target_prec %  mp_core::mp_elem_digits10) != 0) ? 1 : 0);

      for(auto k = static_cast<std::int32_t>(INT8_C(0)); k < static_cast<std::int32_t>(INT8_C(64)); ++k)
      {
        bool break_after_this_iteration = false;

        if(   (k > static_cast<std::int32_t>(INT8_C(7)))
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

    template<typename EnableType> complex<mp_cpp, EnableType>& complex<mp_cpp, EnableType>::calculate_exp()
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
        auto b_negate_sin = false;
        auto b_negate_cos = false;

        if((my_im.isneg)())
        {
          my_im = -my_im;
          b_negate_sin = (!b_negate_sin);
        }

        // Remove even multiples of pi and possibly adjust signs.
        auto b_even_mult_pi = true;

        if(my_im > pi())
        {
          const auto n_pi = mp_cpp::integer_part(my_im / pi());

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
          auto n_angle_identity = static_cast<std::int32_t>(INT8_C(10));

          my_im.div_by_int(9765625);

          if(my_im.precision() > static_cast<std::int32_t>(INT16_C(5000)))
          {
            n_angle_identity += static_cast<std::int32_t>(INT8_C(3));

            my_im.div_by_int(static_cast<int>(INT8_C(125)));
          }

          // Establish the tolerance for the Taylor series expansion.
          double       dd { };
          std::int64_t ne { };
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
          auto b_neg_term = false;

          for(auto k = static_cast<std::int32_t>(INT8_C(4)); ; k += static_cast<std::int32_t>(INT8_C(2)))
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

          for(auto k = static_cast<std::int32_t>(0); k < n_angle_identity; ++k)
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

            val_cos  = sum1.mul_by_int(static_cast<int>(INT8_C( 5)));
            val_cos -= sum3.mul_by_int(static_cast<int>(INT8_C(20)));
            val_cos += sum5.mul_by_int(static_cast<int>(INT8_C(16)));
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
          const auto tol_half    = static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);
          const auto prec_half   = static_cast<std::int32_t>((precision() + 1) / 2);
          const auto target_prec = (std::min)(prec_half, tol_half);

          // Create local copy of original input (*this)
          const complex original_this(*this);

          // Reduce precision for upcoming estimations...
          precision(static_cast<std::int32_t>(INT8_C(0)));

          auto estimated_prec = precision();

          complex iterate_term;

          // Precision for next iteration
          iterate_term.precision(static_cast<std::int32_t>(INT8_C(0)));

          // Generate initial estimate for exp using a low precision calculation.
          // This will use the series expansion above.
          calculate_exp();

          // Set target exponent for the iteration
          const auto target_exp = static_cast<std::int64_t>(order() - static_cast<std::int64_t>(target_prec));

          for(auto k = static_cast<std::int32_t>(INT8_C(0)); k < static_cast<std::int32_t>(INT8_C(64)); ++k)
          {
            // Perform the next iteration.
            iterate_term = *this;

            iterate_term.calculate_log();
            iterate_term.negate();

            iterate_term += original_this;

            const auto iterate_exp = iterate_term.order();
            const auto b_is_zero   = ((my_re.iszero)() && (my_im.iszero)());

            iterate_term += one();
            operator     *= (iterate_term);

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
            estimated_prec =
              static_cast<std::int32_t>
              (
                static_cast<std::int64_t>(static_cast<double>(estimated_prec) * static_cast<double>(1.9))
              );

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
    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const complex<mp_cpp, EnableType>& my_u) { return complex<mp_cpp, EnableType>(my_u); }
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const complex<mp_cpp, EnableType>& my_u) { return complex<mp_cpp, EnableType>(-my_u.real(), -my_u.imag()); }

    // Global binary add, sub, mul, div of complex op complex.
    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const complex<mp_cpp, EnableType>& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_u) += my_v; }
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const complex<mp_cpp, EnableType>& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_u) -= my_v; }
    template<typename EnableType> complex<mp_cpp, EnableType> operator*(const complex<mp_cpp, EnableType>& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_u) *= my_v; }
    template<typename EnableType> complex<mp_cpp, EnableType> operator/(const complex<mp_cpp, EnableType>& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_u) /= my_v; }

    // Global binary add, sub, mul, div of complex op T.
    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const complex<mp_cpp, EnableType>& my_u, const mp_cpp& my_v) { return complex<mp_cpp, EnableType>(my_u.real() + my_v, my_u.imag()); }
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const complex<mp_cpp, EnableType>& my_u, const mp_cpp& my_v) { return complex<mp_cpp, EnableType>(my_u.real() - my_v, my_u.imag()); }
    template<typename EnableType> complex<mp_cpp, EnableType> operator*(const complex<mp_cpp, EnableType>& my_u, const mp_cpp& my_v) { return complex<mp_cpp, EnableType>(my_u.real() * my_v, my_u.imag() * my_v); }
    template<typename EnableType> complex<mp_cpp, EnableType> operator/(const complex<mp_cpp, EnableType>& my_u, const mp_cpp& my_v) { return complex<mp_cpp, EnableType>(my_u.real() / my_v, my_u.imag() / my_v); }

    // Global binary add, sub, mul, div of T op complex.
    template<typename EnableType> complex<mp_cpp, EnableType> operator+(const mp_cpp& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_u) += my_v; }
    template<typename EnableType> complex<mp_cpp, EnableType> operator-(const mp_cpp& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_u) -= my_v; }
    template<typename EnableType> complex<mp_cpp, EnableType> operator*(const mp_cpp& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_v.real() * my_u, my_v.imag() * my_u); }
    template<typename EnableType> complex<mp_cpp, EnableType> operator/(const mp_cpp& my_u, const complex<mp_cpp, EnableType>& my_v) { return complex<mp_cpp, EnableType>(my_v).calculate_inv() * my_u; }

    template<typename EnableType> bool operator==(const complex<mp_cpp, EnableType>& my_u, const complex<mp_cpp, EnableType>& my_v) { return ((my_u.real() == my_v.real()) && (my_u.imag() == my_v.imag())); }
    template<typename EnableType> bool operator==(const complex<mp_cpp, EnableType>& my_u, const mp_cpp&                      my_v) { return ((my_u.real() == my_v)        && (my_u.imag() == mp_cpp(static_cast<unsigned>(UINT8_C(0))))); }
    template<typename EnableType> bool operator==(const mp_cpp&          my_u, const complex<mp_cpp, EnableType>&             my_v) { return ((my_u == my_v.real())        && (my_v.imag() == mp_cpp(static_cast<unsigned>(UINT8_C(0))))); }
    template<typename EnableType> bool operator!=(const complex<mp_cpp, EnableType>& my_u, const complex<mp_cpp, EnableType>& my_v) { return ((my_u.real() != my_v.real()) || (my_u.imag() != my_v.imag())); }
    template<typename EnableType> bool operator!=(const complex<mp_cpp, EnableType>& my_u, const mp_cpp&                      my_v) { return ((my_u.real() != my_v)        || (my_u.imag() != mp_cpp(static_cast<unsigned>(UINT8_C(0))))); }
    template<typename EnableType> bool operator!=(const mp_cpp&          my_u, const complex<mp_cpp, EnableType>&             my_v) { return ((my_u != my_v.real())        || (my_v.imag() != mp_cpp(static_cast<unsigned>(UINT8_C(0))))); }

    template<typename EnableType> mp_cpp real(const complex<mp_cpp, EnableType>& z) { return z.real(); }
    template<typename EnableType> mp_cpp imag(const complex<mp_cpp, EnableType>& z) { return z.imag(); }

    template<typename EnableType> mp_cpp abs (const complex<mp_cpp, EnableType>& z) { return sqrt((z.real() * z.real()) + (z.imag() * z.imag())); }
    template<typename EnableType> mp_cpp arg (const complex<mp_cpp, EnableType>& z) { return atan2(z.imag(), z.real()); }
    template<typename EnableType> mp_cpp norm(const complex<mp_cpp, EnableType>& z) { return (z.real() * z.real()) + (z.imag() * z.imag()); }

    template<typename EnableType> complex<mp_cpp, EnableType> conj(const complex<mp_cpp, EnableType>& z)
    {
      return complex<mp_cpp, EnableType>(z.real(), -z.imag());
    }

    template<typename EnableType> complex<mp_cpp, EnableType> proj(const complex<mp_cpp, EnableType>& z)
    {
      const mp_cpp one_over_denom((norm(z) + one()).calculate_inv());

      return complex<mp_cpp, EnableType>((z.real() * 2) * one_over_denom,
                                         (z.imag() * 2) * one_over_denom);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> polar(const mp_cpp& my_rho, const mp_cpp& my_theta)
    {
      mp_cpp my_s;
      mp_cpp my_c;

      sincos(my_theta, my_s, my_c);

      return complex<mp_cpp, EnableType>(my_rho * my_c, my_rho * my_s);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> acos(const complex<mp_cpp, EnableType>& my_z)
    {
      return complex<mp_cpp, EnableType>(pi_half()) - asin(my_z);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> asin(const complex<mp_cpp, EnableType>& my_z)
    {
      const complex<mp_cpp, EnableType> my_z_times_i(-my_z.imag(), my_z.real());

      const complex<mp_cpp, EnableType> my_pre_result(log(my_z_times_i + sqrt(one() - (my_z * my_z))));

      return complex<mp_cpp, EnableType>(my_pre_result.imag(), -my_pre_result.real());
    }

    template<typename EnableType> complex<mp_cpp, EnableType> atan(const complex<mp_cpp, EnableType>& my_z)
    {
      const complex<mp_cpp, EnableType> my_z_times_i(-my_z.imag(), my_z.real());

      const complex<mp_cpp, EnableType> my_pre_result(log(one() - my_z_times_i) - log(one() + my_z_times_i));

      return complex<mp_cpp, EnableType>(-my_pre_result.imag() / 2, my_pre_result.real() / 2);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> acosh(const complex<mp_cpp, EnableType>& my_z)
    {
      const complex<mp_cpp, EnableType> my_zp = my_z + one();
      const complex<mp_cpp, EnableType> my_zm = my_z - one();

      return log(my_z + (my_zp * sqrt(my_zm / my_zp)));
    }

    template<typename EnableType> complex<mp_cpp, EnableType> asinh(const complex<mp_cpp, EnableType>& my_z)
    {
      return log(my_z + sqrt((my_z * my_z) + one()));
    }

    template<typename EnableType> complex<mp_cpp, EnableType> atanh(const complex<mp_cpp, EnableType>& my_z)
    {
      const complex<mp_cpp, EnableType> my_pre_result(log(one() + my_z) - log(one() - my_z));

      return complex<mp_cpp, EnableType>(my_pre_result.real() / 2, my_pre_result.imag() / 2);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> cos(const complex<mp_cpp, EnableType>& my_z)
    {
      mp_cpp my_sin_x;
      mp_cpp my_cos_x;
      mp_cpp my_sinh_y;
      mp_cpp my_cosh_y;

      sincos  (my_z.real(), my_sin_x,  my_cos_x);
      sinhcosh(my_z.imag(), my_sinh_y, my_cosh_y);

      return complex<mp_cpp, EnableType>(my_cos_x * my_cosh_y, -(my_sin_x * my_sinh_y));
    }

    template<typename EnableType> complex<mp_cpp, EnableType> cosh(const complex<mp_cpp, EnableType>& my_z)
    {
      mp_cpp my_sin_y;
      mp_cpp my_cos_y;
      mp_cpp my_sinh_x;
      mp_cpp my_cosh_x;

      sincos  (my_z.imag(), my_sin_y,  my_cos_y);
      sinhcosh(my_z.real(), my_sinh_x, my_cosh_x);

      return complex<mp_cpp, EnableType>(my_cos_y * my_cosh_x, my_sin_y * my_sinh_x);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> exp(const complex<mp_cpp, EnableType>& my_z)
    {
      return complex<mp_cpp, EnableType>(my_z).calculate_exp();
    }

    template<typename EnableType> complex<mp_cpp, EnableType> log(const complex<mp_cpp, EnableType>& my_z)
    {
      return complex<mp_cpp, EnableType>(my_z).calculate_log();
    }

    template<typename EnableType> complex<mp_cpp, EnableType> log10(const complex<mp_cpp, EnableType>& my_z)
    {
      return log(my_z) / ln10();
    }

    template<typename EnableType> complex<mp_cpp, EnableType> pow(const complex<mp_cpp, EnableType>& my_z, int my_pn)
    {
      if     (my_pn <  static_cast<int>(INT8_C(0))) { return  mp_cpp(static_cast<unsigned>(UINT8_C(1))) / pow(my_z, -my_pn); }
      else if(my_pn == static_cast<int>(INT8_C(0))) { return  complex<mp_cpp, EnableType>(mp_cpp(static_cast<unsigned>(UINT8_C(1)))); }
      else if(my_pn == static_cast<int>(INT8_C(1))) { return  my_z; }
      else if(my_pn == static_cast<int>(INT8_C(2))) { return  my_z * my_z; }
      else if(my_pn == static_cast<int>(INT8_C(3))) { return (my_z * my_z) * my_z; }
      else if(my_pn == static_cast<int>(INT8_C(4))) { const complex<mp_cpp, EnableType> my_z2(my_z * my_z); return (my_z2 * my_z2); }
      else
      {
        complex<mp_cpp, EnableType> my_result = mp_cpp(static_cast<unsigned>(UINT8_C(1)));

        complex<mp_cpp, EnableType> y(my_z);

        auto p_local = static_cast<std::uint64_t>(my_pn);

        // Use the so-called ladder method for the power calculation.
        for(;;)
        {
          const auto lo_bit = static_cast<std::uint_fast8_t>(p_local & static_cast<unsigned>(UINT8_C(1)));

          const auto do_power_multiply = (lo_bit != static_cast<std::uint_fast8_t>(UINT8_C(0)));

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

    template<typename EnableType> complex<mp_cpp, EnableType> pow(const complex<mp_cpp, EnableType>& my_z, const mp_cpp& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<typename EnableType> complex<mp_cpp, EnableType> pow(const complex<mp_cpp, EnableType>& my_z, const complex<mp_cpp, EnableType>& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<typename EnableType> complex<mp_cpp, EnableType> pow(const mp_cpp& my_z, const complex<mp_cpp, EnableType>& my_a)
    {
      return exp(my_a * log(my_z));
    }

    template<typename EnableType> complex<mp_cpp, EnableType> sin(const complex<mp_cpp, EnableType>& my_z)
    {
      mp_cpp my_sin_x;
      mp_cpp my_cos_x;
      mp_cpp my_sinh_y;
      mp_cpp my_cosh_y;

      sincos  (my_z.real(), my_sin_x,  my_cos_x);
      sinhcosh(my_z.imag(), my_sinh_y, my_cosh_y);

      return complex<mp_cpp, EnableType>(my_sin_x * my_cosh_y, my_cos_x * my_sinh_y);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> sinh(const complex<mp_cpp, EnableType>& my_z)
    {
      mp_cpp my_sin_y;
      mp_cpp my_cos_y;
      mp_cpp my_sinh_x;
      mp_cpp my_cosh_x;

      sincos  (my_z.imag(), my_sin_y,  my_cos_y);
      sinhcosh(my_z.real(), my_sinh_x, my_cosh_x);

      return complex<mp_cpp, EnableType>(my_cos_y * my_sinh_x, my_cosh_x * my_sin_y);
    }

    template<typename EnableType> complex<mp_cpp, EnableType> sqrt(const complex<mp_cpp, EnableType>& my_z)
    {
      return complex<mp_cpp, EnableType>(my_z).calculate_sqrt();
    }

    template<typename EnableType> complex<mp_cpp, EnableType> tan(const complex<mp_cpp, EnableType>& my_z)
    {
      mp_cpp my_sin_x;
      mp_cpp my_cos_x;
      mp_cpp my_sinh_y;
      mp_cpp my_cosh_y;

      sincos  (my_z.real(), my_sin_x,  my_cos_x);
      sinhcosh(my_z.imag(), my_sinh_y, my_cosh_y);

      const complex<mp_cpp, EnableType> my_s(my_sin_x * my_cosh_y,   my_cos_x * my_sinh_y);
      const complex<mp_cpp, EnableType> my_c(my_cos_x * my_cosh_y, -(my_sin_x * my_sinh_y));

      return my_s / my_c;
    }

    template<typename EnableType> complex<mp_cpp, EnableType> tanh(const complex<mp_cpp, EnableType>& my_z)
    {
      mp_cpp my_sin_y;
      mp_cpp my_cos_y;
      mp_cpp my_sinh_x;
      mp_cpp my_cosh_x;

      sincos  (my_z.imag(), my_sin_y,  my_cos_y);
      sinhcosh(my_z.real(), my_sinh_x, my_cosh_x);

      const complex<mp_cpp, EnableType> my_sh(my_cos_y * my_sinh_x, my_cosh_x * my_sin_y);
      const complex<mp_cpp, EnableType> my_ch(my_cos_y * my_cosh_x, my_sin_y  * my_sinh_x);

      return my_sh / my_ch;
    }

    // I/O stream operators.
    template<typename char_type, typename traits_type, typename EnableType>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& my_istream, complex<mp_cpp, EnableType>& my_z)
    {
      // Parse an (extended) complex number of any of the forms u, (u) or (u,v).

      const std::array<std::regex, static_cast<std::size_t>(UINT8_C(3))> my_regexes =
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

      mp_cpp my_real_input(static_cast<unsigned>(UINT8_C(0)));
      mp_cpp my_imag_input(static_cast<unsigned>(UINT8_C(0)));

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
        const std::string my_str1(my_mr[static_cast<std::size_t>(UINT8_C(1))]);
        const std::string my_str2(my_mr[static_cast<std::size_t>(UINT8_C(2))]);
        const std::string my_str3(my_mr[static_cast<std::size_t>(UINT8_C(3))]);
        const std::string my_str4(my_mr[static_cast<std::size_t>(UINT8_C(4))]);

        const std::string my_decimal_digits("0123456789");

        const bool my_str1_has_error = ((my_str1.length() != static_cast<std::size_t>(UINT8_C(0))) && (my_str1.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str2_has_error = ((my_str2.length() != static_cast<std::size_t>(UINT8_C(0))) && (my_str2.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str3_has_error = ((my_str3.length() != static_cast<std::size_t>(UINT8_C(0))) && (my_str3.find_first_of(my_decimal_digits) == std::string::npos));
        const bool my_str4_has_error = ((my_str4.length() != static_cast<std::size_t>(UINT8_C(0))) && (my_str4.find_first_of(my_decimal_digits) == std::string::npos));

        if(my_str1_has_error || my_str2_has_error || my_str3_has_error || my_str4_has_error)
        {
          my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream strm;

          strm << my_mr[static_cast<std::size_t>(UINT8_C(1))] << my_mr[static_cast<std::size_t>(UINT8_C(2))];
          strm >> my_real_input;

          strm.str(std::string());
          strm.clear();

          // Extract the imaginary part of the complex number.

          strm << my_mr[static_cast<std::size_t>(UINT8_C(3))] << my_mr[static_cast<std::size_t>(UINT8_C(4))];
          strm >> my_imag_input;
        }
      }
      else if(   (my_it_regex_match == (my_regexes.cbegin() + static_cast<std::size_t>(UINT8_C(1))))
              || (my_it_regex_match == (my_regexes.cbegin() + static_cast<std::size_t>(UINT8_C(2)))))
      {
        // The input matches the_regexes[1U] or the_regexes[2U],
        // corresponding to either of the forms (u) or u.
        // This represents a complex number having only
        // a pure real part.

        // Rule out erroneous match of invalid mantissa or exponent parts.
        const std::string my_str1(my_mr[static_cast<std::size_t>(UINT8_C(1))]);
        const std::string my_str2(my_mr[static_cast<std::size_t>(UINT8_C(2))]);

        const std::string my_decimal_digits("0123456789");

        if(   ((my_str1.length() != static_cast<std::size_t>(UINT8_C(0))) && (my_str1.find_first_of(my_decimal_digits) == std::string::npos))
           || ((my_str2.length() != static_cast<std::size_t>(UINT8_C(0))) && (my_str2.find_first_of(my_decimal_digits) == std::string::npos)))
        {
          my_input_has_error = true;
        }
        else
        {
          // Extract the real part of the complex number.
          std::stringstream strm;

          strm << my_mr[static_cast<std::size_t>(UINT8_C(1))] << my_mr[static_cast<std::size_t>(UINT8_C(2))];
          strm >> my_real_input;

          my_imag_input = mp_cpp(static_cast<unsigned>(UINT8_C(0)));
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
        my_z = complex<mp_cpp, EnableType>(my_real_input, my_imag_input);
      }

      return my_istream;
    }

    template<typename char_type, typename traits_type, typename EnableType>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& my_ostream, const complex<mp_cpp, EnableType>& my_z)
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
