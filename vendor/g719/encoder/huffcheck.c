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
/*  Function  huffcheck                                                     */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Check Huffman encoding for LVQ2 indices                                 */
/*--------------------------------------------------------------------------*/
/*  Word16    *y     (i)   indices of the selected codevectors              */
/*  Word16    *R     (i)   number of bits per coefficinet                   */
/*  Word16    N1     (i)   beginning sub-vector's number in the group       */
/*  Word16    N2     (i)   ending sub-vector's number in the group          */
/*  Word16    L      (o)   number of coefficients in each sub-vector        */
/*--------------------------------------------------------------------------*/
/*  Word16    return (o)   length of Huffman code                           */
/*--------------------------------------------------------------------------*/
Word16 huffcheck(Word16 *y, Word16 *R, Word16 N1, Word16 N2, Word16 L)
{
    Word16 i, j, n, v, offset;
    Word16 hcode_l;
    Word16 *pidx;


    pidx = y;
    hcode_l = (Word16)0;
#if(WMOPS)
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
         hcode_l = add(hcode_l, extract_l(L_mult0(L, v)));
         pidx += L;
       }
       ELSE if (sub(v, 1)>0)
       {
         offset = huffoffset[v];
#if(WMOPS)
  move16();
#endif
         FOR (i=0; i<L; i++)
         {
            j = add(offset, *pidx++);
            hcode_l = add(hcode_l, huffsizc[j]);
         }
       }
       ELSE if (sub(v, 1)==0)
       {
         hcode_l = add(hcode_l, L);
         pidx += L;
       }
       ELSE
       {
         pidx += L;
       }
    }


    return(hcode_l);
}
