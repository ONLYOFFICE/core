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
/* $Id: gxblend.h 9383 2009-01-22 01:48:31Z ray $ */
/* PDF 1.4 blending functions */

#ifndef gxblend_INCLUDED
#  define gxblend_INCLUDED

#include "gxcindex.h"
#include "gxcvalue.h"
#include "gxfrac.h"

#define RAW_DUMP 0

/* #define DUMP_TO_PNG */

#define	PDF14_MAX_PLANES (GX_DEVICE_COLOR_MAX_COMPONENTS + 1)	/* one extra for alpha channel */

typedef bits16 ArtPixMaxDepth;

#define ART_MAX_CHAN GX_DEVICE_COLOR_MAX_COMPONENTS

#ifndef pdf14_device_DEFINED
#  define pdf14_device_DEFINED
typedef struct pdf14_device_s pdf14_device;
#endif

#ifndef pdf14_buf_DEFINED
#  define pdf14_buf_DEFINED
typedef struct pdf14_buf_s pdf14_buf;
#endif

#ifndef gx_device_DEFINED
#  define gx_device_DEFINED
typedef struct gx_device_s gx_device;
#endif

#ifndef gs_separations_DEFINED
#   define gs_separations_DEFINED
    typedef struct gs_separations_s gs_separations;
#endif

/*
 * This structure contains procedures for processing which differ
 * between the different blending color spaces.
 *
 * The Luminosity, Color, Saturation, and Hue blend modes depend
 * upon the blending color space.  Currently the blending color space
 * matches the process color model of the compositing device.  We need
 * two routines to implement the four 'non separable' blend modes.
 */
typedef struct {
    /*
     * Perform luminosity and color blending.  (Also used for hue blending.)
     */
    void (* blend_luminosity)(int n_chan, byte *dst,
		    const byte *backdrop, const byte *src);
    /*
     * Perform saturation blending.  (Also used for hue blending.)
     */
    void (* blend_saturation)(int n_chan, byte *dst,
		    const byte *backdrop, const byte *src);
} pdf14_nonseparable_blending_procs_s;

typedef pdf14_nonseparable_blending_procs_s
		pdf14_nonseparable_blending_procs_t;

/**
 * art_blend_pixel: Compute PDF 1.4 blending function.
 * @dst: Where to store resulting pixel.
 * @backdrop: Backdrop pixel color.
 * @src: Source pixel color.
 * @n_chan: Number of channels.
 * @blend_mode: Blend mode.
 *
 * Computes the blend of two pixels according the PDF 1.4 transparency
 * spec (section 3.2, Blend Mode). A few things to keep in mind about
 * this implementation:
 *
 * 1. This is a reference implementation, not a high-performance one.
 * Blending using this function will incur a function call and switch
 * statement per pixel, and will also incur the extra cost of 16 bit
 * math.
 *
 * 2. Zero is black, one is white. In a subtractive color space such
 * as CMYK, all pixels should be represented as "complemented", as
 * described in section 3.1 (Blending Color Space) of the PDF 1.4
 * transparency spec.
 *
 * 3. I haven't really figured out how to handle the Compatible blend
 * mode. I wouldn't be surprised if it required an API change.
 **/
void
art_blend_pixel(ArtPixMaxDepth * dst, const ArtPixMaxDepth * backdrop,
		const ArtPixMaxDepth * src, int n_chan,
		gs_blend_mode_t blend_mode);

/**
 * art_blend_pixel_8: Compute PDF 1.4 blending function on 8-bit pixels.
 * @dst: Where to store resulting pixel.
 * @backdrop: Backdrop pixel color.
 * @src: Source pixel color.
 * @n_chan: Number of channels.
 * @blend_mode: Blend mode.
 * @pblend_procs: Procs for handling non separable blending modes.
 *
 * Computes the blend of two pixels according the PDF 1.4 transparency
 * spec (section 3.2, Blend Mode). A few things to keep in mind about
 * this implementation:
 *
 * 1. This is a reference implementation, not a high-performance one.
 * Blending using this function will incur a function call and switch
 * statement per pixel, and will also incur the extra cost of 16 bit
 * math.
 *
 * 2. Zero is black, one is white. In a subtractive color space such
 * as CMYK, all pixels should be represented as "complemented", as
 * described in section 3.1 (Blending Color Space) of the PDF 1.4
 * transparency spec.
 *
 * 3. I haven't really figured out how to handle the Compatible blend
 * mode. I wouldn't be surprised if it required an API change.
 **/
void
art_blend_pixel_8(byte *dst, const byte *backdrop,
		const byte *src, int n_chan, gs_blend_mode_t blend_mode,
       		const pdf14_nonseparable_blending_procs_t * pblend_procs);

/**
 * art_pdf_union_8: Union together two alpha values.
 * @alpha1: One alpha value.
 * @alpha2: Another alpha value.
 *
 * Return value: Union (@alpha1, @alpha2).
 **/
