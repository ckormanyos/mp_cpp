///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <mp/mp_fft/mp_fft_fftw2.h>
#include <mp/mp_fft/mp_fft_fftw2_protos.h>

mp::mp_fft_fftw2::mp_fft_fftw2(const std::int32_t     n,
                               const fft_traits_type& fwd1,
                               const fft_traits_type& fwd2,
                               const fft_traits_type& rev)
  : mp_fft_base(n, fwd1, fwd2, rev) { }

mp::mp_fft_fftw2::~mp_fft_fftw2()
{
  if(get_is_created())
  {
    ::rfftw_destroy_plan(static_cast<::rfftw_plan>(fwd_fft_params_1));
    ::rfftw_destroy_plan(static_cast<::rfftw_plan>(fwd_fft_params_2));
    ::rfftw_destroy_plan(static_cast<::rfftw_plan>(rev_fft_params));
  }
}

bool mp::mp_fft_fftw2::create_fft() const
{
  if(get_is_created() == false)
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

    is_created = true;
  }

  return get_is_created();
}

void mp::mp_fft_fftw2::forward_1() const
{
  // Forward transform real a to half-complex b.
  if(fwd_fft_traits_1.n_threads <= 1)
  {
    ::rfftw_one(static_cast<::rfftw_plan>(fwd_fft_params_1),
                fwd_fft_traits_1.p_in,
                fwd_fft_traits_1.p_out);
  }
  else
  {
    ::rfftw_threads_one(fwd_fft_traits_1.n_threads,
                        static_cast<::rfftw_plan>(fwd_fft_params_1),
                        fwd_fft_traits_1.p_in,
                        fwd_fft_traits_1.p_out);
  }
}

void mp::mp_fft_fftw2::forward_2() const
{
  if(fwd_fft_traits_2.n_threads <= 1)
  {
    ::rfftw_one(static_cast<::rfftw_plan>(fwd_fft_params_2),
                fwd_fft_traits_2.p_in,
                fwd_fft_traits_2.p_out);
  }
  else
  {
    ::rfftw_threads_one(fwd_fft_traits_2.n_threads,
                        static_cast<::rfftw_plan>(fwd_fft_params_2),
                        fwd_fft_traits_2.p_in,
                        fwd_fft_traits_2.p_out);
  }
}

void mp::mp_fft_fftw2::reverse() const
{
  if(rev_fft_traits.n_threads <= 1)
  {
    ::rfftw_one(static_cast<::rfftw_plan>(rev_fft_params),
                rev_fft_traits.p_in,
                rev_fft_traits.p_out);
  }
  else
  {
    ::rfftw_threads_one(rev_fft_traits.n_threads,
                        static_cast<::rfftw_plan>(rev_fft_params),
                        rev_fft_traits.p_in,
                        rev_fft_traits.p_out);
  }
}
