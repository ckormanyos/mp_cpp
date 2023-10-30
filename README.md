mp_cpp
==================

<p align="center">
    <a href="https://github.com/ckormanyos/mp_cpp/actions">
        <img src="https://github.com/ckormanyos/mp_cpp/actions/workflows/mp_cpp.yml/badge.svg" alt="Build Status"></a>
</p>

`mp_cpp` preserves a legacy _Multiple_-_Precision_ _C++_ library.
It is the first successful attempt I had ever made at a multiple-precision,
_big_-_float_ class in the C++ language.

Initial work began sometime late last century in the early 1990's
and wrapped up around 2002.

Continuous maintenance has kept the library up-to-date
and compatible with modern C++.

## Description

`mp_cpp` implements a multiple-precision library with compile-time adjustable
precision ranging from hundreds, to thousands, to millions of decimal digits of precision.
This original work was subsequently cloned, renamed and adapted to become
[`e_float`](https://github.com/ckormanyos/e_float-2021),
which itself ultimately provided some motivation for Boost.Multiprecision.

`mp_cpp` still compiles and runs on modern C++ compilers
from C++11, 14, 17, 20, 23 and beyond.

The library offers many functions. This project also features tests
and a variety of non-trivial examples, including a somewhat fast
_million_ (i.e., $10^{6}$) decimal-digit $\pi$-calculation.

## Licensing

The big-number and mathematical-function implementations in `mp_cpp`
are licensed under BSL. This work uses [FFTW](https://www.fftw.org/)
version 2.15 for fast FFT multiplication, which has its own
[license](https://www.fftw.org/faq/section1.html#isfftwfree).
