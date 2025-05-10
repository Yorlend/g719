/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#ifndef _STATE_H
#define _STATE_H

#include <stdio.h>
#include "cnst.h"
#include "typedef.h"

typedef struct {
   Word32            old_wtda[FRAME_LENGTH/2];

   Word32            old_sf[NUM_MAP_BANDS];

   Word16            TransientHangOver;

   Word16            old_hpfilt_in;
   Word16            old_hpfilt_out;

   Word32            EnergyLT_hi;
   Word32            EnergyLT_lo;

   Word16            num_bits;
   Word16            num_bits_spectrum_stationary ;
   Word16            num_bits_spectrum_transient  ;
} CoderState;


typedef struct {
   Word32            old_out[FRAME_LENGTH];

   Word16            num_bits;
   Word16            num_bits_spectrum_stationary ;
   Word16            num_bits_spectrum_transient  ;

   Word32            old_coeffs[FRAME_LENGTH];
   Word16            old_is_transient;
   Word16            old_norm_taudio;
} DecoderState;

#endif
