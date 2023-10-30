///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 1999 - 2019.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef MP_FFT_FFTW2_PROTOS_2011_06_16_H_
  #define MP_FFT_FFTW2_PROTOS_2011_06_16_H_

  extern "C"
  {
    typedef double fftw_real;

    typedef struct fftw_complex_type
    {
      fftw_real re;
      fftw_real im;
    }
    fftw_complex;

    typedef enum fftw_direction_type
    {
      FFTW_FORWARD  = -1,
      FFTW_BACKWARD =  1
    }
    fftw_direction;

    typedef enum fftw_node_type_type
    {
      FFTW_NOTW,
      FFTW_TWIDDLE,
      FFTW_GENERIC,
      FFTW_RADER,
      FFTW_REAL2HC,
      FFTW_HC2REAL,
      FFTW_HC2HC,
      FFTW_RGENERIC
    }
    fftw_node_type;

    // Description of a codelet.
    typedef struct fftw_codelet_desc_type
    {
      const char*    name;             // name of the codelet
      void           (*codelet)(void); // pointer to the codelet itself
      int            size;             // size of the codelet
      fftw_direction dir;              // direction
      fftw_node_type type;             // TWIDDLE or NO_TWIDDLE
      int            signature;        // unique id
      int            ntwiddle;         // number of twiddle factors
      const int*     twiddle_order;    // array that determines the order
                                       // in which the codelet expects
                                       // the twiddle factors
    }
    fftw_codelet_desc;

    typedef struct fftw_twiddle_type
    {
      int                       n;
      const fftw_codelet_desc*  cdesc;
      fftw_complex*             twarray;
      struct fftw_twiddle_type* next;
      int                       refcnt;
    }
    fftw_twiddle;

    typedef struct fftw_rader_data_type
    {
      struct fftw_twiddle_type*    plan;
      fftw_complex*                omega;
      int                          g;
      int                          ginv;
      int                          p;
      int                          flags;
      int                          refcount;
      struct fftw_rader_data_type* next;
      fftw_codelet_desc*           cdesc;
    }
    fftw_rader_data;

    typedef void (*fftw_rader_codelet)   (      fftw_complex*, const fftw_complex*,          int, int, int, fftw_rader_data*);
    typedef void (*fftw_notw_codelet)    (const fftw_complex*,       fftw_complex*,          int, int);
    typedef void (*fftw_twiddle_codelet) (      fftw_complex*, const fftw_complex*,          int, int, int);
    typedef void (*fftw_generic_codelet) (      fftw_complex*, const fftw_complex*,          int, int, int, int);
    typedef void (*fftw_real2hc_codelet) (const fftw_real*,          fftw_real*, fftw_real*, int, int, int);
    typedef void (*fftw_hc2real_codelet) (const fftw_real*,    const fftw_real*, fftw_real*, int, int, int);
    typedef void (*fftw_hc2hc_codelet)   (      fftw_real*,    const fftw_complex*,          int, int, int);
    typedef void (*fftw_rgeneric_codelet)(      fftw_real*,    const fftw_complex*,          int, int, int, int);

    // Structure that holds all the data needed for a given step.
    typedef struct fftw_plan_node_type
    {
      fftw_node_type type;

      union nodeu_type
      {
        // Nodes of type FFTW_NOTW.
        struct notw_type
        {
          int                      size;
          fftw_notw_codelet        codelet;
          const fftw_codelet_desc* codelet_desc;
        }
        notw;

        // Nodes of type FFTW_TWIDDLE.
        struct twiddle_type
        {
          int                         size;
          fftw_twiddle_codelet        codelet;
          fftw_twiddle*               tw;
          struct fftw_plan_node_type* recurse;
          const fftw_codelet_desc*    codelet_desc;
        }
        twiddle;

        // Nodes of type FFTW_GENERIC.
        struct generic_type
        {
          int                         size;
          fftw_generic_codelet        codelet;
          fftw_twiddle*               tw;
          struct fftw_plan_node_type* recurse;
        }
        generic;

        // Nodes of type FFTW_RADER.
        struct rader_type
        {
          int                         size;
          fftw_rader_codelet          codelet;
          fftw_rader_data*            rader_data;
          fftw_twiddle*               tw;
          struct fftw_plan_node_type* recurse;
        }
        rader;

        // Nodes of type FFTW_REAL2HC.
        struct real2hc_type
        {
          int                      size;
          fftw_real2hc_codelet     codelet;
          const fftw_codelet_desc* codelet_desc;
        }
        real2hc;

        // Nodes of type FFTW_HC2REAL.
        struct hc2real_type
        {
          int                      size;
          fftw_hc2real_codelet     codelet;
          const fftw_codelet_desc* codelet_desc;
        }
        hc2real;

        // Nodes of type FFTW_HC2HC.
        struct hc2hc_type
        {
          int                         size;
          fftw_direction              dir;
          fftw_hc2hc_codelet          codelet;
          fftw_twiddle*               tw;
          struct fftw_plan_node_type* recurse;
          const fftw_codelet_desc*    codelet_desc;
        }
        hc2hc;

        // Nodes of type FFTW_RGENERIC.
        struct rgeneric_type
        {
          int                         size;
          fftw_direction              dir;
          fftw_rgeneric_codelet       codelet;
          fftw_twiddle*               tw;
          struct fftw_plan_node_type* recurse;
        }
        rgeneric;
      }
      nodeu;

      int refcnt;
    }
    fftw_plan_node;

    typedef enum fftw_recurse_kind_type
    {
      FFTW_NORMAL_RECURSE = 0,
      FFTW_VECTOR_RECURSE = 1
    }
    fftw_recurse_kind;

    typedef struct fftw_planner_type
    {
      int                       n;
      int                       refcnt;
      fftw_direction            dir;
      int                       flags;
      int                       wisdom_signature;
      fftw_node_type            wisdom_type;
      struct fftw_planner_type* next;
      fftw_plan_node*           root;
      double                    cost;
      fftw_recurse_kind         recurse_kind;
      int                       vector_size;
    }
    fftw_planner;

    typedef fftw_planner* fftw_plan;
    typedef fftw_plan     rfftw_plan;

    void       rfftw_one         (     rfftw_plan, fftw_real*, fftw_real*);
    void       rfftw_threads_one (int, rfftw_plan, fftw_real*, fftw_real*);
    rfftw_plan rfftw_create_plan (int, fftw_direction, int);
    void       rfftw_destroy_plan(rfftw_plan);

    #define FFTW_ESTIMATE     (0)
    #define FFTW_MEASURE      (1)
    #define FFTW_OUT_OF_PLACE (0)
    #define FFTW_IN_PLACE     (8)
    #define FFTW_USE_WISDOM   (16)
  }

#endif // MP_FFT_FFTW2_PROTOS_2011_06_16_H_
