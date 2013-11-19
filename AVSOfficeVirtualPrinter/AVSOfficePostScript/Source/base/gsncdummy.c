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
/* $Id: gsncdummy.c 8945 2008-08-06 19:57:07Z giles $ */
/* Sample implementation for client custom processing of color spaces. */

/*
 * If this flag is 1 then we also do custom processing for the DeviceGray,
 * DeviceRGB, and DeviceCMYK color spaces.  For these color spaces, we
 * convert all text to shades of red, all images to shades of green and
 * all lines and fills to shades of blue.  if the flag is 0 then our example
 * only handles PANTONE colors (see comment below).
 */

/* Added these two defines so that I could compare to ghost script rendering color */

#define OBJECT_TYPE_EXAMPLE 1		/* 0 --> disabled, 1 --> enabled */
#define PANTONE_ONLY 0
/*
 * This module has been created to demonstrate how to support the use of
 * PANTONE colors to the Ghostscript graphics library.  PANTONE colors
 * are specified in both PostScript and PDF files via the use of DeviceN
 * or Separation color spaces.
 *
 * PANTONE is a registered trademark and PANTONE colors are a
 * licensed product of Pantone Inc. See http://www.pantone.com
 * for more information.
 *
 * See the comments at the start of src/gsnamecl.c for description of the
 * client color processing routines.
 *
 * Since this is only a 'demo' implementation, the example implementation does
 * not have some features which might be expected in a 'real' implementation.
 *
 * 1.  The Pantone color data table does not have actual entries for all
 *     of the different Pantone colors.  This data is not included since
 *     the values are dependent upon specific details of the output device,
 *     inks, etc.
 * 2.  Colors in PostScript and PDF are specified with by values between
 *     0 and 1.  The output colorant values are scaled linearly.
 * 3.  DeviceN color spaces can specify multiple colors.  However this
 *     implementation assumes that if a PANTONE color is specified in a
 *     DeviceN color space, then only PANTONE colors or CMYK are present.
 *     This was done to keep the code simple.  If other colors are present,
 *     then this implementation falls back to using the alternate color space
 *     specified with the DeviceN color space.  (This is the normal PS
 *     and PDF operation.)
 *
 * See also src/zsncdummy.c for an example custom color callback.
 */

#include "stdpre.h"
#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gscdefs.h"
#include "gscspace.h"
#include "gxcspace.h"
#include "gscie.h"
#include "gsicc.h"
#include "gxdevice.h"
#include "gzstate.h"
#include "gsutil.h"
#include "gxcie.h"
#include "gsncdummy.h"


#if ENABLE_CUSTOM_COLOR_CALLBACK		/* Defined in src/gsnamecl.h */

/*
 * Since this is only a 'demo' list, the list does have not entries for all
 * of the different PANTONE colors.  Creation of a real list is left as an
 * exercise for the user.
 */
const pantone_list_t pantone_list[] = {
    { "PantoneCyan",	1, 0, 0, 0 },
    { "PantoneMagenta",	0, 1, 0, 0 },
    { "PantoneYellow",	0, 0, 1, 0 },
    { "PantoneBlack",	0, 0, 0, 1 },
    { "Orange",	0, 1, .5, 0 }
};

/*
 * We will handle color spaces that include both PANTONE colors, CMYK, and
 * 'None'.  'None' is a special case in DeviceN color spaces.  It has no
 * effects upon the output color but it can be present in DeviceN color
 * spaces in a PDF file.  To simplify the code, we need pantone index values
 * for these five 'colors'.
 */
#define PANTONE_NONE    count_of(pantone_list)
#define PANTONE_CYAN    (PANTONE_NONE + 1)
#define PANTONE_MAGENTA (PANTONE_NONE + 2)
#define PANTONE_YELLOW  (PANTONE_NONE + 3)
#define PANTONE_BLACK   (PANTONE_NONE + 4)

/* Compare two names */
#define compare_names(name1, name_size1, name2, name_size2) \
    (name_size1 == name_size2 && \
	(memcmp((const char *)name1, (const char *)name2, name_size1) == 0))

/*
 * Define a structure for holding our client specific data.  In our demo,
 * we are only supporting Separation and DeviceN color spaces.  To make
 * life simpler, we are using the same data structure for both types
 * of color spaces.
 */
