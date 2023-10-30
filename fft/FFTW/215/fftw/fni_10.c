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
/* Generated on Mon Mar 24 02:06:28 EST 2003 */

#include "fftw-int.h"
#include "fftw.h"

/* Generated by: /homee/stevenj/cvs/fftw/gensrc/genfft -magic-alignment-check -magic-twiddle-load-all -magic-variables 4 -magic-loopi -notwiddleinv 10 */

/*
 * This function contains 84 FP additions, 24 FP multiplications,
 * (or, 72 additions, 12 multiplications, 12 fused multiply/add),
 * 36 stack variables, and 40 memory accesses
 */
static const fftw_real K559016994 =
FFTW_KONST(+0.559016994374947424102293417182819058860154590);
static const fftw_real K250000000 =
FFTW_KONST(+0.250000000000000000000000000000000000000000000);
static const fftw_real K951056516 =
FFTW_KONST(+0.951056516295153572116439333379382143405698634);
static const fftw_real K587785252 =
FFTW_KONST(+0.587785252292473129168705954639072768597652438);

/*
 * Generator Id's : 
 * $Id: exprdag.ml,v 1.43 2003/03/16 23:43:46 stevenj Exp $
 * $Id: fft.ml,v 1.44 2003/03/16 23:43:46 stevenj Exp $
 * $Id: to_c.ml,v 1.26 2003/03/16 23:43:46 stevenj Exp $
 */

