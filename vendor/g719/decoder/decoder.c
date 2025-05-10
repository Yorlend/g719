/*--------------------------------------------------------------------------*/
/* ITU-T G.719 Source Code                                                  */
/* > Software Release 1.0a (2008-06)                                        */
/* � 2008 Ericsson AB. and Polycom Inc.                                     */
/* All rights reserved.                                                     */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "cnst.h"
#include "stl.h"
#include "count.h"
#include "proto.h"

#define	ARGBEGIN	for(argv++,argc--;\
   argv[0] && argv[0][0]=='-' && argv[0][1];\
   argc--, argv++) {\
   char *_args, *_argt;\
   char _argc;\
   _args = &argv[0][1];\
   if((_args[0]=='-') && (_args[1]==0)){\
   argc--; argv++; break;\
   }\
   _argc = 0;\
   while((_argc = *_args++))\
   switch(_argc)
#define	ARGEND		}
#define	ARGF()		(_argt=_args, _args="",\
   (*_argt? _argt: argv[1]? (argc--, *++argv): 0))
#define	ARGC()		_argc

/*--------------------------------------------------------------------------*/
/*  Function  banner                                                        */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Print banner                                                            */
/*--------------------------------------------------------------------------*/
static void banner(void)
{
   fprintf(stderr,"ITU-T G.719 Decoder. Version %s\n", CODEC_VERSION);
   fprintf(stderr,"Copyright (c) Ericsson AB and Polycom Inc. 2008\n");
   fprintf(stderr,"\n");
}

/*--------------------------------------------------------------------------*/
/*  Function  usage                                                         */
/*  ~~~~~~~~~~~~~~~~~                                                       */
/*                                                                          */
/*  Print usage instructions                                                */
/*--------------------------------------------------------------------------*/
/*  char  *cmd   (i) command name                                           */
/*--------------------------------------------------------------------------*/
static void usage(char *cmd)
{

   fprintf(stderr, "Usage:  %s [-q] -i bsIn -o pcmOut \n\n", cmd);
   fprintf(stderr, "  bsIn                - Input G.192 bitstream file\n");
   fprintf(stderr, "  pcmOut              - File name output raw file\n");
   fprintf(stderr, "  -q                  - Do not display frame counter\n");
   fprintf(stderr, "---------------------------------------------------\n");


   exit(EXIT_FAILURE);
}

/*--------------------------------------------------------------------------*/
/*  Function  parse_cmdline                                                 */
/*  ~~~~~~~~~~~~~~~~~~~~~~~                                                 */
/*                                                                          */
/*  Parse command line arguments                                            */
/*--------------------------------------------------------------------------*/
/*  int     argc            (i)   argument count                            */
/*  char    *argv[]         (i)   argument vector                           */
/*  char    **in_filename   (o)   input filname                             */
/*  char    **out_filename  (o)   output filename                           */
/*  Word32  rate            (o)   rate of codec                             */
/*--------------------------------------------------------------------------*/
static void parse_cmdline(int    argc,
                          char   *argv[],
                          char   **in_filename,
                          char   **out_filename,
                          Word16 *quiet)
{
   char *cmd;

   cmd = argv[0];

   ARGBEGIN {
      default:
         fprintf(stderr, "\n Unknown option\n");
         usage(cmd);
      case 'i':   
         *in_filename = ARGF();
         break;
      case 'o':   
         *out_filename = ARGF();
         break;
      case 'q':
         *quiet = TRUE;
         break;
      case 'h':
         usage(cmd);
         break;     
   }
   ARGEND  
}

/*--------------------------------------------------------------------------*/
/*  Function  write_data                                                    */
/*  ~~~~~~~~~~~~~~~~~~~~                                                    */
/*                                                                          */
/*  Write blocks of data to file                                            */
/*--------------------------------------------------------------------------*/
/*  FILE       *fp      (i)   output file                                   */
/*  Word16     data[]   (i)   data to write                                 */
/*  Word16     size     (i)   size of data read                             */
/*--------------------------------------------------------------------------*/
static void write_data(FILE  *fp,       
                         Word16 data[],   
                         Word16 size)
{
   (Word16)fwrite((void *)data, sizeof(Word16), size, fp);   
}

