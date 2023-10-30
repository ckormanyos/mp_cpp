///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef _FFTW_R2R_PROTOS_2011_05_16_H_
  #define _FFTW_R2R_PROTOS_2011_05_16_H_

  // The following FFTW stuff has been extracted from FFTW 3.2.2
  // and written in a more legible form.

  extern "C"
  {
    enum wakefulness
    {
      SLEEPY,
      AWAKE_ZERO,
      AWAKE_SQRTN_TABLE,
      AWAKE_SINCOS
    };

    typedef struct
    {
      double add;
      double mul;
      double fma;
      double other;
    }
    opcnt;

    // forward declarations.
    typedef struct plan_s plan;
    typedef struct problem_s problem;
    typedef struct printer_s printer;

    typedef struct
    {
      void (*solve)(const plan* ego, const problem* p);
      void (*awake)(plan* ego, enum wakefulness wakefulness);
      void (*print)(const plan* ego, printer* p);
      void (*destroy)(plan* ego);
    }
    plan_adt;

    typedef std::uint32_t md5uint; // Must be 32-bits.

    typedef md5uint md5sig[4];

    typedef struct
    {
      md5sig s; // state and signature

      // fields not meant to be used outside md5.c:
      unsigned char c[64]; // stuff not yet processed.
      unsigned l;          // total length. Should be 64 bits long, but this is good enough for us.
    }
    md5;

    typedef struct
    {
      int problem_kind;
      void (*hash) (const problem* ego, md5* p);
      void (*zero) (const problem* ego);
      void (*print) (const problem* ego, printer* p);
      void (*destroy) (problem* ego);
    }
    problem_adt;

    struct problem_s
    {
      const problem_adt* adt;
    };

    struct plan_s
    {
      const plan_adt* adt;
      opcnt ops;
      double pcost;
      enum wakefulness wakefulness;
      int could_prune_now_p;
    };

    typedef struct fftw_plan_s* fftw_plan;

    enum fftw_r2r_kind_do_not_use_me
    {
      FFTW_R2HC    = 0,
      FFTW_HC2R    = 1,
      FFTW_DHT     = 2,
      FFTW_REDFT00 = 3,
      FFTW_REDFT01 = 4,
      FFTW_REDFT10 = 5,
      FFTW_REDFT11 = 6,
      FFTW_RODFT00 = 7,
      FFTW_RODFT01 = 8,
      FFTW_RODFT10 = 9,
      FFTW_RODFT11 = 10
    };

    typedef enum fftw_r2r_kind_do_not_use_me fftw_r2r_kind;

    // The analogs to rfftw_create_plan and rfftw_one with FFTW_REAL_TO_COMPLEX or
    // FFTW_COMPLEX_TO_REAL directions are fftw_plan_r2r_1d with kind FFTW_R2HC
    // or FFTW_HC2R, followed by fftw_execute.

    // In FFTW2, you had to supply the FFTW_USE_WISDOM flag in order to use wisdom.
    // In FFTW3, wisdom is always used.

    extern fftw_plan fftw_plan_r2r_1d (int, double*, double*, fftw_r2r_kind, unsigned);
    extern void      fftw_execute     (const fftw_plan);
    extern void      fftw_destroy_plan(fftw_plan);
    extern void      fftw_cleanup     ();

    #define FFTW_MEASURE         (0U)
    #define FFTW_DESTROY_INPUT   (1U <<  0)
    #define FFTW_UNALIGNED       (1U <<  1)
    #define FFTW_CONSERVE_MEMORY (1U <<  2)
    #define FFTW_EXHAUSTIVE      (1U <<  3) // NO_EXHAUSTIVE is default.
    #define FFTW_PRESERVE_INPUT  (1U <<  4) // Cancels FFTW_DESTROY_INPUT.
    #define FFTW_PATIENT         (1U <<  5) // IMPATIENT is default.
    #define FFTW_ESTIMATE        (1U <<  6)
    #define FFTW_WISDOM_ONLY     (1U << 21)

    extern int  fftw_init_threads      ();     // Call this before calling any FFTW routines with threads.
    extern void fftw_plan_with_nthreads(int);  // Call this before creating a plan that you want to parallelize.
    extern void fftw_cleanup_threads   ();     // Clear all memory and other resources from FFTW with threads.
  }

#endif // _FFTW_R2R_PROTOS_2011_05_16_H_
