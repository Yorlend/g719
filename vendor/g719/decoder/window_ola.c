/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "cnst.h"

/*--------------------------------------------------------------------------*/
/*  Function  window_ola                                                    */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Windowing, Overlap and Add                                              */
/*--------------------------------------------------------------------------*/
/*  Word32    ImdctOut[]  (i)   input                                       */
/*  Word16    auOut[]     (o)   output audio                                */
/*  Word32    OldauOut[]  (i/o) audio from previous frame                   */
/*--------------------------------------------------------------------------*/
void window_ola(Word32 ImdctOut[], Word16 auOut[], Word32 OldauOut[])
{
    Word16 i;
    Word32 ImdctOutWin[2*FRAME_LENGTH];
   
    FOR (i=0 ; i < 2*FRAME_LENGTH ; i++)
    {
        ImdctOutWin[i] = L_mls(ImdctOut[i], window[i]);
#if(WMOPS)
  move32();
#endif
    }

    FOR (i=0 ; i < FRAME_LENGTH ; i++)
    {
        auOut[i] = round(L_shl(L_add(ImdctOutWin[i], OldauOut[i]), 7));
#if(WMOPS)
  move16();
#endif
    }

    FOR (i=0 ; i < FRAME_LENGTH ; i++)
    {
        OldauOut[i] = ImdctOutWin[i+FRAME_LENGTH];
#if(WMOPS)
  move32();
#endif
    }
}
