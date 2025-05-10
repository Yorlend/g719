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
/*  Function  qcoefs                                                        */
/*  ~~~~~~~~~~~~~~~~                                                        */
/*                                                                          */
/*  Vector quantization for normalized MLT coefficients                     */
/*--------------------------------------------------------------------------*/
/*  Word16    *coefs  (i)   normalized MLT coefficients                     */
/*  Word16    *R      (i)   number of bits per coefficient                  */
/*  Word16    N1      (i)   beginning sub-vector's number in the groups     */
/*  Word16    N2      (i)   ending sub-vector's number in the group         */
/*  Word16    L       (i)   number of coefficients in each sub-vector       */
/*  Word16    *y      (o)   indices of the selected codevectors             */
/*--------------------------------------------------------------------------*/
void qcoefs(Word16 *coefs, Word16 *R, Word16 N1, Word16 N2, Word16 L, Word16 *y)
{
    Word16 i, j, n, v, rv;
    Word16 nb_vecs, temp;
    Word16 x[8];
    Word32 L_temp;
    Word16 *pcoefs, *pidx;


    pidx = y;
    pcoefs = coefs;
    nb_vecs = shr(L, 3);
    FOR (n=N1; n<N2; n++)
    {
       v = R[n];
#if(WMOPS)
  move16();
#endif
       IF (sub(v, 1)>0)
       {
         rv = RV[v];
#if(WMOPS)
  move16();
#endif
         FOR (i=0; i<nb_vecs; i++)
         {
            FOR (j=0; j<8; j++)
            {
               temp = sub(*pcoefs++, OFFSET);
               L_temp = L_mult0(FCT_LVQ2, temp);
               x[j] = extract_l(L_shr(L_temp, 12));
#if(WMOPS)
  move16();
#endif
            }
            lvq2(x, pidx, rv, v);
            pidx += 8;
         }
       }
       ELSE if (sub(v, 1)==0)
       {
         FOR (i=0; i<nb_vecs; i++)
         {
            FOR (j=0; j<8; j++)
            {
               temp = sub(*pcoefs++, OFFSET);
               x[j] = add(temp, mult(temp, FCT_LVQ1));
#if(WMOPS)
  move16();
#endif
            }
            lvq1(x, pidx);
            pidx += 8;
         }
       }
       ELSE
       {
         pidx += L;
         pcoefs += L;
       }
    }


    return;
}
