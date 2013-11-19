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
/* $Id: gxhldevc.c 8022 2007-06-05 22:23:38Z giles $ */
/* High level device color save/compare procedures */

/*
 * See comments at the start of gxhldevc.h for more explanation of the
 * purpose and operation of these procedures.
 */
#include "gx.h"
#include "gzstate.h"
#include "gscspace.h"
#include "gxcspace.h"
#include "gxhldevc.h"
#include "memory_.h"
#include "gxpcolor.h"
#include "gsptype2.h"

/*
 * Initiailze a high level saved color to null
 */
void gx_hld_saved_color_init(gx_hl_saved_color * psc)
{
    gx_device_color temp_devc;

    memset(psc, 0, sizeof(*psc));	/* clear the entire structure */   
    psc->color_space_id = psc->pattern_id = gs_no_id;
    color_set_null(&temp_devc);
    temp_devc.type->save_dc(&temp_devc, &(psc->saved_dev_color));
}

/*
 * Get graphics state pointer (from imager state pointer)
 */
const gs_state * gx_hld_get_gstate_ptr(const gs_imager_state * pis)
{
    extern_st(st_gs_state);		/* only for testing */

    /* Check to verify the structure type is really st_gs_state */
    if (pis == NULL || gs_object_type(pis->memory, pis) != &st_gs_state)
	return NULL;

    return (const gs_state *) pis;
}

/*
 * Save the device color information including the color space id and
 * client color data (if available).
 *
 * More description in src/gxhldevc.h
 */
bool
gx_hld_save_color(const gs_imager_state * pis, const gx_device_color * pdevc,
		gx_hl_saved_color * psc)
{
    const gs_state * pgs = gx_hld_get_gstate_ptr(pis);
    memset(psc, 0, sizeof(*psc));	/* clear the entire structure */

    if (pdevc == NULL) {
        /* No device color given, should not happen */
        gx_hld_saved_color_init(psc);	/* revert to unknown color */
	return false;
    } else if (pgs == NULL) {
        /* No color space, simply save device color specific info */
        psc->color_space_id = psc->pattern_id = gs_no_id;
        pdevc->type->save_dc(pdevc, &(psc->saved_dev_color));
	return false;
    } else {
        /*
	 * Have color space, save id,  ccolor, & device color specific info.
	 * Also save the high level colors since two gx_color_index values
	 * may be the same but for differing high level colors (due to the
	 * usual lower resolution of the gx_color_index values.
	 */
        const gs_color_space * pcs = pgs->color_space;
        int i = gs_color_space_num_components(pcs);

        psc->color_space_id = pcs->id;
        pdevc->type->save_dc(pdevc, &(psc->saved_dev_color));
	if (pdevc->type == gx_dc_type_pattern2)
	    i = 0;
        else if (i < 0)
	    i = -i - 1; /* See gx_num_components_Pattern. */
        for (i--; i >= 0; i--)
	    psc->ccolor.paint.values[i] = pdevc->ccolor.paint.values[i];

	/* Save the pattern id - if present */
	if ((pdevc->type == gx_dc_type_pattern 
	   || pdevc->type == gx_dc_type_pattern2) && pdevc->ccolor_valid)
            psc->pattern_id = pdevc->ccolor.pattern->pattern_id;
	else
            psc->pattern_id = gs_no_id;
	return true;
    }
}

/*
 * Compare two saved colors to check if match.  Note this routine assumes
 * unused parts of the saved color have been zeroed.  See gx_hld_save_color()
 * for what is actually being compared.
 */
bool gx_hld_saved_color_equal(const gx_hl_saved_color * psc1,
			   const gx_hl_saved_color * psc2)
{
    return (memcmp(psc1, psc2, sizeof(*psc1)) == 0);
}

/*
 * Check whether two saved colors have same color space.
 */
bool gx_hld_saved_color_same_cspace(const gx_hl_saved_color * psc1,
			   const gx_hl_saved_color * psc2)
{
    if (psc1->color_space_id != psc2->color_space_id)
	return false;
    if (psc1->pattern_id != psc2->pattern_id)
	return false;
    if (psc1->ccolor_valid != psc2->ccolor_valid)
	return false;
    if (psc1->color_space_id != psc2->color_space_id)
	return false;
    return true;
}

/*
 * Check if a high level color is availavble.
 */
bool
gx_hld_is_hl_color_available(const gs_imager_state * pis,
		const gx_device_color * pdevc)
{
    const gs_state * pgs = gx_hld_get_gstate_ptr(pis);

    if (pgs != NULL && pdevc != NULL && pdevc->ccolor_valid)
	return true;
    return false;
}

/*
 * Get pointers to the current color space and client color.
 *
 * More description in src/gxhldevc.h
 */
gx_hld_get_color_space_and_ccolor_status
gx_hld_get_color_space_and_ccolor(const gs_imager_state * pis,
		const gx_device_color * pdevc, const gs_color_space ** ppcs,
		const gs_client_color ** ppcc)
{
    /* Check if the current color space was used to build the device color */
    if (gx_hld_is_hl_color_available(pis, pdevc)) {
	const gs_state * pgs = gx_hld_get_gstate_ptr(pis);
        const gs_color_space * pcs = pgs->color_space;

	*ppcs = pcs;
	*ppcc = &(pdevc->ccolor);
	if (pdevc->type == gx_dc_type_pattern 
	   || pdevc->type == &gx_dc_pure_masked
	   || pdevc->type == gx_dc_type_pattern2)
            return pattern_color_sapce;
	else {
            return non_pattern_color_space;
	}
    }
    /* No color space */
    *ppcs = NULL;
    *ppcc = NULL;
    return use_process_color;
}

/*
 * Get the number of components in the current color space.
 *
 * More description in src/gxhldevc.h
 */
int
gx_hld_get_number_color_components(const gs_imager_state * pis)
{
    const gs_state * pgs = gx_hld_get_gstate_ptr(pis);

    if (pgs != NULL) {
        const gs_color_space * pcs = pgs->color_space;
	int n = gs_color_space_num_components(pcs);

	return (n >= 0 ? n : -n - 1);
    } else
	return -1;
}

/*
 * Get the requested high level color value.
 *
 * More description in src/gxhldevc.h
 */
gx_hld_get_color_component_status
gx_hld_get_color_component(const gs_imager_state * pis,
			  const gx_device_color * pdevc,
			  int comp_num, float * output)
{
    if (pdevc != NULL && pdevc->ccolor_valid) {
	int ncomp = gx_hld_get_number_color_components(pis);

	if (ncomp < 0)
	    return invalid_color_info;
	if (comp_num < 0 || comp_num >= ncomp) 
	    return invalid_component_requested;
	*output = pdevc->ccolor.paint.values[comp_num];
        return valid_result;
    }
    return invalid_color_info;
}

