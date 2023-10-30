///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_num.h
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2019
// 
// Description : Numerical constants for multiple precision math.
// 
// *****************************************************************************

#include <algorithm>
#include <iostream>
#include <sstream>

#include <mp/mp_cpp.h>
#include <mp/mp_detail.h>

// *****************************************************************************
// Function    : const mp_cpp& mp_cpp::calculate_pi(const bool b_trace, std::ostream& os)
// 
// Description : Compute pi using a quadratically convergent iteration scheme.
//               See the book "Pi Unleashed", algorithm 16.149, page 236.
//               If the input b_trace = true, then the calculation progress
//               will be output to the output stream os.
// 
//               Book reference:
//               https://www.springer.com/de/book/9783642567353
// 
// *****************************************************************************
const mp::mp_cpp& mp::mp_cpp::calculate_pi(const bool b_trace, std::ostream& os)
{
  static bool is_init = false;

  static mp_cpp val_pi;

  if(is_init == false)
  {
    is_init = true;

    if(b_trace)
    {
      os << "Calculating pi.\n";
    }

    mp_cpp a (1U);
    mp_cpp bB("0.5");
    mp_cpp s (bB);
    mp_cpp t ("0.375");

    // This loop is designed for a maximum of several million
    // decimal digits of pi. The index k should reach no higher
    // than about 25. For instance, the decimal precision
    // after 19 iterations exceeds one million digits.

    std::int64_t k_pow_two = 2;

    for(std::int32_t k = static_cast<std::int32_t>(1); k < static_cast<std::int32_t>(64); ++k)
    {
      bB.calculate_sqrt();
      a      += bB;
      a.div_by_int(2);
      val_pi  = a;
      val_pi *= a;
      bB      = val_pi;
      bB     -= t;
      bB.mul_by_int(2);

      mp_cpp iterate_term(bB);

      iterate_term -= val_pi;

      iterate_term.mul_by_int(k_pow_two);

      k_pow_two *= 2;

      s += iterate_term;

      const std::int32_t approximate_digits10 =
        static_cast<std::int32_t>(-iterate_term.my_exp);

      if(b_trace)
      {
        os << "Digits of pi: " << approximate_digits10 << '\n';
      }

      // Test the significant digits of the last iteration change.
      // If there are enough significant digits, then the calculation
      // is finished.
      if(approximate_digits10 >= std::int32_t(float(std::numeric_limits<mp_cpp>::digits10) * 0.55F))
      {
        break;
      }

      t   = val_pi;
      t  += bB;
      t.div_by_int(4);
    }

    if(b_trace)
    {
      os << "The iteration loop is done.\nCompute the inverse.\n";
    }

    val_pi += bB;
    val_pi *= s.calculate_inv();

    if(b_trace)
    {
      os << "Pi calculation is done." << std::endl;
    }
  }

  return val_pi;
}