typedef struct demo_color_space_data_s {
    /*
     * All client color space data blocks must begin with a routine for
     * handling the reference counts for the data block.
     */
    cs_proc_adjust_client_cspace_count((*client_adjust_cspace_count));

    /* Use a reference count for knowing when to release the data block. */
    int ref_count;

    /* the memory allocator used */
    gs_memory_t *memory;

    /* A flag which indicates the client wants to process the color space. */
    bool client_is_going_to_handle_color_space;

    /*
     * We store an index into our Pantone color translation table for each
     * colorant in the color space.
     */
    int color_index[GS_CLIENT_COLOR_MAX_COMPONENTS];
    gs_imager_state *CIEtoXYZ_pis;	/* Used to map CIE spaces to XYZ */
					/* refer to gx_cie_to_xyz_alloc	 */
} demo_color_space_data_t;

gs_private_st_ptrs1(st_demo_color_space_data, demo_color_space_data_t,
	    "demo_color_space_data_t", demo_color_space_enum_ptrs,
	    demo_color_space_reloc_ptrs, CIEtoXYZ_pis);

/*
 * Dummy install routine for color spaces which are not handled by the client.
 */
static bool
client_install_no_op(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
    return false;	/* Do nothing */
}

/*
 * Adjust the reference count on our client data,
 * freeing it if necessary.
 */
static void
client_adjust_cspace_count(const gs_color_space * pcs, int delta)
{
    demo_color_space_data_t * pdata =
	(demo_color_space_data_t *)(pcs->pclient_color_space_data);

    pdata->ref_count += delta;
    if (pdata->ref_count <= 0) {
	/* Free up the CIE to XYZ imager state if it was allocated */
	if (pdata->CIEtoXYZ_pis) {
	    gx_cie_to_xyz_free(pdata->CIEtoXYZ_pis);
	}
	gs_free_object(pdata->memory, pdata, "client_adjust_cspace_count(pdata)");
    }
}

/*
 * Allocate a data block for holding our data for the client specific
 * data for a color space.  In our demo, we are only supporting the
 * Separation and DeviceN color spaces.  We use a single data structure
 * to make the code simpler. We also provide sample hooks for a client
 * that wants to convert ALL CIEBased color spaces to XYZ (or Lab).
 */
static demo_color_space_data_t *
allocate_client_data_block(int initial_ref_count, gs_memory_t *mem)
{
    /*
     * We allocate this with normal GC structure declarations since
     * we need this to be able to allocate the gs_imager_state for XYZ
     * conversion.
     * Since this is in stable memory, we use a simple reference count.
     * See client_adjust_cspace_count.
     */
    demo_color_space_data_t * pdata =
	(demo_color_space_data_t *)gs_alloc_struct(mem, demo_color_space_data_t,
			&st_demo_color_space_data, "allocate_client_data_block(pdata)");

    if (pdata != NULL) {
	memset(pdata, 0, sizeof(demo_color_space_data_t));
	/*
	 * All client color space data blocks must have a pointer to a
	 * reference count adjust routine as their first field.
	 */
	pdata->client_adjust_cspace_count = client_adjust_cspace_count;
	pdata->ref_count = 1;
	pdata->memory = mem;
    }

    return pdata;
}

static bool
client_install_generic(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
	demo_color_space_data_t * pclient_data;

	/* Exit if we have already installed this color space. */
	if (pcs->pclient_color_space_data != NULL)
		return true;

	pclient_data = allocate_client_data_block(1, pcs->rc.memory->stable_memory);
	pcs->pclient_color_space_data = (client_color_space_data_t *) pclient_data;
	if (pclient_data)
	{
		pclient_data->client_is_going_to_handle_color_space = 1;
		return true;
	}
	return false;
}

/*
 * Check if we want to use the PANTONE color processing logic for the given
 * Separation color space.
 */
static bool
client_pantone_install_Separation(client_custom_color_params_t * pparam,
			gs_color_space * pcs, gs_state * pgs)
{
    const gs_separation_name name = pcs->params.separation.sep_name;
    int pan_index;
    byte * pname;
    uint name_size;
    gx_device * dev = pgs->device;
    int num_pantone_colors = count_of(pantone_list);
    bool use_custom_color_callback = false;

    /* Exit if we have already installed this color space. */
    if (pcs->pclient_color_space_data != NULL)
		return true;

    /*
     * Get the character string and length for the component name.
     */
    pcs->params.separation.get_colorname_string(dev->memory, name,
						&pname, &name_size);
    /*
    * Compare the colorant name to those in our PANTONE color list.
    */
    for (pan_index = 0; pan_index < num_pantone_colors ; pan_index++) {
	const char * pan_name = pantone_list[pan_index].name;

	if (compare_names(pname, name_size, pan_name, strlen(pan_name))) {
	    use_custom_color_callback = true;
	    break;
	}
    }

    if (use_custom_color_callback) {
        demo_color_space_data_t * pclient_data =
		allocate_client_data_block(1, pcs->rc.memory->stable_memory);

	if (pclient_data == NULL)
		return false;
	pclient_data->color_index[0] = pan_index;
        pcs->pclient_color_space_data =
	       (client_color_space_data_t *) pclient_data;
    }
    return use_custom_color_callback;
}

