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
/* $Id: gsnamecl.h 8764 2008-05-21 18:27:42Z mvrhel $ */
/* Global definitions for the 'Named Color' callback handling. */

#ifndef gsnamecl_INCLUDED
# define gsnamecl_INCLUDED

#include "gxfrac.h"
#include "gsccolor.h"
#include "gscsel.h"
#include "gxcspace.h"

/*
 * Enable custom client callback color processing.  Note:  There is a sample
 * implementation in src/gsncdummy.c.
 */
#ifndef ENABLE_CUSTOM_COLOR_CALLBACK
#define ENABLE_CUSTOM_COLOR_CALLBACK 0	/* 0 --> disabled, 1 --> enabled */
#endif

#if ENABLE_CUSTOM_COLOR_CALLBACK
/* Ptr to custom color callback struct */
#define CUSTOM_COLOR_PTR void * custom_color_callback;
#define INIT_CUSTOM_COLOR_PTR   NULL,		/* Initial value */
#else
#define CUSTOM_COLOR_PTR
#define INIT_CUSTOM_COLOR_PTR
#endif

#define CustomColorCallbackParamName "CustomColorCallback"

/*
 * For comments upon the client API for working with the custom client
 * callback logic see the section labeled:  "CLIENT COLOR CALLBACK 
 * APPLICATION INTERFACE" below.
 *
 * Also see the comments at the start of src/gsnamecl.c
 */

#ifndef gs_color_space_DEFINED
#  define gs_color_space_DEFINED
typedef struct gs_color_space_s gs_color_space;
#endif

#ifndef gx_device_color_DEFINED
#  define gx_device_color_DEFINED
typedef struct gx_device_color_s gx_device_color;
#endif

#ifndef gs_state_DEFINED
#  define gs_state_DEFINED
typedef struct gs_state_s gs_state;
#endif

#ifndef gs_imager_state_DEFINED
#  define gs_imager_state_DEFINED
typedef struct gs_imager_state_s gs_imager_state;
#endif

#ifndef gx_device_DEFINED
#  define gx_device_DEFINED
typedef struct gx_device_s gx_device;
#endif

#ifndef i_ctx_t_DEFINED
#define i_ctx_t_DEFINED
typedef struct gs_context_state_s i_ctx_t;
#endif

/* Define an opaque type for parameter lists. */
#ifndef gs_param_list_DEFINED
#  define gs_param_list_DEFINED
typedef struct gs_param_list_s gs_param_list;
#endif

#define cs_proc_adjust_client_cspace_count(proc)\
  void proc(const gs_color_space *, int)

/*
 * Put the 'custom color' client callback parameter block pointer.  This value
 * is passed as a string type user paramter.  A string is being used since
 * PostScript does not support pointers as a type.  Note:  An integer type
 * is not being used since PS integers are nominally 32 bits.  Thus there
 * would be a problem using integers to pass pointer values on 64 bit systems.
 */
int custom_color_callback_put_params(gs_state * pgs, gs_param_list * plist);

/*
 * Get the custom client client callback parameter block pointer.  This value
 * is passed as a string type user paramter.  A string is being used since
 * PostScript does not support pointers as a type.  Note:  An integer type
 * is not being used since PS intergers are nominally 32 bits.  Thus there
 * would be a problem using integers to pass pointer values on 64 bit systems.
 */
int custom_color_callback_get_params(gs_state * pgs, gs_param_list * plist);

/*
 * Check if we want to use the callback color processing logic for the given
 * Separation color space.
 */
bool custom_color_callback_install_Separation(gs_color_space * pcs,
	       						gs_state * pgs);

/*
 * Check if we want to use the custom client callback processing logic for the
 * given DeviceN color space.
 */
bool custom_color_callback_install_DeviceN(gs_color_space * pcs, gs_state * pgs);

/*
 * Convert a Separation color into device colorants using the custom client
 * client callback.
 */
int gx_remap_concrete_custom_color_Separation(const frac * pconc,
	const gs_color_space * pcs, gx_device_color * pdc,
       	const gs_imager_state * pis, gx_device * dev, gs_color_select_t select);

/*
 * Convert a DeviceN color into device colorants using the custom client
 * client callback.
 */
int gx_remap_concrete_custom_color_DeviceN(const frac * pconc,
	const gs_color_space * pcs, gx_device_color * pdc,
       	const gs_imager_state * pis, gx_device * dev, gs_color_select_t select);

