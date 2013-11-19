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

/* $Id: sbcp.h 8022 2007-06-05 22:23:38Z giles $ */
/* Interface to [T]BCP streams */

#ifndef sbcp_INCLUDED
#  define sbcp_INCLUDED

/* (T)BCPEncode */
/* (no state) */
extern const stream_template s_BCPE_template;
extern const stream_template s_TBCPE_template;

/* (T)BCPDecode */
typedef struct stream_BCPD_state_s {
    stream_state_common;
    /* The client sets the following before initialization. */
    int (*signal_interrupt) (stream_state *);
    int (*request_status) (stream_state *);
    /* The following are updated dynamically. */
    bool escaped;
    int matched;		/* TBCP only */
    int copy_count;		/* TBCP only */
    const byte *copy_ptr;	/* TBCP only */
} stream_BCPD_state;

#define private_st_BCPD_state()	/* in sbcp.c */\
  gs_private_st_simple(st_BCPD_state, stream_BCPD_state, "(T)BCPDecode state")
extern const stream_template s_BCPD_template;
extern const stream_template s_TBCPD_template;

#endif /* sbcp_INCLUDED */