/*
 * Check if we want to use the PANTONE color processing logic for the given
 * DeviceN color space.
 */
static bool
client_pantone_install_DeviceN(client_custom_color_params_t * pparam,
			gs_color_space * pcs, gs_state * pgs)
{
    const gs_separation_name *names = pcs->params.device_n.names;
    int num_comp = pcs->params.device_n.num_components;
    int i;
    int pan_index;
    byte * pname;
    uint name_size;
    gx_device * dev = pgs->device;
    int num_pantone_colors = count_of(pantone_list);
    bool pantone_found = false;
    bool other_separation_found = false;
    bool use_pantone;
    const char none_str[] = "None";
    const uint none_size = strlen(none_str);
    const char cyan_str[] = "Cyan";
    const uint cyan_size = strlen(cyan_str);
    const char magenta_str[] = "Magenta";
    const uint magenta_size = strlen(magenta_str);
    const char yellow_str[] = "Yellow";
    const uint yellow_size = strlen(yellow_str);
    const char black_str[] = "Black";
    const uint black_size = strlen(black_str);
    int pantone_color_index[GS_CLIENT_COLOR_MAX_COMPONENTS];

    /* Exit if we have already installed this color space. */
    if (pcs->pclient_color_space_data != NULL)
		return true;

    /*
     * Now check the names of the color components.
     */
    for(i = 0; i < num_comp; i++ ) {
	bool match = false;

	/*
	 * Get the character string and length for the component name.
	 */
	pcs->params.device_n.get_colorname_string(dev->memory, names[i],
							&pname, &name_size);
	/*
         * Postscript does not include /None as a color component but it is
         * allowed in PDF so we accept it.  We simply skip components named
	 * 'None'.
         */
	if (compare_names(none_str, none_size, pname, name_size)) {
	    pantone_color_index[i] = PANTONE_NONE;
	    continue;
	}
	/*
	 * Check if our color space includes the CMYK process colors.
	 */
	if (compare_names(cyan_str, cyan_size, pname, name_size)) {
	    pantone_color_index[i] = PANTONE_CYAN;
	    continue;
	}
	if (compare_names(magenta_str, magenta_size, pname, name_size)) {
	    pantone_color_index[i] = PANTONE_MAGENTA;
	    continue;
	}
	if (compare_names(yellow_str, yellow_size, pname, name_size)) {
	    pantone_color_index[i] = PANTONE_YELLOW;
	    continue;
	}
	if (compare_names(black_str, black_size, pname, name_size)) {
	    pantone_color_index[i] = PANTONE_BLACK;
	    continue;
	}
	/*
	 * Compare the colorant name to those in our Pantone color list.
	 */
	for (pan_index = 0; pan_index < num_pantone_colors ; pan_index++) {
	    const char * pan_name = pantone_list[pan_index].name;

	    if (compare_names(pname, name_size, pan_name, strlen(pan_name))) {
	        pantone_color_index[i] = pan_index;
		match = pantone_found = true;
		break;
	    }
	}
	if (!match) {		/* Exit if we find a non Pantone color */
	    other_separation_found = true;
	    break;
	}
    }
    /*
     * Handle this color space as a 'pantone color space' if we have only
     * PANTONE colors and CMYK.  Any other separations will force us to
     * use the normal Ghostscript processing for a DeviceN color space.
     */
    use_pantone = pantone_found && !other_separation_found;
    if (use_pantone) {
        demo_color_space_data_t * pclient_data =
		allocate_client_data_block(1, pcs->rc.memory->stable_memory);

        if (pclient_data == NULL)
	    return false;
        for(i = 0; i < num_comp; i++ )
	    pclient_data->color_index[i] = pantone_color_index[i];
        pcs->pclient_color_space_data =
	       (client_color_space_data_t *) pclient_data;
    }
    return use_pantone;
}


/*
 * Convert a set of color values in a 'PANTONE color space' into a device
 * color values.
 *
 * This routine creates an equivalent CMYK color and then uses
 * gx_remap_concrete_cmyk to convert this into device colorants.  Note:  It
 * is possible to go directy to the output device colorants.  However the
 * pantone_install_xxx routines should verify that the expected device
 * colorants match the actual device colorants.  (For instance, Ghostscript
 * can install temporary compositing devices for functions like handling
 * PDF 1.4 transparency.  The compositing devices may have a process color
 * models which differ from the final output device.)
 */
