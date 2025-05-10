/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "typedef.h"
#include "cnst.h"
#include "rom.h"

/*--------------------------------------------------------------------------*/
/*  Function  find_last_band                                                */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~                                                */
/*                                                                          */
/*  Find the last band which has bits allocated                             */
/*--------------------------------------------------------------------------*/
/*  Word16    *bitalloc  (i)   bit allocation                               */
/*--------------------------------------------------------------------------*/
/*  Word16    return     (o)   index of last band                           */
/*--------------------------------------------------------------------------*/
Word16 find_last_band(Word16 *bitalloc)
{
   Word16 last_sfm, sfm;

   last_sfm = NB_SFM;
#if(WMOPS)
  move16();
#endif
   
   FOR (sfm = NB_SFM-1; sfm >= 0; sfm--) 
   {
      IF (bitalloc[sfm] != 0) 
      {
         last_sfm = sfm;
#if(WMOPS)
  move16();
#endif
         BREAK;
      }
   }

   return last_sfm;
}

/*--------------------------------------------------------------------------*/
/*  Function  noise_adjust                                                  */
/*  ~~~~~~~~~~~~~~~~~~~~~~                                                  */
/*                                                                          */
/*  Calculate attenuation                                                   */
/*--------------------------------------------------------------------------*/
/*  Word16    *coeffs_norm  (i)   normalized coefficients                   */
/*  Word16    *bitalloc     (i)   bit allocation                            */
/*--------------------------------------------------------------------------*/
/*  Word16    return        (o)   index of noise attenuation                */
/*--------------------------------------------------------------------------*/
Word16 noise_adjust(Word16 *coeffs_norm,                    
                     Word16 *bitalloc)
{
   Word16 last_sfm, sfm, bin, diff, nf_idx;
   Word16 E, num_coeffs, inv_num_coeffs;


   last_sfm = find_last_band(bitalloc);

   E = (Word16)0;
   num_coeffs = (Word16)0;
#if(WMOPS)
  move16();
  move16();
#endif

   FOR (sfm = 0; sfm < last_sfm; sfm++)
   {
      IF (bitalloc[sfm] == 0)
      {
         FOR (bin = sfm_start[sfm]; bin < sfm_end[sfm]; bin++)
         {
            IF (coeffs_norm[bin] == 0) 
            {
               E = sub(E, 1);
            }
            ELSE 
            {
               E = add(E, sub(15,norm_s(coeffs_norm[bin])));
            }
                        
            num_coeffs = add(num_coeffs,1);
         }

      }
   }      

   IF (num_coeffs != 0) 
   {      
      inv_num_coeffs = div_s(1,num_coeffs);
      E = mult(E,inv_num_coeffs);
   }
   ELSE
   {
      E = (Word16)0;      
#if(WMOPS)
  move16();
#endif
   }

   diff = sub(7,E);

   nf_idx = s_max(diff,0);
   nf_idx = s_min(diff,3);


   return nf_idx;
}
