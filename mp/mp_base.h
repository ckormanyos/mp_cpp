///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_base.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Declaration of mp_base multiple precision base class.
// 
// *****************************************************************************

#ifndef MP_BASE_2002_10_09_H
  #define MP_BASE_2002_10_09_H

  #include <cmath>
  #include <cstdint>
  #include <limits>

  #include <mp/mp_core.h>

  // Define either one (but not both) of the following
  // fixed-length dynamic array types. The one from vector
  // uses std::vector as a base class. The one from scratch
  // uses a custom container.

  //#define MP_CPP_USE_FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR
  #define MP_CPP_USE_FIXED_LENGTH_DYNAMIC_ARRAY_FROM_SCRATCH

  #if (   defined(MP_CPP_USE_FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR) \
       && defined(MP_CPP_USE_FIXED_LENGTH_DYNAMIC_ARRAY_FROM_SCRATCH))
    #error The type of fixed-length dynamic array is multiply defined.
  #endif

  #if defined(MP_CPP_USE_FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR)
    #include <util/fixed_length_dynamic_array_from_vector.h>
    using fixed_length_dynamic_array_type =
      util::fixed_length_dynamic_array_from_vector<std::uint32_t>;
  #elif defined(MP_CPP_USE_FIXED_LENGTH_DYNAMIC_ARRAY_FROM_SCRATCH)
    #include <util/fixed_length_dynamic_array_from_scratch.h>
    using fixed_length_dynamic_array_type =
      util::fixed_length_dynamic_array_from_scratch<std::uint32_t>;
  #else
    #error The type of fixed-length dynamic array is undefined.
  #endif

  namespace mp
  {
    class mp_base;

    void mp_fft_multiply(std::uint32_t*, const std::uint32_t*, const std::int32_t);
  }

  // *****************************************************************************
  // Function    : class mp_base
  // 
  // Description : Base class definition for multiple precision floating point
  //               numbers (mp = multiple precision). This class is designed
  //               for a decimal precision range of about 10^1 - 10^8 digits.
  // 
  // *****************************************************************************
  class mp::mp_base
  {
  public:
    typedef enum mp_fpclass_type_enum
    {
      mp_finite,
      mp_inf,
      mp_NaN
    }
    mp_fpclass_type;

    using array_type = fixed_length_dynamic_array_type;
    using value_type = array_type::value_type;

    static bool mp_high_digit_range(const std::int32_t n) { return (n >= static_cast<std::int32_t>(5000)); }

    static std::int32_t mp_digits10    () { return mp_core_instance().digit_characteristics.mp_digits10(); }
    static std::int32_t mp_digits10_tol() { return mp_core_instance().digit_characteristics.mp_digits10_tol(); }
    static std::int32_t mp_elem_number () { return mp_core_instance().digit_characteristics.mp_elem_number(); }

    static const std::int32_t mp_exp_digits10  = static_cast<std::int32_t>(std::numeric_limits<std::int64_t>::digits10);

    virtual ~mp_base() = default;

    bool (iszero)() const
    {
      return (   (my_data[0U] == value_type(0U))
              && (my_data[1U] == value_type(0U)));
    }

    int (fpclassify)() const
    {
      const mp_fpclass_type fpc = fpclass;

      int fpc_result = FP_NAN;

      if(fpc == mp_finite)
      {
        fpc_result = (((iszero)() == true) ? FP_ZERO : FP_NORMAL);
      }
      else if(fpc == mp_NaN)
      {
        ;
      }
      else if(fpc == mp_inf)
      {
        fpc_result = FP_INFINITE;
      }

      return fpc_result;
    }

    bool (isfinite)() const { return (fpclass == mp_finite); }
    bool (isnan)   () const { return (fpclass == mp_NaN); }
    bool (isinf)   () const { return (fpclass == mp_inf); }

          array_type&  representation()       { return my_data; }
    const array_type&  representation() const { return my_data; }
    const array_type& crepresentation() const { return my_data; }

    static bool create_mp_base(const std::int32_t my_digits10   = mp_core::mp_default_digits10,
                               const int          n_fft_threads = mp_core::mp_default_fft_threads);

    void         precision(const std::int32_t n);
    std::int32_t precision() const;

  protected:
    array_type      my_data;
    std::int32_t    prec_elem;
    mp_fpclass_type fpclass;

    // Default constructor.
    mp_base() noexcept : my_data  (mp_elem_number()),
                        prec_elem(mp_elem_number()),
                        fpclass  (mp_finite) { }

    // Copy constructor.
    mp_base(const mp_base& other) noexcept : my_data  (other.my_data),
                                            prec_elem(other.prec_elem),
                                            fpclass  (other.fpclass) { }

    // Move constructor.
    mp_base(mp_base&& other) noexcept : my_data  (static_cast<array_type&&>(other.my_data)),
                                       prec_elem(other.prec_elem),
                                       fpclass  (other.fpclass) { }

    // Constructor from the floating-point class.
    explicit mp_base(const mp_fpclass_type floating_point_class) noexcept
      : my_data  (mp_elem_number()),
        prec_elem(mp_elem_number()),
        fpclass  (floating_point_class) { }

    mp_base& operator=(const mp_base& other)
    {
      if(this != &other)
      {
        my_data   = other.my_data;
        prec_elem = other.prec_elem;
        fpclass   = other.fpclass;
      }

      return *this;
    }

    static const mp_core& mp_core_instance(const std::int32_t my_digits10   = mp_core::mp_default_digits10,
                                           const int          n_fft_threads = mp_core::mp_default_fft_threads);

    std::int32_t compare_data(const array_type& v_data,
                              const std::int32_t my_digits10 = mp_digits10()) const;

    static std::uint_fast32_t order_of_an_element(const value_type& elem)
    {
      // Use a binary search to find the order of an element.
      const std::uint_fast32_t prefix =
        ((elem >= 100000U)
          ? ((elem >= 10000000U) ? 7U : ((elem >= 1000000U) ? 6U : 5U))
          : ((elem >= 1000U) ? ((elem >= 10000U) ? 4U : 3U)
                             : ((elem >= 100U) ? 2U : ((elem >=  10U) ? 1U : 0U))));

      return prefix;
    }

    static void          mul_loop_uv(const std::uint32_t* const u, const std::uint32_t* const v, std::uint32_t* const w, const std::int32_t p);
    static std::uint32_t add_loop_uv(std::uint32_t* const u, const std::uint32_t* const v, const std::int32_t p);
    static std::int32_t  sub_loop_uv(std::uint32_t* const u, const std::uint32_t* const v, const std::int32_t p);
    static std::uint32_t mul_loop_n (std::uint32_t* const u, std::uint32_t n, const std::int32_t p);
    static std::uint32_t div_loop_n (std::uint32_t* const u, std::uint32_t n, const std::int32_t p);

    friend void mp::mp_fft_multiply(std::uint32_t*, const std::uint32_t*, const std::int32_t);
  };

#endif // MP_BASE_2002_10_09_H
