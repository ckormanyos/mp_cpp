///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2020 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : gamma.cpp
// 
// Project     : Samples for multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 2020 - 2023
// 
// Description : Gamma function calculation using multiple
//               precision math library. Argument-scaling
//               combined with an expansion in Bernoulli numbers
//               is used. This example has been taken from
//               example008 in the wide-decimal project.
// 
// *****************************************************************************

#include <array>
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <utility>

#include <mp/mp_cpp.h>
#include <samples/samples.h>

namespace local
{
  using wide_decimal_type = ::mp::mp_cpp;

  template<typename FloatingPointType>
  auto pi() -> FloatingPointType
  {
    return static_cast<FloatingPointType>(3.1415926535897932384626433832795029L); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  }

  template<>
  auto pi() -> wide_decimal_type // LCOV_EXCL_LINE
  {
    return ::mp::pi();
  }

  using container_type = std::vector<wide_decimal_type>;

  auto bernoulli_table() -> container_type&
  {
    using bernoulli_table_array_type = container_type;
    using local_size_type            = typename container_type::size_type;

    const auto bernoulli_table_size =
      static_cast<local_size_type>
      (
        static_cast<float>
        (
            static_cast<long double>(std::numeric_limits<wide_decimal_type>::digits10)
          * static_cast<long double>(0.95L)
        )
      );

    static bernoulli_table_array_type bernoulli_table(bernoulli_table_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,cppcoreguidelines-avoid-non-const-global-variables,readability-magic-numbers,cert-err58-cpp)

    return bernoulli_table;
  }

  template<typename FloatingPointType>
  auto bernoulli_b(FloatingPointType* bn, std::uint32_t n) -> void
  {
    using floating_point_type = FloatingPointType;

    // See reference "Computing Bernoulli and Tangent Numbers", Richard P. Brent.
    // See also the book Richard P. Brent and Paul Zimmermann, "Modern Computer Arithmetic",
    // Cambridge University Press, 2010, p. 237.

    using tangent_numbers_array_type = container_type;
    using local_size_type = typename tangent_numbers_array_type::size_type;

    const auto m          = static_cast<local_size_type>(static_cast<local_size_type>(n) / static_cast<local_size_type>(UINT8_C(2)));
    const auto m_plus_one = static_cast<local_size_type>(m + static_cast<local_size_type>(UINT8_C(1)));

    container_type tangent_numbers(m_plus_one);

    tangent_numbers[static_cast<local_size_type>(UINT8_C(0))] = static_cast<unsigned>(UINT8_C(0));
    tangent_numbers[static_cast<local_size_type>(UINT8_C(1))] = static_cast<unsigned>(UINT8_C(1));

    for(auto k = static_cast<local_size_type>(UINT8_C(1)); k < m; ++k) // NOLINT(altera-id-dependent-backward-branch)
    {
      const auto k_plus_one = static_cast<local_size_type>(k + static_cast<local_size_type>(UINT8_C(1)));

      tangent_numbers[k_plus_one] = tangent_numbers[k] * k;
    }

    for(auto k = static_cast<local_size_type>(UINT8_C(2)); k <= m; ++k) // NOLINT(altera-id-dependent-backward-branch)
    {
      for(auto j = k; j <= m; ++j) // NOLINT(altera-id-dependent-backward-branch)
      {
        const auto j_minus_k = static_cast<local_size_type>(j - k);

        tangent_numbers[j] =   (tangent_numbers[j - 1] *  j_minus_k)
                             + (tangent_numbers[j]     * (j_minus_k + 2U));
      }
    }

    floating_point_type two_pow_two_m(4U);

    for(auto   i = static_cast<local_size_type>(UINT8_C(1));
               i < static_cast<local_size_type>(n / static_cast<local_size_type>(UINT8_C(2)));
             ++i)
    {
      const auto two_i = static_cast<local_size_type>(static_cast<local_size_type>(UINT8_C(2)) * i);

      const floating_point_type b = (tangent_numbers[i] * two_i) / (two_pow_two_m * (two_pow_two_m - 1));

      const auto b_neg =
        (static_cast<local_size_type>(two_i % static_cast<local_size_type>(UINT8_C(4))) == static_cast<local_size_type>(UINT8_C(0)));

      bn[two_i] = ((!b_neg) ? b : -b); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

      two_pow_two_m *= 4U;
    }

    bn[static_cast<local_size_type>(UINT8_C(0))] = static_cast<unsigned>(UINT8_C(1));                           // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
    bn[static_cast<local_size_type>(UINT8_C(1))] = floating_point_type(-1) / static_cast<unsigned>(UINT8_C(2)); // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)
  }

