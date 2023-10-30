/*
 * Copyright (c) 1997-1999, 2003 Massachusetts Institute of Technology
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

/* This file was automatically generated --- DO NOT EDIT */
/* Generated on Mon Mar 24 02:07:15 EST 2003 */

#include "fftw-int.h"
#include "fftw.h"

/* Generated by: /homee/stevenj/cvs/fftw/gensrc/genfft -magic-alignment-check -magic-twiddle-load-all -magic-variables 4 -magic-loopi -hc2real 14 */

/*
 * This function contains 62 FP additions, 38 FP multiplications,
 * (or, 60 additions, 36 multiplications, 2 fused multiply/add),
 * 24 stack variables, and 28 memory accesses
 */
static const fftw_real K445041867 =
FFTW_KONST(+0.445041867912628808577805128993589518932711138);
static const fftw_real K1_801937735 =
FFTW_KONST(+1.801937735804838252472204639014890102331838324);
static const fftw_real K1_246979603 =
FFTW_KONST(+1.246979603717467061050009768008479621264549462);
static const fftw_real K867767478 =
FFTW_KONST(+0.867767478235116240951536665696717509219981456);
static const fftw_real K1_949855824 =
FFTW_KONST(+1.949855824363647214036263365987862434465571601);
static const fftw_real K1_563662964 =
FFTW_KONST(+1.563662964936059617416889053348115500464669037);
static const fftw_real K2_000000000 =
FFTW_KONST(+2.000000000000000000000000000000000000000000000);

/*
 * Generator Id's : 
 * $Id: exprdag.ml,v 1.43 2003/03/16 23:43:46 stevenj Exp $
 * $Id: fft.ml,v 1.44 2003/03/16 23:43:46 stevenj Exp $
 * $Id: to_c.ml,v 1.26 2003/03/16 23:43:46 stevenj Exp $
 */

