///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FIXED_LENGTH_DYNAMIC_ARRAY_FROM_SCRATCH_2002_10_09_H
  #define FIXED_LENGTH_DYNAMIC_ARRAY_FROM_SCRATCH_2002_10_09_H

  #include <initializer_list>
  #include <iterator>
  #include <memory>

  #include <util/xutils.h>

  // Implement a fixed-length dynamic array with an
  // allocation mechanism in each constructor and
  // deallocation in the destructor.

  namespace util
  {
    template<typename ValueType,
             typename AllocatorType = std::allocator<ValueType>>
    class fixed_length_dynamic_array_from_scratch final
    {
    public:
      // Type definitions.
      using allocator_type         = AllocatorType;
      using size_type              = std::size_t;
      using difference_type        = std::ptrdiff_t;
      using value_type             = ValueType;
      using pointer                = value_type*;
      using const_pointer          = const value_type*;
      using reference              = value_type&;
      using const_reference        = const value_type&;
      using iterator               = pointer;
      using const_iterator         = const_pointer;
      using reverse_iterator       = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;

      // Constructors.
      explicit fixed_length_dynamic_array_from_scratch(const size_type __other_count)
        : __my_count(__other_count),
          __my_elems(allocator_type().allocate(__my_count))
      {
        xutils::xfill(__my_elems,
                      __my_elems + __my_count,
                      value_type());
      }

      fixed_length_dynamic_array_from_scratch(const size_type __other_count,
                                              const value_type& __init_value)
        : __my_count(__other_count),
          __my_elems(allocator_type().allocate(__my_count))
      {
        xutils::xfill(__my_elems,
                      __my_elems + __my_count,
                      __init_value);
      }

      fixed_length_dynamic_array_from_scratch(const size_type __other_count,
                                              const value_type& __init_value,
                                              const allocator_type& __other_alloc)
        : __my_count(__other_count),
          __my_elems(allocator_type(__other_alloc).allocate(__my_count))
      {
        xutils::xfill(__my_elems,
                      __my_elems + __my_count,
                      __init_value);
      }

      fixed_length_dynamic_array_from_scratch(const fixed_length_dynamic_array_from_scratch& __other_array)
        : __my_count(__other_array.__my_count),
          __my_elems(allocator_type().allocate(__other_array.__my_count))
      {
        const iterator __my_copy_result =
          xutils::xcopy(__other_array.cbegin(),
                        __other_array.cend(),
                        begin());

        static_cast<void>(__my_copy_result);
      }

      // Move constructor.
      fixed_length_dynamic_array_from_scratch(fixed_length_dynamic_array_from_scratch&& __other_array)
        : __my_count( __other_array.__my_count),
          __my_elems(&__other_array.__my_elems[0U]) { }

      // Constructor from initializer_list.
      fixed_length_dynamic_array_from_scratch(std::initializer_list<value_type> __init_list,
                                              const allocator_type& __other_alloc = allocator_type())
        : __my_count(__init_list.size()),
          __my_elems(allocator_type(__other_alloc).allocate(__init_list.size()))
      {
        xutils::xcopy(__init_list.cbegin(),
                      __init_list.cend(),
                      begin());
      }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_scratch(OtherIteratorType __other_first,
                                              OtherIteratorType __other_last)
        : __my_count(__other_last - __other_first),
          __my_elems(allocator_type().allocate(__my_count))
      {
        xutils::xcopy(__other_first,
                      __other_last,
                      __my_elems);
      }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_scratch(OtherIteratorType __other_first,
                                              OtherIteratorType __other_last,
                                              const allocator_type& __other_alloc)
        : __my_count(__other_last - __other_first),
          __my_elems(allocator_type(__other_alloc).allocate(__my_count))
      {
        xutils::xcopy(__other_first,
                      __other_last,
                      __my_elems);
      }

      // Destructor.
      ~fixed_length_dynamic_array_from_scratch()
      {
        xutils::xdeallocate_range( begin(),  end(), allocator_type());
      }

      // Iterator access.
      iterator        begin()       { return  __my_elems; }
      const_iterator  begin() const { return  __my_elems; }
      const_iterator cbegin() const { return  __my_elems; }
      iterator        end  ()       { return (__my_elems + __my_count); }
      const_iterator  end  () const { return (__my_elems + __my_count); }
      const_iterator cend  () const { return (__my_elems + __my_count); }

      reverse_iterator        rbegin()       { return reverse_iterator      (__my_elems + __my_count); }
      const_reverse_iterator  rbegin() const { return const_reverse_iterator(__my_elems + __my_count); }
      const_reverse_iterator crbegin() const { return const_reverse_iterator(__my_elems + __my_count); }
      reverse_iterator         rend ()       { return reverse_iterator      (__my_elems); }
      const_reverse_iterator   rend () const { return const_reverse_iterator(__my_elems); }
      const_reverse_iterator  crend () const { return const_reverse_iterator(__my_elems); }

      // Element random access methods, all without range check.
      reference       operator[](const size_type __random_access_index)       { return __my_elems[__random_access_index]; }
      const_reference operator[](const size_type __random_access_index) const { return __my_elems[__random_access_index]; }

      reference       at(const size_type __random_access_index)       { return __my_elems[__random_access_index]; }
      const_reference at(const size_type __random_access_index) const { return __my_elems[__random_access_index]; }

      // Front and back functions.
      reference       front()       { return __my_elems[0U]; }
      const_reference front() const { return __my_elems[0U]; }
      reference       back ()       { return __my_elems[__my_count - 1U]; }
      const_reference back () const { return __my_elems[__my_count - 1U]; }

      // Size functions. (The size is constant.)
      size_type   size    () const { return __my_count; }
      static bool empty   ()       { return false; }
      size_type   max_size() const { return __my_count; }

      // Swap with linear complexity.
      void swap(fixed_length_dynamic_array_from_scratch& __other_array)
      {
        if(this != &__other_array)
        {
          const iterator __my_swap_result =
            xutils::xswap_ranges(begin(),
                                 end(),
                                 __other_array.begin());

          static_cast<void>(__my_swap_result);
        }
      }

      // Assignment.
      fixed_length_dynamic_array_from_scratch& operator=(const fixed_length_dynamic_array_from_scratch& __other_array)
      {
        if(this != &__other_array)
        {
          const iterator __my_copy_result =
            xutils::xcopy(__other_array.cbegin(),
                          __other_array.cend(),
                          begin());

          static_cast<void>(__my_copy_result);
        }

        return *this;
      }

      // Assignment with type conversion.
      template<typename OtherValueType>
      fixed_length_dynamic_array_from_scratch& operator=(const fixed_length_dynamic_array_from_scratch<OtherValueType>& __other_array)
      {
        const iterator __my_copy_result =
          xutils::xcopy(__other_array.cbegin(),
                        __other_array.cend(),
                        begin());

        static_cast<void>(__my_copy_result);

        return *this;
      }

      // Assign a single value to all elements.
      void assign(const value_type& __value_fill)
      {
        xutils::xfill(__my_elems,
                      __my_elems + __my_count,
                      __value_fill);
      }

    private:
      const size_type __my_count;
      pointer         __my_elems;

      // Since the size of fixed_length_dynamic_array_from_scratch must be
      // known at construction time, the following constructors
      // are not intended to be used. They are private and deleted.

      fixed_length_dynamic_array_from_scratch() = delete;
      explicit fixed_length_dynamic_array_from_scratch(const allocator_type&) = delete;
    };

    // Implement non-member operator==() and operator!=().
    template<typename ValueType,
             typename AllocatorType>
    bool operator==(const fixed_length_dynamic_array_from_scratch<ValueType, AllocatorType>& a,
                    const fixed_length_dynamic_array_from_scratch<ValueType, AllocatorType>& b)
    {
      return (   (a.size() == b.size())
              && (xutils::xequal(a.cbegin(), a.cend(), b.cbegin()) == true));
    }

    template<typename ValueType,
             typename AllocatorType>
    bool operator!=(const fixed_length_dynamic_array_from_scratch<ValueType, AllocatorType>& a,
                    const fixed_length_dynamic_array_from_scratch<ValueType, AllocatorType>& b)
    {
      return (   (a.size() != b.size())
              || (xutils::xequal(a.cbegin(), a.cend(), b.cbegin()) == false));
    }
  }

#endif // FIXED_LENGTH_DYNAMIC_ARRAY_FROM_SCRATCH_2002_10_09_H
