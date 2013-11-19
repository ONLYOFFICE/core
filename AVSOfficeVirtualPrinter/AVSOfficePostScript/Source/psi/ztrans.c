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

/* $Id: ztrans.c 9043 2008-08-28 22:48:19Z giles $ */
/* Transparency operators */
#include "string_.h"
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gscspace.h"		/* for gscolor2.h */
#include "gscolor2.h"
#include "gsipar3x.h"
#include "gstrans.h"
#include "gxiparam.h"		/* for image enumerator */
#include "gxcspace.h"
#include "idict.h"
#include "idparam.h"
#include "ifunc.h"
#include "igstate.h"
#include "iimage.h"
#include "iname.h"
#include "store.h"
#include "gsdfilt.h"
#include "gdevdevn.h"
#include "gxblend.h"
#include "gdevp14.h"

/* ------ Utilities ------ */

static int
set_float_value(i_ctx_t *i_ctx_p, int (*set_value)(gs_state *, floatp))
{
    os_ptr op = osp;
    double value;
    int code;

    if (real_param(op, &value) < 0)
	return_op_typecheck(op);
    if ((code = set_value(igs, value)) < 0)
	return code;
    pop(1);
    return 0;
}

static int
current_float_value(i_ctx_t *i_ctx_p,
		    float (*current_value)(const gs_state *))
{
    os_ptr op = osp;

    push(1);
    make_real(op, current_value(igs));
    return 0;
}

static int
enum_param(const gs_memory_t *mem, const ref *pnref,
	   const char *const names[])
{
    const char *const *p;
    ref nsref;

    name_string_ref(mem, pnref, &nsref);
    for (p = names; *p; ++p)
	if (r_size(&nsref) == strlen(*p) &&
	    !memcmp(*p, nsref.value.const_bytes, r_size(&nsref))
	    )
	    return p - names;
    return_error(e_rangecheck);
}

/* ------ Graphics state operators ------ */

static const char *const blend_mode_names[] = {
    GS_BLEND_MODE_NAMES, 0
};

/* <modename> .setblendmode - */
static int
zsetblendmode(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    int code;

    check_type(*op, t_name);
    if ((code = enum_param(imemory, op, blend_mode_names)) < 0 ||
	(code = gs_setblendmode(igs, code)) < 0
	)
	return code;
    pop(1);
    return 0;
}

/* - .currentblendmode <modename> */
static int
zcurrentblendmode(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    const char *mode_name = blend_mode_names[gs_currentblendmode(igs)];
    ref nref;
    int code = name_enter_string(imemory, mode_name, &nref);

    if (code < 0)
	return code;
    push(1);
    *op = nref;
    return 0;
}

/* <0..1> .setopacityalpha - */
static int
zsetopacityalpha(i_ctx_t *i_ctx_p)
{
    return set_float_value(i_ctx_p, gs_setopacityalpha);
}

/* - .currentopacityalpha <0..1> */
static int
zcurrentopacityalpha(i_ctx_t *i_ctx_p)
{
    return current_float_value(i_ctx_p, gs_currentopacityalpha);
}

/* <0..1> .setshapealpha - */
static int
zsetshapealpha(i_ctx_t *i_ctx_p)
{
    return set_float_value(i_ctx_p, gs_setshapealpha);
}

/* - .currentshapealpha <0..1> */
static int
zcurrentshapealpha(i_ctx_t *i_ctx_p)
{
    return current_float_value(i_ctx_p, gs_currentshapealpha);
}

/* <bool> .settextknockout - */
static int
zsettextknockout(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    check_type(*op, t_boolean);
    gs_settextknockout(igs, op->value.boolval);
    pop(1);
    return 0;
}

/* - .currenttextknockout <bool> */
static int
zcurrenttextknockout(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;

    push(1);
    make_bool(op, gs_currenttextknockout(igs));
    return 0;
}

/* ------ Rendering stack operators ------ */

static int
rect_param(gs_rect *prect, os_ptr op)
{
    double coords[4];
    int code = num_params(op, 4, coords);

    if (code < 0)
	return code;
    prect->p.x = coords[0], prect->p.y = coords[1];
    prect->q.x = coords[2], prect->q.y = coords[3];
    return 0;
}

static int
mask_op(i_ctx_t *i_ctx_p,
	int (*mask_proc)(gs_state *, gs_transparency_channel_selector_t))
{
    int csel;
    int code = int_param(osp, 1, &csel);

    if (code < 0)
	return code;
    code = mask_proc(igs, csel);
    if (code >= 0)
	pop(1);
    return code;

}

