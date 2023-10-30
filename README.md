mp_cpp
==================

`mp_cpp` preserves the legacy multiple-precision C++ library with examples.
This is the first attempt I had made at a multiple-precision big-float class
in the C++ language.

It still compiles and offers many functions, including a rather
fast _million-digit_ $\pi$-calculation.

## Description

`mp_cpp` implements a multiple-precision library with compile-time adjustable
precision ranging from hundreds, to thousands, to millions of decimal digits of precision.
This work was the original work which becam e_float, which itself
subsequently provided some motivation for Boost.Multiprecision.

## Licensing

The big-number and mathematical-function implementations in `mp_cpp`
are licensed under BSL. This also work uses [FFTW](https://www.fftw.org/)
for fast FFT multiplication,
having its own [license](https://www.fftw.org/faq/section1.html#isfftwfree).
