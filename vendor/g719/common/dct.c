/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "cnst.h"
#include "complxop.h"
#include "rom.h"
#include "stl.h"

/*--------------------------------------------------------------------------*/
/*  Function  wfft120                                                       */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Winograd 120 point FFT                                                  */
/*--------------------------------------------------------------------------*/
/*  complex32      *xi      (i/o)    input and output of the FFT            */
/*--------------------------------------------------------------------------*/
void wfft120(complex32 *xi)
{
    Word16 ind0, n3, n5;
    Word32 t0r, t1r, t2r, t3r, t4r, t5r, t6r, t7r, q1r, x1r, x5r;
    Word32 t0i, t1i, t2i, t3i, t4i, t5i, t6i, t7i, q1i, x1i, x5i;
    Word16 *ps;
    complex32 x[144];
    complex32 *p0, *p1, *p2, *p3, *p4, *p5, *p6, *p7;
    complex32 *pi0, *pi1, *pi2, *pi3, *pi4, *pi5, *pi6, *pi7;

    p0 = x;
    FOR (ind0=0; ind0<120; ind0+=8)
    {                
        pi0 = xi + indxPre[ind0+0];
        pi1 = xi + indxPre[ind0+1];
        pi2 = xi + indxPre[ind0+2];
        pi3 = xi + indxPre[ind0+3];
        pi4 = xi + indxPre[ind0+4];
        pi5 = xi + indxPre[ind0+5];
        pi6 = xi + indxPre[ind0+6];
        pi7 = xi + indxPre[ind0+7];

        t0r = L_add(pi0->r, pi4->r);
        t0i = L_add(pi0->i, pi4->i);
        t1r = L_add(pi1->r, pi5->r);
        t1i = L_add(pi1->i, pi5->i);
        t2r = L_add(pi2->r, pi6->r);
        t2i = L_add(pi2->i, pi6->i);
        t3r = L_add(pi3->r, pi7->r);
        t3i = L_add(pi3->i, pi7->i);
        t5r = L_sub(pi1->r, pi5->r);
        t5i = L_sub(pi1->i, pi5->i);
        t7r = L_sub(pi3->r, pi7->r);
        t7i = L_sub(pi3->i, pi7->i);

        p0->r = L_add(t0r, t2r);
        p0->i = L_add(t0i, t2i);
        p0++;
        p0->r = L_add(t1r, t3r);
        p0->i = L_add(t1i, t3i);
        p0++;
        p0->r = L_sub(t0r, t2r);
        p0->i = L_sub(t0i, t2i);
        p0++;
        p0->r = L_sub(t1r, t3r);
        p0->i = L_sub(t1i, t3i);
        p0++;
        p0->r = L_sub(pi0->r, pi4->r);
        p0->i = L_sub(pi0->i, pi4->i);
        p0++;
        p0->r = L_add(t5r, t7r);
        p0->i = L_add(t5i, t7i);
        p0++;
        p0->r = L_sub(pi2->r, pi6->r);
        p0->i = L_sub(pi2->i, pi6->i);
        p0++;
        p0->r = L_sub(t5r, t7r);
        p0->i = L_sub(t5i, t7i);
        p0++;
#if(WMOPS)
{
   Word16 j;
   for (j=0; j<16; j++) 
      move32();
}
#endif
    }

    FOR (n5=0; n5<120; n5+=24)
    {
        FOR (ind0=n5; ind0<(n5+8); ind0++)
        {
            p0 = x + ind0;
            p1 = x + ind0 + 8;
            p2 = x + ind0 + 16;

            x1r = p1->r;
            x1i = p1->i;
            p1->r = L_add(x1r, p2->r);
            p1->i = L_add(x1i, p2->i);
            p2->r = L_sub(x1r, p2->r);
            p2->i = L_sub(x1i, p2->i);
            cadd3232(*p0, *p1, p0);
#if(WMOPS)
  move32();
  move32();
  move32();
  move32();
  move32();
  move32();
#endif
        }
    }

    FOR (n3=0; n3<24; n3+=8)
    {
        FOR (ind0=n3; ind0<(n3+8); ind0++)
        {
            p0 = x + ind0;
            p1 = x + ind0 + 24;
            p2 = x + ind0 + 48;
            p3 = x + ind0 + 72;
            p4 = x + ind0 + 96;
            p5 = x + ind0 + 120;
            ps = fft120cnst + ind0;

            t3r = L_add(p3->r, p2->r);
            t3i = L_add(p3->i, p2->i);
            t2r = L_sub(p3->r, p2->r);
            t2i = L_sub(p3->i, p2->i);
            t1r = L_add(p1->r, p4->r);
            t1i = L_add(p1->i, p4->i);
            csub3232(*p1, *p4, p2);
            
            p1->r = L_add(t1r, t3r);
            p1->i = L_add(t1i, t3i);
            p3->r = L_sub(t1r, t3r);
            p3->i = L_sub(t1i, t3i);
#if(WMOPS)
  move32();
  move32();
  move32();
  move32();
#endif
            x5r = L_add(p2->r, t2r);
            x5i = L_add(p2->i, t2i);
            cadd3232(*p0, *p1, p0);

            cmpy3216s(*p0, *ps, p0);
            ps += 24;
            cmpy3216s(*p1, *ps, p1);
            ps += 24;
            cmpy3216s(*p2, *ps, p2);
            ps += 24;
            cmpy3216s(*p3, *ps, p3);
            ps += 24;
            p4->r = L_mls(t2r, *ps);
            p4->i = L_mls(t2i, *ps);
            ps += 24;
            p5->r = L_mls(x5r, *ps);
            p5->i = L_mls(x5i, *ps);
#if(WMOPS)
  move32();
  move32();
  move32();
  move32();
#endif
        }
    }

    FOR (n3=0; n3<24; n3+=8)
    {
        FOR (ind0=n3; ind0<(n3+8); ind0++)
        {       
            p0 = x + ind0;
            p1 = x + ind0 + 24;
            p2 = x + ind0 + 48;
            p3 = x + ind0 + 72;
            p4 = x + ind0 + 96;
            p5 = x + ind0 + 120;

            cmpy32j(p2);
            cmpy32j(p4);
            cmpy32j(p5);

            q1r = L_add(p0->r, p1->r);
            q1i = L_add(p0->i, p1->i);

            t1r = L_add(q1r, p3->r);
            t1i = L_add(q1i, p3->i);
            t4r = L_add(p2->r, p5->r);
            t4i = L_add(p2->i, p5->i);
            t3r = L_sub(q1r, p3->r);
            t3i = L_sub(q1i, p3->i);
            t2r = L_add(p4->r, p5->r);
            t2i = L_add(p4->i, p5->i);

            p1->r = L_add(t1r, t4r);
            p1->i = L_add(t1i, t4i);
            p4->r = L_sub(t1r, t4r);
            p4->i = L_sub(t1i, t4i);
            p3->r = L_add(t3r, t2r);
            p3->i = L_add(t3i, t2i);
            p2->r = L_sub(t3r, t2r);
            p2->i = L_sub(t3i, t2i);
#if(WMOPS)
{
   Word16 j;
   for (j=0; j<8; j++) 
      move32();
}
#endif
        }
    }

    FOR (n5=0; n5<120; n5+=24)
    {
        FOR (ind0=n5; ind0<(n5+8); ind0++)
        {
            p0 = x + ind0;
            p1 = x + ind0 + 8;
            p2 = x + ind0 + 16;

            t1r = L_add(p0->r, p1->r);
            t1i = L_add(p0->i, p1->i);
            cmpy32j(p2);

            p1->r = L_add(t1r, p2->r);
            p1->i = L_add(t1i, p2->i);
            p2->r = L_sub(t1r, p2->r);          
            p2->i = L_sub(t1i, p2->i);          
#if(WMOPS)
  move32();
  move32();
  move32();
  move32();
#endif
       }

    }

    FOR (ind0=0; ind0<120; ind0+=8)
    {       
        p0 = x + ind0;
        p1 = x + ind0 + 1;
        p2 = x + ind0 + 2;
        p3 = x + ind0 + 3;
        p4 = x + ind0 + 4;
        p5 = x + ind0 + 5;
        p6 = x + ind0 + 6;
        p7 = x + ind0 + 7;

        pi0 = xi + indxPost[ind0+0];
        pi1 = xi + indxPost[ind0+1];
        pi2 = xi + indxPost[ind0+2];
        pi3 = xi + indxPost[ind0+3];
        pi4 = xi + indxPost[ind0+4];
        pi5 = xi + indxPost[ind0+5];
        pi6 = xi + indxPost[ind0+6];
        pi7 = xi + indxPost[ind0+7];

        cmpy32j(p3); 
        cmpy32j(p5); 
        cmpy32j(p6); 

        t5r = L_add(p4->r, p5->r);
        t5i = L_add(p4->i, p5->i);
        t4r = L_sub(p4->r, p5->r);
        t4i = L_sub(p4->i, p5->i);
        t7r = L_add(p6->r, p7->r);
        t7i = L_add(p6->i, p7->i);
        t6r = L_sub(p6->r, p7->r);
        t6i = L_sub(p6->i, p7->i);

        cadd3232(*p0, *p1, pi0);
        csub3232(*p0, *p1, pi4);
        cadd3232(*p2, *p3, pi2);
        csub3232(*p2, *p3, pi6);
        pi1->r = L_add(t5r, t7r);
        pi1->i = L_add(t5i, t7i);
        pi5->r = L_add(t4r, t6r);
        pi5->i = L_add(t4i, t6i);
        pi3->r = L_sub(t5r, t7r);
        pi3->i = L_sub(t5i, t7i);
        pi7->r = L_sub(t4r, t6r);
        pi7->i = L_sub(t4i, t6i);
#if(WMOPS)
{
   Word16 j;
   for (j=0; j<8; j++) 
      move32();
}
#endif
    }


    return;
}

