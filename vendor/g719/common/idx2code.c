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
/*  Function  idx2code                                                      */
/*  ~~~~~~~~~~~~~~~~~~                                                      */
/*                                                                          */
/*  Finding a codevector from its index                                     */
/*--------------------------------------------------------------------------*/
/*  Word16    *k  (i)   index of the codevector                             */
/*  Word16    *y  (o)   codevector                                          */
/*  Word16    R   (i)   number of bits per coefficient                      */
/*--------------------------------------------------------------------------*/
void idx2code(Word16 *k, Word16 *y, Word16 R)
{
    Word16 i, m, tmp;
    Word16 v[8], z[8];


    tmp = sub(FacLVQ2Qv[R], R);
    m = shl(k[0], 1);
    FOR (i=1; i<8; i++)
    {
       m = add(m, k[i]);
    }
    z[0] = shl(m, tmp);
#if(WMOPS)
  move16();
#endif
    FOR (i=1; i<8; i++)
    {
       z[i] = shl(k[i], tmp);
#if(WMOPS)
  move16();
#endif
    }
    codesearch(z, v, R);
    y[0] = sub(m, shl(v[0], R));
#if(WMOPS)
  move16();
#endif
    FOR (i=1; i<8; i++)
    {
       y[i] = sub(k[i], shl(v[i], R));
#if(WMOPS)
  move16();
#endif
    }


    return;
}
