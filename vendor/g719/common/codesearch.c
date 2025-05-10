/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  codesearch                                                    */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Finding the closest point of lattice                                    */
/*--------------------------------------------------------------------------*/
/*  Word16      *x  (i)  arbitrary vector in Qv                             */
/*  Word16      *C  (o)  point of lattice                                   */
/*  Word16      R   (i)  number of bits per coefficient                     */
/*--------------------------------------------------------------------------*/
void codesearch(Word16 *x, Word16 *C, Word16 R)
{
    Word16 i, j, sum;
    Word16 e[8], em, temp, temp1, temp2, tempC;


    sum = (Word16)0;
#if(WMOPS)
  move16();
#endif
    FOR (i=0; i<8; i++)
    {
       temp1 = shr(x[i], FacLVQ2Qv[R]);
       temp = s_and(x[i], FacLVQ2Mask[R]);
       temp2 = sub(temp, FacLVQ2HalfQv[R]);
       if (temp2<=0)
       {
         tempC = (Word16)0;
       }
       if (temp2>0)
       {
         tempC = (Word16)1;
       }
#if(WMOPS)
  move16();
#endif
       if (x[i]>=0)
       {
         temp = (Word16)0;
       }
       if (x[i]<0)
       {
         temp = (Word16)1;
       }
#if(WMOPS)
  move16();
#endif
       if (temp2==0)
       {
         tempC = tempC | temp;
#if(WMOPS)
  logic16();
#endif
       }
       C[i] = add(temp1, tempC);
#if(WMOPS)
  move16();
#endif
       sum = add(sum, C[i]);
    }
    IF (s_and(sum, 1))
    {
      j = (Word16)0;
      em = (Word16)0;
#if(WMOPS)
  move16();
  move16();
#endif
      FOR (i=0; i<8; i++)
      {
         temp = shl(C[i], FacLVQ2Qv[R]);
         e[i] = sub(x[i], temp);
#if(WMOPS)
  move16();
#endif
         temp = abs_s(e[i]);
         if (sub(em, temp)<0)
         {
           j = i;
#if(WMOPS)
  move16();
#endif
         }
         em = s_max(em, temp);
      }
      if (e[j]>=0)
      {
        C[j] = add(C[j], 1);
      }
      if (e[j]<0)
      {
        C[j] = sub(C[j], 1);
      }
#if(WMOPS)
  move16();
#endif
    }


    return;
}