void fftw_hc2real_14(const fftw_real *real_input,
         const fftw_real *imag_input, fftw_real *output,
         int real_istride, int imag_istride, int ostride)
{
     fftw_real tmp3;
     fftw_real tmp13;
     fftw_real tmp6;
     fftw_real tmp14;
     fftw_real tmp26;
     fftw_real tmp35;
     fftw_real tmp23;
     fftw_real tmp34;
     fftw_real tmp12;
     fftw_real tmp16;
     fftw_real tmp20;
     fftw_real tmp33;
     fftw_real tmp9;
     fftw_real tmp15;
     fftw_real tmp1;
     fftw_real tmp2;
     ASSERT_ALIGNED_DOUBLE;
     tmp1 = real_input[0];
     tmp2 = real_input[7 * real_istride];
     tmp3 = tmp1 - tmp2;
     tmp13 = tmp1 + tmp2;
     {
    fftw_real tmp4;
    fftw_real tmp5;
    fftw_real tmp24;
    fftw_real tmp25;
    ASSERT_ALIGNED_DOUBLE;
    tmp4 = real_input[2 * real_istride];
    tmp5 = real_input[5 * real_istride];
    tmp6 = tmp4 - tmp5;
    tmp14 = tmp4 + tmp5;
    tmp24 = imag_input[2 * imag_istride];
    tmp25 = imag_input[5 * imag_istride];
    tmp26 = tmp24 - tmp25;
    tmp35 = tmp24 + tmp25;
     }
     {
    fftw_real tmp21;
    fftw_real tmp22;
    fftw_real tmp10;
    fftw_real tmp11;
    ASSERT_ALIGNED_DOUBLE;
    tmp21 = imag_input[6 * imag_istride];
    tmp22 = imag_input[imag_istride];
    tmp23 = tmp21 - tmp22;
    tmp34 = tmp21 + tmp22;
    tmp10 = real_input[6 * real_istride];
    tmp11 = real_input[real_istride];
    tmp12 = tmp10 - tmp11;
    tmp16 = tmp10 + tmp11;
     }
     {
    fftw_real tmp18;
    fftw_real tmp19;
    fftw_real tmp7;
    fftw_real tmp8;
    ASSERT_ALIGNED_DOUBLE;
    tmp18 = imag_input[4 * imag_istride];
    tmp19 = imag_input[3 * imag_istride];
    tmp20 = tmp18 - tmp19;
    tmp33 = tmp18 + tmp19;
    tmp7 = real_input[4 * real_istride];
    tmp8 = real_input[3 * real_istride];
    tmp9 = tmp7 - tmp8;
    tmp15 = tmp7 + tmp8;
     }
     {
    fftw_real tmp38;
    fftw_real tmp37;
    fftw_real tmp29;
    fftw_real tmp28;
    ASSERT_ALIGNED_DOUBLE;
    output[7 * ostride] =
        tmp3 + (K2_000000000 * (tmp6 + tmp9 + tmp12));
    tmp38 =
        (K1_563662964 * tmp35) + (K1_949855824 * tmp33) +
        (K867767478 * tmp34);
    tmp37 =
        tmp3 + (K1_246979603 * tmp6) - (K1_801937735 * tmp12) -
        (K445041867 * tmp9);
    output[ostride] = tmp37 - tmp38;
    output[13 * ostride] = tmp37 + tmp38;
    {
         fftw_real tmp40;
         fftw_real tmp39;
         fftw_real tmp36;
         fftw_real tmp32;
         ASSERT_ALIGNED_DOUBLE;
         tmp40 =
       (K867767478 * tmp33) + (K1_563662964 * tmp34) -
       (K1_949855824 * tmp35);
         tmp39 =
       tmp3 + (K1_246979603 * tmp12) - (K1_801937735 * tmp9) -
       (K445041867 * tmp6);
         output[5 * ostride] = tmp39 - tmp40;
         output[9 * ostride] = tmp39 + tmp40;
         tmp36 =
       (K1_563662964 * tmp33) - (K1_949855824 * tmp34) -
       (K867767478 * tmp35);
         tmp32 =
       tmp3 + (K1_246979603 * tmp9) - (K445041867 * tmp12) -
       (K1_801937735 * tmp6);
         output[11 * ostride] = tmp32 - tmp36;
         output[3 * ostride] = tmp32 + tmp36;
    }
    output[0] = tmp13 + (K2_000000000 * (tmp14 + tmp15 + tmp16));
    tmp29 =
        (K867767478 * tmp20) + (K1_563662964 * tmp23) -
        (K1_949855824 * tmp26);
    tmp28 =
        tmp13 + (K1_246979603 * tmp16) - (K1_801937735 * tmp15) -
        (K445041867 * tmp14);
    output[12 * ostride] = tmp28 - tmp29;
    output[2 * ostride] = tmp28 + tmp29;
    {
         fftw_real tmp31;
         fftw_real tmp30;
         fftw_real tmp27;
         fftw_real tmp17;
         ASSERT_ALIGNED_DOUBLE;
         tmp31 =
       (K1_563662964 * tmp26) + (K1_949855824 * tmp20) +
       (K867767478 * tmp23);
         tmp30 =
       tmp13 + (K1_246979603 * tmp14) -
       (K1_801937735 * tmp16) - (K445041867 * tmp15);
         output[8 * ostride] = tmp30 - tmp31;
         output[6 * ostride] = tmp30 + tmp31;
         tmp27 =
       (K1_563662964 * tmp20) - (K1_949855824 * tmp23) -
       (K867767478 * tmp26);
         tmp17 =
       tmp13 + (K1_246979603 * tmp15) - (K445041867 * tmp16) -
       (K1_801937735 * tmp14);
         output[4 * ostride] = tmp17 - tmp27;
         output[10 * ostride] = tmp17 + tmp27;
    }
     }
}

fftw_codelet_desc fftw_hc2real_14_desc = {
     "fftw_hc2real_14",
     (void (*)()) fftw_hc2real_14,
     14,
     FFTW_BACKWARD,
     FFTW_HC2REAL,
     323,
     0,
     (const int *) 0,
};
