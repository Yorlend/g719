/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "typedef.h"
#include "state.h"
#include "cnst.h"
#include "basop32.h"
#include "move.h"


/*--------------------------------------------------------------------------*/
/*  Function  hp_filter                                                     */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  High pass filter                                                        */
/*--------------------------------------------------------------------------*/
/*  Word16      x      (i)    input to filter                               */
/*  Word16      y      (o)    output of filter                              */
/*  Word16      *oldy  (i/o)  previous output of filter                     */
/*  Word16      *oldx  (i/o)  previous input of filter                      */
/*--------------------------------------------------------------------------*/
void hp_filter(Word16 x[], Word16 y[], Word16 *oldy, Word16 *oldx)
{
   Word32 i, tmp32;

   tmp32 = L_mac0((Word32)0, *oldy, 32316);
   tmp32 = L_mac0(tmp32, x[0], 24465);
   tmp32 = L_msu0(tmp32, *oldx, 24465);

   
   y[0] = round(tmp32);
#if(WMOPS)
  move16();
#endif

   FOR (i = 1; i < FRAME_LENGTH; i++) 
   {      
      tmp32 = L_mac0((Word32)0, y[i-1], 32316);
      tmp32 = L_mac0(tmp32, x[i], 24465);
      tmp32 = L_msu0(tmp32, x[i-1], 24465);      
      y[i] = round(tmp32);
#if(WMOPS)
  move16();
#endif
   }

   *oldy = y[FRAME_LENGTH-1];
   *oldx = x[FRAME_LENGTH-1];
#if(WMOPS)
  move16();
  move16();
#endif

}

/*--------------------------------------------------------------------------*/
/*  Function  compute_energy                                                */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Compute the energy using 64 bits                                        */
/*--------------------------------------------------------------------------*/
/*  Word16      in[]    (i)    input signal                                 */
/*  Word16      length  (i)    length of input                              */
/*  Word32      * elo   (o)    energy lower word                            */
/*  Word32      * ehi   (o)    energy higher word                           */
/*--------------------------------------------------------------------------*/
void compute_energy(Word16 in[],
                    Word16 length,
                    Word32 * elo,
                    Word32 * ehi)
{
   Word16 i;
   Word32 lo32;
   Word32 hi32;


   Overflow = 0;
   Carry    = 0;
#if(WMOPS)
  move16();
  move16();
#endif

   hi32 = (Word32)0;
   lo32 = (Word32)0;
#if(WMOPS)
  move32();
  move32();
#endif  

   FOR (i = 0; i < length; i++)
   {
      lo32 = L_macNs(lo32, in[i], in[i]);

      if (Overflow)
      {
         hi32 = L_add(hi32, 1);          
      }
      if (Overflow)
      {
         lo32 = L_and(lo32, 0x7fffffff);      
      }
      if (Overflow)
      {
         Overflow = 0;
#if(WMOPS)
  move16();
#endif
      }
   }

   *elo = lo32;
   *ehi = hi32;
#if(WMOPS)
  move32();
  move32();
#endif

}

