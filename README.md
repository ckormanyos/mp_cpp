mp_cpp
==================

<p align="center">
    <a href="https://github.com/ckormanyos/mp_cpp/actions">
        <img src="https://github.com/ckormanyos/mp_cpp/actions/workflows/mp_cpp.yml/badge.svg" alt="Build Status"></a>
</p>

---
**NOTE**

`mp_cpp` preserves a _legacy_ library. More modern work can be found in the [wide-decimal](https://github.com/ckormanyos/wide-decimal) project.

---

`mp_cpp` preserves a legacy _Multiple_-_Precision_ _C++_ library.
It is the first successful attempt I had ever made at a multiple-precision,
_big_-_float_ class in the C++ language.

Initial work began sometime late last century in the early 1990's
and wrapped up around 2002.

Continuous maintenance has kept the library up-to-date
and compatible with modern C++. A sample wrapper for
[`Boost.Multiprecision`](https://www.boost.org/doc/libs/1_88_0/libs/multiprecision/doc/html/index.html)
was made somewhere around 2015 and is still maintained (see notes below).

## Description

`mp_cpp` implements a multiple-precision library with adjustable
precision ranging from hundreds, to thousands, to millions of decimal digits.
The precision can be set during run time, but set once only,
preferably prior to any `mp_cpp`-operations are carried out.

The library offers many functions. This project also features tests
and a variety of non-trivial examples, including a somewhat fast
_million_ (i.e., $10^{6}$) decimal-digit $\pi$-calculation.

`mp_cpp` still compiles and runs on modern C++ compilers
from C++14, 17, 20, 23 and beyond.

This original work was subsequently cloned, renamed and adapted to become
[`e_float`](https://github.com/ckormanyos/e_float-2021),
which itself ultimately provided some motivation for
[`Boost.Multiprecision`](https://www.boost.org/doc/libs/1_88_0/libs/multiprecision/doc/html/index.html).

## Boost.Multiprecision Backend

`mp_cpp` also features a full [backend-wrapper](./boost/multiprecision/mp_cpp_backend.hpp)
for use with [`Boost.Multiprecision`](https://www.boost.org/doc/libs/1_88_0/libs/multiprecision/doc/html/index.html)
and [`Boost.Math`](https://www.boost.org/doc/libs/1_88_0/libs/math/doc/html/index.html).
This ties an old legacy project together with modern
template representations of _big_-_floats_ and implementations
of mathematical functions.

## Continuous Integration

Continuous integration runs on push using GitHub Actions.
Various compilers, operating systems and C++ standards
are included in CI.
Sanitizer runs add improved confidence to the code-integrity.

## Licensing

The big-number and mathematical-function implementations in `mp_cpp`
are licensed under BSL.

This work makes additional use of [FFTW](https://www.fftw.org/)
version 2.15 for fast FFT multiplication, which has its own separate
[license](https://www.fftw.org/faq/section1.html#isfftwfree).
