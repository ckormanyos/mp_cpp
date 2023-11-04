///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : pi.cpp
// 
// Project     : Samples for multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Compute pi and print its value to a text file.
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
  void print_pi_timing_report(std::ostream& os, const double time_for_pi_calculation);
  void print_pi_output_result(std::ostream& os, const double time_for_pi_calculation);

  const mp::mp_cpp& my_pi(const bool b_trace, std::ostream& os = std::cout);

  int& my_pi_computation_method()
  {
    static int the_method;

    return the_method;
  }
}

const mp::mp_cpp& local::my_pi(const bool b_trace, std::ostream& os)
{
  if     (my_pi_computation_method() == 3) { return mp::mp_cpp::calculate_pi_borwein_quintic(b_trace, os); }
  else if(my_pi_computation_method() == 2) { return mp::mp_cpp::calculate_pi_borwein_quartic(b_trace, os); }
  else if(my_pi_computation_method() == 1) { return mp::mp_cpp::calculate_pi_borwein_cubic  (b_trace, os); }
  else                                     { return mp::mp_cpp::calculate_pi                (b_trace, os); }
}

// *****************************************************************************
// Function    : bool pi(const int argc, const char* argv[])
// 
// Description : Multiple precision calculation of pi based on
//               command line user input of various characteristics.
// 
// *****************************************************************************
bool samples::pi(const int argc, const char* argv[])
{
  std::string::size_type pos;

  std::int32_t my_digits10    = 1000000;
  int          my_fft_threads =       4;

  std::stringstream ss;

  for(std::int32_t n = std::int32_t(0); n < std::int32_t(argc); ++n)
  {
    const std::string cmd_str = argv[n];

    // Extract the number of FFT threads.
    // This command has the form -t4, for example,
    // for 4 FFT threads.
    if((pos = cmd_str.find("-t")) != std::string::npos)
    {
      ss << cmd_str.c_str() + (pos + 2U);

      ss >> my_fft_threads;

      ss.str(std::string());
      ss.clear();
    }

    // Extract the number of decimal digits following
    // the decimal point. This command has the form -d1000000,
    // for example, for one million decimal digits after
    // the decimal point.
    if((pos = cmd_str.find("-d")) != std::string::npos)
    {
      ss << cmd_str.c_str() + (pos + 2U);

      ss >> my_digits10;

      ss.str(std::string());
      ss.clear();
    }

    // Extract the number of FFT threads.
    // This command has the form -m1, for example,
    // for calculation method 1.
    if((pos = cmd_str.find("-m")) != std::string::npos)
    {
      ss << cmd_str.c_str() + (pos + 2U);

      ss >> local::my_pi_computation_method();

      ss.str(std::string());
      ss.clear();
    }
  }

  bool calculate_pi_is_ok = false;

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

      if((pos = str_outfile.rfind('\\')) != std::string::npos)
      {
        str_outfile = str_outfile.substr(0U, pos + 1U) + "pi.out";
      }

      if((pos = str_outfile.rfind('/')) != std::string::npos)
      {
        str_outfile = str_outfile.substr(0U, pos + 1U) + "pi.out";
      }
    }
    else
    {
      str_outfile = "pi.out";
    }

    std::ofstream outfile(str_outfile.c_str());

    if(outfile.is_open())
    {
      // Start the time measurement.
      const std::clock_t t0 = std::clock();

      // Calculate pi with the user output trace flag set to true.
      local::my_pi(true);

      // End the time measurement.
      const double time_for_pi_calculation =
        double(std::clock() - t0) / double(CLOCKS_PER_SEC);

      // Print the calculation time to the standard output.
      local::print_pi_timing_report(std::cout, time_for_pi_calculation);

      // Print the result of the pi calculation to the output file.
      local::print_pi_output_result(outfile, time_for_pi_calculation);

      outfile.close();

      calculate_pi_is_ok = true;
    }
  }

  return (create_mp_base_is_ok && calculate_pi_is_ok);
}

