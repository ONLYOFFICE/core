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

/* $Id: gsicc.c 8780 2008-05-27 20:12:08Z mvrhel $ */
/* Implementation of the ICCBased color space family */

#include "math_.h"
#include "memory_.h"
#include "gx.h"
#include "gserrors.h"
#include "gsstruct.h"
#include "stream.h"
#include "gxcspace.h"		/* for gxcie.c */
#include "gxarith.h"
#include "gxcie.h"
#include "gzstate.h"
#include "icc.h"		/* must precede icc.h */
#include "gsicc.h"

#define SAVEICCPROFILE 0

typedef struct _icmFileGs icmFileGs;

struct _icmFileGs {
    ICM_FILE_BASE

    /* Private: */
    stream *strp;
};

/* Garbage collection code */

/*
 * Discard a gs_cie_icc_s structure. This requires that we call the
 * destructor for ICC profile, lookup, and file objects (which are
 * stored in "foreign" memory).
 *
 * No special action is taken with respect to the stream pointer; that is
 * the responsibility of the client.  */
static void
cie_icc_finalize(void * pvicc_info)
{
    gs_cie_icc *    picc_info = (gs_cie_icc *)pvicc_info;

    if (picc_info->plu != NULL) {
        picc_info->plu->del(picc_info->plu);
        picc_info->plu = NULL;
    }
    if (picc_info->picc != NULL) {
        picc_info->picc->del(picc_info->picc);
        picc_info->picc = NULL;
    }
    if (picc_info->pfile != NULL) {
        picc_info->pfile->del(picc_info->pfile);
        picc_info->pfile = NULL;
    }
}

private_st_cie_icc();

/*
 * Because the color space structure stores alternative color space in-line,
 * we must enumerate and relocate pointers in these space explicity.
 */
gs_private_st_composite( st_color_space_CIEICC,
                         gs_color_space,
                         "gs_color_space_CIEICC",
                         cs_CIEICC_enum_ptrs,
                         cs_CIEICC_reloc_ptrs );

/* pointer enumeration routine */
static
ENUM_PTRS_BEGIN(cs_CIEICC_enum_ptrs) return 0;
        ENUM_PTR(0, gs_color_space, params.icc.picc_info);
ENUM_PTRS_END

/* pointer relocation routine */
static
RELOC_PTRS_BEGIN(cs_CIEICC_reloc_ptrs)
    RELOC_PTR(gs_color_space, params.icc.picc_info);
RELOC_PTRS_END


/*
 * Color space methods for ICCBased color spaces.
 *
 * As documented, ICCBased color spaces may be used as both base and
 * alternative color spaces. Futhermore,, they can themselves contain paint
 * color spaces as alternative color space. In this implementation we allow
 * them to be used as base and alternative color spaces, but only to contain
 * "small" base color spaces (CIEBased or smaller). This arrangement avoids
 * breaking the color space heirarchy. Providing a more correct arrangement
 * requires a major change in the color space mechanism.
 *
 * Several of the methods used by ICCBased color space apply as well to
 * DeviceN color spaces, in that they are generic to color spaces having
 * a variable number of components. We have elected not to attempt to 
 * extract and combine these operations, because this would save only a
 * small amount of code, and much more could be saved by intorducing certain
 * common elements (ranges, number of components, etc.) into the color space
 * root class.
 */
static cs_proc_num_components(gx_num_components_CIEICC);
static cs_proc_init_color(gx_init_CIEICC);
static cs_proc_restrict_color(gx_restrict_CIEICC);
static cs_proc_concrete_space(gx_concrete_space_CIEICC);
static cs_proc_concretize_color(gx_concretize_CIEICC);
#if ENABLE_CUSTOM_COLOR_CALLBACK
static cs_proc_remap_color(gx_remap_ICCBased);
#endif
static cs_proc_final(gx_final_CIEICC);
static cs_proc_serialize(gx_serialize_CIEICC);

