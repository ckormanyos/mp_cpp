///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
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
// Date        : 1999 - 2019
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

namespace local
{
  void print_ln2_timing_report(std::ostream& os, const double time_for_ln2_calculation);
  void print_ln2_output_result(std::ostream& os, const double time_for_ln2_calculation);
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
  std::string::size_type pos;

  std::int32_t my_digits10    = 1000000;
  int          my_fft_threads =       4;

  for(std::int32_t n = std::int32_t(0); n < std::int32_t(argc); ++n)
  {
    const std::string cmd_str = argv[n];

    // Extract the number of FFT threads.
    // The command has the form -t4, for example, for 4 FFT threads.
    if((pos = cmd_str.find("-t")) != std::string::npos)
    {
      std::stringstream ss;

      ss << cmd_str.c_str() + (pos + 2U);

      ss >> my_fft_threads;
    }

    // Extract the digit command which has, for example,
    // the form -d1000000 for one million decimal digits.
    if((pos = cmd_str.find("-d")) != std::string::npos)
    {
      std::stringstream ss;

      ss << cmd_str.c_str() + (pos + 2U);

      ss >> my_digits10;
    }
  }

  bool calculate_ln2_is_ok = false;

  // Initialize the multiple precision base.
  // Set the decimal precision and the number of FFT threads.

  const bool create_mp_base_is_ok =
    mp::mp_base::create_mp_base(my_digits10, my_fft_threads);

  if(create_mp_base_is_ok)
  {
    // Get the output file path relative to the path
    // of the executable program.

    std::string str_outfile = std::string(argv[0U]);

    if((pos = str_outfile.rfind(".exe")) != std::string::npos)
    {
      str_outfile = str_outfile.substr(0U, pos + 1U) + "out";

      if((pos = str_outfile.rfind(char('\\'))) != std::string::npos)
      {
        str_outfile = str_outfile.substr(0U, pos + 1U) + "ln2.out";
      }

      if((pos = str_outfile.rfind(char('/'))) != std::string::npos)
      {
        str_outfile = str_outfile.substr(0U, pos + 1U) + "ln2.out";
      }
    }
    else
    {
      str_outfile = "ln2.out";
    }

    std::ofstream outfile(str_outfile.c_str());

    if(outfile.is_open())
    {
      // Start the time measurement.
      const std::clock_t t0 = std::clock();

      // Calculate ln2 with the user output trace flag set to true.
      mp::mp_cpp::calculate_ln2(true);

      // End the time measurement.
      const double time_for_ln2_calculation =
        double(std::clock() - t0) / double(CLOCKS_PER_SEC);

      // Print the calculation time to the standard output.
      local::print_ln2_timing_report(std::cout, time_for_ln2_calculation);

      // Print the result of the ln2 calculation to the output file.
      local::print_ln2_output_result(outfile, time_for_ln2_calculation);

      outfile.close();

      calculate_ln2_is_ok = true;
    }
  }

  return (create_mp_base_is_ok && calculate_ln2_is_ok);
}

// *****************************************************************************
// Function    : static void ln2_timing_report(std::ostream& os)
// 
// Description : Report the timing of the ln2 calculation.
// 
// *****************************************************************************
void local::print_ln2_timing_report(std::ostream& os, const double time_for_ln2_calculation)
{
  os << std::endl
     << "Time for ln2 calculation: "
     << std::numeric_limits<mp::mp_cpp>::digits10
     << " digits in "
     << std::setprecision(4)
     << time_for_ln2_calculation
     << " seconds."
     << std::endl
     << std::endl;
}

// *****************************************************************************
// Function    : static void print_ln2_output(std::ostream& os)
// 
// Description : Print the output of ln2 with a readable format.
// 
// *****************************************************************************
void local::print_ln2_output_result(std::ostream& os, const double time_for_ln2_calculation)
{
  // Print the calculation time to the output stream.
  local::print_ln2_timing_report(os, time_for_ln2_calculation);

  std::stringstream ss;

  // Pipe the value of ln2 into a stringstream object with full precision.
  ss << std::fixed
     << std::setprecision(std::numeric_limits<mp::mp_cpp>::digits10)
     << mp::ln2();

  // Extract the string value of ln2.
  const std::string str_ln2(ss.str());

  // Print ln2 using the following format.

  // ln2 = 0.6931471805 5994530941 7232121458 1765680755 0013436025 : 50
  //         5254120680 0094933936 2196969471 5605863326 9964186875 : 100
  //         4200148102 0570685733 6855202357 5813055703 2670751635 : 150
  //         0759619307 2757082837 1435190307 0386238916 7347112335 : 200
  //         ...

  // The digits after the decimal point are grouped
  // in sets of 10 digits with 50 digits per line.
  // The running-digit count is reported at the end
  // of each line. An additional newline is inserted
  // after groups of ten lines.

  // For a simple verification of 1,000,000 digits,
  // for example, go to Wolfram Alpha and ask:
  //   1000000th digit of log(2).
  // This prints out 50 digits of ln2 in the neighborhood
  // of a million digits, with the millionth digit in bold.

  std::string::size_type pos;

  if(   ((pos = str_ln2.find(char('0'), 0U)) != std::string::npos)
     && ((pos = str_ln2.find(char('.'), 1U)) != std::string::npos)
     && ((pos = str_ln2.find(char('6'), 1U)) != std::string::npos))
  {
    ;
  }
  else
  {
    pos = 0U;
  }

  os << "ln2 = " << str_ln2.substr(0U, pos);

  const std::size_t digit_offset = pos;

  // Extract the digits after the decimal point in a loop.
  // Insert spaces, newlines and a running-digit count
  // in order to create a format for comfortable reading.

  bool all_output_streaming_is_finished = false;

  while(all_output_streaming_is_finished == false)
  {
    // Print a set of ten digits.
    os << str_ln2.substr(pos, 10U) << char(' ');

    pos += 10U;

    const std::size_t number_of_digits(pos - digit_offset);

    // Check if all output streaming is finished.
    all_output_streaming_is_finished = (pos >= str_ln2.length());

    if(all_output_streaming_is_finished)
    {
      // Write the final digit count.
      // Break from the printing loop.
      // Flush the output stream with std::endl.

      os << ": " << number_of_digits << std::endl;
    }
    else
    {
      const bool this_line_is_finished =
        (std::size_t(number_of_digits % 50U) == std::size_t(0U));

      if(this_line_is_finished)
      {
        // Print the running-digit count and start a new line.
        os << ": " << number_of_digits << char('\n');

        const bool this_group_of_ten_lines_is_finished =
          (std::size_t(number_of_digits % 500U) == std::size_t(0U));

        if(this_group_of_ten_lines_is_finished)
        {
          // Insert a new line after a group of ten lines.
          os << char('\n');
        }

        // Insert spaces at the start of the new line.
        os << "        ";
      }
    }
  }
}