/*--------------------------------------------------------------------------*/
/*  Function  read_bitstream                                                */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~                                               */
/*                                                                          */
/*  Read bitstream from file                                                */
/*--------------------------------------------------------------------------*/
/*  FILE       *fp          (i)   input file                                */
/*  Word16     bitstream[]  (o)   bitstream to read                         */
/*  Word16     *num_bits    (i)   number of bits                            */
/*  Word16     *bfi         (o)   bad frame indicator                       */
/*--------------------------------------------------------------------------*/
/*  Word16     return       (o)   result of read                            */
/*--------------------------------------------------------------------------*/
static Word16 read_bitstream(FILE  *fp,   
                             Word16 bitstream[],
                             Word16 *num_bits,
                             Word16 *bfi)
{   
   UWord16   G192_SYNC_WORD;   
   Word16    result;


#if(WMOPS)
   move16();
   move16();
#endif

   if(fread(&G192_SYNC_WORD, sizeof(UWord16), 1, fp) != 1) 
   {
      result = 0;
   } 
   else {
      if((G192_SYNC_WORD != G192_SYNC_GOOD_FRAME) && (G192_SYNC_WORD != G192_SYNC_BAD_FRAME)) 
      {
         fprintf(stderr, "\n Invalid bitstream. Wrong G192_SYNC_WORD ");
         exit(EXIT_FAILURE);      
      } 
      else {
         if(G192_SYNC_WORD == G192_SYNC_BAD_FRAME) {
            *bfi = 1;
         } 
         else {
            *bfi = 0;         
         }

         if( fread(num_bits, sizeof(Word16), 1, fp) != 1) {
            fprintf(stderr, "\n Premature end of file, cannot read frame length  ");
            exit(EXIT_FAILURE);      
         }

         if(*num_bits > MAX_BITS_PER_FRAME) {
            fprintf(stderr, "\n Frame is too large  ");
            exit(EXIT_FAILURE);    
         }

         if( fread(bitstream, sizeof(Word16), *num_bits, fp) != (UWord16)*num_bits) 
         {
            fprintf(stderr, "\n Premature end of file, cannot read frame");
            exit(EXIT_FAILURE);      
         }
      }
      result = 1;
   }

   return(result);
}

/*--------------------------------------------------------------------------*/
/*  Function  main                                                          */
/*  ~~~~~~~~~~~~~~                                                          */
/*                                                                          */
/*  Main function                                                           */
/*--------------------------------------------------------------------------*/
/*  int      argc     (i)   argument count                                  */
/*  char     *argv[]  (i)   argument vector                                 */
/*--------------------------------------------------------------------------*/
int dec_main(int argc, char *argv[])
{
   FILE     *fp_in;
   FILE     *fp_out;

   Word32   frame;
   char     *in_filename  = "";
   char     *out_filename = "";
   Word16   quiet = FALSE;

   Word16   num_bits;
   Word16   out[FRAME_LENGTH];   
   Word16   bitstream[MAX_BITS_PER_FRAME];
   Word16   bfi;
   DecoderState d;

   /*--------------------------------------------------------------------------*/
   /*  Print Banner and Copyright info                                         */
   /*--------------------------------------------------------------------------*/
   banner();

   /*--------------------------------------------------------------------------*/
   /*  Parse the command line                                                  */
   /*--------------------------------------------------------------------------*/
   parse_cmdline(argc, argv, &in_filename, &out_filename, &quiet);

   /*--------------------------------------------------------------------------*/
   /*  Open input G.192 bitstream file                                         */
   /*--------------------------------------------------------------------------*/

   fp_in = fopen(in_filename,"rb");

   if(fp_in == NULL) 
   {
      fprintf(stderr, "\n Error opening input bitstream file %s", in_filename);
      exit(EXIT_FAILURE);   
   }
   
   /*--------------------------------------------------------------------------*/
   /*  Open output 16 bits raw pcm files                                       */
   /*--------------------------------------------------------------------------*/

   fp_out = fopen(out_filename,"wb");

   if(fp_out == NULL) 
   {
      fprintf(stderr, "\n Error opening output file %s", out_filename);
      exit(EXIT_FAILURE);   
   }

   /*--------------------------------------------------------------------------*/
   /*  Initialize decoder (assume 32kbps at initialization)                    */
   /*--------------------------------------------------------------------------*/
   num_bits = 640;
   decoder_init(&d, num_bits);

   /*--------------------------------------------------------------------------*/
   /*  Loop over frames                                                        */
   /*--------------------------------------------------------------------------*/
   frame = 0;

#if (WMOPS)
   Init_WMOPS_counter();
   Reset_WMOPS_counter();
   setFrameRate(48000, 960);
#endif   

   while( read_bitstream(fp_in, bitstream, &num_bits, &bfi) )
   {
#if (WMOPS)
      fwc();
      Reset_WMOPS_counter();
#endif
      frame++;

      /* check if bad frame */
      IF( !bfi) 
      {
         /* good frame, set correct number of bits */
         decoder_reset_tables(&d, num_bits);
      }      

      /* decode frame */
      decode_frame(bitstream, bfi, out, &d);

      /*   ------!!!! REMOVE THIS LINE IF CORRECT DELAY IS DESIRED !!!!-----  */
      /* skip writing the first frame to get encoder and decoder synchronized */
      
      if(frame != 1)
      {
         write_data(fp_out, out, FRAME_LENGTH);
      }
      if (!quiet)
         fprintf(stderr,"Rate: %5.2f kbps Processed: %d frames \r", ((float)num_bits*50.0)/1000.0f, frame);
   }
   fprintf(stderr, "\n");

#ifdef WMOPS
   fwc();
   if (!quiet)
   {
      printf("\n\nDecoder complexity\n");
      WMOPS_output(0);
      printf("\n");
   }
#endif

   /*--------------------------------------------------------------------------*/
   /*  End - close files and exit                                              */
   /*--------------------------------------------------------------------------*/

   fclose(fp_in);
   fclose(fp_out);

   exit(EXIT_SUCCESS);
}