static const gs_color_space_type gs_color_space_type_CIEICC = {
    gs_color_space_index_CIEICC,    /* index */
    true,                           /* can_be_base_space */
    true,                           /* can_be_alt_space */
    &st_color_space_CIEICC,         /* stype - structure descriptor */
    gx_num_components_CIEICC,       /* num_components */
    gx_init_CIEICC,                 /* init_color */
    gx_restrict_CIEICC,             /* restrict_color */
    gx_concrete_space_CIEICC,       /* concrete_space */
    gx_concretize_CIEICC,           /* concreteize_color */
    NULL,                           /* remap_concrete_color */
#if ENABLE_CUSTOM_COLOR_CALLBACK
    gx_remap_ICCBased,		    /* remap_color */
#else
    gx_default_remap_color,         /* remap_color */
#endif
    gx_install_CIE,                 /* install_cpsace */
    gx_spot_colors_set_overprint,   /* set_overprint */
    gx_final_CIEICC,                /* final */
    gx_no_adjust_color_count,       /* adjust_color_count */
    gx_serialize_CIEICC,		    /* serialize */
    gx_cspace_is_linear_default
};


/*
 * Return the number of components used by a ICCBased color space - 1, 3, or 4
 */
static int
gx_num_components_CIEICC(const gs_color_space * pcs)
{
    return pcs->params.icc.picc_info->num_components;
}

/*
 * Set the initial client color for an ICCBased color space. The convention
 * suggested by the ICC specification is to set all components to 0.
 */
static void
gx_init_CIEICC(gs_client_color * pcc, const gs_color_space * pcs)
{
    int     i, ncomps = pcs->params.icc.picc_info->num_components;

    for (i = 0; i < ncomps; ++i)
	pcc->paint.values[i] = 0.0;

    /* make sure that [ 0, ... 0] is in range */
    gx_restrict_CIEICC(pcc, pcs);
}

/*
 * Restrict an color to the range specified for an ICCBased color space.
 */
static void
gx_restrict_CIEICC(gs_client_color * pcc, const gs_color_space * pcs)
{
    int                 i, ncomps = pcs->params.icc.picc_info->num_components;
    const gs_range *    ranges = pcs->params.icc.picc_info->Range.ranges;

    for (i = 0; i < ncomps; ++i) {
        floatp  v = pcc->paint.values[i];
        floatp  rmin = ranges[i].rmin, rmax = ranges[i].rmax;

        if (v < rmin)
            pcc->paint.values[i] = rmin;
        else if (v > rmax)
            pcc->paint.values[i] = rmax;
    }
}

/*
 * Return the conrecte space to which this color space will map. If the
 * ICCBased color space is being used in native mode, the concrete space
 * will be dependent on the current color rendering dictionary, as it is
 * for all CIE bases. If the alternate color space is being used, then
 * this question is passed on the the appropriate method of that space.
 */
static const gs_color_space *
gx_concrete_space_CIEICC(const gs_color_space * pcs, const gs_imager_state * pis)
{
    if (pcs->params.icc.picc_info->picc == NULL) {
        const gs_color_space *  pacs = pcs->base_space;

        return cs_concrete_space(pacs, pis);
    } else
        return gx_concrete_space_CIE(NULL, pis);
}

/*
 * Convert an ICCBased color space to a concrete color space.
 */
