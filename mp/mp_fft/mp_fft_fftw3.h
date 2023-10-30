///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _MP_FFT_FFTW3_2011_06_17_H_
  #define _MP_FFT_FFTW3_2011_06_17_H_

  #include "mp_fft_base.h"

  class mp_fft_fftw3 : public mp_fft_base
  {
  public:
    mp_fft_fftw3(const std::int32_t n,
                 const traits_type& fwd1,
                 const traits_type& fwd2,
                 const traits_type& rev);

    virtual ~mp_fft_fftw3();

  private:
    virtual bool create   ();
    virtual void forward_1() const;
    virtual void forward_2() const;
    virtual void reverse  () const;
  };

#endif // _MP_FFT_FFTW3_2011_06_17_H_
