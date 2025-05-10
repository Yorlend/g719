/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* � 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#ifndef _ROM_H
#define _ROM_H

#include "typedef.h"
#include "cnst.h"
#include "complxop.h"

extern Word16 sin_win[480];
extern Word16 cos_win[480];

extern Word16 short_window[480];
extern Word16 window[1920];

extern const Word16 sfm_start[44] ;
extern const Word16 sfm_end[44];

extern Word32 indxPre[120];
extern Word32 indxPost[120];
extern Word16 fft120cnst[144];
extern complex16 ptwdf[480];
extern complex16 dct480_table_1[480];
extern complex16 dct480_table_2[480];
extern complex16 dct120_table_1[120];
extern complex16 dct120_table_2[120];

extern const Word16 wscw16q15[240];

extern Word16 dicnlg2[40];
extern Word16 moreshift[4];
extern Word16 inv_sbfsize[4];

extern Word16 sfmsize[44];

extern Word16 RV[10];
extern Word16 FacLVQ2Qv[10];
extern Word16 FacLVQ2Mask[10];
extern Word16 FacLVQ2HalfQv[10];

extern Word16 dic0[10][8];
extern Word16 dic1[10][8];
extern Word16 dic1Q8[10][8];
extern Word16 dic2[10];
extern Word16 dic3[256];
extern Word16 dic4[256][8];

extern Word16 huffnorm[32];
extern Word16 huffsizn[32];
extern Word16 huffcoef[60];
extern Word16 huffsizc[60];
extern Word16 huffoffset[6];

extern Word16 sfm_width[20];
extern Word16 a[20];

#endif