// *****************************************************************************
// Function    : const mp_cpp& mp_cpp::calculate_pi_borwein_cubic(const bool b_trace, std::ostream& os)
// 
// Description : Compute pi using a cubically convergent iteration scheme.
//               See the book "Pi Unleashed", algorithm 16.151, page 237.
//               If the input b_trace = true, then the calculation progress
//               will be output to the output stream os.
// 
// *****************************************************************************
const mp::mp_cpp& mp::mp_cpp::calculate_pi_borwein_cubic(const bool b_trace, std::ostream& os)
{
  static bool is_init = false;

  static mp_cpp val_pi(1U);

  if(!is_init)
  {
    is_init = true;

    if(b_trace)
    {
      os << "Calculating pi with Borwein cubic.\n";
    }

    val_pi.div_by_int(3);
    mp_cpp sk((sqrt(mp_cpp(3U)) - mp::one()).div_by_int(2));

    std::int64_t three_pow_k = 1;

    // Determine the requested precision of the upcoming iteration in units of digits10.
    const std::int32_t required_precision_third =
        static_cast<std::int32_t>(static_cast<std::int32_t>(mp_cpp::mp_digits10() * 2) + static_cast<std::int32_t>(3))
      / static_cast<std::int32_t>(6);

    // Set the tolerance for the upcoming iteration in units of mp-elements (limbs).
    const std::int32_t tol_elems_third =
        ( (required_precision_third + (mp_core::mp_elem_digits10 / 2)) / mp_core::mp_elem_digits10)
      + (((required_precision_third %  mp_core::mp_elem_digits10) != 0) ? 1 : 0);

    for(std::int32_t k = static_cast<std::int32_t>(1); k < static_cast<std::int32_t>(40); ++k)
    {
      const mp_cpp rk(((mp::one() + cbrt(mp::one() - ((sk * sk) * sk)).mul_by_int(2)).calculate_inv()).mul_by_int(3));

      sk = (rk - mp::one()).div_by_int(2);

      const mp_cpp rk_squared(rk * rk);

      const mp_cpp previous_ak(val_pi);

      val_pi = (val_pi * rk_squared) - (rk_squared - mp::one()).mul_by_int(three_pow_k);

      std::int32_t approximate_digits10 = 0;

      if(val_pi.order() == previous_ak.order())
      {
        std::int32_t sig_elems;

        const bool representations_are_close =
          mp::detail::check_close_representation(val_pi, previous_ak, tol_elems_third, sig_elems);

        static_cast<void>(representations_are_close);

        approximate_digits10 =
          static_cast<std::int32_t>(sig_elems * mp_core::mp_elem_digits10);
      }

      if(b_trace)
      {
        os << "Digits of pi: " << approximate_digits10 << '\n';
      }

      // Test the significant digits of the last iteration change.
      // If there are enough significant digits, then the calculation
      // is finished.
      if(approximate_digits10 >= std::int32_t(((2 * mp_digits10_tol()) + 3) / 6))
      {
        break;
      }

      three_pow_k *= 3;
    }

    if(b_trace)
    {
      os << "The iteration loop is done.\nCompute the inverse.\n";
    }

    val_pi.calculate_inv();

    if(b_trace)
    {
      os << "Pi calculation is done." << std::endl;
    }
  }

  return val_pi;
}

// *****************************************************************************
// Function    : const mp_cpp& mp_cpp::calculate_pi_borwein_quartic(const bool b_trace, std::ostream& os)
// 
// Description : Compute pi using a quartically convergent iteration scheme.
//               See the book "Pi Unleashed", algorithm 16.152, page 237.
//               If the input b_trace = true, then the calculation progress
//               will be output to the output stream os.
// 
// *****************************************************************************
const mp::mp_cpp& mp::mp_cpp::calculate_pi_borwein_quartic(const bool b_trace, std::ostream& os)
{
  static bool is_init = false;

  static mp_cpp val_pi(2U);

  if(!is_init)
  {
    is_init = true;

    if(b_trace)
    {
      os << "Calculating pi with Borwein quartic.\n";
    }

    // Temporarily use val_pi to store sqrt(2).
    val_pi.calculate_sqrt();

    mp_cpp yk(val_pi - mp::one());
    val_pi = mp_cpp(mp_cpp(6U) - (val_pi * 4));

    std::int64_t two_pow_2k_plus_3 = 8;

    for(std::int32_t k = static_cast<std::int32_t>(1); k < static_cast<std::int32_t>(30); ++k)
    {
      // Compute yk^4.
      mp_cpp yk_pow_four(yk * yk);

      yk_pow_four *= yk_pow_four;

      // Compute 1 / [(1 - yk^4)^(1/4)].
      const mp_cpp one_minus_yk_pow_four_4th_root_inverse((mp::one() - yk_pow_four).calculate_rootn_inv(4));

      yk =   (one_minus_yk_pow_four_4th_root_inverse - mp::one())
           / (one_minus_yk_pow_four_4th_root_inverse + mp::one());

      const std::int32_t approximate_digits10 =
        static_cast<std::int32_t>(-yk.my_exp);

      if(b_trace)
      {
        os << "Digits of pi: " << approximate_digits10 << '\n';
      }

      mp_cpp one_plus_yk_squared(mp::one() + yk);

      one_plus_yk_squared *= one_plus_yk_squared;

      val_pi =   (val_pi * (one_plus_yk_squared * one_plus_yk_squared))
               - (yk * (one_plus_yk_squared - yk)).mul_by_int(two_pow_2k_plus_3);

      // Test the significant digits of the last iteration change.
      // If there are enough significant digits, then the calculation
      // is finished.
      if(approximate_digits10 >= std::int32_t((mp_digits10_tol() + 2) / 4))
      {
        break;
      }

      two_pow_2k_plus_3 *= 4;
    }

    if(b_trace)
    {
      os << "The iteration loop is done.\nCompute the inverse.\n";
    }

    val_pi.calculate_inv();

    if(b_trace)
    {
      os << "Pi calculation is done." << std::endl;
    }
  }

  return val_pi;
}

