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
/*  Function  lvq1                                                          */
/*  ~~~~~~~~~~~~~~                                                          */
/*                                                                          */
/*  1-bit lattice vector quantization                                       */
/*--------------------------------------------------------------------------*/
/*  Word16    *x  (i)   arbitrary vector                                    */
/*  Word16    *k  (o)   index of the codevector                             */
/*--------------------------------------------------------------------------*/
void lvq1(Word16 *x, Word16 *k)
{
    Word16 i, j, m, n, im, offset, temp;
    Word16 idx[8];
    Word32 dist, L_acc;


    /* Searching */
    FOR (i=0; i<8; i++)
    {
       idx[i] = i;
#if(WMOPS)
  move16();
#endif
    }
    FOR (i=0; i<7; i++)
    {
       im = i;
#if(WMOPS)
  move16();
#endif
       n = add(i, 1);
       FOR (j=n; j<8; j++)
       {
          if (sub(x[im], x[j])<0)
          {
            im = j;
#if(WMOPS)
  move16();
#endif
          }
       }
       temp = x[i];
       x[i] = x[im];
       x[im] = temp;
#if(WMOPS)
  move16();
  move16();
  move16();
#endif
       j = idx[i];
       idx[i] = idx[im];
       idx[im] = j;
#if(WMOPS)
  move16();
  move16();
  move16();
#endif
    }
    m = (Word16)0;
    dist = MAX_32;
#if(WMOPS)
  move16();
  move32();
#endif
    FOR (i=0; i<N_LEADER1; i++)
    {
       temp = sub(x[0], dic1Q8[i][0]);
       L_acc = L_mult0(temp, temp);
       FOR (j=1; j<8; j++)
       {
          temp = sub(x[j], dic1Q8[i][j]);
          L_acc = L_mac0(L_acc, temp, temp);
       }
       if (L_sub(L_acc, dist)<0)
       {
         m = i;
#if(WMOPS)
  move16();
#endif
       }
       dist = L_min(L_acc, dist);
    }

    /* Indexing */
    offset = dic2[m];
#if(WMOPS)
  move16();
#endif
    IF (sub(m,3)==0)
    {
      j = extract_l(L_mult0(idx[0], dic1[m][0]));
      FOR (i=1; i<8; i++)
      {
         n = extract_l(L_mult0(idx[i], dic1[m][i]));
         j = add(j, n);
      }
      if (j<0)
      {
        offset = add(offset, 28);
      }
    }
    j = shl(dic0[m][0], idx[0]);
    FOR (i=1; i<8; i++)
    {
       n = shl(dic0[m][i], idx[i]);
       j = add(j, n);
    }
    *k = add(offset, dic3[j]);


    return;
}
