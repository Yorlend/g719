/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "stl.h"
#include "cnst.h"
#include "rom.h"
#include "proto.h"

/*--------------------------------------------------------------------------*/
/*  Function  itda                                                          */
/*  ~~~~~~~~~~~~~~                                                          */
/*                                                                          */
/*  Inverse time domain aliasing                                            */
/*--------------------------------------------------------------------------*/
/*  Word32    *in    (i)   input vector                                     */
/*  Word32    *out   (o)   output vector                                    */
/*--------------------------------------------------------------------------*/
void itda(Word32 *in, Word32 *out)
{
   Word16 i;

   FOR (i = 0; i < MLT960_LENGTH_DIV_2; i++)
   {
      out[i]                         = in[MLT960_LENGTH_DIV_2 + i];
      out[MLT960_LENGTH_DIV_2 + i]   = L_sub(0, in[MLT960_LENGTH - 1 - i]);
      out[MLT960_LENGTH + i]         = L_sub(0, in[MLT960_LENGTH_DIV_2 - 1 - i]);
      out[3*MLT960_LENGTH_DIV_2 + i] = L_sub(0, in[i]);
#if(WMOPS)
  move32();
  move32();
  move32();
  move32();
#endif
   }

}

/*--------------------------------------------------------------------------*/
/*  Function  imdct_short                                                   */
/*  ~~~~~~~~~~~~~~~~~~~~~                                                   */
/*                                                                          */
/*  Inverse MDCT for short frames                                           */
/*--------------------------------------------------------------------------*/
/*  Word32    *in    (i)   input vector                                     */
/*  Word32    *out   (o)   output vector                                    */
/*--------------------------------------------------------------------------*/
void imdct_short(Word32 *in, Word32 *out)
{
   Word32 alias[MAX_SEGMENT_LENGTH];
   Word16 i;

   dct4_240(in, alias);

   FOR (i = 0; i < MAX_SEGMENT_LENGTH/4; i++)
   {
      out[i]                           = alias[MAX_SEGMENT_LENGTH/4 + i];
      out[MAX_SEGMENT_LENGTH/4 + i]    = L_sub(0, alias[MAX_SEGMENT_LENGTH/2 - 1 - i]);
      out[MAX_SEGMENT_LENGTH/2 + i]    = L_sub(0, alias[MAX_SEGMENT_LENGTH/4 - 1 - i]);
      out[3*MAX_SEGMENT_LENGTH/4 + i]  = L_sub(0, alias[i]);
#if(WMOPS)
  move32();
  move32();
  move32();
  move32();
#endif
   }

}

/*--------------------------------------------------------------------------*/
/*  Function  inverse_transform                                             */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~~~                                             */
/*                                                                          */
/*  Inverse transform from the DCT domain to time domain                    */
/*--------------------------------------------------------------------------*/
/*  Word32    *in_mdct       (i)   input MDCT vector                        */
/*  Word32    *out           (o)   output vector                            */
/*  Word16    *is_transient  (o)   transient flag                           */
/*--------------------------------------------------------------------------*/
void inverse_transform(Word32 *in_mdct, Word32 *out, Word16 is_transient)
{
   Word32 out_alias[FRAME_LENGTH];
   Word32 alias[MAX_SEGMENT_LENGTH];
   Word32 *in_segment;
   Word32 *out_segment;
   Word32 tmp;

   Word16 ta, seg, i;

   IF (is_transient)  
   {   
      FOR (ta = 0; ta < FRAME_LENGTH; ta++) 
      {
         out_alias[ta] = (Word32)0;
#if(WMOPS)
  move32();
#endif
      }

      out_segment = out_alias - MAX_SEGMENT_LENGTH / 4;
      in_segment  = in_mdct   +       0          ;
      
      
      imdct_short(in_segment, alias);

      FOR (ta = MAX_SEGMENT_LENGTH /4; ta < MAX_SEGMENT_LENGTH /2; ta++) 
      {
         out_segment[ta] = alias[ta];
#if(WMOPS)
  move32();
#endif
      }

      FOR (ta = MAX_SEGMENT_LENGTH /2; ta < MAX_SEGMENT_LENGTH; ta++) 
      {
         out_segment[ta] = L_mls(alias[ta], short_window[ta]);
#if(WMOPS)
  move32();
#endif
      }
                  
      out_segment = out_segment + MAX_SEGMENT_LENGTH/2;
      in_segment  = in_segment  + MAX_SEGMENT_LENGTH/2;

      FOR (seg = 1 ; seg <  NUM_TIME_SWITCHING_BLOCKS-1; seg++) 
      {
         imdct_short(in_segment, alias);

         FOR (ta = 0; ta < MAX_SEGMENT_LENGTH; ta++) 
         {
            out_segment[ta] = L_add(out_segment[ta], L_mls(alias[ta], short_window[ta]));
#if(WMOPS)
  move32();
#endif
         }

         in_segment  = in_segment  + MAX_SEGMENT_LENGTH/2;
         out_segment = out_segment + MAX_SEGMENT_LENGTH/2;
      } 

      imdct_short(in_segment, alias);

      FOR (ta = 0; ta < MAX_SEGMENT_LENGTH /2; ta++) 
      {
         out_segment[ta] = L_add(out_segment[ta], L_mls(alias[ta], short_window[ta]));
#if(WMOPS)
  move32();
#endif
      }

      FOR (ta = MAX_SEGMENT_LENGTH /2; ta < MAX_SEGMENT_LENGTH /4+MAX_SEGMENT_LENGTH /2; ta++) 
      {
         out_segment[ta] = alias[ta];
#if(WMOPS)
  move32();
#endif
      }

      FOR (ta = 0; ta < FRAME_LENGTH/2; ta++) 
      {
         tmp          = out_alias[ta];
         out_alias[ta] = out_alias[FRAME_LENGTH-1-ta];
         out_alias[FRAME_LENGTH-1-ta] = tmp;
#if(WMOPS)
  move32();
  move32();
  move32();
#endif
      }
   }
   ELSE
   {
      dct4_960(in_mdct, out_alias);
      FOR (i = 0; i < FRAME_LENGTH; i++)
      {
         out_alias[i] = L_shr(out_alias[i], 1);
#if(WMOPS)
  move32();
#endif
      }
   }
   
   itda(out_alias, out);

}
