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
/*  Function  procnf                                                        */
/*  ~~~~~~~~~~~~~~~~                                                        */
/*                                                                          */
/*  Quantization for sub-vectors originally allocated with 0 bits           */
/*--------------------------------------------------------------------------*/
/*  Word16    *coefs  (i)   normalized MLT coefficients                     */
/*  Word16    *y      (i)   indices of the selected codevectors             */
/*  Word16    *pbits  (i)   pointer to bitstream                            */
/*  Word16    nb_vecs (i)   number of 8-D vectors in current sub-vector     */
/*--------------------------------------------------------------------------*/
void procnf(Word16 *coefs, Word16 *y, Word16 *pbits, Word16 nb_vecs)
{
    Word16 i, j;
    Word16 temp;
    Word16 x[8];


    FOR (i=0; i<nb_vecs; i++)
    {
       FOR (j=0; j<8; j++)
       {
          temp = sub(*coefs++, OFFSET);
          x[j] = add(temp, mult(temp, FCT_LVQ1));
#if(WMOPS)
  move16();
#endif
       }
       lvq1(x, y);
       idx2bitsc(y, 8, 1, pbits);
       pbits += 8;
       y += 8;
    }


    return;
}
