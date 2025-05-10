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
/*  Function  flvqdec                                                       */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Decoding of Fast Lattice Vector Quantization (FLVQ)                     */
/*--------------------------------------------------------------------------*/
/*  Word16    *bitstream      (i)   bit-stream vector                       */
/*  Word32    *coefsq         (o)   MLT coefficient vector                  */
/*  Word16    *coefsq_norm    (o)   normalized MLT coefficient vector       */
/*  Word16    R               (o)   bit-allocation vector                   */
/*  Word16    NumSpectumBits  (i)   number of available bits                */
/*  Word16    *ynrm           (o)   norm quantization index vector          */
/*  Word16    is_transient    (i)   transient flag                          */
/*--------------------------------------------------------------------------*/
void flvqdec(Word16 **bitstream,
             Word32 *coefsq,
             Word16 *coefsq_norm,
             Word16 *R,
             Word16 NumSpectumBits,
             Word16 *ynrm,
             Word16 is_transient)
{
    Word16 i, j, k, v, nb_sfm;
    Word16 diff;
    Word16 hcode_l, FlagL, FlagN, FlagC;
    Word16 idx[NB_SFM], normqlg2[NB_SFM], wnorm[NB_SFM], idxbuf[NB_SFM];
    Word16 ycof[STOP_BAND];
    Word16 *pbits;
    Word16 **ppbits;


    pbits = *bitstream;
    /* Unpacking bit stream to flags */
    FlagL = (Word16)0;
    if (sub(*pbits++, G192_BIT1)==0)
    {
      FlagL = (Word16)1;
#if(WMOPS)
  move16();
#endif
    }
    FlagN = (Word16)0;
    if (sub(*pbits++, G192_BIT1)==0)
    {
      FlagN = (Word16)1;
#if(WMOPS)
  move16();
#endif
    }
    FlagC = (Word16)0;
    if (sub(*pbits++, G192_BIT1)==0)
    {
      FlagC = (Word16)1;
#if(WMOPS)
  move16();
#endif
    }
#if(WMOPS)
  move16();
  move16();
  move16();
#endif

    /* Unpacking bit stream and Huffman decoding for indices of quantized norms */
    IF (sub(FlagL, NOALLGROUPS)==0)
    {
      nb_sfm = SFM_N;
    }
    ELSE
    {
      nb_sfm = NB_SFM;
    }
#if(WMOPS)
  move16();
#endif
    bits2idxn(pbits, NORM0_BITS, ynrm);
    pbits += NORM0_BITS;
    IF (sub(FlagN, HUFCODE)==0)
    {
      hdecnrm(pbits, NB_SFM, &ynrm[1]);
      hcode_l = (Word16)0;
      FOR (i=1; i<NB_SFM; i++)
      {
         hcode_l = add(hcode_l, huffsizn[ynrm[i]]);
      }
      pbits += hcode_l;
    }
    ELSE
    {
      FOR (i=1; i<NB_SFM; i++)
      {
         bits2idxn(pbits, NORMI_BITS, &ynrm[i]);
         pbits += NORMI_BITS;
      }
      hcode_l = NUMNRMIBITS;
    }
#if(WMOPS)
  move16();
#endif

    /* De-quantization of norms */
	 /* First sub-vector */
    normqlg2[0] = dicnlg2[ynrm[0]];
#if(WMOPS)
  move16();
#endif
  	 /* Other sub-vectors */
    IF (is_transient)
    {
      idxbuf[0] = ynrm[0];
#if(WMOPS)
  move16();
#endif
      FOR (i=1; i<NB_SFM; i++)
      {
         idxbuf[i] = add(ynrm[i], sub(idxbuf[i-1], 15));
#if(WMOPS)
  move16();
#endif
      }
      recovernorm(idxbuf, ynrm, normqlg2);
    }
    ELSE
    {
      FOR (i=1; i<NB_SFM; i++)
      {
         j = sub(i, 1);
         k = sub(ynrm[j], 15);
         ynrm[i] = add(ynrm[i], k);
         normqlg2[i] = dicnlg2[ynrm[i]];
#if(WMOPS)
  move16();
  move16();
#endif
      }
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
  move16();
#endif
    bitalloc(wnorm, idx, diff, nb_sfm, QBIT_MAX2, R);

    /* Unpacking bit stream and Huffman decoding for indices of quantized coefficients */
    /* First group */
    hcode_l = unpackc(R, pbits, FlagC, 0, 0, SFM_G1, WID_G1, ycof);
    pbits += hcode_l;

    /* Second group */
    k = unpackc(R, pbits, FlagC, NUMC_G1, SFM_G1, SFM_G1G2, WID_G2, ycof);
    pbits += k;
    hcode_l = add(hcode_l, k);

    /* Third group */
    k = unpackc(R, pbits, FlagC, NUMC_G1G2, SFM_G1G2, SFM_N, WID_G3, ycof);
    pbits += k;
    hcode_l = add(hcode_l, k);

    /* Forth group */
    IF (sub(nb_sfm, SFM_N)>0)
    {
      k = unpackc(R, pbits, FlagC, NUMC_N, SFM_N, NB_SFM, WID_GX, ycof);
      pbits += k;
      hcode_l = add(hcode_l, k);
    }
    diff = sub(v, hcode_l);

    /* Lattice Vector De-quantization for normalized MLT coefficients */
	/* First group */
    dqcoefs(&ycof[0], ynrm, R, 0, SFM_G1, WID_G1, &coefsq[0], &coefsq_norm[0]);
   
    /* Second group */
    dqcoefs(&ycof[NUMC_G1], ynrm, R, SFM_G1, SFM_G1G2, WID_G2, &coefsq[NUMC_G1], &coefsq_norm[NUMC_G1]);

    /* Third group */
    dqcoefs(&ycof[NUMC_G1G2], ynrm, R, SFM_G1G2, SFM_N, WID_G3, &coefsq[NUMC_G1G2], &coefsq_norm[NUMC_G1G2]);

    /* Forth group */
    dqcoefs(&ycof[NUMC_N], ynrm, R, SFM_N, NB_SFM, WID_GX, &coefsq[NUMC_N], &coefsq_norm[NUMC_N]);
   
    ppbits = &pbits;
    /* Processing for sub-vectors allocated with 0 bits */
    dprocnobitsbfm(R, idx, ynrm, ycof, ppbits, coefsq, coefsq_norm, nb_sfm, diff);
   
    *bitstream = pbits;
    return;
}
