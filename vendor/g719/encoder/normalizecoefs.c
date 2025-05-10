/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "stl.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  normalizecoefs                                                */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~                                                */
/*                                                                          */
/*  Normalization of MLT coefficients with quantized norms                  */
/*--------------------------------------------------------------------------*/
/*  Word32    *coefs       (i)   MLT coefficients                           */
/*  Word16    *ynrm        (i)   quantization indices for norms             */
/*  Word16    N1           (i)   beginning sub-vector's number              */
/*  Word16    N2           (i)   ending sub-vector's number                 */
/*  Word16    L            (i)   number of coefficients per sub-vector      */
/*  Word16    *coefs_norm  (o)   normalized MLT coefficients                */
/*--------------------------------------------------------------------------*/
void normalizecoefs(Word32 *coefs,
                    Word16 *ynrm,
                    Word16 N1,
                    Word16 N2,
                    Word16 L,
                    Word16 *coefs_norm)
{
   Word16 i, j, k, r, v;
   Word16 *pcoefs16;
   Word32 *pcoefs;


   pcoefs   = coefs;
   pcoefs16 = coefs_norm;
   FOR (i=N1; i<N2; i++)
   {
      r = s_and(ynrm[i], 1);
      v = shr(ynrm[i], 1);
      k = sub(sub(17, r), v);
      FOR (j=0; j<L; j++)
      {
         if (r!=0)
         {
            *pcoefs = L_mls(*pcoefs, INV2POWHALF);
#if(WMOPS)
  move32();
#endif
         }
         *pcoefs16++ = extract_l(L_shr(*pcoefs++, k));
#if(WMOPS)
  move16();
#endif
      }
   }


   return;
}
