/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "stl.h"
#include "cnst.h"
#include "proto.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  direct_transform                                              */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~~                                              */
/*                                                                          */
/*  Transformation of the signal to DCT domain                              */
/*--------------------------------------------------------------------------*/
/*  Word32      *in32         (i)  input signal                             */
/*  Word32      *out32        (o)  output transformation                    */
/*  Word16      is_transient  (i)  is transient                             */
/*--------------------------------------------------------------------------*/
void direct_transform(Word32 *in32,
                      Word32 *out32,
                      Word16 is_transient)
{     
   Word32 i;
   Word32 seg;

   Word16 *wh;
   Word16 *wl;
   Word16 *sh;
   Word16 *sl;
   Word32 tmp32;
   Word16 *iseg;
   Word32 *oseg;
   Word32 dctin32[FRAME_LENGTH];   
   Word16 in32_r16[FRAME_LENGTH];


   IF (is_transient) 
   {    
      FOR (i = 0; i < FRAME_LENGTH/2; i++) 
      {                 
         in32_r16[i]                = round(in32[FRAME_LENGTH-1-i]);          
         in32_r16[FRAME_LENGTH-1-i] = round(in32[i]);
#if(WMOPS)
  move16();
  move16();         
#endif
      }
      iseg = in32_r16 - MAX_SEGMENT_LENGTH / 4; 
      oseg = out32    +       0               ;      

      wh = (Word16 *) wscw16q15 + MAX_SEGMENT_LENGTH/4;
      wl = (Word16 *) wscw16q15 + MAX_SEGMENT_LENGTH/4 - 1;
      sh = iseg      + 3*MAX_SEGMENT_LENGTH/4;
      sl = iseg      + 3*MAX_SEGMENT_LENGTH/4 - 1;
      
      FOR (i = 0; i < MAX_SEGMENT_LENGTH/4; i++) 
      {
         tmp32      = L_mult0(*wl--,  *sl--);
         dctin32[i] = L_msu0(tmp32, *wh++,  *sh++);
#if(WMOPS)
  move32();
#endif
      }

      sl = iseg + MAX_SEGMENT_LENGTH/2 - 1;

      FOR (i = 0; i < MAX_SEGMENT_LENGTH/4; i++) 
      {
         dctin32[MAX_SEGMENT_LENGTH/4 + i] = L_mult0(-32768,*sl--);                       
#if(WMOPS)
  move32();
#endif
      }

      FOR (i = 0; i < MLT240_LENGTH; i++)
      {
         dctin32[i] = L_shr_r(dctin32[i], 7);
#if(WMOPS)
  move32();
#endif
      }
      dct4_240(dctin32, oseg);
      
      iseg = iseg + MAX_SEGMENT_LENGTH/2;                 
      oseg = oseg + MAX_SEGMENT_LENGTH/2;                 
                                                                       
      FOR (seg = 1 ; seg <  NUM_TIME_SWITCHING_BLOCKS-1; seg++) 
      {      
         wh = (Word16 *) wscw16q15 + MAX_SEGMENT_LENGTH/4;
         wl = (Word16 *) wscw16q15 + MAX_SEGMENT_LENGTH/4 - 1;
         sh = iseg      + 3*MAX_SEGMENT_LENGTH/4;
         sl = iseg      + 3*MAX_SEGMENT_LENGTH/4 - 1;

         FOR (i = 0; i < MAX_SEGMENT_LENGTH/4; i++) 
         {
            tmp32      = L_mult0(*wl--,  *sl--);
            dctin32[i] = L_msu0(tmp32, *wh++,  *sh++);               
#if(WMOPS)
  move32();
#endif
         }

         sh = iseg      + 0;
         sl = iseg      + MAX_SEGMENT_LENGTH/2 - 1;
         wh = (Word16 *) wscw16q15 + MAX_SEGMENT_LENGTH/2 - 1;
         wl = (Word16 *) wscw16q15 + 0;

         FOR (i = 0; i < MAX_SEGMENT_LENGTH/4; i++) 
         {
            tmp32                             = L_mult0(*wl++,*sl--);
            dctin32[MAX_SEGMENT_LENGTH/4 + i] = L_mac0(tmp32,*wh--,*sh++);             
#if(WMOPS)
  move32();
#endif
         }

         FOR (i = 0; i < MLT240_LENGTH; i++) 
         {          
            dctin32[i] = L_shr_r(dctin32[i], 7);
#if(WMOPS)
  move32();
#endif
         }
         dct4_240(dctin32, oseg);

         iseg = iseg + MAX_SEGMENT_LENGTH/2;              
         oseg = oseg + MAX_SEGMENT_LENGTH/2;              
      } 

      sh = iseg + 3*MAX_SEGMENT_LENGTH/4 - 1;

      FOR (i = 0; i < MAX_SEGMENT_LENGTH /4; i++) 
      {
         dctin32[i] = L_mult0(-32768 ,*sh--);
#if(WMOPS)
  move32();
#endif
      }

      sh = iseg      + 0;
      sl = iseg      + MAX_SEGMENT_LENGTH/2 - 1;
      wh = (Word16 *) wscw16q15 + MAX_SEGMENT_LENGTH/2 - 1;
      wl = (Word16 *) wscw16q15 + 0;

      FOR (i = 0; i < MAX_SEGMENT_LENGTH/4; i++) 
      {   
         tmp32                             = L_mult0(*wh--,*sh++);
         dctin32[MAX_SEGMENT_LENGTH/4 + i] = L_mac0(tmp32,*wl++,*sl--);      
#if(WMOPS)
  move32();
#endif
      }
  
      FOR (i = 0; i < MLT240_LENGTH; i++) 
      {
         dctin32[i] = L_shr_r(dctin32[i], 7);
#if(WMOPS)
  move32();
#endif
      }
      dct4_240(dctin32, oseg);

   }
   ELSE
   {
      FOR (i = 0; i < FRAME_LENGTH; i++) 
      {
         dctin32[i] = L_shr_r(in32[i], 9);
#if(WMOPS)
  move32();
#endif
      }
      dct4_960(dctin32, out32);
   }

}