static int
client_pantone_remap_color(client_custom_color_params_t * pparam,
	const frac * pconc, const demo_color_space_data_t * pparams,
       	gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
	gs_color_select_t select, int num_comp)
{
    int i, pantone_index, cvalue;
    int cyan = 0;
    int magenta = 0;
    int yellow = 0;
    int black = 0;
    frac cc, cm, cy, ck;
    const pantone_list_t * plist;

    /*
     * If the client color space data pointer is NULL then we are not processing
     * this color space.  The rangecheck error will indicate that GS should do
     * its normal color space processing.
     */
    if (pparams == NULL)
	return_error(gs_error_rangecheck);

    /*
     * Create a CMYK representation of the various colors in our color space.
     * Note:  If we have multiple components, then we do a simple sum of the
     * CMYK equivalent for each color.  If desired, a more complex handling is
     * left to the user.
     */
    for (i = 0; i < num_comp; i++) {
	cvalue = pconc[i];
	pantone_index = pparams->color_index[i];
	switch (pantone_index) {
	    case PANTONE_NONE:
		break;
	    case PANTONE_CYAN:
		cyan += cvalue;
		break;
	    case PANTONE_MAGENTA:
		magenta += cvalue;
		break;
	    case PANTONE_YELLOW:
		yellow += cvalue;
		break;
	    case PANTONE_BLACK:
		black += cvalue;
		break;
	    default:
		plist = &(pantone_list[pantone_index]);
		cyan += (int) floor(cvalue * plist->c);
		magenta += (int) floor(cvalue * plist->m);
		yellow += (int) floor(cvalue * plist->y);
		black += (int) floor(cvalue * plist->k);
		break;
	}
    }
    /* Clamp our color values */
    cc = (cyan > frac_1) ? frac_1 : (cyan < frac_0) ? frac_0 : cyan;
    cm = (magenta > frac_1) ? frac_1 : (magenta < frac_0) ? frac_0 : magenta;
    cy = (yellow > frac_1) ? frac_1 : (yellow < frac_0) ? frac_0 : yellow;
    ck = (black > frac_1) ? frac_1 : (black < frac_0) ? frac_0 : black;
    gx_remap_concrete_cmyk(cc, cm, cy, ck, pdc, pis, dev, select);
    return 0;
}

/*
 * Convert a Separation color (with PANTONE colorants) into device color.
 */
static int
client_pantone_remap_Separation(client_custom_color_params_t * pparam,
	const frac * pconc, const gs_color_space * pcs, gx_device_color * pdc,
       	const gs_imager_state * pis, gx_device * dev, gs_color_select_t select)
{
    return client_pantone_remap_color(pparam, pconc,
	(demo_color_space_data_t *)(pcs->pclient_color_space_data),
       	pdc, pis, dev, select, 1);
}

/*
 * Convert a DeviceN color (with PANTONE colorants) into device color.
 */
static int
client_pantone_remap_DeviceN(client_custom_color_params_t * pparam,
	const frac * pconc, const gs_color_space * pcs, gx_device_color * pdc,
       	const gs_imager_state * pis, gx_device * dev, gs_color_select_t select)
{
   	return client_pantone_remap_color(pparam, pconc,
	(demo_color_space_data_t *)(pcs->pclient_color_space_data),
	pdc, pis, dev, select, gs_color_space_num_components(pcs));
}


#if !PANTONE_ONLY

/*
 * Install a DeviceGray color space.
 */
static bool
client_install_DeviceGray(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
    /* Nothing to do in our demo */
    return true;
}

/*
 * For demo and debug purposes, make our colors a function of the
 * intensity of the given colors and the object type.
 */
static int
convert_intensity_into_device_color(const frac intensity,
	gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
	gs_color_select_t select)
{
    frac cc, cm, cy, ck;

    switch (pis->object_tag) {
	case GS_TEXT_TAG:		/* Make text red. */
		cc = ck = 0;
		cm = cy = frac_1 - intensity;
		break;
	case GS_IMAGE_TAG:		/* Make images green. */
		cm = ck = 0;
		cc = cy = frac_1 - intensity;
		break;
	case GS_PATH_TAG:		/* Make lines and fills blue. */
	default:
		cy = ck = 0;
		cc = cm = frac_1 - intensity;
		break;
    }

    /* Send CMYK colors to the device */
    gx_remap_concrete_cmyk(cc, cm, cy, ck, pdc, pis, dev, select);
    return 0;
}

/*
 * Convert a DeviceGray color into device color.
 */
static int
client_remap_DeviceGray(client_custom_color_params_t * pparams,
    const frac * pconc, const gs_color_space * pcs, gx_device_color * pdc,
    const gs_imager_state * pis, gx_device * dev, gs_color_select_t select)
{

	#if OBJECT_TYPE_EXAMPLE

		 /* For demo and debug purposes, make our colors a function of the
		 * intensity of the given colors and the object type. */

		frac intensity = pconc[0];
		convert_intensity_into_device_color(intensity, pdc, pis, dev, select); 

	#else

		/* If desired, replace with your own color transformation */

		gx_remap_concrete_gray(pconc[0], pdc, pis, dev, select);

	#endif


    return 0;
}