/*--------------------------------------------------------------------------*/
/*  Function  wfft480                                                       */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Winograd 480 points FFT                                                 */
/*--------------------------------------------------------------------------*/
/*  complex32      *pI      (i)    input to the FFT                         */
/*  complex32      *pO      (o)    output to the FFT                        */
/*--------------------------------------------------------------------------*/
static void wfft480(complex32 *pI, complex32 *pO)
{
    Word32 i, j, temp;
    Word32 x0r, x1r, x2r, x3r, t0r, t1r, t2r, t3r;
    Word32 x0i, x1i, x2i, x3i, t0i, t1i, t2i, t3i;
    complex16 *pw1, *pw2, *pw3;
    complex32 *pt0, *pt1, *pt2, *pt3;

    pt0 = pI;
    pt1 = pI + 120;
    pt2 = pI + 240;
    pt3 = pI + 360;
    pw1 = ptwdf + 1;
    pw2 = ptwdf + 2;
    pw3 = ptwdf + 3;
    FOR (i=0; i<120; i++)
    {
        t0r = L_add(pt0->r, pt2->r);
        t0i = L_add(pt0->i, pt2->i);
        t2r = L_sub(pt0->r, pt2->r);
        t2i = L_sub(pt0->i, pt2->i);
        t1r = L_add(pt1->r, pt3->r);
        t1i = L_add(pt1->i, pt3->i);
        t3r = L_sub(pt3->r, pt1->r);
        t3i = L_sub(pt3->i, pt1->i);

        temp = t3r;
        t3r = L_negate(t3i);
        t3i = temp;
#if(WMOPS)
  move32();
  move32();
#endif

        x0r = L_add(t0r, t1r);
        x0i = L_add(t0i, t1i);
        x1r = L_add(t2r, t3r);
        x1i = L_add(t2i, t3i);
        x2r = L_sub(t0r, t1r);
        x2i = L_sub(t0i, t1i);
        x3r = L_sub(t2r, t3r);
        x3i = L_sub(t2i, t3i);

        // twiddle factors
        pt0->r = x0r;
        pt0->i = x0i;
        pt1->r = L_sub(L_mls(x1r, pw1->r), L_mls(x1i, pw1->i));
        pt1->i = L_add(L_mls(x1r, pw1->i), L_mls(x1i, pw1->r));
        pt2->r = L_sub(L_mls(x2r, pw2->r), L_mls(x2i, pw2->i));
        pt2->i = L_add(L_mls(x2r, pw2->i), L_mls(x2i, pw2->r));
        pt3->r = L_sub(L_mls(x3r, pw3->r), L_mls(x3i, pw3->i));
        pt3->i = L_add(L_mls(x3r, pw3->i), L_mls(x3i, pw3->r));
#if(WMOPS)
  move32();
  move32();
  move32();
  move32();
  move32();
  move32();
  move32();
  move32();
#endif
        pt0++;
        pt1++;
        pt2++;
        pt3++;
        pw1 += 4;
        pw2 += 4;
        pw3 += 4;
    }

    FOR (i=0; i<480; i+=120)
    {
        wfft120(pI+i);
    }

    pt0 = pO;
    FOR (j=0; j<120; j++)
    {
        pt1 = pI + j;
        FOR (i=0; i<4; i++)
        {
            *pt0++ = *pt1;
#if(WMOPS)
  move32();
  move32();
#endif
            pt1 += 120;
        }
    }


    return;
}

