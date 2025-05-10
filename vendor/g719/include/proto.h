/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#ifndef _PROTO_H
#define _PROTO_H

#include "stl.h"
#include "cnst.h"
#include "state.h"

void encoder_init(
  CoderState *c,
  Word16 num_bits
);

void encode_frame(
  Word16 *audio, 
  Word16 num_bits,
  Word16 *BitStream, 
  CoderState *c
);

Word16 detect_transient(
  Word16 in[],
  CoderState *c
);

void wtda(
  Word16 new_audio[],
  Word32 wtda_audio[],
  Word32 old_wtda[]
);

void direct_transform(
  Word32 *in32,
  Word32 *out32,
  Word16 is_transient
 );

Word16 noise_adjust(
  Word16 *coeffs_norm,
  Word16 *bitalloc
);

void interleave_spectrum(
  Word32 *coefs
);

void flvqenc(
  Word16 **bitstream,
  Word32 *coefs,
  Word16 *coefs_norm,
  Word16 *R, 
  Word16 NumSpectumBits,
  Word16 total_bits,
  Word16 is_transient
);

void logqnorm(
  Word32 *x,
  Word16 *k,
  Word16 L,
  Word16 N,
  Word16 M
);

void reordernorm(
Word16 *ynrm,
Word16 *normqlg2,
Word16 *idxbuf,
Word16 *normbuf
);

void diffcod(
  Word16 *normqlg2,
  Word16 N,
  Word16 *y,
  Word16 *difidx
);

void normalizecoefs(
Word32 *coefs,
Word16 *ynrm,
Word16 N1,
Word16 N2,
Word16 L,
Word16 *coefs_norm
);

void bitallocsum(
Word16 *R,
Word16 nb_sfm,
Word16 *sum
);

void qcoefs(
  Word16 *coefs,
  Word16 *R,
  Word16 N1,
  Word16 N2,
  Word16 L,
  Word16 *y
);

void lvq1(
  Word16 *x,
  Word16 *k
);

void lvq2(
  Word16 *x,
  Word16 *k,
  Word16 r,
  Word16 R
);

void code2idx(
  Word16 *x,
  Word16 *k,
  Word16 r
);

Word16 huffcheck(
  Word16 *y,
  Word16 *R,
  Word16 N1,
  Word16 N2,
  Word16 L
);

Word16 packingc(
  Word16 *y,
  Word16 *R,
  Word16 *pbits,
  Word16 flag,
  Word16 N1,
  Word16 N2,
  Word16 L
);

void procnobitsbfm(
Word16 *coefs_norm,
Word16 *R,
Word16 *idx,
Word16 *ycof,
Word16 **ppbits,
Word16 nb_sfm,
Word16 diff
);

void procnf(
  Word16 *coefs,
  Word16 *y,
  Word16 *pbits,
  Word16 nb_vecs
);

void idx2bitsn(
  Word16 x,
  Word16 N,
  Word16 *y
);

void idx2bitsc(
  Word16 *x,
  Word16 N,
  Word16 L,
  Word16 *y
);

void decoder_init(
  DecoderState *d,
  Word16 num_bits
);

void decoder_reset_tables(
  DecoderState *d,
  Word16 num_bits
);

void decode_frame(
  Word16 bitstream[], 
  Word16 bfi, 
  Word16 out16[],
  DecoderState *d
);

void flvqdec(
  Word16 **bitstream,
  Word32 *coefsq,
  Word16 *coefsq_norm,
  Word16 *R, 
  Word16 NumSpectumBits,
  Word16 *ynrm,
  Word16 is_transient
);

void hdecnrm(
  Word16 *bitstream,
  Word16 N,
  Word16 *index
);

void hdec2blvq(
  Word16 *bitstream,
  Word16 N,
  Word16 *index
);

void hdec3blvq(
  Word16 *bitstream,
  Word16 N,
  Word16 *index
);

void hdec4blvq(
  Word16 *bitstream,
  Word16 N,
  Word16 *index
);

void hdec5blvq(
  Word16 *bitstream,
  Word16 N,
  Word16 *index
);

Word16 unpackc(
  Word16 *R,
  Word16 *pbits,
  Word16 flag,
  Word16 rv,
  Word16 N1,
  Word16 N2,
  Word16 L,
  Word16 *y
);

void dqcoefs(
  Word16 *y,
  Word16 *idxnrm,
  Word16 *R,
  Word16 N1,
  Word16 N2,
  Word16 L,
  Word32 *coefs,
  Word16 *coefs_norm
);

void dprocnobitsbfm(
Word16 *R,
Word16 *idx,
Word16 *ynrm,
Word16 *ycof,
Word16 **ppbits,
Word32 *coefsq,
Word16 *coefsq_norm,
Word16 nb_sfm,
Word16 diff
);

void dprocnf(
  Word16 *y,
  Word16 *pbits,
  Word16 idxnrm,
  Word16 nb_vecs,
  Word32 *coefs,
  Word16 *coefs_norm
);

void fill_spectrum(
  Word16 *coeff,
  Word32 *coeff_out,
  Word16 *R,
  Word16 is_transient,
  Word16 norm[],
  Word16 nf_idx
);

void de_interleave_spectrum(
  Word32 *coefs
);

void inverse_transform(
  Word32 *in_mdct,
  Word32 *out,
  Word16 is_transient
);

void window_ola(
  Word32 ImdctOut[],
  Word16 auOut[],
  Word32 OldauOut[]
);

void bits2idxn(
  Word16 *y,
  Word16 N,
  Word16 *x
);

void bits2idxc(
  Word16 *y,
  Word16 N,
  Word16 L,
  Word16 *x
);

void dct4_960(
  Word32 v[MLT960_LENGTH],
  Word32 coefs32[MLT960_LENGTH]
);

void dct4_240(
  Word32 v[MLT240_LENGTH],
  Word32 coefs32[MLT240_LENGTH]
);

void map_quant_weight(
  Word16 normqlg2[],
  Word16 wnorm[],
  Word16 is_transient
);

void recovernorm(
Word16 *idxbuf,
Word16 *ynrm,
Word16 *normqlg2
);

void reordvct(
  Word16 *y,
  Word16 N,
  Word16 *idx
);

void bitalloc(
  Word16 *y,
  Word16 *idx,
  Word16 sum,
  Word16 N,
  Word16 M,
  Word16 *r
);

void codesearch(
  Word16 *x,
  Word16 *C,
  Word16 R
);

void idx2code(
  Word16 *k,
  Word16 *y,
  Word16 R
);

#endif
