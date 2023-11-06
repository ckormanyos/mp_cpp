///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <thread>

#include <mp/mp_base.h>
#include <mp/mp_fft/mp_fft_base.h>
#include <mp/mp_fft/mp_fft_multiply.h>
#include <util/noncopyable.h>

namespace local
{
  class fwd_parallel final : private util::noncopyable
  {
  public:
    fwd_parallel(const mp::mp_fft_base* f)
      : pfft      (f),
        h_thread_1(std::thread(thread_factory_1, this)),
        h_thread_2(std::thread(thread_factory_2, this)) { }

    ~fwd_parallel();

    static void thread_factory_1(const fwd_parallel* const my_fwd_parallel)
    {
      my_fwd_parallel->pfft->forward_1();
    }

    static void thread_factory_2(const fwd_parallel* const my_fwd_parallel)
    {
      my_fwd_parallel->pfft->forward_2();
    }

    void wait() const
    {
      h_thread_1.join();
      h_thread_2.join();
    }

  private:
    typedef std::thread thread_handle_type;

    const   mp::mp_fft_base*   pfft;
    mutable thread_handle_type h_thread_1;
    mutable thread_handle_type h_thread_2;
  };

  fwd_parallel::~fwd_parallel() { }

} // namespace local

void mp::mp_fft_multiply(std::uint32_t* u, const std::uint32_t* v, const std::int32_t p)
{
  // Multiplication using FFT.
  // The package fftw is used (see http://www.fftw.org/).
  // Empirically tested with up to 10^7 decimal digits
  // with no loss of numerical precision.
  // In this check, the data are in base-10^8.
  // The FFT is carried out in base-10^4.

  // Get a pointer to the FFT object to be used for the multiplication by finding it
  // in the map according to its required length (size2 * 2). The additional factor
  // of two is required since the arrays are stored in half-complex form.
  const std::int32_t two_p = static_cast<std::int32_t>(2 * p);

  const mp_fft_base* p_fft = mp_base::mp_core_instance().get_fft(static_cast<std::int32_t>(2 * two_p));

  double* a = p_fft->p_in_fwd_1();
  double* b = p_fft->p_in_fwd_2();

  // Fill the unused complex array my_data with 0.
  std::fill(a + (two_p - 1), a + p_fft->get_fft_n(), 0.0);
  std::fill(b + (two_p - 1), b + p_fft->get_fft_n(), 0.0);

  // Fill the floating point data arrays with multiplicand and multiplier.
  // The justification puts the most significant digits at the floating point
  // array position 0. The data are split into hi and lo parts due to the
  // change from base 10^8 to base 10^4.

  for(std::uint_fast32_t j = static_cast<std::uint_fast32_t>(0U); j < static_cast<std::uint_fast32_t>(p); ++j)
  {
    const std::uint_fast32_t u_hi = static_cast<std::uint_fast32_t>(u[j] / static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2));

    a[ j * 2U]       = static_cast<double>(u_hi);
    a[(j * 2U) + 1U] = static_cast<double>(u[j] % static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2));

    b[ j * 2U]       = static_cast<double>(v[j] / static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2));
    b[(j * 2U) + 1U] = static_cast<double>(v[j] % static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2));
  }

  // FFT multiplication performs the following steps:
  // 1) Forward transform of mem_a[]-->mem_buf0[]
  // 2) Forward transform of mem_b[]-->mem_buf[1]
  // 3) Convolution: This is a *= b in transform-space,
  //    which amounts to mem_buf[0] *= mem_buf[1] (see steps 1 and 2).
  // 4) Inverse transform storing my_data in mem_a[]

  // Is the FFT large enough to benefit from parallel processing?

  if(p_fft->get_fft_n() >= mp_fft_base::minimum_size_for_parallel_threads)
  {
    // Launch two forward FFTs in parallel threads and wait for completion.

    // Forward real to half-complex FFT: mem_a[]-->mem_buf0[]
    // Forward real to half-complex FFT: mem_b[]-->mem_buf1[]
    const local::fwd_parallel fft_fwd_parallel(p_fft);

    fft_fwd_parallel.wait();
  }
  else
  {
    // Do two sequential forward FFT's.
    p_fft->forward_1();     // Forward real to half-complex FFT: mem_a[]-->mem_buf0[]
    p_fft->forward_2();     // Forward real to half-complex FFT: mem_b[]-->mem_buf1[]
  }

  // Convolution: a *= b: Equivalent to mem_buf0[] *= mem_buf1[]
  p_fft->convolv();

  // Inverse half-complex to real FFT: mem_buf0[] --> mem_a[]
  p_fft->reverse();

  a = p_fft->p_out_rev();

  // Release the carries, re-combine the low and high parts,
  // and set the data elements.
  std::uint64_t carry = static_cast<std::uint64_t>(0U);

  for(std::int_fast32_t j = std::int_fast32_t((p * 2) - 2); j >= std::int_fast32_t(0); j -= static_cast<std::int32_t>(2))
  {
    double                   xaj   = a[j] * p_fft->get_fft_scale();
    const std::uint64_t      xlo   = static_cast<std::uint64_t>(xaj + 0.5) + carry;
                             carry = static_cast<std::uint64_t>(xlo / static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2));
    const std::uint_fast32_t nlo   = static_cast<std::uint_fast32_t>(xlo - static_cast<std::uint64_t>(carry * static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2)));

                             xaj   = ((j > static_cast<std::int_fast32_t>(0)) ? (a[j - 1U] * p_fft->get_fft_scale()) : 0.0);
    const std::uint64_t      xhi   = static_cast<std::uint64_t>(xaj + 0.5) + carry;
                             carry = static_cast<std::uint64_t>(xhi / static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2));
    const std::uint_fast32_t nhi   = static_cast<std::uint_fast32_t>(xhi - static_cast<std::uint64_t>(carry * static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2)));

    u[(j / 2U)] = static_cast<std::uint_fast32_t>(static_cast<std::uint_fast32_t>(nhi * static_cast<std::uint_fast32_t>(mp_core::mp_elem_mask2)) + nlo);
  }
}
