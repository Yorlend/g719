/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"

/*--------------------------------------------------------------------------*/
/*  Function  reordvct                                                      */
/*  ~~~~~~~~~~~~~~~~~~                                                      */
/*                                                                          */
/*  Rearrange a vector in decreasing order                                  */
/*--------------------------------------------------------------------------*/
/*  Word16    *y    (i/o) vector to rearrange                               */
/*  Word16    N     (i)   dimensions                                        */
/*  Word16    *idx  (o)   reordered vector index                            */
/*--------------------------------------------------------------------------*/
void reordvct(Word16 *y, Word16 N, Word16 *idx)
{
    Word16 i, j, k, n, im, temp;


    n = sub(N, 1);
    FOR (i=0; i<n; i++)
    {
       im = i;
#if(WMOPS)
  move16();
#endif
       k = add(i, 1);
       FOR (j=k; j<N; j++)
       {
          temp = sub(y[im], y[j]);
          if (temp<0)
          {
            im = j;
#if(WMOPS)
  move16();
#endif
          }
       }
       temp = y[i];
       y[i] = y[im];
       y[im] = temp;
       j = idx[i];
       idx[i] = idx[im];
       idx[im] = j;
#if(WMOPS)
  move16();
  move16();
  move16();
  move16();
  move16();
  move16();
#endif
    }


    return;
}