// *****************************************************************************
// Function    : const mp_cpp& mp_cpp::calculate_pi_borwein_quintic(const bool b_trace, std::ostream& os)
// 
// Description : Compute pi using a quintically convergent iteration scheme.
//               See the book "Pi Unleashed", algorithm 16.153, page 237.
//               If the input b_trace = true, then the calculation progress
//               will be output to the output stream os.
// 
// *****************************************************************************
const mp::mp_cpp& mp::mp_cpp::calculate_pi_borwein_quintic(const bool b_trace, std::ostream& os)
{
  static bool is_init = false;

  static mp_cpp val_pi("0.5");

  if(!is_init)
  {
    is_init = true;

    if(b_trace)
    {
      os << "Calculating pi with Borwein quintic.\n";
    }

    std::int64_t five_pow_k = 1;

    const mp_cpp local_five(5U);

    mp_cpp sk((sqrt(local_five) - mp::two()).mul_by_int(5));

    // Determine the requested precision of the upcoming iteration in units of digits10.
    const std::int32_t required_precision_fifth =
        static_cast<std::int32_t>(static_cast<std::int32_t>(mp_cpp::mp_digits10() * 2) + static_cast<std::int32_t>(5))
      / static_cast<std::int32_t>(10);

    // Set the tolerance for the upcoming iteration in units of mp-elements (limbs).
    const std::int32_t tol_elems_fifth =
        ( (required_precision_fifth + (mp_core::mp_elem_digits10 / 2)) / mp_core::mp_elem_digits10)
      + (((required_precision_fifth %  mp_core::mp_elem_digits10) != 0) ? 1 : 0);

    for(std::int32_t k = static_cast<std::int32_t>(1); k < static_cast<std::int32_t>(30); ++k)
    {
      const mp_cpp x         = (local_five / sk) - mp::one();
      const mp_cpp x_squared = x * x;
      const mp_cpp y         = x_squared - mp_cpp(x).mul_by_int(2) + mp_cpp(8U);
      const mp_cpp z         = mp::rootn((x * (y + mp::sqrt((y * y) - (x_squared * x).mul_by_int(4)))).div_by_int(2), 5);

      const mp_cpp term = (z + (x / z) + mp::one());

      const mp_cpp sk_squared(sk * sk);

      const mp_cpp previous_ak(val_pi);

      val_pi =
         (sk_squared * val_pi)
      - (  (sk_squared - local_five).div_by_int(2)
         +  mp::sqrt(sk * (sk_squared - mp_cpp(sk).mul_by_int(2) + local_five))).mul_by_int(five_pow_k);

      sk = mp_cpp(25U) / (sk * (term * term));

      std::int32_t approximate_digits10 = 0;

      if(val_pi.order() == previous_ak.order())
      {
        std::int32_t sig_elems;

        const bool representations_are_close =
          mp::detail::check_close_representation(val_pi, previous_ak, tol_elems_fifth, sig_elems);

        static_cast<void>(representations_are_close);

        approximate_digits10 =
          static_cast<std::int32_t>(sig_elems * mp_core::mp_elem_digits10);
      }

      if(b_trace)
      {
        os << "Digits of pi: " << approximate_digits10 << '\n';
      }

      // Test the significant digits of the last iteration change.
      // If there are enough significant digits, then the calculation
      // is finished.
      if(approximate_digits10 >= std::int32_t(((2 * mp_digits10_tol()) + 5) / 10))
      {
        break;
      }

      five_pow_k *= 5;
    }

    if(b_trace)
    {
      os << "The iteration loop is done.\nCompute the inverse.\n";
    }

    val_pi.calculate_inv();

    if(b_trace)
    {
      os << "Pi calculation is done." << std::endl;
    }
  }

  return val_pi;
}

