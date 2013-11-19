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

/* $Id: gxistate.h 8552 2008-02-27 18:39:31Z leonardo $ */
/* Imager state definition */

#ifndef gxistate_INCLUDED
#  define gxistate_INCLUDED

#include "gscsel.h"
#include "gsrefct.h"
#include "gsropt.h"
#include "gstparam.h"
#include "gxcvalue.h"
#include "gxcmap.h"
#include "gxfixed.h"
#include "gxline.h"
#include "gxmatrix.h"
#include "gxtmap.h"
#include "gscspace.h"
#include "gstrans.h"
#include "gsnamecl.h"

/*
  Define the subset of the PostScript graphics state that the imager library
  API needs.  The intended division between the two state structures is that
  the imager state contain only information that (1) is not part of the
  parameters for individual drawing commands at the gx_ interface (i.e.,
  will likely be different for each drawing call), and (2) is not an
  artifact of the PostScript language (i.e., doesn't need to burden the
  structure when it is being used for other imaging, specifically for
  imaging a command list).  While this criterion is somewhat fuzzy, it leads
  us to INCLUDE the following state elements:
	line parameters: cap, join, miter limit, dash pattern
	transformation matrix (CTM)
	logical operation: RasterOp, transparency
	color modification: alpha, rendering algorithm
  	transparency information:
  	    blend mode
  	    (opacity + shape) (alpha + cached mask)
  	    text knockout flag
  	    rendering stack
	overprint control: overprint flag, mode, and effective mode
	rendering tweaks: flatness, fill adjustment, stroke adjust flag,
	  accurate curves flag, shading smoothness
	color rendering information:
	    halftone, halftone phases
	    transfer functions
	    black generation, undercolor removal
	    CIE rendering tables
	    halftone and pattern caches
  	shared (constant) device color spaces
  We EXCLUDE the following for reason #1 (drawing command parameters):
	path
	clipping path and stack
	color specification: color, color space, substitute color spaces
	font
	device
  We EXCLUDE the following for reason #2 (specific to PostScript):
	graphics state stack
	default CTM
	clipping path stack
  In retrospect, perhaps the device should have been included in the
  imager state, but we don't think this change is worth the trouble now.
 */

/*
 * Define the color rendering state information.
 * This should be a separate object (or at least a substructure),
 * but making this change would require editing too much code.
 */

/* Opaque types referenced by the color rendering state. */
#ifndef gs_halftone_DEFINED
#  define gs_halftone_DEFINED
typedef struct gs_halftone_s gs_halftone;
#endif
#ifndef gx_device_color_DEFINED
#  define gx_device_color_DEFINED
typedef struct gx_device_color_s gx_device_color;
#endif
#ifndef gx_device_halftone_DEFINED
#  define gx_device_halftone_DEFINED
typedef struct gx_device_halftone_s gx_device_halftone;
#endif
#ifndef gs_color_space_DEFINED
#  define gs_color_space_DEFINED
typedef struct gs_color_space_s gs_color_space;
#endif

/*
 * We need some special memory management for the components of a
 * c.r. state, as indicated by the following notations on the elements:
 *      (RC) means the element is reference-counted.
 *      (Shared) means the element is shared among an arbitrary number of
 *        c.r. states and is never freed.
 *      (Owned) means exactly one c.r. state references the element,
 *        and it is guaranteed that no references to it will outlive
 *        the c.r. state itself.
 */

/* Define the interior structure of a transfer function. */
typedef struct gx_transfer_s {
    int red_component_num;
    gx_transfer_map *red;		/* (RC) */
    int green_component_num;
    gx_transfer_map *green;		/* (RC) */
    int blue_component_num;
    gx_transfer_map *blue;		/* (RC) */
    int gray_component_num;
    gx_transfer_map *gray;		/* (RC) */
} gx_transfer;

#define gs_color_rendering_state_common\
\
		/* Halftone screen: */\
\
	gs_halftone *halftone;			/* (RC) */\
	gs_int_point screen_phase[gs_color_select_count];\
		/* dev_ht depends on halftone and device resolution. */\
	gx_device_halftone *dev_ht;		/* (RC) */\
