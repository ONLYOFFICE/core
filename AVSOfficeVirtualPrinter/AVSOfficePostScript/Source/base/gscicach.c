/* Copyright (C) 2001-2007 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: gscicach.c 8250 2007-09-25 13:31:24Z giles $ */
/* A color index cache. */
#include "gx.h"
#include "gserrors.h"
#include "gsccolor.h"
#include "gxcspace.h"
#include "gxdcolor.h"
#include "gscicach.h"
#include "memory_.h"

#define COLOR_INDEX_CACHE_SIZE 256
#define COLOR_INDEX_CACHE_CHAINS (COLOR_INDEX_CACHE_SIZE / 16)

typedef struct gs_color_index_cache_elem_s gs_color_index_cache_elem_t;

struct gs_color_index_cache_elem_s {
    gx_color_index cindex;
    uint chain;
    uint prev, next; /* NULL for unused. */
    uint touch_prev, touch_next;
    bool frac_values_done;
};

struct gs_color_index_cache_s {
    const gs_color_space *direct_space;
    gs_imager_state *pis;
    gx_device *dev;
    int client_num_components;
    int device_num_components;
    gs_memory_t *memory;
    int used;
    gs_color_index_cache_elem_t *buf;
    uint recent_touch;
    float *paint_values;
    frac31 *frac_values;
    int chains[COLOR_INDEX_CACHE_CHAINS];
    /* Note : the 0th element of buf, paint_values, frac_values is never used,
       because we consider the index 0 as NULL
       just for a faster initialization. */
#   define MYNULL 0
};



gs_private_st_ptrs5(st_color_index_cache, gs_color_index_cache_t, "gs_color_index_cache_t",
		    gs_color_index_cache_elem_ptrs, gs_color_index_cache_reloc_ptrs,
		    direct_space, memory, buf, paint_values, frac_values);

gs_color_index_cache_t *
gs_color_index_cache_create(gs_memory_t *memory, const gs_color_space *direct_space, gx_device *dev, gs_imager_state *pis, bool need_frac)
{
    int client_num_components = cs_num_components(direct_space);
    int device_num_components = dev->color_info.num_components;
    gs_color_index_cache_elem_t *buf = ( gs_color_index_cache_elem_t *)gs_alloc_byte_array(memory, COLOR_INDEX_CACHE_SIZE, 
		    sizeof(gs_color_index_cache_elem_t), "gs_color_index_cache_create");
    float *paint_values = (float *)gs_alloc_byte_array(memory, COLOR_INDEX_CACHE_SIZE * client_num_components, 
		    sizeof(float), "gs_color_index_cache_create");
    frac31 *frac_values = (need_frac ? (frac31 *)gs_alloc_byte_array(memory, COLOR_INDEX_CACHE_SIZE * device_num_components, 
					    sizeof(frac31), "gs_color_index_cache_create") : NULL);
    gs_color_index_cache_t *pcic = gs_alloc_struct(memory, gs_color_index_cache_t, &st_color_index_cache, "gs_color_index_cache_create");

    if (buf == NULL || paint_values == NULL || (need_frac && frac_values == NULL) || pcic == NULL) {
	gs_free_object(memory, buf, "gs_color_index_cache_create");
	gs_free_object(memory, paint_values, "gs_color_index_cache_create");
	gs_free_object(memory, frac_values, "gs_color_index_cache_create");
	gs_free_object(memory, pcic, "gs_color_index_cache_create");
	return NULL;
    }
    memset(pcic, 0, sizeof(*pcic));
    memset(buf, 0, COLOR_INDEX_CACHE_SIZE * sizeof(gs_color_index_cache_elem_t));
    pcic->direct_space = direct_space;
    pcic->pis = pis;
    pcic->dev = dev;
    pcic->device_num_components = device_num_components;
    pcic->client_num_components = client_num_components;
    pcic->memory = memory;
    pcic->used = 1; /* Never use the 0th element. */
    pcic->buf = buf;
    pcic->recent_touch = MYNULL;
    pcic->paint_values = paint_values;
    pcic->frac_values = frac_values;
    return pcic;
}

void 
gs_color_index_cache_destroy(gs_color_index_cache_t *pcic)
{
    gs_free_object(pcic->memory, pcic->buf, "gs_color_index_cache_create");
    gs_free_object(pcic->memory, pcic->paint_values, "gs_color_index_cache_create");
    gs_free_object(pcic->memory, pcic->frac_values, "gs_color_index_cache_create");
    pcic->buf = NULL;
    pcic->paint_values = NULL;
    pcic->frac_values = NULL;
    gs_free_object(pcic->memory, pcic, "gs_color_index_cache_create");
}

static inline int 
hash_paint_values(const gs_color_index_cache_t *this, const float *paint_values)
{
    int i;
    float v = 0;
    uint k = 0;
    const uint a_prime = 79;

    for (i = 0; i < this->client_num_components; i++) 
	v = v * a_prime + paint_values[i];
    /* Don't know the range of v, so hash its bytes : */
    for(i = 0; i < sizeof(v); i++)
	k = k * a_prime + ((byte *)&v)[i];
    return k % COLOR_INDEX_CACHE_CHAINS;
}

static inline void
exclude_from_chain(gs_color_index_cache_t *this, uint i)
{
    uint co = this->buf[i].chain;
    uint ip = this->buf[i].prev, in = this->buf[i].next;

    this->buf[ip].next = in;
    this->buf[in].prev = ip;
    if (this->chains[co] == i)
	this->chains[co] = in;
}

