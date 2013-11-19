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

/* $Id: gsht1.h 8022 2007-06-05 22:23:38Z giles $ */
/* Extended public interface to halftones */

#ifndef gsht1_INCLUDED
#  define gsht1_INCLUDED

#include "gsht.h"

/* Procedural interface */
int gs_setcolorscreen(gs_state *, gs_colorscreen_halftone *);
int gs_currentcolorscreen(gs_state *, gs_colorscreen_halftone *);

/*
 * We include sethalftone here, even though it is a Level 2 feature,
 * because it turns out to be convenient to define setcolorscreen
 * using sethalftone.
 */
#ifndef gs_halftone_DEFINED
#  define gs_halftone_DEFINED
typedef struct gs_halftone_s gs_halftone;

#endif
/*
 * gs_halftone structures may have complex substructures.  We provide two
 * procedures for setting them.  gs_halftone assumes that the gs_halftone
 * structure and all its substructures was allocated with the same allocator
 * as the gs_state; gs_halftone_allocated looks in the structure itself (the
 * rc.memory member) to find the allocator that was used.  Both procedures
 * copy the top-level structure (using the appropriate allocator), but take
 * ownership of the substructures.
 */
int gs_sethalftone(gs_state *, gs_halftone *);
int gs_sethalftone_allocated(gs_state *, gs_halftone *);
int gs_currenthalftone(gs_state *, gs_halftone *);

#endif /* gsht1_INCLUDED */
