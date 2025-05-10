/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#ifndef _COMPLXOP_H
#define _COMPLXOP_H

#include "stl.h"


/*******************************************************************************
/* Local type definitions
/******************************************************************************/
typedef struct complex16_struct
{
    Word16 r;
    Word16 i;
} complex16;

typedef struct complex32_struct
{
    Word32 r;
    Word32 i;
} complex32;


void cmpy3216(complex32 var1, complex16 var2, complex32 *res);

void cmpy3216s(complex32 var1, Word16 var2, complex32 *res);

void cadd3232(complex32 var1, complex32 var2, complex32 *res);

void csub3232(complex32 var1, complex32 var2, complex32 *res);

void cmpy32j(complex32* v1);

#endif
