/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "cnst.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  logqnorm                                                      */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Log quantization for norms of sub-vectors                               */
/*--------------------------------------------------------------------------*/
/*  Word32    *x  (i)   coefficient vector                                  */
/*  Word16    *k  (o)   index                                               */
/*  Word16    L   (i)   codebook length                                     */
/*  Word16    N   (i)   sub-vector size                                     */
/*  Word16    M   (i)   group number (0->G1; 1->G2; 2->G3; 3->G4)           */
/*--------------------------------------------------------------------------*/
void logqnorm(Word32 *x, Word16 *k, Word16 L, Word16 N, Word16 M)
{
    Word16 i, m;
    Word16 coefs_shift, power_shift, temp_shift;
    Word32 temp, temp1, temp2;
    Word16 coefs16[WID_GX];


    temp1 = (Word32)1;
#if(WMOPS)
  move32();
#endif
    FOR (i=0; i<N; i++)
    {
       temp2 = L_abs(x[i]);
       temp1 = L_max(temp1, temp2);
    }
    coefs_shift = sub(norm_l(temp1), moreshift[M]);
    temp = (Word32)0;
#if(WMOPS)
  move32();
#endif
    FOR (i=0; i<N; i++)
    {
       coefs16[i] = extract_h(L_shl(x[i], coefs_shift));
       temp = L_mac0(temp, coefs16[i], coefs16[i]);
#if(WMOPS)
  move16();
#endif
    }
    temp = L_mls(temp, inv_sbfsize[M]);
    power_shift = shl(sub(coefs_shift, 16), 1);

    temp_shift = norm_l(temp);
    m = add(temp_shift, power_shift);
    temp1 = L_shl(temp, temp_shift);
    temp2 = L_sub(temp1, THREN2POW);
    if (temp2<0)
    {
      m = add(m, 1);
    }
    *k = add(THRIDXOFST, m);
    *k = s_max(*k, 0);
    i = sub(L, 1);
    *k = s_min(*k, i);


    return;
}
