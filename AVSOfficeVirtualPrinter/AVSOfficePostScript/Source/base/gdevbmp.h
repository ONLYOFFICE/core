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
/* $Id: gdevbmp.h 8022 2007-06-05 22:23:38Z giles $ */
/* .BMP file format definitions and utility interfaces */

#ifndef gdevbmp_INCLUDED
#  define gdevbmp_INCLUDED

/* Define the default X and Y resolution. */
#define X_DPI 72
#define Y_DPI 72

/* Write the BMP file header.  This procedure is used for all formats. */
int write_bmp_header(gx_device_printer *pdev, FILE *file);

/* Write a BMP header for separated CMYK output. */
int write_bmp_separated_header(gx_device_printer *pdev, FILE *file);

/* 24-bit color mappers */
dev_proc_map_rgb_color(bmp_map_16m_rgb_color);
dev_proc_map_color_rgb(bmp_map_16m_color_rgb);

#endif				/* gdevbmp_INCLUDED */