// *****************************************************************************
// Function    : const mp_cpp& mp_cpp::calculate_ln2(const bool b_trace, std::ostream& os)
// 
// Description : Compute the natural (base-e) logarithm of 2 (ln2) using
//               a quadratically convergent AGM technique.
//               If the input b_trace = true, then the calculation progress
//               will be output to the output stream os.
//               For a description of the algorithm see the documentation
//               of the package MPFUN as well as the online book
//               "Algorithms for Programmers" from J. Arndt.
//
//               ln2 = pi / [2m * AGM(1, 4 / 2^m)]
//
//               Here m is chosen such that m > n / 2, with n being
//               the bits of precision required in the result.
//               Chose m > N / (2 * log10(2)), in other words m > N * 1.661,
//               where N is the required base-10 decimal digits of precision.
//
//               The iteration requires initial steps until the significant
//               digits start to double. For example about 20 steps take place
//               during a 10^6 digit calculation before the number of significant
//               digits increases.
//
//               Note that pi must be calculated in order to complete the
//               calculation of the logarithm.
//
//               Online references:
//               http://www.nersc.gov/~dhb/mpdist/mpdist.html
//               http://www.jjj.de/fxt/#fxtbook
//
//               Digits available for test at:
//               http://ja0hxv.calico.jp/pai/elog2val.html
//
// 
// *****************************************************************************
const mp::mp_cpp& mp::mp_cpp::calculate_ln2(const bool b_trace, std::ostream& os)
{
  static bool is_init = false;

  static mp_cpp val_ln2;

  if(is_init == false)
  {
    is_init = true;

    if(b_trace)
    {
      os << "Calculating ln2.\n";
    }

    // Choose m > (N * 1.661).
    const std::int32_t m =
      mp::detail::compute_pow2_for_agm_log(mp::zero(), mp_cpp::mp_digits10());

    // Set a0 = 1.
    // Set b0 = 4 / (2^m) = 1 / 2^(m - 2).
    mp_cpp ak(mp::one());

    mp_cpp bk(mp::pow2(static_cast<std::int64_t>(m)));

    bk.calculate_inv();

    bk.mul_by_int(4);

    mp_cpp ak_tmp;

    // Determine the requested precision of the upcoming iteration in units of digits10.
    const std::int32_t target_prec =
      static_cast<std::int32_t>((mp_cpp::mp_digits10_tol() + 1) / 2);

    // Set the tolerance for the upcoming iteration in units of mp-elements (limbs).
    const std::int32_t tol_elems_half =
        ( (target_prec + (mp_core::mp_elem_digits10 / 2)) / mp_core::mp_elem_digits10)
      + (((target_prec %  mp_core::mp_elem_digits10) != 0) ? 1 : 0);

    for(std::int32_t k = static_cast<std::int32_t>(0); k < static_cast<std::int32_t>(64); ++k)
    {
      bool break_after_this_iteration = false;

      std::int32_t sig_elems = static_cast<std::int32_t>(0);

      if(   (k > static_cast<std::int32_t>(7))
         && (ak.order() == bk.order()))
      {
        // Check for the number of significant digits
        // to be at least half of the requested digits.

        // If at least half of the requested digits
        // have been achieved, then break in the
        // following iteration.

        break_after_this_iteration =
          mp::detail::check_close_representation(ak, bk, tol_elems_half, sig_elems);
      }

      const std::int32_t approximate_digits10 =
        static_cast<std::int32_t>(sig_elems * mp_core::mp_elem_digits10);

      if(b_trace)
      {
        os << "Digits of ln2: " << approximate_digits10 << '\n';
      }

      ak_tmp  = ak;
      ak     += bk;
      ak.div_by_int(2);

      if(break_after_this_iteration)
      {
        break;
      }

      bk     *= ak_tmp;
      bk.calculate_sqrt();
    }

    // The iteration is finished: Compute ln2 = pi / [2m * AGM(1, 4 / 2^m)].
    // Note that ak = bk = AGM(...).

    if(b_trace)
    {
      os << "The iteration loop is done.\nCompute pi.\n";
    }

    // Retrieve the value of pi.
    val_ln2 = calculate_pi(b_trace);

    if(b_trace)
    {
      os << "Pi calculation done. Compute inverse\n";
    }

    // Invert ak and divide by 2m.
    ak.mul_by_int(static_cast<std::int64_t>(2 * static_cast<std::int64_t>(m)));

    val_ln2 *= ak.calculate_inv();

    if(b_trace)
    {
      os << "ln2 calculation is done." << std::endl;
    }
  }

  return val_ln2;
}

