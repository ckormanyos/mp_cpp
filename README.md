mp_cpp
==================

<p align="center">
    <a href="https://github.com/ckormanyos/mp_cpp/actions">
        <img src="https://github.com/ckormanyos/mp_cpp/actions/workflows/mp_cpp.yml/badge.svg" alt="Build Status"></a>
</p>

`mp_cpp` preserves a legacy _Multiple_-_Precision_ _C++_ library,
which is the first attempt I had ever made at a multiple-precision big-float
class in the C++ language.

Initial work began sometime late last century in the early 1990's
and wrapped up around 2002.

Continuous support has kept the library modern and compatible with modern C++.

## Description

`mp_cpp` implements a multiple-precision library with compile-time adjustable
precision ranging from hundreds, to thousands, to millions of decimal digits of precision.
This work was the original work which became
[`e_float`](https://github.com/ckormanyos/e_float-2021),
which itself subsequently provided some motivation for Boost.Multiprecision.

`mp_cpp` still compiles and runs on modern C++ compilers (C++20 and beyond).

The library offers many functions a variety of non-trivial examples.
These include a somewhat fast million (i.e., $10^{6}$)
decimal-digit $\pi$-calculation.

## Licensing

The big-number and mathematical-function implementations in `mp_cpp`
are licensed under BSL. This also work uses [FFTW](https://www.fftw.org/)
for fast FFT multiplication,
having its own [license](https://www.fftw.org/faq/section1.html#isfftwfree).
