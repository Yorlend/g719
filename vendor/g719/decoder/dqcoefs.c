/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "proto.h"
#include "cnst.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  dqcoefs                                                       */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Vector de-quantization for normalized MLT coefficients                  */
/*--------------------------------------------------------------------------*/
/*  Word16    *y           (i)   indices of the selected codevectors        */
/*  Word16    *idxnrm      (i)   indices of quantized norms                 */
/*  Word16    *R           (i)   number of bits per coefficient             */
/*  Word16    N1           (i)   beginning sub-vector's number in the group */
/*  Word16    N2           (i)   ending sub-vector's number in the group    */
/*  Word16    L            (i)   number of coefficients in each sub-vector  */
/*  Word32    *coefs       (o)   MLT coefficients                           */
/*  Word16    *coefs_norm  (o)   normalized MLT coefficients                */
/*--------------------------------------------------------------------------*/
void dqcoefs(Word16 *y,
             Word16 *idxnrm,
             Word16 *R,
             Word16 N1,
             Word16 N2,
             Word16 L,
             Word32 *coefs,
             Word16 *coefs_norm)
{
   Word16 i, j, n, v, k, m, r;
   Word16 nb_vecs, factor, s_temp, pre_idx;
   Word16 x[8];
   Word32 L_temp, acca;
   Word16 *pidx, *pcoefs_norm;
   Word32 *pcoefs;


   pidx = y;
   pcoefs = coefs;
   pcoefs_norm =  coefs_norm;
   nb_vecs = shr(L, 3);
   FOR (n=N1; n<N2; n++)
   {
      v = R[n];
#if(WMOPS)
  move16();
#endif
      r = s_and(idxnrm[n], 1);
      m = shr(idxnrm[n], 1);
      IF (sub(v, 1)>0)
      {
         k = sub(sub(16, m), v);
         k = sub(k, 3);
         m = add(SHFT_OFST, v);
         IF (r==0)
         {
            acca = L_shl(1, m);
            factor = FCT_DLVQ2;
#if(WMOPS)
  move16();
#endif
         }
         ELSE
         {
            m = sub(m, 15);
            acca = L_shl(INV2POWHALF, m);
            factor = mult(FCT_DLVQ2, INV2POWHALF);
         }
         FOR (i=0; i<nb_vecs; i++)
         {
            idx2code(pidx, x, v);
            FOR (j=0; j<8; j++)
            {
               L_temp = L_mac0(acca, x[j], factor);
               *pcoefs++ = L_shl(L_temp, k);
               *pcoefs_norm++ = add(mult(shl(x[j], FacLVQ2Qv[v]), DLVQ2FCT), OFFSET);

#if(WMOPS)
  move32();
  move16();
#endif
            }
            pidx += 8;
         }
      }
      ELSE if (sub(v, 1)==0)
      {
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
            IF ((sub(pre_idx, 128)<0) && (sub(*pidx, 16)<0))
            {
              FOR (j=0; j<8; j++)
              {
                 *pcoefs++ = L_shl(acca, k);
                 *pcoefs_norm++ = (Word16)OFFSET;
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
                 L_temp = L_mac0(acca, dic4[*pidx][j], s_temp);
                 *pcoefs++ = L_shl(L_temp, k);
                 *pcoefs_norm++ = add(mult(shl(dic4[*pidx][j], Qv1), DLVQ1FCT), OFFSET);
#if(WMOPS)
  move32();
  move16();
#endif
              }
            }
            pre_idx = *pidx;
            pidx += 8;
         }
      }
      ELSE
      {
         FOR (i=0; i<L; i++)
         {
            *pcoefs++ = (Word32)0;
            *pcoefs_norm++ = (Word16) 0;
#if(WMOPS)
  move32();
  move16();
#endif
         }
         pidx += L;
      }
   }


   return;
}