/*--------------------------------------------------------------------------*/
/*  Function  detect_transient                                              */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Detect if the signal is a transient                                     */
/*--------------------------------------------------------------------------*/
/*  Word16      in[]    (i)      input signal                               */
/*  CoderState  *c      (i/o)    state of coder                             */
/*--------------------------------------------------------------------------*/
/*  Word16      return  (o)      result of transient check                  */
/*--------------------------------------------------------------------------*/
Word16 detect_transient(Word16 in[],
                        CoderState *c)
{
   Word16 IsTransient;
   Word16 out_filt[FRAME_LENGTH];
   Word16 blk;
   Word16 start;

   Word32 Energy_lo;
   Word32 Energy_hi;

   Word32 EnergyLT_hi;
   Word32 EnergyLT_lo;

   Word32 lsb;
   Word32 diff32_hi;
   Word32 diff32_lo;
   Word32 tmp32_hi;
   Word32 tmp32_lo;

   Word16 n;
   Word16 n_inv;
   Word32 e32;
   Word32 elt32;


   IsTransient = FALSE;
#if(WMOPS)
  move16();
#endif

   hp_filter(in, out_filt, &(c->old_hpfilt_in), &(c->old_hpfilt_out));
   
   EnergyLT_hi = c->EnergyLT_hi;
   EnergyLT_lo = c->EnergyLT_lo;
#if(WMOPS)
  move32();
  move32();
#endif

   start = (Word16)0;
#if(WMOPS)
  move16();
#endif

   FOR (blk = 0; blk < 4; blk++) 
   {  
      compute_energy(&out_filt[start], FRAME_LENGTH / 4, &Energy_lo, &Energy_hi);

      diff32_hi = L_sub(Energy_hi, EnergyLT_hi);
      diff32_lo = L_sub(Energy_lo, EnergyLT_lo);
#if(WMOPS)
   test();
   test();
#endif
      IF (diff32_hi > 0 || (diff32_hi == 0 && diff32_lo > 0))
      {
         n = (Word16)31;
#if(WMOPS)
  move16();
#endif
         if(Energy_hi)
         {
            n = norm_l(Energy_hi);
         }
         n_inv = sub(31, n);
         e32 = L_shl(Energy_hi, n);
         e32 = L_add(e32, L_shr(Energy_lo, n_inv));

         elt32 = L_shl(EnergyLT_hi, n);
         elt32 = L_add(elt32, L_shr(EnergyLT_lo, n_inv));

         e32 = L_mls(e32, TRANSIENTDETECTOR_THRESHOLD);

         if(L_sub(e32, elt32)>0)
         {
            IsTransient = TRUE;
#if(WMOPS)
  move16();
#endif
         }
      }

      tmp32_hi = L_add(EnergyLT_hi, Energy_hi);
      Overflow = 0;
      Carry    = 0;
#if(WMOPS)
  move16();
  move16();
#endif
      tmp32_lo = L_add_c(EnergyLT_lo, Energy_lo);

      IF (Overflow) 
      {
         tmp32_hi = L_add(tmp32_hi,1);
         tmp32_lo = L_and(tmp32_lo, 0x7fffffff);   
         Overflow = 0;
#if(WMOPS)
  move16();
#endif
      }

      lsb      = L_and(tmp32_hi,1);      
      tmp32_hi = L_shr(tmp32_hi,1);
      tmp32_lo = L_shr(tmp32_lo,1);
      
      if(lsb) 
      {
         tmp32_lo = L_add(tmp32_lo,0x40000000);
      }

      tmp32_hi = L_add(EnergyLT_hi, tmp32_hi);
      
      Overflow = 0;
      Carry    = 0;
#if(WMOPS)
  move16();
  move16();
#endif
      tmp32_lo = L_add_c(EnergyLT_lo, tmp32_lo);
     
      IF (Overflow) {         
         tmp32_hi = L_add(tmp32_hi,1);
         tmp32_lo = L_and(tmp32_lo, 0x7fffffff);   
         Overflow = 0;
#if(WMOPS)
  move16();
#endif
      }

      lsb         = L_and(tmp32_hi,1);      
      EnergyLT_hi = L_shr(tmp32_hi,1);
      EnergyLT_lo = L_shr(tmp32_lo,1);      
      if(lsb) 
      {
         EnergyLT_lo = L_add(EnergyLT_lo,0x40000000);
      }
      start = add(start,FRAME_LENGTH / 4);
   }

   c->EnergyLT_hi = EnergyLT_hi;
   c->EnergyLT_lo = EnergyLT_lo;

#if(WMOPS)
  move32();
  move32();
#endif   

   IF (IsTransient) 
   {
      c->TransientHangOver = TRUE;  
#if(WMOPS)
  move16();
#endif
   }
   ELSE 
   {
      IF ( c->TransientHangOver ) 
      {
         c->TransientHangOver = FALSE;
         IsTransient          = TRUE;     
#if(WMOPS)
  move16();
  move16();
#endif
      }   
   }

   return IsTransient;
}