\
		/* Color (device-dependent): */\
\
	struct gs_cie_render_s *cie_render;	/* (RC) may be 0 */\
	bool cie_to_xyz;			/* flag for conversion to XYZ, no CRD req'd */\
	gx_transfer_map *black_generation;	/* (RC) may be 0 */\
	gx_transfer_map *undercolor_removal;	/* (RC) may be 0 */\
		/* set_transfer holds the transfer functions specified by */\
		/* set[color]transfer; effective_transfer includes the */\
		/* effects of overrides by TransferFunctions in halftone */\
		/* dictionaries.  (In Level 1 systems, set_transfer and */\
		/* effective_transfer are always the same.) */\
	gx_transfer set_transfer;		/* members are (RC) */\
	gx_transfer_map *effective_transfer[GX_DEVICE_COLOR_MAX_COMPONENTS]; /* see below */\
	int object_tag; /* */\
\
		/* Color caches: */\
\
		/* cie_joint_caches depend on cie_render and */\
		/* the color space. */\
	struct gx_cie_joint_caches_s *cie_joint_caches;		/* (RC) */\
		/* cmap_procs depend on the device's color_info. */\
	const struct gx_color_map_procs_s *cmap_procs;		/* static */\
		/* DeviceN component map for current color space */\
	gs_devicen_color_map color_component_map;\
		/* The contents of pattern_cache depend on the */\
		/* the color space and the device's color_info and */\
		/* resolution. */\
	struct gx_pattern_cache_s *pattern_cache;	/* (Shared) by all gstates */\
\
	/* Simple color spaces, stored here for easy access from */ 	\
	/* gx_concrete_space_CIE */ \
	gs_color_space *devicergb_cs;\
	gs_color_space *devicecmyk_cs

/*
 * Enumerate the reference-counted pointers in a c.r. state.  Note that
 * effective_transfer doesn't contribute to the reference count: it points
 * either to the same objects as set_transfer, or to objects in a halftone
 * structure that someone else worries about.
 */
#define gs_cr_state_do_rc_ptrs(m)\
  m(halftone) m(dev_ht) m(cie_render)\
  m(black_generation) m(undercolor_removal)\
  m(set_transfer.red) m(set_transfer.green)\
  m(set_transfer.blue) m(set_transfer.gray)\
  m(cie_joint_caches)\
  m(devicergb_cs) m(devicecmyk_cs)

/* Enumerate the pointers in a c.r. state. */
#define gs_cr_state_do_ptrs(m)\
  m(0,halftone) m(1,dev_ht)\
  m(2,cie_render) m(3,black_generation) m(4,undercolor_removal)\
  m(5,set_transfer.red) m(6,set_transfer.green)\
  m(7,set_transfer.blue) m(8,set_transfer.gray)\
  m(9,cie_joint_caches) m(10,pattern_cache)\
  m(11,devicergb_cs) m(12,devicecmyk_cs)
  /*
   * We handle effective_transfer specially in gsistate.c since its pointers
   * are not enumerated for garbage collection but they are are relocated.
   */
/*
 * This count does not include the effective_transfer pointers since they
 * are not enumerated for GC.
 */
#define st_cr_state_num_ptrs 13


typedef struct gs_devicen_color_map_s {
    bool use_alt_cspace;
    separation_type sep_type;
    uint num_components;	/* Input - Duplicate of value in gs_device_n_params */
    uint num_colorants;		/* Number of colorants - output */ 
    gs_id cspace_id;		/* Used to verify color space and map match */
    int color_map[GS_CLIENT_COLOR_MAX_COMPONENTS];
} gs_devicen_color_map;


/* Define the imager state structure itself. */
/*
 * Note that the ctm member is a gs_matrix_fixed.  As such, it cannot be
 * used directly as the argument for procedures like gs_point_transform.
 * Instead, one must use the ctm_only macro, e.g., &ctm_only(pis) rather
 * than &pis->ctm.
 */
