///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_core.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Declaration of mp_core multiple precision class.
// 
// *****************************************************************************

#ifndef MP_CORE_2002_10_09_H
  #define MP_CORE_2002_10_09_H

  #include <cstdint>
  #include <limits>
  #include <memory>
  #include <vector>

  #include <util/noncopyable.h>

  // Forward class declarations.
  namespace mp
  {
    class mp_base;
    class mp_core;
    class mp_cpp;
    class mp_fft_base;
  }

  class mp::mp_core final : private util::noncopyable
  {
  public:
    static const std::int32_t  mp_elem_digits10       = static_cast<std::int32_t> (        8);
    static const std::int32_t  mp_default_digits10    = static_cast<std::int32_t> (     1000);
    static const std::uint32_t mp_elem_mask           = static_cast<std::uint32_t>(100000000U);
    static const std::uint32_t mp_elem_mask2          = static_cast<std::uint32_t>(    10000U);
    static const std::int32_t  mp_elem_fft_min        = static_cast<std::int32_t> (      300);  // About 2,400 decimal digits.
    static const int           mp_default_fft_threads = 4;

    ~mp_core();

    const mp_fft_base* get_fft(const std::int32_t n) const;

  private:
    bool create_mp_core();

    class digit_characteristics_type final : private util::noncopyable
    {
    public:
      digit_characteristics_type(const std::int32_t input_digits10) noexcept
        : my_digits10       ((input_digits10 < std::numeric_limits<float>::digits10) ? std::numeric_limits<float>::digits10 : input_digits10),
          my_digits10_extra (static_cast<std::int32_t>(static_cast<float>(my_digits10) * 0.15F)),
          my_digits10_padded(my_digits10 + ((my_digits10_extra < 16) ? 16 : ((my_digits10_extra > 32) ? 32 : my_digits10_extra))),
          my_elem_number    (2 + ((my_digits10_padded / mp_elem_digits10) + (((my_digits10_padded % mp_elem_digits10) != 0) ? 1 : 0))) { }

      digit_characteristics_type() = delete;

      ~digit_characteristics_type() = default;

      std::int32_t mp_digits10    () const { return my_digits10; }
      std::int32_t mp_digits10_tol() const { return my_digits10_padded; }
      std::int32_t mp_elem_number () const { return my_elem_number; }

    private:
      const std::int32_t my_digits10;
      const std::int32_t my_digits10_extra;
      const std::int32_t my_digits10_padded;
      const std::int32_t my_elem_number;
    };

    class mp_core_memory_type final : private util::noncopyable
    {
    public:
      mp_core_memory_type(const std::size_t int_count,
                          const std::size_t fft_count = 0U) noexcept;

      ~mp_core_memory_type();

      double*        mem_a   () const { return mem_dbl + (fft_max_size * 0U); }
      double*        mem_b   () const { return mem_dbl + (fft_max_size * 1U); }
      double*        mem_buf0() const { return mem_dbl + (fft_max_size * 2U); }
      double*        mem_buf1() const { return mem_dbl + (fft_max_size * 3U); }
      std::uint32_t* mem_n   () const { return mem_int; }

      bool is_valid() const { return m_valid; }

    private:
      const std::size_t fft_max_size;
      double*           mem_dbl;
      std::uint32_t*    mem_int;
      bool              m_valid;

      mp_core_memory_type() = delete;

      mp_core_memory_type(const mp_core_memory_type&) = delete;
      mp_core_memory_type& operator=(const mp_core_memory_type&) = delete;
    };

    const digit_characteristics_type digit_characteristics;
    const int                        number_of_fft_threads;
    std::vector<const mp_fft_base*>  fft_list;
    mp_core_memory_type*             mp_core_memory;
    const bool                       m_valid;

    mp_core(const std::int32_t my_digits10, const int n_fft_threads)
      : digit_characteristics(my_digits10),
        number_of_fft_threads(n_fft_threads),
        fft_list             (),
        mp_core_memory       (nullptr),
        m_valid              (create_mp_core()) { }

    static void forward_set_of_mp_cpp_numeric_limits(const std::int32_t n);

    bool is_valid() const { return m_valid; }

    friend class mp_base;
    friend class mp_cpp;
  };

#endif // MP_CORE_2002_10_09_H
