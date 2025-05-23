/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* � 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "proto.h"
#include "cnst.h"

/*--------------------------------------------------------------------------*/
/*  Function  dprocnobitsbfm                                                */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~                                                */
/*                                                                          */
/*  Decoding for sub-vectors originally allocated with 0 bits               */
/*--------------------------------------------------------------------------*/
/*  Word16    *R            (i/o) number of bits per coefficinet            */
/*  Word16    *idx          (i)   reordered sub-vector indices              */
/*  Word16    *ynrm         (i)   quantization indices for norms            */
/*  Word16    *ycof         (i)   quantization indices for MLT coefficients */
/*  Word16    *pbits        (i/o) pointer to bitstream                      */
/*  Word32    *coefsq       (o)   reconstructed MLT coefficients            */
/*  Word16    *coefsq_norm  (o)   reconstructed normalized MLT coefficients */
/*  Word16    nb_sfm        (i)   number of sub-vectors to be coded         */
/*  Word16    diff          (i)   number of available bits                  */
/*--------------------------------------------------------------------------*/
void dprocnobitsbfm(
Word16 *R,
Word16 *idx,
Word16 *ynrm,
Word16 *ycof,
Word16 **ppbits,
Word32 *coefsq,
Word16 *coefsq_norm,
Word16 nb_sfm,
Word16 diff
)
{
    Word16 i, k, m, im, offset;
    Word16 *pbits;

    pbits = *ppbits;

    im = (Word16)MAX_16;
#if(WMOPS)
  move16();
#endif
    k = sub(nb_sfm, 1);
    FOR (i=k; i>0; i--)
    {
       if (R[idx[i]]==0)
       {
         im = i;
#if(WMOPS)
  move16();
#endif
       }
    }
    FOR (i=im; i<nb_sfm; i++)
    {
      m = idx[i];
#if(WMOPS)
  move16();
#endif
      IF (R[m]==0)
      {
        IF (sub(m, SFM_G1)<0)
        {
          IF (sub(diff, WID_G1)>=0)
          {
            R[m] = (Word16)1;
#if(WMOPS)
  move16();
#endif
            offset = shl(m, 3);
            dprocnf(&ycof[offset], pbits, ynrm[m], NB_VECT1, &coefsq[offset], &coefsq_norm[offset]);
            pbits += WID_G1;
            diff = sub(diff, WID_G1);
          }
        }
        ELSE if (sub(m, SFM_G1G2)<0)
        {
          IF (sub(diff, WID_G2)>=0)
          {
            R[m] = (Word16)1;
#if(WMOPS)
  move16();
#endif
            offset = add(NUMC_G1, shl(sub(m, SFM_G1), 4));
            dprocnf(&ycof[offset], pbits, ynrm[m], NB_VECT2, &coefsq[offset], &coefsq_norm[offset]);
            pbits += WID_G2;
            diff = sub(diff, WID_G2);
          }
        }
        ELSE if (sub(m, SFM_N)<0)
        {
          IF (sub(diff, WID_G3)>=0)
          {
            R[m] = (Word16)1;
#if(WMOPS)
  move16();
#endif
            offset = add(NUMC_G1G2, extract_l(L_mult0(sub(m, SFM_G1G2), WID_G3)));
            dprocnf(&ycof[offset], pbits, ynrm[m], NB_VECT3, &coefsq[offset], &coefsq_norm[offset]);
            pbits += WID_G3;
            diff = sub(diff, WID_G3);
          }
        }
        ELSE
        {
          IF (sub(diff, WID_GX)>=0)
          {
            R[m] = (Word16)1;
#if(WMOPS)
  move16();
#endif
            offset = add(NUMC_N, shl(sub(m, SFM_N), 5));
            dprocnf(&ycof[offset], pbits, ynrm[m], NB_VECTX, &coefsq[offset], &coefsq_norm[offset]);
            pbits += WID_GX;
            diff = sub(diff, WID_GX);
          }
        }
      }
    }

    *ppbits = pbits;
    return;
}
