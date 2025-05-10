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
   fprintf(stderr,"ITU-T G.719 Encoder. Version %s\n", CODEC_VERSION);
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

   fprintf(stderr, "Usage:  %s [-q] -r rate -i pcmIn -o bsOut \n\n", cmd);
   fprintf(stderr, "  pcmIn              - File name input  PCM wave\n");
   fprintf(stderr, "  bsOut              - File name output bit stream\n");
   fprintf(stderr, "  rate               - Bit rate of codec in bit/s\n");
   fprintf(stderr, "  -q                 - Do not display frame counter\n");
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
                          Word32 *rate,
                          Word16 *quiet) 
{
   char *cmd;

   *rate = 0L;
   cmd = argv[0];   

   ARGBEGIN {
      default:
         fprintf(stderr, "\n Unknown option\n"); 
         usage(cmd);
      case 'r':       
         *rate = atoi(ARGF());
         break;
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
/*  Function  read_data                                                     */
/*  ~~~~~~~~~~~~~~~~~~~                                                     */
/*                                                                          */
/*  Read blocks of data from file                                           */
/*--------------------------------------------------------------------------*/
/*  FILE       *fp      (i)   input file                                    */
/*  Word16     data[]   (o)   data read                                     */
/*  Word16     size     (i)   size of data read                             */
/*--------------------------------------------------------------------------*/
static Word16 read_data(
                 FILE  *fp,
                 Word16 data[],
                 Word16 size
                 )
{
   Word16   i, n;
   Word16   data16[FRAME_LENGTH];

   n = (Word16)fread((void *)data16, sizeof(short), size, fp);

   FOR (i = 0; i < n; i++)  
   {
      data[i] = data16[i];    
#if(WMOPS)
      move16();
#endif
   }

   FOR (i = n; i < size; i++)  
   {
      data[i] = 0;      

#if(WMOPS)
      move16();
#endif
   }
   return n;
}

/*--------------------------------------------------------------------------*/
/*  Function  write_bitstream                                               */
/*  ~~~~~~~~~~~~~~~~~~~~~~~~~                                               */
/*                                                                          */
/*  Write bitstream to file                                                 */
/*--------------------------------------------------------------------------*/
/*  FILE       *fp          (i)   output file                               */
/*  Word16     bitstream[]  (o)   bitstream to write                        */
/*  Word16     num_bits     (o)   number of bits                            */
/*--------------------------------------------------------------------------*/
static void write_bitstream(FILE  *fp,   
                            Word16 bitstream[],
                            Word16 num_bits)
{   
   UWord16   G192_HEADER[2];


   G192_HEADER[0] = G192_SYNC_GOOD_FRAME;
   G192_HEADER[1] = (UWord16) num_bits;

#if(WMOPS)
   move16();
   move16();
#endif

   fwrite(G192_HEADER, sizeof(UWord16), 2, fp);
   fwrite(bitstream, sizeof(Word16), num_bits, fp);
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
int enc_main(int argc, char *argv[])
{

   FILE     *fp_in;
   FILE     *fp_out;
   Word32   frame;
   char     *in_filename  = "";
   char     *out_filename = "";
   Word32   rate;
   Word16   num_bits;
   Word16   quiet = FALSE;

   Word16   in[FRAME_LENGTH];
   Word16   bitstream[MAX_BITS_PER_FRAME];
   Word16   samples;
   CoderState		c;

   /*--------------------------------------------------------------------------*/
   /*  Print Banner and Copyright info                                         */
   /*--------------------------------------------------------------------------*/

   banner();

   /*--------------------------------------------------------------------------*/
   /*  Parse the command line                                                  */
   /*--------------------------------------------------------------------------*/

   parse_cmdline(argc, argv, &in_filename, &out_filename, &rate, &quiet);

   /*--------------------------------------------------------------------------*/
   /*  Open input 16 bits raw pcm files                                        */
   /*--------------------------------------------------------------------------*/

   fp_in = fopen(in_filename,"rb");

   if(fp_in == NULL) 
   {
      fprintf(stderr, "\n Error opening input file %s",in_filename);
      exit(EXIT_FAILURE);   
   }

   /*--------------------------------------------------------------------------*/
   /*  Check bitrate                                                           */
   /*--------------------------------------------------------------------------*/

   if((rate > 128000) || (rate < 32000)) 
   {
      fprintf(stderr,"\n Invalid bitrate");
      exit(EXIT_FAILURE);   
   }
   
   num_bits = (Word16)(rate/50);

   /*--------------------------------------------------------------------------*/
   /*  Open output G.192 bitstream file                                        */
   /*--------------------------------------------------------------------------*/

   fp_out = fopen(out_filename,"wb");
   
   if(fp_out == NULL) 
   {
      fprintf(stderr, "\n Error opening output raw file");
      exit(EXIT_FAILURE);   
   }
   
#if (WMOPS)
  Init_WMOPS_counter();
  Reset_WMOPS_counter();
  setFrameRate(48000, 960);
#endif

   
   /*--------------------------------------------------------------------------*/
   /*  Initialize encoder                                                      */
   /*--------------------------------------------------------------------------*/

   encoder_init(&c,num_bits);
   
   /*--------------------------------------------------------------------------*/
   /*  Loop over frames                                                        */
   /*--------------------------------------------------------------------------*/

   frame = 0;
   do 
   {
#if (WMOPS)
      fwc();
      Reset_WMOPS_counter();
#endif
      /* read a 20ms frame */
      samples = read_data(fp_in, in, FRAME_LENGTH);

      /* encode the frame */
      encode_frame(in, num_bits, bitstream, &c);

      /* write bitstream */
      write_bitstream(fp_out, bitstream, num_bits);

      /* print progress */
      if (!quiet)
         fprintf(stderr,"Rate: %5.2f kbps Processed: %d frames \r", ((float)num_bits*50.0)/1000.0f, frame); frame++;
   } while (samples == FRAME_LENGTH);
   fprintf(stderr, "\n");

#ifdef WMOPS
  fwc();
  if (!quiet)
  {
     printf("\nEncoder complexity\n");
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
