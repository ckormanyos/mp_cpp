///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : ln2.cpp
// 
// Project     : Samples for multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Compute ln2 and print its value to a text file.
// 
// *****************************************************************************

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>

#include <mp/mp_math.h>
#include <samples/samples.h>

namespace local_ln2
{
  void print_timing_report(std::ostream& os, const double time_for_calculation);
  bool print_output_result(std::ostream& os, const double time_for_calculation);

  const mp::mp_cpp& my_ln2(const bool b_trace, std::ostream& os = std::cout);
}

const mp::mp_cpp& local_ln2::my_ln2(const bool b_trace, std::ostream& os)
{
  static const mp::mp_cpp& my_local_ln2_ref = mp::mp_cpp::calculate_ln2(b_trace, os);

  return my_local_ln2_ref;
}

// *****************************************************************************
// Function    : bool ln2(const int argc, const char* argv[])
// 
// Description : Multiple precision calculation of ln2 based on
//               command line user input of various characteristics.
// 
// *****************************************************************************
bool samples::ln2(const int argc, const char* argv[])
{
  std::string::size_type pos { };

  auto my_digits10    = static_cast<std::int32_t>(INT32_C(1000000));
  auto my_fft_threads = static_cast<int>(INT8_C(4));

  std::stringstream strm;

  for(auto n = static_cast<std::int32_t>(INT8_C(0)); n < std::int32_t(argc); ++n)
  {
    const std::string cmd_str = argv[static_cast<std::size_t>(n)];

    // Extract the number of FFT threads.
    // This command has the form -t4, for example, for 4 FFT threads.
    if((pos = cmd_str.find("-t")) != std::string::npos)
    {
      strm << cmd_str.c_str() + static_cast<std::size_t>(pos + static_cast<std::size_t>(UINT8_C(2)));

      strm >> my_fft_threads;

      strm.str(std::string());
      strm.clear();
    }

    // Extract the number of decimal digits following
    // the decimal point. This command has the form -d1000000,
    // for example, for one million decimal digits after
    // the decimal point.
    if((pos = cmd_str.find("-d")) != std::string::npos)
    {
      strm << cmd_str.c_str() + static_cast<std::size_t>(pos + static_cast<std::size_t>(UINT8_C(2)));

      strm >> my_digits10;

      strm.str(std::string());
      strm.clear();
    }
  }

  auto result_calculation_is_ok = false;

  // Initialize the multiple precision base.
  // Set the decimal precision and the number of FFT threads.

  const auto result_create_mp_base_is_ok = mp::mp_base::create_mp_base(my_digits10, my_fft_threads);

  constexpr char default_fileout_name[] = "ln2.out";

  if(result_create_mp_base_is_ok)
  {
    // Get the output file path relative to the path
    // of the executable program.

    auto str_outfile = std::string(argv[static_cast<std::size_t>(UINT8_C(0))]);

    if((pos = str_outfile.rfind(".exe")) != std::string::npos)
    {
      str_outfile = str_outfile.substr(static_cast<std::size_t>(UINT8_C(0)), pos + static_cast<std::size_t>(UINT8_C(1))) + "out";

      if((pos = str_outfile.rfind('\\')) != std::string::npos)
      {
        str_outfile = str_outfile.substr(static_cast<std::size_t>(UINT8_C(0)), pos + static_cast<std::size_t>(UINT8_C(1))) + std::string(default_fileout_name);
      }

      if((pos = str_outfile.rfind('/')) != std::string::npos)
      {
        str_outfile = str_outfile.substr(static_cast<std::size_t>(UINT8_C(0)), pos + static_cast<std::size_t>(UINT8_C(1))) + std::string(default_fileout_name);
      }
    }
    else
    {
      str_outfile = std::string(default_fileout_name);
    }

    std::ofstream outfile(str_outfile.c_str());

    if(outfile.is_open())
    {
      // Start the time measurement.
      const auto t0 = std::clock();

      // Calculate the value with the user output trace flag set to true.
      local_ln2::my_ln2(true);

      // End the time measurement.
      const auto t1 = std::clock();

      const auto time_for_calculation = static_cast<double>(static_cast<double>(t1 - t0) / CLOCKS_PER_SEC);

      // Print the calculation time to the standard output.
      local_ln2::print_timing_report(std::cout, time_for_calculation);

      // Print the result of the pi calculation to the output file.
      result_calculation_is_ok = local_ln2::print_output_result(outfile, time_for_calculation);

      outfile.close();
    }
  }

  return (result_create_mp_base_is_ok && result_calculation_is_ok);
}

// *****************************************************************************
// Function    : static void timing_report(std::ostream& os)
// 
// Description : Report the timing of the calculation.
// 
// *****************************************************************************
void local_ln2::print_timing_report(std::ostream& os, const double time_for_calculation)
{
  os << std::endl
     << "Time for calculation: "
     << std::numeric_limits<mp::mp_cpp>::digits10
     << " digits in "
     << std::setprecision(static_cast<int>(INT8_C(4)))
     << time_for_calculation
     << " seconds."
     << std::endl
     << std::endl;
}

