/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"

/*--------------------------------------------------------------------------*/
/*  Function  code2idx                                                      */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Finding the index of a codevector                                       */
/*--------------------------------------------------------------------------*/
/*  Word16      *x  (i)  codevector                                         */
/*  Word16      *k  (o)  index of the codevector                            */
/*  Word16      r   (i)  number of indices                                  */
/*--------------------------------------------------------------------------*/
void code2idx(Word16 *x, Word16 *k, Word16 r)
{
    Word16 i, m, v;


    m = x[0];
#if(WMOPS)
  move16();
#endif
    v = sub(r, 1);
    FOR (i=1; i<8; i++)
    {
       m = sub(m, x[i]);
    }
    m = shr(m, 1);
    k[0] = s_and(m, v);
#if(WMOPS)
  move16();
#endif
    FOR (i=1; i<8; i++)
    {
       k[i] = s_and(x[i], v);
#if(WMOPS)
  move16();
#endif
    }


    return;
}
