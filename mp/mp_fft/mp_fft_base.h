///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MP_FFT_BASE_2011_06_17_H
  #define MP_FFT_BASE_2011_06_17_H

  #include <algorithm>
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
    static const std::int32_t minimum_size_for_parallel_threads = static_cast<std::int32_t>(8192);

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

    private:
      fft_traits_type() = delete;
    };

    virtual ~mp_fft_base();

    double* p_in_fwd_1() const { return fwd_fft_traits_1.p_in; }
    double* p_in_fwd_2() const { return fwd_fft_traits_2.p_in; }
    double* p_out_rev () const { return rev_fft_traits.p_out; }

    bool get_is_created() const { return is_created; }

    virtual bool create_fft() const = 0;
    virtual void forward_1 () const = 0;
    virtual void forward_2 () const = 0;
    virtual void reverse   () const = 0;

    void convolv() const;

    std::int32_t get_fft_n    () const { return my_fft_n; };
    double       get_fft_scale() const { return my_fft_scale; };

  protected:
    mutable bool            is_created;
    const std::int32_t      my_fft_n;
    const double            my_fft_scale;
    mutable fft_traits_type fwd_fft_traits_1;
    mutable fft_traits_type fwd_fft_traits_2;
    mutable fft_traits_type rev_fft_traits;
    mutable void*           fwd_fft_params_1;
    mutable void*           fwd_fft_params_2;
    mutable void*           rev_fft_params;

    mp_fft_base(const std::int32_t     n,
                const fft_traits_type& fwd_1,
                const fft_traits_type& fwd_2,
                const fft_traits_type& rev);

  private:
    mp_fft_base() = delete;

    mp_fft_base(const mp_fft_base&)  = delete;
    mp_fft_base(const mp_fft_base&&) = delete;

    mp_fft_base& operator=(const mp_fft_base&) = delete;
  };

#endif // MP_FFT_BASE_2011_06_17_H
