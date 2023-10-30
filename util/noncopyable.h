///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef NONCOPYABLE_2008_12_01_H_
  #define NONCOPYABLE_2008_12_01_H_

  // Implement a non-copyable private base class,
  // taken and slightly modified from boost::noncopyable.

  namespace util
  {
    class noncopyable
    {
    public:
      ~noncopyable() = default;

    protected:
      noncopyable() = default;

    private:  // emphasize the following members are private
      noncopyable(const noncopyable&) = delete;
      noncopyable& operator=(const noncopyable&) = delete;
    };
  }

#endif // NONCOPYABLE_2008_12_01_H_