static inline void
include_into_chain(gs_color_index_cache_t *this, uint i, uint c)
{
    if (this->chains[c] != MYNULL) {
	uint in = this->chains[c], ip = this->buf[in].prev;

	this->buf[i].next = in;
	this->buf[i].prev = ip;
	this->buf[in].prev = i;
	this->buf[ip].next = i;
    } else
	this->buf[i].prev = this->buf[i].next = i;
    this->chains[c] = i;
    this->buf[i].chain = c;
}

static inline void
exclude_from_touch_list(gs_color_index_cache_t *this, uint i)
{
    uint ip = this->buf[i].touch_prev, in = this->buf[i].touch_next;

    this->buf[ip].touch_next = in;
    this->buf[in].touch_prev = ip;
    if (this->recent_touch == i) {
	if (i == in)
	    this->recent_touch = MYNULL;
	else
	    this->recent_touch = in;
    }
}

static inline void
include_into_touch_list(gs_color_index_cache_t *this, uint i)
{
    if (this->recent_touch != MYNULL) {
	uint in = this->recent_touch, ip = this->buf[in].touch_prev;

	this->buf[i].touch_next = in;
	this->buf[i].touch_prev = ip;
	this->buf[in].touch_prev = i;
	this->buf[ip].touch_next = i;
    } else
	this->buf[i].touch_prev = this->buf[i].touch_next = i;
    this->recent_touch = i;
}

static int 
get_color_index_cache_elem(gs_color_index_cache_t *this, const float *paint_values, uint *pi)
{
    int client_num_components = this->client_num_components;
    uint c = hash_paint_values(this, paint_values);
    uint i = this->chains[c], j;

    if (i != MYNULL) {
	uint tries = 16; /* Arbitrary. */

	if (!memcmp(paint_values, this->paint_values + i * client_num_components, sizeof(*paint_values) * client_num_components)) {
	    if (this->recent_touch != i) {
		exclude_from_touch_list(this, i);
		include_into_touch_list(this, i);
	    }
	    *pi = i;
	    return 1;
	}
	for (j = this->buf[i].next; tries -- && j != i; j = this->buf[j].next) {
	    if (!memcmp(paint_values, this->paint_values + j * client_num_components, sizeof(*paint_values) * client_num_components)) {
		exclude_from_chain(this, j);
		include_into_chain(this, j, c);
		if (this->recent_touch != j) {
		    exclude_from_touch_list(this, j);
		    include_into_touch_list(this, j);
		}
		*pi = j;
		return 1;
	    }
	}
	tries+=0;
    }
    if (this->used < COLOR_INDEX_CACHE_SIZE) {
	/* Use a new one */
	i = this->used++;
	include_into_touch_list(this, i);
    } else {
	i = this->recent_touch;
	this->recent_touch = this->buf[i].touch_prev; /* Assuming the cyclic list, 
						      just move the head pointer to the last element. */
	exclude_from_chain(this, i);
    }
    include_into_chain(this, i, c);
    *pi = i;
    return 0;
}

static inline void
compute_frac_values(gs_color_index_cache_t *this, uint i)
{
    gx_color_index c = this->buf[i].cindex;
    const gx_device_color_info *cinfo = &this->dev->color_info;
    int device_num_components = this->device_num_components;
    int j;

    for (j = 0; j < device_num_components; j++) {
	    int shift = cinfo->comp_shift[j];
	    int bits = cinfo->comp_bits[j];

	    this->frac_values[i * device_num_components + j] = ((c >> shift) & ((1 << bits) - 1)) << (sizeof(frac31) * 8 - 1 - bits);
    }
    this->buf[i].frac_values_done = true;
}

int 
gs_cached_color_index(gs_color_index_cache_t *this, const float *paint_values, gx_device_color *pdevc, frac31 *frac_values)
{
    /* Must return 2 if the color is not pure. 
       See patch_color_to_device_color. */
    const gs_color_space *pcs = this->direct_space;
    int client_num_components = this->client_num_components;
    int device_num_components = this->device_num_components;
    uint i;
    int code;

    if (get_color_index_cache_elem(this, paint_values, &i)) {
	if (pdevc != NULL) {
	    pdevc->colors.pure = this->buf[i].cindex;
	    pdevc->type = &gx_dc_type_data_pure;
	    memcpy(pdevc->ccolor.paint.values, paint_values, sizeof(*paint_values) * client_num_components);
	    pdevc->ccolor_valid = true;
	}
	if (frac_values != NULL && !this->buf[i].frac_values_done)
	    compute_frac_values(this, i);
    } else {
	gx_device_color devc_local;
	gs_client_color fcc;

	if (pdevc == NULL)
	    pdevc = &devc_local;
	memcpy(this->paint_values + i * client_num_components, paint_values, sizeof(*paint_values) * client_num_components);
	memcpy(fcc.paint.values, paint_values, sizeof(*paint_values) * client_num_components);
	code = pcs->type->remap_color(&fcc, pcs, pdevc, this->pis, this->dev, gs_color_select_texture);
	if (code < 0)
	    return code;
	if (pdevc->type != &gx_dc_type_data_pure)
	    return 2;
	this->buf[i].cindex = pdevc->colors.pure;
	
	if (frac_values != NULL)
    	    compute_frac_values(this, i);
	else
	    this->buf[i].frac_values_done = false;
    }
    if (frac_values != NULL)
	memcpy(frac_values, this->frac_values + i * device_num_components, sizeof(*frac_values) * device_num_components);
    return 0;
}
