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
/*  Function  recovernorm                                                   */
/*  ~~~~~~~~~~~~~~~~~~~~~                                                   */
/*                                                                          */
/*  Recover reordered quantization indices and norms                        */
/*--------------------------------------------------------------------------*/
/*  Word16    *idxbuf    (i)   reordered quantization indices               */
/*  Word16    *ynrm      (o)   recovered quantization indices               */
/*  Word16    *normqlg2  (o)   recovered quantized norms                    */
/*--------------------------------------------------------------------------*/
void recovernorm(
Word16 *idxbuf,
Word16 *ynrm,
Word16 *normqlg2
)
{
    Word16 i;
    Word16 *pidx, *pnormq;


    FOR (i=0; i<2; i++)
    {
       pidx = ynrm + i;
       pnormq = normqlg2 + i;
       *pidx = idxbuf[i];
       *pnormq = dicnlg2[*pidx];
       pidx += 2;
       pnormq += 2;
       *pidx = idxbuf[21-i];
       *pnormq = dicnlg2[*pidx];
       pidx += 2;
       pnormq += 2;
       *pidx = idxbuf[22+i];
       *pnormq = dicnlg2[*pidx];
       pidx += 2;
       pnormq += 2;
       *pidx = idxbuf[43-i];
       *pnormq = dicnlg2[*pidx];
       pidx += 2;
       pnormq += 2;
       *pidx  = idxbuf[i+2];
       *pnormq = dicnlg2[*pidx];
       pidx += 2;
       pnormq += 2;
       *pidx = idxbuf[19-i];
       *pnormq = dicnlg2[*pidx];
       pidx += 2;
       pnormq += 2;
       *pidx = idxbuf[24+i];
       *pnormq = dicnlg2[*pidx];
       pidx += 2;
       pnormq += 2;
       *pidx = idxbuf[41-i];
       *pnormq = dicnlg2[*pidx];
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
       *pidx = idxbuf[i];
       *pnormq++ = dicnlg2[*pidx++];
       *pidx = idxbuf[21-i];
       *pnormq++ = dicnlg2[*pidx++];
       *pidx = idxbuf[22+i];
       *pnormq++ = dicnlg2[*pidx++];
       *pidx = idxbuf[43-i];
       *pnormq++ = dicnlg2[*pidx++];
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
