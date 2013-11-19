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

/* $Id: sa85d.h 8022 2007-06-05 22:23:38Z giles $ */
/* ASCII85Decode filter interface */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef sa85d_INCLUDED
#  define sa85d_INCLUDED

/* ASCII85Decode */
typedef struct stream_A85D_state_s {
    stream_state_common;
    int odd;			/* # of odd digits */
    ulong word;			/* word being accumulated */
} stream_A85D_state;

#define private_st_A85D_state()	/* in sfilter2.c */\
  gs_private_st_simple(st_A85D_state, stream_A85D_state,\
    "ASCII85Decode state")
/* We define the initialization procedure here, so that the scanner */
/* can avoid a procedure call. */
#define s_A85D_init_inline(ss)\
  ((ss)->min_left = 1, (ss)->word = 0, (ss)->odd = 0)
extern const stream_template s_A85D_template;

#endif /* sa85d_INCLUDED */
