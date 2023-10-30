///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include "mp_fft_fftw3.h"

mp_fft_fftw3::mp_fft_fftw3(const std::int32_t n,
                           const traits_type& fwd1,
                           const traits_type& fwd2,
                           const traits_type& rev) : mp_fft_base(n, fwd1, fwd2, rev)
{
}

mp_fft_fftw3::~mp_fft_fftw3()
{
  if(get_is_created())
  {
    ::fftw_destroy_plan(static_cast<fftw_plan>(params_fwd_1));
    ::fftw_destroy_plan(static_cast<fftw_plan>(params_fwd_2));
    ::fftw_destroy_plan(static_cast<fftw_plan>(params_rev));
  }
}

bool mp_fft_fftw3::create()
{
  static const int n_init = ::fftw_init_threads();

  static_cast<void>(n_init);

  if(get_is_created() == false)
  {
    if(N < min_size_parallel)
    {
      traits_fwd_1.n_threads = 1;
      traits_fwd_2.n_threads = 1;
      traits_rev.n_threads   = 1;
    }

    if(traits_fwd_1.n_threads > 1)
    {
      ::fftw_plan_with_nthreads(static_cast<int>(traits_fwd_1.n_threads));
    }

    params_fwd_1 = static_cast<void*>(::fftw_plan_r2r_1d(N, traits_fwd_1.p_in, traits_fwd_1.p_out, FFTW_R2HC, FFTW_ESTIMATE));

    if(traits_fwd_2.n_threads > 1)
    {
      ::fftw_plan_with_nthreads(static_cast<int>(traits_fwd_2.n_threads));
    }

    params_fwd_2 = static_cast<void*>(::fftw_plan_r2r_1d(N, traits_fwd_2.p_in, traits_fwd_2.p_out, FFTW_R2HC, FFTW_ESTIMATE));

    if(traits_rev.n_threads > 1)
    {
      ::fftw_plan_with_nthreads(static_cast<int>(traits_rev.n_threads));
    }

    params_rev = static_cast<void*>(::fftw_plan_r2r_1d(N, traits_rev.p_in, traits_rev.p_out, FFTW_HC2R, FFTW_ESTIMATE));

    is_created = true;
  }

  return get_is_created();
}

void mp_fft_fftw3::forward_1() const
{
  // Forward transform real a to half-complex b.
  ::fftw_execute(static_cast<::fftw_plan>(params_fwd_1));
}

void mp_fft_fftw3::forward_2() const
{
  ::fftw_execute(static_cast<::fftw_plan>(params_fwd_2));
}

void mp_fft_fftw3::reverse() const
{
  ::fftw_execute(static_cast<::fftw_plan>(params_rev));
}