/*
 * Install a DeviceRGB color space.
 */
static bool
client_install_DeviceRGB(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
    /* Nothing to do in our demo */
    dlprintf1("client_install_DeviceRGB ri = %d\n", pgs->renderingintent);
    return true;
}

/*
 * Convert a DeviceRGB color into device color.
 */
static int
client_remap_DeviceRGB(client_custom_color_params_t * pparams,
	const frac * pconc, const gs_color_space * pcs, gx_device_color * pdc,
       	const gs_imager_state * pis, gx_device * dev, gs_color_select_t select)
{

	#if OBJECT_TYPE_EXAMPLE

		/* For demo and debug purposes, make our colors a function of the
		 * intensity of the given colors and the object type. */

		frac intensity = (frac)(pconc[0] * 0.30 + pconc[1] * 0.59 + pconc[2] * 0.11);
		convert_intensity_into_device_color(intensity, pdc, pis, dev, select);

	#else

		 /* If desired, replace with your own color transformation */

		 gx_remap_concrete_rgb(pconc[0], pconc[1], pconc[2], pdc, pis, dev, select);

	#endif

    return 0;
}

/*
 * Install a DeviceCMYK color space.
 */
static bool
client_install_DeviceCMYK(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
    /* Nothing to do in our demo */
    return true;
}

/*
 * Convert a DeviceGray color into device color.
 */
static int
client_remap_DeviceCMYK(client_custom_color_params_t * pparams,
	const frac * pconc, const gs_color_space * pcs, gx_device_color * pdc,
       	const gs_imager_state * pis, gx_device * dev, gs_color_select_t select)
{

	#if OBJECT_TYPE_EXAMPLE

	   /*
		 * For demo and debug purposes, make our colors a function of the
		 * intensity of the given colors and the object type.  */

		frac intensity = frac_1 - (frac)(pconc[0] * 0.30 + pconc[1] * 0.59
				+ pconc[2] * 0.11 + pconc[3]);

		if (intensity < frac_0)
		intensity = frac_0;
		convert_intensity_into_device_color(intensity, pdc, pis, dev, select); 		

	#else

		 /* If desired, replace with your own color transformation */
		 gx_remap_concrete_cmyk(pconc[0], pconc[1], pconc[2], pconc[3],pdc, pis, dev, select);

	#endif

    return 0;
}

/*
 * Convert a floating point color value into a fixed (frac) color value
 * given a specified floating point range.
 */
#define convert2frac(color, range) \
	(color <= range.rmin) ? frac_0 \
	    : (color >= range.rmax)  ? frac_1 \
		: (frac) (frac_1 * \
			(color - range.rmin) / (range.rmax - range.rmin))

static bool
client_install_CIEtoA(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
    /* get ready for converting to XYZ */
    demo_color_space_data_t * pdata;

    /* Exit if we have already installed this color space. */
    if (pcs->pclient_color_space_data != NULL)
	return true;

    pdata = allocate_client_data_block(1, pcs->rc.memory->stable_memory);
    pcs->pclient_color_space_data = (client_color_space_data_t *) pdata;
    if (pdata)
    {
	int code;
	gs_cie_a *pcie = pcs->params.a;
	gs_sample_loop_params_t lp;
	int i;

	pdata->client_is_going_to_handle_color_space = 1;

	/* Fill the caches we need in the CIE color space */
	gs_cie_cache_init(&pcie->caches.DecodeA.floats.params, &lp,
			  &pcie->RangeA, "DecodeA");
	for (i = 0; i <= lp.N; ++i) {
	    float in = SAMPLE_LOOP_VALUE(i, lp);

	    pcie->caches.DecodeA.floats.values[i] = (*pcie->DecodeA)(in, pcie);
	}
	gx_cie_load_common_cache(&pcie->common, pgs);
	gs_cie_a_complete(pcie);
	if ((code=gs_cie_cs_complete(pgs, true)) >= 0) {
	    /* Now allocate the conversion imager state in stable_memory */
	    /* so that the garbage collector won't free it		 */
	    code = gx_cie_to_xyz_alloc(&pdata->CIEtoXYZ_pis, pcs,
					pcs->rc.memory->stable_memory);
	}
	if (code < 0) {
	    client_adjust_cspace_count(pcs, -1);  /* free it up */
	    return false;
	}
    }
    return true;
}

