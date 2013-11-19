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

/* $Id: simscale.h 6651 2006-03-13 16:18:19Z raph $ */
/* Definitions for image mask interpolation filter */
/* Requires scommon.h; strimpl.h if any templates are referenced */

#ifndef simscale_INCLUDED
#  define simscale_INCLUDED

typedef struct stream_imscale_state_s stream_imscale_state;

struct stream_imscale_state_s {
    stream_image_scale_state_common;
    byte *window;

    int src_y;
    int src_offset;
    int src_size;

    byte *dst;
    int dst_y;
    int dst_offset;
    int dst_size;
};

extern const stream_template s_imscale_template;

#endif /* simscale_INCLUDED */