/* <paramdict> <llx> <lly> <urx> <ury> .begintransparencygroup - */
static int
zbegintransparencygroup(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    os_ptr dop = op - 4;
    gs_transparency_group_params_t params;
    gs_rect bbox;
    int code;

    check_type(*dop, t_dictionary);
    check_dict_read(*dop);
    gs_trans_group_params_init(&params);
    if ((code = dict_bool_param(dop, "Isolated", false, &params.Isolated)) < 0 ||
	(code = dict_bool_param(dop, "Knockout", false, &params.Knockout)) < 0 ||
	(code = dict_bool_param(dop, ".image_with_SMask", false, &params.image_with_SMask)) < 0
	)
	return code;
    code = rect_param(&bbox, op);
    if (code < 0)
	return code;
    params.ColorSpace = gs_currentcolorspace(igs);
    code = gs_begin_transparency_group(igs, &params, &bbox);
    if (code < 0)
	return code;
    pop(5);
    return code;
}

/* - .discardtransparencygroup - */
static int
zdiscardtransparencygroup(i_ctx_t *i_ctx_p)
{
    if (gs_current_transparency_type(igs) != TRANSPARENCY_STATE_Group)
	return_error(e_rangecheck);
    return gs_discard_transparency_layer(igs);
}

/* - .endtransparencygroup - */
static int
zendtransparencygroup(i_ctx_t *i_ctx_p)
{
    return gs_end_transparency_group(igs);
}

/* <paramdict> <llx> <lly> <urx> <ury> .begintransparencymaskgroup - */
static int tf_using_function(floatp, float *, void *);
static int
zbegintransparencymaskgroup(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    os_ptr dop = op - 4;
    gs_transparency_mask_params_t params;
    ref *pparam;
    gs_rect bbox;
    int code;
    static const char *const subtype_names[] = {
	GS_TRANSPARENCY_MASK_SUBTYPE_NAMES, 0
    };

    check_type(*dop, t_dictionary);
    check_dict_read(*dop);
    if (dict_find_string(dop, "Subtype", &pparam) <= 0)
	return_error(e_rangecheck);
    if ((code = enum_param(imemory, pparam, subtype_names)) < 0)
	return code;
    gs_trans_mask_params_init(&params, code);
    params.replacing = true;
    if ((code = dict_floats_param(imemory, dop, "Background",
		    cs_num_components(gs_currentcolorspace(i_ctx_p->pgs)),
				  params.Background, NULL)) < 0
	)
	return code;
    else if (code > 0)
	params.Background_components = code;
    if ((code = dict_floats_param(imemory, dop, "GrayBackground",
		    1, &params.GrayBackground, NULL)) < 0
	)
	return code;
    if (dict_find_string(dop, "TransferFunction", &pparam) >0) {
	gs_function_t *pfn = ref_function(pparam);

	if (pfn == 0 || pfn->params.m != 1 || pfn->params.n != 1)
	    return_error(e_rangecheck);
	params.TransferFunction = tf_using_function;
	params.TransferFunction_data = pfn;
    }
    code = rect_param(&bbox, op);
    if (code < 0)
	return code;
    code = gs_begin_transparency_mask(igs, &params, &bbox, false);
    if (code < 0)
	return code;
    pop(5);
    return code;
}

/* - .begintransparencymaskimage - */
static int
zbegintransparencymaskimage(i_ctx_t *i_ctx_p)
{
    gs_transparency_mask_params_t params;
    gs_rect bbox = { { 0, 0} , { 1, 1} };
    int code;

    gs_trans_mask_params_init(&params, TRANSPARENCY_MASK_Luminosity);
    code = gs_begin_transparency_mask(igs, &params, &bbox, true);
    if (code < 0)
	return code;
    return code;
}

/* Implement the TransferFunction using a Function. */
static int
tf_using_function(floatp in_val, float *out, void *proc_data)
{
    float in = in_val;
    gs_function_t *const pfn = proc_data;

    return gs_function_evaluate(pfn, &in, out);
}

/* - .discardtransparencymask - */
static int
zdiscardtransparencymask(i_ctx_t *i_ctx_p)
{
    if (gs_current_transparency_type(igs) != TRANSPARENCY_STATE_Mask)
	return_error(e_rangecheck);
    return gs_discard_transparency_layer(igs);
}

/* <mask#> .endtransparencymask - */
static int
zendtransparencymask(i_ctx_t *i_ctx_p)
{
    return mask_op(i_ctx_p, gs_end_transparency_mask);
}

/* ------ Soft-mask images ------ */

/* <dict> .image3x - */
static int mask_dict_param(const gs_memory_t *mem, os_ptr,
			    image_params *, const char *, int,
			    gs_image3x_mask_t *);
