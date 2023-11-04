///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2023.
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
// Date        : 2015 - 2023
// 
// Description : Base class for:
//               Provide a backend floating-point type based on mp_cpp
//               that is intended to be used with Boost.Multiprecision.
// 
// *****************************************************************************

#ifndef MP_CPP_BACKEND_BASE_2019_11_17_HPP_
  #define MP_CPP_BACKEND_BASE_2019_11_17_HPP_

  namespace boost { namespace multiprecision { namespace detail {

  // The base class for the mp_cpp_backend backend multiple precision class.
  template<const std::int32_t MyDigits10,
           const int          MyFftThreadCount>
  class mp_cpp_backend_base
  {
  public:
    virtual ~mp_cpp_backend_base() = default;

  protected:
    mp_cpp_backend_base()
    {
      mp_base_is_created = mp::mp_base::create_mp_base(MyDigits10, MyFftThreadCount);
    }

    static bool mp_base_is_created;

  private:
    mp_cpp_backend_base(const mp_cpp_backend_base&) = delete;

    mp_cpp_backend_base& operator=(const mp_cpp_backend_base&) = delete;
  };

  template<const std::int32_t MyDigits10,
           const int          MyFftThreadCount>
  bool mp_cpp_backend_base<MyDigits10, MyFftThreadCount>::mp_base_is_created;

  } } } // namespace boost::multiprecision.:detail

#endif // MP_CPP_BACKEND_BASE_2019_11_17_HPP_