// *****************************************************************************
// Function    : mp_cpp mp_cpp::calculate_pow2(const std::int64_t p)
// 
// Description : Compute the pure power of two^p.
//               Binary splitting of the power of two is used.
// 
// *****************************************************************************

mp::mp_cpp mp::mp_cpp::calculate_pow2(const std::int64_t p)
{
  if(p < static_cast<std::int64_t>(0))
  {
    // For negative powers, return the inverse.
    return mp::one() / calculate_pow2(static_cast<std::int64_t>(-p));
  }
  else if(p == static_cast<std::int64_t>(0))
  {
    // Two raised to the power zero is one.
    return mp::one();
  }
  else if(p < static_cast<std::int64_t>(63))
  {
    // Use a simple left-shift on an integer for small powers of two.
    return mp_cpp(static_cast<std::int64_t>(static_cast<std::uint64_t>(1U) << static_cast<unsigned>(p)));
  }
  else
  {
    mp_cpp value(mp::two());

    std::int64_t n         = static_cast<std::int64_t>(1);
    std::int32_t sig_elems = static_cast<std::int32_t>(1);

    for(;;)
    {
      // Use the appropriate precision for faster multiplication.
      // The number of significant digits should double with each
      // multiplication (multiplication by 2.2 in the next line of code).
      const std::int32_t n_prec =
        static_cast<std::int32_t>(static_cast<std::int64_t>(2.2F * static_cast<float>(sig_elems * mp_core::mp_elem_digits10)));

      value.precision(n_prec);

      n     *= 2;
      value *= value;

      if(static_cast<std::int64_t>(p / n) < static_cast<std::int64_t>(2))
      {
        break;
      }

      // Determine the necessary precision for the next multiplication
      // based on the number of significant elements.

      for( ; sig_elems < mp_elem_number(); ++sig_elems)
      {
        if(value.my_data[static_cast<std::size_t>(sig_elems)] == static_cast<value_type>(0U))
        {
          break;
        }
      }
    }

    // Set the precision to the maximum precision.
    value.precision(mp_elem_number() * mp_core::mp_elem_digits10);

    // Here, we either finish the power-of-two calculation,
    // or multiply by a small integer, or call pow2 again
    // recursively for computing the remaining powers of two.

    const std::int64_t p_minus_n = static_cast<std::int64_t>(p - n);

    if(p_minus_n == static_cast<std::int64_t>(0))
    {
      // Do nothing.
      // The power-of-two calculation is done.
      ;
    }
    else if(p_minus_n < static_cast<std::int64_t>(63))
    {
      // Multiply by a left-shifted integer for small
      // remaining powers of two.
      value.mul_by_int(static_cast<std::int64_t>(static_cast<std::uint64_t>(1U) << static_cast<unsigned>(p_minus_n)));
    }
    else
    {
      // Call the function again recursively (one or more times)
      // for computing the remaining powers of two.
      value *= mp_cpp::calculate_pow2(p_minus_n);
    }

    return value;
  }
}

const mp::mp_cpp& mp::zero() { static const mp::mp_cpp val( 0U);   return val; }
const mp::mp_cpp& mp::one () { static const mp::mp_cpp val( 1U);   return val; }
const mp::mp_cpp& mp::two () { static const mp::mp_cpp val( 2U);   return val; }
const mp::mp_cpp& mp::half() { static const mp::mp_cpp val("0.5"); return val; }

