///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR_2017_01_08_H_
  #define FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR_2017_01_08_H_

  #include <initializer_list>
  #include <vector>

  // Implement a fixed-length dynamic array derived
  // from std::vector.

  namespace util
  {
    template<typename ValueType,
             typename AllocatorType = std::allocator<ValueType>>
    class fixed_length_dynamic_array_from_vector final : public std::vector<ValueType, AllocatorType>
    {
    private:
      using base_class_type = std::vector<ValueType, AllocatorType>;

    public:
      using value_type     = typename base_class_type::value_type;
      using size_type      = typename base_class_type::size_type;
      using allocator_type = typename base_class_type::allocator_type;

      // Constructors.
      explicit fixed_length_dynamic_array_from_vector(const size_type __other_count)
        : base_class_type(__other_count) { }

      fixed_length_dynamic_array_from_vector(const size_type __other_count,
                                             const value_type& __init_value)
        : base_class_type(__other_count, __init_value) { }

      fixed_length_dynamic_array_from_vector(const size_type __other_count,
                                             const value_type& __init_value,
                                             const allocator_type& __other_alloc)
        : base_class_type(__other_count, __init_value, __other_alloc) { }

      fixed_length_dynamic_array_from_vector(const fixed_length_dynamic_array_from_vector& __other_array)
        : base_class_type(__other_array) { }

      fixed_length_dynamic_array_from_vector(const fixed_length_dynamic_array_from_vector& __other_array,
                                             const allocator_type& __other_alloc)
        : base_class_type(__other_array, __other_alloc) { }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_vector(OtherIteratorType __other_first,
                                             OtherIteratorType __other_last)
        : base_class_type(__other_first, __other_last) { }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_vector(OtherIteratorType __other_first,
                                             OtherIteratorType __other_last,
                                             const allocator_type& __other_alloc)
        : base_class_type(__other_first, __other_last, __other_alloc) { }

      // Move constructors.
      fixed_length_dynamic_array_from_vector(fixed_length_dynamic_array_from_vector&& __other_array)
        : base_class_type(__other_array) { }

      fixed_length_dynamic_array_from_vector(fixed_length_dynamic_array_from_vector&& __other_array,
                                             const allocator_type& __other_alloc)
        : base_class_type(__other_array, __other_alloc) { }

      // Constructor from initializer_list.
      fixed_length_dynamic_array_from_vector(std::initializer_list<value_type> __init_list,
                                             const allocator_type& __other_alloc = allocator_type())
        : base_class_type(__init_list, __other_alloc) { }

      // Destructor.
      virtual ~fixed_length_dynamic_array_from_vector();

      // Assignment from another fixed_length_dynamic_array_from_vector.
      fixed_length_dynamic_array_from_vector& operator=(const fixed_length_dynamic_array_from_vector& __other_array)
      {
        static_cast<void>(base_class_type::operator=(__other_array));

        return *this;
      }

      // Move assignment.
      fixed_length_dynamic_array_from_vector& operator=(fixed_length_dynamic_array_from_vector&& __other_array)
      {
        static_cast<void>(base_class_type::operator=(__other_array));

        return *this;
      }

      // Assignment from initializer_list.
      fixed_length_dynamic_array_from_vector& operator=(std::initializer_list<value_type> __init_list)
      {
        static_cast<void>(base_class_type::operator=(__init_list));

        return *this;
      }

    private:
      // These constructors are not intended to be used
      // because the size of the fixed-length dynamic array
      // can not be determined at construction time.

      fixed_length_dynamic_array_from_vector() = delete;

      fixed_length_dynamic_array_from_vector(const allocator_type&) = delete;
    };

    // Destructor of fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>.
    template<typename ValueType,
             typename AllocatorType>
    fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>::~fixed_length_dynamic_array_from_vector()
    {
      // This calls the vector base class destructor and relies
      // on vector's memory cleanup.
    }

    // Implement non-member operator==() and operator!=().
    template<typename ValueType,
             typename AllocatorType>
    bool operator==(const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& a,
                    const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& b)
    {
      return ((a.size() == b.size()) && (a == b));
    }

    template<typename ValueType,
             typename AllocatorType>
    bool operator!=(const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& a,
                    const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& b)
    {
      return ((a.size() != b.size()) || (a != b));
    }
  }

#endif // FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR_2017_01_08_H_
