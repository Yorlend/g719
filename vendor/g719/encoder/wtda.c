/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "cnst.h"
#include "stl.h"
 
/*--------------------------------------------------------------------------*/
/*  Function  wtda                                                          */
/*  ~~~~~~~~~~~~~~                                                          */
/*                                                                          */
/*  Windowing and Time Domain Aliasing                                      */
/*--------------------------------------------------------------------------*/
/*  Word16      new_audio[]   (i)    input audio                            */
/*  Word16      wtda_audio[]  (o)    windowed audio                         */
/*  Word16      old_wtda[]    (i/o)  windowed audio from previous frame     */
/*--------------------------------------------------------------------------*/
void wtda(Word16 new_audio[], Word32 wtda_audio[], Word32 old_wtda[])
{
   Word16 i;   
   Word16 *x1,*x2;
   Word32 *y1,*y2;
   Word16 *c ,*s;
   Word32 tmp32;

   FOR (i = 0; i < FRAME_LENGTH / 2 ; i++) 
   {
      wtda_audio[FRAME_LENGTH/2 + i] =  old_wtda[i];                        
#if(WMOPS)
  move32();
#endif
   }   

   y1 = wtda_audio;
   y2 = old_wtda + FRAME_LENGTH / 2 - 1;

   x1 = new_audio + FRAME_LENGTH/2;
   x2 = new_audio + FRAME_LENGTH / 2 - 1;

   c =  cos_win;
   s =  sin_win;

   FOR (i = 0; i < FRAME_LENGTH / 2 ; i++) 
   {

      tmp32    =  L_mult0(*c, *x2); 
      *y2--    =  L_mac0(tmp32, *s, *x1);

      tmp32    =  L_mult0(*s++, *x2--);
      *y1++    =  L_msu0(tmp32, *c++, *x1++);            
#if(WMOPS)
  move32();
  move32();
#endif
   }      
}