static int
gx_concretize_CIEICC(
    const gs_client_color * pcc,
    const gs_color_space *  pcs,
    frac *                  pconc,
    const gs_imager_state * pis )
{
    const gs_icc_params *   picc_params = &pcs->params.icc;
    const gs_cie_icc *      picc_info = picc_params->picc_info;
    stream *                instrp = picc_info->instrp;
    icc *                   picc = picc_info->picc;
    double                  inv[4], outv[3];
    cie_cached_vector3      vlmn;
    gs_client_color         lcc = *pcc;
    int                     i, ncomps = picc_info->num_components;
    int code;

    /* use the altenate space concretize if appropriate */
    if (picc == NULL)
        return pcs->base_space->type->concretize_color(
                            pcc,
                            pcs->base_space,
                            pconc,
                            pis );

    /* set up joint cache as required */
    code = gx_cie_check_rendering(pcs, pconc, pis);
    if (code < 0)
	return code;
    if (code == 1)
	return 0;

    /* verify and update the stream pointer */
    if (picc_info->file_id != (instrp->read_id | instrp->write_id))
        return_error(gs_error_ioerror);
    ((icmFileGs *)picc->fp)->strp = instrp;

    /* translate the input components */
    gx_restrict_CIEICC(&lcc, pcs);
    for (i = 0; i < ncomps; i++)
        inv[i] = lcc.paint.values[i];
		
	/* Since the original limits were wrong for this case, We need to adjust things a bit different */

    /* For input Lab color space massage the values into Lab range */

   /* if (picc_info->plu->e_inSpace == icSigLabData) {

        inv[0] *= 100;
        inv[1] = inv[1]*255 - 128;
        inv[2] = inv[2]*255 - 128; 

    } */

    /*
     * Perform the lookup operation. A return value of 1 indicates that
     * clipping occurred somewhere in the operation, but the result is
     * legitimate. Other non-zero return values indicate an error, which
     * should not occur in practice.
     */

    if (picc_info->plu->lookup(picc_info->plu, outv, inv) > 1)
        return_error(gs_error_unregistered);

    /* if the output is in the CIE L*a*b* space, convert to XYZ */
    if (picc_info->pcs_is_cielab) {
        floatp              f[3];
        const gs_vector3 *  pwhtpt = &picc_info->common.points.WhitePoint;


        f[1] = (outv[0] + 16.0) / 116.0;
        f[0] = f[1] + outv[1] / 500.0;
        f[2] = f[1] - outv[2] / 200;

        for (i = 0; i < 3; i++) {
            if (f[i] >= 6.0 / 29.0)
                outv[i] = f[i] * f[i] * f[i];
            else
                outv[i] = 108.0 * (f[i] - 4.0 / 29.0) / 841.0;
        }

        /*
         * The connection space white-point is known to be D50, but we
         * use the more general form in case of future revisions.
         */
        outv[0] *= pwhtpt->u;
        outv[1] *= pwhtpt->v;
        outv[2] *= pwhtpt->w;
    }

    /* translate the output */
    vlmn.u = float2cie_cached(outv[0]);
    vlmn.v = float2cie_cached(outv[1]);
    vlmn.w = float2cie_cached(outv[2]);

    gx_cie_remap_finish(vlmn, pconc, pis, pcs);
    return 0;
}

#if ENABLE_CUSTOM_COLOR_CALLBACK
/*
 * This routine is only used if ENABLE_CUSTOM_COLOR_CALLBACK is true.
 * Otherwise we use gx_default_remap_color directly for CIEBasedDEFG color
 * spaces.
 *
 * Render a CIEBasedDEFG color.
 */
int
gx_remap_ICCBased(const gs_client_color * pc, const gs_color_space * pcs,
	gx_device_color * pdc, const gs_imager_state * pis, gx_device * dev,
		gs_color_select_t select)
{
    client_custom_color_params_t * pcb =
	    (client_custom_color_params_t *) (pis->memory->gs_lib_ctx->custom_color_callback);

    if (pcb != NULL) {
	if (pcb->client_procs->remap_ICCBased(pcb, pc, pcs,
			   			pdc, pis, dev, select) == 0)
	    return 0;
    }
    /* Use default routine for non custom color processing. */
    return gx_default_remap_color(pc, pcs, pdc, pis, dev, select);
}
#endif

/*
 * Finalize the contents of an ICC color space. Now that color space
 * objects have straightforward reference counting discipline, there's
 * nothing special about it. In the previous state of affairs, the
 * argument in favor of correct reference counting spoke of "an
 * unintuitive but otherwise legitimate state of affairs".
 */
static void
gx_final_CIEICC(const gs_color_space * pcs)
{
    rc_decrement_only(pcs->params.icc.picc_info, "gx_final_CIEICC");
}

static int
icmFileGs_seek(icmFile *pp, long int offset)
{
    icmFileGs *p = (icmFileGs *)pp;

    return spseek(p->strp, offset);
}

static size_t
icmFileGs_read(icmFile *pp, void *buffer, size_t size, size_t count)
{
    icmFileGs *p = (icmFileGs *)pp;
    uint    tot;
    int     status = sgets(p->strp, buffer, size * count, &tot);

    return (status < 0) ? status : tot;
}

