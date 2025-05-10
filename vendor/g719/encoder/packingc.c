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
/*  Function  packingc                                                      */
/*  ~~~~~~~~~~~~~~~~~~                                                      */
/*                                                                          */
/*  Huffman encoding and packing indices of quantized coefficients          */
/*--------------------------------------------------------------------------*/
/*  Word32    *y      (i)   indices of the selected codevectors             */
/*  Word16    *R      (i)   number of bits per coefficinet                  */
/*  Word16    *pbits  (i)   pointer to bitstream                            */
/*  Word16    flag    (i)   Huffman code flag                               */
/*  Word16    N1      (i)   beginning sub-vector's number in the group      */
/*  Word16    N2      (i)   ending sub-vector's number in the group         */
/*  Word16    L       (i)   number of coefficients in each sub-vector       */
/*--------------------------------------------------------------------------*/
/*  Word16    return  (o)   length of Huffman code                          */
/*--------------------------------------------------------------------------*/
Word16 packingc(Word16 *y, Word16 *R, Word16 *pbits, Word16 flag, Word16 N1, Word16 N2, Word16 L)
{
    Word16 i, j, n, r, v, rv, offset, sum;
    Word16 nb_vecs, length, temp;
    Word16 *pidx, *ptmp;


    pidx = y;
    nb_vecs = shr(L, 3);
    length = (Word16)0;
#if(WMOPS)
  move16();
#endif
    IF (sub(flag, NOHUFCODE)==0)
    {
      FOR (n=N1; n<N2; n++)
      {
         rv = R[n];
#if(WMOPS)
  move16();
#endif
         IF (sub(rv, 1)>0)
         {
           idx2bitsc(pidx, L, rv, pbits);
           sum = extract_l(L_mult0(rv, L));
           pbits += sum;
           pidx += L;
           length = add(length, sum);
         }
         ELSE if (sub(rv, 1)==0)
         {
           FOR (i=0; i<nb_vecs; i++)
           {
              idx2bitsc(pidx, 8, 1, pbits);
              pbits += 8;
              pidx += 8;
           }
           length = add(length, L);
         }
         ELSE
         {
           pidx += L;
         }
      }
    }
    ELSE
    {
      FOR (n=N1; n<N2; n++)
      {
         rv = R[n];
#if(WMOPS)
  move16();
#endif
         IF (sub(rv, QBIT_MAX1)>0)
         {
           idx2bitsc(pidx, L, rv, pbits);
           sum = extract_l(L_mult0(rv, L));
           pbits += sum;
           pidx += L;
           length = add(length, sum);
         }
         ELSE if (sub(rv, 1)>0)
         {
           offset = huffoffset[rv];
#if(WMOPS)
  move16();
#endif
           FOR (i=0; i<L; i++)
           {
              j = add(offset, *pidx++);
              v = huffcoef[j];
              r = huffsizc[j];
#if(WMOPS)
  move16();
  move16();
#endif
              ptmp = pbits + r;
              FOR (j=0; j<r; j++)
              {
                 temp = s_and(shr(v, j), 1);
                 ptmp--;
                 *ptmp = G192_BIT1;
#if(WMOPS)
  move16();
#endif
                 if (temp==0)
                 {
                   *ptmp = G192_BIT0;
#if(WMOPS)
  move16();
#endif
                 }
              }
              pbits += r;
              length = add(length, r);
           }
         }
         ELSE if (sub(rv, 1)==0)
         {
           FOR (i=0; i<nb_vecs; i++)
           {
              idx2bitsc(pidx, 8, 1, pbits);
              pbits += 8;
              pidx += 8;
           }
           length = add(length, L);
         }
         ELSE
         {
           pidx += L;
         }
      }
    }


    return(length);
}