/*--------------------------------------------------------------------------*/
/*  Function  dct4_960                                                      */
/*  ~~~~~~~~~~~~~~~~~~                                                      */
/*                                                                          */
/*  DCT4 960 points                                                         */
/*--------------------------------------------------------------------------*/
/*  Word32      v[]        (i)    input of the DCT4                         */
/*  Word32      coefs32[]  (o)    coefficient of the DCT4                   */
/*--------------------------------------------------------------------------*/
void dct4_960(Word32 v[MLT960_LENGTH], Word32 coefs32[MLT960_LENGTH])
{
    Word16 n;
    Word32 f_int[MLT960_LENGTH];


    FOR (n=0; n<MLT960_LENGTH; n+=2)  
    {
        f_int[n]   = L_sub(L_mls(v[n], dct480_table_1[n>>1].r ), 
                           L_mls(v[(MLT960_LENGTH_MINUS_1-n)], dct480_table_1[n>>1].i)); 
        f_int[n+1] = L_add(L_mls(v[n], dct480_table_1[n>>1].i), 
                           L_mls(v[(MLT960_LENGTH_MINUS_1-n)], dct480_table_1[n>>1].r));
#if(WMOPS)
  move32();
  move32();
#endif
    }

    wfft480((complex32 *)f_int, (complex32 *)v); 

    FOR (n=0; n<MLT960_LENGTH; n+=2)
    {
        coefs32[n] = L_sub(L_mls(v[n], dct480_table_2[n>>1].r), 
                           L_mls(v[n+1], dct480_table_2[n>>1].i));
        coefs32[MLT960_LENGTH_MINUS_1-n] = L_negate(L_add(L_mls(v[n], dct480_table_2[n>>1].i), 
                                                          L_mls(v[n+1], dct480_table_2[n>>1].r)));
#if(WMOPS)
  move32();
  move32();
#endif
    }

}

