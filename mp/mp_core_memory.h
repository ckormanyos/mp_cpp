///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_core_memory.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Declaration of mp_core_memory class.
// 
// *****************************************************************************

#ifndef MP_CORE_MEMORY_2023_11_13_H
  #define MP_CORE_MEMORY_2023_11_13_H

  #include <cstdint>
  #include <memory>

  #include <util/noncopyable.h>

  namespace mp
  {
    class mp_core_memory final : private util::noncopyable
    {
    public:
      explicit mp_core_memory(const std::size_t int_mem_count,
                              const std::size_t fft_mem_count = static_cast<std::size_t>(UINT8_C(0))) noexcept
        : fft_max_size(fft_mem_count)
      {
        if(fft_max_size > static_cast<std::size_t>(UINT8_C(0)))
        {
          mem_dbl = new (std::nothrow) double[static_cast<std::size_t>(fft_max_size * static_cast<std::size_t>(UINT8_C(4)))];

          m_valid = ((mem_dbl != nullptr) && m_valid);
        }

        if(int_mem_count > static_cast<std::size_t>(UINT8_C(0)))
        {
          mem_int = new (std::nothrow) std::uint32_t[int_mem_count];

          m_valid = ((mem_int != nullptr) && m_valid);
        }
      }

      ~mp_core_memory()
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

      mp_core_memory() = delete;

    private:
      double*        mem_dbl { nullptr };
      std::uint32_t* mem_int { nullptr };
      bool           m_valid { true };

      const std::size_t fft_max_size;
    };

    using mp_core_memory_type = mp_core_memory;
  } // namespace mp

#endif // MP_CORE_MEMORY_2023_11_13_H
