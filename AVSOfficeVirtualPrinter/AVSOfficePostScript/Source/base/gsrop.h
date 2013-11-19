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

/* $Id: gsrop.h 8022 2007-06-05 22:23:38Z giles $ */
/* RasterOp / transparency procedure interface */

#ifndef gsrop_INCLUDED
#  define gsrop_INCLUDED

#include "gsropt.h"

/* Procedural interface */

int gs_setrasterop(gs_state *, gs_rop3_t);
gs_rop3_t gs_currentrasterop(const gs_state *);
int gs_setsourcetransparent(gs_state *, bool);
bool gs_currentsourcetransparent(const gs_state *);
int gs_settexturetransparent(gs_state *, bool);
bool gs_currenttexturetransparent(const gs_state *);

/* Save/restore the combined logical operation. */
gs_logical_operation_t gs_current_logical_op(const gs_state *);
int gs_set_logical_op(gs_state *, gs_logical_operation_t);

#endif /* gsrop_INCLUDED */
