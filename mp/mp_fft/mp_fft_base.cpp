///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstddef>

#include <mp/mp_fft/mp_fft_base.h>

mp::mp_fft_base::mp_fft_base(const std::int32_t     n,
                             const fft_traits_type& fwd_1,
                             const fft_traits_type& fwd_2,
                             const fft_traits_type& rev) : is_created      (false),
                                                           my_fft_n        (n),
                                                           my_fft_scale    (1.0 / static_cast<double>(my_fft_n)),
                                                           fwd_fft_traits_1(fwd_1),
                                                           fwd_fft_traits_2(fwd_2),
                                                           rev_fft_traits  (rev),
                                                           fwd_fft_params_1(nullptr),
                                                           fwd_fft_params_2(nullptr),
                                                           rev_fft_params  (nullptr) { }

mp::mp_fft_base::~mp_fft_base() { }

void mp::mp_fft_base::convolv() const
{
  // Convolution a *= b: Format is half-complex which means
  // that the upper-half elements contain the complex amplitudes.
  double* a = fwd_fft_traits_1.p_out;
  double* b = fwd_fft_traits_2.p_out;

  for(auto   i = static_cast<std::uint_fast32_t>(UINT8_C(1));
             i < static_cast<std::uint_fast32_t>(my_fft_n / 2);
           ++i)
  {
    const double u  = a[i];

    const auto n_minus_i =
      static_cast<std::uint_fast32_t>
      (
        static_cast<std::uint_fast32_t>(my_fft_n) - i
      );

    a[i]         = (u            * b[i]) - (a[n_minus_i] * b[n_minus_i]);
    a[n_minus_i] = (a[n_minus_i] * b[i]) + (u            * b[n_minus_i]);
  }

  const auto n_half = static_cast<std::size_t>(my_fft_n / 2);

  a[n_half]                               *= b[n_half];                               // Element [my_fft_n / 2].
  a[static_cast<std::size_t>(UINT8_C(0))] *= b[static_cast<std::size_t>(UINT8_C(0))]; // Element [0].
}
