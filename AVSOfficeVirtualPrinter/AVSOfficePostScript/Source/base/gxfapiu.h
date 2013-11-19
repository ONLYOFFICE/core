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

/* $Id: gxfapiu.h 8022 2007-06-05 22:23:38Z giles $ */
/* Font API support : UFST callback dispatch. */

#ifndef gxfapiu_INCLUDED
#define gxfapiu_INCLUDED

#include "gp.h"

/* Set UFST callbacks. */
/* Warning : the language switch progect doesn't guarantee
   that this function is called when switching
   to another interpreter. Therefore each interpreter must take
   care for its own callback methods before they 
   may be called by UFST.
 */
 /* Warning : this function may cause a reentrancy problem
    due to a modification of static variables.
    Nevertheless this problem isn't important in a
    sngle interpreter build because the values
    really change on the first demand only.
    See also a comment in gs_fapiufst_finit.
  */
void gx_set_UFST_Callbacks(LPUB8 (*p_PCLEO_charptr)(FSP LPUB8 pfont_hdr, UW16  sym_code),
                           LPUB8 (*p_PCLchId2ptr)(FSP UW16 chId),
                           LPUB8 (*p_PCLglyphID2Ptr)(FSP UW16 glyphID));

void gx_reset_UFST_Callbacks(void);

typedef struct fco_list_elem_s fco_list_elem;
struct fco_list_elem_s {
    int open_count;
    SW16 fcHandle;
    char *file_path;
    fco_list_elem *next;
};

/* Access to the static FCO list for the language switching project : */
/* For the language switch : */
UW16 gx_UFST_open_static_fco(const char *font_file_path, SW16 *result_fcHandle);
UW16 gx_UFST_close_static_fco(SW16 fcHandle);
/* close all open FCO's */
void gx_UFST_close_static_fcos(void);
SW16 gx_UFST_find_fco_handle_by_name(const char *font_file_path);

/* For fapiufst.c : */
fco_list_elem *gx_UFST_find_static_fco(const char *font_file_path);
fco_list_elem *gx_UFST_find_static_fco_handle(SW16 fcHandle);

int gx_UFST_init(const UB8 *ufst_root_dir);

int gx_UFST_fini(void);

#endif /* gxfapiu_INCLUDED */
