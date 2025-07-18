///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2025.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_cpp_backend_base.hpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 2015 - 2025
// 
// Description : This file implements the base class mp_cpp_backend_base.
// 
// *****************************************************************************

#ifndef MP_CPP_BACKEND_BASE_2019_11_17_HPP
  #define MP_CPP_BACKEND_BASE_2019_11_17_HPP

  namespace boost { namespace multiprecision { namespace detail {

  // The base class for the mp_cpp_backend backend multiple precision class.
  template<const std::int32_t MyDigits10,
           const int          MyFftThreadCount>
  class mp_cpp_backend_base
  {
  public:
    mp_cpp_backend_base(const mp_cpp_backend_base&) = default;
    mp_cpp_backend_base(mp_cpp_backend_base&&) noexcept = default;

    mp_cpp_backend_base& operator=(const mp_cpp_backend_base&) = default;
    mp_cpp_backend_base& operator=(mp_cpp_backend_base&&) noexcept = default;

    virtual ~mp_cpp_backend_base() = default;

  protected:
    static bool mp_base_is_created;

    mp_cpp_backend_base()
    {
      if(!mp_base_is_created)
      {
        mp::mp_base::create_mp_base(MyDigits10, MyFftThreadCount);
      }
    }
  };

  template<const std::int32_t MyDigits10,
           const int          MyFftThreadCount>
  bool mp_cpp_backend_base<MyDigits10, MyFftThreadCount>::mp_base_is_created { };

  } } } // namespace boost::multiprecision.:detail

#endif // MP_CPP_BACKEND_BASE_2019_11_17_HPP