static int
zimage3x(i_ctx_t *i_ctx_p)
{
    os_ptr op = osp;
    gs_image3x_t image;
    ref *pDataDict;
    image_params ip_data;
    int num_components =
	gs_color_space_num_components(gs_currentcolorspace(igs));
    int ignored;
    int code;

    check_type(*op, t_dictionary);
    check_dict_read(*op);
    gs_image3x_t_init(&image, NULL);
    if (dict_find_string(op, "DataDict", &pDataDict) <= 0)
	return_error(e_rangecheck);
    if ((code = pixel_image_params(i_ctx_p, pDataDict,
		   (gs_pixel_image_t *)&image, &ip_data,
		   16, false, gs_currentcolorspace(igs))) < 0 ||
	(code = dict_int_param(pDataDict, "ImageType", 1, 1, 0, &ignored)) < 0
	)
	return code;
    /*
     * We have to process the masks in the reverse order, because they
     * insert their DataSource before the one(s) for the DataDict.
     */
    if ((code = mask_dict_param(imemory, op, &ip_data,
				"ShapeMaskDict", num_components,
				&image.Shape)) < 0 ||
	(code = mask_dict_param(imemory, op, &ip_data,
				"OpacityMaskDict", num_components,
				&image.Opacity)) < 0
	)
	return code;
    return zimage_setup(i_ctx_p, (gs_pixel_image_t *)&image,
			&ip_data.DataSource[0],
			image.CombineWithColor, 1);
}

/* Get one soft-mask dictionary parameter. */
static int
mask_dict_param(const gs_memory_t *mem, os_ptr op,
image_params *pip_data, const char *dict_name,
		int num_components, gs_image3x_mask_t *pixm)
{
    ref *pMaskDict;
    image_params ip_mask;
    int ignored;
    int code, mcode;

    if (dict_find_string(op, dict_name, &pMaskDict) <= 0)
	return 1;
    if ((mcode = code = data_image_params(mem, pMaskDict, &pixm->MaskDict,
					  &ip_mask, false, 1, 16, false)) < 0 ||
	(code = dict_int_param(pMaskDict, "ImageType", 1, 1, 0, &ignored)) < 0 ||
	(code = dict_int_param(pMaskDict, "InterleaveType", 1, 3, -1,
			       &pixm->InterleaveType)) < 0 ||
	(code = dict_floats_param(mem, op, "Matte", num_components,
				  pixm->Matte, NULL)) < 0
	)
	return code;
    pixm->has_Matte = code > 0;
    /*
     * The MaskDict must have a DataSource iff InterleaveType == 3.
     */
    if ((pip_data->MultipleDataSources && pixm->InterleaveType != 3) ||
	ip_mask.MultipleDataSources ||
	mcode != (pixm->InterleaveType != 3)
	)
	return_error(e_rangecheck);
    if (pixm->InterleaveType == 3) {
	/* Insert the mask DataSource before the data DataSources. */
	memmove(&pip_data->DataSource[1], &pip_data->DataSource[0],
		(countof(pip_data->DataSource) - 1) *
		sizeof(pip_data->DataSource[0]));
	pip_data->DataSource[0] = ip_mask.DataSource[0];
    }
    return 0;
}

/* depth .pushpdf14devicefilter - */
/* this is a filter operator, but we include it here to maintain
   modularity of the pdf14 transparency support */
static int
zpushpdf14devicefilter(i_ctx_t *i_ctx_p)
{
    int code;
    os_ptr op = osp;

    check_type(*op, t_integer);
    code = gs_push_pdf14trans_device(igs);
    if (code < 0)
        return code;
    pop(1);
    return 0;
}

/* this is a filter operator, but we include it here to maintain
   modularity of the pdf14 transparency support */
static int
zpoppdf14devicefilter(i_ctx_t *i_ctx_p)
{
    return gs_pop_pdf14trans_device(igs);
}

/* ------ Initialization procedure ------ */

/* We need to split the table because of the 16-element limit. */
const op_def ztrans1_op_defs[] = {
    {"1.setblendmode", zsetblendmode},
    {"0.currentblendmode", zcurrentblendmode},
    {"1.setopacityalpha", zsetopacityalpha},
    {"0.currentopacityalpha", zcurrentopacityalpha},
    {"1.setshapealpha", zsetshapealpha},
    {"0.currentshapealpha", zcurrentshapealpha},
    {"1.settextknockout", zsettextknockout},
    {"0.currenttextknockout", zcurrenttextknockout},
    op_def_end(0)
};
const op_def ztrans2_op_defs[] = {
    {"5.begintransparencygroup", zbegintransparencygroup},
    {"0.discardtransparencygroup", zdiscardtransparencygroup},
    {"0.endtransparencygroup", zendtransparencygroup},
    {"5.begintransparencymaskgroup", zbegintransparencymaskgroup},
    {"5.begintransparencymaskimage", zbegintransparencymaskimage},
    {"0.discardtransparencymask", zdiscardtransparencymask},
    {"1.endtransparencymask", zendtransparencymask},
    {"1.image3x", zimage3x},
    {"1.pushpdf14devicefilter", zpushpdf14devicefilter},
    {"0.poppdf14devicefilter", zpoppdf14devicefilter},
    op_def_end(0)
};
