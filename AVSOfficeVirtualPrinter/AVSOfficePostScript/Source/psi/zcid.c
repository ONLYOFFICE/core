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

/* $Id: zcid.c 9043 2008-08-28 22:48:19Z giles $ */
/* CMap and CID-keyed font services */
#include "ghost.h"
#include "ierrors.h"
#include "gxcid.h"
#include "icid.h"		/* for checking prototype */
#include "idict.h"
#include "idparam.h"
#include "store.h"
#include "oper.h"

/* Get the information from a CIDSystemInfo dictionary. */
int
cid_system_info_param(gs_cid_system_info_t *pcidsi, const ref *prcidsi)
{
    ref *pregistry;
    ref *pordering;
    int code;

    if (!r_has_type(prcidsi, t_dictionary))
	return_error(e_typecheck);
    if (dict_find_string(prcidsi, "Registry", &pregistry) <= 0 ||
	dict_find_string(prcidsi, "Ordering", &pordering) <= 0
	)
	return_error(e_rangecheck);
    check_read_type_only(*pregistry, t_string);
    check_read_type_only(*pordering, t_string);
    pcidsi->Registry.data = pregistry->value.const_bytes;
    pcidsi->Registry.size = r_size(pregistry);
    pcidsi->Ordering.data = pordering->value.const_bytes;
    pcidsi->Ordering.size = r_size(pordering);
    code = dict_int_param(prcidsi, "Supplement", 0, max_int, -1,
			  &pcidsi->Supplement);
    return (code < 0 ? code : 0);
}

/* Convert a CID into TT char code or to TT glyph index. */
static bool 
TT_char_code_from_CID_no_subst(const gs_memory_t *mem, 
			       const ref *Decoding, const ref *TT_cmap, uint nCID, uint *c)
{   ref *DecodingArray, char_code, char_code1, ih, *glyph_index;
    bool found = false;
    int i = nCID % 256, n;

    make_int(&ih, nCID / 256);
    if (dict_find(Decoding, &ih, &DecodingArray) <= 0 || 
	    !r_has_type(DecodingArray, t_array) ||
	    array_get(mem, DecodingArray, i, &char_code) < 0)
	return false;
    if (r_has_type(&char_code, t_integer))
	n = 1; 
    else if (r_has_type(&char_code, t_array)) {
	DecodingArray = &char_code;
	i = 0;
	n = r_size(DecodingArray);
    } else
	return false; /* Must not happen. */
    for (;n--; i++) {
	if (array_get(mem, DecodingArray, i, &char_code1) < 0 ||
	    !r_has_type(&char_code1, t_integer))
	    return false; /* Must not happen. */
	if (dict_find(TT_cmap, &char_code1, &glyph_index) >= 0 &&
		r_has_type(glyph_index, t_integer)) {
	    *c = glyph_index->value.intval;
	    found = true;
	    if (*c != 0)
		return true;
	}
    }
    return found;
}

