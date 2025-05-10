/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#ifndef _CNST_H
#define _CNST_H

#include "typedef.h"

#define CODEC_VERSION   "1.0"

#define FRAME_LENGTH                960
#define MAX_SEGMENT_LENGTH          480
#define MAX_BITS_PER_FRAME          2560
#define TRUE                        1
#define FALSE                       0
#define NUM_TIME_SWITCHING_BLOCKS   4
#define NUM_MAP_BANDS               20
#define FREQ_LENGTH                 800
#define TRANSIENTDETECTOR_THRESHOLD 5461
#define  MLT960_LENGTH              960
#define  MLT960_LENGTH_DIV_2        480
#define  MLT960_LENGTH_MINUS_1      959
#define  MLT240_LENGTH              240
#define  MLT240_LENGTH_MINUS_1      239

#define  STOP_BAND                  (FREQ_LENGTH)
#define  STOP_BAND4                 (STOP_BAND/4)

#define  SFM_G1                     16
#define  SFM_G2                     8
#define  SFM_G1G2                   24
#define  SFM_G3                     12
#define  SFM_N                      36
#define  SFM_GX                     8
#define  NB_SFM                     44
#define  WID_G1                     8
#define  WID_G2                     16
#define  WID_G3                     24
#define  WID_GX                     32
#define  NUMC_G1                    128
#define  NUMC_G1G2                  256
#define  NUMC_N                     544
#define  NB_VECT1                   1
#define  NB_VECT2                   2
#define  NB_VECT3                   3
#define  NB_VECTX                   4

#define  NUMC_G23                   256
#define  NUMC_G1SUB                 32
#define  NUMC_G1G2SUB               64
#define  NUMC_G1G2G3SUB             136

#define  QBIT_MAX1                  5
#define  QBIT_MAX2                  9
#define  OFFSET                     4
#define  FCT_LVQ1                   3277
#define  FCT_LVQ2                   21845
#define  SHFT_OFST                  6
#define  FCT_DLVQ2                  24576
#define  N_LEADER1                  10

#define  FLAGS_BITS                 3
#define  NORM0_BITS                 5
#define  NORMI_BITS                 5
#define  NUMNRMIBITS                215

#define  NOALLGROUPS                0
#define  ALLGROUPS                  1

#define  NOHUFCODE                  0
#define  HUFCODE                    1
#define G192_SYNC_GOOD_FRAME        (UWord16) 0x6B21 
#define G192_SYNC_BAD_FRAME         (UWord16) 0x6B20 
#define G192_BIT0                   (UWord16) 0x007F 
#define G192_BIT1                   (UWord16) 0x0081 

#define  Qv1                        8

#define  THREN2POW                  1518500250L
#define  THRIDXOFST                 19
#define  INV2POWHALF                23170
#define  LVQ1SCALECW1L              14894
#define  LVQ1SCALECW2L              10532
#define  DLVQ1FCT                   29789
#define  DLVQ2FCT                   6144


#include "rom.h"
#endif
