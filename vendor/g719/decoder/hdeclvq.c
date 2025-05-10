/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  hdec2blvq                                                     */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Huffman decoding for LVQ2 quantization indices                          */
/*--------------------------------------------------------------------------*/
/*  Word16      *bitstream  (i)    Huffman code                             */
/*  Word16      N           (i)    number of coefficients                   */
/*  Word16      *index      (o)    LVQ2 quantization indices                */
/*--------------------------------------------------------------------------*/
void hdec2blvq(Word16 *bitstream, Word16 N, Word16 *index)
{
  Word16 i;
  Word16 temp;
  Word16 *pbits, *pidx;


  pbits = bitstream;
  pidx  = index;

  FOR (i=0; i<N; i++)
  {
     temp = (Word16)0;
#if(WMOPS)
  move16();
#endif
     IF (sub(*pbits++, G192_BIT1)==0)
     {
       temp = (Word16)3;
#if(WMOPS)
  move16();
#endif
       IF (sub(*pbits++, G192_BIT1)==0)
       {
         temp = (Word16)1;
#if(WMOPS)
  move16();
#endif
         if (sub(*pbits++, G192_BIT1)==0)
         {
           temp = add(temp, 1);
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

void hdec3blvq(Word16 *bitstream, Word16 N, Word16 *index)
{
  Word16 i, j, k, m;
  Word16 temp;
  Word16 *pbits, *pidx;


  pbits = bitstream;
  pidx  = index;

  FOR (i=0; i<N; i++)
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
     k = shl(j, 1);
     if (sub(*pbits++, G192_BIT1)==0)
     {
       k = add(k, 1);
     }
     temp = add(shl(j, 2), k);
     IF (sub(k, 2)==0)
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
       m = shl(j, 1);
       if (sub(*pbits++, G192_BIT1)==0)
       {
         m = add(m, 1);
       }
       temp = add(add(shl(j, 1), m), 1);
       IF (m==0)
       {
         temp = (Word16)3;
#if(WMOPS)
  move16();
#endif
         if (sub(*pbits++, G192_BIT1)==0)
         {
           temp = add(temp, 1);
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

void hdec4blvq(Word16 *bitstream, Word16 N, Word16 *index)
{
  Word16 i, j, k, m;
  Word16 temp;
  Word16 *pbits, *pidx;


  pbits = bitstream;
  pidx  = index;

  FOR (i=0; i<N; i++)
  {
     k = (Word16)0;
#if(WMOPS)
  move16();
#endif
     if (sub(*pbits++, G192_BIT1)==0)
     {
       k = (Word16)2;
#if(WMOPS)
  move16();
#endif
     }
     if (sub(*pbits++, G192_BIT1)==0)
     {
       k = add(k, 1);
     }
     temp = (Word16)0;
#if(WMOPS)
  move16();
#endif
     IF (k!=0)
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
       temp = (Word16)1;
#if(WMOPS)
  move16();
#endif
       if (j!=0)
       {
         temp = (Word16)15;
#if(WMOPS)
  move16();
#endif
       }
       IF (sub(k, 3)!=0)
       {
         m = shl(j, 1);
         if (sub(*pbits++, G192_BIT1)==0)
         {
           m = add(m, 1);
         }
         temp = m;
#if(WMOPS)
  move16();
#endif
         if (j==0)
         {
           temp = add(m, 13);
         }
         IF (sub(k, 1)!=0)
         {
           m = shl(m, 1);
           if (sub(*pbits++, G192_BIT1)==0)
           {
             m = add(m, 1);
           }
           temp = m;
#if(WMOPS)
  move16();
#endif
           if (j==0)
           {
             temp = add(m, 9);
           }
           IF (sub(m, 7)==0)
           {
             temp = m;
#if(WMOPS)
  move16();
#endif
             if (sub(*pbits++, G192_BIT1)==0)
             {
               temp = add(1, m);
             }
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


void hdec5blvq(Word16 *bitstream, Word16 N, Word16 *index)
{
  Word16 i, j, k, m, n;
  Word16 temp;
  Word16 *pbits, *pidx;


  pbits = bitstream;
  pidx  = index;

  FOR (i=0; i<N; i++)
  {
     n = (Word16)0;
#if(WMOPS)
  move16();
#endif
     if (sub(*pbits++, G192_BIT1)==0)
     {
       n = (Word16)2;
#if(WMOPS)
  move16();
#endif
     }
     if (sub(*pbits++, G192_BIT1)==0)
     {
       n = add(n, 1);
     }
     temp = (Word16)0;
#if(WMOPS)
  move16();
#endif
     IF (n!=0)
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
       temp = (Word16)1;
#if(WMOPS)
  move16();
#endif
       if (j!=0)
       {
         temp = (Word16)31;
#if(WMOPS)
  move16();
#endif
       }
       IF (sub(n, 1)!=0)
       {
         k = (Word16)0;
#if(WMOPS)
  move16();
#endif
         if (sub(*pbits++, G192_BIT1)==0)
         {
           k = (Word16)1;
#if(WMOPS)
  move16();
#endif
         }
         IF (sub(n, 2)==0)
         {
           temp = (Word16)2;
#if(WMOPS)
  move16();
#endif
           if (k!=0)
           {
             temp = (Word16)30;
#if(WMOPS)
  move16();
#endif
           }
           IF (j!=0)
           {
             temp = (Word16)3;
#if(WMOPS)
  move16();
#endif
             if (sub(*pbits++, G192_BIT1)==0)
             {
               temp = add(temp, 1);
             }
             if (k!=0)
             {
               temp = add(temp, 25);
             }
           }
         }
         ELSE
         {
           m = (Word16)0;
#if(WMOPS)
  move16();
#endif
           if (sub(*pbits++, G192_BIT1)==0)
           {
             m = (Word16)2;
#if(WMOPS)
  move16();
#endif
           }
           if (sub(*pbits++, G192_BIT1)==0)
           {
             m = add(m, 1);
           }
           temp = add(m, 5);
           if (k!=0)
           {
             temp = add(temp, 19);
           }
           IF (j!=0)
           {
             m = add(shl(k, 2), m);
             temp = (Word16)23;
#if(WMOPS)
  move16();
#endif
             IF (sub(m, 7)!=0)
             {
               m = shl(m, 1);
               if (sub(*pbits++, G192_BIT1)==0)
               {
                 m = add(m, 1);
               }
               temp = add(9, m);
             }
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
