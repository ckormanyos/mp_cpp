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
    constexpr auto xmin(const value_type& my_value_a, const value_type& my_value_b) -> value_type&
    {
      return ((my_value_a < my_value_b) ? my_value_a : my_value_b);
    }

    template<typename value_type, typename binary_predicate>
    constexpr auto xmin(const value_type& my_value_a, const value_type& my_value_b, binary_predicate my_cmp) -> value_type&
    {
      return (*my_cmp)(my_value_a, my_value_b);
    }

    template<typename value_type>
    constexpr auto xmax(const value_type& my_value_a, const value_type& my_value_b) -> value_type&
    {
      return ((my_value_a > my_value_b) ? my_value_a : my_value_b);
    }

    template<typename value_type, typename binary_predicate>
    constexpr auto xmax(const value_type& my_value_a, const value_type& my_value_b, binary_predicate my_cmp) -> value_type&
    {
      return (*my_cmp)(my_value_a, my_value_b);
    }

    template<typename input_iterator_left, typename input_iterator_right>
    auto xequal(input_iterator_left first_left, input_iterator_left last_left, input_iterator_right first_right) -> bool
    {
      while(first_left != last_left)
      {
        using my_first_type = typename std::iterator_traits<input_iterator_left>::value_type;

        if(*first_left != static_cast<my_first_type>(*first_right))
        {
          break;
        }

        ++first_left;
        ++first_right;
      }

      return (first_left == last_left);
    }

    template<typename input_iterator, typename function_type>
    auto xfor_each(input_iterator first, input_iterator last, function_type func) -> function_type
    {
      while(first != last)
      {
        func(*first);

        ++first;
      }

      return func;
    }

    template<class input_iterator, class value_type>
    auto xfill(input_iterator my_first, input_iterator my_last, const value_type& my_fill_value) -> void
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
    auto xcopy(input_iterator in_first, input_iterator in_last, output_iterator out_first) -> output_iterator
    {
      while(in_first != in_last)
      {
        using local_value_type = typename std::iterator_traits<input_iterator>::value_type;

        *out_first = static_cast<local_value_type>(*in_first);

        ++in_first;
        ++out_first;
      }

      return out_first;
    }

    template<class input_iterator1, class input_iterator2>
    auto xswap_ranges(input_iterator1 my_first1, input_iterator1 my_last1, input_iterator2 my_first2) -> input_iterator2
    {
      while(my_first1 != my_last1)
      {
        std::iter_swap(my_first1, my_first2);

        ++my_first1;
        ++my_first2;
      }

      return my_first2;
    }
  }

#endif // XUTILS_2002_10_09_H
