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

/* $Id: gxiscale.c 9184 2008-10-25 04:34:30Z mvrhel $ */
/* Interpolated image procedures */
#include "gx.h"
#include "math_.h"
#include "memory_.h"
#include "stdint_.h"
#include "gpcheck.h"
#include "gserrors.h"
#include "gxfixed.h"
#include "gxfrac.h"
#include "gxarith.h"
#include "gxmatrix.h"
#include "gsccolor.h"
#include "gspaint.h"
#include "gxdevice.h"
#include "gxcmap.h"
#include "gxdcolor.h"
#include "gxistate.h"
#include "gxdevmem.h"
#include "gxcpath.h"
#include "gximage.h"
#include "stream.h"		/* for s_alloc_state */
#include "siinterp.h"		/* for spatial interpolation */
#include "siscale.h"		/* for Mitchell filtering */
#include "sidscale.h"		/* for special case downscale filter */
#include "vdtrace.h"
#include "gscindex.h"           /* included for proper handling of index color spaces 
                                and keeping data in source color space */
#include "gxcolor2.h"           /* define of float_color_to_byte_color */
#include "gscspace.h"           /* Needed for checking is space is CIE */


static void 
decode_sample_frac_to_float(gx_image_enum *penum, frac sample_value, gs_client_color *cc, int i);

    /*
 * Define whether we are using Mitchell filtering or spatial
 * interpolation to implement Interpolate.  (The latter doesn't work yet.)
 */
#define USE_MITCHELL_FILTER

/* ------ Strategy procedure ------ */

/* Check the prototype. */
iclass_proc(gs_image_class_0_interpolate);

/* If we're interpolating, use special logic. 
   This function just gets interpolation stucture
   initialized and allocates buffer space if needed */
