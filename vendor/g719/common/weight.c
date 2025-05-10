/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* © 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/
#include "stl.h"
#include "cnst.h"

/*--------------------------------------------------------------------------*/
/*  Function  sfm2mqb                                                       */
/*  ~~~~~~~~~~~~~~~~~~                                                      */
/*                                                                          */
/*  Map sub-vectors to pbands                                               */
/*--------------------------------------------------------------------------*/
/*  Word16    spe[]    (i)   sub-vectors                                    */
/*  Word16    spe2q[]  (o)   pbands                                         */
/*--------------------------------------------------------------------------*/
static void sfm2mqb(Word16 spe[], Word16 spe2q[])
{
   Word16 tmp,i;

   spe2q[0]  = add(spe[0], 3);
   spe2q[1]  = add(spe[1], 3);
   spe2q[2]  = add(spe[2], 3);
   spe2q[3]  = add(spe[3], 3);
   spe2q[4]  = add(spe[4], 3);
   spe2q[5]  = add(spe[5], 3);
   spe2q[6]  = add(spe[6], 3);
   spe2q[7]  = add(spe[7], 3);
   spe2q[8]  = add(spe[8], 3);
   spe2q[9]  = add(spe[9], 3);

   spe2q[10]  = add(shr(add(spe[10], spe[11]),1),4);
   spe2q[11]  = add(shr(add(spe[12], spe[13]),1),4);
   spe2q[12]  = add(shr(add(spe[14], spe[15]),1),4);

   spe2q[13]  = add(shr(add(spe[16], spe[17]),1),5);
   spe2q[14]  = add(shr(add(spe[18], spe[19]),1),5);

   tmp = 0;
#if(WMOPS)
  move16();
#endif
   FOR (i=20; i < 24; i++) 
   {
      tmp = add(tmp,spe[i]);
   }
   spe2q[15] = add(mult(tmp,8192),6);

   tmp = 0;
#if(WMOPS)
  move16();
#endif
   FOR (i=24; i < 27; i++) 
   {
      tmp = add(tmp,spe[i]);
   }
   spe2q[16] = add(mult(tmp,10923),6);

   tmp = 0;
#if(WMOPS)
  move16();
#endif
   FOR (i=27; i < 30; i++) 
   {
      tmp = add(tmp,spe[i]);
   }
   spe2q[17] = add(mult(tmp,10923),6);

   tmp = 0;
#if(WMOPS)
  move16();
#endif
   FOR (i=30; i < 35; i++) 
   {
      tmp = add(tmp,spe[i]);
   }
   spe2q[18] = add(mult(tmp,6553),7);

   tmp = 0;
#if(WMOPS)
  move16();
#endif
   FOR (i=35; i < 44; i++) {
      tmp = add(tmp,spe[i]);
   }
   spe2q[19] = add(mult(tmp,3641),8);

#if(WMOPS)
   for (i=0; i < NUM_MAP_BANDS; i++) 
   {
      move16();   
   }
#endif
}


/*--------------------------------------------------------------------------*/
/*  Function  mqb2sfm                                                       */
/*  ~~~~~~~~~~~~~~~~~~                                                      */
/*                                                                          */
/*  Map pbands to sub-vectors                                               */
/*--------------------------------------------------------------------------*/
/*  Word16    spe2q[]  (i)   pbands                                         */
/*  Word16    spe[]    (o)   sub-vectors                                    */
/*--------------------------------------------------------------------------*/
static void mqb2sfm(Word16 spe2q[],Word16 spe[])
{
   Word16 i;

   spe[0]  = spe2q[0];
   spe[1]  = spe2q[1];
   spe[2]  = spe2q[2];
   spe[3]  = spe2q[3];
   spe[4]  = spe2q[4];
   spe[5]  = spe2q[5];
   spe[6]  = spe2q[6];
   spe[7]  = spe2q[7];
   spe[8]  = spe2q[8];
   spe[9]  = spe2q[9];

   spe[10] = spe2q[10];
   spe[11] = spe2q[10];

   spe[12] = spe2q[11];
   spe[13] = spe2q[11];

   spe[14] = spe2q[12];
   spe[15] = spe2q[12];

   spe[16] = spe2q[13];
   spe[17] = spe2q[13];

   spe[18] = spe2q[14];
   spe[19] = spe2q[14];

   FOR (i=20; i < 24; i++) 
   {
      spe[i] = spe2q[15];
   }

   FOR (i=24; i < 27; i++) 
   {
      spe[i] = spe2q[16];
   }

   FOR (i=27; i < 30; i++) 
   {
      spe[i] = spe2q[17];
   }
  
   FOR (i=30; i < 35; i++) 
   {
      spe[i] = spe2q[18];
   }
  
   FOR (i=35; i < 44; i++)
   {
      spe[i] = spe2q[19];
   }
   

#if(WMOPS)
   for (i=0; i < NB_SFM; i++)
   {
      move16();   
   }
#endif
}