// *****************************************************************************
// Function    : static void pi_timing_report(std::ostream& os)
// 
// Description : Report the timing of the pi calculation.
// 
// *****************************************************************************
void local::print_pi_timing_report(std::ostream& os, const double time_for_pi_calculation)
{
  os << std::endl
     << "Time for pi calculation: "
     << std::numeric_limits<mp::mp_cpp>::digits10
     << " digits in "
     << std::setprecision(4)
     << time_for_pi_calculation
     << " seconds."
     << std::endl
     << std::endl;
}

// *****************************************************************************
// Function    : static void print_pi_output(std::ostream& os)
// 
// Description : Print the output of pi with a readable format.
// 
// *****************************************************************************
void local::print_pi_output_result(std::ostream& os, const double time_for_pi_calculation)
{
  // Print the calculation time to the output stream.
  local::print_pi_timing_report(os, time_for_pi_calculation);

  std::stringstream ss;

  // Pipe the value of pi into a stringstream object with full precision.
  ss << std::fixed
     << std::setprecision(std::numeric_limits<mp::mp_cpp>::digits10)
     << local::my_pi(false);

  // Extract the string value of pi.
  const std::string str_pi(ss.str());

  // Print pi using the following paramater-tunable format.

  // pi = 3.1415926535 8979323846 2643383279 5028841971 6939937510 : 50
  //        5820974944 5923078164 0628620899 8628034825 3421170679 : 100
  //        8214808651 3282306647 0938446095 5058223172 5359408128 : 150
  //        4811174502 8410270193 8521105559 6446229489 5493038196 : 200
  //        ...

  const char* char_set_separator   = " ";
  const char* char_group_separator = "\n";
  //const char* char_set_separator   = "";
  //const char* char_group_separator = "";

  const std::size_t digits_per_set   = 10U;
  const std::size_t digits_per_line  = digits_per_set * 10U;
  const std::size_t digits_per_group = digits_per_line * 10U;

  // The digits after the decimal point are grouped
  // in sets of digits_per_set with digits_per_line
  // digits per line. The running-digit count is reported
  // at the end of each line.
  
  // The char_set_separator character string is inserted
  // between sets of digits. Between groups of lines,
  // we insert a char_group_separator character string
  // (which likely might be selected as a newline).

  // For a simple verification of 1,000,000 digits,
  // for example, go to Wolfram Alpha and ask:
  //   1000000th digit of Pi.
  // This prints out 50 digits of pi in the neighborhood
  // of a million digits, with the millionth digit in bold.

  std::string::size_type pos;

  if(   ((pos = str_pi.find('3', 0U)) != std::string::npos)
     && ((pos = str_pi.find('.', 1U)) != std::string::npos)
     && ((pos = str_pi.find('1', 1U)) != std::string::npos))
  {
    ;
  }
  else
  {
    pos = 0U;
  }

  os << "pi = " << str_pi.substr(0U, pos);

  const std::size_t digit_offset = pos;

  // Extract the digits after the decimal point in a loop.
  // Insert spaces, newlines and a running-digit count
  // in order to create a format for comfortable reading.

  bool all_output_streaming_is_finished = false;

  while(all_output_streaming_is_finished == false)
  {
    // Print a set of digits (i.e. having 10 digits per set).
    const std::string str_pi_substring(str_pi.substr(pos, digits_per_set));

    os << str_pi_substring << char_set_separator;

    pos += (std::min)(std::string::size_type(digits_per_set),
                      str_pi_substring.length());

    const std::size_t number_of_digits(pos - digit_offset);

    // Check if all output streaming is finished.
    all_output_streaming_is_finished = (pos >= str_pi.length());

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
        (std::size_t(number_of_digits % digits_per_line) == std::size_t(0U));

      if(this_line_is_finished)
      {
        // Print the running-digit count and start a new line.
        os << ": " << number_of_digits << '\n';

        const bool this_group_of_lines_is_finished =
          (std::size_t(number_of_digits % digits_per_group) == std::size_t(0U));

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
}
