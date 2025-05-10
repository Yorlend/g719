/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "cnst.h"

/*--------------------------------------------------------------------------*/
/*  Function  idx2bitsn                                                     */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Conversion from norm index into ITU bit stream                          */
/*--------------------------------------------------------------------------*/
/*  Word16      x   (i)  index of quantized norm                            */
/*  Word16      N   (i)  bits per norm                                      */
/*  Word16      *y  (o)  ITU bitstream                                      */
/*--------------------------------------------------------------------------*/
void idx2bitsn(Word16 x, Word16 N, Word16 *y)
{
    Word16 i;
    Word16 temp;


    y += N;
    FOR (i=0; i<N; i++)
    {
       temp = s_and(shr(x, i), 1);
       y--;
       *y = G192_BIT1;
#if(WMOPS)
  move16();
#endif
       if (temp==0)
       {
         *y = G192_BIT0;
#if(WMOPS)
  move16();
#endif
       }
    }


    return;
}

/*--------------------------------------------------------------------------*/
/*  Function  idx2bitsc                                                     */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Conversion from quantization index into ITU bit stream                  */
/*--------------------------------------------------------------------------*/
/*  Word16      *x  (i)  quantization index                                 */
/*  Word16      N   (i)  vector dimensions                                  */
/*  Word16      L   (i)  bits per coefficient                               */
/*  Word16      *y  (o)  ITU bitstream                                      */
/*--------------------------------------------------------------------------*/
void idx2bitsc(Word16 *x, Word16 N, Word16 L, Word16 *y)
{
    Word16 i, j, m, n;
    Word16 temp;
    Word16 *pty;


    IF (sub(L, 1)==0)
    {
      n = (Word16)1;
      m = N;
    }
    ELSE
    {
      n = N;
      m = L;
    }
#if(WMOPS)
  move16();
  move16();
#endif

    FOR (j=0; j<n; j++)
    {
       y += m;
       pty = y;
       FOR (i=0; i<m; i++)
       {
          temp = s_and(shr(x[j], i), 1);
          pty--;
          *pty = G192_BIT1;
#if(WMOPS)
  move16();
#endif
          if (temp==0)
          {
            *pty = G192_BIT0;
#if(WMOPS)
  move16();
#endif
          }
       }
    }


    return;
}

/*--------------------------------------------------------------------------*/
/*  Function  bits2idxn                                                     */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Conversion from ITU bit stream into norm index                          */
/*--------------------------------------------------------------------------*/
/*  Word16      *y   i)  ITU bitstream                                      */
/*  Word16      N   (i)  bits per norm                                      */
/*  Word16      *x  (o)  index of quantized norm                            */
/*--------------------------------------------------------------------------*/
void bits2idxn(Word16 *y, Word16 N, Word16 *x)
{
    Word16 i;


    *x = (Word16)0;
#if(WMOPS)
  move16();
#endif
    FOR (i=0; i<N; i++)
    {
       *x = shl(*x, 1);
       if (sub(*y++, G192_BIT1)==0)
       {
         *x = add(*x, 1);
       }
    }


    return;
}

/*--------------------------------------------------------------------------*/
/*  Function  bits2idxc                                                     */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Conversion from ITU bit stream into coefficient index                   */
/*--------------------------------------------------------------------------*/
/*  Word16      *y  (i)  ITU bitstream                                      */
/*  Word16      N   (i)  vector dimensions                                  */
/*  Word16      L   (i)  bits per coefficient                               */
/*  Word16      *x  (o)  index of quantized coefficient                     */
/*--------------------------------------------------------------------------*/
void bits2idxc(Word16 *y, Word16 N, Word16 L, Word16 *x)
{
    Word16 i, k, m, n;
    Word16 temp;


    IF (sub(L, 1)==0)
    {
      n = (Word16)1;
      m = N;
    }
    ELSE
    {
      n = N;
      m = L;
    }
#if(WMOPS)
  move16();
  move16();
#endif

    FOR (k=0; k<n; k++)
    {
       x[k] = (Word16)0;
#if(WMOPS)
  move16();
#endif
       FOR (i=0; i<m; i++)
       {
          temp = shl(x[k], 1);
          if (sub(*y++, G192_BIT1)==0)
          {
            temp = add(temp, 1);
          }
          x[k] = temp;
#if(WMOPS)
  move16();
#endif
       }
    }


    return;
}