const mp::mp_cpp& mp::pi()
{
  if(std::numeric_limits<mp_cpp>::digits10 < static_cast<std::int32_t>(1001))
  {
    // Express 1100 digits of pi in a string.
    static const std::string str =
      std::string("3.")
    + std::string("1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679")
    + std::string("8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196")
    + std::string("4428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273")
    + std::string("7245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094")
    + std::string("3305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912")
    + std::string("9833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132")
    + std::string("0005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235")
    + std::string("4201995611212902196086403441815981362977477130996051870721134999999837297804995105973173281609631859")
    + std::string("5024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303")
    + std::string("5982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989")
    + std::string("3809525720106548586327886593615338182796823030195203530185296899577362259941389124972177528347913152")
    ;

    static const mp_cpp val(str.c_str());

    return val;
  }
  else
  {
    return mp_cpp::calculate_pi();
  }
}

const mp::mp_cpp& mp::pi_half()
{
  static const mp_cpp val(mp::pi() / static_cast<std::int32_t>(2));

  return val;
}

const mp::mp_cpp& mp::ln2()
{
  if(std::numeric_limits<mp_cpp>::digits10 < static_cast<std::int32_t>(1001))
  {
    // Express 1100 digits of ln2 in a string.
    static const std::string str =
      std::string("0.")
    + std::string("6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875")
    + std::string("4200148102057068573368552023575813055703267075163507596193072757082837143519030703862389167347112335")
    + std::string("0115364497955239120475172681574932065155524734139525882950453007095326366642654104239157814952043740")
    + std::string("4303855008019441706416715186447128399681717845469570262716310645461502572074024816377733896385506952")
    + std::string("6066834113727387372292895649354702576265209885969320196505855476470330679365443254763274495125040606")
    + std::string("9438147104689946506220167720424524529612687946546193165174681392672504103802546259656869144192871608")
    + std::string("2938031727143677826548775664850856740776484514644399404614226031930967354025744460703080960850474866")
    + std::string("3852313818167675143866747664789088143714198549423151997354880375165861275352916610007105355824987941")
    + std::string("4729509293113897155998205654392871700072180857610252368892132449713893203784393530887748259701715591")
    + std::string("0708823683627589842589185353024363421436706118923678919237231467232172053401649256872747782344535347")
    + std::string("6481149418642386776774406069562657379600867076257199184734022651462837904883062033061144630073719489")
    ;

    static const mp_cpp val(str.c_str());

    return val;
  }
  else
  {
    return mp_cpp::calculate_ln2();
  }
}

const mp::mp_cpp& mp::ln10()
{
  if(std::numeric_limits<mp_cpp>::digits10 < static_cast<std::int32_t>(1001))
  {
    // Express 1100 digits of ln10 in a string.
    static const std::string str =
      std::string("2.")
    + std::string("3025850929940456840179914546843642076011014886287729760333279009675726096773524802359972050895982983")
    + std::string("4196778404228624863340952546508280675666628736909878168948290720832555468084379989482623319852839350")
    + std::string("5308965377732628846163366222287698219886746543667474404243274365155048934314939391479619404400222105")
    + std::string("1017141748003688084012647080685567743216228355220114804663715659121373450747856947683463616792101806")
    + std::string("4450706480002775026849167465505868569356734206705811364292245544057589257242082413146956890167589402")
    + std::string("5677631135691929203337658714166023010570308963457207544037084746994016826928280848118428931484852494")
    + std::string("8644871927809676271275775397027668605952496716674183485704422507197965004714951050492214776567636938")
    + std::string("6629769795221107182645497347726624257094293225827985025855097852653832076067263171643095059950878075")
    + std::string("2371033310119785754733154142180842754386359177811705430982748238504564801909561029929182431823752535")
    + std::string("7709750539565187697510374970888692180205189339507238539205144634197265287286965110862571492198849978")
    + std::string("7488737713456862091670584980782805975119385444500997813114691593466624107184669231010759843831919129")
    ;

    static const mp_cpp val(str.c_str());

    return val;
  }
  else
  {
    static const mp_cpp val(mp_cpp(10U).calculate_log(0, zero()));

    return val;
  }
}