static bool
client_install_CIEtoXYZ(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
    /* get ready for converting to XYZ */
    demo_color_space_data_t * pdata;

    /* Exit if we have already installed this color space. */
    if (pcs->pclient_color_space_data != NULL)
	return true;

    pdata = allocate_client_data_block(1, pcs->rc.memory->stable_memory);
    pcs->pclient_color_space_data = (client_color_space_data_t *) pdata;
    if (pdata)
    {
	int code;
	const gs_cie_abc *pcie;

	pdata->client_is_going_to_handle_color_space = 1;
        switch (pcs->type->index) {
	  case gs_color_space_index_CIEDEF: {
	    gs_cie_def *pcie_def = pcs->params.def;

	    pcie = (gs_cie_abc *)pcie_def;
	    CIE_LOAD_CACHE_BODY(pcie_def->caches_def.DecodeDEF, pcie_def->RangeDEF.ranges,
			    &pcie_def->DecodeDEF, DecodeDEF_default, pcie_def,
			    "DecodeDEF");
	    break;
	  }
	  case gs_color_space_index_CIEDEFG: {
	    gs_cie_defg *pcie_defg = pcs->params.defg;

	    pcie = (gs_cie_abc *)pcie_defg;
	    CIE_LOAD_CACHE_BODY(pcie_defg->caches_defg.DecodeDEFG, pcie_defg->RangeDEFG.ranges,
			    &pcie_defg->DecodeDEFG, DecodeDEFG_default, pcie_defg,
			    "DecodeDEFG");
	    break;
	  }
	  case gs_color_space_index_CIEABC: {
	    pcie = pcs->params.abc;
	    break;
	  }
	  default:
	    /* can't happen since we only come here for certain color spaces */
	    return false;
	}
	/* Fill the caches we need in the CIE color space */
        if ((code=gx_install_cie_abc((gs_cie_abc *)pcie, pgs)) >= 0) {
	    /* Now allocate the conversion imager state in stable_memory */
	    /* so that the garbage collector won't free it               */
	    code = gx_cie_to_xyz_alloc(&pdata->CIEtoXYZ_pis, pcs,
				    pcs->rc.memory->stable_memory);
	}
	if (code < 0) {
	    client_adjust_cspace_count(pcs, -1);  /* free it up */
	    return false;
	}
    }
    return true;
}


static bool
client_install_ICCtoXYZ(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs)
{
    int code;
    const gs_icc_params * picc_params = (const gs_icc_params *)&pcs->params.icc;
    gs_cie_icc *    picc_info = picc_params->picc_info;
    demo_color_space_data_t * pdata;

    if (pcs->pclient_color_space_data != NULL)
	return true;

    pdata = allocate_client_data_block(1, pcs->rc.memory->stable_memory);
    pcs->pclient_color_space_data = (client_color_space_data_t *) pdata;

	/* Need to initialize the client data.  The imager_state is what is needed in pdata->CIEtoXZY_ps */

    /* update the stub information used by the joint caches  */
    gx_cie_load_common_cache(&picc_info->common, pgs);
    gx_cie_common_complete(&picc_info->common);

    if ((code=gs_cie_cs_complete(pgs, true)) < 0) {
	client_adjust_cspace_count(pcs, -1);  /* free it up  */
	return false;
    }

	/* Now allocate the conversion imager state in stable_memory	*/
	/* so that the garbage collector won't free it			*/
	code = gx_cie_to_xyz_alloc(&pdata->CIEtoXYZ_pis, pcs,
				pcs->rc.memory->stable_memory);

	if (code < 0) {
	    client_adjust_cspace_count(pcs, -1);  /* free it up */
	    return false;
	}

    return true;
}


/*
 * Convert a CIEBasedA color into device color.
 */
static int
client_remap_CIEBasedA(client_custom_color_params_t * pparams,
    const gs_client_color * pc, const gs_color_space * pcs,
    gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
    gs_color_select_t select)
{
    demo_color_space_data_t * pdata =
	(demo_color_space_data_t *)(pcs->pclient_color_space_data);
    frac gray = convert2frac(pc->paint.values[0], pcs->params.a->RangeA);

    /*** Demonstrate method to convert to XYZ ***/
    if (pdata->CIEtoXYZ_pis) {
	frac xyz[3];

	cs_concretize_color(pc, pcs, xyz, pdata->CIEtoXYZ_pis);
	/* We don't really do anything with these values, but this */
	/* is where a real client could convert to a device color  */
	if_debug4('|', "[c]client_remap CIEA [%g] -> XYZ [%g, %g, %g]\n",
		  pc->paint.values[0],
		  frac2float(xyz[0]), frac2float(xyz[1]), frac2float(xyz[2]));

    }
    /*
     * For demo and debug purposes, make our colors a function of the
     * intensity of the given color value and the object type.
     */
    return client_remap_DeviceGray(pparams, &gray, pcs, pdc, pis, dev, select);
}

