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

/* $Id: gxfmap.h 8022 2007-06-05 22:23:38Z giles $ */
/* Fraction map representation for Ghostscript */

#ifndef gxfmap_INCLUDED
#  define gxfmap_INCLUDED

#include "gsrefct.h"
#include "gsstype.h"
#include "gxfrac.h"
#include "gxtmap.h"

/*
 * Define a cached map from fracs to fracs.  Level 1 uses this only
 * for the transfer function; level 2 also uses it for black generation
 * and undercolor removal.  Note that reference counting macros must
 * be used to allocate, free, and assign references to gx_transfer_maps.
 *
 * NOTE: proc and closure are redundant.  Eventually closure will replace
 * proc.  For now, things are in an uneasy intermediate state where
 * proc = 0 means use closure.
 */
/* log2... must not be greater than frac_bits, and must be least 8. */
#define log2_transfer_map_size 8
#define transfer_map_size (1 << log2_transfer_map_size)
/*typedef struct gx_transfer_map_s gx_transfer_map; *//* in gxtmap.h */
struct gx_transfer_map_s {
    rc_header rc;
    gs_mapping_proc proc;	/* typedef is in gxtmap.h */
    gs_mapping_closure_t closure;	/* SEE ABOVE */
    /* The id changes whenever the map or function changes. */
    gs_id id;
    frac values[transfer_map_size];
};

extern_st(st_transfer_map);
#define public_st_transfer_map() /* in gscolor.c */\
  gs_public_st_composite(st_transfer_map, gx_transfer_map, "gx_transfer_map",\
    transfer_map_enum_ptrs, transfer_map_reloc_ptrs)

/* Set a transfer map to the identity map. */
void gx_set_identity_transfer(gx_transfer_map *);

/*
 * Map a color fraction through a transfer map.  If the map is small,
 * we interpolate; if it is large, we don't, and save a lot of time.
 */
#define FRAC_MAP_INTERPOLATE (log2_transfer_map_size <= 8)
#if FRAC_MAP_INTERPOLATE

frac gx_color_frac_map(frac, const frac *);		/* in gxcmap.c */

#  define gx_map_color_frac(pgs,cf,m)\
     (pgs->m->proc == gs_identity_transfer ? cf :\
      gx_color_frac_map(cf, &pgs->m->values[0]))

#else /* !FRAC_MAP_INTERPOLATE */

/* Do the lookup in-line. */
#  define gx_map_color_frac(pgs,cf,m)\
     (pgs->m->values[frac2bits(cf, log2_transfer_map_size)])

#endif /* (!)FRAC_MAP_INTERPOLATE */

/* Map a color fraction expressed as a byte through a transfer map. */
/* (We don't use this anywhere right now.) */
/****************
#if log2_transfer_map_size <= 8
#  define byte_to_tmx(b) ((b) >> (8 - log2_transfer_map_size))
#else
#  define byte_to_tmx(b)\
	(((b) << (log2_transfer_map_size - 8)) +\
	 ((b) >> (16 - log2_transfer_map_size)))
#endif
#define gx_map_color_frac_byte(pgs,b,m)\
 (pgs->m->values[byte_to_tmx(b)])
 ****************/

/* Map a floating point value through a transfer map. */
#define gx_map_color_float(pmap,v)\
  ((pmap)->values[(int)((v) * transfer_map_size + 0.5)] / frac_1_float)

/* Define a mapping procedure that just looks up the value in the cache. */
/* (It is equivalent to gx_map_color_float with the arguments swapped.) */
float gs_mapped_transfer(floatp, const gx_transfer_map *);

/* Define an identity mapping procedure. */
/* Don't store this directly in proc/closure.proc: */
/* use gx_set_identity_transfer. */
float gs_identity_transfer(floatp, const gx_transfer_map *);

#endif /* gxfmap_INCLUDED */
