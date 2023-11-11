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
      explicit fixed_length_dynamic_array_from_scratch(const size_type other_count)
        : my_count(other_count),
          my_elems(allocator_type().allocate(my_count))
      {
        xutils::xfill(my_elems, my_elems + my_count, value_type());
      }

      fixed_length_dynamic_array_from_scratch(const size_type other_count,
                                              const value_type& init_value)
        : my_count(other_count),
          my_elems(allocator_type().allocate(my_count))
      {
        xutils::xfill(my_elems, my_elems + my_count, init_value);
      }

      fixed_length_dynamic_array_from_scratch(const size_type other_count,
                                              const value_type& init_value,
                                              const allocator_type& other_alloc)
        : my_count(other_count),
          my_elems(allocator_type(other_alloc).allocate(my_count))
      {
        xutils::xfill(my_elems, my_elems + my_count, init_value);
      }

      fixed_length_dynamic_array_from_scratch(const fixed_length_dynamic_array_from_scratch& other)
        : my_count(other.my_count),
          my_elems(allocator_type().allocate(other.my_count))
      {
        const iterator my_copy_result = xutils::xcopy(other.cbegin(), other.cend(), begin());

        static_cast<void>(my_copy_result);
      }

      // Move constructor.
      fixed_length_dynamic_array_from_scratch(fixed_length_dynamic_array_from_scratch&& other)
        : my_count(other.my_count),
          my_elems(other.my_elems)
      {
        xutils::xdeallocate_range(other.begin(), other.end(), allocator_type());

        other.my_elems = nullptr;
      }

      // Constructor from initializer_list.
      fixed_length_dynamic_array_from_scratch(std::initializer_list<value_type> init_list,
                                              const allocator_type& other_alloc = allocator_type())
        : my_count(init_list.size()),
          my_elems(allocator_type(other_alloc).allocate(init_list.size()))
      {
        auto my_copy_result = xutils::xcopy(init_list.cbegin(), init_list.cend(), begin());

        static_cast<void>(my_copy_result);
      }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_scratch(OtherIteratorType other_first,
                                              OtherIteratorType other_last)
        : my_count(other_last - other_first),
          my_elems(allocator_type().allocate(my_count))
      {
        auto my_copy_result = xutils::xcopy(other_first, other_last, my_elems);

        static_cast<void>(my_copy_result);
      }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_scratch(OtherIteratorType other_first,
                                              OtherIteratorType other_last,
                                              const allocator_type& other_alloc)
        : my_count(other_last - other_first),
          my_elems(allocator_type(other_alloc).allocate(my_count))
      {
        auto my_copy_result = xutils::xcopy(other_first, other_last, my_elems);

        static_cast<void>(my_copy_result);
      }

      // Since the size of fixed_length_dynamic_array_from_scratch must be
      // known at construction time, the following constructors
      // are not intended to be used. They are private and deleted.

      fixed_length_dynamic_array_from_scratch() = delete;
      explicit fixed_length_dynamic_array_from_scratch(const allocator_type&) = delete;

      // Destructor.
      ~fixed_length_dynamic_array_from_scratch()
      {
        if(my_elems != nullptr)
        {
          xutils::xdeallocate_range(begin(), end(), allocator_type());
        }
      }

      // Iterator access.
      iterator        begin()       { return  my_elems; }
      const_iterator  begin() const { return  my_elems; }
      const_iterator cbegin() const { return  my_elems; }
      iterator        end  ()       { return (my_elems + my_count); }
      const_iterator  end  () const { return (my_elems + my_count); }
      const_iterator cend  () const { return (my_elems + my_count); }

      reverse_iterator        rbegin()       { return reverse_iterator      (my_elems + my_count); }
      const_reverse_iterator  rbegin() const { return const_reverse_iterator(my_elems + my_count); }
      const_reverse_iterator crbegin() const { return const_reverse_iterator(my_elems + my_count); }
      reverse_iterator         rend ()       { return reverse_iterator      (my_elems); }
      const_reverse_iterator   rend () const { return const_reverse_iterator(my_elems); }
      const_reverse_iterator  crend () const { return const_reverse_iterator(my_elems); }

      // Element random access methods, all without range check.
      reference       operator[](const size_type random_access_index)       { return my_elems[random_access_index]; }
      const_reference operator[](const size_type random_access_index) const { return my_elems[random_access_index]; }

      reference       at(const size_type random_access_index)       { return my_elems[random_access_index]; }
      const_reference at(const size_type random_access_index) const { return my_elems[random_access_index]; }

      // Front and back functions.
      reference       front()       { return my_elems[static_cast<std::size_t>(UINT8_C(0))]; }
      const_reference front() const { return my_elems[static_cast<std::size_t>(UINT8_C(0))]; }
      reference       back ()       { return my_elems[static_cast<std::size_t>(my_count - static_cast<std::ptrdiff_t>(INT8_C(1)))]; }
      const_reference back () const { return my_elems[static_cast<std::size_t>(my_count - static_cast<std::ptrdiff_t>(INT8_C(1)))]; }

      // Size functions. (The size is constant.)
             constexpr size_type   size    () const { return my_count; }
      static constexpr bool        empty   ()       { return false; }
             constexpr size_type   max_size() const { return my_count; }

      // Swap with linear complexity.
      void swap(fixed_length_dynamic_array_from_scratch& other)
      {
        if(this != &other)
        {
          auto my_swap_result = xutils::xswap_ranges(begin(), end(), other.begin());

          static_cast<void>(my_swap_result);
        }
      }

      // Assignment operator.
      fixed_length_dynamic_array_from_scratch& operator=(const fixed_length_dynamic_array_from_scratch& other)
      {
        if(this != &other)
        {
          auto my_copy_result = xutils::xcopy(other.cbegin(), other.cend(), begin());

          static_cast<void>(my_copy_result);
        }

        return *this;
      }

      // Move assignment operator.
      fixed_length_dynamic_array_from_scratch& operator=(fixed_length_dynamic_array_from_scratch&& other) noexcept
      {
        my_elems = &other.my_elems[0U];

        xutils::xdeallocate_range(other.begin(), other.end(), allocator_type());

        other.my_elems = nullptr;

        return *this;
      }

      // Assignment operator with type conversion.
      template<typename OtherValueType>
      fixed_length_dynamic_array_from_scratch& operator=(const fixed_length_dynamic_array_from_scratch<OtherValueType>& other)
      {
        auto my_copy_result = xutils::xcopy(other.cbegin(), other.cend(), begin());

        static_cast<void>(my_copy_result);

        return *this;
      }

      // Assign a single value to all elements.
      void assign(const value_type& value_fill)
      {
        xutils::xfill(my_elems, my_elems + my_count, value_fill);
      }

    private:
      const size_type my_count;
      pointer         my_elems;
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
