///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_cpp_backend.hpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 2015 - 2023
// 
// Description : Provide a backend floating-point type based on mp_cpp
//               that is intended to be used with Boost.Multiprecision.
// 
// *****************************************************************************

// Example:
//
//  #include <boost/multiprecision/mp_cpp_backend.hpp>
//
//  typedef boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<100>,
//                                        boost::multiprecision::et_off>
//  mp_type;
//
//  const mp_type x(1.25F);
//  const mp_type y(sqrt(x));


#ifndef MP_CPP_BACKEND_2016_05_12_HPP_
  #define MP_CPP_BACKEND_2016_05_12_HPP_

  #include <cstdint>
  #include <limits>
  #include <string>
  #include <type_traits>

  #include <boost/config.hpp>
  #include <boost/multiprecision/number.hpp>
  #include <boost/multiprecision/mp_cpp_backend_base.hpp>

  #include <mp/mp_math.h>

  namespace boost { namespace multiprecision {

  // Forward declaration of the mp_cpp_backend multiple precision class.
  // This class binds native ::mp_cpp to boost::multiprecsion::mp_cpp_backend.
  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  class mp_cpp_backend;

  // Define the number category as a floating-point kind
  // for the mp_cpp_backend. This is needed for properly
  // interacting as a backend with boost::muliprecision.
  template<const std::int32_t MyDigits10,
           const int          MyFftThreadCount>
  struct boost::multiprecision::number_category<mp_cpp_backend<MyDigits10, MyFftThreadCount>>
    : public boost::mpl::int_<boost::multiprecision::number_kind_floating_point> { };

  // This is the mp_cpp_backend multiple precision class.
  template<const std::int32_t MyDigits10,
           const int          MyFftThreadCount = 4>
  class mp_cpp_backend : public detail::mp_cpp_backend_base<MyDigits10, MyFftThreadCount>
  {
  public:
    using signed_types   = std::tuple<signed char, signed short, signed int, signed long, signed long long>;
    using unsigned_types = std::tuple<unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;
    using float_types    = std::tuple<float, double, long double>;
    using exponent_type  = std::int64_t;

    mp_cpp_backend() : m_value() { }

    explicit mp_cpp_backend(const mp::mp_cpp& rep) : m_value(rep) { }

    mp_cpp_backend(const mp_cpp_backend& other) : m_value(other.m_value) { }

    template<typename UnsignedIntegralType,
             typename std::enable_if<(   (std::is_integral<UnsignedIntegralType>::value == true)
                                      && (std::is_unsigned<UnsignedIntegralType>::value == true))>::type const* = nullptr>
    mp_cpp_backend(UnsignedIntegralType u) : m_value(mp::mp_cpp(std::uint64_t(u))) { }

    template<typename SignedIntegralType,
             typename std::enable_if<(   (std::is_integral<SignedIntegralType>::value == true)
                                      && (std::is_signed  <SignedIntegralType>::value == true))>::type const* = nullptr>
    mp_cpp_backend(SignedIntegralType n) : m_value(mp::mp_cpp(std::int64_t(n))) { }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value == true>::type const* = nullptr>
    mp_cpp_backend(FloatingPointType f) : m_value(mp::mp_cpp(static_cast<long double>(f))) { }

    mp_cpp_backend(const char* c) : m_value(c) { }

    mp_cpp_backend(const std::string& str) : m_value(str) { }

    virtual ~mp_cpp_backend() = default;

    mp_cpp_backend& operator=(const mp_cpp_backend& other)
    {
      m_value = other.m_value;

      return *this;
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value == true>::type const* = nullptr>
    mp_cpp_backend& operator=(const ArithmeticType& x)
    {
      m_value = mp::mp_cpp(x);

      return *this;
    }

    mp_cpp_backend& operator=(const std::string& str_rep)  { m_value = mp::mp_cpp(str_rep);  return *this; }
    mp_cpp_backend& operator=(const char*        char_ptr) { m_value = mp::mp_cpp(char_ptr); return *this; }

    void swap(mp_cpp_backend& other_mp_cpp_backend)
    {
      m_value.swap(other_mp_cpp_backend.m_value);
    }

          mp::mp_cpp&  representation()       { return m_value; }
    const mp::mp_cpp&  representation() const { return m_value; }
    const mp::mp_cpp& crepresentation() const { return m_value; }

    std::string str(std::streamsize number_of_digits, const std::ios::fmtflags format_flags) const
    {
      std::string result_str;

      m_value.write_string(result_str, number_of_digits, format_flags);

      return result_str;
    }

    void negate()
    {
      m_value.negate();
    }

    int compare(const mp_cpp_backend& other_mp_cpp_backend) const
    {
      return static_cast<int>(m_value.compare(other_mp_cpp_backend.m_value));
    }

    template<typename ArithmeticType,
             typename std::enable_if<std::is_arithmetic<ArithmeticType>::value == true>::type const* = nullptr>
    int compare(ArithmeticType x)
    {
      return static_cast<int>(m_value.compare(mp::mp_cpp(x)));
    }

  private:
    mp::mp_cpp m_value;

    mp_cpp_backend& operator=(const mp::mp_cpp&) = delete;
  };

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_add(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() += x.crepresentation();
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_subtract(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() -= x.crepresentation();
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_multiply(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() *= x.crepresentation();
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename SignedIntegralType,
           typename std::enable_if<(   (std::is_integral<SignedIntegralType>::value == true)
                                    && (std::is_signed  <SignedIntegralType>::value == true))>::type const* = nullptr>
  void eval_multiply(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result, const SignedIntegralType& n)
  {
    result.representation().mul_by_int(static_cast<std::int64_t>(n));
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_divide(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() /= x.crepresentation();
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename SignedIntegralType,
           typename std::enable_if<(   (std::is_integral<SignedIntegralType>::value == true)
                                    && (std::is_signed  <SignedIntegralType>::value == true))>::type const* = nullptr>
  void eval_divide(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result, const SignedIntegralType& n)
  {
    result.representation().div_by_int(static_cast<std::int64_t>(n));
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  bool eval_eq(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& a, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& b)
  {
    return (a.compare(b) == 0);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename ArithmeticType,
           typename std::enable_if<std::is_arithmetic<ArithmeticType>::value == true>::type const* = nullptr>
  bool eval_eq(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& a, const ArithmeticType& b)
  {
    return (a.compare(b) == 0);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename ArithmeticType,
           typename std::enable_if<std::is_arithmetic<ArithmeticType>::value == true>::type const* = nullptr>
  bool eval_eq(const ArithmeticType& a, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& b)
  {
    return (mp_cpp_backend(a).compare(b) == 0);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  bool eval_gt(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& a, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& b)
  {
    return (a.compare(b) == 1);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename ArithmeticType,
           typename std::enable_if<std::is_arithmetic<ArithmeticType>::value == true>::type const* = nullptr>
  bool eval_gt(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& a, const ArithmeticType& b)
  {
    return (a.compare(b) == 1);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename ArithmeticType,
           typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr>
  bool eval_gt(const ArithmeticType& a, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& b)
  {
    return (mp_cpp_backend(a).compare(b) == 1);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  bool eval_lt(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& a, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& b)
  {
    return (a.compare(b) == -1);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename ArithmeticType,
           typename std::enable_if<std::is_arithmetic<ArithmeticType>::value == true>::type const* = nullptr>
  bool eval_lt(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& a, const ArithmeticType& b)
  {
    return (a.compare(b) == -1);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename ArithmeticType,
           typename std::enable_if<std::is_arithmetic<ArithmeticType>::value == true>::type const* = nullptr>
  bool eval_lt(const ArithmeticType& a, const mp_cpp_backend<MyDigits10, MyFftThreadCount>& b)
  {
    return (mp_cpp_backend(a).compare(b) == -1);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  bool eval_is_zero(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    return (x.crepresentation().iszero)();
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  int eval_get_sign(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    if     ((x.crepresentation().iszero)()) { return  0; }
    else if((x.crepresentation().isneg) ()) { return -1; }
    else                                    { return  1; }
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_convert_to(unsigned long long* result,
                       const mp_cpp_backend<MyDigits10, MyFftThreadCount>& val)
  {
    *result = static_cast<unsigned long long>(mp::mp_cpp::to_uint64(val.crepresentation()));
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_convert_to(signed long long* result,
                       const mp_cpp_backend<MyDigits10, MyFftThreadCount>& val)
  {
    *result = static_cast<signed long long>(mp::mp_cpp::to_int64(val.crepresentation()));
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_convert_to(long double* result,
                       const mp_cpp_backend<MyDigits10, MyFftThreadCount>& val)
  {
    *result = mp::mp_cpp::to_long_double(val.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_frexp(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x,
                  const typename mp_cpp_backend<MyDigits10, MyFftThreadCount>::exponent_type* expptr)
  {
    typedef std::int64_t local_exponent_type;

    local_exponent_type exp2;

    result.representation() = mp::frexp(x.crepresentation(), &exp2);

    *expptr = static_cast<typename mp_cpp_backend<MyDigits10, MyFftThreadCount>::exponent_type>(exp2);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_frexp(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x,
                  int* expptr,
                  typename std::enable_if<std::is_same<typename mp_cpp_backend<MyDigits10, MyFftThreadCount>::exponent_type, int>::value == false>::type const* = nullptr)
  {
    typedef std::int64_t local_exponent_type;

    local_exponent_type exp2;

    result.representation() = mp::frexp(x.crepresentation(), &exp2);

    *expptr = static_cast<int>(exp2);
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_ldexp(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x,
                  const typename mp_cpp_backend<MyDigits10, MyFftThreadCount>::exponent_type exp_value)
  {
    typedef std::int64_t local_exponent_type;

    result.representation() = mp::ldexp(x.crepresentation(), local_exponent_type(exp_value));
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_ldexp(mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x,
                  int exp_value,
                  typename std::enable_if<std::is_same<typename mp_cpp_backend<MyDigits10, MyFftThreadCount>::exponent_type, int>::value == false>::type const* = nullptr)
  {
    typedef std::int64_t local_exponent_type;

    result.representation() = mp::ldexp(x.crepresentation(), local_exponent_type(exp_value));
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_floor(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = mp::floor(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_ceil(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = mp::ceil(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  int eval_fpclassify(const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    return (x.crepresentation().fpclassify)();
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_trunc(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = mp::mp_cpp::integer_part(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_abs(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = x.crepresentation();

    if((result.crepresentation().isneg)())
    {
      result.representation().negate();
    }
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_fabs(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = x.crepresentation();

    if((result.crepresentation().isneg)())
    {
      result.representation().negate();
    }
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_sqrt(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = sqrt(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_sin(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = sin(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_cos(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = cos(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_tan(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = tan(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_asin(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = asin(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_acos(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = acos(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_atan(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = atan(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_atan2(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& y,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = atan2(y.crepresentation(), x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_log(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = log(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_log10(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                  const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = log10(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_exp(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = exp(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_sinh(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = sinh(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_cosh(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = cosh(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_tanh(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x)
  {
    result.representation() = tanh(x.crepresentation());
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_fmod(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x,
                 const mp_cpp_backend<MyDigits10, MyFftThreadCount>& y)
  {
    if((y.crepresentation().iszero)())
    {
      result.representation() = mp::zero();
    }
    else
    {
      // Calculate the fractional part of x such that:
      //   x = (integer_part * y) + fractional_part,
      // where
      //   |fractional_part| < |y|,
      // and fractional_part has the same sign as x.

      const mp::mp_cpp integer_part =
        mp::floor(x.crepresentation() / y.crepresentation());

      result.representation() =
        x.crepresentation() - (integer_part * y.crepresentation());

      if((x.crepresentation().isneg)() != (y.crepresentation().isneg)())
      {
        result.representation() -= y.crepresentation();
      }
    }
  }

  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount>
  void eval_pow(      mp_cpp_backend<MyDigits10, MyFftThreadCount>& result,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& x,
                const mp_cpp_backend<MyDigits10, MyFftThreadCount>& a)
  {
    result.representation() = pow(x.crepresentation(), a.crepresentation());
  }

  } } // namespace boost::multiprecision

  namespace boost { namespace math { namespace policies {

  // Specialization of the precision structure.
  template<const std::int32_t MyDigits10,
           const int MyFftThreadCount,
           typename ThisPolicy,
           const boost::multiprecision::expression_template_option ExpressionTemplates>
  struct precision<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>,
                                                 ExpressionTemplates>,
                   ThisPolicy>
  {
    typedef typename ThisPolicy::precision_type precision_type;

    typedef digits2<((MyDigits10 + 1LL) * 1000LL) / 301LL> local_digits_2;

    typedef typename mpl::if_c
      <
        (   (local_digits_2::value <= precision_type::value)
         || (precision_type::value <= 0)),
        local_digits_2,
        precision_type
      >::type
    type;
  };

  } } } // namespaces boost::math::policies

  namespace std
  {
    template<const std::int32_t MyDigits10,
             const int MyFftThreadCount,
             const boost::multiprecision::expression_template_option ExpressionTemplates>
    class numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>,
                                                       ExpressionTemplates>>
    {
    public:
      BOOST_STATIC_CONSTEXPR bool is_specialized = true;
      BOOST_STATIC_CONSTEXPR bool is_signed      = true;
      BOOST_STATIC_CONSTEXPR bool is_integer     = false;
      BOOST_STATIC_CONSTEXPR bool is_exact       = false;
      BOOST_STATIC_CONSTEXPR bool is_bounded     = true;
      BOOST_STATIC_CONSTEXPR bool is_modulo      = false;
      BOOST_STATIC_CONSTEXPR bool is_iec559      = false;
      BOOST_STATIC_CONSTEXPR int  digits         = MyDigits10;
      BOOST_STATIC_CONSTEXPR int  digits10       = MyDigits10;
      BOOST_STATIC_CONSTEXPR int  max_digits10   = MyDigits10 + 1;

      BOOST_STATIC_CONSTEXPR std::int64_t max_exponent   = std::numeric_limits< ::mp::mp_cpp>::max_exponent;
      BOOST_STATIC_CONSTEXPR std::int64_t max_exponent10 = std::numeric_limits< ::mp::mp_cpp>::max_exponent10;
      BOOST_STATIC_CONSTEXPR std::int64_t min_exponent   = std::numeric_limits< ::mp::mp_cpp>::min_exponent;
      BOOST_STATIC_CONSTEXPR std::int64_t min_exponent10 = std::numeric_limits< ::mp::mp_cpp>::min_exponent10;

      BOOST_STATIC_CONSTEXPR int                     radix             = 10;
      BOOST_STATIC_CONSTEXPR std::float_round_style  round_style       = std::round_indeterminate;
      BOOST_STATIC_CONSTEXPR bool                    has_infinity      = true;
      BOOST_STATIC_CONSTEXPR bool                    has_quiet_NaN     = true;
      BOOST_STATIC_CONSTEXPR bool                    has_signaling_NaN = false;
      BOOST_STATIC_CONSTEXPR std::float_denorm_style has_denorm        = std::denorm_absent;
      BOOST_STATIC_CONSTEXPR bool                    has_denorm_loss   = false;
      BOOST_STATIC_CONSTEXPR bool                    traps             = false;
      BOOST_STATIC_CONSTEXPR bool                    tinyness_before   = false;

      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> (min)        () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::mp_cpp::value_min ()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> (max)        () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::mp_cpp::value_max ()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> lowest       () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::zero()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> epsilon      () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::mp_cpp::value_eps ()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> round_error  () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::half()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> infinity     () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::mp_cpp::value_inf ()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> quiet_NaN    () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::mp_cpp::value_nan ()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> signaling_NaN() { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::zero()); }
      BOOST_STATIC_CONSTEXPR boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates> denorm_min   () { return boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>(mp::zero()); }
    };

    #ifndef BOOST_NO_INCLASS_MEMBER_INITIALIZATION

    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::is_specialized;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::is_signed;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::is_integer;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::is_exact;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::is_bounded;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::is_modulo;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::is_iec559;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST int  std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::digits;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST int  std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::digits10;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST int  std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::max_digits10;

    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST std::int64_t std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::max_exponent;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST std::int64_t std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::max_exponent10;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST std::int64_t std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::min_exponent;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST std::int64_t std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::min_exponent10;

    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST int                     std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::radix;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST std::float_round_style  std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::round_style;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool                    std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::has_infinity;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool                    std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::has_quiet_NaN;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool                    std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::has_signaling_NaN;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST std::float_denorm_style std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::has_denorm;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool                    std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::has_denorm_loss;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool                    std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::traps;
    template<const std::int32_t MyDigits10, const int MyFftThreadCount, const boost::multiprecision::expression_template_option ExpressionTemplates> BOOST_CONSTEXPR_OR_CONST bool                    std::numeric_limits<boost::multiprecision::number<boost::multiprecision::mp_cpp_backend<MyDigits10, MyFftThreadCount>, ExpressionTemplates>>::tinyness_before;

    #endif // !BOOST_NO_INCLASS_MEMBER_INITIALIZATION

  } // namespace std

#endif // MP_CPP_BACKEND_2016_05_12_HPP_
