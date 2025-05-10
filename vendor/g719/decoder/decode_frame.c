/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "state.h"
#include "cnst.h"
#include "proto.h"


/*--------------------------------------------------------------------------*/
/*  Function  decode_frame                                                  */
/*  ~~~~~~~~~~~~~~~~~~~~~~                                                  */
/*                                                                          */
/*  Decodes a single frame                                                  */
/*--------------------------------------------------------------------------*/
/*  Word16        bitstream[]  (i)    bitstream to decode                   */
/*  Word16        bfi          (i)    bad frame indicator                   */
/*  Word16        out16[]      (o)    decoded audio                         */
/*  DecoderState  *d           (i/o)  state of decoder                      */
/*--------------------------------------------------------------------------*/
void decode_frame(Word16 bitstream[], 
                  Word16 bfi, 
                  Word16 out16[],
                  DecoderState *d)
{
   Word16 is_transient;
   Word32 t_audio_q[FRAME_LENGTH];
   Word32 wtda_audio[2*FRAME_LENGTH];
   Word16 bitalloc[NB_SFM];   
   Word16 ynrm[NB_SFM];
   Word16 i;
   Word16 audio_q_norm[FREQ_LENGTH];
   Word32 taudio_max;
   Word16 norm_taudio;
   Word16 nf_idx;
   Word16 **pbitstream;
   Word16 *tbitstream;


   IF (bfi) 
   {   
      /* processing in case of frame-errasures */
      FOR (i=0; i < FRAME_LENGTH; i++) 
      {
         /* replace by old frame */
         t_audio_q[i] = d->old_coeffs[i];
         /* fade-out */
         d->old_coeffs[i] = L_shr(d->old_coeffs[i],1);
#if(WMOPS)
  move32();
  move32();
#endif
         
      }
      norm_taudio = d->old_norm_taudio;
      is_transient = d->old_is_transient;
#if(WMOPS)
  move16();
  move16();
#endif
   }
   ELSE
   {
      /* get the transient bit */ 
      IF (*bitstream == G192_BIT1) 
      {
         is_transient = (Word16)1;
      } 
      ELSE 
      {
         is_transient = (Word16)0;
      }   
#if(WMOPS)
  move16();
#endif
      bitstream++;
      
      tbitstream = bitstream;
      pbitstream = &bitstream;
      /* decode spectrum */
      IF (is_transient) 
      {
         flvqdec(pbitstream, t_audio_q, audio_q_norm, bitalloc, d->num_bits_spectrum_transient, ynrm, is_transient);  
         nf_idx = (Word16)0;
#if(WMOPS)
  move16();
#endif
      }
      ELSE 
      {
         flvqdec(pbitstream, t_audio_q, audio_q_norm, bitalloc, d->num_bits_spectrum_stationary, ynrm, is_transient);               
         bits2idxn(bitstream, 2, &nf_idx);
         bitstream += 2;
      }

      FOR (i = FREQ_LENGTH; i < FRAME_LENGTH; i++) 
      {
         t_audio_q[i] = (Word32)0;
#if(WMOPS)
  move32();
#endif
      }

      /* spectrum fill */
      fill_spectrum(audio_q_norm, t_audio_q,  bitalloc, is_transient, ynrm, nf_idx);
      
      taudio_max = (Word32)0;
#if(WMOPS)
  move32();
#endif

      FOR (i=0; i < FRAME_LENGTH; i++) 
      {
         taudio_max = L_max(taudio_max,L_abs(t_audio_q[i]));   
      }

      norm_taudio = norm_l(taudio_max);
      norm_taudio = sub(norm_taudio,7);

      FOR (i=0; i < FRAME_LENGTH; i++) 
      {
         t_audio_q[i]= L_shl(t_audio_q[i],norm_taudio);   
#if(WMOPS)
  move32();
#endif
      }

      IF (is_transient) 
      {
         de_interleave_spectrum(t_audio_q);
      }

      /* save coefficients for re-use in case of frame errors */
      FOR (i=0; i < FRAME_LENGTH; i++) 
      {
         d->old_coeffs[i] = t_audio_q[i];
#if(WMOPS)
  move32();
#endif
      }
      d->old_norm_taudio = norm_taudio;
      d->old_is_transient = is_transient;
#if(WMOPS)
  move16();
  move16();
#endif
   }
   
   /* inverse transform */
   inverse_transform(t_audio_q, wtda_audio, is_transient);

   FOR (i=0; i < 2*FRAME_LENGTH; i++) 
   {
      wtda_audio[i]= L_shr(wtda_audio[i],norm_taudio);   
#if(WMOPS)
  move32();
#endif
   }


   window_ola(wtda_audio, out16, d->old_out);

}
