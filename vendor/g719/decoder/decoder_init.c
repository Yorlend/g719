/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include "state.h"
#include "cnst.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  decoder_reset_tables                                          */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                                          */
/*                                                                          */
/*  Reset decoder tables                                                    */
/*--------------------------------------------------------------------------*/
/*  decoderState  *d          (i)   state of decoder                        */
/*  Word16        num_bits    (i)   number of bits                          */
/*--------------------------------------------------------------------------*/
void decoder_reset_tables(DecoderState *d, Word16 num_bits)
{   
   d->num_bits_spectrum_stationary = sub(num_bits, 3);
   d->num_bits_spectrum_transient  = sub(num_bits, 1);
   d->num_bits                     = num_bits;

#if(WMOPS)
   move16();
   move16();
   move16();
#endif
}

/*--------------------------------------------------------------------------*/
/*  Function  decoder_init                                                  */
/*  ~~~~~~~~~~~~~~~~~~~~~~                                                  */
/*                                                                          */
/*  Initialize the state of the decoder                                     */
/*--------------------------------------------------------------------------*/
/*  DecoderState  *d          (i)   state of decoder                        */
/*  Word16        num_bits    (i)   number of bits                          */
/*--------------------------------------------------------------------------*/
void decoder_init(DecoderState *d, Word16 num_bits)
{
   Word16 i;

   FOR (i=0 ; i < FRAME_LENGTH ; i++)
   {      
      d->old_out[i] = (Word32)0;
      d->old_coeffs[i] = (Word32)0;
      d->old_norm_taudio = (Word32)0;
#if(WMOPS)
  move32();
  move32();
  move32();
#endif
   }
     
   d->old_is_transient = (Word16)0;
#if(WMOPS)
  move16();
#endif

   decoder_reset_tables(d, num_bits);
}
