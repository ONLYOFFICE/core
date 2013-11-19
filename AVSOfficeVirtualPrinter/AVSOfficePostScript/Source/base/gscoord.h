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

/* $Id: gscoord.h 8022 2007-06-05 22:23:38Z giles $ */
/* Interface to graphics state CTM procedures */
/* Requires gsmatrix.h and gsstate.h */

#ifndef gscoord_INCLUDED
#  define gscoord_INCLUDED

/* Coordinate system modification */
int gs_initmatrix(gs_state *),
    gs_defaultmatrix(const gs_state *, gs_matrix *),
    gs_currentmatrix(const gs_state *, gs_matrix *),
    gs_setmatrix(gs_state *, const gs_matrix *),
    gs_translate(gs_state *, floatp, floatp),
    gs_scale(gs_state *, floatp, floatp),
    gs_rotate(gs_state *, floatp),
    gs_concat(gs_state *, const gs_matrix *);

/* Extensions */
int gs_setdefaultmatrix(gs_state *, const gs_matrix *),
    gs_currentcharmatrix(gs_state *, gs_matrix *, bool),
    gs_setcharmatrix(gs_state *, const gs_matrix *),
    gs_settocharmatrix(gs_state *);

/* Coordinate transformation */
int gs_transform(gs_state *, floatp, floatp, gs_point *),
    gs_dtransform(gs_state *, floatp, floatp, gs_point *),
    gs_itransform(gs_state *, floatp, floatp, gs_point *),
    gs_idtransform(gs_state *, floatp, floatp, gs_point *);

#ifndef gs_imager_state_DEFINED
#  define gs_imager_state_DEFINED
typedef struct gs_imager_state_s gs_imager_state;
#endif

int gs_imager_setmatrix(gs_imager_state *, const gs_matrix *);
int gs_imager_idtransform(const gs_imager_state *, floatp, floatp, gs_point *);

#endif /* gscoord_INCLUDED */
