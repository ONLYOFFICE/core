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

/* $Id: gsdllwin.h 8022 2007-06-05 22:23:38Z giles $ */
/* gsdll extension for Microsoft Windows platforms */

#ifndef gsdllwin_INCLUDED
#  define gsdllwin_INCLUDED

/* DLL exported functions */
/* for load time dynamic linking */
GSDLLEXPORT HGLOBAL GSDLLAPI gsdll_copy_dib(unsigned char * device);
GSDLLEXPORT HPALETTE GSDLLAPI gsdll_copy_palette(unsigned char * device);
GSDLLEXPORT void GSDLLAPI gsdll_draw(unsigned char * device, HDC hdc, LPRECT dest,
			 LPRECT src);
GSDLLEXPORT int GSDLLAPI gsdll_get_bitmap_row(unsigned char *device,
				  LPBITMAPINFOHEADER pbmih,
				  LPRGBQUAD prgbquad, LPBYTE * ppbyte,
				  unsigned int row);

/* Function pointer typedefs */
/* for run time dynamic linking */
typedef HGLOBAL (GSDLLAPI * PFN_gsdll_copy_dib)(unsigned char *);
typedef HPALETTE (GSDLLAPI * PFN_gsdll_copy_palette)(unsigned char *);
typedef void (GSDLLAPI * PFN_gsdll_draw) (unsigned char *, HDC, LPRECT,
					  LPRECT);
typedef int (GSDLLAPI * PFN_gsdll_get_bitmap_row)
     (unsigned char *device, LPBITMAPINFOHEADER pbmih, LPRGBQUAD prgbquad,
      LPBYTE * ppbyte, unsigned int row);

#endif /* gsdllwin_INCLUDED */