/* "CLIENT COLOR CALLBACK APPLICATION INTERFACE" */
/*
 * In order to give some flexibility to the Ghostscript client API, we are
 * allowing the client to define a set of call back procedures for processing
 * color spaces.
 *
 * See the comments at the start of src/gsnamecl.c
 */
/*
 * The 'client color' call back structure definitions.  The call back structure
 * consists of a pointer to a list of client color space handling procedures
 * and a pointer to a client data structure.
 */

#ifndef client_custom_color_params_DEFINED
#define client_custom_color_params_DEFINED
typedef struct client_custom_color_params_s {
    /* Client callback handlers */
    struct client_custom_color_procs_s * client_procs;
    /* For global client data */
    void * data;
} client_custom_color_params_t;
#endif


/*
 * Define a base type for client color space data.  Most clients will
 * overload this type with a structure of their own.  That type must
 * start with a pointer to a handler routine for the structure's
 * reference count.
 */
typedef struct client_color_space_data_s {
	cs_proc_adjust_client_cspace_count((*client_adjust_cspace_count));
} client_color_space__data_t;

/*
 * Define the client custom client callback procedures.
 */
typedef struct client_custom_color_procs_s {
    /*
     * Install a DeviceGray color space.
     */
    bool (* install_DeviceGray)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a DeviceGray color into device color.
     */
    int (* remap_DeviceGray)(client_custom_color_params_t * pparams,
	    const frac * pconc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Install a DeviceRGB color space.
     */
    bool (* install_DeviceRGB)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a DeviceRGB color into device color.
     */
    int (* remap_DeviceRGB)(client_custom_color_params_t * pparams,
	    const frac * pconc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Install a DeviceCMYK color space.
     */
    bool (* install_DeviceCMYK)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a DeviceGray color into device color.
     */
    int (* remap_DeviceCMYK)(client_custom_color_params_t * pparams,
	    const frac * pconc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Check if we want to use the callback color processing logic for the
     * given Separation color space.
     */
    bool (* install_Separation)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a Separation color into device color.
     */
    int (* remap_Separation)(client_custom_color_params_t * pparams,
	    const frac * pconc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Check if we want to use the callback color processing logic for the
     * given DeviceN color space.
     */
    bool (* install_DeviceN)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a DeviceN color into device color.
     */
    int (* remap_DeviceN)(client_custom_color_params_t * pparams,
	    const frac * pconc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Check if we want to use the callback color processing logic for the
     * given CIEBasedA color space.
     */
    bool (* install_CIEBasedA)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Please note that the 'complex' color spaces (CIEBasedA, CIEBasedABC,
     * CIEBasedDEF, CIEBasedDEFG, and ICCBased) have a different prototype,
     * versus the simpler color spces, for the callback for converting a
     * the color values.
     */
    /*
     * Convert a CIEBasedA color into device color.
     */
    int (* remap_CIEBasedA)(client_custom_color_params_t * pparams,
	    const gs_client_color * pc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Check if we want to use the callback color processing logic for the
     * given CIEBasedABC color space.
     */
    bool (* install_CIEBasedABC)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a CIEBasedABC color into device color.
     */
    int (* remap_CIEBasedABC)(client_custom_color_params_t * pparams,
	    const gs_client_color * pc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Check if we want to use the callback color processing logic for the
     * given CIEBasedDEF color space.
     */
    bool (* install_CIEBasedDEF)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a CIEBasedDEF color into device color.
     */
    int (* remap_CIEBasedDEF)(client_custom_color_params_t * pparams,
	    const gs_client_color * pc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Check if we want to use the callback color processing logic for the
     * given CIEBasedDEFG color space.
     */
    bool (* install_CIEBasedDEFG)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a CIEBasedDEFG color into device color.
     */
    int (* remap_CIEBasedDEFG)(client_custom_color_params_t * pparams,
	    const gs_client_color * pc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);
    /*
     * Check if we want to use the callback color processing logic for the
     * given ICCBased color space.
     */
    bool (* install_ICCBased)(client_custom_color_params_t * pparams,
	    gs_color_space * pcs, gs_state * pgs);
    /*
     * Convert a ICCBased color into device color.
     */
    int (* remap_ICCBased)(client_custom_color_params_t * pparams,
	    const gs_client_color * pc, const gs_color_space * pcs,
	    gx_device_color * pdc, const gs_imager_state * pis,
	    gx_device * dev, gs_color_select_t select);

} client_custom_color_procs_t;
    
#endif		/* ifndef gsnamecl_INCLUDED */
