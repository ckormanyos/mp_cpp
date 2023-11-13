///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2023.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// *****************************************************************************
// Filename    : mp_core.cpp
// 
// Project     : Multiple precision math
// 
// Author      : Christopher Kormanyos
// Owner       : Christopher Kormanyos
// 
// Date        : 1999 - 2023
// 
// Description : Implementation of mp_core multiple precision class.
// 
// *****************************************************************************

#include <algorithm>
#include <array>
#include <limits>

#include <mp/mp_core.h>
#include <mp/mp_cpp_limits.h>
#include <mp/mp_fft/mp_fft_fftw2.h>

auto mp::mp_core::create_mp_core() -> bool
{
  mp_core::forward_set_of_mp_cpp_numeric_limits(digit_characteristics.mp_digits10());

  auto result_is_ok = true;

  // Allocate the memory needed for the FFT arrays.
  // Allocate enough for the largest one of the FFTs.
  // But use parts of the same memory for all of them.
  if(digit_characteristics.mp_elem_number() < mp_elem_fft_min)
  {
    mp_core_memory = new (std::nothrow) mp_core_memory_type(static_cast<std::size_t>(digit_characteristics.mp_elem_number()));

    const auto result_new_is_ok = (mp_core_memory != nullptr);

    result_is_ok = (result_new_is_ok && result_is_ok);
  }
  else
  {
    // Select FFT lengths that run quickly with FFTW.

    // First generate a table of so-called 5-smooth numbers.
    // FFTW is generally efficient with FFTs having lengths
    // of 5-smooth numbers. Dynamically select the FFT length
    // by choosing the nearest 5-smooth number that is greater
    // than or equal to the size of the half-complex input FFT
    // data array.

    // Create a constant vector of even-valued 5-smooth numbers
    // up to and including 5*10^8. The 5-smooth numbers are given
    // by (2^i * 3^j * 5^k). 5-smooth numbers can be generated
    // quickly with Mathematica using:
    // mx = 500000000; Sort@ Flatten@Table[2^a*3^b*5^c, {a, 0, Log[2, mx]}, {b, 0, Log[3, mx/2^a]}, {c, 0, Log[5, mx/(2^a*3^b)]}]
    // See Sloane's A051037 for more information.

    constexpr auto A051037_even_array_size =
      static_cast<std::size_t>
      (
        static_cast<std::uint32_t>
        (
          static_cast<std::uint32_t>
          (
              static_cast<std::uint32_t>(UINT8_C(79))
            * static_cast<std::uint32_t>(UINT8_C(16))
          )
          - static_cast<std::uint32_t>(UINT8_C(1))
        )
      );

    using A051037_even_array_type = std::array<std::uint32_t, A051037_even_array_size>;

    constexpr A051037_even_array_type A051037_even =
    {{
          UINT32_C(        2),   UINT32_C(        4),   UINT32_C(        6),   UINT32_C(        8),   UINT32_C(       10),   UINT32_C(       12),   UINT32_C(       16),   UINT32_C(       18),   UINT32_C(       20),   UINT32_C(       24),   UINT32_C(       30),   UINT32_C(       32),   UINT32_C(       36),   UINT32_C(       40),   UINT32_C(       48),   UINT32_C(       50),
          UINT32_C(       54),   UINT32_C(       60),   UINT32_C(       64),   UINT32_C(       72),   UINT32_C(       80),   UINT32_C(       90),   UINT32_C(       96),   UINT32_C(      100),   UINT32_C(      108),   UINT32_C(      120),   UINT32_C(      128),   UINT32_C(      144),   UINT32_C(      150),   UINT32_C(      160),   UINT32_C(      162),   UINT32_C(      180),
          UINT32_C(      192),   UINT32_C(      200),   UINT32_C(      216),   UINT32_C(      240),   UINT32_C(      250),   UINT32_C(      256),   UINT32_C(      270),   UINT32_C(      288),   UINT32_C(      300),   UINT32_C(      320),   UINT32_C(      324),   UINT32_C(      360),   UINT32_C(      384),   UINT32_C(      400),   UINT32_C(      432),   UINT32_C(      450),
          UINT32_C(      480),   UINT32_C(      486),   UINT32_C(      500),   UINT32_C(      512),   UINT32_C(      540),   UINT32_C(      576),   UINT32_C(      600),   UINT32_C(      640),   UINT32_C(      648),   UINT32_C(      720),   UINT32_C(      750),   UINT32_C(      768),   UINT32_C(      800),   UINT32_C(      810),   UINT32_C(      864),   UINT32_C(      900),
          UINT32_C(      960),   UINT32_C(      972),   UINT32_C(     1000),   UINT32_C(     1024),   UINT32_C(     1080),   UINT32_C(     1152),   UINT32_C(     1200),   UINT32_C(     1250),   UINT32_C(     1280),   UINT32_C(     1296),   UINT32_C(     1350),   UINT32_C(     1440),   UINT32_C(     1458),   UINT32_C(     1500),   UINT32_C(     1536),   UINT32_C(     1600),
          UINT32_C(     1620),   UINT32_C(     1728),   UINT32_C(     1800),   UINT32_C(     1920),   UINT32_C(     1944),   UINT32_C(     2000),   UINT32_C(     2048),   UINT32_C(     2160),   UINT32_C(     2250),   UINT32_C(     2304),   UINT32_C(     2400),   UINT32_C(     2430),   UINT32_C(     2500),   UINT32_C(     2560),   UINT32_C(     2592),   UINT32_C(     2700),
          UINT32_C(     2880),   UINT32_C(     2916),   UINT32_C(     3000),   UINT32_C(     3072),   UINT32_C(     3200),   UINT32_C(     3240),   UINT32_C(     3456),   UINT32_C(     3600),   UINT32_C(     3750),   UINT32_C(     3840),   UINT32_C(     3888),   UINT32_C(     4000),   UINT32_C(     4050),   UINT32_C(     4096),   UINT32_C(     4320),   UINT32_C(     4374),
          UINT32_C(     4500),   UINT32_C(     4608),   UINT32_C(     4800),   UINT32_C(     4860),   UINT32_C(     5000),   UINT32_C(     5120),   UINT32_C(     5184),   UINT32_C(     5400),   UINT32_C(     5760),   UINT32_C(     5832),   UINT32_C(     6000),   UINT32_C(     6144),   UINT32_C(     6250),   UINT32_C(     6400),   UINT32_C(     6480),   UINT32_C(     6750),
          UINT32_C(     6912),   UINT32_C(     7200),   UINT32_C(     7290),   UINT32_C(     7500),   UINT32_C(     7680),   UINT32_C(     7776),   UINT32_C(     8000),   UINT32_C(     8100),   UINT32_C(     8192),   UINT32_C(     8640),   UINT32_C(     8748),   UINT32_C(     9000),   UINT32_C(     9216),   UINT32_C(     9600),   UINT32_C(     9720),   UINT32_C(    10000),
          UINT32_C(    10240),   UINT32_C(    10368),   UINT32_C(    10800),   UINT32_C(    11250),   UINT32_C(    11520),   UINT32_C(    11664),   UINT32_C(    12000),   UINT32_C(    12150),   UINT32_C(    12288),   UINT32_C(    12500),   UINT32_C(    12800),   UINT32_C(    12960),   UINT32_C(    13122),   UINT32_C(    13500),   UINT32_C(    13824),   UINT32_C(    14400),
          UINT32_C(    14580),   UINT32_C(    15000),   UINT32_C(    15360),   UINT32_C(    15552),   UINT32_C(    16000),   UINT32_C(    16200),   UINT32_C(    16384),   UINT32_C(    17280),   UINT32_C(    17496),   UINT32_C(    18000),   UINT32_C(    18432),   UINT32_C(    18750),   UINT32_C(    19200),   UINT32_C(    19440),   UINT32_C(    20000),   UINT32_C(    20250),
          UINT32_C(    20480),   UINT32_C(    20736),   UINT32_C(    21600),   UINT32_C(    21870),   UINT32_C(    22500),   UINT32_C(    23040),   UINT32_C(    23328),   UINT32_C(    24000),   UINT32_C(    24300),   UINT32_C(    24576),   UINT32_C(    25000),   UINT32_C(    25600),   UINT32_C(    25920),   UINT32_C(    26244),   UINT32_C(    27000),   UINT32_C(    27648),
          UINT32_C(    28800),   UINT32_C(    29160),   UINT32_C(    30000),   UINT32_C(    30720),   UINT32_C(    31104),   UINT32_C(    31250),   UINT32_C(    32000),   UINT32_C(    32400),   UINT32_C(    32768),   UINT32_C(    33750),   UINT32_C(    34560),   UINT32_C(    34992),   UINT32_C(    36000),   UINT32_C(    36450),   UINT32_C(    36864),   UINT32_C(    37500),
          UINT32_C(    38400),   UINT32_C(    38880),   UINT32_C(    39366),   UINT32_C(    40000),   UINT32_C(    40500),   UINT32_C(    40960),   UINT32_C(    41472),   UINT32_C(    43200),   UINT32_C(    43740),   UINT32_C(    45000),   UINT32_C(    46080),   UINT32_C(    46656),   UINT32_C(    48000),   UINT32_C(    48600),   UINT32_C(    49152),   UINT32_C(    50000),
          UINT32_C(    51200),   UINT32_C(    51840),   UINT32_C(    52488),   UINT32_C(    54000),   UINT32_C(    55296),   UINT32_C(    56250),   UINT32_C(    57600),   UINT32_C(    58320),   UINT32_C(    60000),   UINT32_C(    60750),   UINT32_C(    61440),   UINT32_C(    62208),   UINT32_C(    62500),   UINT32_C(    64000),   UINT32_C(    64800),   UINT32_C(    65536),
          UINT32_C(    65610),   UINT32_C(    67500),   UINT32_C(    69120),   UINT32_C(    69984),   UINT32_C(    72000),   UINT32_C(    72900),   UINT32_C(    73728),   UINT32_C(    75000),   UINT32_C(    76800),   UINT32_C(    77760),   UINT32_C(    78732),   UINT32_C(    80000),   UINT32_C(    81000),   UINT32_C(    81920),   UINT32_C(    82944),   UINT32_C(    86400),
          UINT32_C(    87480),   UINT32_C(    90000),   UINT32_C(    92160),   UINT32_C(    93312),   UINT32_C(    93750),   UINT32_C(    96000),   UINT32_C(    97200),   UINT32_C(    98304),   UINT32_C(   100000),   UINT32_C(   101250),   UINT32_C(   102400),   UINT32_C(   103680),   UINT32_C(   104976),   UINT32_C(   108000),   UINT32_C(   109350),   UINT32_C(   110592),
          UINT32_C(   112500),   UINT32_C(   115200),   UINT32_C(   116640),   UINT32_C(   118098),   UINT32_C(   120000),   UINT32_C(   121500),   UINT32_C(   122880),   UINT32_C(   124416),   UINT32_C(   125000),   UINT32_C(   128000),   UINT32_C(   129600),   UINT32_C(   131072),   UINT32_C(   131220),   UINT32_C(   135000),   UINT32_C(   138240),   UINT32_C(   139968),
          UINT32_C(   144000),   UINT32_C(   145800),   UINT32_C(   147456),   UINT32_C(   150000),   UINT32_C(   153600),   UINT32_C(   155520),   UINT32_C(   156250),   UINT32_C(   157464),   UINT32_C(   160000),   UINT32_C(   162000),   UINT32_C(   163840),   UINT32_C(   165888),   UINT32_C(   168750),   UINT32_C(   172800),   UINT32_C(   174960),   UINT32_C(   180000),
          UINT32_C(   182250),   UINT32_C(   184320),   UINT32_C(   186624),   UINT32_C(   187500),   UINT32_C(   192000),   UINT32_C(   194400),   UINT32_C(   196608),   UINT32_C(   196830),   UINT32_C(   200000),   UINT32_C(   202500),   UINT32_C(   204800),   UINT32_C(   207360),   UINT32_C(   209952),   UINT32_C(   216000),   UINT32_C(   218700),   UINT32_C(   221184),
          UINT32_C(   225000),   UINT32_C(   230400),   UINT32_C(   233280),   UINT32_C(   236196),   UINT32_C(   240000),   UINT32_C(   243000),   UINT32_C(   245760),   UINT32_C(   248832),   UINT32_C(   250000),   UINT32_C(   256000),   UINT32_C(   259200),   UINT32_C(   262144),   UINT32_C(   262440),   UINT32_C(   270000),   UINT32_C(   276480),   UINT32_C(   279936),
          UINT32_C(   281250),   UINT32_C(   288000),   UINT32_C(   291600),   UINT32_C(   294912),   UINT32_C(   300000),   UINT32_C(   303750),   UINT32_C(   307200),   UINT32_C(   311040),   UINT32_C(   312500),   UINT32_C(   314928),   UINT32_C(   320000),   UINT32_C(   324000),   UINT32_C(   327680),   UINT32_C(   328050),   UINT32_C(   331776),   UINT32_C(   337500),
          UINT32_C(   345600),   UINT32_C(   349920),   UINT32_C(   354294),   UINT32_C(   360000),   UINT32_C(   364500),   UINT32_C(   368640),   UINT32_C(   373248),   UINT32_C(   375000),   UINT32_C(   384000),   UINT32_C(   388800),   UINT32_C(   393216),   UINT32_C(   393660),   UINT32_C(   400000),   UINT32_C(   405000),   UINT32_C(   409600),   UINT32_C(   414720),
          UINT32_C(   419904),   UINT32_C(   432000),   UINT32_C(   437400),   UINT32_C(   442368),   UINT32_C(   450000),   UINT32_C(   460800),   UINT32_C(   466560),   UINT32_C(   468750),   UINT32_C(   472392),   UINT32_C(   480000),   UINT32_C(   486000),   UINT32_C(   491520),   UINT32_C(   497664),   UINT32_C(   500000),   UINT32_C(   506250),   UINT32_C(   512000),
          UINT32_C(   518400),   UINT32_C(   524288),   UINT32_C(   524880),   UINT32_C(   540000),   UINT32_C(   546750),   UINT32_C(   552960),   UINT32_C(   559872),   UINT32_C(   562500),   UINT32_C(   576000),   UINT32_C(   583200),   UINT32_C(   589824),   UINT32_C(   590490),   UINT32_C(   600000),   UINT32_C(   607500),   UINT32_C(   614400),   UINT32_C(   622080),
          UINT32_C(   625000),   UINT32_C(   629856),   UINT32_C(   640000),   UINT32_C(   648000),   UINT32_C(   655360),   UINT32_C(   656100),   UINT32_C(   663552),   UINT32_C(   675000),   UINT32_C(   691200),   UINT32_C(   699840),   UINT32_C(   708588),   UINT32_C(   720000),   UINT32_C(   729000),   UINT32_C(   737280),   UINT32_C(   746496),   UINT32_C(   750000),
          UINT32_C(   768000),   UINT32_C(   777600),   UINT32_C(   781250),   UINT32_C(   786432),   UINT32_C(   787320),   UINT32_C(   800000),   UINT32_C(   810000),   UINT32_C(   819200),   UINT32_C(   829440),   UINT32_C(   839808),   UINT32_C(   843750),   UINT32_C(   864000),   UINT32_C(   874800),   UINT32_C(   884736),   UINT32_C(   900000),   UINT32_C(   911250),
          UINT32_C(   921600),   UINT32_C(   933120),   UINT32_C(   937500),   UINT32_C(   944784),   UINT32_C(   960000),   UINT32_C(   972000),   UINT32_C(   983040),   UINT32_C(   984150),   UINT32_C(   995328),   UINT32_C(  1000000),   UINT32_C(  1012500),   UINT32_C(  1024000),   UINT32_C(  1036800),   UINT32_C(  1048576),   UINT32_C(  1049760),   UINT32_C(  1062882),
          UINT32_C(  1080000),   UINT32_C(  1093500),   UINT32_C(  1105920),   UINT32_C(  1119744),   UINT32_C(  1125000),   UINT32_C(  1152000),   UINT32_C(  1166400),   UINT32_C(  1179648),   UINT32_C(  1180980),   UINT32_C(  1200000),   UINT32_C(  1215000),   UINT32_C(  1228800),   UINT32_C(  1244160),   UINT32_C(  1250000),   UINT32_C(  1259712),   UINT32_C(  1280000),
          UINT32_C(  1296000),   UINT32_C(  1310720),   UINT32_C(  1312200),   UINT32_C(  1327104),   UINT32_C(  1350000),   UINT32_C(  1382400),   UINT32_C(  1399680),   UINT32_C(  1406250),   UINT32_C(  1417176),   UINT32_C(  1440000),   UINT32_C(  1458000),   UINT32_C(  1474560),   UINT32_C(  1492992),   UINT32_C(  1500000),   UINT32_C(  1518750),   UINT32_C(  1536000),
          UINT32_C(  1555200),   UINT32_C(  1562500),   UINT32_C(  1572864),   UINT32_C(  1574640),   UINT32_C(  1600000),   UINT32_C(  1620000),   UINT32_C(  1638400),   UINT32_C(  1640250),   UINT32_C(  1658880),   UINT32_C(  1679616),   UINT32_C(  1687500),   UINT32_C(  1728000),   UINT32_C(  1749600),   UINT32_C(  1769472),   UINT32_C(  1771470),   UINT32_C(  1800000),
          UINT32_C(  1822500),   UINT32_C(  1843200),   UINT32_C(  1866240),   UINT32_C(  1875000),   UINT32_C(  1889568),   UINT32_C(  1920000),   UINT32_C(  1944000),   UINT32_C(  1966080),   UINT32_C(  1968300),   UINT32_C(  1990656),   UINT32_C(  2000000),   UINT32_C(  2025000),   UINT32_C(  2048000),   UINT32_C(  2073600),   UINT32_C(  2097152),   UINT32_C(  2099520),
          UINT32_C(  2125764),   UINT32_C(  2160000),   UINT32_C(  2187000),   UINT32_C(  2211840),   UINT32_C(  2239488),   UINT32_C(  2250000),   UINT32_C(  2304000),   UINT32_C(  2332800),   UINT32_C(  2343750),   UINT32_C(  2359296),   UINT32_C(  2361960),   UINT32_C(  2400000),   UINT32_C(  2430000),   UINT32_C(  2457600),   UINT32_C(  2488320),   UINT32_C(  2500000),
          UINT32_C(  2519424),   UINT32_C(  2531250),   UINT32_C(  2560000),   UINT32_C(  2592000),   UINT32_C(  2621440),   UINT32_C(  2624400),   UINT32_C(  2654208),   UINT32_C(  2700000),   UINT32_C(  2733750),   UINT32_C(  2764800),   UINT32_C(  2799360),   UINT32_C(  2812500),   UINT32_C(  2834352),   UINT32_C(  2880000),   UINT32_C(  2916000),   UINT32_C(  2949120),
          UINT32_C(  2952450),   UINT32_C(  2985984),   UINT32_C(  3000000),   UINT32_C(  3037500),   UINT32_C(  3072000),   UINT32_C(  3110400),   UINT32_C(  3125000),   UINT32_C(  3145728),   UINT32_C(  3149280),   UINT32_C(  3188646),   UINT32_C(  3200000),   UINT32_C(  3240000),   UINT32_C(  3276800),   UINT32_C(  3280500),   UINT32_C(  3317760),   UINT32_C(  3359232),
          UINT32_C(  3375000),   UINT32_C(  3456000),   UINT32_C(  3499200),   UINT32_C(  3538944),   UINT32_C(  3542940),   UINT32_C(  3600000),   UINT32_C(  3645000),   UINT32_C(  3686400),   UINT32_C(  3732480),   UINT32_C(  3750000),   UINT32_C(  3779136),   UINT32_C(  3840000),   UINT32_C(  3888000),   UINT32_C(  3906250),   UINT32_C(  3932160),   UINT32_C(  3936600),
          UINT32_C(  3981312),   UINT32_C(  4000000),   UINT32_C(  4050000),   UINT32_C(  4096000),   UINT32_C(  4147200),   UINT32_C(  4194304),   UINT32_C(  4199040),   UINT32_C(  4218750),   UINT32_C(  4251528),   UINT32_C(  4320000),   UINT32_C(  4374000),   UINT32_C(  4423680),   UINT32_C(  4478976),   UINT32_C(  4500000),   UINT32_C(  4556250),   UINT32_C(  4608000),
          UINT32_C(  4665600),   UINT32_C(  4687500),   UINT32_C(  4718592),   UINT32_C(  4723920),   UINT32_C(  4800000),   UINT32_C(  4860000),   UINT32_C(  4915200),   UINT32_C(  4920750),   UINT32_C(  4976640),   UINT32_C(  5000000),   UINT32_C(  5038848),   UINT32_C(  5062500),   UINT32_C(  5120000),   UINT32_C(  5184000),   UINT32_C(  5242880),   UINT32_C(  5248800),
          UINT32_C(  5308416),   UINT32_C(  5314410),   UINT32_C(  5400000),   UINT32_C(  5467500),   UINT32_C(  5529600),   UINT32_C(  5598720),   UINT32_C(  5625000),   UINT32_C(  5668704),   UINT32_C(  5760000),   UINT32_C(  5832000),   UINT32_C(  5898240),   UINT32_C(  5904900),   UINT32_C(  5971968),   UINT32_C(  6000000),   UINT32_C(  6075000),   UINT32_C(  6144000),
          UINT32_C(  6220800),   UINT32_C(  6250000),   UINT32_C(  6291456),   UINT32_C(  6298560),   UINT32_C(  6377292),   UINT32_C(  6400000),   UINT32_C(  6480000),   UINT32_C(  6553600),   UINT32_C(  6561000),   UINT32_C(  6635520),   UINT32_C(  6718464),   UINT32_C(  6750000),   UINT32_C(  6912000),   UINT32_C(  6998400),   UINT32_C(  7031250),   UINT32_C(  7077888),
          UINT32_C(  7085880),   UINT32_C(  7200000),   UINT32_C(  7290000),   UINT32_C(  7372800),   UINT32_C(  7464960),   UINT32_C(  7500000),   UINT32_C(  7558272),   UINT32_C(  7593750),   UINT32_C(  7680000),   UINT32_C(  7776000),   UINT32_C(  7812500),   UINT32_C(  7864320),   UINT32_C(  7873200),   UINT32_C(  7962624),   UINT32_C(  8000000),   UINT32_C(  8100000),
          UINT32_C(  8192000),   UINT32_C(  8201250),   UINT32_C(  8294400),   UINT32_C(  8388608),   UINT32_C(  8398080),   UINT32_C(  8437500),   UINT32_C(  8503056),   UINT32_C(  8640000),   UINT32_C(  8748000),   UINT32_C(  8847360),   UINT32_C(  8857350),   UINT32_C(  8957952),   UINT32_C(  9000000),   UINT32_C(  9112500),   UINT32_C(  9216000),   UINT32_C(  9331200),
          UINT32_C(  9375000),   UINT32_C(  9437184),   UINT32_C(  9447840),   UINT32_C(  9565938),   UINT32_C(  9600000),   UINT32_C(  9720000),   UINT32_C(  9830400),   UINT32_C(  9841500),   UINT32_C(  9953280),   UINT32_C( 10000000),   UINT32_C( 10077696),   UINT32_C( 10125000),   UINT32_C( 10240000),   UINT32_C( 10368000),   UINT32_C( 10485760),   UINT32_C( 10497600),
          UINT32_C( 10616832),   UINT32_C( 10628820),   UINT32_C( 10800000),   UINT32_C( 10935000),   UINT32_C( 11059200),   UINT32_C( 11197440),   UINT32_C( 11250000),   UINT32_C( 11337408),   UINT32_C( 11520000),   UINT32_C( 11664000),   UINT32_C( 11718750),   UINT32_C( 11796480),   UINT32_C( 11809800),   UINT32_C( 11943936),   UINT32_C( 12000000),   UINT32_C( 12150000),
          UINT32_C( 12288000),   UINT32_C( 12441600),   UINT32_C( 12500000),   UINT32_C( 12582912),   UINT32_C( 12597120),   UINT32_C( 12656250),   UINT32_C( 12754584),   UINT32_C( 12800000),   UINT32_C( 12960000),   UINT32_C( 13107200),   UINT32_C( 13122000),   UINT32_C( 13271040),   UINT32_C( 13436928),   UINT32_C( 13500000),   UINT32_C( 13668750),   UINT32_C( 13824000),
          UINT32_C( 13996800),   UINT32_C( 14062500),   UINT32_C( 14155776),   UINT32_C( 14171760),   UINT32_C( 14400000),   UINT32_C( 14580000),   UINT32_C( 14745600),   UINT32_C( 14762250),   UINT32_C( 14929920),   UINT32_C( 15000000),   UINT32_C( 15116544),   UINT32_C( 15187500),   UINT32_C( 15360000),   UINT32_C( 15552000),   UINT32_C( 15625000),   UINT32_C( 15728640),
          UINT32_C( 15746400),   UINT32_C( 15925248),   UINT32_C( 15943230),   UINT32_C( 16000000),   UINT32_C( 16200000),   UINT32_C( 16384000),   UINT32_C( 16402500),   UINT32_C( 16588800),   UINT32_C( 16777216),   UINT32_C( 16796160),   UINT32_C( 16875000),   UINT32_C( 17006112),   UINT32_C( 17280000),   UINT32_C( 17496000),   UINT32_C( 17694720),   UINT32_C( 17714700),
          UINT32_C( 17915904),   UINT32_C( 18000000),   UINT32_C( 18225000),   UINT32_C( 18432000),   UINT32_C( 18662400),   UINT32_C( 18750000),   UINT32_C( 18874368),   UINT32_C( 18895680),   UINT32_C( 19131876),   UINT32_C( 19200000),   UINT32_C( 19440000),   UINT32_C( 19531250),   UINT32_C( 19660800),   UINT32_C( 19683000),   UINT32_C( 19906560),   UINT32_C( 20000000),
          UINT32_C( 20155392),   UINT32_C( 20250000),   UINT32_C( 20480000),   UINT32_C( 20736000),   UINT32_C( 20971520),   UINT32_C( 20995200),   UINT32_C( 21093750),   UINT32_C( 21233664),   UINT32_C( 21257640),   UINT32_C( 21600000),   UINT32_C( 21870000),   UINT32_C( 22118400),   UINT32_C( 22394880),   UINT32_C( 22500000),   UINT32_C( 22674816),   UINT32_C( 22781250),
          UINT32_C( 23040000),   UINT32_C( 23328000),   UINT32_C( 23437500),   UINT32_C( 23592960),   UINT32_C( 23619600),   UINT32_C( 23887872),   UINT32_C( 24000000),   UINT32_C( 24300000),   UINT32_C( 24576000),   UINT32_C( 24603750),   UINT32_C( 24883200),   UINT32_C( 25000000),   UINT32_C( 25165824),   UINT32_C( 25194240),   UINT32_C( 25312500),   UINT32_C( 25509168),
          UINT32_C( 25600000),   UINT32_C( 25920000),   UINT32_C( 26214400),   UINT32_C( 26244000),   UINT32_C( 26542080),   UINT32_C( 26572050),   UINT32_C( 26873856),   UINT32_C( 27000000),   UINT32_C( 27337500),   UINT32_C( 27648000),   UINT32_C( 27993600),   UINT32_C( 28125000),   UINT32_C( 28311552),   UINT32_C( 28343520),   UINT32_C( 28697814),   UINT32_C( 28800000),
          UINT32_C( 29160000),   UINT32_C( 29491200),   UINT32_C( 29524500),   UINT32_C( 29859840),   UINT32_C( 30000000),   UINT32_C( 30233088),   UINT32_C( 30375000),   UINT32_C( 30720000),   UINT32_C( 31104000),   UINT32_C( 31250000),   UINT32_C( 31457280),   UINT32_C( 31492800),   UINT32_C( 31850496),   UINT32_C( 31886460),   UINT32_C( 32000000),   UINT32_C( 32400000),
          UINT32_C( 32768000),   UINT32_C( 32805000),   UINT32_C( 33177600),   UINT32_C( 33554432),   UINT32_C( 33592320),   UINT32_C( 33750000),   UINT32_C( 34012224),   UINT32_C( 34560000),   UINT32_C( 34992000),   UINT32_C( 35156250),   UINT32_C( 35389440),   UINT32_C( 35429400),   UINT32_C( 35831808),   UINT32_C( 36000000),   UINT32_C( 36450000),   UINT32_C( 36864000),
          UINT32_C( 37324800),   UINT32_C( 37500000),   UINT32_C( 37748736),   UINT32_C( 37791360),   UINT32_C( 37968750),   UINT32_C( 38263752),   UINT32_C( 38400000),   UINT32_C( 38880000),   UINT32_C( 39062500),   UINT32_C( 39321600),   UINT32_C( 39366000),   UINT32_C( 39813120),   UINT32_C( 40000000),   UINT32_C( 40310784),   UINT32_C( 40500000),   UINT32_C( 40960000),
          UINT32_C( 41006250),   UINT32_C( 41472000),   UINT32_C( 41943040),   UINT32_C( 41990400),   UINT32_C( 42187500),   UINT32_C( 42467328),   UINT32_C( 42515280),   UINT32_C( 43200000),   UINT32_C( 43740000),   UINT32_C( 44236800),   UINT32_C( 44286750),   UINT32_C( 44789760),   UINT32_C( 45000000),   UINT32_C( 45349632),   UINT32_C( 45562500),   UINT32_C( 46080000),
          UINT32_C( 46656000),   UINT32_C( 46875000),   UINT32_C( 47185920),   UINT32_C( 47239200),   UINT32_C( 47775744),   UINT32_C( 47829690),   UINT32_C( 48000000),   UINT32_C( 48600000),   UINT32_C( 49152000),   UINT32_C( 49207500),   UINT32_C( 49766400),   UINT32_C( 50000000),   UINT32_C( 50331648),   UINT32_C( 50388480),   UINT32_C( 50625000),   UINT32_C( 51018336),
          UINT32_C( 51200000),   UINT32_C( 51840000),   UINT32_C( 52428800),   UINT32_C( 52488000),   UINT32_C( 53084160),   UINT32_C( 53144100),   UINT32_C( 53747712),   UINT32_C( 54000000),   UINT32_C( 54675000),   UINT32_C( 55296000),   UINT32_C( 55987200),   UINT32_C( 56250000),   UINT32_C( 56623104),   UINT32_C( 56687040),   UINT32_C( 57395628),   UINT32_C( 57600000),
          UINT32_C( 58320000),   UINT32_C( 58593750),   UINT32_C( 58982400),   UINT32_C( 59049000),   UINT32_C( 59719680),   UINT32_C( 60000000),   UINT32_C( 60466176),   UINT32_C( 60750000),   UINT32_C( 61440000),   UINT32_C( 62208000),   UINT32_C( 62500000),   UINT32_C( 62914560),   UINT32_C( 62985600),   UINT32_C( 63281250),   UINT32_C( 63700992),   UINT32_C( 63772920),
          UINT32_C( 64000000),   UINT32_C( 64800000),   UINT32_C( 65536000),   UINT32_C( 65610000),   UINT32_C( 66355200),   UINT32_C( 67108864),   UINT32_C( 67184640),   UINT32_C( 67500000),   UINT32_C( 68024448),   UINT32_C( 68343750),   UINT32_C( 69120000),   UINT32_C( 69984000),   UINT32_C( 70312500),   UINT32_C( 70778880),   UINT32_C( 70858800),   UINT32_C( 71663616),
          UINT32_C( 72000000),   UINT32_C( 72900000),   UINT32_C( 73728000),   UINT32_C( 73811250),   UINT32_C( 74649600),   UINT32_C( 75000000),   UINT32_C( 75497472),   UINT32_C( 75582720),   UINT32_C( 75937500),   UINT32_C( 76527504),   UINT32_C( 76800000),   UINT32_C( 77760000),   UINT32_C( 78125000),   UINT32_C( 78643200),   UINT32_C( 78732000),   UINT32_C( 79626240),
          UINT32_C( 79716150),   UINT32_C( 80000000),   UINT32_C( 80621568),   UINT32_C( 81000000),   UINT32_C( 81920000),   UINT32_C( 82012500),   UINT32_C( 82944000),   UINT32_C( 83886080),   UINT32_C( 83980800),   UINT32_C( 84375000),   UINT32_C( 84934656),   UINT32_C( 85030560),   UINT32_C( 86093442),   UINT32_C( 86400000),   UINT32_C( 87480000),   UINT32_C( 88473600),
          UINT32_C( 88573500),   UINT32_C( 89579520),   UINT32_C( 90000000),   UINT32_C( 90699264),   UINT32_C( 91125000),   UINT32_C( 92160000),   UINT32_C( 93312000),   UINT32_C( 93750000),   UINT32_C( 94371840),   UINT32_C( 94478400),   UINT32_C( 95551488),   UINT32_C( 95659380),   UINT32_C( 96000000),   UINT32_C( 97200000),   UINT32_C( 97656250),   UINT32_C( 98304000),
          UINT32_C( 98415000),   UINT32_C( 99532800),   UINT32_C(100000000),   UINT32_C(100663296),   UINT32_C(100776960),   UINT32_C(101250000),   UINT32_C(102036672),   UINT32_C(102400000),   UINT32_C(103680000),   UINT32_C(104857600),   UINT32_C(104976000),   UINT32_C(105468750),   UINT32_C(106168320),   UINT32_C(106288200),   UINT32_C(107495424),   UINT32_C(108000000),
          UINT32_C(109350000),   UINT32_C(110592000),   UINT32_C(111974400),   UINT32_C(112500000),   UINT32_C(113246208),   UINT32_C(113374080),   UINT32_C(113906250),   UINT32_C(114791256),   UINT32_C(115200000),   UINT32_C(116640000),   UINT32_C(117187500),   UINT32_C(117964800),   UINT32_C(118098000),   UINT32_C(119439360),   UINT32_C(120000000),   UINT32_C(120932352),
          UINT32_C(121500000),   UINT32_C(122880000),   UINT32_C(123018750),   UINT32_C(124416000),   UINT32_C(125000000),   UINT32_C(125829120),   UINT32_C(125971200),   UINT32_C(126562500),   UINT32_C(127401984),   UINT32_C(127545840),   UINT32_C(128000000),   UINT32_C(129600000),   UINT32_C(131072000),   UINT32_C(131220000),   UINT32_C(132710400),   UINT32_C(132860250),
          UINT32_C(134217728),   UINT32_C(134369280),   UINT32_C(135000000),   UINT32_C(136048896),   UINT32_C(136687500),   UINT32_C(138240000),   UINT32_C(139968000),   UINT32_C(140625000),   UINT32_C(141557760),   UINT32_C(141717600),   UINT32_C(143327232),   UINT32_C(143489070),   UINT32_C(144000000),   UINT32_C(145800000),   UINT32_C(147456000),   UINT32_C(147622500),
          UINT32_C(149299200),   UINT32_C(150000000),   UINT32_C(150994944),   UINT32_C(151165440),   UINT32_C(151875000),   UINT32_C(153055008),   UINT32_C(153600000),   UINT32_C(155520000),   UINT32_C(156250000),   UINT32_C(157286400),   UINT32_C(157464000),   UINT32_C(159252480),   UINT32_C(159432300),   UINT32_C(160000000),   UINT32_C(161243136),   UINT32_C(162000000),
          UINT32_C(163840000),   UINT32_C(164025000),   UINT32_C(165888000),   UINT32_C(167772160),   UINT32_C(167961600),   UINT32_C(168750000),   UINT32_C(169869312),   UINT32_C(170061120),   UINT32_C(172186884),   UINT32_C(172800000),   UINT32_C(174960000),   UINT32_C(175781250),   UINT32_C(176947200),   UINT32_C(177147000),   UINT32_C(179159040),   UINT32_C(180000000),
          UINT32_C(181398528),   UINT32_C(182250000),   UINT32_C(184320000),   UINT32_C(186624000),   UINT32_C(187500000),   UINT32_C(188743680),   UINT32_C(188956800),   UINT32_C(189843750),   UINT32_C(191102976),   UINT32_C(191318760),   UINT32_C(192000000),   UINT32_C(194400000),   UINT32_C(195312500),   UINT32_C(196608000),   UINT32_C(196830000),   UINT32_C(199065600),
          UINT32_C(200000000),   UINT32_C(201326592),   UINT32_C(201553920),   UINT32_C(202500000),   UINT32_C(204073344),   UINT32_C(204800000),   UINT32_C(205031250),   UINT32_C(207360000),   UINT32_C(209715200),   UINT32_C(209952000),   UINT32_C(210937500),   UINT32_C(212336640),   UINT32_C(212576400),   UINT32_C(214990848),   UINT32_C(216000000),   UINT32_C(218700000),
          UINT32_C(221184000),   UINT32_C(221433750),   UINT32_C(223948800),   UINT32_C(225000000),   UINT32_C(226492416),   UINT32_C(226748160),   UINT32_C(227812500),   UINT32_C(229582512),   UINT32_C(230400000),   UINT32_C(233280000),   UINT32_C(234375000),   UINT32_C(235929600),   UINT32_C(236196000),   UINT32_C(238878720),   UINT32_C(239148450),   UINT32_C(240000000),
          UINT32_C(241864704),   UINT32_C(243000000),   UINT32_C(245760000),   UINT32_C(246037500),   UINT32_C(248832000),   UINT32_C(250000000),   UINT32_C(251658240),   UINT32_C(251942400),   UINT32_C(253125000),   UINT32_C(254803968),   UINT32_C(255091680),   UINT32_C(256000000),   UINT32_C(258280326),   UINT32_C(259200000),   UINT32_C(262144000),   UINT32_C(262440000),
          UINT32_C(265420800),   UINT32_C(265720500),   UINT32_C(268435456),   UINT32_C(268738560),   UINT32_C(270000000),   UINT32_C(272097792),   UINT32_C(273375000),   UINT32_C(276480000),   UINT32_C(279936000),   UINT32_C(281250000),   UINT32_C(283115520),   UINT32_C(283435200),   UINT32_C(286654464),   UINT32_C(286978140),   UINT32_C(288000000),   UINT32_C(291600000),
          UINT32_C(292968750),   UINT32_C(294912000),   UINT32_C(295245000),   UINT32_C(298598400),   UINT32_C(300000000),   UINT32_C(301989888),   UINT32_C(302330880),   UINT32_C(303750000),   UINT32_C(306110016),   UINT32_C(307200000),   UINT32_C(311040000),   UINT32_C(312500000),   UINT32_C(314572800),   UINT32_C(314928000),   UINT32_C(316406250),   UINT32_C(318504960),
          UINT32_C(318864600),   UINT32_C(320000000),   UINT32_C(322486272),   UINT32_C(324000000),   UINT32_C(327680000),   UINT32_C(328050000),   UINT32_C(331776000),   UINT32_C(335544320),   UINT32_C(335923200),   UINT32_C(337500000),   UINT32_C(339738624),   UINT32_C(340122240),   UINT32_C(341718750),   UINT32_C(344373768),   UINT32_C(345600000),   UINT32_C(349920000),
          UINT32_C(351562500),   UINT32_C(353894400),   UINT32_C(354294000),   UINT32_C(358318080),   UINT32_C(360000000),   UINT32_C(362797056),   UINT32_C(364500000),   UINT32_C(368640000),   UINT32_C(369056250),   UINT32_C(373248000),   UINT32_C(375000000),   UINT32_C(377487360),   UINT32_C(377913600),   UINT32_C(379687500),   UINT32_C(382205952),   UINT32_C(382637520),
          UINT32_C(384000000),   UINT32_C(388800000),   UINT32_C(390625000),   UINT32_C(393216000),   UINT32_C(393660000),   UINT32_C(398131200),   UINT32_C(398580750),   UINT32_C(400000000),   UINT32_C(402653184),   UINT32_C(403107840),   UINT32_C(405000000),   UINT32_C(408146688),   UINT32_C(409600000),   UINT32_C(410062500),   UINT32_C(414720000),   UINT32_C(419430400),
          UINT32_C(419904000),   UINT32_C(421875000),   UINT32_C(424673280),   UINT32_C(425152800),   UINT32_C(429981696),   UINT32_C(430467210),   UINT32_C(432000000),   UINT32_C(437400000),   UINT32_C(442368000),   UINT32_C(442867500),   UINT32_C(447897600),   UINT32_C(450000000),   UINT32_C(452984832),   UINT32_C(453496320),   UINT32_C(455625000),   UINT32_C(459165024),
          UINT32_C(460800000),   UINT32_C(466560000),   UINT32_C(468750000),   UINT32_C(471859200),   UINT32_C(472392000),   UINT32_C(477757440),   UINT32_C(478296900),   UINT32_C(480000000),   UINT32_C(483729408),   UINT32_C(486000000),   UINT32_C(488281250),   UINT32_C(491520000),   UINT32_C(492075000),   UINT32_C(497664000),   UINT32_C(500000000),
    }};

    static_assert(A051037_even.back() == UINT32_C(500000000), "Error: FFT array initialization seems incorrect.");

    // Dynamically create a container of FFT objects within a min/max range
    // appropriate for the dynamic precision of mp_cpp. Each FFT has a size
    // taken from the table of 5-smooth numbers above. Only even-numbered
    // FFT sizes are included.
    auto itr_n_lo =
      std::lower_bound(A051037_even.cbegin(),
                       A051037_even.cend() - static_cast<std::size_t>(UINT8_C(1)),
                       static_cast<std::uint32_t>((mp_elem_fft_min * 2) * 2));

    auto itr_n_hi =
      std::upper_bound(A051037_even.cbegin(),
                       A051037_even.cend() - static_cast<std::size_t>(UINT8_C(1)),
                       static_cast<std::uint32_t>((digit_characteristics.mp_elem_number() * 2) * 2));

    mp_core_memory =
      new (std::nothrow) mp_core_memory_type(static_cast<std::size_t>(digit_characteristics.mp_elem_number()),
                                             static_cast<std::size_t>(*itr_n_hi));

    if(mp_core_memory->is_valid())
    {
      ++itr_n_hi;

      const auto fft_list_size = static_cast<std::size_t>(std::distance(itr_n_lo, itr_n_hi));

      fft_list.resize(fft_list_size);

      auto itr_n   = itr_n_lo;
      auto itr_fft = fft_list.begin();

      while(itr_n != itr_n_hi)
      {
        // Make the traits for the next FFT.
        mp_fft_base::fft_traits_type traits_fwd_1(mp_core_memory->mem_a(),    mp_core_memory->mem_buf0(), number_of_fft_threads);
        mp_fft_base::fft_traits_type traits_fwd_2(mp_core_memory->mem_b(),    mp_core_memory->mem_buf1(), number_of_fft_threads);
        mp_fft_base::fft_traits_type traits_rev  (mp_core_memory->mem_buf0(), mp_core_memory->mem_a(),    number_of_fft_threads);

        // Allocate the next FFT and add it to the list.
        *itr_fft = new mp_fft_fftw2(static_cast<std::int32_t>(*itr_n),
                                    traits_fwd_1,
                                    traits_fwd_2,
                                    traits_rev);

        ++itr_n;
        ++itr_fft;
      }
    }

    const auto result_fft_list_is_ok = (!fft_list.empty());

    result_is_ok = (result_fft_list_is_ok && result_is_ok);
  }

  return result_is_ok;
}

mp::mp_core::~mp_core()
{
  // Delete the FFT memory.
  delete mp_core_memory;

  // Delete the FFT objects in the FFT list.
  for(const auto* p_fft : fft_list) { delete p_fft; };
}

auto mp::mp_core::get_fft(const std::int32_t n) const -> const mp::mp_fft_base*
{
  using fft_const_pointer_type = const mp::mp_fft_base*;

  const auto itr_fft = std::lower_bound(fft_list.cbegin(),
                                        fft_list.cend(),
                                        n,
                                        [](fft_const_pointer_type p_fft, const std::int32_t n_fft)
                                        {
                                          return (p_fft->get_fft_n() < n_fft);
                                        });

  return (itr_fft != fft_list.cend()) ? *itr_fft : fft_list.back();
}

auto mp::mp_core::forward_set_of_mp_cpp_numeric_limits(const std::int32_t n) -> void
{
  static auto is_init = false;

  if(!is_init)
  {
    is_init = true;

    // Non-standard: Set digit limits at runtime.

    std::numeric_limits<mp::mp_cpp>::digits       = n;
    std::numeric_limits<mp::mp_cpp>::digits10     = n;
    std::numeric_limits<mp::mp_cpp>::max_digits10 = n + 1;
  }
}
