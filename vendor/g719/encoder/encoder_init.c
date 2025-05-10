/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "state.h"
#include "cnst.h"
#include "stl.h"

/*--------------------------------------------------------------------------*/
/*  Function  encoder_reset_tables                                          */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                          */
/*                                                                          */
/*  Reset encoder tables                                                    */
/*--------------------------------------------------------------------------*/
/*  CoderState    *c          (i)   state of encoder                        */
/*  Word16        num_bits    (i)   number of bits                          */
/*--------------------------------------------------------------------------*/
void encoder_reset_tables(CoderState *c,Word16 num_bits)
{

   c->num_bits_spectrum_stationary = sub(num_bits, 3);
   c->num_bits_spectrum_transient  = sub(num_bits, 1);
   c->num_bits                     = num_bits;

#if(WMOPS)
   move16();
   move16();
   move16();
#endif
}

/*--------------------------------------------------------------------------*/
/*  Function  encoder_init                                                  */
/*  ~~~~~~~~~~~~~~~~~~~~~~                                                  */
/*                                                                          */
/*  Initialize the state of the encoder                                     */
/*--------------------------------------------------------------------------*/
/*  CoderState    *c          (i)   state of coder                          */
/*  Word16        num_bits    (i)   number of bits                          */
/*--------------------------------------------------------------------------*/
void encoder_init(CoderState *c, Word16 num_bits)
{
   Word16 i;
  
   FOR (i=0 ; i < FRAME_LENGTH/2 ; i++)
   {
      c->old_wtda[i] = (Word16)0;  
#if(WMOPS)
  move16();
#endif
   }

   FOR (i=0 ; i < NUM_MAP_BANDS ; i++)
   {     
      c->old_sf[i] = (Word16)0;
#if(WMOPS)
  move16();
#endif
   }

   c->old_hpfilt_in = (Word16)0;
   c->old_hpfilt_out = (Word16)0;

   c->EnergyLT_hi = (Word32)0;
   c->EnergyLT_lo = (Word32)0;
   
   c->TransientHangOver = FALSE;
#if(WMOPS)
  move16();
  move16();
  move32();
  move32();
  move16();
#endif

   encoder_reset_tables(c,num_bits);
}
