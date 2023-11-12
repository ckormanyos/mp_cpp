///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MP_FFT_BASE_2011_06_17_H
  #define MP_FFT_BASE_2011_06_17_H

  #include <algorithm>
  #include <cstddef>
  #include <cstdint>

  namespace mp
  {
    class mp_fft_base;
  }

  // *****************************************************************************
  // Function    : class mp_fft_base
  // 
  // Description : Wrapper class for FFTs.
  // 
  // *****************************************************************************
  class mp::mp_fft_base
  {
  public:
    static constexpr auto minimum_size_for_parallel_threads =
      static_cast<std::int32_t>
      (
          static_cast<std::int32_t>(INT16_C(8192))
        + static_cast<std::int32_t>(INT16_C(4096))
      );

    struct fft_traits_type final
    {
    public:
      double* p_in;
      double* p_out;
      int     n_threads;

      fft_traits_type(double*   i,
                      double*   o,
                      const int n) : p_in     (i),
                                     p_out    (o),
                                     n_threads(n) { }

      fft_traits_type(const fft_traits_type& other_fft_traits)
        : p_in     (other_fft_traits.p_in),
          p_out    (other_fft_traits.p_out),
          n_threads((std::max)(1, other_fft_traits.n_threads)) { }

      fft_traits_type(const fft_traits_type&& other_fft_traits)
        : p_in     (other_fft_traits.p_in),
          p_out    (other_fft_traits.p_out),
          n_threads((std::max)(1, other_fft_traits.n_threads)) { }

      auto has_threads() const -> bool { return (n_threads > 1); }

      fft_traits_type() = delete;
    };

    virtual ~mp_fft_base() = default;

    double* p_in_fwd_1() const { return fwd_fft_traits_1.p_in; }
    double* p_in_fwd_2() const { return fwd_fft_traits_2.p_in; }
    double* p_out_rev () const { return rev_fft_traits.p_out; }

    virtual void forward_1 () const = 0;
    virtual void forward_2 () const = 0;
    virtual void reverse   () const = 0;

    void convolv() const
    {
      // Convolution a *= b: Format is half-complex which means
      // that the upper-half elements contain the complex amplitudes.
      auto ptr_a = fwd_fft_traits_1.p_out;
      auto ptr_b = fwd_fft_traits_2.p_out;

      for (auto   i = static_cast<std::uint_fast32_t>(UINT8_C(1));
                  i < static_cast<std::uint_fast32_t>(my_fft_n / 2);
                ++i)
      {
        const double u = ptr_a[i];

        const auto n_minus_i =
          static_cast<std::uint_fast32_t>
          (
            static_cast<std::uint_fast32_t>(my_fft_n) - i
          );

        ptr_a[i]         = (u                * ptr_b[i]) - (ptr_a[n_minus_i] * ptr_b[n_minus_i]);
        ptr_a[n_minus_i] = (ptr_a[n_minus_i] * ptr_b[i]) + (u                * ptr_b[n_minus_i]);
      }

      const auto n_half = static_cast<std::size_t>(my_fft_n / 2);

      ptr_a[n_half]                               *= ptr_b[n_half];                               // Element [my_fft_n / 2].
      ptr_a[static_cast<std::size_t>(UINT8_C(0))] *= ptr_b[static_cast<std::size_t>(UINT8_C(0))]; // Element [0].
    }

    std::int32_t get_fft_n    () const { return my_fft_n; };
    double       get_fft_scale() const { return my_fft_scale; };

  protected:
    const std::int32_t      my_fft_n;
    const double            my_fft_scale;
    mutable fft_traits_type fwd_fft_traits_1;
    mutable fft_traits_type fwd_fft_traits_2;
    mutable fft_traits_type rev_fft_traits;
    mutable void*           fwd_fft_params_1 { nullptr };
    mutable void*           fwd_fft_params_2 { nullptr };
    mutable void*           rev_fft_params   { nullptr };

    mp_fft_base(const std::int32_t     n,
                const fft_traits_type& fwd_1,
                const fft_traits_type& fwd_2,
                const fft_traits_type& rev)
      : my_fft_n(n),
        my_fft_scale(1.0 / static_cast<double>(my_fft_n)),
        fwd_fft_traits_1(fwd_1),
        fwd_fft_traits_2(fwd_2),
        rev_fft_traits(rev) { }

  private:
    mp_fft_base() = delete;

    mp_fft_base(const mp_fft_base&) = delete;
    mp_fft_base(const mp_fft_base&&) noexcept = delete;

    mp_fft_base& operator=(const mp_fft_base&) = delete;
    mp_fft_base& operator=(mp_fft_base&&) noexcept = delete;
  };

#endif // MP_FFT_BASE_2011_06_17_H
