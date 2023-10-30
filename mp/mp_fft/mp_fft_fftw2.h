///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MP_FFT_FFTW2_2011_06_17_H_
  #define MP_FFT_FFTW2_2011_06_17_H_

  #include <cstdint>

  #include <mp/mp_fft/mp_fft_base.h>

  namespace mp
  {
    class mp_fft_fftw2;
  }

  class mp::mp_fft_fftw2 final : public mp::mp_fft_base
  {
  public:
    mp_fft_fftw2(const std::int32_t     n,
                 const fft_traits_type& fwd_1,
                 const fft_traits_type& fwd_2,
                 const fft_traits_type& rev);

    virtual ~mp_fft_fftw2();

  private:
    virtual bool create_fft() const override;
    virtual void forward_1 () const override;
    virtual void forward_2 () const override;
    virtual void reverse   () const override;

    mp_fft_fftw2() = delete;
  };

#endif // MP_FFT_FFTW2_2011_06_17_H_