/* Convert a CID into a TT char code or into a TT glyph index, using SubstNWP. */
/* Returns 1 if a glyph presents, 0 if not, <0 if error. */
int
cid_to_TT_charcode(const gs_memory_t *mem, 
		   const ref *Decoding, const ref *TT_cmap, const ref *SubstNWP, 
                   uint nCID, uint *c, ref *src_type, ref *dst_type)
{
    int SubstNWP_length = r_size(SubstNWP), i, code;

    if (TT_char_code_from_CID_no_subst(mem, Decoding, TT_cmap, nCID, c)) {
	make_null(src_type);
	/* Leaving dst_type uninitialized. */
	return 1;
    }
    for (i = 0; i < SubstNWP_length; i += 5) {
        ref rb, re, rs;
        int nb, ne, ns;
        
	if ((code = array_get(mem, SubstNWP, i + 1, &rb)) < 0)
	    return code;
        if ((code = array_get(mem, SubstNWP, i + 2, &re)) < 0)
	    return code;
        if ((code = array_get(mem, SubstNWP, i + 3, &rs)) < 0)
	    return code;
        nb = rb.value.intval;
        ne = re.value.intval;
        ns = rs.value.intval;
        if (nCID >= nb && nCID <= ne)
            if (TT_char_code_from_CID_no_subst(mem, Decoding, TT_cmap, ns + (nCID - nb), c)) {
		if ((code = array_get(mem, SubstNWP, i + 0, src_type)) < 0)
		    return code;
		if ((code = array_get(mem, SubstNWP, i + 4, dst_type)) < 0)
		    return code;
                return 1;
	    }
        if (nCID >= ns && nCID <= ns + (ne - nb))
            if (TT_char_code_from_CID_no_subst(mem, Decoding, TT_cmap, nb + (nCID - ns), c)) {
		if ((code = array_get(mem, SubstNWP, i + 0, dst_type)) < 0)
		    return code;
		if ((code = array_get(mem, SubstNWP, i + 4, src_type)) < 0)
		    return code;
                return 1;
	    }
    }
    *c = 0;
    return 0;
}

/* Set a CIDMap element. */
static int 
set_CIDMap_element(const gs_memory_t *mem, ref *CIDMap, uint cid, uint glyph_index)
{   /* Assuming the CIDMap is already type-checked. */
    /* Assuming GDBytes == 2. */
    int offset = cid * 2;
    int count = r_size(CIDMap), size, i;
    ref s;
    uchar *c;

    if (glyph_index >= 65536)
	return_error(e_rangecheck); /* Can't store with GDBytes == 2. */
    for (i = 0; i < count; i++) {
	array_get(mem, CIDMap, i, &s);
	size = r_size(&s) & ~1;
	if (offset < size) {
	    c = s.value.bytes + offset;
	    c[0] = (uchar)(glyph_index >> 8);
	    c[1] = (uchar)(glyph_index & 255);
	    break;
	}
	offset -= size;
    }
    /* We ignore the substitution if it goes out the CIDMap range.
       It must not happen, except for empty Decoding elements */
    return 0;
}

/* Create a CIDMap from a True Type cmap array, Decoding and SubstNWP. */
int
cid_fill_CIDMap(const gs_memory_t *mem, 
		const ref *Decoding, const ref *TT_cmap, const ref *SubstNWP, int GDBytes, 
                ref *CIDMap)
{   int dict_enum;
    ref el[2];
    int count, i;

    if (GDBytes != 2)
	return_error(e_unregistered); /* Unimplemented. */
    if (r_type(CIDMap) != t_array)
	return_error(e_unregistered); /* Unimplemented. It could be a single string. */
    count = r_size(CIDMap);
    /* Checking the CIDMap structure correctness : */
    for (i = 0; i < count; i++) {
	ref s;
	int code = array_get(mem, CIDMap, i, &s);

	if (code < 0)
	    return code;
	check_type(s, t_string); /* fixme : optimize with moving to TT_char_code_from_CID. */
    }
    /* Compute the CIDMap : */
    dict_enum = dict_first(Decoding);
    for (;;) {
        int index, count, i;
	
	if ((dict_enum = dict_next(Decoding, dict_enum, el)) == -1)
	    break;
	if (!r_has_type(&el[0], t_integer))
	    continue;
	if (!r_has_type(&el[1], t_array))
	    return_error(e_typecheck);
	index = el[0].value.intval;
	count = r_size(&el[1]);
	for (i = 0; i < count; i++) {
	    uint cid = index * 256 + i, glyph_index;
	    ref src_type, dst_type;
	    int code = cid_to_TT_charcode(mem, Decoding, TT_cmap, SubstNWP, 
                                cid, &glyph_index, &src_type, &dst_type);

	    if (code < 0)
		return code;
	    if (code > 0) {
	        code = set_CIDMap_element(mem, CIDMap, cid, glyph_index);
		if (code < 0)
		    return code;
	    }
	}
    }
    return 0;
}
