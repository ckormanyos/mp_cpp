///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef XUTILS_2002_10_09_H_
  #define XUTILS_2002_10_09_H_

  #include <iterator>

  namespace xutils
  {
    template<typename value_type>
    const value_type& xmin(const value_type& __value_a, const value_type& __value_b)
    {
      return ((__value_a < __value_b) ? __value_a : __value_b);
    }

    template<typename value_type, typename binary_predicate>
    const value_type& xmin(const value_type& __value_a, const value_type& __value_b, binary_predicate __cmp)
    {
      return (*__cmp)(__value_a, __value_b);
    }

    template<typename value_type>
    const value_type& xmax(const value_type& __value_a, const value_type& __value_b)
    {
      return ((__value_a > __value_b) ? __value_a : __value_b);
    }

    template<typename value_type, typename binary_predicate>
    const value_type& xmax(const value_type& __value_a, const value_type& __value_b, binary_predicate __cmp)
    {
      return (*__cmp)(__value_a, __value_b);
    }

    template<typename input_iterator1, typename input_iterator2>
    inline bool xequal(input_iterator1 __first1, input_iterator1 __last1, input_iterator2 __first2)
    {
      typedef typename std::iterator_traits<input_iterator1>::value_type __first_type;

      while(__first1 != __last1)
      {
        if(*__first1 != __first_type(*__first2))
        {
          break;
        }

        ++__first1;
        ++__first2;
      }

      return (__first1 == __last1);
    }

    template<typename input_iterator, typename function_type>
    inline function_type xfor_each(input_iterator __first, input_iterator __last, function_type __func)
    {
      while(__first != __last)
      {
        __func(*__first);

        ++__first;
      }

      return __func;
    }

    template<class input_iterator, class value_type>
    void xfill(input_iterator __first, input_iterator __last, const value_type& __fill_value)
    {
      while(__first != __last)
      {
        *__first = typename std::iterator_traits<input_iterator>::value_type(__fill_value);

        ++__first;
      }
    }

    template<class input_iterator,
             class output_iterator>
    output_iterator xcopy(input_iterator __first, input_iterator __last, output_iterator __copy_result)
    {
      while(__first != __last)
      {
        *__copy_result = typename std::iterator_traits<output_iterator>::value_type(*__first);

        ++__first;
        ++__copy_result;
      }

      return __copy_result;
    }

    template<class input_iterator1, class input_iterator2>
    input_iterator2 xswap_ranges(input_iterator1 __first1, input_iterator1 __last1, input_iterator2 __first2)
    {
      while(__first1 != __last1)
      {
        typedef typename std::iterator_traits<input_iterator1>::value_type __first_type;

        const __first_type __tmp1(*__first1);

        *__first1 = *__first2;
        *__first2 =  __tmp1;

        ++__first1;
        ++__first2;
      }

      return __first2;
    }

    template<typename input_iterator, typename allocator_type>
    void xdestroy_range(input_iterator __first, input_iterator __last, const allocator_type& __alloc)
    {
      while(__first != __last)
      {
        (allocator_type(__alloc)).destroy(__first);

        ++__first;
      }
    }

    template<typename input_iterator, typename allocator_type>
    void xdeallocate_range(input_iterator __first, input_iterator __last, const allocator_type& __alloc)
    {
      (allocator_type(__alloc)).deallocate(__first, std::size_t(__last - __first));
    }
  }

#endif // XUTILS_2002_10_09_H_
