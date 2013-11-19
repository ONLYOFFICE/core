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

/* $Id: gdevstc1.c 8022 2007-06-05 22:23:38Z giles $*/
/* Epson Stylus-Color Printer-Driver */

/***
     This file holds the sample-implementation of a monochrome-algorithm for
     the stcolor-driver. It is available via

          gs -sDEVICE=stcolor -sDithering=gsmono ...

     Actually this is no dithering-algorithm, it lets ghostscript do the job.
     This achieved, by requesting BYTE-Values between 0 and 1 to be delivered,
     which causes a depth of 1-Bit by default.

 ***/

/*
 * gdevstc.h holds all the includes and the driver-specific definitions, so
 * it is the only include you need. To add a new algorthim, STC_MODI in
 * gdevstc.h should be extended. (see the instructions there)
 */

#include "gdevstc.h"

/*
 * the routine required.
 */

/*ARGSUSED*/
int 
stc_gsmono(stcolor_device *sdev,int npixel,byte *in,byte *buf,byte *out) 
{

/*
 * There are basically 3 Types of calls:
 * npixel < 0    => initialize buf, if this is required
 *                  (happens only if requested)
 * npixel > 0    => process next scanline, if the flag STC_WHITE is set, then
 *                  in == NULL signals, that the basic-driver has decided
 *                  that this scanline is white. (Useful for really dithering
 *                  drivers)
 */

/* ============================================================= */
   if(npixel > 0) {  /* npixel >  0 -> scanline-processing       */
/* ============================================================= */

/*    -----------------------------------------------*/
      if(in != NULL) { /* normal processing          */
/*    -----------------------------------------------*/

         memcpy(out,in,npixel); /* really simple algorithm */

/*    -----------------------------------------------*/
      } else {                  /* skip-notification */
/*    -----------------------------------------------*/

         /* An algorithm may use the output-line as a buffer.
            So it might need to be cleared on white-lines.
         */

         memset(out,0,npixel);

/*    -----------------------------------------------*/
      }                             /* normal / skip */
/*    -----------------------------------------------*/

/* ============================================================= */
   } else {          /* npixel <= 0 -> initialisation            */
/* ============================================================= */
/*
 *    the optional buffer is already allocated by the basic-driver, here
 *    you just need to fill it, for instance, set it all to zeros:
 */
     int buf_size;

/*
 * compute the size of the buffer, according to the requested values
 * the buffer consists of a constant part, e.g. related to the number
 * of color-components, and a number of arrays, which are multiples of
 * the size of a scanline times the number of components.
 * additionally, the size of the scanlines may be expanded by one to the
 * right and to the left.
 */
     buf_size = 
           sdev->stc.dither->bufadd              /* scanline-independend size */
             + (-npixel)                                     /* pixels */
               * (sdev->stc.dither->flags/STC_SCAN)          /* * scanlines */
               * sdev->color_info.num_components;            /* * comp */

     if(buf_size > 0) { /* we obviously have a buffer */
        memset(buf,0,buf_size * sdev->stc.alg_item);
     }                  /* we obviously have a buffer */

/*
 * Usually one should check parameters upon initializaon
 */
     if(sdev->color_info.num_components         !=        1) return -1;

     if((sdev->stc.dither->flags & STC_TYPE)    != STC_BYTE) return -2;

/*
 * must neither have STC_DIRECT nor STC_WHITE
 */
      if((sdev->stc.dither->flags & STC_DIRECT) !=        0) return -3;

   } /* scanline-processing or initialisation */

   return 0; /* negative values are error-codes, that abort printing */
}
