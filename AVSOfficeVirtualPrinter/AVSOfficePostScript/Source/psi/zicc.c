/* Copyright (C) 2001-2008 Artifex Software, Inc.
   All Rights Reserved.
  
   This software is provided AS-IS with no warranty, either express or
   implied.

   This software is distributed under license and may not be copied, modified
   or distributed except as expressly authorized under the terms of that
   license.  Refer to licensing information at http://www.artifex.com/
   or contact Artifex Software, Inc.,  7 Mt. Lassen Drive - Suite A-134,
   San Rafael, CA  94903, U.S.A., +1(415)492-9861, for further information.
*/

/* $Id: zicc.c 9380 2009-01-21 15:34:55Z ken $ */
/* ICCBased color operators */

#include "math_.h"
#include "memory_.h"
#include "ghost.h"
#include "oper.h"
#include "gsstruct.h"
#include "gxcspace.h"		/* gscolor2.h requires gscspace.h */
#include "stream.h"
#include "files.h"
#include "gscolor2.h"
#include "icc.h"			
#include "gsicc.h"
#include "estack.h"
#include "idict.h"
#include "idparam.h"
#include "igstate.h"
#include "icie.h"
#include "ialloc.h"
#include "zicc.h"

int seticc(i_ctx_t * i_ctx_p, int ncomps, ref *ICCdict, float *range_buff)
{
    os_ptr                  op = osp;
    int edepth = ref_stack_count(&e_stack);
    int                     code, reuse_op = 0;
    gs_color_space *        pcs;
    gs_color_space *  palt_cs;
    int                     i;
    gs_cie_icc *            picc_info;
    ref *                   pstrmval;
    stream *                s = 0L;

    palt_cs = gs_currentcolorspace(igs);
    /* verify the DataSource entry */
    if (dict_find_string(ICCdict, "DataSource", &pstrmval) <= 0)
        return_error(e_undefined);
    check_read_file(s, pstrmval);

    /* build the color space object */
    code = gs_cspace_build_CIEICC(&pcs, NULL, gs_state_memory(igs));
    if (code < 0)
        return code;
    picc_info = pcs->params.icc.picc_info;
    picc_info->num_components = ncomps;
    picc_info->instrp = s;
    picc_info->file_id = (s->read_id | s->write_id);
    for (i = 0; i < ncomps; i++) {
        picc_info->Range.ranges[i].rmin = range_buff[2 * i];
        picc_info->Range.ranges[i].rmax = range_buff[2 * i + 1];

    }

    /* record the current space as the alternative color space */
    pcs->base_space = palt_cs;
    rc_increment(palt_cs);

    code = gx_load_icc_profile(picc_info);
    if (code < 0)
	return code;

    /* If the input space to this profile is CIELAB, then we need to adjust the limits */
    /* See ICC spec ICC.1:2004-10 Section 6.3.4.2 and 6.4 */
    if(picc_info->plu->e_inSpace == icSigLabData)
    {
        picc_info->Range.ranges[0].rmin = 0.0;
        picc_info->Range.ranges[0].rmax = 100.0;

        picc_info->Range.ranges[1].rmin = -128.0;
        picc_info->Range.ranges[1].rmax = 127.0;

        picc_info->Range.ranges[2].rmin = -128.0;
        picc_info->Range.ranges[2].rmax = 127.0;

    } 
    /* If the input space is icSigXYZData, then we should do the limits based upon the white point of the profile.  */
    if(picc_info->plu->e_inSpace == icSigXYZData)
    {
	for (i = 0; i < 3; i++) 
	{
	    picc_info->Range.ranges[i].rmin = 0;
	}

	picc_info->Range.ranges[0].rmax = picc_info->common.points.WhitePoint.u;
	picc_info->Range.ranges[1].rmax = picc_info->common.points.WhitePoint.v;
    	picc_info->Range.ranges[2].rmax = picc_info->common.points.WhitePoint.w;
    }

    code = cie_cache_joint(i_ctx_p, &istate->colorrendering.procs,
			   (gs_cie_common *)picc_info, igs);
    if (code < 0)
	return code;

    return cie_set_finish( i_ctx_p,
                           pcs,
                           &istate->colorspace.procs.cie,
                           edepth,
                           code );
}

/*
 *   <dict>  .seticcspace  -
 *
 * Create an ICCBased color space and set it to be the current color space.
 *
 * The PostScript structure of an ICCBased color space is that same as that
 * for a CIEBased* color space:
 *
 *     [ /ICCBased <dictionary> ]
 *
 * As is the for other .setcie*space operators, the operand dictionary rather
 * than the complete color space array is on the stack when this operator
 * is inovked.
 *
 * At the time this procedure is called, the alternative color space for
 * the ICCBased color space is expected to be the current color space,
 * whether that space was explicitly specified or implied by the number
 * of components in the ICCBased color space dictionary. This is consistent
 * with the handling of alternative spaces in Separation, DeviceN, and
 * Indexed color spaces. Unlike the "zset*space" routines for those spaces,
 * however, the current code does not attempt to build the color space
 * "in place" in the graphic state.
 *
 * The procedure that invokes this operator will already have checked that
 * the operand is a dictionary, is readable, and defines the key /N
 * (number of components).
 */
static int
zseticcspace(i_ctx_t * i_ctx_p)
{
    os_ptr                  op = osp;
    int                     code;
    gs_color_space *  palt_cs;
    ref *                   pnval;
    ref *                   pstrmval;
    stream *                s;
    int                     i, ncomps;
    float                   range_buff[8];
    static const float      dflt_range[8] = { 0, 1,   0, 1,   0, 1,   0, 1 };

    code = dict_find_string(op, "N", &pnval);
    if (code < 0)
	return code;

    ncomps = pnval->value.intval;

    if (2*ncomps > sizeof(range_buff)/sizeof(range_buff[0]))
        return_error(e_rangecheck);

    /* verify the DataSource entry */
    if (dict_find_string(op, "DataSource", &pstrmval) <= 0)
        return_error(e_undefined);
    check_read_file(s, pstrmval);

    /*
     * Verify that the current color space can be a alternative color space.
     * The check for ICCBased color space is a hack to avoid introducing yet
     * another category indicator into the gs_color_space_type structur.
     */
    palt_cs = gs_currentcolorspace(igs);
    if ( !palt_cs->type->can_be_alt_space                                ||
         gs_color_space_get_index(palt_cs) == gs_color_space_index_CIEICC  )
        return_error(e_rangecheck);

    /*
     * Fetch and verify the Range array.
     *
     * The PDF documentation is unclear as to the purpose of this array.
     * Essentially all that is stated is that "These values must match the
     * information in the ICC profile" (PDF Reference, 2nd ed., p. 174).
     * If that is the case, why not use the information in the profile?
     * The only reason we can think of is range specification is intended
     * to be used to limit the range of values passed to the alternate
     * color space (the range may be smaller than the native range of values
     * provided by that color space).
     *
     * Because the icclib code will perform normalization based on color
     * space, we use the range values only to restrict the set of input
     * values; they are not used for normalization.
     */
    code = dict_floats_param( imemory, 
			      op,
                              "Range",
                              2 * ncomps,
                              range_buff,
                              dflt_range );
    for (i = 0; i < 2 * ncomps && range_buff[i + 1] >= range_buff[i]; i += 2)
        ;
    if (i != 2 * ncomps)
        return_error(e_rangecheck);

    return seticc(i_ctx_p, ncomps, op, range_buff);
}


const op_def    zicc_ll3_op_defs[] = {
    op_def_begin_ll3(),
    { "1.seticcspace", zseticcspace },
    op_def_end(0)
};
