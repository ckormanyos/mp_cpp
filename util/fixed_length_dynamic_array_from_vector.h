///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR_2017_01_08_H
  #define FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR_2017_01_08_H

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
      explicit fixed_length_dynamic_array_from_vector(const size_type other_count)
        : base_class_type(other_count) { }

      fixed_length_dynamic_array_from_vector(const size_type other_count,
                                             const value_type& init_value)
        : base_class_type(other_count, init_value) { }

      fixed_length_dynamic_array_from_vector(const size_type other_count,
                                             const value_type& init_value,
                                             const allocator_type& other_alloc)
        : base_class_type(other_count, init_value, other_alloc) { }

      fixed_length_dynamic_array_from_vector(const fixed_length_dynamic_array_from_vector& other_array)
        : base_class_type(static_cast<const base_class_type&>(other_array)) { }

      fixed_length_dynamic_array_from_vector(const fixed_length_dynamic_array_from_vector& other_array,
                                             const allocator_type& other_alloc)
        : base_class_type(static_cast<const base_class_type&>(other_array), other_alloc) { }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_vector(OtherIteratorType other_first,
                                             OtherIteratorType other_last)
        : base_class_type(other_first, other_last) { }

      template<typename OtherIteratorType>
      fixed_length_dynamic_array_from_vector(OtherIteratorType other_first,
                                             OtherIteratorType other_last,
                                             const allocator_type& other_alloc)
        : base_class_type(other_first, other_last, other_alloc) { }

      // Move constructors.
      fixed_length_dynamic_array_from_vector(fixed_length_dynamic_array_from_vector&& other_array)
        : base_class_type(static_cast<base_class_type&&>(other_array)) { }

      fixed_length_dynamic_array_from_vector(fixed_length_dynamic_array_from_vector&& other_array,
                                             const allocator_type& other_alloc)
        : base_class_type(static_cast<base_class_type&&>(other_array), other_alloc) { }

      // Constructor from initializer_list.
      fixed_length_dynamic_array_from_vector(std::initializer_list<value_type> init_list,
                                             const allocator_type& other_alloc = allocator_type())
        : base_class_type(init_list, other_alloc) { }

      // Destructor.
      virtual ~fixed_length_dynamic_array_from_vector();

      // Assignment operator.
      auto operator=(const fixed_length_dynamic_array_from_vector& other_array) -> fixed_length_dynamic_array_from_vector&
      {
        static_cast<void>(base_class_type::operator=(static_cast<const base_class_type&>(other_array)));

        return *this;
      }

      // Move assignment operator.
      auto operator=(fixed_length_dynamic_array_from_vector&& other_array) noexcept -> fixed_length_dynamic_array_from_vector&
      {
        static_cast<void>(base_class_type::operator=(static_cast<base_class_type&&>(other_array)));

        return *this;
      }

      // Assignment from initializer_list.
      auto operator=(std::initializer_list<value_type> init_list) -> fixed_length_dynamic_array_from_vector&
      {
        static_cast<void>(base_class_type::operator=(init_list));

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
    auto operator==(const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& a,
                    const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& b) noexcept -> bool
    {
      return ((a.size() == b.size()) && (a == b));
    }

    template<typename ValueType,
             typename AllocatorType>
    auto operator!=(const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& a,
                    const fixed_length_dynamic_array_from_vector<ValueType, AllocatorType>& b) noexcept -> bool
    {
      return ((a.size() != b.size()) || (a != b));
    }
  }

#endif // FIXED_LENGTH_DYNAMIC_ARRAY_FROM_VECTOR_2017_01_08_H