/*
 * Convert a CIEBasedABC color into device color.
 */
static int
client_remap_CIEBasedABC(client_custom_color_params_t * pparams,
    const gs_client_color * pc, const gs_color_space * pcs,
    gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
    gs_color_select_t select)
{
    demo_color_space_data_t * pdata =
	(demo_color_space_data_t *)(pcs->pclient_color_space_data);
    frac rgb[3];
    int i;

    /*** Demonstrate method to convert to XYZ ***/
    if (pdata->CIEtoXYZ_pis) {
	frac xyz[3];

	cs_concretize_color(pc, pcs, xyz, pdata->CIEtoXYZ_pis);
	/* We don't really do anything with these values, but this */
	/* is where a real client could convert to a device color  */
	if_debug6('|', "[c]client_remap CIEABC [%g, %g, %g] -> XYZ [%g, %g, %g]\n",
		  pc->paint.values[0], pc->paint.values[1], pc->paint.values[2],
		  frac2float(xyz[0]), frac2float(xyz[1]), frac2float(xyz[2]));
    }
    /*
     * For demo and debug purposes, make our colors a function of the
     * intensity of the given color value and the object type.  The color
     * values could represent almost anything.  However we are assuming
     * that they are RGB values.
     */
    for (i = 0; i < 3; i++)
	rgb[i] = convert2frac(pc->paint.values[i],
		       	pcs->params.abc->RangeABC.ranges[i]);
    return client_remap_DeviceRGB(pparams, rgb, pcs, pdc, pis, dev, select);
}

/*
 * Convert a CIEBasedDEF color into device color.
 */
static int
client_remap_CIEBasedDEF(client_custom_color_params_t * pparams,
    const gs_client_color * pc, const gs_color_space * pcs,
    gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
    gs_color_select_t select)
{
    demo_color_space_data_t * pdata =
	(demo_color_space_data_t *)(pcs->pclient_color_space_data);
    frac rgb[3];
    int i;

    /*** Demonstrate method to convert to XYZ ***/
    if (pdata->CIEtoXYZ_pis) {
	frac xyz[3];

	cs_concretize_color(pc, pcs, xyz, pdata->CIEtoXYZ_pis);
	/* We don't really do anything with these values, but this */
	/* is where a real client could convert to a device color  */
	if_debug6('|', "[c]client_remap CIEDEF [%g, %g, %g] -> XYZ [%g, %g, %g]\n",
		  pc->paint.values[0], pc->paint.values[1], pc->paint.values[2],
		  frac2float(xyz[0]), frac2float(xyz[1]), frac2float(xyz[2]));
    }
    /*
     * For demo and debug purposes, make our colors a function of the
     * intensity of the given color value and the object type.  The color
     * values could represent almost anything.  However we are assuming
     * that they are RGB values.
     */
    for (i = 0; i < 3; i++)
	rgb[i] = convert2frac(pc->paint.values[i],
		       	pcs->params.def->RangeDEF.ranges[i]);
    return client_remap_DeviceRGB(pparams, rgb, pcs, pdc, pis, dev, select);
}

/*
 * Convert a CIEBasedDEFG color into device color.
 */
static int
client_remap_CIEBasedDEFG(client_custom_color_params_t * pparams,
    const gs_client_color * pc, const gs_color_space * pcs,
    gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
    gs_color_select_t select)
{
    demo_color_space_data_t * pdata =
	(demo_color_space_data_t *)(pcs->pclient_color_space_data);
    frac cmyk[4];
    int i;

    /*** Demonstrate method to convert to XYZ ***/
    if (pdata->CIEtoXYZ_pis) {
	frac xyz[3];

	cs_concretize_color(pc, pcs, xyz, pdata->CIEtoXYZ_pis);
	/* We don't really do anything with these values, but this */
	/* is where a real client could convert to a device color  */
	if_debug7('|', "[c]client_remap CIEDEFG [%g, %g, %g] -> XYZ [%g, %g, %g]\n",
		  pc->paint.values[0], pc->paint.values[1], pc->paint.values[2],
		  pc->paint.values[3],
		  frac2float(xyz[0]), frac2float(xyz[1]), frac2float(xyz[2]));
    }
    /*
     * For demo and debug purposes, make our colors a function of the
     * intensity of the given color value and the object type.  The color
     * values could represent almost anything.  However we are assuming
     * that they are CMYK values.
     */
    for (i = 0; i < 4; i++)
	cmyk[i] = convert2frac(pc->paint.values[i],
		       	pcs->params.defg->RangeDEFG.ranges[i]);
    return client_remap_DeviceRGB(pparams, cmyk, pcs, pdc, pis, dev, select);
}

