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

/* $Id: icid.h 9043 2008-08-28 22:48:19Z giles $ */
/* Interface to zcid.c, zfcid0.c */

#ifndef icid_INCLUDED
#  define icid_INCLUDED

#ifndef gs_cid_system_info_DEFINED
#  define gs_cid_system_info_DEFINED
typedef struct gs_cid_system_info_s gs_cid_system_info_t;
#endif

/* Get the information from a CIDSystemInfo dictionary. */
int cid_system_info_param(gs_cid_system_info_t *, const ref *);

/* Convert a CID into TT char code or to TT glyph index, using SubstNWP. */
/* Returns 1 if a glyph presents, 0 if not, <0 if error. */
int cid_to_TT_charcode(const gs_memory_t *mem, 
		       const ref *Decoding, const ref *TT_cmap,  
		       const ref *SubstNWP, 
                       uint nCID, uint *c, ref *src_type, ref *dst_type);

/* Create a CIDMap from a True Type cmap, Decoding and SubstNWP. */
int cid_fill_CIDMap(const gs_memory_t *mem, const ref *Decoding, const ref *TT_cmap, const ref *SubstNWP, 
                    int GDBytes, ref *CIDMap);

/* <cid9font> <cid> .type9mapcid <charstring> <font_index> */
int ztype9mapcid(i_ctx_t *i_ctx_p);

#endif /* icid_INCLUDED */