// *****************************************************************************
// Function    : static void print_output(std::ostream& os)
// 
// Description : Print the output using a readable format.
// 
// *****************************************************************************
bool local_ln2::print_output_result(std::ostream& os, const double time_for_calculation)
{
  // Print the calculation time to the output stream.
  print_timing_report(os, time_for_calculation);

  std::stringstream strm;

  // Pipe the value into a stringstream object with full precision.
  strm << std::fixed
       << std::setprecision(static_cast<int>(std::numeric_limits<mp::mp_cpp>::digits10))
       << my_ln2(false);

  // Extract the string value.
  const std::string str_val(strm.str());

  const auto result_str_val_head_is_ok = (str_val.find("0.693147180") != std::string::npos);

  const auto result_str_val_tail_is_ok =
  (
    (std::numeric_limits<mp::mp_cpp>::digits10 > static_cast<std::uint32_t>(UINT32_C(1000000)))
      ? (str_val.rfind("6808836542") != std::string::npos)
      : true
  );

  const auto result_str_val_is_ok = (result_str_val_head_is_ok && result_str_val_tail_is_ok);

  // Print the value using the following paramater-tunable format.

  // ln2 = 0.6931471805 5994530941 7232121458 1765680755 0013436025 : 50
  //         5254120680 0094933936 2196969471 5605863326 9964186875 : 100
  //         4200148102 0570685733 6855202357 5813055703 2670751635 : 150
  //         0759619307 2757082837 1435190307 0386238916 7347112335 : 200
  //         ...

  const char* char_set_separator   = " ";
  const char* char_group_separator = "\n";

  constexpr auto digits_per_set   = static_cast<std::size_t>(UINT8_C(10));
  constexpr auto digits_per_line  = static_cast<std::size_t>(digits_per_set  * static_cast<std::size_t>(UINT8_C(10)));
  constexpr auto digits_per_group = static_cast<std::size_t>(digits_per_line * static_cast<std::size_t>(UINT8_C(10)));

  // The digits after the decimal point are grouped
  // in sets of digits_per_set with digits_per_line
  // digits per line. The running-digit count is reported
  // at the end of each line.
  
  // The char_set_separator character string is inserted
  // between sets of digits. Between groups of lines,
  // we insert a char_group_separator character string
  // (which likely might be selected as a newline).

  std::string::size_type pos { };

  if(   ((pos = str_val.find('0', static_cast<std::size_t>(UINT8_C(0)))) != std::string::npos)
     && ((pos = str_val.find('.', static_cast<std::size_t>(UINT8_C(1)))) != std::string::npos)
     && ((pos = str_val.find('6', static_cast<std::size_t>(UINT8_C(2)))) != std::string::npos))
  {
    os << "ln2= " << str_val.substr(static_cast<std::string::size_type>(UINT8_C(0)), pos);
  }
  else
  {
    pos = static_cast<std::string::size_type>(UINT8_C(0));
  }

  const auto digit_offset = pos;

  // Extract the digits after the decimal point in a loop.
  // Insert spaces, newlines and a running-digit count
  // in order to create a format for comfortable reading.

  auto all_output_streaming_is_finished = false;

  while(all_output_streaming_is_finished == false)
  {
    // Print a set of digits (i.e. having 10 digits per set).
    const std::string str_val_substring(str_val.substr(pos, digits_per_set));

    os << str_val_substring << char_set_separator;

    {
      const auto min_ofs = (std::min)(static_cast<std::string::size_type>(digits_per_set), str_val_substring.length());

      pos += min_ofs;
    }

    const auto number_of_digits = static_cast<std::size_t>(pos - digit_offset);

    // Check if all output streaming is finished.
    all_output_streaming_is_finished = (pos >= str_val.length());

    if(all_output_streaming_is_finished)
    {
      // Write the final digit count.
      // Break from the printing loop.
      // Flush the output stream with std::endl.

      os << ": " << number_of_digits << std::endl;
    }
    else
    {
      const auto this_line_is_finished =
        (static_cast<std::size_t>(number_of_digits % digits_per_line) == static_cast<std::size_t>(UINT8_C(0)));

      if(this_line_is_finished)
      {
        // Print the running-digit count and start a new line.
        os << ": " << number_of_digits << '\n';

        const auto this_group_of_lines_is_finished =
          (static_cast<std::size_t>(number_of_digits % digits_per_group) == static_cast<std::size_t>(UINT8_C(0)));

        if(this_group_of_lines_is_finished)
        {
          // Insert a character (which might be a blank line)
          // after a group of lines.
          os << char_group_separator;
        }

        // Insert spaces at the start of the new line.
        os << "       ";
      }
    }
  }

  return result_str_val_is_ok;
}
