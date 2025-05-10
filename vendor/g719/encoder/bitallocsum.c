/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "cnst.h"

/*--------------------------------------------------------------------------*/
/*  Function  bitallocsum                                                   */
/*  ~~~~~~~~~~~~~~~~~~~~~                                                   */
/*                                                                          */
/*  Calculate the total number of bits allocated over frame                 */
/*--------------------------------------------------------------------------*/
/*  Word16      *R      (i)    bit-allocation vector                        */
/*  Word16      nb_sfm  (i)    number of sub-vectors                        */
/*  Word16      *sum    (o)    total number of bits allocated               */
/*--------------------------------------------------------------------------*/
void bitallocsum(Word16 *R, Word16 nb_sfm, Word16 *sum)
{
   Word16 i, temp;
   Word16 sumg1, sumg2, sumg3, sumgx;


   temp = (Word16)0;
#if(WMOPS)
  move16();
#endif
   FOR (i=0; i<SFM_G1; i++)
   {
      temp = add(temp, R[i]);
   }
   sumg1 = shl(temp, 3);
   temp = (Word16)0;
#if(WMOPS)
  move16();
#endif
   FOR (i=SFM_G1; i<SFM_G1G2; i++)
   {
      temp = add(temp, R[i]);
   }
   sumg2 = shl(temp, 4);
   temp = (Word16)0;
#if(WMOPS)
  move16();
#endif
   FOR (i=SFM_G1G2; i<SFM_N; i++)
   {
      temp = add(temp, R[i]);
   }
   sumg3 = extract_l(L_mult0(temp, WID_G3));
   temp = (Word16)0;
#if(WMOPS)
  move16();
#endif
   FOR (i=SFM_N; i<nb_sfm; i++)
   {
      temp = add(temp, R[i]);
   }
   sumgx = shl(temp, 5);
   *sum = add(add(sumg1, sumg2), add(sumg3, sumgx));


   return;
}
