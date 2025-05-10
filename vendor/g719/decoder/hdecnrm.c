/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "stl.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  hdecnrm                                                       */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Huffman decoding for indices of quantized norms                         */
/*--------------------------------------------------------------------------*/
/*  Word16      *bitstream  (i)    Huffman code                             */
/*  Word16      N           (i)    number of norms                          */
/*  Word16      *index      (o)    indices of quantized norms               */
/*--------------------------------------------------------------------------*/
void hdecnrm(Word16 *bitstream, Word16 N, Word16 *index)
{
  Word16 i, j, k, n, m;
  Word16 temp;
  Word16 *pbits, *pidx;


  pbits = bitstream;
  pidx  = index;

  m = sub(N, 1);
  FOR (i=0; i<m; i++)
  {
     j = (Word16)0;
     k = (Word16)0;
#if(WMOPS)
  move16();
  move16();
#endif
     if (sub(*pbits++, G192_BIT1)==0)
     {
       j = (Word16)1;
#if(WMOPS)
  move16();
#endif
     }
     if (sub(*pbits++, G192_BIT1)==0)
     {
       k = (Word16)1;
#if(WMOPS)
  move16();
#endif
     }
     n = add(shl(j, 1), k);
     j = shl(j, 2);
     temp = sub(add(16, n), j);
     IF (sub(*pbits++, G192_BIT1)==0)
     {
       temp = add(add(12, n), j);
       IF (sub(*pbits++, G192_BIT1)==0)
       {
         j = (Word16)0;
#if(WMOPS)
  move16();
#endif
         if (sub(*pbits++, G192_BIT1)==0)
         {
           j = (Word16)1;
#if(WMOPS)
  move16();
#endif
         }
         temp = add(8, n);
         if (j!=0)
         {
           temp = add(temp, 12);
         }
         IF (sub(*pbits++, G192_BIT1)==0)
         {
           temp = n;
#if(WMOPS)
  move16();
#endif
           if (sub(*pbits++, G192_BIT1)==0)
           {
             temp = add(4, n);
           }
           if (j!=0)
           {
             temp = add(temp, 24);
           }
         }
       }
     }
     *pidx++ = temp;
#if(WMOPS)
  move16();
#endif
  }


  return;
}