/*--------------------------------------------------------------------------*/
/*  Function  map_quant_weight                                              */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~~                                              */
/*                                                                          */
/*  Calculate the quantization weights                                      */
/*--------------------------------------------------------------------------*/
/*  Word16    normqlg2[]    (i)   quantized norms                           */
/*  Word16    wnorm[]       (o)   weighted norm                             */
/*  Word16    is_transient  (i)   transient flag                            */
/*--------------------------------------------------------------------------*/
void map_quant_weight(Word16 normqlg2[], Word16 wnorm[], Word16 is_transient)
{
   Word16 sfm;
   Word16 tmp16;
   Word16 spe2q[NUM_MAP_BANDS];
   Word16 spe[NB_SFM];

   Word16 spe2q_max;
   Word16 spe2q_min;
   Word16 norm_max;
   Word16 shift;
   Word16 sum;
   Word16 k;

   IF (is_transient) 
   {
      FOR (sfm = 0; sfm < NB_SFM; sfm+=4) 
      {
         sum = (Word16)0;
#if(WMOPS)
  move16();
#endif
         FOR (k=0; k < 4; k++) {
            sum = add(sum,normqlg2[sfm+k]);         
         }
         sum = shr(sum,2);

         FOR (k=0; k < 4; k++) 
         {
            spe[sfm +k] = sum;
#if(WMOPS)
  move16();
#endif
         }                  
      }
#if(WMOPS)
  move16();
#endif      
   }
   ELSE {
      FOR (sfm = 0; sfm < NB_SFM; sfm++) 
      {
         spe[sfm] = normqlg2[sfm];
#if(WMOPS)
  move16();
#endif
      }      
   }

   sfm2mqb(spe, spe2q);

   FOR (sfm = 0; sfm < NUM_MAP_BANDS; sfm++) 
   {
      spe2q[sfm] = sub(spe2q[sfm],10);
#if(WMOPS)
  move16();
#endif
   }

   FOR (sfm = 1; sfm < NUM_MAP_BANDS; sfm++)
   {    
      tmp16 = sub(spe2q[sfm-1], 4);             
      spe2q[sfm] = s_max(spe2q[sfm], tmp16);
#if(WMOPS)
  move16();
#endif
   }

   FOR (sfm = NUM_MAP_BANDS-2 ; sfm >= 0 ; sfm--)
   {
      tmp16 = sub(spe2q[sfm+1], 8);             
      spe2q[sfm] = s_max(spe2q[sfm], tmp16); 
#if(WMOPS)
  move16();
#endif
   }

   FOR (sfm = 0; sfm < NUM_MAP_BANDS ; sfm++)
   {
      spe2q[sfm] = s_max(spe2q[sfm], a[sfm]);
#if(WMOPS)
  move16();
#endif
   }   

   spe2q_max = MIN_16;
   spe2q_min = MAX_16;
#if(WMOPS)
  move16();
  move16();
#endif


   FOR (sfm = 0; sfm < NUM_MAP_BANDS ; sfm++)
   {
      spe2q[sfm] = sub(sfm_width[sfm],spe2q[sfm]);
      spe2q_max = s_max(spe2q[sfm] , spe2q_max);
      spe2q_min = s_min(spe2q[sfm] , spe2q_min);
#if(WMOPS)
  move16();
#endif
   }   

   FOR (sfm = 0; sfm < NUM_MAP_BANDS ; sfm++)
   {
      spe2q[sfm] = sub(spe2q[sfm], spe2q_min);
#if(WMOPS)
  move16();
#endif
   }   

   spe2q_max = sub(spe2q_max,spe2q_min);

   norm_max = norm_s(spe2q_max);
   
   shift = sub(norm_max,13);
   
   FOR (sfm = 0; sfm < NUM_MAP_BANDS ; sfm++)
   {
      spe2q[sfm] = shl(spe2q[sfm],shift);      
#if(WMOPS)
  move16();
#endif
   }   

   mqb2sfm(spe2q,spe);

   IF (is_transient) 
   {
      FOR (sfm = 0; sfm < NB_SFM; sfm+=4) 
      {
         sum = (Word16)0;
#if(WMOPS)
  move16();
#endif
         FOR (k=0; k < 4; k++) {
            sum = add(sum,spe[sfm+k]);         
         }
         sum = shr(sum,2);

         FOR (k=0; k < 4; k++) 
         {
            spe[sfm +k] = sum;
#if(WMOPS)
  move16();
#endif
         }                  
      }      
   }

   /* modify the norms for bit-allocation */
   FOR (sfm = 0; sfm < NB_SFM ; sfm++)
   {
      wnorm[sfm] = add(spe[sfm],normqlg2[sfm]);      
#if(WMOPS)
  move16();
#endif
   }

}
