///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_detail.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Header for multiple precision details.
// 
// *****************************************************************************

#ifndef MP_DETAIL_2016_11_06_H
  #define MP_DETAIL_2016_11_06_H

  #include <cstdint>
  #include <ios>
  #include <iterator>
  #include <string>

  namespace mp { class mp_cpp; }

  namespace mp { namespace detail {

  std::int32_t      compute_pow2_for_agm_log  (const mp::mp_cpp& x, const std::int32_t my_digits10);
  void              compute_fast_exp_string   (const mp::mp_cpp& x,       std::string& str_exp_value);
  double            compute_fast_log_value    (const mp::mp_cpp& x);
  const mp::mp_cpp& maximum_argument_for_exp  ();
  bool              format_float_string       (      std::string& str,
                                               const std::int64_t my_exp_value,
                                               const std::streamsize my_digits10,
                                               const std::ios::fmtflags format_flags,
                                               const bool is_neg,
                                               const bool string_value_is_zero);
  bool              check_close_representation(const mp::mp_cpp& a,
                                               const mp::mp_cpp& b,
                                               const std::int32_t tol_elems,
                                                     std::int32_t& sig_elems);

  template<typename InputIteratorLeftType,
           typename InputIteratorRightType>
  auto compare_ranges(      InputIteratorLeftType  pa,
                            InputIteratorRightType pb,
                      const std::uint_fast32_t     count) -> std::int_fast8_t
  {
    std::int_fast8_t n_return = 0;

    using difference_left_type = typename std::iterator_traits<InputIteratorLeftType>::difference_type;

    InputIteratorRightType it_a_end(pa + static_cast<difference_left_type>(count));

    while(pa != it_a_end) // NOLINT(altera-id-dependent-backward-branch)
    {
      using value_left_type = typename std::iterator_traits<InputIteratorLeftType>::value_type;

      const auto value_a = *pa++;
      const auto value_b = static_cast<value_left_type>(*pb++);

      if(value_a != value_b)
      {
        n_return =
          static_cast<std::int_fast8_t>
          (
            (value_a > value_b) ? static_cast<std::int_fast8_t>(INT8_C(1))
                                : static_cast<std::int_fast8_t>(INT8_C(-1))
          );

        break;
      }
    }

    return n_return;
  }

  } } // namespace mp::detail

#endif // MP_DETAIL_2016_11_06_H
