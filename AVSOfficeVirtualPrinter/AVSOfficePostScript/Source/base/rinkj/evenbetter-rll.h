/* Copyright (C) 2001-2006 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/*$Id: evenbetter-rll.h 8022 2007-06-05 22:23:38Z giles $ */
/* Header file for testbed implementation of Even Better Screening. */

#ifdef __cplusplus
extern "C" {
#endif

/* Definitions for source pixel format.
 * Reasonable settings are as follows:
 * 8-bit: unsigned char ET_SrcPixel; ET_SRC_MAX = 255
 * 12-bit 0..4095: unsigned short (or int) ET_SrcPixel; ET_SRC_MAX = 4095
 * 12-bit 0..4096: unsigned short (or int) ET_SrcPixel; ET_SRC_MAX = 4096
 * 16-bit 0..65535: unsigned short (or int) ET_SrcPixel; ET_SRC_MAX = 65535
 * 16-bit 0..65536: unsigned int ET_SrcPixel; ET_SRC_MAX = 65536
 */
typedef unsigned char ET_SrcPixel;
#define ET_SRC_MAX 255

/* Simple runlength code */
typedef struct {
  unsigned short length;
  ET_SrcPixel value;
} ET_Rll;

typedef unsigned char uchar;

/* To use the file dump capability:

   Open a file as with: fopen ("dumpfile", "wb");
   Put the resulting FILE * pointer in params->dump_file.
   Set params->dump_level to the desired level. EB_DUMP_ALL dumps all
   inputs and outputs. Other values will lead to much smaller files,
   but may not be as insightful.

   If no dump file is desired, set params->dump_file to NULL.
*/

typedef enum {
  EB_DUMP_MINIMAL,
  EB_DUMP_PARAMS,
  EB_DUMP_LUTS,
  EB_DUMP_INPUT,
  EB_DUMP_ALL
} EbDumpLevel;

typedef struct {
  int source_width;
  int dest_width;
  int n_planes;
  int levels; /* Number of levels on output, <= 256 */
  int **luts;
  double rbscale; /* Should be 0.95 for 1:1, 1.8 for 2:1 */
  int aspect;
  int *strengths;
  int rand_scale; /* 0 is default */
  int even_c1_scale; /* 0 is default */
  int do_shadows;
  int dotsize;
  FILE *dump_file;
  EbDumpLevel dump_level;
  double gamma;   /* 0 to enable luts, other values are 1.0, 1.8 and 2.0 */
} EvenBetterParams;

typedef struct _EvenBetterCtx EvenBetterCtx;

EvenBetterCtx *
even_better_new (const EvenBetterParams *params);

void
even_better_line_rll (EvenBetterCtx *ctx, uchar **dest,
		      const ET_Rll *const *src);

void
even_better_line (EvenBetterCtx *ctx, uchar **dest,
		  const ET_SrcPixel *const *src);

void
even_better_free (EvenBetterCtx *ctx);

void *
eb_malloc_aligned(int size, int align);

void
eb_free_aligned(void *p);

#ifdef __cplusplus
}
#endif
