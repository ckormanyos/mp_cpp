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
    static constexpr auto mp_elem_digits10       = static_cast<std::int32_t> (UINT32_C(        8));
    static constexpr auto mp_default_digits10    = static_cast<std::int32_t> (UINT32_C(     1000));
    static constexpr auto mp_elem_mask           = static_cast<std::uint32_t>(UINT32_C(100000000));
    static constexpr auto mp_elem_mask2          = static_cast<std::uint32_t>(UINT32_C(    10000));
    static constexpr auto mp_elem_fft_min        = static_cast<std::int32_t> ( INT32_C(      300));  // About 2,400 decimal digits.
    static constexpr auto mp_default_fft_threads = static_cast<int>(INT8_C(4));

    ~mp_core();

    auto get_fft(const std::int32_t n) const -> const mp_fft_base*;

  private:
    auto create_mp_core() -> bool;

    struct digit_characteristics_type final : private util::noncopyable
    {
    public:
      digit_characteristics_type(std::int32_t input_digits10)
        : my_digits10      ((input_digits10 < std::numeric_limits<float>::digits10) ? std::numeric_limits<float>::digits10 : input_digits10),
          my_digits10_extra(static_cast<std::int32_t>(static_cast<float>(my_digits10) * 0.15F)),
          my_digits10_tol  (my_digits10 + ((my_digits10_extra < 16) ? 16 : ((my_digits10_extra > 32) ? 32 : my_digits10_extra))),
          my_elem_number   (2 + ((my_digits10_tol / mp_elem_digits10) + (((my_digits10_tol % mp_elem_digits10) != 0) ? 1 : 0))) { }

      constexpr auto mp_digits10    () const noexcept -> std::int32_t { return my_digits10; }
      constexpr auto mp_digits10_tol() const noexcept -> std::int32_t { return my_digits10_tol; }
      constexpr auto mp_elem_number () const noexcept -> std::int32_t { return my_elem_number; }

    private:
      const std::int32_t my_digits10;
      const std::int32_t my_digits10_extra;
      const std::int32_t my_digits10_tol;
      const std::int32_t my_elem_number;
    };

    class mp_core_memory_type final : private util::noncopyable
    {
    public:
      explicit mp_core_memory_type(const std::size_t int_mem_count,
                                   const std::size_t fft_mem_count = static_cast<std::size_t>(UINT8_C(0))) noexcept
        : fft_max_size(fft_mem_count)
      {
        if(fft_mem_count > static_cast<std::size_t>(UINT8_C(0)))
        {
          mem_dbl = new (std::nothrow) double[static_cast<std::size_t>(fft_mem_count * static_cast<std::size_t>(UINT8_C(4)))];

          m_valid = ((mem_dbl != nullptr) && m_valid);
        }

        if(int_mem_count > static_cast<std::size_t>(UINT8_C(0)))
        {
          mem_int = new (std::nothrow) std::uint32_t[int_mem_count];

          m_valid = ((mem_int != nullptr) && m_valid);
        }
      }

      ~mp_core_memory_type()
      {
        if(mem_dbl != nullptr) { delete [] mem_dbl; }
        if(mem_int != nullptr) { delete [] mem_int; }
      }

      auto mem_a   () const noexcept -> double*        { return mem_dbl + static_cast<std::size_t>(fft_max_size * static_cast<std::size_t>(UINT8_C(0))); }
      auto mem_b   () const noexcept -> double*        { return mem_dbl + static_cast<std::size_t>(fft_max_size * static_cast<std::size_t>(UINT8_C(1))); }
      auto mem_buf0() const noexcept -> double*        { return mem_dbl + static_cast<std::size_t>(fft_max_size * static_cast<std::size_t>(UINT8_C(2))); }
      auto mem_buf1() const noexcept -> double*        { return mem_dbl + static_cast<std::size_t>(fft_max_size * static_cast<std::size_t>(UINT8_C(3))); }
      auto mem_n   () const noexcept -> std::uint32_t* { return mem_int; }

      auto is_valid() const noexcept -> bool { return m_valid; }

      mp_core_memory_type() = delete;

    private:
      double*        mem_dbl { nullptr };
      std::uint32_t* mem_int { nullptr };
      bool           m_valid { true };

      const std::size_t fft_max_size;
    };

    std::vector<const mp_fft_base*>  fft_list { };
    mp_core_memory_type*             mp_core_memory { nullptr };

    const digit_characteristics_type digit_characteristics;
    const int                        number_of_fft_threads;
    const bool                       m_valid;

    explicit mp_core(const std::int32_t my_digits10, const int n_fft_threads = 1)
      : digit_characteristics(my_digits10),
        number_of_fft_threads(n_fft_threads),
        m_valid              (create_mp_core()) { }

    static auto forward_set_of_mp_cpp_numeric_limits(const std::int32_t n) -> void;

    auto is_valid() const -> bool { return m_valid; }

    friend class mp_base;
    friend class mp_cpp;
  };

#endif // MP_CORE_2002_10_09_H
