/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "stl.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  reordernorm                                                   */
/*  ~~~~~~~~~~~~~~~~~~~~~                                                   */
/*                                                                          */
/*  Reorder quantization indices and norms                                  */
/*--------------------------------------------------------------------------*/
/*  Word16    *ynrm      (i)   quantization indices for norms               */
/*  Word16    *normqlg2  (i)   quantized norms                              */
/*  Word16    *idxbuf    (o)   reordered quantization indices               */
/*  Word16    *normbuf   (o)   reordered quantized norms                    */
/*--------------------------------------------------------------------------*/
void reordernorm(Word16 *ynrm, Word16 *normqlg2, Word16 *idxbuf, Word16 *normbuf)
{
    Word16 i;
    Word16 *pidx, *pnormq;


    FOR (i=0; i<2; i++)
    {
       pidx = ynrm + i;
       pnormq = normqlg2 + i;
       idxbuf[i]     = *pidx;
       normbuf[i]    = *pnormq;
       pidx += 2;
       pnormq += 2;
       idxbuf[21-i]  = *pidx;
       normbuf[21-i] = *pnormq;
       pidx += 2;
       pnormq += 2;
       idxbuf[22+i]  = *pidx;
       normbuf[22+i] = *pnormq;
       pidx += 2;
       pnormq += 2;
       idxbuf[43-i]  = *pidx;
       normbuf[43-i] = *pnormq;
       pidx += 2;
       pnormq += 2;
       idxbuf[i+2]   = *pidx;
       normbuf[i+2]  = *pnormq;
       pidx += 2;
       pnormq += 2;
       idxbuf[19-i]  = *pidx;
       normbuf[19-i] = *pnormq;
       pidx += 2;
       pnormq += 2;
       idxbuf[24+i]  = *pidx;
       normbuf[24+i] = *pnormq;
       pidx += 2;
       pnormq += 2;
       idxbuf[41-i]  = *pidx;
       normbuf[41-i] = *pnormq;
#if(WMOPS)
{
   Word16 j;
   for (j=0; j<16; j++)
      move16();
}
#endif
    }

    pidx = ynrm + 16;
    pnormq = normqlg2 + 16;
    FOR (i=4; i<(NB_SFM/4); i++)
    {
       idxbuf[i]    = *pidx++;
       idxbuf[21-i] = *pidx++;
       idxbuf[22+i] = *pidx++;
       idxbuf[43-i] = *pidx++;
       normbuf[i]    = *pnormq++;
       normbuf[21-i] = *pnormq++;
       normbuf[22+i] = *pnormq++;
       normbuf[43-i] = *pnormq++;
#if(WMOPS)
{
   Word16 j;
   for (j=0; j<8; j++)
      move16();
}
#endif
    }


    return;
}