byte art_pdf_union_8(byte alpha1, byte alpha2);

/**
 * art_pdf_union_mul_8: Union together two alpha values, with mask.
 * @alpha1: One alpha value.
 * @alpha2: Another alpha value.
 * @alpha_mask: A mask alpha value;
 *
 * Return value: Union (@alpha1, @alpha2 * @alpha_mask).
 **/
byte art_pdf_union_mul_8(byte alpha1, byte alpha2, byte alpha_mask);

/**
 * art_pdf_composite_pixel_alpha_8: Composite two alpha pixels.
 * @dst: Where to store resulting pixel, also initially backdrop color.
 * @src: Source pixel color.
 * @n_chan: Number of channels.
 * @blend_mode: Blend mode.
 * @pblend_procs: Procs for handling non separable blending modes.
 *
 * Composites two pixels using the basic compositing operation. A few
 * things to keep in mind:
 *
 * 1. This is a reference implementation, not a high-performance one.
 *
 * 2. All pixels are assumed to have a single alpha channel.
 *
 * 3. Zero is black, one is white.
 *
 * Also note that src and dst are expected to be allocated aligned to
 * 32 bit boundaries, ie bytes from [0] to [(n_chan + 3) & -4] may
 * be accessed.
 **/
void
art_pdf_composite_pixel_alpha_8(byte *dst, const byte *src, int n_chan,
	gs_blend_mode_t blend_mode,
       	const pdf14_nonseparable_blending_procs_t * pblend_procs);

/**
 * art_pdf_uncomposite_group_8: Uncomposite group pixel.
 * @dst: Where to store uncomposited pixel.
 * @backdrop: Backdrop.
 * @src: Composited source pixel.
 * @src_alpha_g: alpha_g value associated with @src.
 * @n_chan: Number of channels.
 *
 * Performs uncompositing operation as described in 5.3 of the Adobe spec.
 **/
void
art_pdf_uncomposite_group_8(byte *dst,
			    const byte *backdrop,

			    const byte *src, byte src_alpha_g, int n_chan);

/**
 * art_pdf_recomposite_group_8: Recomposite group pixel.
 * @dst: Where to store pixel, also initial backdrop of group.
 * @dst_alpha_g: Optional pointer to alpha g value associated with @dst.
 * @alpha: Alpha mask value.
 * @src_alpha_g: alpha_g value associated with @src.
 * @blend_mode: Blend mode for compositing.
 * @pblend_procs: Procs for handling non separable blending modes.
 *
 * Note: this is only for non-isolated groups. This covers only the
 * single-alpha case. A separate function is needed for dual-alpha,
 * and that probably needs to treat knockout separately.
 *
 * @src_alpha_g corresponds to $\alpha g_n$ in the Adobe notation.
 *
 * @alpha corresponds to $fk_i \cdot fm_i \cdot qk_i \cdot qm_i$.
 **/
void
art_pdf_recomposite_group_8(byte *dst, byte *dst_alpha_g,
	const byte *src, byte src_alpha_g, int n_chan,
	byte alpha, gs_blend_mode_t blend_mode,
       	const pdf14_nonseparable_blending_procs_t * pblend_procs);

/**
 * art_pdf_composite_group_8: Composite group pixel.
 * @dst: Where to store pixel, also initial backdrop of group.
 * @dst_alpha_g: Optional pointer to alpha g value.
 * @alpha: Alpha mask value.
 * @blend_mode: Blend mode for compositing.
 * @pblend_procs: Procs for handling non separable blending modes.
 *
 * Note: this is only for isolated groups. This covers only the
 * single-alpha case. A separate function is needed for dual-alpha,
 * and that probably needs to treat knockout separately.
 *
 * @alpha corresponds to $fk_i \cdot fm_i \cdot qk_i \cdot qm_i$.
 **/
void
art_pdf_composite_group_8(byte *dst, byte *dst_alpha_g,
	const byte *src, int n_chan, byte alpha, gs_blend_mode_t blend_mode,
       	const pdf14_nonseparable_blending_procs_t * pblend_procs);

/**
 * art_pdf_composite_knockout_simple_8: Simple knockout compositing.
 * @dst: Destination pixel. 
 * @dst_shape: Shape associated with @dst.
 * @src: Source pixel.
 * @n_chan: Number of channels.
 * @opacity: Opacity.
 *
 * This function handles the simplest knockout case: an isolated
 * knockout group, and an elementary shape. The alpha channel of @src
 * is interpreted as shape.
 **/
void
art_pdf_composite_knockout_simple_8(byte *dst,
				    byte *dst_shape,

				    const byte *src,
				    int n_chan, byte opacity);

/**
 * art_pdf_composite_knockout_isolated_8: Simple knockout compositing.
 * @dst: Destination pixel. 
 * @dst_shape: Shape associated with @dst.
 * @src: Source pixel.
 * @n_chan: Number of channels.
 * @shape: Shape.
 * @alpha_mask: Alpha mask.
 * @shape_mask: Shape mask.
 *
 * This function handles compositin in an isolated knockout case. The
 * alpha channel of @src is interpreted as alpha.
 **/