/*
 * Convert a ICCBased color into device color.
 */
static int
client_remap_ICCBased(client_custom_color_params_t * pparams,
    const gs_client_color * pc, const gs_color_space * pcs,
    gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
    gs_color_select_t select)
{
    demo_color_space_data_t * pdata =
	(demo_color_space_data_t *)(pcs->pclient_color_space_data);
    frac frac_color[GS_CLIENT_COLOR_MAX_COMPONENTS];
    int i, num_values = pcs->params.icc.picc_info->num_components;

    /*** Demonstrate method to convert to XYZ ***/
    if (pdata->CIEtoXYZ_pis) {

		frac xyz[3];

		cs_concretize_color(pc, pcs, xyz, pdata->CIEtoXYZ_pis);
		/* We don't really do anything with these values, but this */
		/* is where a real client could convert to a device color  */
		if_debug6('|', "[c]client_remap ICCBased [%g, %g, %g] -> XYZ [%g, %g, %g]\n",
			  pc->paint.values[0], pc->paint.values[1], pc->paint.values[2],
			  frac2float(xyz[0]), frac2float(xyz[1]), frac2float(xyz[2]));
    }

    /*
     * For demo and debug purposes, make our colors a function of the
     * intensity of the given color value and the object type.  The color
     * values could represent almost anything.  However based upon the
     * number of color values, we are assuming that they are either
     * gray, RGB, or CMYK values.
     */
    for (i = 0; i < num_values; i++)
	frac_color[i] = convert2frac(pc->paint.values[i],
		       	pcs->params.icc.picc_info->Range.ranges[i]);
    switch (num_values) {
	case 0:
	case 2:
	    return_error(gs_error_rangecheck);
	case 1:
	    return client_remap_DeviceGray(pparams, frac_color, pcs,
			   		 pdc, pis, dev, select);
	case 3:
	    return client_remap_DeviceRGB(pparams, frac_color, pcs,
			   		 pdc, pis, dev, select);
	case 4:
	default:
	    return client_remap_DeviceCMYK(pparams, frac_color, pcs,
			   		 pdc, pis, dev, select);
    }
}

#undef convert2frac

#endif 		/* NOT PANTONE_ONLY */

#if PANTONE_ONLY

	/*
	 * For PANTONE colors, we only need to handle Separation and DeviceN
	 * color spaces.  These are the only color spaces that can have PANTONE
	 * colors.
	 */
	client_custom_color_procs_t demo_procs = {
		client_install_no_op,		/* DeviceGray */
		NULL,
		client_install_no_op,		/* DeviceRGB */
		NULL,
		client_install_no_op,		/* DeviceCMYK */
		NULL,
		client_pantone_install_Separation,	/* Separation */
		client_pantone_remap_Separation,
		client_pantone_install_DeviceN,	/* DeviceN */
		client_pantone_remap_DeviceN,
		client_install_no_op,		/* CIEBasedA */
		NULL,
		client_install_no_op,		/* CIEBasedABC */
		NULL,
		client_install_no_op,		/* CIEBasedDEF */
		NULL,
		client_install_no_op,		/* CIEBasedDEFG */
		NULL,
		client_install_no_op,		/* ICCBased */
		NULL
	};

#else			/* Not PANTONE_ONLY special */

/*
 * Client call back procedures for our demo which illustrates color
 * processing based upon object type.
 */
client_custom_color_procs_t demo_procs = {
    client_install_DeviceGray,		/* DeviceGray */
    client_remap_DeviceGray,
    client_install_DeviceRGB,		/* DeviceRGB */
    client_remap_DeviceRGB,
    client_install_DeviceCMYK,		/* DeviceCMYK */
    client_remap_DeviceCMYK,
    client_pantone_install_Separation,	/* Separation */
    client_pantone_remap_Separation,
    client_pantone_install_DeviceN,	/* DeviceN */
    client_pantone_remap_DeviceN,
    client_install_CIEtoA,		/* CIEBasedA */
    client_remap_CIEBasedA,
    client_install_CIEtoXYZ,		/* CIEBasedABC */
    client_remap_CIEBasedABC,
    client_install_CIEtoXYZ,		/* CIEBasedDEF */
    client_remap_CIEBasedDEF,
    client_install_CIEtoXYZ,		/* CIEBasedDEFG */
    client_remap_CIEBasedDEFG,
    client_install_ICCtoXYZ,		/* ICCBased */
    client_remap_ICCBased
};

#endif		/* PANTONE_ONLY_EXAMPLE */

#endif		/* ENABLE_CUSTOM_COLOR_CALLBACK */
