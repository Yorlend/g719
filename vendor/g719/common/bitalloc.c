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
/*  Function  bitalloc                                                      */
/*  ~~~~~~~~~~~~~~~~~~~~~                                                   */
/*                                                                          */
/*  Adaptive bit allocation for 20kHz audio codec                           */
/*--------------------------------------------------------------------------*/
/*  Word16      *y     (i)    reordered norm of sub-vectors                 */
/*  Word16      *idx   (i)    reordered sub-vector indices                  */
/*  Word16      sum    (i)    number of available bits                      */
/*  Word16      N      (i)    number of norms                               */
/*  Word16      M      (i)    maximum number of bits per dimension          */
/*  Word16      *r     (o)    bit-allacation vector                         */
/*--------------------------------------------------------------------------*/
void bitalloc(Word16 *y, Word16 *idx, Word16 sum, Word16 N, Word16 M, Word16 *r)
{
    Word16 i, j, k, n, m, v, im;
    Word16 diff, temp;


    im = (Word16)1;
    diff = sum;
#if(WMOPS)
  move16();
  move16();
#endif
    n = shr(sum, 3);
    FOR (i=0; i<n; i++)
    {
       k = (Word16)0;
       temp = y[0];
#if(WMOPS)
  move16();
  move16();
#endif
       FOR (m=1; m<=im; m++)
       {
          v = sub(y[m], temp);
          temp = s_max(temp, y[m]);
          if (v>0)
          {
            k = m;
#if(WMOPS)
  move16();
#endif
          }
       }
       if (sub(k, im)==0)
       {
         im = add(im, 1);
       }
       j = idx[k];
#if(WMOPS)
  move16();
#endif
#if(WMOPS)
  test();
#endif
       IF ((sub(sum, sfmsize[j])>=0) && (sub(r[j], M)<0))
       {
         y[k] = sub(y[k], 2);
         r[j] = add(r[j], 1);
#if(WMOPS)
  move16();
  move16();
#endif
         if (sub(r[j], M)>=0)
         {
           y[k] = (Word16)MIN_16;
#if(WMOPS)
  move16();
#endif
         }
         sum = sub(sum, sfmsize[j]);
       }
       ELSE
       {
         y[k] = (Word16)MIN_16;
#if(WMOPS)
  move16();
#endif
         k = add(k, 1);
         if (sub(k, im)==0)
         {
           im = add(im, 1);
         }
       }
#if(WMOPS)
  test();
#endif
       IF ((sub(sum, WID_G1)<0) || (sub(diff, sum)==0))
       {
         BREAK;
       }
       diff = sum;
#if(WMOPS)
  move16();
#endif
       v = sub(N, 2);
       IF (sub(k, v)>0)
       {
         FOR (i=0; i<N; i++)
         {
            IF (sub(y[i], MIN_16)>0)
            {
              im = add(i, 1);
              BREAK;
            }
         }
       }
    }
    IF (sub(sum, WID_G2)>=0)
    {
      FOR (i=0; i<N; i++)
      {
         j = idx[i];
#if(WMOPS)
  move16();
#endif
         v = add(SFM_G1, SFM_G2);
#if(WMOPS)
  test();
  test();
#endif
         IF ((sub(j, SFM_G1)>=0) && (sub(j, v)<0) && (r[j]==0))
         {
           r[j] = (Word16)1;
#if(WMOPS)
  move16();
#endif
           sum = sub(sum, WID_G2);
           IF (sub(sum, WID_G2)<0)
           {
             BREAK;
           }
         }
      }
    }
    IF (sub(sum, WID_G2)>=0)
    {
      FOR (i=0; i<N; i++)
      {
         j = idx[i];
#if(WMOPS)
  move16();
#endif
         v = add(SFM_G1, SFM_G2);
#if(WMOPS)
  test();
  test();
#endif
         IF ((sub(j, SFM_G1)>=0) && (sub(j, v)<0) && (r[j]==1))
         {
           r[j] = (Word16)2;
#if(WMOPS)
  move16();
#endif
           sum = sub(sum, WID_G2);
           IF (sub(sum, WID_G2)<0)
           {
             BREAK;
           }
         }
      }
    }
    IF (sub(sum, WID_G1)>=0)
    {
      FOR (i=0; i<N; i++)
      {
         j = idx[i];
#if(WMOPS)
  move16();
#endif
#if(WMOPS)
  test();
#endif
         IF ((sub(j, SFM_G1)<0) && (r[j]==0))
         {
           r[j] = (Word16)1;
#if(WMOPS)
  move16();
#endif
           sum = sub(sum, WID_G1);
           IF (sub(sum, WID_G1)<0)
           {
             BREAK;
           }
         }
      }
    }
    IF (sub(sum, WID_G1)>=0)
    {
      FOR (i=0; i<N; i++)
      {
         j = idx[i];
#if(WMOPS)
  move16();
#endif
#if(WMOPS)
  test();
#endif
         IF ((sub(j, SFM_G1)<0) && (r[j]==1))
         {
           r[j] = (Word16)2;
#if(WMOPS)
  move16();
#endif
           sum = sub(sum, WID_G1);
           IF (sub(sum, WID_G1)<0)
           {
             BREAK;
           }
         }
      }
    }


    return;
}