static size_t
icmFileGs_write(icmFile *pp, void *buffer, size_t size, size_t count)
{
    icmFileGs *p = (icmFileGs *)pp;
    uint    tot;
    int     status = sputs(p->strp, buffer, size * count, &tot);

    return (status < 0) ? status : tot;
}

static int
icmFileGs_flush(icmFile *pp)
{
    icmFileGs *p = (icmFileGs *)pp;

    return s_std_write_flush(p->strp);
}

static int
icmFileGs_delete(icmFile *pp)
{
    free(pp);
    return 0;
}

/**
 * gx_wrap_icc_stream: Wrap a Ghostscript stream as an icclib file.
 * @strp: The Ghostscript stream.
 *
 * Creates an icmFile object that wraps @stream.
 *
 * Note: the memory for this object is allocated using malloc, and the
 * relocation of the stream pointer is done lazily, before an icclu
 * operation. It would probably be cleaner to allocate the icmFile in
 * garbage collected memory, and have the relocation happen there, but
 * I wanted to minimally modify Jan's working code.
 *
 * Return value: the stream wrapped as an icmFile object, or NULL on
 * error.
 **/
static icmFile *
gx_wrap_icc_stream(stream *strp)
{
    icmFileGs *p;

    if ((p = (icmFileGs *) calloc(1,sizeof(icmFileGs))) == NULL)
	return NULL;
    p->seek  = icmFileGs_seek;
    p->read  = icmFileGs_read;
    p->write = icmFileGs_write;
    p->flush = icmFileGs_flush;
    p->del   = icmFileGs_delete;

    p->strp = strp;

    return (icmFile *)p;
}

