///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MP_MUL_FFT_2008_11_30_H
  #define MP_MUL_FFT_2008_11_30_H

  #include <cstdint>

  namespace mp
  {
    void mp_fft_multiply(      std::uint32_t*,
                         const std::uint32_t*,
                         const std::int32_t);
  }

#endif // MP_MUL_FFT_2008_11_30_H
