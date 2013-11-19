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

/* $Id: sarc4.h 8022 2007-06-05 22:23:38Z giles $ */
/* Definitions for Arcfour cipher and filter */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef sarc4_INCLUDED
#  define sarc4_INCLUDED

#include "scommon.h"

/* Arcfour is a symmetric cipher whose state is maintained
 * in two indices into an accompanying 8x8 S box. this will
 * typically be allocated on the stack, and so has no memory
 * management associated.
 */
struct stream_arcfour_state_s
{
    stream_state_common;	/* a define from scommon.h */
    unsigned int x, y;
    unsigned char S[256];
};

#ifndef stream_arcfour_state_DEFINED
#define stream_arcfour_state_DEFINED
typedef struct stream_arcfour_state_s stream_arcfour_state;
#endif

int s_arcfour_set_key(stream_arcfour_state * state, const unsigned char *key,
		      int keylength);

#define private_st_arcfour_state()	/* used in sarc4.c */\
  gs_private_st_simple(st_arcfour_state, stream_arcfour_state,\
    "Arcfour filter state")
extern const stream_template s_arcfour_template;

/* (de)crypt a section of text in a buffer -- the procedure is the same
 * in each direction. see strimpl.h for return codes.
 */
int s_arcfour_process_buffer(stream_arcfour_state *ss, byte *buf, int buf_size);

#endif /* sarc4_INCLUDED */