int
gx_load_icc_profile(gs_cie_icc *picc_info)
{
    stream *        instrp = picc_info->instrp;
    icc *           picc;
    icmLuBase * plu = NULL;
    icmFile *pfile = NULL;

#if   SAVEICCPROFILE

    unsigned int num_bytes;
    unsigned char *iccbuffer;
    FILE *fid;

#endif
	
    /* verify that the file is legitimate */
    if (picc_info->file_id != (instrp->read_id | instrp->write_id))
	return_error(gs_error_ioerror);
    /*
     * Load the top-level ICC profile.
     *
     * If an ICC profile fails to load, generate an error.
     * 
     * Testing demonstrates, however, Acrobat Reader silently
     * ignores the error and uses the alternate color space.
     * This behaviour is implemented by catching the error using
     * a stopped context from within the interpreter (gs_icc.ps).
     *
     * Failure to allocate the top-level profile object is considered
     * a limitcheck rather than a VMerror, as profile data structures
     * are stored in "foreign" memory.
     */
    if ((picc = new_icc()) == NULL)
	return_error(gs_error_limitcheck);
    {
	icProfileClassSignature profile_class;
	icColorSpaceSignature   cspace_type;
	gs_vector3 *            ppt;

	pfile = gx_wrap_icc_stream (instrp);
      
	if ((picc->read(picc, pfile, 0)) != 0)
	    goto return_rangecheck;

#if SAVEICCPROFILE

        num_bytes = picc->header->size;
        iccbuffer = (unsigned char *) malloc(num_bytes);
        pfile->seek(pfile,0);
        pfile->read(pfile,iccbuffer,1,num_bytes);
        fid = fopen("DumpedICC.icm","wb");
        fwrite(iccbuffer,sizeof(unsigned char),num_bytes,fid);
        fclose(fid);
        free(iccbuffer);

#endif
        
            
	/* verify the profile type */
	profile_class = picc->header->deviceClass;
	if ( profile_class != icSigInputClass     &&
	     profile_class != icSigDisplayClass   &&
	     profile_class != icSigOutputClass    &&
	     profile_class != icSigColorSpaceClass  )
	    goto return_rangecheck;

	/* verify the profile connection space */
	cspace_type = picc->header->pcs;
	if (cspace_type == icSigLabData)
	    picc_info->pcs_is_cielab = true;
	else if (cspace_type == icSigXYZData)
	    picc_info->pcs_is_cielab = false;
	else
	    goto return_rangecheck;

	/* verify the source color space */
	cspace_type = picc->header->colorSpace;
	if (cspace_type == icSigCmykData) {
	    if (picc_info->num_components != 4)
		goto return_rangecheck;
	} else if ( cspace_type == icSigRgbData ||
		    cspace_type == icSigLabData   ) {
	    if (picc_info->num_components != 3)
		goto return_rangecheck;
	} else if (cspace_type == icSigGrayData) {
	    if (picc_info->num_components != 1)
		goto return_rangecheck;
	}

	/*
	 * Fetch the lookup object.
	 *
	 * PostScript and PDF deal with rendering intent as strictly a
	 * rendering dictionary facility. ICC profiles allow a rendering
	 * intent to be specified for both the input (device ==> pcs) and
	 * output (pcs ==> device) operations. Hence, when using ICCBased
	 * color spaces with PDF, no clue is provided as to which source
	 * mapping to select.
	 *
	 * In the absence of other information, there are two possible
	 * selections. If our understanding is correct, when relative
	 * colorimetry is specified, the icclib code will map source
	 * color values to XYZ or L*a*b* values such that the relationship
	 * of the source color, relative to the source white and black
	 * points, will be the same as the output colors and the
	 * profile connection space illuminant (currently always D50)
	 * and pure black ([0, 0, 0]). In this case, the white and black
	 * points that should be listed in the color space are the
	 * profile connection space illuminant (D50) and pure black.
	 *
	 * If absolute colorimetry is employed, the XYZ or L*a*b* values
	 * generated will be absolute in the chromatic sense (they are
	 * not literally "absolute", as we still must have overall
	 * intensity information inorder to determine weighted spectral
	 * power levels). To achieve relative colorimetry for the output,
	 * these colors must be evaluated relative to the source white
	 * and black points. Hence, in this case, the appropriate white
	 * and black points to list in the color space are the source
	 * white and black points provided in the profile tag array.
	 *
	 * In this implementation, we will always request relative
	 * colorimetry from the icclib, and so will use the profile
	 * connection space illuminant and pure black as the white and
	 * black points of the color space. This approach is somewhat
	 * simpler, as it allows the color space white point to also
	 * be used for L*a*b* to XYZ conversion (otherwise we would
	 * need to store the profile connection space illuminant
	 * separately for that purpose). The approach does reduce to
	 * to some extent the range of mappings that can be achieved
	 * via the color rendering dictionary, but for now we believe
	 * this loss is not significant.
	 *
	 * For reasons that are not clear to us, the icclib code does
	 * not support relative colorimetry for all color profiles. For
	 * this reason, we specify icmDefaultIntent rather than
	 * icRelativeColormetric.
	 *
	 * NB: We are not color experts; our understanding of this area
	 *     may well be incorrect.
	 */
	plu = picc->get_luobj( picc,
			       icmFwd,
			       icmDefaultIntent,
			       0, /* PCS override */
			       icmLuOrdNorm );
	if (plu == NULL)
	    goto return_rangecheck;

	/* 
	 * Get the appropriate white and black points. See the note on
	 * rendering intent above for a discussion of why we are using
	 * the profile space illuminant and pure black. (Pure black need
	 * not be set explicitly, as it is the default.)
	 */
	ppt = &picc_info->common.points.WhitePoint;
	ppt->u = picc->header->illuminant.X;
	ppt->v = picc->header->illuminant.Y;
	ppt->w = picc->header->illuminant.Z;

	picc_info->picc = picc;
	picc_info->plu = plu;
	picc_info->pfile = pfile;
    }

    return 0;

 return_rangecheck:
    if (plu != NULL)
	plu->del(plu);
    if (picc != NULL)
	picc->del(picc);
    if (pfile != NULL)
	pfile->del(pfile);
    return_error(gs_error_rangecheck);
}

/*
 * Install an ICCBased color space.
 *
 * Note that an ICCBased color space must be installed before it is known if
 * the ICC profile or the alternate color space is to be used.
 */
