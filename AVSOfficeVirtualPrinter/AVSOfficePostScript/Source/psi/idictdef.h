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

/* $Id: idictdef.h 9043 2008-08-28 22:48:19Z giles $ */
/* Internals of dictionary implementation */

#ifndef idictdef_INCLUDED
#  define idictdef_INCLUDED

/*
 * A dictionary of capacity M is a structure containing the following
 * elements (refs):
 *
 *      keys - a t_shortarray or t_array of M+1 elements, containing
 *      the keys.
 *
 *      values - a t_array of M+1 elements, containing the values.
 *
 *      count - a t_integer whose value tells how many entries are
 *      occupied (N).
 *
 *      maxlength - a t_integer whose value gives the client's view of
 *      the capacity (C).  C may be less than M (see below).
 *
 *      memory - a foreign t_struct referencing the allocator used to
 *      create this dictionary, which will also be used to expand or
 *      unpack it if necessary.
 *
 * C < M is possible because on large-memory systems, we usually round up M
 * so that M is a power of 2 (see idict.h for details); this allows us to
 * use masking rather than division for computing the initial hash probe.
 * However, C is always the maxlength specified by the client, so clients
 * get a consistent story.
 *
 * As noted above, the keys may be either in packed or unpacked form.
 * The markers for unused and deleted entries are different in the two forms.
 * In the packed form:
 *      unused entries contain packed_key_empty;
 *      deleted entries contain packed_key_deleted.
 * In the unpacked form:
 *      unused entries contain a literal null;
 *      deleted entries contain an executable null.
 *
 * The first entry is always marked deleted, to reduce the cost of the
 * wrap-around check.
 *
 * Note that if the keys slot in the dictionary is new,
 * all the key slots are new (more recent than the last save).
 * We use this fact to avoid saving stores into packed keys
 * for newly created dictionaries.
 *
 * Note that name keys with indices above packed_name_max_index require using
 * the unpacked form.  */
#define dict_is_packed(dct) r_has_type(&(dct)->keys, t_shortarray)
#define packed_key_empty (pt_tag(pt_integer) + 0)
#define packed_key_deleted (pt_tag(pt_integer) + 1)
#define packed_key_impossible pt_tag(pt_full_ref)	/* never matches */
#define packed_name_key(nidx)\
  ((nidx) <= packed_name_max_index ? pt_tag(pt_literal_name) + (nidx) :\
   packed_key_impossible)
/*
 * Using a special mark for deleted entries causes lookup time to degrade
 * as entries are inserted and deleted.  This is not a problem, because
 * entries are almost never deleted.
 */
#define d_maxlength(dct) ((uint)((dct)->maxlength.value.intval))
#define d_set_maxlength(dct,siz) ((dct)->maxlength.value.intval = (siz))
#define nslots(dct) r_size(&(dct)->values)
#define npairs(dct) (nslots(dct) - 1)
#define d_length(dct) ((uint)((dct)->count.value.intval))

/* packed_search_value_pointer simplifies the access to 
   packed dictionary search template data - see idicttpl.h . */
#define packed_search_value_pointer (pdict->values.value.refs + (kp - kbot))

#endif /* idictdef_INCLUDED */