static irender_proc(image_render_interpolate);
irender_proc_t
gs_image_class_0_interpolate(gx_image_enum * penum)
{
    gs_memory_t *mem = penum->memory;
    stream_image_scale_params_t iss;
    stream_image_scale_state *pss;
    const stream_template *template;
    byte *line;
    const gs_color_space *pcs = penum->pcs;
    gs_point dst_xy;
    uint in_size;

    if (!penum->interpolate) 
	return 0;
    if (penum->use_mask_color || penum->posture != image_portrait ||
    	penum->masked || penum->alpha) {
	/* We can't handle these cases yet.  Punt. */
	penum->interpolate = false;
	return 0;
    }

/*
 * USE_CONSERVATIVE_INTERPOLATION_RULES is normally NOT defined since
 * the MITCHELL digital filter seems OK as long as we are going out to
 * a device that can produce > 15 shades.
 */
#if defined(USE_MITCHELL_FILTER) && defined(USE_CONSERVATIVE_INTERPOLATION_RULES)
    /*
     * We interpolate using a digital filter, rather than Adobe's
     * spatial interpolation algorithm: this produces very bad-looking
     * results if the input resolution is close to the output resolution,
     * especially if the input has low color resolution, so we resort to
     * some hack tests on the input color resolution and scale to suppress
     * interpolation if we think the result would look especially bad.
     * If we used Adobe's spatial interpolation approach, we wouldn't need
     * to do this, but the spatial interpolation filter doesn't work yet.
     */
    if (penum->bps < 4 || penum->bps * penum->spp < 8 ||
	(fabs(penum->matrix.xx) <= 5 && fabs(penum->matrix.yy <= 5))
	) {
	penum->interpolate = false;
	return 0;
    }
#endif
    /* Non-ANSI compilers require the following casts: */
    gs_distance_transform((float)penum->rect.w, (float)penum->rect.h,
			  &penum->matrix, &dst_xy);
    iss.BitsPerComponentOut = sizeof(frac) * 8;
    iss.MaxValueOut = frac_1;
    iss.WidthOut = (int)ceil(fabs(dst_xy.x));
    iss.HeightOut = fixed2int_pixround_perfect((fixed)((int64_t)(penum->rect.y + penum->rect.h) * 
						penum->dst_height / penum->Height))
	- fixed2int_pixround_perfect((fixed)((int64_t)penum->rect.y * penum->dst_height / penum->Height));
    iss.HeightOut = any_abs(iss.HeightOut);
    iss.WidthIn = penum->rect.w;
    iss.HeightIn = penum->rect.h;
    iss.src_y_offset = penum->rect.y;
    iss.EntireWidthIn = penum->Width;
    iss.EntireHeightIn = penum->Height;
    iss.EntireWidthOut = fixed2int_pixround(any_abs(penum->dst_width));
    iss.EntireHeightOut = fixed2int_pixround(any_abs(penum->dst_height));

    /* If we are in an indexed space then we need to use the number of components
       in the base space.  Otherwise we use the number of components in the source space */

    if (pcs->type->index == gs_color_space_index_Indexed) {

        /* Use the number of colors in the base space */
        iss.Colors = cs_num_components(pcs->base_space);

    } else {

        /* Use the number of colors that exist in the source space
        as this is where we are doing our interpolation */
        iss.Colors = cs_num_components(pcs);

    }

    if (penum->bps <= 8 ) {

       /* If the input is ICC or other device independent format, go ahead 
          and do the interpolation in that space. 
          If we have more than 8 bits per channel then we will need to 
          handle that in a slightly different manner so
          that the interpolation algorithm handles it properly. 
          The interpolation will still be in the source 
          color space.  Note that if image data was less the 8 bps
          It is handed here to us in 8 bit form already decoded. */

	iss.BitsPerComponentIn = 8;
	iss.MaxValueIn = 0xff;

        /* If it is an index color space we will need to allocate for the decoded data */

       if (pcs->type->index == gs_color_space_index_Indexed) {
        
           in_size = iss.WidthIn * iss.Colors;

       } else {

           /* Non indexed case, we either use the data as 
           is, or allocate space if it is reversed in X */

	    in_size =
	        (penum->matrix.xx < 0 ?
	         /* We need a buffer for reversing each scan line. */
	         iss.WidthIn * iss.Colors : 0);  

            /* If it is not reversed, and we have 8 bit/color channel data then            
            no need to allocate extra as we will use the source directly */

            /* However, if we have a nonstandard encoding and are in 
                a device color space we will need to allocate
               in that case also. We will maintain 8 bits but
               do the decode and then interpolate.  This is OK
               for the linear decode */

            if (!penum->device_color && !gs_color_space_is_CIE(pcs)){

                in_size = iss.WidthIn * iss.Colors;

            }

       }

    } else {

        /* If it has more than 8 bits per color channel then we will go to frac 
           for the interpolation to mantain precision.  */

	iss.BitsPerComponentIn = sizeof(frac) * 8;
	iss.MaxValueIn = frac_1;
	in_size = round_up(iss.WidthIn * iss.Colors * sizeof(frac),
			   align_bitmap_mod);   
        /* Size to allocate space to store the input as frac type */
    }
#ifdef USE_MITCHELL_FILTER
    template = &s_IScale_template;
#else
    template = &s_IIEncode_template;
#endif
    if (((penum->dev->color_info.num_components == 1 &&
	  penum->dev->color_info.max_gray < 15) ||
	 (penum->dev->color_info.num_components > 1 &&
	  penum->dev->color_info.max_color < 15))
	) {
	/* halftone device -- restrict interpolation */
	if ((iss.WidthOut < iss.WidthIn * 4) && (iss.HeightOut < iss.HeightIn * 4)) {
	    if ((iss.WidthOut < iss.WidthIn) && (iss.HeightOut < iss.HeightIn) &&	/* downsampling */
		(penum->dev->color_info.polarity != GX_CINFO_POLARITY_UNKNOWN)) {	/* colorspace OK */
		/* Special case handling for when we are downsampling to a dithered device	*/
		/* The point of this non-linear downsampling is to preserve dark pixels		*/
		/* from the source image to avoid dropout. The color polarity is used for this	*/
		template = &s_ISpecialDownScale_template;
	    } else {
		penum->interpolate = false;
		return 0;	/* no interpolation / downsampling */
	    }
	}
	/* else, continue with the Mitchell filter (for upscaling of at least 4:1) */
    }
    /* The SpecialDownScale filter needs polarity, either ADDITIVE or SUBTRACTIVE */
    /* UNKNOWN case (such as for palette colors) has been handled above */
    iss.ColorPolarityAdditive = penum->dev->color_info.polarity == GX_CINFO_POLARITY_ADDITIVE;
    /* Allocate a buffer for one source/destination line. */
    {
	uint out_size =
	    iss.WidthOut * max(iss.Colors * (iss.BitsPerComponentOut / 8),
			       arch_sizeof_color_index);  
        /* Allocate based upon frac size (as BitsPerComponentOut=16) */

        /* output scan line input plus output */
        /* The outsize may have an adjustment for word boundary on it.
           Need to account for that now */

 	out_size += align_bitmap_mod;  

	line = gs_alloc_bytes(mem, in_size + out_size,       
			      "image scale src+dst line");
    }
    pss = (stream_image_scale_state *)
	s_alloc_state(mem, template->stype, "image scale state");
    if (line == 0 || pss == 0 ||
	(pss->params = iss, pss->template = template,
	 (*pss->template->init) ((stream_state *) pss) < 0)
	) {
	gs_free_object(mem, pss, "image scale state");
	gs_free_object(mem, line, "image scale src+dst line");
	/* Try again without interpolation. */
	penum->interpolate = false;
	return 0;
    }
    penum->line = line;  /* Set to the input and output buffer */
    penum->scaler = pss;
    penum->line_xy = 0;
    {
	gx_dda_fixed x0;

	x0 = penum->dda.pixel0.x;
	if (penum->matrix.xx < 0)
	    dda_advance(x0, penum->rect.w);
	penum->xyi.x = fixed2int_pixround(dda_current(x0));
    }
    penum->xyi.y = penum->yi0 + fixed2int_pixround_perfect((fixed)((int64_t)penum->rect.y 
				    * penum->dst_height / penum->Height))
		* (penum->matrix.yy > 0 ? 1 : -1);
    if_debug0('b', "[b]render=interpolate\n");

    return &image_render_interpolate;
}

