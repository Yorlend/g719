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
/*  Function  flvqenc                                                       */
/*  ~~~~~~~~~~~~~~~~~~~~~                                                   */
/*                                                                          */
/*  Encoding of Fast Lattice Vector Quantization (FLVQ)                     */
/*--------------------------------------------------------------------------*/
/*  Word16      *bitstream      (o)    bitstream vector                     */
/*  Word32      *coefs          (i)    MLT coefficient vector               */
/*  Word16      *coefs_norm     (o)    normalized MLT coefficient vector    */
/*  Word16      *R              (o)    bit-allocation vector                */
/*  Word16      *NumSpectumBits (i)    number of available bits             */
/*  Word16      *total_bits     (i)    total available bits                 */
/*  Word16      *is_transient   (i)    transient flag                       */
/*--------------------------------------------------------------------------*/
void flvqenc(Word16 **bitstream,
             Word32 *coefs,
             Word16 *coefs_norm,
             Word16 *R,
             Word16 NumSpectumBits,
             Word16 total_bits,
             Word16 is_transient)
{
   Word16 i, j, k, m, n, r, v, im, nb_sfm, temp;
   Word16 diff, sum;
   Word16 hcode_l, offset, FlagL, FlagN, FlagC;
   Word16 idx[NB_SFM], difidx[NB_SFM], idxbuf[NB_SFM];
   Word16 normqlg2[NB_SFM], wnorm[NB_SFM], normbuf[NB_SFM];
   Word16 ynrm[NB_SFM], ycof[STOP_BAND];
   Word16 *pbits;
   Word16 **ppbits;
   
   /* Interleave MLT coefficients of 4 sub-vectors in case of transient */
   IF (is_transient)
   {
      interleave_spectrum(coefs);
   }

   /* Scalar quantization for norms */
   /* First group */
   logqnorm(coefs, ynrm, 32, WID_G1, 0);
   j = ynrm[0];
   normqlg2[0] = dicnlg2[j];
   offset = WID_G1;
#if(WMOPS)
  move16();
  move16();
  move16();
#endif
   FOR (i=1; i<SFM_G1; i++)
   {
      logqnorm(&coefs[offset], &ynrm[i], 40, WID_G1, 0);
      offset = add(offset, WID_G1);
   }
   /* Second group */
   FOR (i=SFM_G1; i<SFM_G1G2; i++)
   {
      logqnorm(&coefs[offset], &ynrm[i], 40, WID_G2, 1);
      offset = add(offset, WID_G2);
   }
   /* Third group */
   FOR (i=SFM_G1G2; i<SFM_N; i++)
   {
      logqnorm(&coefs[offset], &ynrm[i], 40, WID_G3, 2);
      offset = add(offset, WID_G3);
   }
   /* Forth group */
   FOR (i=SFM_N; i<NB_SFM; i++)
   {
      logqnorm(&coefs[offset], &ynrm[i], 40, WID_GX, 3);
      offset = add(offset, WID_GX);
   }
   FOR (i=0; i<NB_SFM; i++)
   {
      j = ynrm[i];
      normqlg2[i] = dicnlg2[j];
#if(WMOPS)
  move16();
  move16();
#endif
   }

   /* Find the maximum of normq and its position */
   temp = normqlg2[0];
   im = (Word16)0;
#if(WMOPS)
  move16();
  move16();
#endif
   FOR (i=1; i<NB_SFM; i++)
   {
      v = sub(normqlg2[i], temp);
      temp = s_max(temp, normqlg2[i]);
      if (v>0)
      {
         im = i;
#if(WMOPS)
  move16();
#endif
      }
   }
   m = sub(SFM_N, 2);
#if(WMOPS)
  test();
#endif
   IF ((sub(total_bits, 960)<0) && (sub(im, m)<0))
   {
      nb_sfm = SFM_N;
      FlagL  = NOALLGROUPS;
   }
   ELSE
   {
      nb_sfm = NB_SFM;
      FlagL  = ALLGROUPS;
   }
#if(WMOPS)
  move16();
  move16();
#endif
   /* Differential coding for indices of quantized norms */
   IF (is_transient)
   {
      /* Reorder quantization indices and quantized norms */
     reordernorm(ynrm, normqlg2, idxbuf, normbuf);
     diffcod(normbuf, NB_SFM, idxbuf, &difidx[1]);
     difidx[0] = idxbuf[0];
#if(WMOPS)
  move16();
#endif
     recovernorm(idxbuf, ynrm, normqlg2);
   }
   ELSE
   {
     diffcod(normqlg2, NB_SFM, ynrm, &difidx[1]);
     difidx[0] = ynrm[0];
     normqlg2[0] = dicnlg2[ynrm[0]];
#if(WMOPS)
  move16();
  move16();
#endif
   }

   /* Normalize coefficients with quantized norms */
   /* First group */
   normalizecoefs(&coefs[0], ynrm, 0, SFM_G1, WID_G1, &coefs_norm[0]);
   /* Second group */
   normalizecoefs(&coefs[NUMC_G1], ynrm, SFM_G1, SFM_G1G2, WID_G2, &coefs_norm[NUMC_G1]);
   /* Third group */
   normalizecoefs(&coefs[NUMC_G1G2], ynrm, SFM_G1G2, SFM_N, WID_G3, &coefs_norm[NUMC_G1G2]);
   /* Forth group */
   IF (FlagL==ALLGROUPS)
   {
     normalizecoefs(&coefs[NUMC_N], ynrm, SFM_N, NB_SFM, WID_GX, &coefs_norm[NUMC_N]);
   }

   /* Check Huffman encoding for QNorm indices */
   hcode_l = (Word16)0;
#if(WMOPS)
  move16();
#endif
   FOR (i=1; i<NB_SFM; i++)
   {
      j = difidx[i];
#if(WMOPS)
  move16();
#endif
      hcode_l = add(hcode_l, huffsizn[j]);
   }
   FlagN = HUFCODE;
#if(WMOPS)
  move16();
#endif
   v = sub(hcode_l, NUMNRMIBITS);
   hcode_l = s_min(NUMNRMIBITS, hcode_l);
   if (v>=0)
   {
      FlagN = NOHUFCODE;
#if(WMOPS)
  move16();
#endif
   }

   /* Bit allocation */
   FOR (i=0; i<nb_sfm; i++)
   {
      idx[i] = i;
#if(WMOPS)
  move16();
#endif
   }
   map_quant_weight(normqlg2, wnorm, is_transient);
   reordvct(wnorm, nb_sfm, idx);
   FOR (i=0; i<NB_SFM; i++)
   {
      R[i] = (Word16)0;
#if(WMOPS)
  move16();
#endif
   }
   diff = sub(sub(NumSpectumBits, FLAGS_BITS), NORM0_BITS);
   v = sub(diff, hcode_l);
   diff = v;
#if(WMOPS)
  move16();
#endif
   bitalloc(wnorm, idx, diff, nb_sfm, QBIT_MAX2, R);
   /* Check bit allocation */
   bitallocsum(R, nb_sfm, &sum);
   diff = sub(v, sum);

   /* Lattice Vector Quantization for normalized MLT coefficients */
   /* First group */
   qcoefs(&coefs_norm[0], R, 0, SFM_G1, WID_G1, &ycof[0]);
   hcode_l = huffcheck(&ycof[0], R, 0, SFM_G1, WID_G1);

   /* Second group */
   qcoefs(&coefs_norm[NUMC_G1], R, SFM_G1, SFM_G1G2, WID_G2, &ycof[NUMC_G1]);
   k = huffcheck(&ycof[NUMC_G1], R, SFM_G1, SFM_G1G2, WID_G2);
   hcode_l = add(hcode_l, k);

   /* Third group */
   qcoefs(&coefs_norm[NUMC_G1G2], R, SFM_G1G2, SFM_N, WID_G3, &ycof[NUMC_G1G2]);
   k = huffcheck(&ycof[NUMC_G1G2], R, SFM_G1G2, SFM_N, WID_G3);
   hcode_l = add(hcode_l, k);

   /* Forth group */
   IF (sub(nb_sfm, SFM_N)>0)
   {
      qcoefs(&coefs_norm[NUMC_N], R, SFM_N, NB_SFM, WID_GX, &ycof[NUMC_N]);
      k = huffcheck(&ycof[NUMC_N], R, SFM_N, NB_SFM, WID_GX);
      hcode_l = add(hcode_l, k);
   }
   n = sub(sum, hcode_l);
   FlagC = NOHUFCODE;
   IF (n>0)
   {
      FlagC = HUFCODE;
      diff = add(diff, n);
   }
#if(WMOPS)
  move16();
#endif

   /* Packing indices to bit stream */
   pbits = *bitstream;
   /* Flags */
   *pbits = G192_BIT1;
   if (FlagL==0)
   {
      *pbits = G192_BIT0;
#if(WMOPS)
  move16();
#endif
   }
   *++pbits = G192_BIT1;
   if (FlagN==0)
   {
      *pbits = G192_BIT0;
#if(WMOPS)
  move16();
#endif
   }
   *++pbits = G192_BIT1;
   if (FlagC==0)
   {
      *pbits = G192_BIT0;
#if(WMOPS)
  move16();
#endif
   }
   pbits++;
#if(WMOPS)
  move16();
  move16();
  move16();
#endif

   /* Quantized norms */ 
   idx2bitsn(difidx[0], NORM0_BITS, pbits);
   pbits += NORM0_BITS;
   IF (sub(FlagN, HUFCODE)==0)
   {
      /* Huffman encoding and then packing */
      FOR (i=1; i<NB_SFM; i++)
      {
         j = difidx[i];
         m = huffnorm[j];
         r = huffsizn[j];
#if(WMOPS)
  move16();
  move16();
  move16();
#endif
         idx2bitsn(m, r, pbits);
         pbits += r;
      }
   }
   ELSE
   {
      FOR (i=1; i<NB_SFM; i++)
      {
         idx2bitsn(difidx[i], NORMI_BITS, pbits);
         pbits += NORMI_BITS;
      }
   }

   /* Quantized MLT coefficients */
   /* First group */
   k = packingc(&ycof[0], R, pbits, FlagC, 0, SFM_G1, WID_G1);
   pbits += k;
   /* Second group */
   k = packingc(&ycof[NUMC_G1], R, pbits, FlagC, SFM_G1, SFM_G1G2, WID_G2);
   pbits += k;
   /* Third group */
   k = packingc(&ycof[NUMC_G1G2], R, pbits, FlagC, SFM_G1G2, SFM_N, WID_G3);
   pbits += k;
   /* Forth group */
   IF (sub(nb_sfm, SFM_N)>0)
   {
      k = packingc(&ycof[NUMC_N], R, pbits, FlagC, SFM_N, NB_SFM, WID_GX);
      pbits += k;
   }

   /* Processing for sub-vectors allocated with no bits */
   ppbits = &pbits;
   procnobitsbfm(coefs_norm, R, idx, ycof, ppbits, nb_sfm, diff);
   
   *bitstream = pbits;
   return;
}