void fftwi_no_twiddle_10(const fftw_complex *input, fftw_complex *output,
       int istride, int ostride)
{
     fftw_real tmp3;
     fftw_real tmp19;
     fftw_real tmp67;
     fftw_real tmp76;
     fftw_real tmp59;
     fftw_real tmp60;
     fftw_real tmp10;
     fftw_real tmp17;
     fftw_real tmp18;
     fftw_real tmp74;
     fftw_real tmp73;
     fftw_real tmp22;
     fftw_real tmp25;
     fftw_real tmp26;
     fftw_real tmp36;
     fftw_real tmp43;
     fftw_real tmp62;
     fftw_real tmp63;
     fftw_real tmp68;
     fftw_real tmp52;
     fftw_real tmp55;
     fftw_real tmp77;
     fftw_real tmp78;
     fftw_real tmp79;
     ASSERT_ALIGNED_DOUBLE;
     {
    fftw_real tmp1;
    fftw_real tmp2;
    fftw_real tmp65;
    fftw_real tmp66;
    ASSERT_ALIGNED_DOUBLE;
    tmp1 = c_re(input[0]);
    tmp2 = c_re(input[5 * istride]);
    tmp3 = tmp1 - tmp2;
    tmp19 = tmp1 + tmp2;
    tmp65 = c_im(input[0]);
    tmp66 = c_im(input[5 * istride]);
    tmp67 = tmp65 - tmp66;
    tmp76 = tmp65 + tmp66;
     }
     {
    fftw_real tmp6;
    fftw_real tmp20;
    fftw_real tmp16;
    fftw_real tmp24;
    fftw_real tmp9;
    fftw_real tmp21;
    fftw_real tmp13;
    fftw_real tmp23;
    ASSERT_ALIGNED_DOUBLE;
    {
         fftw_real tmp4;
         fftw_real tmp5;
         fftw_real tmp14;
         fftw_real tmp15;
         ASSERT_ALIGNED_DOUBLE;
         tmp4 = c_re(input[2 * istride]);
         tmp5 = c_re(input[7 * istride]);
         tmp6 = tmp4 - tmp5;
         tmp20 = tmp4 + tmp5;
         tmp14 = c_re(input[6 * istride]);
         tmp15 = c_re(input[istride]);
         tmp16 = tmp14 - tmp15;
         tmp24 = tmp14 + tmp15;
    }
    {
         fftw_real tmp7;
         fftw_real tmp8;
         fftw_real tmp11;
         fftw_real tmp12;
         ASSERT_ALIGNED_DOUBLE;
         tmp7 = c_re(input[8 * istride]);
         tmp8 = c_re(input[3 * istride]);
         tmp9 = tmp7 - tmp8;
         tmp21 = tmp7 + tmp8;
         tmp11 = c_re(input[4 * istride]);
         tmp12 = c_re(input[9 * istride]);
         tmp13 = tmp11 - tmp12;
         tmp23 = tmp11 + tmp12;
    }
    tmp59 = tmp6 - tmp9;
    tmp60 = tmp13 - tmp16;
    tmp10 = tmp6 + tmp9;
    tmp17 = tmp13 + tmp16;
    tmp18 = tmp10 + tmp17;
    tmp74 = tmp23 - tmp24;
    tmp73 = tmp20 - tmp21;
    tmp22 = tmp20 + tmp21;
    tmp25 = tmp23 + tmp24;
    tmp26 = tmp22 + tmp25;
     }
     {
    fftw_real tmp32;
    fftw_real tmp50;
    fftw_real tmp42;
    fftw_real tmp54;
    fftw_real tmp35;
    fftw_real tmp51;
    fftw_real tmp39;
    fftw_real tmp53;
    ASSERT_ALIGNED_DOUBLE;
    {
         fftw_real tmp30;
         fftw_real tmp31;
         fftw_real tmp40;
         fftw_real tmp41;
         ASSERT_ALIGNED_DOUBLE;
         tmp30 = c_im(input[2 * istride]);
         tmp31 = c_im(input[7 * istride]);
         tmp32 = tmp30 - tmp31;
         tmp50 = tmp30 + tmp31;
         tmp40 = c_im(input[6 * istride]);
         tmp41 = c_im(input[istride]);
         tmp42 = tmp40 - tmp41;
         tmp54 = tmp40 + tmp41;
    }
    {
         fftw_real tmp33;
         fftw_real tmp34;
         fftw_real tmp37;
         fftw_real tmp38;
         ASSERT_ALIGNED_DOUBLE;
         tmp33 = c_im(input[8 * istride]);
         tmp34 = c_im(input[3 * istride]);
         tmp35 = tmp33 - tmp34;
         tmp51 = tmp33 + tmp34;
         tmp37 = c_im(input[4 * istride]);
         tmp38 = c_im(input[9 * istride]);
         tmp39 = tmp37 - tmp38;
         tmp53 = tmp37 + tmp38;
    }
    tmp36 = tmp32 - tmp35;
    tmp43 = tmp39 - tmp42;
    tmp62 = tmp32 + tmp35;
    tmp63 = tmp39 + tmp42;
    tmp68 = tmp62 + tmp63;
    tmp52 = tmp50 - tmp51;
    tmp55 = tmp53 - tmp54;
    tmp77 = tmp50 + tmp51;
    tmp78 = tmp53 + tmp54;
    tmp79 = tmp77 + tmp78;
     }
     c_re(output[5 * ostride]) = tmp3 + tmp18;
     {
    fftw_real tmp44;
    fftw_real tmp46;
    fftw_real tmp29;
    fftw_real tmp45;
    fftw_real tmp27;
    fftw_real tmp28;
    ASSERT_ALIGNED_DOUBLE;
    tmp44 = (K587785252 * tmp36) - (K951056516 * tmp43);
    tmp46 = (K951056516 * tmp36) + (K587785252 * tmp43);
    tmp27 = tmp3 - (K250000000 * tmp18);
    tmp28 = K559016994 * (tmp10 - tmp17);
    tmp29 = tmp27 - tmp28;
    tmp45 = tmp28 + tmp27;
    c_re(output[7 * ostride]) = tmp29 - tmp44;
    c_re(output[3 * ostride]) = tmp29 + tmp44;
    c_re(output[ostride]) = tmp45 - tmp46;
    c_re(output[9 * ostride]) = tmp45 + tmp46;
     }
     c_re(output[0]) = tmp19 + tmp26;
     {
    fftw_real tmp56;
    fftw_real tmp58;
    fftw_real tmp49;
    fftw_real tmp57;
    fftw_real tmp47;
    fftw_real tmp48;
    ASSERT_ALIGNED_DOUBLE;
    tmp56 = (K587785252 * tmp52) - (K951056516 * tmp55);
    tmp58 = (K951056516 * tmp52) + (K587785252 * tmp55);
    tmp47 = tmp19 - (K250000000 * tmp26);
    tmp48 = K559016994 * (tmp22 - tmp25);
    tmp49 = tmp47 - tmp48;
    tmp57 = tmp48 + tmp47;
    c_re(output[2 * ostride]) = tmp49 - tmp56;
    c_re(output[8 * ostride]) = tmp49 + tmp56;
    c_re(output[6 * ostride]) = tmp57 - tmp58;
    c_re(output[4 * ostride]) = tmp57 + tmp58;
     }
     c_im(output[5 * ostride]) = tmp68 + tmp67;
     {
    fftw_real tmp61;
    fftw_real tmp72;
    fftw_real tmp70;
    fftw_real tmp71;
    fftw_real tmp64;
    fftw_real tmp69;
    ASSERT_ALIGNED_DOUBLE;
    tmp61 = (K951056516 * tmp59) + (K587785252 * tmp60);
    tmp72 = (K587785252 * tmp59) - (K951056516 * tmp60);
    tmp64 = K559016994 * (tmp62 - tmp63);
    tmp69 = tmp67 - (K250000000 * tmp68);
    tmp70 = tmp64 + tmp69;
    tmp71 = tmp69 - tmp64;
    c_im(output[ostride]) = tmp61 + tmp70;
    c_im(output[9 * ostride]) = tmp70 - tmp61;
    c_im(output[3 * ostride]) = tmp71 - tmp72;
    c_im(output[7 * ostride]) = tmp72 + tmp71;
     }
     c_im(output[0]) = tmp79 + tmp76;
     {
    fftw_real tmp75;
    fftw_real tmp84;
    fftw_real tmp82;
    fftw_real tmp83;
    fftw_real tmp80;
    fftw_real tmp81;
    ASSERT_ALIGNED_DOUBLE;
    tmp75 = (K587785252 * tmp73) - (K951056516 * tmp74);
    tmp84 = (K951056516 * tmp73) + (K587785252 * tmp74);
    tmp80 = tmp76 - (K250000000 * tmp79);
    tmp81 = K559016994 * (tmp77 - tmp78);
    tmp82 = tmp80 - tmp81;
    tmp83 = tmp81 + tmp80;
    c_im(output[2 * ostride]) = tmp75 + tmp82;
    c_im(output[8 * ostride]) = tmp82 - tmp75;
    c_im(output[4 * ostride]) = tmp83 - tmp84;
    c_im(output[6 * ostride]) = tmp84 + tmp83;
     }
}

fftw_codelet_desc fftwi_no_twiddle_10_desc = {
     "fftwi_no_twiddle_10",
     (void (*)()) fftwi_no_twiddle_10,
     10,
     FFTW_BACKWARD,
     FFTW_NOTW,
     232,
     0,
     (const int *) 0,
};