/* ------ Rendering for interpolated images ------ */

static int
image_render_interpolate(gx_image_enum * penum, const byte * buffer,
			 int data_x, uint iw, int h, gx_device * dev)
{
    stream_image_scale_state *pss = penum->scaler;
    const gs_imager_state *pis = penum->pis;
    const gs_color_space *pcs = penum->pcs;
    gs_logical_operation_t lop = penum->log_op;
    int c = pss->params.Colors;
    stream_cursor_read r;
    stream_cursor_write w;
    unsigned char index_space;
    byte *out = penum->line;   /* buffer for output scan line.  It may be large
                               enough to hold a temporary converted input scan
                               line also depending upon what occured in
                               gs_image_class_0_interpolate */

    if (h != 0) {

	/* Convert the unpacked data to concrete values in */
	/* the source buffer. */
	int sizeofPixelIn = pss->params.BitsPerComponentIn / 8;
	uint row_size = pss->params.WidthIn * c * sizeofPixelIn;
	const unsigned char *bdata = buffer + data_x * c * sizeofPixelIn;    /* raw input data */       
        index_space = 0;

        /* We have the following cases to worry about 

          1) Device 8 bit color or nondevice but not indexed (e.g. ICC).  Use as is directly. Remap after interpolation.
          2) Indexed 8 bit color.  Get to the base space. We will then be in the same state as 1.  
          3) 16 bit not indexed.  Remap after interpolation.  
          4) Indexed 16bit color.   Get to base space in 16bit frac form. We will then be in same state as 3.  

       */

	if (sizeofPixelIn == 1) {

            if (pcs->type->index != gs_color_space_index_Indexed) {


                /* An issue here is that we may not be "device color" due to 
                   how the data is encoded.  Need to check for that case here */

                if (penum->device_color || gs_color_space_is_CIE(pcs)){

                    /* 8-bit color values, possibly device 
                    indep. or device depend., not indexed. 
                    Decode range was [0 1] */
                    if (penum->matrix.xx >= 0) {
	                /* Use the input data directly. */
	                r.ptr = bdata - 1;   /* sets up data in the stream buffere structure */
                    } else {
	                /* Mirror the data in X. */
	                const byte *p = bdata + row_size - c;
	                byte *q = out;
	                int i;

	                for (i = 0; i < pss->params.WidthIn; p -= c, q += c, ++i)
	                    memcpy(q, p, c);
	                r.ptr = out - 1;
	                out = q;
                    }

                } else {

                    /* We need to do some decoding.
                       Data will remain in 8 bits 
                       This does not occur if color
                       space was CIE encoded.  Then we
                       do the decode during concretization 
                       which occurs after interpolation */

	            int bps = 1;
	            int dc = penum->spp;
	            const byte *pdata = bdata;
	            byte *psrc = (byte *) penum->line;
	            int i, j;
                    int dpd = dc;
	            gs_client_color cc;

                    /* Go backwards through the data */
                    if (penum->matrix.xx < 0) {
                      pdata += (pss->params.WidthIn - 1) * dpd;
                      dpd = - dpd;
                    }

	            r.ptr = (byte *) psrc - 1;
	            for (i = 0; i < pss->params.WidthIn; i++, psrc += c) {

                        /* Do the decode but remain in 8 bits */

                        for (j = 0; j < dc;  ++j) {
                            decode_sample(pdata[j], cc, j);
                            psrc[j] = float_color_to_byte_color(cc.paint.values[j]);
                         }

	                pdata += dpd;
                    }

	            out += round_up(pss->params.WidthIn * c,align_bitmap_mod);
                }


            } else {

                /* indexed 8 bit color values, possibly a device indep. or device depend. base space 
                   We need to get out of the indexed space and into the base color space.
                   Note that we need to worry about the decode function for the index values. */

      	        int bps = penum->bps;
	        int dc = penum->spp;
	        const byte *pdata = bdata; /* Input buffer */
	        unsigned char *psrc = (unsigned char *) penum->line;  /* Output buffer */
	        int i;
                int dpd = dc * (bps <= 8 ? 1 : sizeof(frac));
                float max_range;

                /* Get max of decode range */

                max_range = (penum->map[0].decode_factor < 0 ? 
                    penum->map[0].decode_base : 
                penum->map[0].decode_base + 255.0 * penum->map[0].decode_factor);

                index_space = 1;

                /* flip the horizontal direction if indicated by the matrix value */
                if (penum->matrix.xx < 0) {
                  pdata += (pss->params.WidthIn - 1) * dpd;
                  dpd = - dpd;
                }
	        r.ptr = (byte *) psrc - 1;

	        for (i = 0; i < pss->params.WidthIn; i++, psrc += c) {

                    /* Let's get directly to a decoded byte type loaded into psrc, 
                        and do the interpolation in the source space 
                        Then we will do the appropriate remap function after interpolation. */

                    /* First we need to get the properly decoded value. */

                    float decode_value;

                    switch ( penum->map[0].decoding )
                    {
                        case sd_none:

                         /* while our indexin is going to be 0 to 255.0 due to what is getting handed to us,
                            the range of our original data may not have been as such and we may need to rescale, 
                            to properly lookup at the correct location (or do the proc correctly) during the 
                            index look-up.  This occurs even if decoding was set to sd_none.  */

                            decode_value = (float) pdata[0] * (float)max_range / 255.0; 

                        break;

                        case sd_lookup:	
                            decode_value = 
                              (float) penum->map[0].decode_lookup[pdata[0] >> 4];
                        break;

                        case sd_compute:
                            decode_value =   
                              penum->map[0].decode_base + 
                              ((float) pdata[0]) * penum->map[0].decode_factor;
			    break;

		        default:
			    decode_value = 0; /* Quiet gcc warning. */
                  }

                  gs_cspace_indexed_lookup_bytes(pcs, decode_value,psrc);	
	          pdata += dpd;    /* Can't have just ++ 
                                   since we could be going backwards */
                }

                 /* We need to set the output to the end of the input buffer 
                    moving it to the next desired word boundary.  This must
                    be accounted for in the memory allocation of gs_image_class_0_interpolate */

 	         out += round_up(pss->params.WidthIn*c, align_bitmap_mod);  
              }

	} else {

	    /* More than 8-bits/color values */
            /* Even in this case we need to worry about an indexed color space. 
               We need to get to the base color space for the interpolation and
               then if necessary do the remap to the device space */

            if (pcs->type->index != gs_color_space_index_Indexed) {

	        int bps = penum->bps;
	        int dc = penum->spp;
	        const byte *pdata = bdata;
	        frac *psrc = (frac *) penum->line;
	        int i, j;
                int dpd = dc * (bps <= 8 ? 1 : sizeof(frac));

                if (penum->matrix.xx < 0) {
                  pdata += (pss->params.WidthIn - 1) * dpd;
                  dpd = - dpd;
                }
	        r.ptr = (byte *) psrc - 1;
	        if_debug0('B', "[B]Remap row:\n[B]");
	        for (i = 0; i < pss->params.WidthIn; i++, psrc += c) {

                    /* Lets get directly to a frac type loaded into psrc, and do 
                        the interpolation in the source space. 
                           Then we will do the appropriate remap 
                           function after interpolation. */

                    for (j = 0; j < dc;  ++j) {
	                DECODE_FRAC_FRAC(((const frac *)pdata)[j], psrc[j], j);
                    }

	            pdata += dpd;
        #ifdef DEBUG
	            if (gs_debug_c('B')) {
	                int ci;

	                for (ci = 0; ci < c; ++ci)
		            dprintf2("%c%04x", (ci == 0 ? ' ' : ','), psrc[ci]);
	            }
        #endif
                }

	        out += round_up(pss->params.WidthIn * c * sizeof(frac),
			        align_bitmap_mod);
	        if_debug0('B', "\n");


            } else {
                
                /* indexed and more than 8bps.  Need to get to the base space */

      	        int bps = penum->bps;
	        int dc = penum->spp;
	        const byte *pdata = bdata; /* Input buffer */
	        frac *psrc = (frac *) penum->line;    /* Output buffer */
	        int i;
                int dpd = dc * (bps <= 8 ? 1 : sizeof(frac));
                float decode_value;

                index_space = 1;

                /* flip the horizontal direction if indicated by the matrix value */
                if (penum->matrix.xx < 0) {
                  pdata += (pss->params.WidthIn - 1) * dpd;
                  dpd = - dpd;
                }
	        r.ptr = (byte *) psrc - 1;

	        for (i = 0; i < pss->params.WidthIn; i++, psrc += c) {

                    /* Lets get the decoded value. Then we need to do the lookup of this */

                    decode_value = penum->map[i].decode_base + (((const frac *)pdata)[0]) * penum->map[i].decode_factor;

                     /* Now we need to do the lookup of this value, and stick it in psrc as a frac, which is what
                        the interpolator is expecting, since we had more than 8 bits of original image data */

                      gs_cspace_indexed_lookup_frac(pcs, decode_value,psrc);	
	              pdata += dpd;  

                }

                 /* We need to set the output to the end of the input buffer 
                    moving it to the next desired word boundary.  This must
                    be accounted for in the memory allocation of gs_image_class_0_interpolate */
 	         out += round_up(pss->params.WidthIn*c, align_bitmap_mod);                 

            } /* end of else on indexed */
	}  /* end of else on more than 8 bps */

	r.limit = r.ptr + row_size;
    } else {			/* h == 0 */
	r.ptr = 0, r.limit = 0;
	index_space = 0; /* Quiet gcc warning. We didn't figure out whether it was a real bug. */
    }

    /*
     * Process input and/or collect output.  By construction, the pixels are
     * 1-for-1 with the device, but the Y coordinate might be inverted.
     */

    {
	int xo = penum->xyi.x;
	int yo = penum->xyi.y;
	int width = pss->params.WidthOut;
	int sizeofPixelOut = pss->params.BitsPerComponentOut / 8;
        int dy;
	const gs_color_space *pconcs;
        const gs_color_space *pactual_cs;
	int bpp = dev->color_info.depth;
	uint raster = bitmap_raster(width * bpp);
        bool device_color;

	if (penum->matrix.yy > 0)
	    dy = 1;
	else
	    dy = -1, yo--;
	for (;;) {
	    int ry = yo + penum->line_xy * dy;
	    int x;
	    const frac *psrc;
	    gx_device_color devc;
	    int status, code;

	    DECLARE_LINE_ACCUM_COPY(out, bpp, xo);

	    w.limit = out + width *
		max(c * sizeofPixelOut, arch_sizeof_color_index) - 1;
	    w.ptr = w.limit - width * c * sizeofPixelOut;
	    psrc = (const frac *)(w.ptr + 1);

            /* This is where the rescale takes place */
 /*    z=_CrtCheckMemory();
    if (z != 1)
        z = 0; */
            
            status = (*pss->template->process)
		((stream_state *) pss, &r, &w, h == 0);

  /*  z=_CrtCheckMemory();
    if (z != 1)
        z = 0;  */

	    if (status < 0 && status != EOFC)
		return_error(gs_error_ioerror);
	    if (w.ptr == w.limit) {
		int xe = xo + width;

		if_debug1('B', "[B]Interpolated row %d:\n[B]",
			  penum->line_xy);
		for (x = xo; x < xe;) {

#ifdef DEBUG
		    if (gs_debug_c('B')) {
			int ci;

			for (ci = 0; ci < c; ++ci)
			    dprintf2("%c%04x", (ci == 0 ? ' ' : ','),
				     psrc[ci]);
		    }
#endif                   

                    /* if we are in a non device space then work 
                       from the pcs not from the concrete space 
                       also handle index case, where base case was device type */
            
                    if (pcs->type->index == gs_color_space_index_Indexed) {

                        pactual_cs = pcs->base_space;

                    } else {

                        pactual_cs = pcs;
                    }

                    pconcs = cs_concrete_space(pactual_cs, pis);
	            device_color = (pactual_cs->type->concrete_space) (pactual_cs, pis) == pactual_cs;

                    if (device_color) {

                        /* Use the underlying concrete space remap */
                       	
                        code = (*pconcs->type->remap_concrete_color)
		        (psrc, pactual_cs, &devc, pis, dev, gs_color_select_source);

                    } else {

                          /* if we are device dependent we need to get back to float 
                            prior to remap.  This stuff needs to be reworked  as 
                            part of the ICC flow update.  In such a flow, we will want
                            the interpolation algorithm output likely to be 8 bit (if the
                            input were 8 bit) and hit that buffer of values directly 
                            with the linked transform */

	                  gs_client_color cc;
                          int j;
                          int num_components = gs_color_space_num_components(pactual_cs);

                          for (j = 0; j < num_components;  ++j) {

                            /* If we were indexed, dont use the decode procedure for the index values just get to float directly */
                            if (index_space)
                            {
		            
                                cc.paint.values[j] = frac2float(psrc[j]); 

                            } else {

                                decode_sample_frac_to_float(penum, psrc[j], &cc, j);
                            }

                          }

                            code = (pactual_cs->type->remap_color)
                                (&cc, pactual_cs, &devc, pis, dev, gs_color_select_source);

                    }

		    if (code < 0)
			return code;
		    if (color_is_pure(&devc)) {
			/* Just pack colors into a scan line. */
			gx_color_index color = devc.colors.pure;

			/* Skip runs quickly for the common cases. */
			switch (c) {
			    case 1:
				do {
				    LINE_ACCUM(color, bpp);
				    vd_pixel(int2fixed(x), int2fixed(ry), color);
				    x++, psrc += 1;
				} while (x < xe && psrc[-1] == psrc[0]);
				break;
			    case 3:
				do {
				    LINE_ACCUM(color, bpp);
				    vd_pixel(int2fixed(x), int2fixed(ry), color);
				    x++, psrc += 3;
				} while (x < xe && psrc[-4] == psrc[0] &&
				     psrc[-3] == psrc[1] && psrc[-2] == psrc[2] &&
				     psrc[-1] == psrc[3]);
				break;
			    case 4:
				do {
				    LINE_ACCUM(color, bpp);
				    x++, psrc += 4;
				} while (x < xe && psrc[-3] == psrc[0] &&
				     psrc[-2] == psrc[1] &&
				     psrc[-1] == psrc[2]);
				break;
			    default:
				LINE_ACCUM(color, bpp);
				x++, psrc += c;
			}
		    } else {
			int rcode;

			LINE_ACCUM_COPY(dev, out, bpp, xo, x, raster, ry);
			rcode = gx_fill_rectangle_device_rop(x, ry,
						     1, 1, &devc, dev, lop);
			if (rcode < 0)
			    return rcode;
			LINE_ACCUM_SKIP(bpp);
			l_xprev = x + 1;
			x++, psrc += c;
		    }
		}
		LINE_ACCUM_COPY(dev, out, bpp, xo, x, raster, ry);
		/*if_debug1('w', "[w]Y=%d:\n", ry);*/ /* See siscale.c about 'w'. */
		penum->line_xy++;
		if_debug0('B', "\n");
	    }
	    if ((status == 0 && r.ptr == r.limit) || status == EOFC)
		break;
	}
    }
    
    return (h == 0 ? 0 : 1);
}


/* Decode a 16-bit sample into a floating point color component. 
   This is used for cases where the spatial interpolation function output is 16 bit.
   It is only used here, hence the static declaration for now. */

static void 
decode_sample_frac_to_float(gx_image_enum *penum, frac sample_value, gs_client_color *cc, int i)
{
  switch ( penum->map[i].decoding )
  {
  case sd_none:
    cc->paint.values[i] = frac2float(sample_value);  
    break;
  case sd_lookup:	
    cc->paint.values[i] =
      penum->map[i].decode_lookup[(frac2byte(sample_value)) >> 4];
    break;
  case sd_compute:
    cc->paint.values[i] =
      penum->map[i].decode_base + frac2float(sample_value)*255.0 * penum->map[i].decode_factor;
  }
}
