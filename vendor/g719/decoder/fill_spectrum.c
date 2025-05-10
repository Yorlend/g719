/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include "cnst.h"
#include "state.h"
#include "rom.h"
#include "stl.h"
#include "proto.h"

/*--------------------------------------------------------------------------*/
/*  Function  fill_spectrum                                                 */
/*  ~~~~~~~~~~~~~~~~~~~~~~~                                                 */
/*                                                                          */
/*  Fill the spectrum which has been quantized with 0 bits                  */
/*--------------------------------------------------------------------------*/
/*  Word16    *coeff        (i)   normalized MLT coefficients               */
/*  Word32    *coeff_out    (o)   MLT coefficients                          */
/*  Word16    *R            (i)   number of bits per coefficient            */
/*  Word16    is_transient  (i)   transient flag                            */
/*  Word16    norm          (i)   quantization indices for norms            */
/*  Word16    nf_idx        (i)   noise fill index                          */
/*--------------------------------------------------------------------------*/
void fill_spectrum(Word16 *coeff, 
                   Word32 *coeff_out, 
                   Word16 *R, 
                   Word16 is_transient, 
                   Word16 norm[],
                   Word16 nf_idx)
{
   Word16 CodeBook[FREQ_LENGTH];
   Word16 *src, *dst, *end;
   Word16 cb_size, cb_pos;
   Word16 sfm, j;
   
   Word16 last_sfm;
   Word16 first_coeff;
   Word16 low_coeff;

   Word16 r,m,k,i;
   Word32 tmp;

   cb_size = 0;
#if(WMOPS)
  move16();
#endif


   FOR (sfm = 0; sfm < NB_SFM; sfm++)
   {
      IF (R[sfm] != 0)
      {
         FOR (j = sfm_start[sfm]; j < sfm_end[sfm]; j++)
         {
            CodeBook[cb_size] = coeff[j];
            cb_size = add(cb_size,1);
#if(WMOPS)
  move16();
#endif
         }
      }
   }

   last_sfm = sub(NB_SFM, 1);
   IF (is_transient == 0)
   {
      FOR (sfm = NB_SFM-1; sfm >= 0; sfm--) 
      {
         IF (R[sfm] != 0) 
         {
            last_sfm = sfm;
#if(WMOPS)
  move16();
#endif
            BREAK;
         }
      }
   }

   IF (cb_size != 0) 
   {
      cb_pos = (Word16)0;
#if(WMOPS)
  move16();
#endif

      FOR (sfm = 0; sfm <= last_sfm; sfm++)
      {
         IF (R[sfm] == 0)
         {
            FOR (j = sfm_start[sfm]; j < sfm_end[sfm]; j++)
            {
               coeff[j] = CodeBook[cb_pos];
#if(WMOPS)
  move16();
#endif
               cb_pos = add(cb_pos,1);

               if( sub(cb_pos, cb_size) >= 0)
               {
                  cb_pos = (Word16)0;
#if(WMOPS)
  move16();
#endif
               }
            }
         }
      }

      IF (is_transient == 0)
      {
         low_coeff = shr(sfm_end[last_sfm], 1);
         src       = coeff + sfm_end[last_sfm] - 1;

         first_coeff = sfm_end[last_sfm];
         dst = coeff + sfm_end[last_sfm];
         end = coeff + sfm_end[NB_SFM-1];

         WHILE(dst < end) 
         {
#if(WMOPS)
  test();
#endif
            WHILE(dst < end && src >= &coeff[low_coeff]) 
            {
               *dst++ = *src--;
#if(WMOPS)
  move16();
#endif
            }

            src++;

#if(WMOPS)
  test();
#endif
            WHILE(dst < end && src < &coeff[first_coeff]) 
            {
               *dst++ = *src++;
#if(WMOPS)
  move16();
#endif
            }
         }
      }
      
      FOR (sfm = 0; sfm <= last_sfm; sfm++)
      {
         IF (R[sfm] == 0)
         {
            FOR (j = sfm_start[sfm]; j < sfm_end[sfm]; j++)
            {
               coeff[j] = shr(coeff[j], nf_idx);
#if(WMOPS)
  move16();
#endif
            }
         }
      }
   }

   FOR (sfm = 0; sfm < NB_SFM; sfm++) 
   {
      r = s_and(norm[sfm],0x0001);
      m = shr(norm[sfm], 1);
      k = sub(2,m);
      IF (sub(r,1) == 0)
      {
         FOR (i = sfm_start[sfm]; i < sfm_end[sfm]; i++) 
         {
            tmp = L_mult0(coeff[i], 23170);
            coeff_out[i] = L_shl(tmp,k);
#if(WMOPS)
  move32();
#endif
         }    
      }
      ELSE
      {
         FOR (i = sfm_start[sfm]; i < sfm_end[sfm]; i++) 
         {  
            tmp = L_mult(coeff[i],16384);
            coeff_out[i] = L_shl(tmp,k);
#if(WMOPS)
  move32();
#endif
         }    
      }
   }

}
