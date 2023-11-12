///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mp/mp_fft/mp_fft_fftw2.h>
#include <mp/mp_fft/mp_fft_fftw2_protos.h>

mp::mp_fft_fftw2::~mp_fft_fftw2()
{
  ::rfftw_destroy_plan(static_cast<::rfftw_plan>(fwd_fft_params_1));
  ::rfftw_destroy_plan(static_cast<::rfftw_plan>(fwd_fft_params_2));
  ::rfftw_destroy_plan(static_cast<::rfftw_plan>(rev_fft_params));
}

void mp::mp_fft_fftw2::create_fft() const
{
  if(my_fft_n < minimum_size_for_parallel_threads)
  {
    fwd_fft_traits_1.n_threads = 1;
    fwd_fft_traits_2.n_threads = 1;
    rev_fft_traits.n_threads   = 1;
  }

  fwd_fft_params_1 = static_cast<void*>(::rfftw_create_plan(my_fft_n, FFTW_FORWARD,  FFTW_OUT_OF_PLACE | FFTW_ESTIMATE | FFTW_USE_WISDOM));
  fwd_fft_params_2 = static_cast<void*>(::rfftw_create_plan(my_fft_n, FFTW_FORWARD,  FFTW_OUT_OF_PLACE | FFTW_ESTIMATE | FFTW_USE_WISDOM));
  rev_fft_params   = static_cast<void*>(::rfftw_create_plan(my_fft_n, FFTW_BACKWARD, FFTW_OUT_OF_PLACE | FFTW_ESTIMATE | FFTW_USE_WISDOM));
}

void mp::mp_fft_fftw2::forward_1() const
{
  // Forward transform real a to half-complex b.
  (!fwd_fft_traits_1.has_threads())
    ? ::rfftw_one(static_cast<::rfftw_plan>(fwd_fft_params_1),
                  fwd_fft_traits_1.p_in,
                  fwd_fft_traits_1.p_out)
    : ::rfftw_threads_one(fwd_fft_traits_1.n_threads,
                          static_cast<::rfftw_plan>(fwd_fft_params_1),
                          fwd_fft_traits_1.p_in,
                          fwd_fft_traits_1.p_out);
}

void mp::mp_fft_fftw2::forward_2() const
{
  (!fwd_fft_traits_2.has_threads())
    ? ::rfftw_one(static_cast<::rfftw_plan>(fwd_fft_params_2),
                  fwd_fft_traits_2.p_in,
                  fwd_fft_traits_2.p_out)
    : ::rfftw_threads_one(fwd_fft_traits_2.n_threads,
                          static_cast<::rfftw_plan>(fwd_fft_params_2),
                          fwd_fft_traits_2.p_in,
                          fwd_fft_traits_2.p_out);
}

void mp::mp_fft_fftw2::reverse() const
{
  (!rev_fft_traits.has_threads())
    ? ::rfftw_one(static_cast<::rfftw_plan>(rev_fft_params),
                  rev_fft_traits.p_in,
                  rev_fft_traits.p_out)
    : ::rfftw_threads_one(rev_fft_traits.n_threads,
                          static_cast<::rfftw_plan>(rev_fft_params),
                          rev_fft_traits.p_in,
                          rev_fft_traits.p_out);
}