  template<typename FloatingPointType>
  auto tgamma(const FloatingPointType& x) -> FloatingPointType
  {
    using floating_point_type = FloatingPointType;

    // Check if the argument should be scaled up for the Bernoulli series expansion.
    const auto min_arg_n =
      static_cast<std::int32_t>
      (
        static_cast<float>(static_cast<float>(std::numeric_limits<floating_point_type>::digits10) * 0.8F)
      );

    static const floating_point_type min_arg_x = floating_point_type(min_arg_n);

    const auto n_recur =
      static_cast<std::uint32_t>
      (
        (x < min_arg_x) ? static_cast<std::uint32_t>(static_cast<std::uint32_t>(min_arg_n - static_cast<std::int32_t>(floating_point_type::to_int64(x))) + 1U)
                        : static_cast<std::uint32_t>(0U)
      );

    floating_point_type xx(x);

    // Scale the argument up and use downward recursion later for the final result.
    if(n_recur != static_cast<std::uint32_t>(UINT8_C(0)))
    {
      xx += n_recur;
    }

          floating_point_type one_over_x_pow_two_n_minus_one = 1 / xx;
    const floating_point_type one_over_x2                    =  one_over_x_pow_two_n_minus_one * one_over_x_pow_two_n_minus_one;
          floating_point_type sum                            = (one_over_x_pow_two_n_minus_one * bernoulli_table()[2U]) / 2U;

    auto tol = std::numeric_limits<floating_point_type>::epsilon();

    if(xx > static_cast<unsigned>(UINT8_C(8)))
    {
      // In the following code sequence, we extract the approximate logarithm
      // of the argument x and use the leading term of Stirling's approximation,
      // which is Log[Gamma[x]] aprox. (x (Log[x] - 1)) in order to scale
      // the tolerance. In order to do this, we find the built-in floating point
      // approximation of (x (Log[x] - 1)).

      // Limit fx to the range 8 <= fx <= 10^16, where 8 is chosen to
      // ensure that (log(fx) - 1.0F) remains positive and 10^16 is
      // selected arbitrarily, yet ensured to be rather large.
      const auto fx_max = (std::max)(static_cast<floating_point_type>(8U), xx); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

      const auto fx_lim = (std::min)(fx_max, static_cast<floating_point_type>(UINT64_C(10000000000000000)));

      auto fx = floating_point_type::to_float(fx_lim);

      using std::log;

      tol *= static_cast<float>(fx * (log(fx) - 1.0F));
    }

    // Perform the Bernoulli series expansion.
    for(auto n2 = static_cast<std::uint32_t>(UINT8_C(4)); n2 < static_cast<std::uint32_t>(bernoulli_table().size()); n2 += 2U)
    {
      one_over_x_pow_two_n_minus_one *= one_over_x2;

      const floating_point_type term =
          (one_over_x_pow_two_n_minus_one * bernoulli_table()[n2])
        / static_cast<std::uint64_t>(static_cast<std::uint64_t>(n2) * static_cast<std::uint32_t>(n2 - 1U));

      using std::fabs;

      if((n2 > static_cast<unsigned>(UINT8_C(10))) && (fabs(term) < tol))
      {
        break;
      }

      sum += term;
    }

    using local::pi;
    using std::exp;

    static const floating_point_type half           = floating_point_type(1U) / 2U;
    static const floating_point_type half_ln_two_pi = log(pi<floating_point_type>() * 2U) / 2U;

    floating_point_type g = exp(((((xx - half) * log(xx)) - xx) + half_ln_two_pi) + sum);

    // Rescale the result using downward recursion if necessary.
    for(auto k = static_cast<std::uint32_t>(0U); k < n_recur; ++k) // NOLINT(altera-id-dependent-backward-branch)
    {
      g /= --xx;
    }

    return g;
  }

