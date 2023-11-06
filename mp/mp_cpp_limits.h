///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_cpp_limits.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Specialization of std::numeric_limits<mp_cpp> for multiple precision math
// 
// *****************************************************************************

#ifndef MP_CPP_LIMITS_2019_02_16_H
  #define MP_CPP_LIMITS_2019_02_16_H

  #include <cstdint>
  #include <limits>

  namespace std
  {
    // Implement a specialization of std::numeric_limits<mp_cpp>.
    template<>
    class numeric_limits<mp::mp_cpp>
    {
    public:
      static const bool         is_specialized = true;
      static const bool         is_signed      = true;
      static const bool         is_integer     = false;
      static const bool         is_exact       = false;
      static const bool         is_bounded     = true;
      static const bool         is_modulo      = false;
      static const bool         is_iec559      = false;
      static       std::int64_t digits;        // Not strictly standard: Not of type int, not known at compile time.
      static       std::int64_t digits10;      // Not strictly standard: Not of type int, not known at compile time.
      static       std::int64_t max_digits10;  // Not strictly standard: Not of type int, not known at compile time.

      static const std::int64_t max_exponent   =  std::int64_t((static_cast<std::uint64_t>(1U) << 63) - 1U);
      static const std::int64_t max_exponent10 = (std::int64_t((static_cast<std::uint64_t>(1U) << 63) - 1U) / static_cast<std::int64_t>(1000)) * static_cast<std::int64_t>(301);
      static const std::int64_t min_exponent   =  std::int64_t(-std::int64_t((static_cast<std::uint64_t>(1U) << 63) - 1U) - 1);
      static const std::int64_t min_exponent10 = (std::int64_t(-std::int64_t((static_cast<std::uint64_t>(1U) << 63) - 1U) - 1) / static_cast<std::int64_t>(1000)) * static_cast<std::int64_t>(301);

      static const int                     radix             = 10;
      static const std::float_round_style  round_style       = std::round_indeterminate;
      static const bool                    has_infinity      = true;
      static const bool                    has_quiet_NaN     = true;
      static const bool                    has_signaling_NaN = false;
      static const std::float_denorm_style has_denorm        = std::denorm_absent;
      static const bool                    has_denorm_loss   = false;
      static const bool                    traps             = false;
      static const bool                    tinyness_before   = false;

      static const mp::mp_cpp& (min)        ();
      static const mp::mp_cpp& (max)        ();
      static const mp::mp_cpp& lowest       ();
      static const mp::mp_cpp& epsilon      ();
      static const mp::mp_cpp& round_error  ();
      static const mp::mp_cpp& infinity     ();
      static const mp::mp_cpp& quiet_NaN    ();
      static const mp::mp_cpp& signaling_NaN();
      static const mp::mp_cpp& denorm_min   ();
    };
  } // namespace std

#endif // MP_CPP_LIMITS_2019_02_16_H
