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

/* $Id: gxcid.h 8022 2007-06-05 22:23:38Z giles $ */
/* Common data definitions for CMaps and CID-keyed fonts */

#ifndef gxcid_INCLUDED
#  define gxcid_INCLUDED

#include "gsstype.h"

/* Define the structure for CIDSystemInfo. */
#ifndef gs_cid_system_info_DEFINED
#  define gs_cid_system_info_DEFINED
typedef struct gs_cid_system_info_s gs_cid_system_info_t;
#endif
struct gs_cid_system_info_s {
    gs_const_string Registry;
    gs_const_string Ordering;
    int Supplement;
};
extern_st(st_cid_system_info);
extern_st(st_cid_system_info_element);
#define public_st_cid_system_info() /* in gsfcid.c */\
  gs_public_st_const_strings2(st_cid_system_info, gs_cid_system_info_t,\
    "gs_cid_system_info_t", cid_si_enum_ptrs, cid_si_reloc_ptrs,\
    Registry, Ordering)
#define st_cid_system_info_num_ptrs 2
#define public_st_cid_system_info_element() /* in gsfcid.c */\
  gs_public_st_element(st_cid_system_info_element, gs_cid_system_info_t,\
    "gs_cid_system_info_t[]", cid_si_elt_enum_ptrs, cid_si_elt_reloc_ptrs,\
    st_cid_system_info)

/*
 * The CIDSystemInfo of a CMap may be null.  We represent this by setting
 * Registry and Ordering to empty strings, and Supplement to 0.
 */
void cid_system_info_set_null(gs_cid_system_info_t *);
bool cid_system_info_is_null(const gs_cid_system_info_t *);

#endif /* gxcid_INCLUDED */