  auto example008_bernoulli_tgamma_run() -> bool
  {
    const auto begin = std::clock();

    // Initialize the table of Bernoulli numbers.
    bernoulli_b
    (
      bernoulli_table().data(),
      static_cast<std::uint32_t>(bernoulli_table().size())
    );

    const auto end = std::clock();

    const auto elapsed = static_cast<float>(static_cast<float>(end - begin) / CLOCKS_PER_SEC);

    std::cout << "Initialize coefficient table       : "
              << elapsed
              << std::endl;

    using uint_ratio_pair_type = std::pair<std::uint64_t, std::uint32_t>;

    // In this example, we compute values of Gamma[1/2 + n].

    // We will make use of the relation
    //                     (2n)!
    //   Gamma[1/2 + n] = -------- * Sqrt[Pi].
    //                    (4^n) n!

    // Table[Factorial[2 n]/((4^n) Factorial[n]), {n, 0, 17, 1}]
    using uint_ratio_pair_array_type = std::array<uint_ratio_pair_type, static_cast<std::size_t>(UINT8_C(18))>;

    const uint_ratio_pair_array_type ratios =
    {{
      { UINT64_C(                  1), UINT32_C(     1) },
      { UINT64_C(                  1), UINT32_C(     2) },
      { UINT64_C(                  3), UINT32_C(     4) },
      { UINT64_C(                 15), UINT32_C(     8) },
      { UINT64_C(                105), UINT32_C(    16) },
      { UINT64_C(                945), UINT32_C(    32) },
      { UINT64_C(              10395), UINT32_C(    64) },
      { UINT64_C(             135135), UINT32_C(   128) },
      { UINT64_C(            2027025), UINT32_C(   256) },
      { UINT64_C(           34459425), UINT32_C(   512) },
      { UINT64_C(          654729075), UINT32_C(  1024) },
      { UINT64_C(        13749310575), UINT32_C(  2048) },
      { UINT64_C(       316234143225), UINT32_C(  4096) },
      { UINT64_C(      7905853580625), UINT32_C(  8192) },
      { UINT64_C(    213458046676875), UINT32_C( 16384) },
      { UINT64_C(   6190283353629375), UINT32_C( 32768) },
      { UINT64_C( 191898783962510625), UINT32_C( 65536) },
      { UINT64_C(6332659870762850625), UINT32_C(131072) }
    }};

    bool result_is_ok = true;

    const wide_decimal_type tol (std::numeric_limits<wide_decimal_type>::epsilon() * static_cast<std::uint32_t>(UINT32_C(100000)));
    const wide_decimal_type half(0.5F);

    for(auto i = static_cast<std::size_t>(0U); i < std::tuple_size<uint_ratio_pair_array_type>::value; ++i)
    {
      // Calculate Gamma[1/2 + i]

      using local::tgamma;

      const wide_decimal_type result_gamma = tgamma(half + i);

      // Calculate the control value.

      using local::pi;

      const wide_decimal_type ctrl = (sqrt(pi<wide_decimal_type>()) * ratios[i].first) / ratios[i].second; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

      const wide_decimal_type delta = fabs(1 - (result_gamma / ctrl));

      const auto result_gamma_is_ok = (delta < tol);

      result_is_ok = (result_gamma_is_ok && result_is_ok);

      {
        std::stringstream strm;

        strm << "result_gamma: "   << std::scientific << result_gamma
             << ", closeness: "    << std::scientific << delta
             << ", tol: "          << std::scientific << tol
             << ", result_is_ok: " << std::boolalpha << result_is_ok;

        std::cout << strm.str()  << std::endl;
      }
    }

    return result_is_ok;
  }
} // namespace local

auto samples::gamma(const int argc, const char* argv[]) -> bool
{
  static_cast<void>(argc);
  static_cast<void>(argv);

  auto result_is_ok = true;

  {
    constexpr auto d10 = static_cast<std::int32_t>(UINT16_C(5001));

    const auto number_of_threads_hint =
      static_cast<unsigned>
      (
        std::thread::hardware_concurrency()
      );

    const auto n_threads = static_cast<int>(number_of_threads_hint / 2);

    const auto result_create_mp_base_is_ok = mp::mp_base::create_mp_base(d10, n_threads);

    result_is_ok = (result_create_mp_base_is_ok && result_is_ok);
  }

  float elapsed { };

  if(result_is_ok)
  {
    const auto begin = std::clock();

    const auto result_tgamma_is_ok = local::example008_bernoulli_tgamma_run();

    const auto end = std::clock();

    elapsed = static_cast<float>(static_cast<float>(end - begin) / CLOCKS_PER_SEC);

    result_is_ok = (result_tgamma_is_ok && result_is_ok);
  }

  std::cout << "Time example008_bernoulli_tgamma() : "
            << elapsed
            << std::endl;

  return result_is_ok;
}