static int
gx_install_CIEICC(gs_color_space * pcs, gs_state * pgs)
{
    const gs_icc_params * picc_params = (const gs_icc_params *)&pcs->params.icc;
    gs_cie_icc *    picc_info = picc_params->picc_info;

#if ENABLE_CUSTOM_COLOR_CALLBACK
    {
        /*
         * Check if we want to use the callback color processing for this
         * color space.
         */
        client_custom_color_params_t * pcb =
	    (client_custom_color_params_t *) pgs->memory->gs_lib_ctx->custom_color_callback;

        if (pcb != NULL) {
	    if (pcb->client_procs->install_ICCBased(pcb, pcs, pgs))
    	        /* Exit if the client will handle the colorspace completely */
		return 0;
        }
    }
#endif
    /* update the stub information used by the joint caches */
    gx_cie_load_common_cache(&picc_info->common, pgs);
    gx_cie_common_complete(&picc_info->common);
    return gs_cie_cs_complete(pgs, true);
}


/*
 * Constructor for ICCBased color space. As with the other color space
 * constructors, this provides only minimal initialization.
 */
int
gs_cspace_build_CIEICC(
    gs_color_space **   ppcspace,
    void *              client_data,
    gs_memory_t *       pmem )
{
    gs_cie_icc *        picc_info;
    gs_color_space *    pcs;

    /*
     * The gs_cie_icc_s structure is the only CIE-based color space structure
     * which accesses additional memory for which it is responsible. We make
     * use of the finalization procedure to handle this task, so we can use
     * the generic CIE space build routine (otherwise we would need a
     * separate build routine that provided its own reference count freeing
     * procedure).
     */
    picc_info = gx_build_cie_space( ppcspace,
                                    &gs_color_space_type_CIEICC,
                                    &st_cie_icc,
                                    pmem );

    if (picc_info == NULL)
        return_error(gs_error_VMerror);

    gx_set_common_cie_defaults(&picc_info->common, client_data);
    /*
     * Now set the D50 WhitePoint. The above function does not set any
     * valid WhitepPoint since PostScript always requires this, but ICC
     * assumes a D50 WhitePoint as a default
     */
    picc_info->common.points.WhitePoint.u = (float)0.9642;		/* Profile illuminant - D50 */
    picc_info->common.points.WhitePoint.v = 1.0000;
    picc_info->common.points.WhitePoint.w = (float)0.8249;
    picc_info->common.install_cspace = gx_install_CIEICC;
    picc_info->num_components = 0;
    picc_info->Range = Range4_default;
    picc_info->instrp = NULL;
    picc_info->pcs_is_cielab = false;
    picc_info->picc = NULL;
    picc_info->plu = NULL;
    picc_info->pfile = NULL;

    pcs = *ppcspace;
    pcs->params.icc.picc_info = picc_info;
    return 0;
}

/* ---------------- Serialization. -------------------------------- */

static int 
gx_serialize_CIEICC(const gs_color_space * pcs, stream * s)
{
    const gs_icc_params * p = &pcs->params.icc;
    gs_cie_icc *picc = p->picc_info;
    uint n;
    int code = gx_serialize_cspace_type(pcs, s);
    long avail, pos, count;
    byte buf[100];

    if (code < 0)
	return code;
    code = gx_serialize_cie_common_elements(pcs, s);
    if (code < 0)
	return code;
    code = sputs(s, (byte *)&picc->num_components, sizeof(picc->num_components), &n);
    if (code < 0)
	return code;
    code = sputs(s, (byte *)&picc->Range, sizeof(picc->Range), &n);
    if (code < 0)
	return code;
    if (sseek(picc->instrp, 0) < 0)
	return_error(gs_error_unregistered); /* Unimplemented. */
    if (savailable(picc->instrp, &avail) != 0)
	return_error(gs_error_unregistered); /* Unimplemented. */
    code = sputs(s, (byte *)&avail, sizeof(avail), &n);
    if (code < 0)
	return code;
    for (pos = 0; pos < avail; pos += count) {
	count = min(sizeof(buf), avail - pos);
	code = sgets(picc->instrp, buf, count, &n);
	if (code < 0)
	    return code;
	code = sputs(s, buf, count, &n);
	if (code < 0)
	    return code;
    }
    return sputs(s, (byte *)&picc->pcs_is_cielab, sizeof(picc->pcs_is_cielab), &n);
}
