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

/* $Id: gxttfb.h 8022 2007-06-05 22:23:38Z giles $ */
/* A bridge to True Type interpreter. */

#ifndef gxttfb_INCLUDED
#  define gxttfb_INCLUDED

#include "ttfoutl.h"

#ifndef gx_ttfReader_DEFINED
#  define gx_ttfReader_DEFINED
typedef struct gx_ttfReader_s gx_ttfReader;
#endif

#ifndef gs_font_type42_DEFINED
#  define gs_font_type42_DEFINED
typedef struct gs_font_type42_s gs_font_type42;
#endif

struct gx_ttfReader_s {
    ttfReader super;
    int pos;
    bool error;
    int extra_glyph_index;
    gs_font_type42 *pfont;
    gs_memory_t *memory;
    gs_glyph_data_t glyph_data;
    /*  When TT interpreter is invoked, a font and the TT interpreter instance
	may use different memory allocators (local and global correspondingly).
	Since we don't want to change Free Type function prototypes, 
	we place the gx_ttfReader instance into the global memory,
	to provide an access to it through TExecution_Context.
	Due to that, the fields 'pfont' and 'glyph_data' may contain pointers from global 
	to local memory. They must be NULL when a garbager is invoked.
	We reset them whan the TT interpreter exits.
     */
};

gx_ttfReader *gx_ttfReader__create(gs_memory_t *mem);
void gx_ttfReader__destroy(gx_ttfReader *this);
void gx_ttfReader__set_font(gx_ttfReader *this, gs_font_type42 *pfont);
ttfFont *ttfFont__create(gs_font_dir *dir);
void ttfFont__destroy(ttfFont *this, gs_font_dir *dir);
int ttfFont__Open_aux(ttfFont *this, ttfInterpreter *tti, gx_ttfReader *r, gs_font_type42 *pfont,
    	       const gs_matrix * char_tm, const gs_log2_scale_point *log2_scale,
	       bool design_grid);
int gx_ttf_outline(ttfFont *ttf, gx_ttfReader *r, gs_font_type42 *pfont, int glyph_index, 
	const gs_matrix *m, const gs_log2_scale_point * pscale, 
	gx_path *path, bool grid_fit);

#endif /* gxttfb_INCLUDED */