const mp::mp_cpp& mp::mp_cpp::value_nan()
{
  static const mp_cpp val(mp_NaN);

  return val;
}

const mp::mp_cpp& mp::mp_cpp::value_inf()
{
  static const mp_cpp val(mp_inf);

  return val;
}

namespace local
{
  template<typename ArithmeticType>
  std::string my_lexical_cast(const ArithmeticType x)
  {
    std::stringstream ss;

    ss << x;

    return ss.str();
  }

  template<typename ArithmeticType>
  ArithmeticType my_lexical_cast(const std::string& str)
  {
    std::stringstream ss;

    ss << str;

    ArithmeticType x;

    ss >> x;

    return x;
  }
}

const mp::mp_cpp& mp::mp_cpp::value_max()
{
  // Select a value near the absolute maximum.

  static const mp_cpp val(  std::string("2E")
                          + local::my_lexical_cast(std::numeric_limits<mp_cpp>::max_exponent10));

  return val;
}

const mp::mp_cpp& mp::mp_cpp::value_min()
{
  // Select a value near the absolute minimum.

  static const mp_cpp val(  std::string("2E")
                          + local::my_lexical_cast(std::numeric_limits<mp_cpp>::min_exponent10));

  return val;
}

const mp::mp_cpp& mp::mp_cpp::value_eps()
{
  // Compute epsilon with string manipulation.

  static const mp_cpp val
    (  std::string("0.")
     + std::string(static_cast<std::string::size_type>(std::numeric_limits<mp_cpp>::digits10 - 2),
                   static_cast<char>('0'))
     + std::string(static_cast<std::string::size_type>(1U),
                   static_cast<char>('1')));

  return val;
}

// *****************************************************************************
// Function    : std::string mp_cpp::factorial_binsplit(const std::int32_t n_hi, const std::int32_t n_lo)
// 
// Description : Implementation of a recursive binary splitting algorithm
//               for the factorial. This subroutine manipulates STL-strings
//               which are converted to and from mp_cpp objects.
//               Strings are used because mp_cpp objects are not well
//               suited for recursive subroutine calls since they have a
//               fixed, large size equal to that of the maximum necessary
//               size. This imposes significant memory usage for a recursive
//               routine. Even with the overhead of converting mp_cpp objects
//               to and from strings, the implementation using strings is
//               at the moment faster than alternative implementations
//               using a recursive return of mp_cpp objects.
// 
// *****************************************************************************
std::string mp::mp_cpp::factorial_binsplit(const std::int32_t n_hi, const std::int32_t n_lo)
{
  std::string factorial_binsplit_string;

  switch(n_hi - n_lo)
  {
    case 0:
      // This is an error condition that should not occur.
      factorial_binsplit_string = std::string(1U, char('0'));

      break;

    case 1:
      // Binary splitting has reduced the size of adjacent multiply
      // to just one integer. No more recursive calls are needed.
      // Return the integer value converted to a string.

      factorial_binsplit_string = local::my_lexical_cast<std::int32_t>(n_lo);

      break;

    case 2:
      // Binary splitting has reduced the size of adjacent multiply
      // to two integers. No more recursive calls are needed.
      // Return the product of the two integer multiplicands
      // converted to a strings.

      factorial_binsplit_string = local::my_lexical_cast(std::int64_t(std::int64_t(n_lo) * (n_lo + 1)));

      break;

    default:
    {
      const std::int32_t n_mid = (n_hi + n_lo) / 2;

      // Execute the recursive calls.
      std::string lo_str = factorial_binsplit(n_mid, n_lo);
      std::string hi_str = factorial_binsplit(n_hi,  n_mid);

      const std::int32_t n_length = static_cast<std::int32_t>(lo_str.length() + hi_str.length());

      if(   n_length <= 18
         && lo_str.find('.') == std::string::npos
         && hi_str.find('.') == std::string::npos)
      {
        // For final values <= 10^18 use a signed 64-bit integer calculation.
        // Note that the maximum value of std::uint64_t is approx. 1.84 * 10^19

        factorial_binsplit_string =
          local::my_lexical_cast(std::int64_t(  local::my_lexical_cast<std::int64_t>(lo_str)
                                              * local::my_lexical_cast<std::int64_t>(hi_str)));
      }
      else
      {
        // Convert strings to mp_cpp objects for the multiplication
        static mp_cpp lo_part;
        static mp_cpp hi_part;

        lo_part.read_string(lo_str.c_str());
        hi_part.read_string(hi_str.c_str());

        // Only use the minimum necessary precision for the multiplication.
        const std::int32_t n_prec_elems =     (n_length / mp_core::mp_elem_digits10)
                                          + (((n_length % mp_core::mp_elem_digits10) != 0) ? 1 : 0);

        const std::int32_t n_prec = n_prec_elems * mp_core::mp_elem_digits10;

        lo_part.precision(n_prec);
        lo_part *= hi_part;

        // Extract the output string.
        // Once again, use only the required precision.
        lo_part.write_string(lo_str, n_prec, std::ios::fmtflags());

        factorial_binsplit_string = lo_str;
      }
    }

    break;
  }

  return factorial_binsplit_string;
}

