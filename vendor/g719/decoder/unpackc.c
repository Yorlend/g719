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
/*  Function  unpackc                                                       */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Huffman decoding and unpacking indices for quantized coefficients       */
/*--------------------------------------------------------------------------*/
/*  Word16    *R      (i)   number of bits per coefficinet                  */
/*  Word16    *pbits  (i)   pointer to bitstream                            */
/*  Word16    flag    (i)   Huffman code flag                               */
/*  Word16    rv      (i)   offset for index of quantized coefficients      */
/*  Word16    N1      (i)   beginning sub-vector's number in the group      */
/*  Word16    N2      (i)   ending sub-vector's number in the group         */
/*  Word16    L       (o)   number of coefficients in each sub-vector       */
/*  Word16    *y      (o)   indices of the selected codevectors             */
/*--------------------------------------------------------------------------*/
/*  Word16    return  (o)   length of Huffman code                          */
/*--------------------------------------------------------------------------*/
Word16 unpackc(Word16 *R,
               Word16 *pbits, 
               Word16 flag, 
               Word16 rv,
               Word16 N1,
               Word16 N2,
               Word16 L,
               Word16 *y)
{
    Word16 i, j, k, n, r, v, hcode_l, offset, sum;
    Word16 nb_vecs, length;


    nb_vecs = shr(L, 3);
    length = (Word16)0;
#if(WMOPS)
  move16();
#endif
    IF (sub(flag, NOHUFCODE)==0)
    {
      FOR (n=N1; n<N2; n++)
      {
         v = R[n];
#if(WMOPS)
  move16();
#endif
         IF (sub(v, 1)>0)
         {
           bits2idxc(pbits, L, v, &y[rv]);
           sum = extract_l(L_mult0(v, L));
           pbits += sum;
           length = add(length, sum);
         }
         ELSE if (sub(v, 1)==0)
         {
           k = rv;
#if(WMOPS)
  move16();
#endif
           FOR (i=0; i<nb_vecs; i++)
           {
              bits2idxc(pbits, 8, 1, &y[k]);
              pbits += 8;
              k = add(k, 8);
           }
           length = add(length, L);
         }
         rv = add(rv, L);
      }
    }
    ELSE
    {
      r = (Word16)0;
      hcode_l = (Word16)0;
#if(WMOPS)
  move16();
  move16();
#endif
      FOR (n=N1; n<N2; n++)
      {
         v = R[n];
#if(WMOPS)
  move16();
#endif
         IF (sub(v, QBIT_MAX1)>0)
         {
           bits2idxc(pbits, L, v, &y[rv]);
           sum = extract_l(L_mult0(v, L));
           pbits += sum;
           r = add(r, sum);
         }
         ELSE if (sub(v, 1)>0)
         {
           IF (sub(v, 2)==0)
           {
             hdec2blvq(pbits, L, &y[rv]);
           }
           ELSE if (sub(v, 3)==0)
           {
             hdec3blvq(pbits, L, &y[rv]);
           }
           ELSE if (sub(v, 4)==0)
           {
             hdec4blvq(pbits, L, &y[rv]);
           }
           ELSE
           {
             hdec5blvq(pbits, L, &y[rv]);
           }
           offset = huffoffset[v];
#if(WMOPS)
  move16();
#endif
           FOR (i=0; i<L; i++)
           {
              k = add(rv, i);
              j = add(offset, y[k]);
              hcode_l = add(hcode_l, huffsizc[j]);
              pbits += huffsizc[j];
           }
         }
         ELSE if (sub(v, 1)==0)
         {
           k = rv;
#if(WMOPS)
  move16();
#endif
           FOR (i=0; i<nb_vecs; i++)
           {
              bits2idxc(pbits, 8, 1, &y[k]);
              pbits += 8;
              k = add(k, 8);
           }
           r = add(r, L);
         }
         rv = add(rv, L);
      }
      length = add(hcode_l, r);
    }


    return(length);
}