/*--------------------------------------------------------------------------*/
/*  Function  dct4_240                                                      */
/*  ~~~~~~~~~~~~~~~~~~                                                      */
/*                                                                          */
/*  DCT4 240 points                                                         */
/*--------------------------------------------------------------------------*/
/*  Word32      v[]        (i)    input of the DCT4                         */
/*  Word32      coefs32[]  (o)    coefficient of the DCT4                   */
/*--------------------------------------------------------------------------*/
void dct4_240(Word32 v[MLT240_LENGTH], Word32 coefs32[MLT240_LENGTH])
{
    Word16 n;
    Word32 f_int[MLT240_LENGTH];


    FOR (n=0;n<MLT240_LENGTH;n+=2)  
    {
        f_int[n]   = L_sub(L_mls(v[n], dct120_table_1[n>>1].r ), 
                           L_mls(v[(MLT240_LENGTH_MINUS_1-n)], dct120_table_1[n>>1].i)); 
        f_int[n+1] = L_add(L_mls(v[n], dct120_table_1[n>>1].i), 
                           L_mls(v[(MLT240_LENGTH_MINUS_1-n)],dct120_table_1[n>>1].r));
#if(WMOPS)
  move32();
  move32();
#endif
    }

    wfft120((complex32 *)f_int); 

    FOR (n=0;n<MLT240_LENGTH;n+=2)
    {
        coefs32[n] = L_sub(L_mls(f_int[n], dct120_table_2[n>>1].r), 
                           L_mls(f_int[n+1],dct120_table_2[n>>1].i));
        coefs32[MLT240_LENGTH_MINUS_1-n] = L_negate(L_add(L_mls(f_int[n], dct120_table_2[n>>1].i), 
                                                          L_mls(f_int[n+1], dct120_table_2[n>>1].r)));
#if(WMOPS)
  move32();
  move32();
#endif
    }

}
