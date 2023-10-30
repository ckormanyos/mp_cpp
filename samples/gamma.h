///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : gamma.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Header for calculating sets of Lanczos coefficients.
// 
// *****************************************************************************

#ifndef GAMMA_2019_02_28_H
  #define GAMMA_2019_02_28_H

  #include <algorithm>
  #include <atomic>
  #include <cmath>
  #include <ctime>
  #include <limits>
  #include <memory>
  #include <numeric>
  #include <vector>

  #include <util/noncopyable.h>
  #include <util/parallel_for.h>

  namespace my_gamma { namespace lanczos { namespace detail {

  // *****************************************************************************
  // Function    : template<typename T> class vcr
  // 
  // Description : One dimensional vector template class.
  // 
  // *****************************************************************************
  template<typename T,
           typename Alloc = std::allocator<T>>
  class vcr final : public std::vector<T, Alloc>
  {
  private:
    using base_class_type = std::vector<T, Alloc>;

  public:
    using allocator_type  = Alloc;
    using size_type       = typename std::allocator_traits<allocator_type>::size_type;
    using value_type      = typename std::allocator_traits<allocator_type>::value_type;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    // Constructors.
    vcr() = delete;

    vcr(const size_type length,
        const_reference value = value_type(0U))
      : base_class_type(length, value) { }

    vcr(const vcr& other)
      : base_class_type(other.cbegin(), other.cend()) { }

    vcr(vcr&& other)
      : base_class_type(static_cast<base_class_type&&>(other)) { }

    ~vcr()
    {
      // This destructor is, by design, empty and non-virtual.
    }

    // Assignment operator.
    vcr& operator=(const vcr& other)
    {
      if(this != &other)
      {
        const size_type upper_index = (std::min)(base_class_type::size(), other.size());

        std::copy(other.cbegin(),
                  other.cbegin() + upper_index,
                  base_class_type::begin());

        if(base_class_type::size() > upper_index)
        {
          std::fill(base_class_type::begin() + upper_index,
                    base_class_type::end(),
                    value_type(0U));
        }
      }

      return *this;
    }

    static value_type dot(const vcr& a, const vcr& b)
    {
      // Compute the inner product (i.e., the dot product) of a.dot.b.
      const size_type upper_index = (std::min)(a.size(), b.size());

      return std::inner_product(a.cbegin(),
                                a.cbegin() + upper_index,
                                b.cbegin(),
                                value_type(0U));
    }
  };

  // *****************************************************************************
  // Function    : template<typename T> class mtx
  // 
  // Description : Two-dimensional matrix template class.
  //               The matrix is arranged as a single chunk
  //               of memory allocated with new at creation time.
  //               This memory chunk is divided into rows, each
  //               with as many elements as the number of columns.
  //               The rows and columns are mapped with STL-vectors
  //               of pointers using std::vector<const T*>. This
  //               allows for easy row-column manipulations such as
  //               inner products.
  // 
  // *****************************************************************************
  template<typename T,
           typename Alloc = std::allocator<T>>
  class mtx final
  {
  public:
    using allocator_type  = Alloc;
    using size_type       = typename std::allocator_traits<allocator_type>::size_type;
    using value_type      = typename std::allocator_traits<allocator_type>::value_type;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using reference       = value_type&;
    using const_reference = const value_type&;

    // Constructors.
    mtx() = delete;

    mtx(const size_type length,
        const_reference value = value_type(0U))
      : m_size   (length),
        m_buf    (size() * size()),
        m_x      (size()),
        m_row_ref(size(), vector_of_const_pointer_type(size(), nullptr)),
        m_col_ref(size(), vector_of_const_pointer_type(size(), nullptr))
    {
      std::fill(m_buf.begin(), m_buf.end(), value);

      for(size_type i = 0U; i < size(); ++i)
      {
        pointer ptr = m_buf.data() + (i * size());

        m_x[i] = ptr;
      }

      assign_row_and_column_references();
    }

    // Constructor from another mtx.
    mtx(const mtx& other)
      : m_size   (other.m_size),
        m_buf    (other.m_buf),
        m_x      (size()),
        m_row_ref(size(), vector_of_const_pointer_type(size(), nullptr)),
        m_col_ref(size(), vector_of_const_pointer_type(size(), nullptr))
    {
      for(size_type i = 0U; i < size(); ++i)
      {
        pointer ptr = m_buf.data() + (i * size());

        m_x[i] = ptr;
      }

      assign_row_and_column_references();
    }

    ~mtx() = default;

    // Assignment operator.
    mtx& operator=(const mtx& other)
    {
      if(this != &other)
      {
        const size_type upper_index = (std::min)(m_buf.size(), other.m_buf.size());

        std::copy(other.m_buf.cbegin(),
                  other.m_buf.cbegin() + upper_index,
                  m_buf.begin());

        if(m_buf.size() > upper_index)
        {
          std::fill(m_buf.begin() + upper_index,
                    m_buf.end(),
                    value_type(0U));
        }
      }

      return *this;
    }

    size_type size() const { return m_size; }

    // Access to full rows with range check.
    const_pointer operator[](const size_type i) const { return ((i < size()) ? m_x[i] : cdummy()); }
          pointer operator[](const size_type i)       { return ((i < size()) ? m_x[i] :  dummy()); }

    // Unary multiplication operator.
    mtx& operator*=(const mtx* pv)
    {
      const size_type upper_index = (std::min)(size(), pv->size());

      mtx* w = new mtx(upper_index);

      std::atomic_flag mtx_multiplication_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        size_type(0U),
        upper_index,
        [&mtx_multiplication_lock, &upper_index, this, pv, w](size_type i)
        {
          for(size_type j = 0U; j < upper_index; ++j)
          {
            auto first1 = this->m_row_ref[i].cbegin();
            auto first2 =   pv->m_col_ref[j].cbegin();

            value_type sum(0U);

            for(size_type k = 0U; k < upper_index; ++k)
            {
              const value_type uv((*(*(first1 + k))) * (*(*(first2 + k))));

              while(mtx_multiplication_lock.test_and_set()) { ; }
              sum += uv;
              mtx_multiplication_lock.clear();
            }

            (w->operator[](i))[j] = sum;
          }
        }
      );

      operator=(*w);

      delete w;

      return *this;
    }

    // Matrix-vector multiplication.
    vcr<value_type> operator*(const vcr<value_type>* pv) const
    {
      const size_type upper_index = (std::min)(size(), size_type(pv->size()));

      vcr<value_type> w(upper_index);

      std::atomic_flag mtx_multiplication_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        size_type(0U),
        upper_index,
        [&mtx_multiplication_lock, &upper_index, this, pv, &w](size_type i)
        {
          auto first1 = this->m_row_ref[i].cbegin();
          auto first2 =                pv->cbegin();

          value_type sum(0U);

          for(size_type k = 0U; k < upper_index; ++k)
          {
            const value_type uv((*(*(first1 + k))) * (*(first2 + k)));

            while(mtx_multiplication_lock.test_and_set()) { ; }
            sum += uv;
            mtx_multiplication_lock.clear();
          }

          w[i] = sum;
        }
      );

      return w;
    }

  private:
    using vector_type = std::vector<value_type, allocator_type>;

    using allocator_of_pointer_type =
      typename allocator_type::template rebind<pointer>::other;

    using vector_of_pointer_type =
      typename std::vector<pointer, allocator_of_pointer_type>;

    using allocator_of_const_pointer_type =
      typename allocator_type::template rebind<const_pointer>::other;

    using vector_of_const_pointer_type =
      typename std::vector<const_pointer, allocator_of_const_pointer_type>;

    using allocator_of_vector_of_const_pointer_type =
      typename allocator_type::template rebind<vector_of_const_pointer_type>::other;

    using vector_of_vector_of_const_pointer_type =
      std::vector<vector_of_const_pointer_type, allocator_of_vector_of_const_pointer_type>;

    const size_type m_size;

    vector_type                            m_buf;
    vector_of_pointer_type                 m_x;
    vector_of_vector_of_const_pointer_type m_row_ref;
    vector_of_vector_of_const_pointer_type m_col_ref;

    static       pointer  dummy() { static       value_type d(0U); return &d; }
    static const_pointer cdummy() { static const value_type d(0U); return &d; }

    bool assign_row_and_column_references()
    {
      for(size_type i = 0U; i < size(); ++i)
      {
        for(size_type j = 0U; j < size(); ++j)
        {
          m_row_ref[i][j] = &m_x[i][j];
          m_col_ref[j][i] = &m_x[i][j];
        }
      }

      return true;
    }
  };

  } } } // namespace gamma::lanczos::detail

  namespace my_gamma { namespace lanczos {

  // *****************************************************************************
  // Function    : class lanczos
  // 
  // Description : The lanczos class is used to calculate sets of Lanczos
  //               coefficients for approximations of the gamma function.
  // 
  // *****************************************************************************
  template<typename FloatingPointType>
  class lanczos final : private util::noncopyable
  {
  public:
    using floating_point_type = FloatingPointType;

    using matrix_type = detail::mtx<floating_point_type>;
    using vector_type = detail::vcr<floating_point_type>;

    // *****************************************************************************
    // Function    : static const mp_cpp gamma_abs(const mp_cpp& x)
    // 
    // Description : Compute the gamma function for positive real x
    //               using the Lanczos approximation.
    // 
    // *****************************************************************************
    static const floating_point_type gamma(const floating_point_type& x)
    {
      using std::exp;
      using std::isfinite;
      using std::log;
      using std::sin;

      floating_point_type gamma_result;

      if(isfinite(x) == false)
      {
        gamma_result = x;
      }
      else if(x == 0)
      {
        gamma_result = (std::numeric_limits<floating_point_type>::infinity)();
      }
      else
      {
        if(x < 0)
        {
          const floating_point_type z = -x;

          const floating_point_type zg(z * lanczos::gamma(z));

          gamma_result = -(pi() / (zg * sin(floating_point_type(pi()) * z)));
        }
        else
        {
          vector_type zn(get_n());

          zn[0U] = one();

          {
            floating_point_type z_plus_i(x);

            for(typename vector_type::size_type i = 1U; i < zn.size(); ++i)
            {
              zn[i] = one() / z_plus_i;

              ++z_plus_i;
            }
          }

          const floating_point_type x_minus_half(floating_point_type(x) - half());

          const floating_point_type r(x_minus_half + get_g());

          const floating_point_type zp(vector_type::dot(zn, get_coefficients()));

          const floating_point_type g =
            (log(zp) - ln2() + (x_minus_half * log(r))) - r;

          gamma_result = exp(g);
        }
      }

      return gamma_result;
    }

    static const vector_type& get_coefficients(const bool b_trace = false)
    {
      static bool is_init = false;

      using local_size_type = typename vector_type::size_type;

      static const local_size_type n_coefs =
        static_cast<local_size_type>(static_cast<std::int64_t>((static_cast<double>(std::numeric_limits<floating_point_type>::digits10) * 8.5) / 25.0));

      static vector_type coefs(n_coefs);

      if(is_init == false)
      {
        is_init = true;

        // Make the vector P in a timed function call.
        std::clock_t t0 = std::clock();
        make_coefs(coefs);
        std::clock_t t1 = std::clock();

        if(b_trace == true)
        {
          // Print the timing to user console.
          const double delta = static_cast<double>(t1) - static_cast<double>(t0);

          std::cout << delta / static_cast<double>(CLOCKS_PER_SEC) << " seconds" << std::endl;
        }
      }

      return coefs;
    }

    // *****************************************************************************
    // Function    : Parameters for Lanczos coefficients.
    // 
    // Description : Known good values seem to lie along lines of stability
    //               with a fixed ratio g/n and a clear relation between n
    //               and the desired precision of the results.
    //               Example:
    //                 g = n * (5.25 / 8)
    //               where n = std::numeric_limits<mp_cpp>::digits10 * (8.5 / 25.0).
    //
    // *****************************************************************************

    static const std::int32_t& get_n()
    {
      static const std::int32_t n =
        static_cast<std::int32_t>(get_coefficients().size());

      return n;
    }

    static const floating_point_type& get_g()
    {
      static const floating_point_type g =
        (floating_point_type(get_n()) * floating_point_type(5.25F)) / 8;

      return g;
    }

  private:
    static const floating_point_type& zero() { static const floating_point_type t(0U);   return t; }
    static const floating_point_type& one () { static const floating_point_type t(1U);   return t; }
    static const floating_point_type& half() { static const floating_point_type t(0.5F); return t; }

    static const floating_point_type& pi()
    {
      using std::acos;

      static const floating_point_type t(acos(-one()));

      return t;
    }

    static const floating_point_type& ln2()
    {
      using std::log;

      static const floating_point_type t(log(floating_point_type(2U)));

      return t;
    }

    // *****************************************************************************
    // Function    : static floating_point_type comb(const std::int32_t n, const std::int32_t k)
    // 
    // Description : Compute the binomial coefficient (n, k).
    //               A matrix is used to store the binomials.
    //               Recursion is used to compute the matrix
    //               entries, thus avoiding costly factorial
    //               calculations.
    // 
    // *****************************************************************************
    static const floating_point_type& comb(const std::uint32_t n, const std::uint32_t k)
    {
      static matrix_type* p_comb = nullptr;

      if(p_comb == nullptr)
      {
        using local_size_type = typename matrix_type::size_type;

        static const local_size_type upper_index =
          static_cast<local_size_type>(2ULL * static_cast<unsigned long long>(get_n()));

        p_comb = new matrix_type(upper_index);

        for(local_size_type i = 0U; i < upper_index; ++i)
        {
          (p_comb->operator[](i))[0U] = one();
        }

        for(local_size_type i = 1U; i < upper_index; ++i)
        {
          for(local_size_type j = i; j < upper_index; ++j)
          {
            (p_comb->operator[](j))[i] =   (p_comb->operator[](j - 1U))[i]
                                         + (p_comb->operator[](j - 1U))[i - 1U];
          }
        }
      }

      return ((k > n) ? zero() : (p_comb->operator[](n))[k]);
    }

    // *****************************************************************************
    // Function    : static void make_b(matrix_type* pb)
    // 
    // Description : Make the matrix B for Lanczos coefficient calculations.
    //               The source (with slight modifications) has been taken from:
    //               http://www.rskey.org/lanczos.htm
    // 
    // *****************************************************************************
    static void make_b(matrix_type* pb)
    {
      using local_size_type = typename matrix_type::size_type;

      std::fill((pb->operator[](0U)), (pb->operator[](0U)) + pb->size(), one());

      for(local_size_type i = 1U; i < pb->size(); ++i)
      {
        for(local_size_type j = 0U; j < pb->size(); ++j)
        {
          if((i > 0U) && (j >= i))
          {
            const floating_point_type binom =
              comb(std::uint32_t((i + j) - 1U), std::uint32_t(j - i));

            const bool j_minus_i_is_odd =
              (local_size_type(local_size_type(j - i) & 1U) != local_size_type(0U));

            (pb->operator[](i))[j] = (j_minus_i_is_odd ? -binom : binom);
          }
          else
          {
            (pb->operator[](i))[j] = zero();
          }
        }
      }
    }

    // *****************************************************************************
    // Function    : static void make_c(matrix_type* pc)
    // 
    // Description : Make the matrix C for Lanczos coefficient calculations.
    //               The source (with slight modifications) has been taken from:
    //               http://www.rskey.org/lanczos.htm
    // 
    // *****************************************************************************
    static void make_c(matrix_type* pc)
    {
      using local_size_type = typename matrix_type::size_type;

      const local_size_type upper_index = pc->size();

      std::atomic_flag mtx_multiplication_lock = ATOMIC_FLAG_INIT;

      my_concurrency::parallel_for
      (
        local_size_type(0U),
        upper_index,
        [&mtx_multiplication_lock, &upper_index, pc](local_size_type i)
        {
          for(local_size_type j = 0U; j < upper_index; ++j)
          {
            floating_point_type sum;

            if((i == 0U) && (j == 0U))
            {
              sum = one();
            }
            else if(j <= i)
            {
              sum = zero();

              for(std::size_t k = 0U; k <= i; ++k)
              {
                const std::size_t k_plus_j_minus_i = static_cast<std::size_t>((k + j) - i);

                if((i >= k) && (k >= k_plus_j_minus_i))
                {
                  const floating_point_type uv =
                    (  comb(std::uint32_t(2U * i), std::uint32_t(2U * k))
                     * comb(std::uint32_t(k),      std::uint32_t(k_plus_j_minus_i)));

                  while(mtx_multiplication_lock.test_and_set()) { ; }
                  sum += uv;
                  mtx_multiplication_lock.clear();
                }
              }

              sum *= 2;

              const bool i_minus_j_is_odd =
                (local_size_type(local_size_type(i - j) & 1U) != local_size_type(0U));

              if(i_minus_j_is_odd)
              {
                sum = -sum;
              }
            }
            else
            {
              sum = zero();
            }

            (pc->operator[](i))[j] = sum;
          }
        }
      );
    }

    // *****************************************************************************
    // Function    : static void make_d(matrix_type* pd)
    // 
    // Description : Make the matrix D for Lanczos coefficient calculations.
    //               The source (with slight modifications) has been taken from:
    //               http://www.rskey.org/lanczos.htm
    // 
    // *****************************************************************************
    static void make_d(matrix_type* pd)
    {
      using local_size_type = typename matrix_type::size_type;

      const local_size_type upper_index = pd->size();

      for(local_size_type i = 0U; i < pd->size(); ++i)
      {
        std::fill((pd->operator[](i)), (pd->operator[](i)) + upper_index, zero());
      }

      (pd->operator[](0U))[0U] =  one();
      (pd->operator[](1U))[1U] = -one();

      for(local_size_type i = 2U; i < upper_index; ++i)
      {
        (pd->operator[](i))[i] = (pd->operator[](i - 1U))[i - 1U];

        (pd->operator[](i))[i] *= std::int32_t((i * 4U) - 2U);
        (pd->operator[](i))[i] /= std::int32_t (i - 1U);
      }
    }

    // *****************************************************************************
    // Function    : static void make_f(vector_type* pf)
    // 
    // Description : Make the vector F for Lanczos coefficient calculations.
    //               The source (with slight modifications) has been taken from:
    //               http://www.rskey.org/lanczos.htm
    // 
    // *****************************************************************************
    static void make_f(vector_type* pf)
    {
      using std::exp;
      using std::pow;
      using std::sqrt;

      const floating_point_type exp_one_div_two    = exp(one()) / 2;
            floating_point_type i_plus_g_plus_half = floating_point_type(get_g()) + half();

      floating_point_type term = exp(i_plus_g_plus_half) * 2;

      pf->operator[](0U) = term / sqrt(i_plus_g_plus_half);

      using local_size_type = typename vector_type::size_type;

      for(local_size_type i = 1U; i < pf->size(); ++i)
      {
        term *= std::int32_t((2U * i) - 1U);
        term *= exp_one_div_two;

        ++i_plus_g_plus_half;

        const floating_point_type denom = (  sqrt(i_plus_g_plus_half)
                                           * pow (i_plus_g_plus_half, i));

        pf->operator[](i) = term / denom;
      }
    }

    // *****************************************************************************
    // Function    : static void make_coefs(vector_type& coefs)
    // 
    // Description : Make the vector P for Lanczos coefficient calculations.
    // 
    // *****************************************************************************
    static void make_coefs(vector_type& coefs)
    {
      using local_size_type = typename vector_type::size_type;

      const local_size_type size_n = get_n();

      matrix_type* p_tmp = new matrix_type(size_n);

      matrix_type* p_x = new matrix_type(size_n);

      make_b(p_tmp);
      make_c(p_x);
      p_tmp->operator*=(p_x);

      make_d(p_x);
      p_x->operator*=(p_tmp);
      delete p_tmp;

      vector_type* p_f = new vector_type(size_n);

      make_f(p_f);
      coefs = p_x->operator*(p_f);

      delete p_f;
      delete p_x;
    }
  };

  } } // namespace gamma::lanczos

#endif // GAMMA_2019_02_28_H
