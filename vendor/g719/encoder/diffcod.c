/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "stl.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  diffcod                                                       */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Differential coding for indices of quantized norms                      */
/*--------------------------------------------------------------------------*/
/*  Word16   *normqlg2  (i)   quantized norms in log2                       */
/*  Word16   N          (i)   number of sub-vectors                         */
/*  Word16   *y         (i/o) indices of quantized norms                    */
/*  Word16   *difidx    (o)   differential code                             */
/*--------------------------------------------------------------------------*/
void diffcod(Word16 *normqlg2, Word16 N, Word16 *y, Word16 *difidx)
{
    Word16 i, j, k, m, r, v;


    m = sub(N, 1);
    FOR (i=m; i>0; i--)
    {
       r = sub(i, 1);
       k = sub(y[i], y[r]);
       v = sub(k, (Word16)-15);
       if (v<0)
       {
         y[r] = add(y[i], 15);
#if(WMOPS)
  move16();
#endif
       }
    }
    FOR (i=1; i<N; i++)
    {
       r = sub(i, 1);
       k = sub(y[i], y[r]);
       v = sub(k, 16);
       k = s_min(k, 16);
       if (v>0)
       {
         y[i] = add(y[r], k);
#if(WMOPS)
  move16();
#endif
       }
       j = y[i];
       normqlg2[i] = dicnlg2[j];
       difidx[r] = add(k, 15);
#if(WMOPS)
  move16();
  move16();
  move16();
#endif
    }


    return;
}