void
art_pdf_composite_knockout_isolated_8(byte *dst,
				      byte *dst_shape,
				      const byte *src,
				      int n_chan,
				      byte shape,
				      byte alpha_mask, byte shape_mask);

/**
 * art_pdf_composite_knockout_8: General knockout compositing.
 * @dst: Destination pixel. 
 * @dst_alpha_g: Pointer to alpha g value associated with @dst.
 * @backdrop: Backdrop pixel (initial backdrop of knockout group).
 * @src: Source pixel.
 * @n_chan: Number of channels.
 * @shape: Shape.
 * @alpha_mask: Alpha mask.
 * @shape_mask: Shape mask.
 * @blend_mode: Blend mode for compositing.
 * @pblend_procs: Procs for handling non separable blending modes.
 *
 * This function handles compositing in the case where the knockout
 * group is non-isolated. If the @src pixels themselves come from a
 * non-isolated group, they should be uncomposited before calling this
 * routine.
 **/
void
art_pdf_composite_knockout_8(byte *dst,
		byte *dst_alpha_g, const byte *backdrop, const byte *src,
		int n_chan, byte shape, byte alpha_mask,
		byte shape_mask, gs_blend_mode_t blend_mode,
       		const pdf14_nonseparable_blending_procs_t * pblend_procs);

/*
 * Routines for handling the non separable blending modes.
 */
/* RGB blending color space */
void art_blend_luminosity_rgb_8(int n_chan, byte *dst, const byte *backdrop,
			   const byte *src);
void art_blend_saturation_rgb_8(int n_chan, byte *dst, const byte *backdrop,
			   const byte *src);
/* CMYK and CMYK + spot blending color space */
void art_blend_saturation_cmyk_8(int n_chan, byte *dst, const byte *backdrop,
			   const byte *src);
void art_blend_luminosity_cmyk_8(int n_chan, byte *dst, const byte *backdrop,
			   const byte *src);
/* 'Custom' i.e. unknown blending color space. */
void art_blend_luminosity_custom_8(int n_chan, byte *dst, const byte *backdrop,
			   const byte *src);
void art_blend_saturation_custom_8(int n_chan, byte *dst, const byte *backdrop,
			   const byte *src);

void pdf14_unpack_additive(int num_comp, gx_color_index color,
			       	pdf14_device * p14dev, byte * out);
void pdf14_unpack_subtractive(int num_comp, gx_color_index color,
			       	pdf14_device * p14dev, byte * out);

void pdf14_unpack_compressed(int num_comp, gx_color_index color,
			       	pdf14_device * p14dev, byte * out);

void pdf14_unpack_custom(int num_comp, gx_color_index color,
			       	pdf14_device * p14dev, byte * out);

void pdf14_preserve_backdrop(pdf14_buf *buf, pdf14_buf *tos, bool has_shape);

void pdf14_compose_group(pdf14_buf *tos, pdf14_buf *nos, pdf14_buf *maskbuf, 
	      int x0, int x1, int y0, int y1, int n_chan, bool additive,
	      const pdf14_nonseparable_blending_procs_t * pblend_procs);


gx_color_index pdf14_encode_color(gx_device *dev, const gx_color_value colors[]);

int pdf14_decode_color(gx_device * dev, gx_color_index color, gx_color_value * out);
gx_color_index pdf14_compressed_encode_color(gx_device *dev, const gx_color_value colors[]);
int pdf14_compressed_decode_color(gx_device * dev, gx_color_index color,
	       						gx_color_value * out);
void pdf14_gray_cs_to_cmyk_cm(gx_device * dev, frac gray, frac out[]);
void pdf14_rgb_cs_to_cmyk_cm(gx_device * dev, const gs_imager_state *pis,
  			   frac r, frac g, frac b, frac out[]);
void pdf14_cmyk_cs_to_cmyk_cm(gx_device * dev, frac c, frac m, frac y, frac k, frac out[]);

void gx_build_blended_image_row(byte *buf_ptr, int y, int planestride, 
			   int width, int num_comp, byte bg, byte *linebuf);
int gx_put_blended_image_cmykspot(gx_device *target, byte *buf_ptr, 
		      int planestride, int rowstride,
		      int x0, int y0, int width, int height, int num_comp, byte bg,
		      gs_separations *pseparations);
int gx_put_blended_image_custom(gx_device *target, byte *buf_ptr, 
		      int planestride, int rowstride,
		      int x0, int y0, int width, int height, int num_comp, byte bg);

#if RAW_DUMP

void dump_raw_buffer(int num_rows, int width, int n_chan,
                    int plane_stride, int rowstride, 
                    char filename[],byte *Buffer);
#endif

#endif /* gxblend_INCLUDED */