// *****************************************************************************
// Function    : mp_cpp mp_cpp::calculate_factorial(const std::int32_t n)
// 
// Description : Compute the factorial n!.
//               For (integer n  <   0), the function returns NaN.
//               For (integer n == 0,1), the function returns one.
//               For (integer n <=  20), a 64-bit integer calculation is used.
// 
// *****************************************************************************
mp::mp_cpp mp::mp_cpp::calculate_factorial(const std::int32_t n)
{
  mp_cpp factorial_result;

  if(n < std::int32_t(0))
  {
    // Here, we simply define complex infinity to be NaN.
    factorial_result = mp_cpp::value_nan();
  }
  else if((n == std::int32_t(0)) || (n == std::int32_t(1)))
  {
    factorial_result = mp::one();
  }
  else if(n == std::int32_t(2))
  {
    factorial_result = mp::two();
  }
  else if (n <= std::int32_t(20))
  {
    // Note that [(2^63) - 1 > 20!].
    // In particular, with [((2^63) - 1) - 20!] = approx. 6.8 * 10^18.

    std::uint64_t n_value = 2U;

    for(std::uint_fast16_t i = 3U; i <= std::uint_fast16_t(n); ++i)
    {
      n_value *= i;
    }

    factorial_result = mp_cpp(std::int64_t(n_value));
  }
  else
  {
    const std::string str(factorial_binsplit(n + 1, 1));

    factorial_result = mp_cpp(str.data());
  }

  return factorial_result;
}

// *****************************************************************************
// Function    : const mp_cpp binomial(const std::int32_t n, const std::int32_t k)
// 
// Description : Compute the binomial coefficient (n, k) = n! / [k! (n - k)!]
//               Extend the computation to include negative integers.
// 
// *****************************************************************************
mp::mp_cpp mp::binomial(const std::int32_t n, const std::int32_t k)
{
  mp_cpp binomial_result;

  if((n >= static_cast<std::int32_t>(0)) && (k >= static_cast<std::int32_t>(0)))
  {
    if(k < n)
    {
      binomial_result = mp::factorial(n) / (mp::factorial(k) * mp::factorial(n - k));
    }
    else
    {
      binomial_result = mp::zero();
    }
  }
  else
  {
    if((k < static_cast<std::int32_t>(0)) && (k <= n))
    {
      const std::int32_t n_minus_k = static_cast<std::int32_t>(k - n);

      const bool is_neg =
        (static_cast<std::int32_t>(n_minus_k % static_cast<std::int32_t>(2)) != static_cast<std::int32_t>(0));

      binomial_result = binomial(-k - 1, n_minus_k);

      if(is_neg) { binomial_result.negate(); }
    }
    else if(k >= static_cast<std::int32_t>(0))
    {
      const bool is_neg =
        (static_cast<std::int32_t>(k % static_cast<std::int32_t>(2)) != static_cast<std::int32_t>(0));

      binomial_result = binomial(-n + k - 1, k);

      if(is_neg) { binomial_result.negate(); }
    }
    else
    {
      binomial_result = mp::zero();
    }
  }

  return binomial_result;
}
