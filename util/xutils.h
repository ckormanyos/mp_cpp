///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef XUTILS_2002_10_09_H
  #define XUTILS_2002_10_09_H

  #include <iterator>

  namespace xutils
  {
    template<typename value_type>
    const value_type& xmin(const value_type& my_value_a, const value_type& my_value_b)
    {
      return ((my_value_a < my_value_b) ? my_value_a : my_value_b);
    }

    template<typename value_type, typename binary_predicate>
    const value_type& xmin(const value_type& my_value_a, const value_type& my_value_b, binary_predicate my_cmp)
    {
      return (*my_cmp)(my_value_a, my_value_b);
    }

    template<typename value_type>
    const value_type& xmax(const value_type& my_value_a, const value_type& my_value_b)
    {
      return ((my_value_a > my_value_b) ? my_value_a : my_value_b);
    }

    template<typename value_type, typename binary_predicate>
    const value_type& xmax(const value_type& my_value_a, const value_type& my_value_b, binary_predicate my_cmp)
    {
      return (*my_cmp)(my_value_a, my_value_b);
    }

    template<typename input_iterator1, typename input_iterator2>
    bool xequal(input_iterator1 my_first1, input_iterator1 my_last1, input_iterator2 my_first2)
    {
      while(my_first1 != my_last1)
      {
        using my_first_type = typename std::iterator_traits<input_iterator1>::value_type;

        if(*my_first1 != static_cast<my_first_type>(*my_first2))
        {
          break;
        }

        ++my_first1;
        ++my_first2;
      }

      return (my_first1 == my_last1);
    }

    template<typename input_iterator, typename function_type>
    function_type xfor_each(input_iterator my_first, input_iterator my_last, function_type my_func)
    {
      while(my_first != my_last)
      {
        my_func(*my_first);

        ++my_first;
      }

      return my_func;
    }

    template<class input_iterator, class value_type>
    void xfill(input_iterator my_first, input_iterator my_last, const value_type& my_fill_value)
    {
      while(my_first != my_last)
      {
        using local_value_type = typename std::iterator_traits<input_iterator>::value_type;

        *my_first = static_cast<local_value_type>(my_fill_value);

        ++my_first;
      }
    }

    template<class input_iterator,
             class output_iterator>
    output_iterator xcopy(input_iterator my_first, input_iterator my_last, output_iterator my_copy_result)
    {
      while(my_first != my_last)
      {
        using local_value_type = typename std::iterator_traits<input_iterator>::value_type;

        *my_copy_result = static_cast<local_value_type>(*my_first);

        ++my_first;
        ++my_copy_result;
      }

      return my_copy_result;
    }

    template<class input_iterator1, class input_iterator2>
    input_iterator2 xswap_ranges(input_iterator1 my_first1, input_iterator1 my_last1, input_iterator2 my_first2)
    {
      while(my_first1 != my_last1)
      {
        std::iter_swap(my_first1, my_first2);

        ++my_first1;
        ++my_first2;
      }

      return my_first2;
    }

    template<typename input_iterator, typename allocator_type>
    void xdeallocate_range(input_iterator my_first, input_iterator my_last, const allocator_type& my_alloc)
    {
      (allocator_type(my_alloc)).deallocate(my_first, static_cast<std::size_t>(static_cast<std::ptrdiff_t>(my_last - my_first)));
    }
  }

#endif // XUTILS_2002_10_09_H
