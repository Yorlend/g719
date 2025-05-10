/*----------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                       */
/* All rights reserved.                                                       */
/*----------------------------------------------------------------------------*/
#include "stl.h"
#include "proto.h"
#include "cnst.h"
#include "rom.h"

/*----------------------------------------------------------------------------*/
/*  Function  dprocnf                                                         */
/*  ~~~~~~~~~~~~~~~~~                                                         */
/*                                                                            */
/*  De-quantization for sub-vectors originally allocated with 0 bits          */
/*----------------------------------------------------------------------------*/
/*  Word16     *y           (i)   indices of the selected codevectors         */
/*  Word16     *pbits       (i)   pointer to bitstream                        */
/*  Word16     idxnrm       (i)   indices of quantized norms                  */
/*  Word16     nb_vecs      (i)   number of 8-D vectors in current sub-vector */
/*  Word32     *coefs       (o)   MLT coefficients                            */
/*  Word16     *coefs_norm  (o)   normalized MLT coefficients                 */
/*----------------------------------------------------------------------------*/
void dprocnf(Word16 *y,
             Word16 *pbits,
             Word16 idxnrm,
             Word16 nb_vecs,
             Word32 *coefs,
             Word16* coefs_norm)
{
   Word16 i, j, k, m, r;
   Word16 s_temp, pre_idx;
   Word32 L_temp, acca;


   r = s_and(idxnrm, 1);
   m = shr(idxnrm, 1);
   k = sub(14, m);
   k = sub(k, 3);
   IF (r==0)
   {
     acca = L_shl(1, 8);
     s_temp = LVQ1SCALECW1L;
   }
   ELSE
   {
     acca = L_shr(INV2POWHALF, 7);
     s_temp = LVQ1SCALECW2L;
   }
#if(WMOPS)
  move16();
  move16();
#endif
   pre_idx = MAX_16;
   FOR (i=0; i<nb_vecs; i++)
   {
      bits2idxc(pbits, 8, 1, y);
      pbits += 8;
#if(WMOPS)
  test();
#endif
      IF ((sub(pre_idx, 128)<0) && (sub(*y, 16)<0))
      {
        FOR (j=0; j<8; j++)
        {
           *coefs++ = L_shl(acca, k);
           *coefs_norm++ = (Word16)OFFSET;
#if(WMOPS)
  move32();
  move16();
#endif
        }
      }
      ELSE
      {
        FOR (j=0; j<8; j++)
        {
           L_temp = L_mac0(acca, dic4[*y][j], s_temp);
           *coefs++ = L_shl(L_temp, k);
           *coefs_norm++ = add(mult(shl(dic4[*y][j], Qv1), DLVQ1FCT), OFFSET);
#if(WMOPS)
  move32();
  move16();
#endif
        }
      }
      pre_idx = *y;
      y += 8;
   }


   return;
}
