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

/* $Id: zht2.h 9043 2008-08-28 22:48:19Z giles $ */
/* Level 2 sethalftone support */

#ifndef zht2_INCLUDED
#  define zht2_INCLUDED

#include "gscspace.h"            /* for gs_separation_name */

/*
 * This routine translates a gs_separation_name value into a character string
 * pointer and a string length.
 */
int gs_get_colorname_string(const gs_memory_t *mem, 
			    gs_separation_name colorname_index,
			    unsigned char **ppstr, 
			    unsigned int *pname_size);

#endif /* zht2_INCLUDED */
