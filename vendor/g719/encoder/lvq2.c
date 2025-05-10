/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "stl.h"
#include "proto.h"

/*--------------------------------------------------------------------------*/
/*  Function  lvq2                                                          */
/*  ~~~~~~~~~~~~~~                                                          */
/*                                                                          */
/*  High-rate lattice vector quantization                                   */
/*--------------------------------------------------------------------------*/
/*  Word16    *x  (i)   arbitrary vector                                    */
/*  Word16    *k  (o)   index of the selected codevector                    */
/*  Word16    r   (i)   number of indices                                   */
/*  Word16    R   (i)   number of bits per coefficinet                      */
/*--------------------------------------------------------------------------*/
void lvq2(Word16 *x, Word16 *k, Word16 r, Word16 R)
{
    Word16 i, j;
    Word16 err, temp, temp1, temp2;
    Word16 v[8], y[8], kk[8], w[8];
    Word32 L_tmp;


    codesearch(x, v, R);
    code2idx(v, k, r);
    idx2code(k, y, R);

    err = sub(v[0], y[0]);
    temp = abs_s(err);
    FOR (i=1; i<8; i++)
    {
       err = sub(v[i], y[i]);
       temp = add(temp, abs_s(err));
    }
    IF (temp!=0)
    {
      FOR (i=0; i<15; i++)
      {
         FOR (j=0; j<8; j++)
         {
            x[j] = shr_r(x[j], 1);
#if(WMOPS)
  move16();
#endif
         }
         codesearch(x, v, R);
         code2idx(v, k, r);
         idx2code(k, y, R);
         err = sub(v[0], y[0]);
         temp1 = abs_s(err);
         FOR (j=1; j<8; j++)
         {
            err = sub(v[j], y[j]);
            temp1 = add(temp1, abs_s(err));
         }
         IF (temp1==0)
         {
           BREAK;
         }
      }

      IF (temp1!=0)
      {
        FOR (i=0; i<8; i++)
        {
           k[i] = (Word16)0;
#if(WMOPS)
  move16();
#endif
        }
      }
      ELSE
      {
        FOR (i=1; i<16; i++)
        {
           FOR (j=0; j<8; j++)
           {
              L_tmp = L_mult0(x[j], i);
              L_tmp = L_shr(L_tmp, 4);
              temp2 = extract_l(L_tmp);
              if (temp2<0)
              {
                temp2 = add(temp2, 1);
              }
              w[j] = add(x[j], temp2);
#if(WMOPS)
  move16();
#endif
           }
           codesearch(w, v, R);
           code2idx(v, kk, r);
           idx2code(kk, y, R);
           err = sub(v[0], y[0]);
           temp2 = abs_s(err);
           FOR (j=1; j<8; j++)
           {
              err = sub(v[j], y[j]);
              temp2 = add(temp2, abs_s(err));
           }
           IF (temp2!=0)
           {
             BREAK;
           }
           FOR (j=0; j<8; j++)
           {
              k[j] = kk[j];
#if(WMOPS)
  move16();
#endif
           }
        }
      }
    }


    return;
}
