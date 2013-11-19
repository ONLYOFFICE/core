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

/* $Id: gscsepr.h 8022 2007-06-05 22:23:38Z giles $ */
/* Client interface to Separation color */

#ifndef gscsepr_INCLUDED
#  define gscsepr_INCLUDED

#include "gscspace.h"


/*
 * Separation color spaces.
 *
 * With the implementation of full DeviceN support, the API for creating
 * A Separation color space object has changed.  Separation color spaces
 * are treated as a single component version of a DeviceN color space.
 * The /All separation and the /None separation are the only exceptions.
 * The /All separation and the /None separation is each treated as its own
 * case and thus forms the majority of the remaining Separation color space
 * code.
 *
 * The earlier API which had a multi element cache has been removed.
 * The tint transform procedure now is executed as required and must
 * be executable without doing a call out.
 */

/*
 * This routine constructs a new separation color space object.
 */
extern int gs_cspace_new_Separation(
    gs_color_space **ppcs,
    gs_color_space * palt_cspace,
    gs_memory_t * pmem
    );

/* Set the Separation tint transformation procedure to a Function. */
#ifndef gs_function_DEFINED
typedef struct gs_function_s gs_function_t;
#  define gs_function_DEFINED
#endif
int gs_cspace_set_sepr_proc(gs_color_space * pcspace,
			int (*proc)(const float *,
				    float *,
				    const gs_imager_state *,
				    void *
				    ),
			void *proc_data
			);
/*
 * Set the Separation tint transformation procedure to a Function.
 */
int gs_cspace_set_sepr_function(const gs_color_space *pcspace,
				    gs_function_t *pfn);
/*
 * If the Separation tint transformation procedure is a Function,
 * return the function object, otherwise return 0.
 */
gs_function_t *gs_cspace_get_sepr_function(const gs_color_space *pcspace);

#endif /* gscsepr_INCLUDED */
