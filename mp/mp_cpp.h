///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_cpp.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Declaration of mp_cpp class for multiple precision math
// 
// *****************************************************************************

#ifndef MP_CPP_2002_10_09_H_
  #define MP_CPP_2002_10_09_H_

  #include <cstdint>
  #include <istream>
  #include <limits>
  #include <iostream>
  #include <iomanip>
  #include <istream>
  #include <ostream>
  #include <sstream>
  #include <string>
  #include <type_traits>

  #include <mp/mp_base.h>
  #include <mp/mp_detail_pown_template.h>

  namespace mp
  {
    class mp_cpp;

    const mp_cpp& zero   ();
    const mp_cpp& one    ();
    const mp_cpp& two    ();
    const mp_cpp& half   ();
    const mp_cpp& pi     ();
    const mp_cpp& pi_half();
    const mp_cpp& ln2    ();
    const mp_cpp& ln10   ();

    // Forward declarations of I/O stream operations of mp_cpp.
    template<typename char_type, typename traits_type>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>&, mp_cpp&);

    template<class char_type, class traits_type>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>&, const mp_cpp&);
  }

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<mp_cpp>.
    template<>
    class numeric_limits<mp::mp_cpp>;
  }

  // *****************************************************************************
  // Function    : class mp_cpp : public mp_base
  // 
  // Description : Class definition for multiple precision
  //               floating point numbers (mp = multiple precision).
  // 
  // *****************************************************************************
  class mp::mp_cpp : public mp::mp_base
  {
  public:
    mp_cpp() noexcept : my_neg(false),
                       my_exp(static_cast<std::int64_t>(0)) { }

    template<typename UnsignedIntegralType,
             typename std::enable_if<(   (std::is_integral<UnsignedIntegralType>::value == true)
                                      && (std::numeric_limits<UnsignedIntegralType>::is_signed == false))>::type const* = nullptr>
    mp_cpp(const UnsignedIntegralType u) noexcept : my_neg(false),
                                                   my_exp(static_cast<std::int64_t>(0))
    {
      from_uint64(u);
    }

    template<typename SignedIntegralType,
             typename std::enable_if<(   (std::is_integral<SignedIntegralType>::value == true)
                                      && (std::numeric_limits<SignedIntegralType>::is_signed == true))>::type const* = nullptr>
    mp_cpp(const SignedIntegralType n) noexcept : my_neg(n < static_cast<std::int64_t>(0)),
                                                 my_exp(static_cast<std::int64_t>(0))
    {
      from_uint64((my_neg ? static_cast<std::uint64_t>(-n) : static_cast<std::uint64_t>(n)));
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    mp_cpp(const FloatingPointType f) noexcept : my_neg(false),
                                                my_exp(static_cast<std::int64_t>(0))
    {
      from_long_double(static_cast<long double>(f));
    }

    explicit mp_cpp(const char* const s) noexcept : my_neg(false),
                                                   my_exp(static_cast<std::int64_t>(0))
    {
      const bool read_is_ok = read_string(s);

      static_cast<void>(read_is_ok);
    }

    explicit mp_cpp(const std::string& str) noexcept: my_neg(false),
                                                     my_exp(static_cast<std::int64_t>(0))
    {
      const bool read_is_ok = read_string(str.c_str());

      static_cast<void>(read_is_ok);
    }

    // Copy constructor.
    mp_cpp(const mp_cpp& other) noexcept : mp_base(other),
                                          my_neg(other.my_neg),
                                          my_exp(other.my_exp) { }

    // Move constructor.
    mp_cpp(mp_cpp&& other) noexcept : mp_base(other),
                                     my_neg(other.my_neg),
                                     my_exp(other.my_exp) { }

    // Constructor from the floating-point class.
    explicit mp_cpp(const mp_fpclass_type other_fpclass) noexcept
      : mp_base(other_fpclass),
        my_neg(false),
        my_exp(static_cast<std::int64_t>(0)) { }

    virtual ~mp_cpp() = default;

    // Constant values
    static const mp_cpp& value_nan();
    static const mp_cpp& value_inf();
    static const mp_cpp& value_max();
    static const mp_cpp& value_min();
    static const mp_cpp& value_eps();

    // Assignment operator.
    mp_cpp& operator=(const mp_cpp& other)
    {
      static_cast<void>(mp_base::operator=(static_cast<const mp_base&>(other)));

      if(this != &other)
      {
        my_neg = other.my_neg;
        my_exp = other.my_exp;
      }

      return *this;
    }

    void swap(mp_cpp& other)
    {
      const mp_cpp tmp(*this);

      *this = other;

      other = tmp;
    }

    // Basic unary math operations.
    mp_cpp& operator+=(const mp_cpp& v);
    mp_cpp& operator-=(const mp_cpp& v);
    mp_cpp& operator*=(const mp_cpp& v);
    mp_cpp& operator/=(const mp_cpp& v);

    mp_cpp& div_by_int(const std::int64_t n);
    mp_cpp& mul_by_int(const std::int64_t n);

    std::int64_t order() const;

    void negate() { my_neg = (!my_neg); }

    // Operators pre-increment and pre-decrement.
    mp_cpp& operator++();
    mp_cpp& operator--();

    // Operators post-increment and post-decrement.
    mp_cpp  operator++(int) { const mp_cpp w(*this); ++(*this); return w; }
    mp_cpp  operator--(int) { const mp_cpp w(*this); --(*this); return w; }

    // Conversions to built-in types.
    static float         to_float      (const mp_cpp& x) { return convert_to_builtin_float<float>(x); }
    static double        to_double     (const mp_cpp& x) { return convert_to_builtin_float<double>(x); }
    static long double   to_long_double(const mp_cpp& x) { return convert_to_builtin_float<long double>(x); }
    static std::uint64_t to_uint64     (const mp_cpp& x);
    static std::int64_t  to_int64      (const mp_cpp& x);
    static void          to_parts      (const mp_cpp& x, double& mantissa, std::int64_t& exponent);

    // Extraction of integer and decimal parts.
    static mp_cpp integer_part(const mp_cpp& x);
    static mp_cpp decimal_part(const mp_cpp& x);

    // Common mathematical functions and constants.
    mp_cpp& calculate_inv      ();
    mp_cpp& calculate_sqrt     ();
    mp_cpp& calculate_rootn_inv(const std::int32_t p);
    mp_cpp& calculate_log      (const std::int32_t my_m, const mp_cpp& p2);
    mp_cpp& calculate_exp      ();

    static const mp_cpp& calculate_pi                (const bool b_trace = false, std::ostream& os = std::cout);
    static const mp_cpp& calculate_pi_borwein_cubic  (const bool b_trace = false, std::ostream& os = std::cout);
    static const mp_cpp& calculate_pi_borwein_quartic(const bool b_trace = false, std::ostream& os = std::cout);
    static const mp_cpp& calculate_pi_borwein_quintic(const bool b_trace = false, std::ostream& os = std::cout);
    static const mp_cpp& calculate_ln2               (const bool b_trace = false, std::ostream& os = std::cout);
    static       mp_cpp  calculate_pow2              (const std::int64_t p);
    static       mp_cpp  calculate_factorial         (const std::int32_t n);

    bool write_string(std::string& result_str,
                      std::streamsize number_of_digits,
                      const std::ios::fmtflags format_flags) const;

    // Comparison functions
    std::int32_t compare(const mp_cpp& v) const;

    bool isone() const
    {
      return (   (my_data.front() == value_type(1U))
              && (my_neg == false)
              && (my_exp == 0)
              && ((isfinite)() == true)
              && (compare_data(one().crepresentation()) == static_cast<std::int32_t>(0)));
    }

    bool (isneg) () const { return  my_neg; }
    bool (ispos) () const { return ((isneg)() == false); }
    bool (iseven)() const;

  private:
    bool         my_neg;
    std::int64_t my_exp;

    // Constructor from mantissa and exponent.
    mp_cpp(const double mantissa, const std::int64_t exponent) noexcept;

    // Conversion from string to mp_cpp.
    bool read_string(const char* const s) noexcept;

    static std::string factorial_binsplit(const std::int32_t n_hi, const std::int32_t n_lo);

    // Math operation algorithms.
    void add(const mp_cpp& v, const std::int64_t v_ofs = 0);
    void sub(const mp_cpp& v, const std::int64_t v_ofs = 0);

    void from_uint64     (const std::uint64_t u)  noexcept;
    void from_uint32     (const std::uint32_t u)  noexcept;
    void from_long_double(const long double   ld) noexcept;

    template<typename builtin_float_type>
    static builtin_float_type convert_to_builtin_float(const mp_cpp& x)
    {
      // Returns the built-in floating-point type conversion of the input value x.
      builtin_float_type value;

      // Check for zero or subnormal mp_cpp.
      if((x.iszero)())
      {
        value = builtin_float_type(0.0L);
      }
      else if((x.isfinite)() == false)
      {
        if((x.isnan)())
        {
          value = std::numeric_limits<builtin_float_type>::quiet_NaN();
        }
        else if((x.isinf)())
        {
          value = (((x.isneg)() == false) ?  std::numeric_limits<builtin_float_type>::infinity()
                                          : -std::numeric_limits<builtin_float_type>::infinity());
        }
        else
        {
          value = builtin_float_type(0.0L);
        }
      }
      else
      {
        const bool b_neg = x.my_neg;

        mp_cpp xx(x);

        xx.my_neg = false;

        // Check if mp_cpp exceeds the maximum value of the built-in floating-point type.
        static const mp_cpp builtin_float_type_max((std::numeric_limits<builtin_float_type>::max)());

        if(xx.compare(builtin_float_type_max) > 0)
        {
          return ((b_neg == false) ?  std::numeric_limits<builtin_float_type>::infinity()
                                   : -std::numeric_limits<builtin_float_type>::infinity());
        }

        std::stringstream ss;

        ss << std::setprecision(static_cast<std::streamsize>(std::numeric_limits<builtin_float_type>::digits10 + 4))
           << std::scientific
           << xx;

        ss >> value;

        if(b_neg)
        {
          value = -value;
        }
      }

      return value;
    }

    template<typename char_type, typename traits_type>
    friend std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>&, mp_cpp&);

    template<class char_type, class traits_type>
    friend std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>&, const mp_cpp&);
  };

  namespace mp
  {
    // Unary +/-
    inline mp_cpp operator+(const mp_cpp& self)
    {
      return mp_cpp(self);
    }

    inline mp_cpp operator-(const mp_cpp& self)
    {
      mp_cpp tmp(self);

      if(((tmp.iszero)() == false) && ((tmp.isfinite)() == true))
      {
        tmp.negate();
      }

      return tmp;
    }

    // Global binary add/sub/mul/div of const mp_cpp op POD.
    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator+(const mp_cpp& u, const ArithmeticType& x)
    {
      return mp_cpp(u) += mp_cpp(x);
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator-(const mp_cpp& u, const ArithmeticType& x)
    {
      return mp_cpp(u) -= mp_cpp(x);
    }

    template<typename SignedIntegralType,
             typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                     && (std::numeric_limits<SignedIntegralType>::is_signed == true)>::type const* = nullptr>
    mp_cpp operator*(const mp_cpp& u, const SignedIntegralType& n)
    {
      return mp_cpp(u).mul_by_int(n);
    }

    template<typename UnsignedIntegralType,
             typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                     && (std::numeric_limits<UnsignedIntegralType>::is_signed == false)>::type const* = nullptr>
    mp_cpp operator*(const mp_cpp& u, const UnsignedIntegralType& n)
    {
      return mp_cpp(u).mul_by_int(static_cast<std::int64_t>(n));
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    mp_cpp operator*(const mp_cpp& u, const FloatingPointType& f)
    {
      return mp_cpp(u) *= mp_cpp(f);
    }

    template<typename SignedIntegralType,
             typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                     && (std::numeric_limits<SignedIntegralType>::is_signed == true)>::type const* = nullptr>
    mp_cpp operator/(const mp_cpp& u, const SignedIntegralType& n)
    {
      return mp_cpp(u).div_by_int(n);
    }

    template<typename UnsignedIntegralType,
             typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                     && (std::numeric_limits<UnsignedIntegralType>::is_signed == false)>::type const* = nullptr>
    mp_cpp operator/(const mp_cpp& u, const UnsignedIntegralType& n)
    {
      return mp_cpp(u).div_by_int(static_cast<std::int64_t>(n));
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    mp_cpp operator/(const mp_cpp& u, const FloatingPointType& f)
    {
      return mp_cpp(u) /= mp_cpp(f);
    }

    // Global binary add/sub/mul/div of const POD op mp_cpp.
    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator+(const ArithmeticType& x, const mp_cpp& v)
    {
      return mp_cpp(x) += v;
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator-(const ArithmeticType& x, const mp_cpp& v)
    {
      return mp_cpp(x) -= v;
    }

    template<typename SignedIntegralType,
             typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                     && (std::numeric_limits<SignedIntegralType>::is_signed == true)>::type const* = nullptr>
    mp_cpp operator*(const SignedIntegralType& n, const mp_cpp& v)
    {
      return mp_cpp(v).mul_by_int(n);
    }

    template<typename UnsignedIntegralType,
             typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                     && (std::numeric_limits<UnsignedIntegralType>::is_signed == false)>::type const* = nullptr>
    mp_cpp operator*(const UnsignedIntegralType& n, const mp_cpp& v)
    {
      return mp_cpp(v).mul_by_int(static_cast<std::int64_t>(n));
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    mp_cpp operator*(const FloatingPointType& f, const mp_cpp& v)
    {
      return mp_cpp(f) *= v;
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator/(const ArithmeticType& x, const mp_cpp& v)
    {
      return mp_cpp(x) /= v;
    }

    // Global binary add/sub/mul/div of const mp_cpp op mp_cpp.
    inline mp_cpp operator+(const mp_cpp& u, const mp_cpp& v) { return mp_cpp(u) += v; }
    inline mp_cpp operator-(const mp_cpp& u, const mp_cpp& v) { return mp_cpp(u) -= v; }
    inline mp_cpp operator*(const mp_cpp& u, const mp_cpp& v) { return mp_cpp(u) *= v; }
    inline mp_cpp operator/(const mp_cpp& u, const mp_cpp& v) { return mp_cpp(u) /= v; }

    // Global unary add/sub/mul/div of mp_cpp op= POD.
    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp& operator+=(mp_cpp& u, const ArithmeticType& x)
    {
      return u += mp_cpp(x);
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp& operator-=(mp_cpp& u, const ArithmeticType& x)
    {
      return u -= mp_cpp(x);
    }

    template<typename SignedIntegralType,
             typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                     && (std::numeric_limits<SignedIntegralType>::is_signed == true)>::type const* = nullptr>
    mp_cpp& operator*=(mp_cpp& u, const SignedIntegralType& n)
    {
      return u.mul_by_int(n);
    }

    template<typename UnsignedIntegralType,
             typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                     && (std::numeric_limits<UnsignedIntegralType>::is_signed == false)>::type const* = nullptr>
    mp_cpp& operator*=(mp_cpp& u, const UnsignedIntegralType& n)
    {
      return u.mul_by_int(static_cast<std::int64_t>(n));
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    mp_cpp& operator*=(mp_cpp& u, const FloatingPointType& f)
    {
      return u *= mp_cpp(f);
    }

    template<typename SignedIntegralType,
             typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                     && (std::numeric_limits<SignedIntegralType>::is_signed == true)>::type const* = nullptr>
    mp_cpp& operator/=(mp_cpp& u, const SignedIntegralType& n)
    {
      return u.div_by_int(n);
    }

    template<typename UnsignedIntegralType,
             typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                     && (std::numeric_limits<UnsignedIntegralType>::is_signed == false)>::type const* = nullptr>
    mp_cpp& operator/=(mp_cpp& u, const UnsignedIntegralType& n)
    {
      return u.div_by_int(static_cast<std::int64_t>(n));
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    mp_cpp& operator/=(mp_cpp& u, const FloatingPointType& f)
    {
      return u /= mp_cpp(f);
    }

    // Global unary add/sub/mul/div of POD op= mp_cpp.
    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator+=(const ArithmeticType& x, const mp_cpp& v)
    {
      return mp_cpp(x) += v;
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator-=(const ArithmeticType& x, const mp_cpp& v)
    {
      return mp_cpp(x) -= v;
    }

    template<typename SignedIntegralType,
             typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                     && (std::numeric_limits<SignedIntegralType>::is_signed == true)>::type const* = nullptr>
    mp_cpp operator*=(const SignedIntegralType& n, const mp_cpp& v)
    {
      return mp_cpp(v).mul_by_int(n);
    }

    template<typename UnsignedIntegralType,
             typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                     && (std::numeric_limits<UnsignedIntegralType>::is_signed == false)>::type const* = nullptr>
    mp_cpp operator*=(const UnsignedIntegralType& n, const mp_cpp& v)
    {
      return mp_cpp(v).mul_by_int(static_cast<std::int64_t>(n));
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    mp_cpp operator*=(const FloatingPointType& f, const mp_cpp& v)
    {
      return mp_cpp(f) /= v;
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
    mp_cpp operator/=(const ArithmeticType& x, const mp_cpp& v)
    {
      return mp_cpp(x) /= v;
    }

    // Binary comparison operators mp_cpp cmp. with mp_cpp.
    inline bool operator< (const mp_cpp& u, const mp_cpp& v) { return (u.compare(v) <  static_cast<std::int32_t>(0)); }
    inline bool operator<=(const mp_cpp& u, const mp_cpp& v) { return (u.compare(v) <= static_cast<std::int32_t>(0)); }
    inline bool operator==(const mp_cpp& u, const mp_cpp& v) { return (u.compare(v) == static_cast<std::int32_t>(0)); }
    inline bool operator!=(const mp_cpp& u, const mp_cpp& v) { return (u.compare(v) != static_cast<std::int32_t>(0)); }
    inline bool operator>=(const mp_cpp& u, const mp_cpp& v) { return (u.compare(v) >= static_cast<std::int32_t>(0)); }
    inline bool operator> (const mp_cpp& u, const mp_cpp& v) { return (u.compare(v) >  static_cast<std::int32_t>(0)); }

    // Binary comparison operators mp_cpp cmp. with POD.
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator< (const mp_cpp& u, const ArithmeticType& x) { return (u <  mp_cpp(x)); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator<=(const mp_cpp& u, const ArithmeticType& x) { return (u <= mp_cpp(x)); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator==(const mp_cpp& u, const ArithmeticType& x) { return (u == mp_cpp(x)); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator!=(const mp_cpp& u, const ArithmeticType& x) { return (u != mp_cpp(x)); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator>=(const mp_cpp& u, const ArithmeticType& x) { return (u >= mp_cpp(x)); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator> (const mp_cpp& u, const ArithmeticType& x) { return (u >  mp_cpp(x)); }

    // Binary comparison operators POD cmp. with mp_cpp.
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator< (const ArithmeticType& x, const mp_cpp& v) { return (mp_cpp(x) <  v); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator<=(const ArithmeticType& x, const mp_cpp& v) { return (mp_cpp(x) <= v); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator==(const ArithmeticType& x, const mp_cpp& v) { return (mp_cpp(x) == v); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator!=(const ArithmeticType& x, const mp_cpp& v) { return (mp_cpp(x) != v); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator>=(const ArithmeticType& x, const mp_cpp& v) { return (mp_cpp(x) >= v); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> bool operator> (const ArithmeticType& x, const mp_cpp& v) { return (mp_cpp(x) >  v); }

    // I/O stream operators.
    template<typename char_type, typename traits_type>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& __my_istream, mp_cpp& __my_x)
    {
      std::string __my_str;

      __my_istream >> __my_str;

      __my_x.read_string(__my_str.c_str());

      return __my_istream;
    }

    template<class char_type, class traits_type>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& __my_ostream, const mp_cpp& __my_x)
    {
      std::basic_ostringstream<char_type, traits_type> __my_tmp_ostream;

      __my_tmp_ostream.flags    (__my_ostream.flags());
      __my_tmp_ostream.imbue    (__my_ostream.getloc());
      __my_tmp_ostream.precision(__my_ostream.precision());

      std::string __my_str;

      const bool write_result =
        __my_x.write_string(__my_str,
                            __my_tmp_ostream.precision(),
                            __my_tmp_ostream.flags());

      static_cast<void>(write_result);

      __my_tmp_ostream << __my_str;

      return (__my_ostream << __my_tmp_ostream.str());
    }

    // Multiple precision elementary and transcendental functions.

    mp_cpp binomial(const std::int32_t n, const std::int32_t k);
    mp_cpp floor   (const mp_cpp& x);
    mp_cpp ceil    (const mp_cpp& x);
    mp_cpp frexp   (const mp_cpp& x, std::int64_t* expptr);
    mp_cpp ldexp   (const mp_cpp& x, std::int64_t  exp_value);
    mp_cpp pow2    (const std::int64_t p);
    mp_cpp rootn   (const mp_cpp& x, const std::int32_t p);
    mp_cpp sin     (const mp_cpp& x);
    mp_cpp cos     (const mp_cpp& x);
    mp_cpp tan     (const mp_cpp& x);
    void   sincos  (const mp_cpp& x, mp_cpp& s, mp_cpp& c);
    mp_cpp asin    (const mp_cpp& x);
    mp_cpp acos    (const mp_cpp& x);
    mp_cpp atan    (const mp_cpp& x);
    mp_cpp atan2   (const mp_cpp& y, const mp_cpp& x);
    mp_cpp sinh    (const mp_cpp& x);
    mp_cpp cosh    (const mp_cpp& x);
    mp_cpp tanh    (const mp_cpp& x);
    void   sinhcosh(const mp_cpp& x, mp_cpp& sh, mp_cpp& ch);
    mp_cpp asinh   (const mp_cpp& x);
    mp_cpp acosh   (const mp_cpp& x);
    mp_cpp atanh   (const mp_cpp& x);
    mp_cpp pow     (const mp_cpp& x, const mp_cpp& a);

    template<typename UnsignedIntegralType,
             typename std::enable_if<(   (std::is_fundamental<UnsignedIntegralType>::value == true)
                                      && (std::is_integral   <UnsignedIntegralType>::value == true)
                                      && (std::is_unsigned   <UnsignedIntegralType>::value == true))>::type const* = nullptr>
    mp_cpp pow(const mp_cpp& d, const UnsignedIntegralType& p)
    {
      return mp::detail::pown_template<mp_cpp, UnsignedIntegralType>(d, p);
    }

    template<typename SignedIntegralType,
             typename std::enable_if<(   (std::is_fundamental<SignedIntegralType>::value == true)
                                      && (std::is_integral   <SignedIntegralType>::value == true)
                                      && (std::is_signed     <SignedIntegralType>::value == true))>::type const* = nullptr>
    mp_cpp pow(const mp_cpp& d, const SignedIntegralType& p)
    {
      return ((p < 0) ? mp::one() / mp::detail::pown_template<mp_cpp, SignedIntegralType>(d, -p)
                      :             mp::detail::pown_template<mp_cpp, SignedIntegralType>(d,  p));
    }

    inline mp_cpp  factorial(const std::int32_t n)                  { return mp_cpp::calculate_factorial(n); }
    inline mp_cpp  fabs     (const mp_cpp& x)                       { return (((x.isneg)() == false) ? x : -x); }
    inline int     sgn      (const mp_cpp& x)                       { return (((x.iszero)() == true) ? 0 : (((x.isneg)() == true) ? -1 : 1)); }
    inline mp_cpp  sqrt     (const mp_cpp& x)                       { return  mp_cpp(x).calculate_sqrt(); }
    inline mp_cpp  cbrt     (const mp_cpp& x)                       { return  rootn(x, static_cast<std::int32_t>(3)); }
    inline mp_cpp  log      (const mp_cpp& x)                       { return  mp_cpp(x).calculate_log(0, zero()); }
    inline mp_cpp  log10    (const mp_cpp& x)                       { return  log(x) / ln10(); }
    inline mp_cpp  loga     (const mp_cpp& x, const mp_cpp& a)      { return  log(x) / log(a); }
    inline mp_cpp  exp      (const mp_cpp& x)                       { return  mp_cpp(x).calculate_exp(); }

    inline bool (isnan)     (const mp_cpp& x) { return (x.isnan)(); }
    inline bool (isinf)     (const mp_cpp& x) { return (x.isinf)(); }
    inline bool (isfinite)  (const mp_cpp& x) { return (x.isfinite)(); }
    inline int  (fpclassify)(const mp_cpp& x) { return (x.fpclassify)(); }

  } // namespace mp

  #include <mp/mp_cpp_limits.h>

#endif // MP_CPP_2002_10_09_H_
