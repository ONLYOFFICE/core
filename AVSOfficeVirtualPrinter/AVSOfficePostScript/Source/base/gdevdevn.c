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
/*$Id: gdevdevn.c 8250 2007-09-25 13:31:24Z giles $ */
/* Example DeviceN process color model devices. */

#include "math_.h"
#include "string_.h"
#include "gdevprn.h"
#include "gsparam.h"
#include "gscrd.h"
#include "gscrdp.h"
#include "gxlum.h"
#include "gdevdcrd.h"
#include "gstypes.h"
#include "gxdcconv.h"
#include "gdevdevn.h"
#include "gsequivc.h"
#include "gxblend.h"
#include "gdevp14.h"

/*
 * Utility routines for common DeviceN related parameters:
 *   SeparationColorNames, SeparationOrder, and MaxSeparations
 */

/* Convert a gray color space to DeviceN colorants. */
void
gray_cs_to_devn_cm(gx_device * dev, int * map, frac gray, frac out[])
{
    int i = dev->color_info.num_components - 1;

    for(; i >= 0; i--)			/* Clear colors */
        out[i] = frac_0;
    if ((i = map[3]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = frac_1 - gray;
}

/* Convert an RGB color space to DeviceN colorants. */
void
rgb_cs_to_devn_cm(gx_device * dev, int * map,
		const gs_imager_state *pis, frac r, frac g, frac b, frac out[])
{
    int i = dev->color_info.num_components - 1;
    frac cmyk[4];

    for(; i >= 0; i--)			/* Clear colors */
        out[i] = frac_0;
    color_rgb_to_cmyk(r, g, b, pis, cmyk);
    if ((i = map[0]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = cmyk[0];
    if ((i = map[1]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = cmyk[1];
    if ((i = map[2]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = cmyk[2];
    if ((i = map[3]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = cmyk[3];
}

/* Convert a CMYK color space to DeviceN colorants. */
void
cmyk_cs_to_devn_cm(gx_device * dev, int * map,
		frac c, frac m, frac y, frac k, frac out[])
{
    int i = dev->color_info.num_components - 1;

    for(; i >= 0; i--)			/* Clear colors */
        out[i] = frac_0;
    if ((i = map[0]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = c;
    if ((i = map[1]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = m;
    if ((i = map[2]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = y;
    if ((i = map[3]) != GX_DEVICE_COLOR_MAX_COMPONENTS)
        out[i] = k;
}

/*
 * This utility routine calculates the number of bits required to store
 * color information.  In general the values are rounded up to an even
 * byte boundary except those cases in which mulitple pixels can evenly
 * into a single byte.
 *
 * The parameter are:
 *   ncomp - The number of components (colorants) for the device.  Valid
 * 	values are 1 to GX_DEVICE_COLOR_MAX_COMPONENTS
 *   bpc - The number of bits per component.  Valid values are 1, 2, 4, 5,
 *	and 8.
 * Input values are not tested for validity.
 */
int
bpc_to_depth(int ncomp, int bpc)
{
    static const byte depths[4][8] = {
	{1, 2, 0, 4, 8, 0, 0, 8},
	{2, 4, 0, 8, 16, 0, 0, 16},
	{4, 8, 0, 16, 16, 0, 0, 24},
	{4, 8, 0, 16, 32, 0, 0, 32}
    };

    if (ncomp <=4 && bpc <= 8)
        return depths[ncomp -1][bpc-1];
    else
    	return (ncomp * bpc + 7) & 0xf8;
}

#define compare_color_names(name, name_size, str, str_size) \
    (name_size == str_size && \
	(strncmp((const char *)name, (const char *)str, name_size) == 0))

/*
 * This routine will check if a name matches any item in a list of process
 * color model colorant names.
 */
static bool
check_process_color_names(fixed_colorant_names_list plist,
			  const gs_param_string * pstring)
{
    if (plist) {
        uint size = pstring->size;
    
	while( *plist) {
	    if (compare_color_names(*plist, strlen(*plist), pstring->data, size)) {
		return true;
	    }
	    plist++;
	}
    }
    return false;
}

/*
 * This routine will check to see if the color component name  match those
 * of either the process color model colorants or the names on the
 * SeparationColorNames list.
 *
 * Parameters:
 *   dev - pointer to device data structure.
 *   pname - pointer to name (zero termination not required)
 *   nlength - length of the name
 *
 * This routine returns a positive value (0 to n) which is the device colorant
 * number if the name is found.  It returns a negative value if not found.
 */
int
check_pcm_and_separation_names(const gx_device * dev,
		const gs_devn_params * pparams, const char * pname,
		int name_size, int component_type)
{
    fixed_colorant_name * pcolor = pparams->std_colorant_names;
    int color_component_number = 0;
    int i;

    /* Check if the component is in the process color model list. */
    if (pcolor) {
	while( *pcolor) {
	    if (compare_color_names(pname, name_size, *pcolor, strlen(*pcolor)))
		return color_component_number;
	    pcolor++;
	    color_component_number++;
	}
    }

    /* Check if the component is in the separation names list. */
    {
	const gs_separations * separations = &pparams->separations;
	int num_spot = separations->num_separations;

	for (i=0; i<num_spot; i++) {
	    if (compare_color_names((const char *)separations->names[i].data,
		  separations->names[i].size, pname, name_size)) {
		return color_component_number;
	    }
	    color_component_number++;
	}
    }

    return -1;
}

/*
 * This routine will check to see if the color component name  match those
 * that are available amoung the current device's color components.  
 *
 * Parameters:
 *   dev - pointer to device data structure.
 *   pname - pointer to name (zero termination not required)
 *   nlength - length of the name
 *   component_type - separation name or not
 *   pdevn_params - pointer to device's DeviceN paramters
 *   pequiv_colors - pointer to equivalent color structure (may be NULL)
 *
 * This routine returns a positive value (0 to n) which is the device colorant
 * number if the name is found.  It returns GX_DEVICE_COLOR_MAX_COMPONENTS if
 * the color component is found but is not being used due to the
 * SeparationOrder device parameter.  It returns a negative value if not found.
 *
 * This routine will also add separations to the device if space is
 * available.
 */
int
devn_get_color_comp_index(gx_device * dev, gs_devn_params * pdevn_params,
		    equivalent_cmyk_color_params * pequiv_colors,
		    const char * pname, int name_size, int component_type,
		    int auto_spot_colors)
{
    int num_order = pdevn_params->num_separation_order_names;
    int color_component_number = 0;
    int max_spot_colors = GX_DEVICE_MAX_SEPARATIONS;

    /*
     * Check if the component is in either the process color model list
     * or in the SeparationNames list.
     */
    color_component_number = check_pcm_and_separation_names(dev, pdevn_params,
					pname, name_size, component_type);

    /* If we have a valid component */
    if (color_component_number >= 0) {
        /* Check if the component is in the separation order map. */
        if (num_order)
	    color_component_number = 
		pdevn_params->separation_order_map[color_component_number];
	else
	    /*
	     * We can have more spot colors than we can image.  We simply
	     * ignore the component (i.e. treat it the same as we would
	     * treat a component that is not in the separation order map).
	     * Note:  Most device do not allow more spot colors than we can
	     * image.  (See the options for auto_spot_color in gdevdevn.h.)
	     */
	    if (color_component_number >= dev->color_info.num_components)
	        color_component_number = GX_DEVICE_COLOR_MAX_COMPONENTS;
	
        return color_component_number;
    }
    /*
     * The given name does not match any of our current components or
     * separations.  Check if we should add the spot color to our list.
     * If the SeparationOrder parameter has been specified then we should
     * already have our complete list of desired spot colorants.
     */
    if (component_type != SEPARATION_NAME ||
	    auto_spot_colors == NO_AUTO_SPOT_COLORS ||
	    pdevn_params->num_separation_order_names != 0)
	return -1;	/* Do not add --> indicate colorant unknown. */
    /*
     * Check if we have room for another spot colorant.
     */
    if (auto_spot_colors == ENABLE_AUTO_SPOT_COLORS)
	max_spot_colors = dev->color_info.num_components -
	    pdevn_params->num_std_colorant_names;
    if (pdevn_params->separations.num_separations < max_spot_colors) {
	byte * sep_name;
	gs_separations * separations = &pdevn_params->separations;
	int sep_num = separations->num_separations++;

	/* We have a new spot colorant */
	sep_name = gs_alloc_bytes(dev->memory,
			name_size, "devn_get_color_comp_index");
	memcpy(sep_name, pname, name_size);
	separations->names[sep_num].size = name_size;
	separations->names[sep_num].data = sep_name;
	color_component_number = sep_num + pdevn_params->num_std_colorant_names;
	if (color_component_number >= dev->color_info.num_components)
	    color_component_number = GX_DEVICE_COLOR_MAX_COMPONENTS;
	else
	    pdevn_params->separation_order_map[color_component_number] =
					       color_component_number;

	if (pequiv_colors != NULL) {
    	    /* Indicate that we need to find equivalent CMYK color. */
	    pequiv_colors->color[sep_num].color_info_valid = false;
	    pequiv_colors->all_color_info_valid = false;
	}
    }

    return color_component_number;
}

#define set_param_array(a, d, s)\
  (a.data = d, a.size = s, a.persistent = false);

/* Get parameters.  We provide a default CRD. */
int
devn_get_params(gx_device * pdev, gs_param_list * plist,
    gs_devn_params * pdevn_params, equivalent_cmyk_color_params * pequiv_colors)
{
    int code;
    bool seprs = false;
    gs_param_string_array scna;
    gs_param_string_array sona;

    set_param_array(scna, NULL, 0);
    set_param_array(sona, NULL, 0);

    if ( (code = sample_device_crd_get_params(pdev, plist, "CRDDefault")) < 0 ||
	 (code =
	    param_write_name_array(plist, "SeparationColorNames", &scna)) < 0 ||
	 (code = param_write_name_array(plist, "SeparationOrder", &sona)) < 0 ||
	 (code = param_write_bool(plist, "Separations", &seprs)) < 0)
	return code;

    return 0;
}
#undef set_param_array

#define BEGIN_ARRAY_PARAM(pread, pname, pa, psize, e)\
    BEGIN\
    switch (code = pread(plist, (param_name = pname), &(pa))) {\
      case 0:\
	if ((pa).size != psize) {\
	  ecode = gs_note_error(gs_error_rangecheck);\
	  (pa).data = 0;	/* mark as not filled */\
	} else
#define END_ARRAY_PARAM(pa, e)\
	goto e;\
      default:\
	ecode = code;\
e:	param_signal_error(plist, param_name, ecode);\
      case 1:\
	(pa).data = 0;		/* mark as not filled */\
    }\
    END

/* 
 * Utility routine for handling DeviceN related parameters.  This routine
 * may modify the color_info, devn_params, and the equiv_cmyk_colors fields.
 *
 * Note:  This routine does not restore values in case of a problem.  This
 * is left to the caller.
 */
int
devn_put_params(gx_device * pdev, gs_param_list * plist,
    gs_devn_params * pdevn_params, equivalent_cmyk_color_params * pequiv_colors)
{
    int code = 0, ecode;
    gs_param_name param_name;
    int npcmcolors = pdevn_params->num_std_colorant_names;
    int num_spot = pdevn_params->separations.num_separations;
    bool num_spot_changed = false;
    int num_order = pdevn_params->num_separation_order_names;
    int max_sep = pdevn_params->max_separations;
    int page_spot_colors = pdevn_params->page_spot_colors;
    gs_param_string_array scna;		/* SeparationColorNames array */
    gs_param_string_array sona;		/* SeparationOrder names array */

    /* Get the SeparationOrder names */
    BEGIN_ARRAY_PARAM(param_read_name_array, "SeparationOrder",
		    			sona, sona.size, sone)
    {
	break;
    } END_ARRAY_PARAM(sona, sone);
    if (sona.data != 0 && sona.size > GX_DEVICE_COLOR_MAX_COMPONENTS)
	return_error(gs_error_rangecheck);

    /* Get the SeparationColorNames */
    BEGIN_ARRAY_PARAM(param_read_name_array, "SeparationColorNames",
		    			scna, scna.size, scne)
    {
	break;
    } END_ARRAY_PARAM(scna, scne);
    if (scna.data != 0 && scna.size > GX_DEVICE_MAX_SEPARATIONS)
	return_error(gs_error_rangecheck);

    /* Separations are only valid with a subrtractive color model */
    if (pdev->color_info.polarity == GX_CINFO_POLARITY_SUBTRACTIVE) {
        /*
         * Process the SeparationColorNames.  Remove any names that already
	 * match the process color model colorant names for the device.
         */
        if (scna.data != 0) {
	    int i;
	    int num_names = scna.size;
	    fixed_colorant_names_list pcomp_names = 
	        pdevn_params->std_colorant_names;

	    for (i = num_spot = 0; i < num_names; i++) {
		/* Verify that the name is not one of our process colorants */
	        if (!check_process_color_names(pcomp_names, &scna.data[i])) {
		    byte * sep_name;
		    int name_size = scna.data[i].size;

		    /* We have a new separation */
		    sep_name = (byte *)gs_alloc_bytes(pdev->memory,
			name_size, "devicen_put_params_no_sep_order");
		    memcpy(sep_name, scna.data[i].data, name_size);
	            pdevn_params->separations.names[num_spot].size = name_size;
	            pdevn_params->separations.names[num_spot].data = sep_name;
		    if (pequiv_colors != NULL) {
			/* Indicate that we need to find equivalent CMYK color. */
			pequiv_colors->color[num_spot].color_info_valid = false;
			pequiv_colors->all_color_info_valid = false;
		    }
		    num_spot++;
		}
	    }
	    pdevn_params->separations.num_separations = num_spot;
	    num_spot_changed = true;
	    for (i = 0; i < num_spot + npcmcolors; i++)
		pdevn_params->separation_order_map[i] = i;
        }
        /*
         * Process the SeparationOrder names.
         */
        if (sona.data != 0) {
	    int i, comp_num;

	    num_order = sona.size;
	    for (i = 0; i < num_spot + npcmcolors; i++)
		pdevn_params->separation_order_map[i] =
		       			GX_DEVICE_COLOR_MAX_COMPONENTS;
	    for (i = 0; i < num_order; i++) {
	        /*
	         * Check if names match either the process color model or
	         * SeparationColorNames.  If not then error.
	         */
	        if ((comp_num = check_pcm_and_separation_names(pdev, pdevn_params,
		    (const char *)sona.data[i].data, sona.data[i].size, 0)) < 0) {
		    return_error(gs_error_rangecheck);
		}
		pdevn_params->separation_order_map[comp_num] = i;
	    }
        }
        /*
         * Adobe says that MaxSeparations is supposed to be 'read only'
	 * however we use this to allow the specification of the maximum
	 * number of separations.  Memory is allocated for the specified
	 * number of separations.  This allows us to then accept separation
	 * colors in color spaces even if they we not specified at the start
	 * of the image file.
         */
        code = param_read_int(plist, param_name = "MaxSeparations", &max_sep);
        switch (code) {
            default:
	        param_signal_error(plist, param_name, code);
            case 1:
		break;
            case 0:
	        if (max_sep < 1 || max_sep > GX_DEVICE_COLOR_MAX_COMPONENTS)
		    return_error(gs_error_rangecheck);
        }
        /*
         * The PDF interpreter scans the resources for pages to try to
	 * determine the number of spot colors.  (Unfortuneately there is
	 * no way to determine the number of spot colors for a PS page
	 * except to interpret the entire page.)  The spot color count for
	 * a PDF page may be high since there may be spot colors in a PDF
	 * page's resources that are not used.  However this does give us
	 * an upper limit on the number of spot colors.  A value of -1
	 * indicates that the number of spot colors in unknown (a PS file).
         */
        code = param_read_int(plist, param_name = "PageSpotColors",
		       					&page_spot_colors);
        switch (code) {
            default:
	        param_signal_error(plist, param_name, code);
            case 1:
		break;
            case 0:
	        if (page_spot_colors < -1)
		    return_error(gs_error_rangecheck);
	        if (page_spot_colors > GX_DEVICE_COLOR_MAX_COMPONENTS)
		    page_spot_colors = GX_DEVICE_COLOR_MAX_COMPONENTS;
        }
        /* 
         * The DeviceN device can have zero components if nothing has been
	 * specified.  This causes some problems so force at least one
	 * component until something is specified.
         */
        if (!pdev->color_info.num_components)
	    pdev->color_info.num_components = 1;
	/*
	 * Update the number of device components if we have changes in
	 * SeparationColorNames, SeparationOrder, or MaxSeparations.
	 */
	if (num_spot_changed || pdevn_params->max_separations != max_sep ||
	    	    pdevn_params->num_separation_order_names != num_order ||
		    pdevn_params->page_spot_colors != page_spot_colors) {
	    pdevn_params->separations.num_separations = num_spot;
	    pdevn_params->num_separation_order_names = num_order;
    	    pdevn_params->max_separations = max_sep;
    	    pdevn_params->page_spot_colors = page_spot_colors;
	    if (max_sep != 0)
		 pdev->color_info.max_components = max_sep;
	    /*
	     * If we have SeparationOrder specified then the number of
	     * components is given by the number of names in the list.
	     * Otherwise check if the MaxSeparations parameter has specified
	     * a value.  If so then use that value, otherwise use the number
	     * of ProcessColorModel components plus the number of
	     * SeparationColorNames is used.
	     */
            pdev->color_info.num_components = (num_order)
		? num_order 
		: (pdevn_params->max_separations)
			? pdevn_params->max_separations
			: (page_spot_colors >= 0)
		       		? npcmcolors + num_spot + page_spot_colors
	    			: pdev->color_info.max_components;
            if (pdev->color_info.num_components >
		    pdev->color_info.max_components)
                pdev->color_info.num_components =
		       	pdev->color_info.max_components;
#if !USE_COMPRESSED_ENCODING
	    /*
	     * See earlier comment about the depth and non compressed
	     * pixel encoding.
	     */
            pdev->color_info.depth = bpc_to_depth(pdev->color_info.num_components, 
					pdevn_params->bitspercomponent);
#endif
	}
    }
    return code;
}

/*
 * Utility routine for handling DeviceN related parameters in a
 * standard raster printer type device.
 */
int
devn_printer_put_params(gx_device * pdev, gs_param_list * plist,
    gs_devn_params * pdevn_params, equivalent_cmyk_color_params * pequiv_colors)
{
    int code;
    /* Save current data in case we have a problem */
    gx_device_color_info save_info = pdev->color_info;
    gs_devn_params saved_devn_params = *pdevn_params;
    equivalent_cmyk_color_params saved_equiv_colors;

    if (pequiv_colors != NULL)
        saved_equiv_colors = *pequiv_colors;

    /* Use utility routine to handle parameters */
    code = devn_put_params(pdev, plist, pdevn_params, pequiv_colors);

    /* Check for default printer parameters */
    if (code >= 0)
        code = gdev_prn_put_params(pdev, plist);

    /* If we have an error then restore original data. */
    if (code < 0) {
	pdev->color_info = save_info;
	*pdevn_params = saved_devn_params;
	if (pequiv_colors != NULL)
	   *pequiv_colors = saved_equiv_colors;
	return code;
    }

    /* If anything changed, then close the device, etc. */
    if (memcmp(&pdev->color_info, &save_info, sizeof(gx_device_color_info)) ||
	memcmp(pdevn_params, &saved_devn_params,
					sizeof(gs_devn_params)) ||
	(pequiv_colors != NULL &&
	    memcmp(pequiv_colors, &saved_equiv_colors,
				sizeof(equivalent_cmyk_color_params)))) {
	gs_closedevice(pdev);
        /* Reset the sparable and linear shift, masks, bits. */
	set_linear_color_bits_mask_shift(pdev);
    }
    /*
     * Also check for parameters which are being passed from the PDF 1.4
     * compositior clist write device.  This device needs to pass info
     * to the PDF 1.4 compositor clist reader device.  However this device
     * is not crated until the clist is being read.  Thus we have to buffer
     * this info in the output device.   (This is only needed for devices
     * which support spot colors.)
     */
    code = pdf14_put_devn_params(pdev, pdevn_params, plist);
    return code;
}

/*
 * The following routines are for compressing colorant values into a 64 bit
 * gx_color_index value.  This is needed since Ghostscript uses an integer type
 * (usually 64 bit long long) as the representation for a pixel.  This is a
 * problem for handling output devices which support spot colors.  Ideally these
 * devices should be able to handle any number of colorants.  This would require
 * an arbitrarily large number of bits to represent a pixel.
 *
 * See comments before devn_encode_compressed_color for more information.
 */

/* GC procedures */
static 
ENUM_PTRS_WITH(compressed_color_list_enum_ptrs, compressed_color_list_t *plist)
{
    if (index < plist->num_sub_level_ptrs)
	ENUM_RETURN(plist->u.sub_level_ptrs[index]);
    return 0;
}
ENUM_PTRS_END

static RELOC_PTRS_WITH(compressed_color_list_reloc_ptrs, compressed_color_list_t *plist)
{
    int i;

    for (i = 0; i < plist->num_sub_level_ptrs; i++) {
	RELOC_PTR(compressed_color_list_t, u.sub_level_ptrs[i]);
    }
}
RELOC_PTRS_END

gs_private_st_composite(st_compressed_color_list, compressed_color_list_t,
		"encode color list", compressed_color_list_enum_ptrs,
    		compressed_color_list_reloc_ptrs);
/*
 * A routine for debugging the encoded color colorant list.  This routine
 * dumps the contents of the list.
 */
void
print_compressed_color_list(compressed_color_list_t * pcomp_list, int num_comp)
{
    int i, j, comp_num, comp;
    comp_bit_map_list_t * pcomp_bit_map;

    if (pcomp_list == NULL)
	return;

    /* Indent our print out for sub levels */
    for (i = TOP_ENCODED_LEVEL - pcomp_list->level_num_comp; i > 0; i--)
	dlprintf("    ");
    dlprintf1("List level = %d\n", pcomp_list->level_num_comp);
    /*
     * Print the colorant bit maps for this level.
     */
    for (i = NUM_ENCODE_LIST_ITEMS - 1; i >= pcomp_list->first_bit_map; i--) {
	pcomp_bit_map = &(pcomp_list->u.comp_data[i]);
        /* Indent our print out for sub levels */
        for (j = TOP_ENCODED_LEVEL - pcomp_list->level_num_comp; j > 0; j--)
	    dlprintf("    ");
        dlprintf4("%3d%4d%4d %d ", i, pcomp_bit_map->num_comp,
		pcomp_bit_map->num_non_solid_comp, pcomp_bit_map->solid_not_100);
	for (comp_num = num_comp - 1; comp_num >= 0; comp_num--) {
	    comp = colorant_present(pcomp_bit_map, colorants, comp_num);
            dlprintf1("%d", comp);
	    if ((comp_num & 7) == 0)	/* Separate into groups of 8 bits */
                dlprintf(" ");
	}
        dlprintf("    ");
	for (comp_num = num_comp - 1; comp_num >= 0; comp_num--) {
	    comp = colorant_present(pcomp_bit_map, solid_colorants, comp_num);
            dlprintf1("%d", comp);
	    if ((comp_num & 7) == 0)	/* Separate into groups of 8 bits */
                dlprintf(" ");
	}
        dlprintf("\n");
    }

    /*
     * Print the sub levels.
     */
    for (i = 0; i < pcomp_list->num_sub_level_ptrs; i++)
       print_compressed_color_list(pcomp_list->u.sub_level_ptrs[i], num_comp);

    return;
}

/*
 * Allocate an list level element for our encode color list.
 */
compressed_color_list_t * 
alloc_compressed_color_list_elem(gs_memory_t * mem, int num_comps)
{
    compressed_color_list_t * plist =
        gs_alloc_struct(mem, compressed_color_list_t, &st_compressed_color_list,
			       "alloc_compressed_color_list");
    if (plist != NULL) {
	/* Initialize the data in the element. */
	memset(plist, 0, size_of(*plist));
	plist->level_num_comp = num_comps;
	plist->first_bit_map = NUM_ENCODE_LIST_ITEMS;
    }
    return plist;
}

/*
 * Free the elements of a compressed color list.
 */
void
free_compressed_color_list(gs_memory_t * mem,
	       	compressed_color_list_t * pcomp_list)
{
    int i;

    if (pcomp_list == NULL)
	return;

    /* Discard the sub levels. */
    for (i = 0; i < pcomp_list->num_sub_level_ptrs; i++)
       free_compressed_color_list(mem, pcomp_list->u.sub_level_ptrs[i]);

    gs_free_object(mem, pcomp_list, "free_compressed_color_list");
    return;
}

/*
 * Free a set of separation names
 */
void
free_separation_names(gs_memory_t * mem,
	       	gs_separations * pseparation)
{
    int i;

    /* Discard the sub levels. */
    for (i = 0; i < pseparation->num_separations; i++)
        gs_free_object(mem, pseparation->names[i].data,
				"free_separation_names");
    pseparation->num_separations = 0;
    return;
}

/*
 * Add a new set of bit mapped colorant lists to our list of encoded color
 * colorants.
 */
static bool
sub_level_add_compressed_color_list(gs_memory_t * mem,
		comp_bit_map_list_t * pnew_comp_bit_map,
		compressed_color_list_t * pcomp_list, gx_color_index * plist_index)
{
    int i, entry_num;
    int num_non_solid_comp = pnew_comp_bit_map->num_non_solid_comp;
    bool status;

    /*
     * Check if this is the level for the specified number of entries.  If so
     * then add the bit map to this level (if we have room).
     */
    if (num_non_solid_comp >= pcomp_list->level_num_comp) {
	entry_num = pcomp_list->first_bit_map - 1;

	if (entry_num > pcomp_list->num_sub_level_ptrs) {
	    memcpy(&(pcomp_list->u.comp_data[entry_num]), pnew_comp_bit_map,
				    size_of(comp_bit_map_list_t));
	    pcomp_list->first_bit_map = entry_num;
	    *plist_index =
		    ((gx_color_index) entry_num) << (NUM_GX_COLOR_INDEX_BITS - 8);
	    return true;
	}
	return false;
    }
    /*
     * Try to insert the bit map into the sub levels.
     */
    for (i = 0; i < pcomp_list->num_sub_level_ptrs; i++) {
	status = sub_level_add_compressed_color_list(mem, pnew_comp_bit_map,
			pcomp_list->u.sub_level_ptrs[i], plist_index);
	if (status) {
	    *plist_index = (((gx_color_index) i) << (NUM_GX_COLOR_INDEX_BITS - 8)) 
		    + (*plist_index >> 8);
	    return true;
	}
    }
    /*
     * If we did not add this bit map into a sub level then create a new sub
     * level and insert it there.
     */
    entry_num = pcomp_list->num_sub_level_ptrs;
    if (entry_num < pcomp_list->first_bit_map) {
	pcomp_list->u.sub_level_ptrs[entry_num] =
	    alloc_compressed_color_list_elem(mem, pcomp_list->level_num_comp - 1);
	if (pcomp_list->u.sub_level_ptrs[entry_num] != NULL) {
	    pcomp_list->num_sub_level_ptrs++;
	    status = sub_level_add_compressed_color_list(mem, pnew_comp_bit_map,
			pcomp_list->u.sub_level_ptrs[entry_num], plist_index);
	    if (status) {
		*plist_index = (((gx_color_index) i) << (NUM_GX_COLOR_INDEX_BITS - 8)) 
		    + (*plist_index >> 8);
		return true;
	    }
	}
    }
    /*
     * If we get to here then there was no space available in this list element.
     */
    return false;
}

/*
 * Add a new bit mapped colorant list to our list of encoded color colorants.
 *
 * Our simple linear search for entries gets very inefficient if we have many
 * entries.  So we are doing two things to minimize the number of entries.
 * We need separate entries for each combination of solid colorants.  if we
 * do not have many non solid colorants, we use non solid colorants even for
 * solid colorants.  For small numbers of colorants, we add more colorants
 * to try to create an entry that can be used for more situations.  We add extra
 * process color colorants since these are the ones most likely to be mixed
 * with spot colors.
 */
static bool
add_compressed_color_list(gs_memory_t * mem,
	comp_bit_map_list_t * pnew_comp_bit_map,
	compressed_color_list_t * pcomp_list, gx_color_index * plist_index)
{
    int num_comp = pnew_comp_bit_map->num_comp;
    int num_non_solid = pnew_comp_bit_map->num_non_solid_comp;
    int num_solid = num_comp - num_non_solid;
    int comp_num = 0;

    /*
     * If we have room for more 'non solid' colorants then convert some of
     * the solid colorants to using the non solid encodings.
     */
    while (num_non_solid < MIN_ENCODED_COMPONENTS && num_solid > 0) {
	if (colorant_present(pnew_comp_bit_map, solid_colorants, comp_num)) {
	    clear_colorant_present(pnew_comp_bit_map,
			    		solid_colorants, comp_num);
	    num_solid--;
	    num_non_solid++;
	}
	comp_num++;
    }
    if (num_non_solid < MIN_ENCODED_COMPONENTS) {
        /*
	 * For small numbers of colorants, we add more colorants to try to
	 * create an entry that can be used for more situations.
         */
	for (comp_num = 0; num_comp < MIN_ENCODED_COMPONENTS; comp_num++) {
	    if ((colorant_present(pnew_comp_bit_map, colorants, comp_num)) == 0) {
	        set_colorant_present(pnew_comp_bit_map, colorants, comp_num);
		num_non_solid++;
		num_comp++;
	    }
	}
    }
    pnew_comp_bit_map->num_comp = num_comp;
    pnew_comp_bit_map->num_non_solid_comp = num_non_solid;
    return sub_level_add_compressed_color_list(mem, pnew_comp_bit_map,
				pcomp_list, plist_index);
}

/*
 * Initialize our encode color list.  When we initialize the list, we add two
 * initial colorant maps.  The first one is good for any image that uses zeven
 * or fewer colorants.  The second is good for any image which uses seven spot
 * colors (or less) and no process colors.  These are placed at the start of
 * the list to minimize the add and search times for these common situations.
 */
static compressed_color_list_t * 
init_compressed_color_list(gs_memory_t *mem)
{
    /*
     * Create our first list element.
     */
    compressed_color_list_t * plist =
	alloc_compressed_color_list_elem(mem, TOP_ENCODED_LEVEL);

    /*
     * Add a first colorant bit map to the list.  This bit map covers the first
     * TOP_ENCODED_LEVEL colorants.  Typically this covers CMYK plus the
     * first three spot colors.  This bit map should handle many situations.
     */
    if (plist != NULL) {
	int comp_num;
        comp_bit_map_list_t comp_bit_map;
	gx_color_index temp;

        /*
         * Add a first colorant bit map to the list.  This bit map covers the
	 * first TOP_ENCODED_LEVEL colorants.  Typically this covers CMYK plus
	 * the first three spot colors.  This bit map should handle many
	 * situations.
         */
	memset(&comp_bit_map, 0, size_of(comp_bit_map));
	for (comp_num = 0; comp_num < TOP_ENCODED_LEVEL; comp_num++)
	    set_colorant_present(&comp_bit_map, colorants, comp_num);
	comp_bit_map.num_comp =
		comp_bit_map.num_non_solid_comp = TOP_ENCODED_LEVEL;
	add_compressed_color_list(mem, &comp_bit_map, plist, &temp);
        /*
         * Add a second colorant bit map to the list.  This bit map covers the
	 * first TOP_ENCODED_LEVEL colorants after the first four colorants.
	 * Typically this covers the first seven spot colors.  This bit map is
	 * being placed to cover images that use only spot colors.
         */
	memset(&comp_bit_map, 0, size_of(comp_bit_map));
	for (comp_num = 4; comp_num < TOP_ENCODED_LEVEL + 4; comp_num++)
	    set_colorant_present(&comp_bit_map, colorants, comp_num);
	comp_bit_map.num_comp =
		comp_bit_map.num_non_solid_comp = TOP_ENCODED_LEVEL;
	add_compressed_color_list(mem, &comp_bit_map, plist, &temp);
    }
    return plist;
}

/*
 * For most combinations of colorants we use 8 bits for saving the colorant
 * value.  However if we get above 7 colorants (in a pixel, not total) we use
 * fewer bits.  The constraint is that the size of the index value plus the
 * the number of colorants being used times size of the colorant value saved
 * must fit into a gx_color_index value.
 */
int num_comp_bits[MAX_ENCODED_COMPONENTS + 1] = {
	8,	/* 0 colorants - not used */
	8,	/* 1 colorants */
	8,	/* 2 colorants */
	8,	/* 3 colorants */
	8,	/* 4 colorants */
	8,	/* 5 colorants */
	8,	/* 6 colorants */
	8,	/* 7 colorants */
	7,	/* 8 colorants */
	6,	/* 9 colorants */
	5,	/* 10 colorants */
	5,	/* 11 colorants */
	4,	/* 12 colorants */
	4,	/* 13 colorants */
	4	/* 14 colorants */
};

/*
 * Values used to decompressed the colorants in our encoded values back into
 * a gx_color value.  The color value will be (comp_bits * entry) >> 8
 * The number of bits in comp_bits are defined in the num_comp_bits table.
 * These values are chosen to expand these bit combinations back to 16 bit values
 * (after shifting right 8 bits).
 */
#define gx_color_value_factor(num_bits) \
    ((gx_max_color_value << 8) + 0xff) / ((1 << num_bits) - 1)

int comp_bit_factor[MAX_ENCODED_COMPONENTS + 1] = {
	gx_color_value_factor(8),		 /*  0 colorants (8 bits) */
	gx_color_value_factor(8),		 /*  1 colorants (8 bits) */
	gx_color_value_factor(8),		 /*  2 colorants (8 bits) */
	gx_color_value_factor(8),		 /*  3 colorants (8 bits) */
	gx_color_value_factor(8),		 /*  4 colorants (8 bits) */
	gx_color_value_factor(8),		 /*  5 colorants (8 bits) */
	gx_color_value_factor(8),		 /*  6 colorants (8 bits) */
	gx_color_value_factor(8),		 /*  7 colorants (8 bits) */
	gx_color_value_factor(7),		 /*  8 colorants (7 bits) */
	gx_color_value_factor(6),		 /*  9 colorants (6 bits) */
	gx_color_value_factor(5),		 /* 10 colorants (5 bits) */
	gx_color_value_factor(5),		 /* 11 colorants (5 bits) */
	gx_color_value_factor(4),		 /* 12 colorants (4 bits) */
	gx_color_value_factor(4),		 /* 13 colorants (4 bits) */
	gx_color_value_factor(4)		 /* 14 colorants (4 bits) */
};
#undef gx_color_value_factor

/*
 * Find a given colorant bit map is the list of encoded colorant bit map.
 *
 * Note:  This routine is called recursively to search sub levels of the
 * list.
 *
 * The parameters are:
 *    num_comp - The number of colorants for the device.
 *    pcomp_list - The current list of encoded colorants.
 *    pnew_comp_bit_map - Pointer to the bit map found to be encoded.
 *    plist_index - Pointer to 'encode bits' (return value)
 *    pcomp_bit_map - Pointer to pointer to the actual bit map found
 *    			(return value).
 *    returns true if the bit map is found.
 */
static bool
search_compressed_color_list(int num_comp, compressed_color_list_t * pcomp_list,
	comp_bit_map_list_t * pnew_comp_bit_map, gx_color_index * plist_index,
	comp_bit_map_list_t * * pcomp_bit_map)
{
    int i;
#if DEVN_ENCODE_COLOR_USING_BIT_MAP_ARRAY
    int j, num_bit_map_elem;
#endif
    bool found;

    /*
     * Search the colorant bit maps for this level of the map.
     */
#if DEVN_ENCODE_COLOR_USING_BIT_MAP_ARRAY
    num_bit_map_elem = (num_comp + BITS_PER_COMP_BIT_MAP_ELEM - 1) /
	    					BITS_PER_COMP_BIT_MAP_ELEM;
#endif
    for (i = NUM_ENCODE_LIST_ITEMS - 1; i >= pcomp_list->first_bit_map; i--) {
	*pcomp_bit_map = &(pcomp_list->u.comp_data[i]);
	/*
	 * Do not try to match if one entry uses a 'solid' set of colorants
	 * that is not really solid (i.e. not 100%) and the other is.  It is
	 * possible to work if different but it would make some of the logic
	 * more difficult.
	 */
	if (pnew_comp_bit_map->solid_not_100 !=
			(*pcomp_bit_map)->solid_not_100)
	    continue;
        /*
         * It is a match if the new colorant bit map is a subset of the one
	 * in the list and the solid colorants for new map is a super set of
	 * the solid colorants for the one in the list.  I.e. we can use
	 * the non solid part of the entry for either zero or solid colorants.
         */
#if DEVN_ENCODE_COLOR_USING_BIT_MAP_ARRAY
	for (j = 0; j < num_bit_map_elem; j++) {
	    if ((pnew_comp_bit_map->colorants[j] &
		    (*pcomp_bit_map)->colorants[j]) !=
	   		pnew_comp_bit_map->colorants[j])
		break;		/* No match if a colorant is missing. */
	    if ((pnew_comp_bit_map->solid_colorants[j] &
		    (*pcomp_bit_map)->solid_colorants[j]) !=
	   		(*pcomp_bit_map)->solid_colorants[j])
		break;		/* No match if extra solid colorants */
	}
	if (j == num_bit_map_elem) {
#else
	if (((pnew_comp_bit_map->colorants &
		(*pcomp_bit_map)->colorants) ==
		    pnew_comp_bit_map->colorants) &&
		((pnew_comp_bit_map->solid_colorants &
	     	    (*pcomp_bit_map)->solid_colorants) ==
		    (*pcomp_bit_map)->solid_colorants)) {
#endif
	    /*
	     * To prevent possible loss of accuracy, ignore matches in which the
	     * packing will use fewer bits in the encoded colorant values than
	     * is possible for the given number of colorants.
	     */
	    if (num_comp_bits[pnew_comp_bit_map->num_comp] > 
	    		num_comp_bits[(*pcomp_bit_map)->num_comp])
		break;
	    /*
	     * We have a match.  Put our object number into the top eight
	     * bits of the encoded gx_color_index and exit.
	     */
	    *plist_index = ((gx_color_index) i) << (NUM_GX_COLOR_INDEX_BITS - 8);
	    return true;
	}
    }

    /*
     * Search the lower levels (i.e. with fewer colorants to see if we
     * can find a match.
     */
    if (pcomp_list->level_num_comp <= pnew_comp_bit_map->num_non_solid_comp)
	return false;	/* Exit if not enough colorants in the sub levels */

    for (i = 0; i < pcomp_list->num_sub_level_ptrs; i++) {
	found = search_compressed_color_list(num_comp,
			pcomp_list->u.sub_level_ptrs[i],
			pnew_comp_bit_map, plist_index, pcomp_bit_map);
	if (found) {
	    /*
	     * We have a match.  Combine the encode index for the sub level
	     * with our index for this level.
	     */
	    *plist_index = (((gx_color_index) i) << (NUM_GX_COLOR_INDEX_BITS - 8)) 
		    + (*plist_index >> 8);
	    return true;
	}
    }
    return false;
}

/*
 * Encode a list of colorant values into a gx_color_index_value.
 *
 * This routine is designed to pack more than eight 8 bit colorant values into
 * a 64 bit gx_color_index value.  It does this piece of magic by keeping a list
 * of which colorant combinations are actualy used (i.e. which colorants are non
 * zero).  The non zero colorant values and and an 'index' value are packed into
 * the output gx_color_index value.
 *
 * The the different combinations of used colorants are saved into a table
 * defined by the comp_bit_map_list_t structure type.  This table is kept as
 * a list with 256 elements.  Each element can be either a pair of bit maps which
 * indicates the combination of colorants being used or a pointer to a sub list
 * for the next lower level of combinations.  There are two bit maps to indicate
 * which colorants are specified by the 'index' value.  One bit map indicates
 * which colorants are used.  The second bit map is used to indicate a group of
 * colorants with the same value.  Normally this second bit map is used to
 * indicate which colorants are 'solid' (i.e. 100% and the 'solid_not_100 flag
 * is set to false).  However if there is a larger group of colorants with the
 * same value (and not solid) then the 'solid_not_100' flag is set to true and
 * second bit map is used to indicate the colorants in this group.  In this
 * case, the value of the colorant group is stored in the first colorant entry
 * in the gx_color_index.
 *
 * The number of bits allocated to storing the 'index' and the number of bits
 * allocated to storing colorant values depends upon the number of colorant
 * being used.
 *
 * 	Number of non zero colorant	Index bits	Bits per colorant
 *		0 to 5			   24		   8
 *		   6			   16		   8
 *		   7			    8		   8
 *		   8	       		    8		   7
 *		   9			    8		   6
 *		  10			    8		   5
 *		  11			    8		   5
 *		  12			    8		   4
 *		  13			    8		   4
 *		  14			    8		   4
 *	    More than 14		Not encodeable
 *
 * The 'index' bits can be logically divided into groups of 8 bits.  The
 * first (upper) group of 8 bits is used to select either one of 256
 * combinations of 7 or more colorant or to select a pointer to a sub
 * level.  If a sub level pointer is specified, then the next group of 8
 * index bits is used to select either one of 256 combinations of 6 colorants
 * of a sub level pointer.  A sub level pointer points to one of 256
 * combinations of 5 colorants.  If we have fewer than 5 colorants being
 * used, we add extra componnents to bring the total up to 5 colorants.
 * This is done to prevent having a bunch of 1 or two colorant combinations.
 */
gx_color_index
devn_encode_compressed_color(gx_device *pdev, const gx_color_value colors[],
					gs_devn_params * pdevn_params)
{
    int num_comp = pdev->color_info.num_components;
    int comp_num, comp_count = 0, solid_comp_count = 0, bit_pos = 0;
    int bit_shift, bit_count, group = 0;
    int color_resolution = gx_max_color_value / STD_ENCODED_VALUE;
    bool found, added;
    comp_bit_map_list_t new_comp_bit_map = {0};
    comp_bit_map_list_t * pbit_map;
    gx_color_index color = 0, list_index;

    /*
     * Determine what colorants are being used (non zero).  We bit pack
     * this info.  Note:  We treat any colorant value which is less than
     * 256 as zero.  Color values are 16 bits and we only keep the top
     * eight bits.  Likewise for solid (100%) colors.
     */
    for (comp_num = 0; comp_num < num_comp; comp_num++) {
        if (colors[comp_num] > color_resolution) {
	    set_colorant_present(&new_comp_bit_map, colorants, comp_num);
	    comp_count++;
	    /* Check if the color is solid */
            if (colors[comp_num] > (gx_max_color_value - color_resolution)) {
	        set_colorant_present(&new_comp_bit_map,
						solid_colorants, comp_num);
	        solid_comp_count++;
	    }
	}
    }
    new_comp_bit_map.num_comp = comp_count;
    new_comp_bit_map.num_non_solid_comp = comp_count - solid_comp_count;
    /*
     * We may get less loss of accuracy if instead of checking for zero and
     * 100% colorant values, we look for a group of colorants with the same
     * colorant value.
     */
    if (new_comp_bit_map.num_non_solid_comp > TOP_ENCODED_LEVEL &&
		    solid_comp_count < (comp_count / 2)) {
	short group_size[(gx_max_color_value / STD_ENCODED_VALUE) + 1] = {0};
	int value, largest_group_size = 0;

	/* Scan to determine the size of the largest group */
        for (comp_num = 0; comp_num < num_comp; comp_num++) {
            value = colors[comp_num] / STD_ENCODED_VALUE;
	    group_size[value]++;
	    if (group_size[value] > largest_group_size) {
		largest_group_size = group_size[value];
		group = value;
	    }
        }
	/*
	 * If using this group instead of the solid colorants will improve
	 * our situation, then switch to using this group.
	 */
	if (largest_group_size > (solid_comp_count + 1) && 
		(comp_count - largest_group_size) < MAX_ENCODED_COMPONENTS) {
	    /* Setup the colorant description to use this group */
	    memset(&(new_comp_bit_map.solid_colorants), 0,
			    		size_of(comp_bit_map_t));
            for (comp_num = 0; comp_num < num_comp; comp_num++) {
                value = colors[comp_num] / STD_ENCODED_VALUE;
	        if (value == group) {
	            set_colorant_present(&new_comp_bit_map,
						solid_colorants, comp_num);
	        }
            }
	    new_comp_bit_map.solid_not_100 = true;
	    new_comp_bit_map.num_non_solid_comp =
		    			comp_count - largest_group_size + 1;
	}
    }

    /* Our encoding scheme cannot handle too many non solid colorants. */
    if (new_comp_bit_map.num_non_solid_comp > MAX_ENCODED_COMPONENTS)
	return NON_ENCODEABLE_COLOR;

    /*
     * We keep a list of which colorant combinations we have used.  Make
     * sure that this list has been initialized.
     */
    if (pdevn_params->compressed_color_list == NULL) {
        pdevn_params->compressed_color_list =
		init_compressed_color_list(pdev->memory);
        if (pdevn_params->compressed_color_list == NULL)
	    return NON_ENCODEABLE_COLOR;	/* Unable to initialize list */
    }

    /*
     * Check our list of colorant combinations to see if we already have a
     * combination that is useable.  I.e. a combination that includes all of our
     * non zero colorants.
     */
    found = search_compressed_color_list(num_comp,
	    		pdevn_params->compressed_color_list,
	    		&new_comp_bit_map, &list_index, &pbit_map);

    /*
     * If our new colorant list was not found then add it to our encode color
     * list.
     */
    if (!found) {
	added = add_compressed_color_list(pdev->memory, &new_comp_bit_map,
				pdevn_params->compressed_color_list, &list_index);
	if (!added)
	    return NON_ENCODEABLE_COLOR;
	pbit_map = &new_comp_bit_map;
    }
    
    /*
     * Form the encoded color gx_color_index value.  This is a combination
     * of the bits that encode which colorants are used (non zero) and the
     * colorant values.
     */
    bit_count = num_comp_bits[pbit_map->num_non_solid_comp];
    bit_shift = sizeof(gx_color_value) * 8 - bit_count;
    if (pbit_map->solid_not_100) {
	color = group >> (8 - bit_count);
	bit_pos += bit_count;
    }
    for (comp_num = 0; comp_num < num_comp; comp_num++) {
	if (colorant_present(pbit_map, colorants, comp_num) &&
	   !colorant_present(pbit_map, solid_colorants, comp_num)) {
	    color |=
		((gx_color_index)(colors[comp_num] >> bit_shift)) << bit_pos;
	    bit_pos += bit_count;
	}
    }
    color |= list_index;
    /*
     * Make sure that our color index does not match one of the reserved
     * values.
     */
    if (color == NON_ENCODEABLE_COLOR)
	color -= 1;
    else if (color == gx_no_color_index)
	color -= 2;
    return color;
}

/*
 * Find the bit map for given bit map index.
 */
comp_bit_map_list_t *
find_bit_map(gx_color_index index, compressed_color_list_t * pcomp_list)
{
    int loc = (int)(index >> (NUM_GX_COLOR_INDEX_BITS - 8));

    /*
     * Search for the level which contains the bit map.  If our index
     * for this level is less than the number of sub level pointers for
     * this level then we need to go down another level.
     */
    while (loc < pcomp_list->num_sub_level_ptrs) {
	pcomp_list = pcomp_list->u.sub_level_ptrs[loc];
	index <<= 8;
	loc = (int)(index >> (NUM_GX_COLOR_INDEX_BITS - 8));
    }
    return &(pcomp_list->u.comp_data[loc]);
}

/*
 * Decode a gx_color_index value back to a list of colorant values.  This
 * routine assumes that the gx_color_index value is 'encoded' as described
 * for devn_encode_compressed_color.
 *
 * See comments preceding devn_encode_compressed_color for more information
 * about the way that we are compressing colorant values in a gx_color_index.
 */
int
devn_decode_compressed_color(gx_device * dev, gx_color_index color,
			gx_color_value * out, gs_devn_params * pdevn_params)
{
    int comp_num = 0;
    int factor, bit_count, bit_mask;
    int ncomp = dev->color_info.num_components;
    comp_bit_map_list_t * pbitmap;
    gx_color_value solid_color = gx_max_color_value;

    /*
     * Set all colorants to max if we get a non encodeable color.  We set the
     * values to a max since this will represent another non encodeable color.
     * Thus if we have a non decodable color, it will continue to propogate.
     */
    if (color == NON_ENCODEABLE_COLOR) {
        for (; comp_num < ncomp; comp_num++)
            out[comp_num] = gx_max_color_value;
        return 0;
    }
    pbitmap = find_bit_map(color, pdevn_params->compressed_color_list);
    bit_count = num_comp_bits[pbitmap->num_non_solid_comp];
    bit_mask = (1 << bit_count) - 1;
    factor = comp_bit_factor[pbitmap->num_non_solid_comp];
    if (pbitmap->solid_not_100) {
	solid_color = (factor * ((int)color & bit_mask)) >> 8;
	color >>= bit_count;
    }
    for (; comp_num < ncomp; comp_num++) {
	if (colorant_present(pbitmap, colorants, comp_num)) {
	    if (colorant_present(pbitmap, solid_colorants, comp_num))
                out[comp_num] = solid_color;
	    else {
                out[comp_num] = (factor * ((int)color & bit_mask)) >> 8;
	        color >>= bit_count;
	    }
	}
	else
            out[comp_num] = 0;
    }
    return 0;
}


/*
 * Unpack a row of 'compressed color' values.  These values are encoded as
 * described for the devn_encode_compressed_color routine.
 *
 * The routine takes a raster line of data and expands each pixel into a buffer
 * of 8 bit values for each colorant.
 *
 * See comments preceding devn_encode_compressed_color for more information
 * about the way that we are encoding colorant values in a gx_color_index.
 *
 * Note: For simplicity of coding the calling routines, this routine will also
 * handle 'uncompressed' bit maps.
 */
int
devn_unpack_row(gx_device * dev, int num_comp, gs_devn_params * pdevn_params,
					 int width, byte * in, byte * out)
{
    int i, comp_num, pixel_num;

    if (pdevn_params->compressed_color_list == NULL) {
        int bytes_pp = dev->color_info.depth >> 3;

        /*
         * For 'uncompressed' data, the number of bytes per pixel in the input
         * raster line is defined by the device depth.  This may be more than
         * the number of actual device components.
         */
        for (pixel_num = 0; pixel_num < width; pixel_num++) {
	    for (i = 0; i < num_comp; i++)
	        *out++ = *in++;
	    in += bytes_pp - num_comp;
        }
        return 0;
    }
    else {
        int non_encodeable_count = 0;
        int factor, bit_count, bit_mask;
        comp_bit_map_list_t * pbitmap;
        gx_color_index color;
        gx_color_value solid_color = gx_max_color_value;

        for (pixel_num = 0; pixel_num < width; pixel_num++) {
	    /*
	     * Get the encoded color value.
	     */
	    color = ((gx_color_index)(*in++)) << (NUM_GX_COLOR_INDEX_BITS - 8);
	    for (i = NUM_GX_COLOR_INDEX_BITS - 16; i >= 0; i -= 8)
	        color |= ((gx_color_index)(*in++)) << i;
            /*
             * Set all colorants to zero if we get a non encodeable color.
             */
            if (color == NON_ENCODEABLE_COLOR) {
                for (comp_num = 0; comp_num < num_comp; comp_num++)
                    *out++ = 0;
	        non_encodeable_count++;
            }
	    else {
    	        pbitmap = find_bit_map(color,
			       	pdevn_params->compressed_color_list);
	        bit_count = num_comp_bits[pbitmap->num_non_solid_comp];
	        bit_mask = (1 << bit_count) - 1;
	        factor = comp_bit_factor[pbitmap->num_non_solid_comp];
	        if (pbitmap->solid_not_100) {
		    solid_color = (factor * ((int)color & bit_mask)) >> 8;
		    color >>= bit_count;
	        }
                for (comp_num = 0; comp_num < num_comp; comp_num++) {
		    if (colorant_present(pbitmap, colorants, comp_num)) {
		        if (colorant_present(pbitmap,
					       	solid_colorants, comp_num))
       		            *out++ = solid_color >> 8;
		        else {
            	            *out++ = (factor * ((int)color & bit_mask)) >> 16;
	    	            color >>= bit_count;
		        }
		    }
		    else
            	        *out++ = 0;
       	        }
            }
        }
        return non_encodeable_count;
    }
}


/* ***************** The spotcmyk and devicen devices ***************** */

/* Define the device parameters. */
#ifndef X_DPI
#  define X_DPI 72
#endif
#ifndef Y_DPI
#  define Y_DPI 72
#endif

/* The device descriptor */
static dev_proc_open_device(spotcmyk_prn_open);
static dev_proc_get_params(spotcmyk_get_params);
static dev_proc_put_params(spotcmyk_put_params);
static dev_proc_print_page(spotcmyk_print_page);
static dev_proc_get_color_mapping_procs(get_spotcmyk_color_mapping_procs);
static dev_proc_get_color_mapping_procs(get_devicen_color_mapping_procs);
static dev_proc_get_color_comp_index(spotcmyk_get_color_comp_index);
static dev_proc_encode_color(spotcmyk_encode_color);
static dev_proc_decode_color(spotcmyk_decode_color);

/*
 * A structure definition for a DeviceN type device
 */
typedef struct spotcmyk_device_s {
    gx_device_common;
    gx_prn_device_common;
    gs_devn_params devn_params;
} spotcmyk_device;

/* GC procedures */

static 
ENUM_PTRS_WITH(spotcmyk_device_enum_ptrs, spotcmyk_device *pdev)
{
    if (index < pdev->devn_params.separations.num_separations)
	ENUM_RETURN(pdev->devn_params.separations.names[index].data);
    ENUM_PREFIX(st_device_printer,
		    pdev->devn_params.separations.num_separations);
}

ENUM_PTRS_END
static RELOC_PTRS_WITH(spotcmyk_device_reloc_ptrs, spotcmyk_device *pdev)
{
    RELOC_PREFIX(st_device_printer);
    {
	int i;

	for (i = 0; i < pdev->devn_params.separations.num_separations; ++i) {
	    RELOC_PTR(spotcmyk_device, devn_params.separations.names[i].data);
	}
    }
}
RELOC_PTRS_END

/* Even though spotcmyk_device_finalize is the same as gx_device_finalize, */
/* we need to implement it separately because st_composite_final */
/* declares all 3 procedures as private. */
static void
spotcmyk_device_finalize(void *vpdev)
{
    gx_device_finalize(vpdev);
}

gs_private_st_composite_final(st_spotcmyk_device, spotcmyk_device,
    "spotcmyk_device", spotcmyk_device_enum_ptrs, spotcmyk_device_reloc_ptrs,
    spotcmyk_device_finalize);

/*
 * Macro definition for DeviceN procedures
 */
#define device_procs(get_color_mapping_procs)\
{	spotcmyk_prn_open,\
	gx_default_get_initial_matrix,\
	NULL,				/* sync_output */\
	gdev_prn_output_page,		/* output_page */\
	gdev_prn_close,			/* close */\
	NULL,				/* map_rgb_color - not used */\
	NULL,				/* map_color_rgb - not used */\
	NULL,				/* fill_rectangle */\
	NULL,				/* tile_rectangle */\
	NULL,				/* copy_mono */\
	NULL,				/* copy_color */\
	NULL,				/* draw_line */\
	NULL,				/* get_bits */\
	spotcmyk_get_params,		/* get_params */\
	spotcmyk_put_params,		/* put_params */\
	NULL,				/* map_cmyk_color - not used */\
	NULL,				/* get_xfont_procs */\
	NULL,				/* get_xfont_device */\
	NULL,				/* map_rgb_alpha_color */\
	gx_page_device_get_page_device,	/* get_page_device */\
	NULL,				/* get_alpha_bits */\
	NULL,				/* copy_alpha */\
	NULL,				/* get_band */\
	NULL,				/* copy_rop */\
	NULL,				/* fill_path */\
	NULL,				/* stroke_path */\
	NULL,				/* fill_mask */\
	NULL,				/* fill_trapezoid */\
	NULL,				/* fill_parallelogram */\
	NULL,				/* fill_triangle */\
	NULL,				/* draw_thin_line */\
	NULL,				/* begin_image */\
	NULL,				/* image_data */\
	NULL,				/* end_image */\
	NULL,				/* strip_tile_rectangle */\
	NULL,				/* strip_copy_rop */\
	NULL,				/* get_clipping_box */\
	NULL,				/* begin_typed_image */\
	NULL,				/* get_bits_rectangle */\
	NULL,				/* map_color_rgb_alpha */\
	NULL,				/* create_compositor */\
	NULL,				/* get_hardware_params */\
	NULL,				/* text_begin */\
	NULL,				/* finish_copydevice */\
	NULL,				/* begin_transparency_group */\
	NULL,				/* end_transparency_group */\
	NULL,				/* begin_transparency_mask */\
	NULL,				/* end_transparency_mask */\
	NULL,				/* discard_transparency_layer */\
	get_color_mapping_procs,	/* get_color_mapping_procs */\
	spotcmyk_get_color_comp_index,	/* get_color_comp_index */\
	spotcmyk_encode_color,		/* encode_color */\
	spotcmyk_decode_color,		/* decode_color */\
	NULL,				/* pattern_manage */\
	NULL				/* fill_rectangle_hl_color */\
}

fixed_colorant_name DeviceCMYKComponents[] = {
	"Cyan",
	"Magenta",
	"Yellow",
	"Black",
	0		/* List terminator */
};


#define spotcmyk_device_body(procs, dname, ncomp, pol, depth, mg, mc, cn)\
    std_device_full_body_type_extended(spotcmyk_device, &procs, dname,\
	  &st_spotcmyk_device,\
	  (int)((long)(DEFAULT_WIDTH_10THS) * (X_DPI) / 10),\
	  (int)((long)(DEFAULT_HEIGHT_10THS) * (Y_DPI) / 10),\
	  X_DPI, Y_DPI,\
    	  GX_DEVICE_COLOR_MAX_COMPONENTS,	/* MaxComponents */\
	  ncomp,		/* NumComp */\
	  pol,			/* Polarity */\
	  depth, 0,		/* Depth, GrayIndex */\
	  mg, mc,		/* MaxGray, MaxColor */\
	  mg + 1, mc + 1,	/* DitherGray, DitherColor */\
	  GX_CINFO_SEP_LIN,	/* Linear & Separable */\
	  cn,			/* Process color model name */\
	  0, 0,			/* offsets */\
	  0, 0, 0, 0		/* margins */\
	),\
	prn_device_body_rest_(spotcmyk_print_page)

/*
 * Example device with CMYK and spot color support
 */
static const gx_device_procs spot_cmyk_procs = device_procs(get_spotcmyk_color_mapping_procs);

const spotcmyk_device gs_spotcmyk_device =
{   
    spotcmyk_device_body(spot_cmyk_procs, "spotcmyk", 4, GX_CINFO_POLARITY_SUBTRACTIVE, 4, 1, 1, "DeviceCMYK"),
    /* DeviceN device specific parameters */
    { 1,			/* Bits per color - must match ncomp, depth, etc. above */
      DeviceCMYKComponents,	/* Names of color model colorants */
      4,			/* Number colorants for CMYK */
      0,			/* MaxSeparations has not been specified */
      -1,			/* PageSpotColors has not been specified */
      {0},			/* SeparationNames */
      0,			/* SeparationOrder names */
      {0, 1, 2, 3, 4, 5, 6, 7 }	/* Initial component SeparationOrder */
    }
};

/*
 * Example DeviceN color device
 */
static const gx_device_procs devicen_procs = device_procs(get_devicen_color_mapping_procs);

const spotcmyk_device gs_devicen_device =
{   
    spotcmyk_device_body(devicen_procs, "devicen", 4, GX_CINFO_POLARITY_SUBTRACTIVE, 32, 255, 255, "DeviceCMYK"),
    /* DeviceN device specific parameters */
    { 8,			/* Bits per color - must match ncomp, depth, etc. above */
      NULL,			/* No names for standard DeviceN color model */
      0,			/* No standard colorants for DeviceN */
      0,			/* MaxSeparations has not been specified */
      -1,			/* PageSpotColors has not been specified */
      {0},			/* SeparationNames */
      0,			/* SeparationOrder names */
      {0, 1, 2, 3, 4, 5, 6, 7 }	/* Initial component SeparationOrder */
    }
};

/* Open the psd devices */
int
spotcmyk_prn_open(gx_device * pdev)
{
    int code = gdev_prn_open(pdev);

    set_linear_color_bits_mask_shift(pdev);
    pdev->color_info.separable_and_linear = GX_CINFO_SEP_LIN;
    return code;
}

/* Color mapping routines for the spotcmyk device */

static void
gray_cs_to_spotcmyk_cm(gx_device * dev, frac gray, frac out[])
{
    int * map = ((spotcmyk_device *) dev)->devn_params.separation_order_map;

    gray_cs_to_devn_cm(dev, map, gray, out);
}

static void
rgb_cs_to_spotcmyk_cm(gx_device * dev, const gs_imager_state *pis,
				   frac r, frac g, frac b, frac out[])
{
    int * map = ((spotcmyk_device *) dev)->devn_params.separation_order_map;

    rgb_cs_to_devn_cm(dev, map, pis, r, g, b, out);
}

static void
cmyk_cs_to_spotcmyk_cm(gx_device * dev, frac c, frac m, frac y, frac k, frac out[])
{
    int * map = ((spotcmyk_device *) dev)->devn_params.separation_order_map;

    cmyk_cs_to_devn_cm(dev, map, c, m, y, k, out);
}

static const gx_cm_color_map_procs spotCMYK_procs = {
    gray_cs_to_spotcmyk_cm, rgb_cs_to_spotcmyk_cm, cmyk_cs_to_spotcmyk_cm
};

static const gx_cm_color_map_procs *
get_spotcmyk_color_mapping_procs(const gx_device * dev)
{
    return &spotCMYK_procs;
}

/* Also use the spotcmyk procs for the devicen device. */

static const gx_cm_color_map_procs *
get_devicen_color_mapping_procs(const gx_device * dev)
{
    return &spotCMYK_procs;
}


/*
 * Encode a list of colorant values into a gx_color_index_value.
 */
static gx_color_index
spotcmyk_encode_color(gx_device *dev, const gx_color_value colors[])
{
    int bpc = ((spotcmyk_device *)dev)->devn_params.bitspercomponent;
    int drop = sizeof(gx_color_value) * 8 - bpc;
    gx_color_index color = 0;
    int i = 0;
    int ncomp = dev->color_info.num_components;

    for (; i<ncomp; i++) {
	color <<= bpc;
        color |= (colors[i] >> drop);
    }
    return (color == gx_no_color_index ? color ^ 1 : color);
}

/*
 * Decode a gx_color_index value back to a list of colorant values.
 */
static int
spotcmyk_decode_color(gx_device * dev, gx_color_index color, gx_color_value * out)
{
    int bpc = ((spotcmyk_device *)dev)->devn_params.bitspercomponent;
    int drop = sizeof(gx_color_value) * 8 - bpc;
    int mask = (1 << bpc) - 1;
    int i = 0;
    int ncomp = dev->color_info.num_components;

    for (; i<ncomp; i++) {
        out[ncomp - i - 1] = (gx_color_value)((color & mask) << drop);
	color >>= bpc;
    }
    return 0;
}

/* Get parameters. */
static int
spotcmyk_get_params(gx_device * pdev, gs_param_list * plist)
{
    int code = gdev_prn_get_params(pdev, plist);

    if (code < 0)
	return code;
    return devn_get_params(pdev, plist,
    	&(((spotcmyk_device *)pdev)->devn_params), NULL);
}

/* Set parameters. */
static int
spotcmyk_put_params(gx_device * pdev, gs_param_list * plist)
{
    return devn_printer_put_params(pdev, plist,
	&(((spotcmyk_device *)pdev)->devn_params), NULL);
}

/*
 * This routine will check to see if the color component name  match those
 * that are available amoung the current device's color components.  
 *
 * Parameters:
 *   dev - pointer to device data structure.
 *   pname - pointer to name (zero termination not required)
 *   nlength - length of the name
 *
 * This routine returns a positive value (0 to n) which is the device colorant
 * number if the name is found.  It returns GX_DEVICE_COLOR_MAX_COMPONENTS if
 * the colorant is not being used due to a SeparationOrder device parameter.
 * It returns a negative value if not found.
 */
static int
spotcmyk_get_color_comp_index(gx_device * dev, const char * pname,
					int name_size, int component_type)
{
    return devn_get_color_comp_index(dev,
		&(((spotcmyk_device *)dev)->devn_params), NULL,
		pname, name_size, component_type, ENABLE_AUTO_SPOT_COLORS);
}


/*
 * This routine will extract a specified set of bits from a buffer and pack
 * them into a given buffer.
 *
 * Parameters:
 *   source - The source of the data
 *   dest - The destination for the data
 *   depth - The size of the bits per pixel - must be a multiple of 8
 *   first_bit - The location of the first data bit (LSB).
 *   bit_width - The number of bits to be extracted.
 *   npixel - The number of pixels.
 *
 * Returns:
 *   Length of the output line (in bytes)
 *   Data in dest.
 */
int
repack_data(byte * source, byte * dest, int depth, int first_bit,
		int bit_width, int npixel)
{
    int in_nbyte = depth >> 3;		/* Number of bytes per input pixel */
    int out_nbyte = bit_width >> 3;	/* Number of bytes per output pixel */
    gx_color_index mask = 1;
    gx_color_index data;
    int i, j, length = 0;
    byte temp;
    byte * out = dest;
    int in_bit_start = 8 - depth;
    int out_bit_start = 8 - bit_width;
    int in_byte_loc = in_bit_start, out_byte_loc = out_bit_start;

    mask = (mask << bit_width) - 1;
    for (i=0; i<npixel; i++) {
        /* Get the pixel data */
	if (!in_nbyte) {		/* Multiple pixels per byte */
	    data = *source;
	    data >>= in_byte_loc;
	    in_byte_loc -= depth;
	    if (in_byte_loc < 0) {	/* If finished with byte */
	        in_byte_loc = in_bit_start;
		source++;
	    }
	}
	else {				/* One or more bytes per pixel */
	    data = *source++;
	    for (j=1; j<in_nbyte; j++)
	        data = (data << 8) + *source++;
	}
	data >>= first_bit;
	data &= mask;

	/* Put the output data */
	if (!out_nbyte) {		/* Multiple pixels per byte */
	    temp = (byte)(*out & ~(mask << out_byte_loc));
	    *out = (byte)(temp | (data << out_byte_loc));
	    out_byte_loc -= bit_width;
	    if (out_byte_loc < 0) {	/* If finished with byte */
	        out_byte_loc = out_bit_start;
		out++;
	    }
	}
	else {				/* One or more bytes per pixel */
	    *out++ = (byte)(data >> ((out_nbyte - 1) * 8));
	    for (j=1; j<out_nbyte; j++) {
	        *out++ = (byte)(data >> ((out_nbyte - 1 - j) * 8));
	    }
	}
    }
    /* Return the number of bytes in the destination buffer. */
    if (out_byte_loc != out_bit_start) { 	/* If partially filled last byte */
	*out = *out & ((~0) << out_byte_loc);	/* Mask unused part of last byte */
	out++;
    }
    length = out - dest;
    return length;
}

static int devn_write_pcx_file(gx_device_printer * pdev, char * filename, int ncomp,
			    int bpc, int pcmlinelength);
/* 
 * This is an example print page routine for a DeviceN device.  This routine
 * will handle a DeviceN, a CMYK with spot colors, or an RGB process color model.
 *
 * This routine creates several output files.  If the process color model is
 * RGB or CMYK then a bit image file is created which contains the data for the
 * process color model data.  This data is put into the given file stream.
 * I.e. into the output file specified by the user.  This file is not created
 * for the DeviceN process color model.  A separate bit image file is created 
 * is created for the data for each of the given spot colors.  The names for
 * these files are created by taking the given output file name and appending
 * "sn" (where n is the spot color number 0 to ...) to the output file name.
 * The results are unknown if the output file is stdout etc.
 *
 * After the bit image files are created, then a set of PCX format files are
 * created from the bit image files.  This files have a ".pcx" appended to the
 * end of the files.  Thus a CMYK process color model with two spot colors
 * would end up with a total of six files being created.  (xxx, xxxs0, xxxs1,
 * xxx.pcx, xxxs0.pcx, and xxxs1.pcx).
 *
 * I do not assume that any users will actually want to create all of these
 * different files.  However I wanted to show an example of how each of the
 * spot * colorants could be unpacked from the process color model colorants.
 * The bit images files are an easy way to show this without the complication
 * of trying to put the data into a specific format.  However I do not have a
 * tool which will display the bit image data directly so I needed to convert
 * it to a form which I can view.  Thus the PCX format files are being created.
 * Note:  The PCX implementation is not complete.  There are many (most)
 * combinations of bits per pixel and number of colorants that are not supported.
 */
static int
spotcmyk_print_page(gx_device_printer * pdev, FILE * prn_stream)
{
    int line_size = gdev_mem_bytes_per_scan_line((gx_device *) pdev);
    byte *in = gs_alloc_bytes(pdev->memory, line_size, "spotcmyk_print_page(in)");
    byte *buf = gs_alloc_bytes(pdev->memory, line_size + 3, "spotcmyk_print_page(buf)");
    const spotcmyk_device * pdevn = (spotcmyk_device *) pdev;
    int npcmcolors = pdevn->devn_params.num_std_colorant_names;
    int ncomp = pdevn->color_info.num_components;
    int depth = pdevn->color_info.depth;
    int nspot = pdevn->devn_params.separations.num_separations;
    int bpc = pdevn->devn_params.bitspercomponent;
    int lnum = 0, bottom = pdev->height;
    int width = pdev->width;
    FILE * spot_file[GX_DEVICE_COLOR_MAX_COMPONENTS] = {0};
    int i, code = 0;
    int first_bit;
    int pcmlinelength = 0; /* Initialize against indeterminizm in case of pdev->height == 0. */
    int linelength[GX_DEVICE_COLOR_MAX_COMPONENTS];
    byte *data;
    char spotname[gp_file_name_sizeof];

    if (in == NULL || buf == NULL) {
	code = gs_error_VMerror;
	goto prn_done;
    }
    /*
     * Check if the SeparationOrder list has changed the order of the process
     * color model colorants. If so then we will treat all colorants as if they
     * are spot colors.
     */
    for (i = 0; i < npcmcolors; i++)
	if (pdevn->devn_params.separation_order_map[i] != i)
	    break;
    if (i < npcmcolors || ncomp < npcmcolors) {
	nspot = ncomp;
	npcmcolors = 0;
    }

    /* Open the output files for the spot colors */
    for(i = 0; i < nspot; i++) {
	sprintf(spotname, "%ss%d", pdevn->fname, i);
        spot_file[i] = fopen(spotname, "wb");
	if (spot_file[i] == NULL) {
	    code = gs_error_VMerror;
	    goto prn_done;
	}
    }


    /* Now create the output bit image files */
    for (; lnum < bottom; ++lnum) {
	gdev_prn_get_bits(pdev, lnum, in, &data);
        /* Now put the pcm data into the output file */
	if (npcmcolors) {
	    first_bit = bpc * (ncomp - npcmcolors);
	    pcmlinelength = repack_data(data, buf, depth, first_bit, bpc * npcmcolors, width);
	    fwrite(buf, 1, pcmlinelength, prn_stream);
	}
	/* Put spot color data into the output files */
        for (i = 0; i < nspot; i++) {
	    first_bit = bpc * (nspot - 1 - i);
	    linelength[i] = repack_data(data, buf, depth, first_bit, bpc, width);
	    fwrite(buf, 1, linelength[i], spot_file[i]);
        }
    }

    /* Close the bit image files */
    for(i = 0; i < nspot; i++) {
        fclose(spot_file[i]);
	spot_file[i] = NULL;
    }

    /* Now convert the bit image files into PCX files */
    if (npcmcolors) {
	code = devn_write_pcx_file(pdev, (char *) &pdevn->fname,
				npcmcolors, bpc, pcmlinelength);
	if (code < 0)
	    return code;
    }
    for(i = 0; i < nspot; i++) {
	sprintf(spotname, "%ss%d", pdevn->fname, i);
	code = devn_write_pcx_file(pdev, spotname, 1, bpc, linelength[i]);
	if (code < 0)
	    return code;
    }


    /* Clean up and exit */
  prn_done:
    for(i = 0; i < nspot; i++) {
	if (spot_file[i] != NULL)
            fclose(spot_file[i]);
    }
    if (in != NULL)
        gs_free_object(pdev->memory, in, "spotcmyk_print_page(in)");
    if (buf != NULL)
        gs_free_object(pdev->memory, buf, "spotcmyk_print_page(buf)");
    return code;
}

/*
 * We are using the PCX output format.  This is done for simplicity.
 * Much of the following code was copied from gdevpcx.c.
 */

/* ------ Private definitions ------ */

/* All two-byte quantities are stored LSB-first! */
#if arch_is_big_endian
#  define assign_ushort(a,v) a = ((v) >> 8) + ((v) << 8)
#else
#  define assign_ushort(a,v) a = (v)
#endif

typedef struct pcx_header_s {
    byte manuf;			/* always 0x0a */
    byte version;
#define version_2_5			0
#define version_2_8_with_palette	2
#define version_2_8_without_palette	3
#define version_3_0 /* with palette */	5
    byte encoding;		/* 1=RLE */
    byte bpp;			/* bits per pixel per plane */
    ushort x1;			/* X of upper left corner */
    ushort y1;			/* Y of upper left corner */
    ushort x2;			/* x1 + width - 1 */
    ushort y2;			/* y1 + height - 1 */
    ushort hres;		/* horz. resolution (dots per inch) */
    ushort vres;		/* vert. resolution (dots per inch) */
    byte palette[16 * 3];	/* color palette */
    byte reserved;
    byte nplanes;		/* number of color planes */
    ushort bpl;			/* number of bytes per line (uncompressed) */
    ushort palinfo;
#define palinfo_color	1
#define palinfo_gray	2
    byte xtra[58];		/* fill out header to 128 bytes */
} pcx_header;

/* Define the prototype header. */
static const pcx_header pcx_header_prototype =
{
    10,				/* manuf */
    0,				/* version (variable) */
    1,				/* encoding */
    0,				/* bpp (variable) */
    00, 00,			/* x1, y1 */
    00, 00,			/* x2, y2 (variable) */
    00, 00,			/* hres, vres (variable) */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* palette (variable) */
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    0,				/* reserved */
    0,				/* nplanes (variable) */
    00,				/* bpl (variable) */
    00,				/* palinfo (variable) */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* xtra */
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


/* Forward declarations */
static void devn_pcx_write_rle(const byte *, const byte *, int, FILE *);
static int devn_pcx_write_page(gx_device_printer * pdev, FILE * infile,
    int linesize, FILE * outfile, pcx_header * phdr, bool planar, int depth);

static const byte pcx_cmyk_palette[16 * 3] =
{
    0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x0f, 0x0f, 0x00,
    0xff, 0x00, 0xff, 0x0f, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0x00, 0x00,
    0x00, 0xff, 0xff, 0x00, 0x0f, 0x0f, 0x00, 0xff, 0x00, 0x00, 0x0f, 0x00,
    0x00, 0x00, 0xff, 0x00, 0x00, 0x0f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f,
};

static const byte pcx_ega_palette[16 * 3] =
{
    0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0xaa, 0x00, 0x00, 0xaa, 0xaa,
    0xaa, 0x00, 0x00, 0xaa, 0x00, 0xaa, 0xaa, 0xaa, 0x00, 0xaa, 0xaa, 0xaa,
    0x55, 0x55, 0x55, 0x55, 0x55, 0xff, 0x55, 0xff, 0x55, 0x55, 0xff, 0xff,
    0xff, 0x55, 0x55, 0xff, 0x55, 0xff, 0xff, 0xff, 0x55, 0xff, 0xff, 0xff
};


/*
 * This routine will set up the revision and palatte for the output
 * file.
 *
 * Please note that this routine does not currently handle all possible
 * combinations of bits and planes.
 *
 * Input parameters:
 *   pdev - Pointer to device data structure
 *   file - output file
 *   header - The header structure to hold the data.
 *   bits_per_plane - The number of bits per plane.
 *   num_planes - The number of planes.
 */
static bool
devn_setup_pcx_header(gx_device_printer * pdev, pcx_header * phdr, int num_planes, int bits_per_plane)
{
    bool planar = true; /* Invalid cases could cause an indeterminizm. */
 
    *phdr = pcx_header_prototype;
    phdr->bpp = bits_per_plane;
    phdr->nplanes = num_planes;

    switch (num_planes) {
        case 1:
	    switch (bits_per_plane) {
	        case 1:
    			phdr->version = version_2_8_with_palette;
    			assign_ushort(phdr->palinfo, palinfo_gray);
    			memcpy((byte *) phdr->palette, "\000\000\000\377\377\377", 6);
			planar = false;
			break;
		case 2:				/* Not defined */
			break;
		case 4:	
    			phdr->version = version_2_8_with_palette;
    			memcpy((byte *) phdr->palette, pcx_ega_palette, sizeof(pcx_ega_palette));
			planar = true;
			break;
		case 5:				/* Not defined */
			break;
		case 8:
    			phdr->version = version_3_0;
    			assign_ushort(phdr->palinfo, palinfo_gray);
			planar = false;
			break;
		case 16:			/* Not defined */
			break;
	    }
	    break;
	case 2:
	    switch (bits_per_plane) {
	        case 1:				/* Not defined */
			break;
		case 2:				/* Not defined */
			break;
		case 4:				/* Not defined */
			break;
		case 5:				/* Not defined */
			break;
		case 8:				/* Not defined */
			break;
		case 16:			/* Not defined */
			break;
	    }
	    break;
	case 3:
	    switch (bits_per_plane) {
	        case 1:				/* Not defined */
			break;
		case 2:				/* Not defined */
			break;
		case 4:				/* Not defined */
			break;
		case 5:				/* Not defined */
			break;
		case 8:
    			phdr->version = version_3_0;
    			assign_ushort(phdr->palinfo, palinfo_color);
			planar = true;
			break;
		case 16:			/* Not defined */
			break;
	    }
	    break;
	case 4:
	    switch (bits_per_plane) {
	        case 1:
    			phdr->version = 2;
    			memcpy((byte *) phdr->palette, pcx_cmyk_palette,
	   			sizeof(pcx_cmyk_palette));
			planar = false;
			phdr->bpp = 4;
			phdr->nplanes = 1;
			break;
		case 2:				/* Not defined */
			break;
		case 4:				/* Not defined */
			break;
		case 5:				/* Not defined */
			break;
		case 8:				/* Not defined */
			break;
		case 16:			/* Not defined */
			break;
	    }
	    break;
    }
    return planar;
}

/* Write a palette on a file. */
static int
pc_write_mono_palette(gx_device * dev, uint max_index, FILE * file)
{
    uint i, c;
    gx_color_value rgb[3];

    for (i = 0; i < max_index; i++) {
	rgb[0] = rgb[1] = rgb[2] = i << 8;
	for (c = 0; c < 3; c++) {
	    byte b = gx_color_value_to_byte(rgb[c]);

	    fputc(b, file);
	}
    }
    return 0;
}
/*
 * This routine will send any output data required at the end of a file
 * for a particular combination of planes and bits per plane.
 *
 * Please note that most combinations do not require anything at the end
 * of a data file.
 *
 * Input parameters:
 *   pdev - Pointer to device data structure
 *   file - output file
 *   header - The header structure to hold the data.
 *   bits_per_plane - The number of bits per plane.
 *   num_planes - The number of planes.
 */
static int
devn_finish_pcx_file(gx_device_printer * pdev, FILE * file, pcx_header * header, int num_planes, int bits_per_plane)
{
    switch (num_planes) {
        case 1:
	    switch (bits_per_plane) {
	        case 1:				/* Do nothing */
			break;
		case 2:				/* Not defined */
			break;
		case 4:				/* Do nothing */
			break;
		case 5:				/* Not defined */
			break;
		case 8:
			fputc(0x0c, file);
			return pc_write_mono_palette((gx_device *) pdev, 256, file);
		case 16:			/* Not defined */
			break;
	    }
	    break;
	case 2:
	    switch (bits_per_plane) {
	        case 1:				/* Not defined */
			break;
		case 2:				/* Not defined */
			break;
		case 4:				/* Not defined */
			break;
		case 5:				/* Not defined */
			break;
		case 8:				/* Not defined */
			break;
		case 16:			/* Not defined */
			break;
	    }
	    break;
	case 3:
	    switch (bits_per_plane) {
	        case 1:				/* Not defined */
			break;
		case 2:				/* Not defined */
			break;
		case 4:				/* Not defined */
			break;
		case 5:				/* Not defined */
			break;
		case 8:				/* Do nothing */
			break;
		case 16:			/* Not defined */
			break;
	    }
	    break;
	case 4:
	    switch (bits_per_plane) {
	        case 1:				/* Do nothing */
			break;
		case 2:				/* Not defined */
			break;
		case 4:				/* Not defined */
			break;
		case 5:				/* Not defined */
			break;
		case 8:				/* Not defined */
			break;
		case 16:			/* Not defined */
			break;
	    }
	    break;
    }
    return 0;
}

/* Send the page to the printer. */
static int
devn_write_pcx_file(gx_device_printer * pdev, char * filename, int ncomp,
			    int bpc, int linesize)
{
    pcx_header header;
    int code;
    bool planar;
    char outname[gp_file_name_sizeof];
    FILE * in;
    FILE * out;
    int depth = bpc_to_depth(ncomp, bpc);

    in = fopen(filename, "rb");
    if (!in)
	return_error(gs_error_invalidfileaccess);
    sprintf(outname, "%s.pcx", filename);
    out = fopen(outname, "wb");
    if (!out) {
	fclose(in);
	return_error(gs_error_invalidfileaccess);
    }

    planar = devn_setup_pcx_header(pdev, &header, ncomp, bpc);
    code = devn_pcx_write_page(pdev, in, linesize, out, &header, planar, depth);
    if (code >= 0)
        code = devn_finish_pcx_file(pdev, out, &header, ncomp, bpc);

    fclose(in);
    fclose(out);
    return code;
}

/* Write out a page in PCX format. */
/* This routine is used for all formats. */
/* The caller has set header->bpp, nplanes, and palette. */
static int
devn_pcx_write_page(gx_device_printer * pdev, FILE * infile, int linesize, FILE * outfile,
	       pcx_header * phdr, bool planar, int depth)
{
    int raster = linesize;
    uint rsize = ROUND_UP((pdev->width * phdr->bpp + 7) >> 3, 2);	/* PCX format requires even */
    int height = pdev->height;
    uint lsize = raster + rsize;
    byte *line = gs_alloc_bytes(pdev->memory, lsize, "pcx file buffer");
    byte *plane = line + raster;
    int y;
    int code = 0;		/* return code */

    if (line == 0)		/* can't allocate line buffer */
	return_error(gs_error_VMerror);

    /* Fill in the other variable entries in the header struct. */

    assign_ushort(phdr->x2, pdev->width - 1);
    assign_ushort(phdr->y2, height - 1);
    assign_ushort(phdr->hres, (int)pdev->x_pixels_per_inch);
    assign_ushort(phdr->vres, (int)pdev->y_pixels_per_inch);
    assign_ushort(phdr->bpl, (planar || depth == 1 ? rsize :
			      raster + (raster & 1)));

    /* Write the header. */

    if (fwrite((const char *)phdr, 1, 128, outfile) < 128) {
	code = gs_error_ioerror;
	goto pcx_done;
    }
    /* Write the contents of the image. */
    for (y = 0; y < height; y++) {
	byte *row = line;
	byte *end;

	code = fread(line, sizeof(byte), linesize, infile);
	if (code < 0)
	    break;
	end = row + raster;
	if (!planar) {		/* Just write the bits. */
	    if (raster & 1) {	/* Round to even, with predictable padding. */
		*end = end[-1];
		++end;
	    }
	    devn_pcx_write_rle(row, end, 1, outfile);
	} else
	    switch (depth) {

		case 4:
		    {
			byte *pend = plane + rsize;
			int shift;

			for (shift = 0; shift < 4; shift++) {
			    register byte *from, *to;
			    register int bright = 1 << shift;
			    register int bleft = bright << 4;

			    for (from = row, to = plane;
				 from < end; from += 4
				) {
				*to++ =
				    (from[0] & bleft ? 0x80 : 0) |
				    (from[0] & bright ? 0x40 : 0) |
				    (from[1] & bleft ? 0x20 : 0) |
				    (from[1] & bright ? 0x10 : 0) |
				    (from[2] & bleft ? 0x08 : 0) |
				    (from[2] & bright ? 0x04 : 0) |
				    (from[3] & bleft ? 0x02 : 0) |
				    (from[3] & bright ? 0x01 : 0);
			    }
			    /* We might be one byte short of rsize. */
			    if (to < pend)
				*to = to[-1];
			    devn_pcx_write_rle(plane, pend, 1, outfile);
			}
		    }
		    break;

		case 24:
		    {
			int pnum;

			for (pnum = 0; pnum < 3; ++pnum) {
			    devn_pcx_write_rle(row + pnum, row + raster, 3, outfile);
			    if (pdev->width & 1)
				fputc(0, outfile);		/* pad to even */
			}
		    }
		    break;

		default:
		    code = gs_note_error(gs_error_rangecheck);
		    goto pcx_done;

	    }
	code = 0;
    }

  pcx_done:
    gs_free_object(pdev->memory, line, "pcx file buffer");

    return code;
}

/* ------ Internal routines ------ */

/* Write one line in PCX run-length-encoded format. */
static void
devn_pcx_write_rle(const byte * from, const byte * end, int step, FILE * file)
{  /*
    * The PCX format theoretically allows encoding runs of 63
    * identical bytes, but some readers can't handle repetition
    * counts greater than 15.
    */
#define MAX_RUN_COUNT 15
    int max_run = step * MAX_RUN_COUNT;

    while (from < end) {
	byte data = *from;

	from += step;
	if (data != *from || from == end) {
	    if (data >= 0xc0)
		putc(0xc1, file);
	} else {
	    const byte *start = from;

	    while ((from < end) && (*from == data))
		from += step;
	    /* Now (from - start) / step + 1 is the run length. */
	    while (from - start >= max_run) {
		putc(0xc0 + MAX_RUN_COUNT, file);
		putc(data, file);
		start += max_run;
	    }
	    if (from > start || data >= 0xc0)
		putc((from - start) / step + 0xc1, file);
	}
	putc(data, file);
    }
#undef MAX_RUN_COUNT
}