#define gs_imager_state_common\
	gs_memory_t *memory;\
	void *client_data;\
	gx_line_params line_params;\
	gs_matrix_fixed ctm;\
	bool current_point_valid;\
	gs_point current_point;\
	gs_point subpath_start;\
	bool clamp_coordinates;\
	gs_logical_operation_t log_op;\
	gx_color_value alpha;\
	gs_blend_mode_t blend_mode;\
	gs_transparency_source_t opacity, shape;\
	gs_id soft_mask_id;\
	bool text_knockout;\
	uint text_rendering_mode;\
	gs_transparency_state_t *transparency_stack;\
	bool overprint;\
	int overprint_mode;\
	int effective_overprint_mode;\
	float flatness;\
	gs_fixed_point fill_adjust; /* A path expansion for fill; -1 = dropout prevention*/\
	bool stroke_adjust;\
	bool accurate_curves;\
	bool have_pattern_streams;\
	float smoothness;\
	int renderingintent; /* See gsstate.c */\
	CUSTOM_COLOR_PTR	/* Pointer to custom color callback struct */\
	const gx_color_map_procs *\
	  (*get_cmap_procs)(const gs_imager_state *, const gx_device *);\
	gs_color_rendering_state_common
#define st_imager_state_num_ptrs\
  (st_line_params_num_ptrs + st_cr_state_num_ptrs + 2)
/* Access macros */
#define ctm_only(pis) (*(const gs_matrix *)&(pis)->ctm)
#define ctm_only_writable(pis) (*(gs_matrix *)&(pis)->ctm)
#define set_ctm_only(pis, mat) (*(gs_matrix *)&(pis)->ctm = (mat))
#define gs_init_rop(pis) ((pis)->log_op = lop_default)
#define gs_currentflat_inline(pis) ((pis)->flatness)
#define gs_currentlineparams_inline(pis) (&(pis)->line_params)
#define gs_current_logical_op_inline(pis) ((pis)->log_op)
#define gs_set_logical_op_inline(pis, lop) ((pis)->log_op = (lop))

#ifndef gs_imager_state_DEFINED
#  define gs_imager_state_DEFINED
typedef struct gs_imager_state_s gs_imager_state;
#endif

struct gs_imager_state_s {
    gs_imager_state_common;
};

/* Initialization for gs_imager_state */
#define gs_imager_state_initial(scale)\
  0, 0, { gx_line_params_initial },\
   { (float)(scale), 0.0, 0.0, (float)(-(scale)), 0.0, 0.0 },\
  false, {0, 0}, {0, 0}, false, \
  lop_default, gx_max_color_value, BLEND_MODE_Compatible,\
  { 1.0 }, { 1.0 }, 0, 0/*false*/, 0, 0, 0/*false*/, 0, 0, 1.0,  \
   { fixed_half, fixed_half }, 0/*false*/, 0/*false*/, 0/*false*/, 1.0,\
  1, INIT_CUSTOM_COLOR_PTR	/* 'Custom color' callback pointer */  \
  gx_default_get_cmap_procs

/* The imager state structure is public only for subclassing. */
#define public_st_imager_state()	/* in gsistate.c */\
  gs_public_st_composite(st_imager_state, gs_imager_state, "gs_imager_state",\
    imager_state_enum_ptrs, imager_state_reloc_ptrs)

/* Initialize an imager state, other than the parts covered by */
/* gs_imager_state_initial. */
int gs_imager_state_initialize(gs_imager_state * pis, gs_memory_t * mem);

/* Make a temporary copy of a gs_imager_state.  Note that this does not */
/* do all the necessary reference counting, etc. */
gs_imager_state *
    gs_imager_state_copy(const gs_imager_state * pis, gs_memory_t * mem);

/* Increment reference counts to note that an imager state has been copied. */
void gs_imager_state_copied(gs_imager_state * pis);

/* Adjust reference counts before assigning one imager state to another. */
void gs_imager_state_pre_assign(gs_imager_state *to,
				const gs_imager_state *from);


/* Release an imager state. */
void gs_imager_state_release(gs_imager_state * pis);
int gs_currentscreenphase_pis(const gs_imager_state *, gs_int_point *, gs_color_select_t);

#endif /* gxistate_INCLUDED */
