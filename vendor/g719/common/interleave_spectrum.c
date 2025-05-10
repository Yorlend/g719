/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  interleave_spectrum                                           */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                           */
/*                                                                          */
/*  Interleave the spectrum                                                 */
/*--------------------------------------------------------------------------*/
/*  Word32    *coefs  (i/o)   input and output coefficients                 */
/*--------------------------------------------------------------------------*/
void interleave_spectrum(Word32 *coefs)
{
   Word16   i, j;
   Word32   *p1a, *p1b, *p2a, *p2b, *p3a, *p3b, *p3c, *p4a, *p4b;
   Word32   coefs_short[STOP_BAND];
   Word32   *pcoefs, *pcoefs1, *pcoefs2;

   p1a    = coefs_short;
   pcoefs = coefs;

   FOR (i=0; i<4; i++) 
   {      
      FOR (j=0; j<STOP_BAND4; j++) 
      {
         p1a[j] = pcoefs[j];
#if(WMOPS)
  move32();
#endif
      }
      p1a += STOP_BAND4;
      pcoefs += FRAME_LENGTH/4;   
   }

   p1a = coefs;
   p1b = coefs + 64;
   p2a = coefs + NUMC_G1;
   p2b = coefs + NUMC_G1 + 64;
   p3a = coefs + NUMC_G23;
   p3b = coefs + NUMC_G23 + 96;
   p3c = coefs + NUMC_G23 + 192;
   p4a = coefs + NUMC_N;
   p4b = coefs + NUMC_N + 128;
   FOR (i=0; i<STOP_BAND; i+=STOP_BAND4)
   {
      pcoefs  = coefs_short + i;
      pcoefs1 = coefs_short + 16 + i;
      FOR (j=0; j<16; j++)
      {
         *p1a++ = *pcoefs++;
         *p1b++ = *pcoefs1++;
#if(WMOPS)
  move32();
  move32();
#endif
      }

      pcoefs  = coefs_short + NUMC_G1SUB + i;
      pcoefs1 = coefs_short + NUMC_G1SUB + 16 + i;
      FOR (j=0; j<16; j++)
      {
         *p2a++ = *pcoefs++;
         *p2b++ = *pcoefs1++;
#if(WMOPS)
  move32();
  move32();
#endif
      }

      pcoefs  = coefs_short + NUMC_G1G2SUB + i;
      pcoefs1 = coefs_short + NUMC_G1G2SUB + WID_G3 + i;
      pcoefs2 = coefs_short + NUMC_G1G2SUB + 2 * WID_G3 + i;
      FOR (j=0; j<WID_G3; j++)
      {
         *p3a++ = *pcoefs++;
         *p3b++ = *pcoefs1++;
         *p3c++ = *pcoefs2++;
#if(WMOPS)
  move32();
  move32();
  move32();
#endif
      }

      pcoefs  = coefs_short + NUMC_G1G2G3SUB + i;
      pcoefs1 = coefs_short + NUMC_G1G2G3SUB + WID_GX + i;
      FOR (j=0; j<WID_GX; j++)
      {
         *p4a++ = *pcoefs++;
         *p4b++ = *pcoefs1++;
#if(WMOPS)
  move32();
  move32();
#endif
      }
   }

}

/*--------------------------------------------------------------------------*/
/*  Function  de_interleave_spectrum                                        */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                        */
/*                                                                          */
/*  Deinterleave the spectrum                                               */
/*--------------------------------------------------------------------------*/
/*  Word32    *coefs  (i/o)   input and output coefficients                 */
/*--------------------------------------------------------------------------*/
void de_interleave_spectrum(Word32 *coefs)
{
   Word16   i, j;
   Word32   *p1a, *p1b, *p2a, *p2b, *p3a, *p3b, *p3c, *p4a, *p4b;
   Word32   coefs_short[STOP_BAND];
   Word32   *pcoefs, *pcoefs1, *pcoefs2;


   p1a = coefs;
   p1b = coefs + 64;
   p2a = coefs + NUMC_G1;
   p2b = coefs + NUMC_G1 + 64;
   p3a = coefs + NUMC_G23;
   p3b = coefs + NUMC_G23 + 96;
   p3c = coefs + NUMC_G23 + 192;
   p4a = coefs + NUMC_N;
   p4b = coefs + NUMC_N + 128;
   FOR (i=0; i<STOP_BAND; i+=STOP_BAND4)
   {
      pcoefs  = coefs_short + i;
      pcoefs1 = coefs_short + 16 + i;
      FOR (j=0; j<16; j++)
      {
         *pcoefs++  = *p1a++;
         *pcoefs1++ = *p1b++;
#if(WMOPS)
  move32();
  move32();
#endif
      }

      pcoefs  = coefs_short + NUMC_G1SUB + i;
      pcoefs1 = coefs_short + NUMC_G1SUB + 16 + i;
      FOR (j=0; j<16; j++)
      {
         *pcoefs++  = *p2a++;
         *pcoefs1++ = *p2b++;
#if(WMOPS)
  move32();
  move32();
#endif
      }

      pcoefs  = coefs_short + NUMC_G1G2SUB + i;
      pcoefs1 = coefs_short + NUMC_G1G2SUB + WID_G3 + i;
      pcoefs2 = coefs_short + NUMC_G1G2SUB + 2 * WID_G3 + i;
      FOR (j=0; j<WID_G3; j++)
      {
         *pcoefs++  = *p3a++;
         *pcoefs1++ = *p3b++;
         *pcoefs2++ = *p3c++;
#if(WMOPS)
  move32();
  move32();
  move32();
#endif
      }

      pcoefs  = coefs_short + NUMC_G1G2G3SUB + i;
      pcoefs1 = coefs_short + NUMC_G1G2G3SUB + WID_GX + i;
      FOR (j=0; j<WID_GX; j++)
      {
         *pcoefs++  = *p4a++;
         *pcoefs1++ = *p4b++;
#if(WMOPS)
  move32();
  move32();
#endif
      }
   }

   p1a    = coefs_short;
   pcoefs = coefs;
   FOR (i=0; i<4; i++) 
   {      
      FOR (j=0; j<STOP_BAND4; j++) 
      {
         pcoefs[j] = p1a[j];
#if(WMOPS)
  move32();
#endif
      }
      FOR (j=STOP_BAND4; j<FRAME_LENGTH/4; j++) 
      {
         pcoefs[j] = (Word32)0;
#if(WMOPS)
  move32();
#endif
      }
      p1a += STOP_BAND4;
      pcoefs += FRAME_LENGTH/4;   
   }

}
