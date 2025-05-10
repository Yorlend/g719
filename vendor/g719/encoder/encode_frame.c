/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "state.h"
#include "cnst.h"
#include "typedef.h"
#include "stl.h"
#include "proto.h"

/*--------------------------------------------------------------------------*/
/*  Function  encode_frame                                                  */
/*  ~~~~~~~~~~~~~~~~~~~~~                                                   */
/*                                                                          */
/*  Encodes a single frame                                                  */
/*--------------------------------------------------------------------------*/
/*  Word16      *audio      (i)    input audio                              */
/*  Word16      num_bits    (i)    number of bits                           */
/*  Word16      *bitstream  (o)    encoded bitstream                        */
/*  CoderState  *c          (i/o)  coderstate                               */
/*--------------------------------------------------------------------------*/
void encode_frame(Word16      *audio, 
                  Word16      num_bits,
                  Word16      *bitstream, 
                  CoderState  *c)
{
   Word16 i;
   Word16 audio_window[2*FRAME_LENGTH];
   Word16 *new_audio = audio_window + FRAME_LENGTH;   

   Word32 wtda_audio[FRAME_LENGTH];
   Word32 t_audio[FRAME_LENGTH];
   
   Word16 t_audio_norm[FREQ_LENGTH];

   Word16 is_transient;
   Word16 bitalloc[NB_SFM];

   Word32 wtda_max;
   Word16 norm_wtda;
   Word16 nf_idx;
   Word16 bits;
   Word16 **pbitstream;
   Word16 *tbitstream;
   
   tbitstream = bitstream;

   /* detect transient */
   is_transient = detect_transient(audio, c);

   /* set the transient bit */ 
   IF (is_transient) 
   {      
      *bitstream++ = G192_BIT1;
   } 
   ELSE 
   {
      *bitstream++ = G192_BIT0;
   }   
#if(WMOPS)
  move16();
#endif

   /* windowing and time domain aliasing */
   wtda(audio, wtda_audio,c->old_wtda);

   wtda_max = (Word32)0;
#if(WMOPS)
  move32();
#endif
   FOR (i=0; i < FRAME_LENGTH; i++) 
   {
      wtda_max = L_max(wtda_max,L_abs(wtda_audio[i]));   
   }

   norm_wtda = norm_l(wtda_max);

   FOR (i=0; i < FRAME_LENGTH; i++) 
   {
      wtda_audio[i] = L_shl(wtda_audio[i],norm_wtda);   
#if(WMOPS)
  move32();
#endif
   }

   /* transform */
   direct_transform(wtda_audio, t_audio, is_transient);

   FOR (i=0; i < FRAME_LENGTH; i++) 
   {
      t_audio[i] = L_shr(t_audio[i],norm_wtda);   
#if(WMOPS)
  move32();
#endif
   }
     
   pbitstream = &bitstream;
   /* quantize normalized spectrum */
   IF (is_transient) 
   {
      flvqenc(pbitstream, t_audio, t_audio_norm, bitalloc,  
              c->num_bits_spectrum_transient, c->num_bits, is_transient);
   }        
   ELSE 
   {
      flvqenc(pbitstream, t_audio, t_audio_norm, bitalloc,  
              c->num_bits_spectrum_stationary, c->num_bits, is_transient);
      nf_idx = noise_adjust(t_audio_norm, bitalloc);
      idx2bitsn(nf_idx, 2, bitstream);
      bitstream += 2;
   }
   
   /* fill the bitstream buffer */
   bits = bitstream - tbitstream;
   bits = sub(c->num_bits , bits);

   FOR (i = 0; i < bits; i++)
   {
      *bitstream++ = G192_BIT0;
#if(WMOPS)
  move16();
#endif
   }

}
