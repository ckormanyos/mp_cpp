///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

 #include <functions/integer/bernoulli_b.h>

// *****************************************************************************
// Function    : void mp::bernoulli_b(std::vector<mp_cpp>& bn, const std::uint32_t n)
//
// Description : Compute the array of Bernoulli numbers up to and including n.
//
// *****************************************************************************

void mp::bernoulli_b(std::vector<mp_cpp>& bn, const std::uint32_t n)
{
  // See reference "Computing Bernoulli and Tangent Numbers", Richard P. Brent.
  // See also the book Richard P. Brent and Paul Zimmermann, "Modern Computer Arithmetic",
  // Cambridge University Press, 2010, 237 pp.

  const std::uint32_t nn = (((n % static_cast<std::uint32_t>(2)) != static_cast<std::uint32_t>(0U)) ? static_cast<std::uint32_t>(n + 1U) : n);

  const std::int32_t m = nn / 2;

  std::vector<mp_cpp> tangent_numbers(static_cast<std::vector<mp_cpp>::size_type>(m + 1));

  tangent_numbers[0U] = mp::zero();
  tangent_numbers[1U] = mp::one();

  for(std::int32_t k = 2; k <= m; ++k)
  {
    tangent_numbers[k] = (k - 1) * tangent_numbers[k - 1];
  }

  for(std::int32_t k = 2; k <= m; ++k)
  {
    for(std::int32_t j = k; j <= m; ++j)
    {
      tangent_numbers[j] = (tangent_numbers[j - 1] * (j - k)) + (tangent_numbers[j] * (j - k + 2));
    }
  }

  mp_cpp two_pow_two_m(4U);

  bn.clear();
  bn.resize(static_cast<std::vector<mp_cpp>::size_type>(nn + 1));

  for(std::int32_t i = 1; i < static_cast<std::int32_t>(tangent_numbers.size()); ++i)
  {
    const std::int32_t two_i = static_cast<std::int32_t>(static_cast<std::int32_t>(2) * i);

    const mp_cpp b = (tangent_numbers[i] * two_i) / (two_pow_two_m * (two_pow_two_m - 1));

    const bool  b_neg = (static_cast<std::int32_t>(two_i % static_cast<std::int32_t>(4)) == static_cast<std::int32_t>(0));

    bn[2 * i] = ((!b_neg) ? b : -b);

    two_pow_two_m *= 4;
  }

  bn[0U] =  mp::one();
  bn[1U] = -mp::half();
}
