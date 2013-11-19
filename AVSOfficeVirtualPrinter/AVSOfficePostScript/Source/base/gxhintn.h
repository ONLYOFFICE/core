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

/* $Id: gxhintn.h 8022 2007-06-05 22:23:38Z giles $ */
/* Type 1 hinter, prototypes */

#ifndef gxhintn_INCLUDED
#  define gxhintn_INCLUDED

#include "stdint_.h"

#ifndef gs_type1_data_DEFINED
#define gs_type1_data_DEFINED
typedef struct gs_type1_data_s gs_type1_data;
#endif

#ifndef gs_type42_data_DEFINED
#define gs_type42_data_DEFINED
typedef struct gs_type42_data_s gs_type42_data;
#endif

#ifndef gx_path_DEFINED
#  define gx_path_DEFINED
typedef struct gx_path_s gx_path;
#endif

#define T1_MAX_STEM_SNAPS 12
#define T1_MAX_ALIGNMENT_ZONES 6
#define T1_MAX_CONTOURS 10
#define T1_MAX_SUBGLYPHS 3
#define T1_MAX_POLES (100 + T1_MAX_CONTOURS) /* Must be grater than 8 for 'flex'. */
#define T1_MAX_HINTS 30

typedef fixed t1_glyph_space_coord; /* measured in original glyph space units */
typedef int32_t t1_hinter_space_coord; /* measured in internal outliner's space units */
typedef int32_t int19;

enum t1_hint_type
{   hstem, vstem, dot
};

enum t1_pole_type
{   offcurve, oncurve, closepath, moveto
};

enum t1_zone_type
{   topzone, botzone
};

enum t1_align_type
{   unaligned, weak, aligned, topzn, botzn
};

typedef struct {
    double xx, xy, yx, yy;
} double_matrix;

typedef struct {
    int19 xx, xy, yx, yy;
    int32_t denominator;
    unsigned int bitshift;
} fraction_matrix;

typedef struct t1_pole_s /* a pole of outline */
{   t1_glyph_space_coord gx,gy; /* source unaligned coords */
    t1_glyph_space_coord ax,ay; /* aligned coords */
    t1_hinter_space_coord ox,oy;
    enum t1_pole_type type;
    int contour_index;
    enum t1_align_type aligned_x, aligned_y;
    t1_glyph_space_coord boundary_length_x, boundary_length_y;
} t1_pole;

typedef struct t1_hint_s
{   enum t1_hint_type type;
    t1_glyph_space_coord g0, g1; /* starting and ending transversal coord of the stem */
    t1_glyph_space_coord ag0, ag1; /* starting and ending transversal aligned coord of the stem */
    bool b0, b1;  /* g0, g1 correspond to a real stem. */
    enum t1_align_type aligned0, aligned1; /* ag0, ag1 is aligned */
    int q0, q1; /* Stem quality tangent. */
    unsigned int stem3_index; /* 1,2,3 for stem3 (not used yet), 0 for other types */
    int range_index; /* type 2 only */
    int side_mask;
    short stem_snap_index0, stem_snap_index1; /* Applicable StemSnap* index range. */
    t1_glyph_space_coord boundary_length0, boundary_length1;
} t1_hint;

typedef struct t1_hint_range_s
{   short beg_pole, end_pole;
    int next;
} t1_hint_range;

typedef struct t1_hint_applying_s
{   int pole;
    int opposite;
    int next;
} t1_hint_applying;

typedef struct t1_zone_s /* alignment zone */
{   enum t1_zone_type type;
    t1_glyph_space_coord y, overshoot_y;
    t1_glyph_space_coord y_min, y_max;
} t1_zone;

typedef struct t1_hinter_s
{   fraction_matrix ctmf;
    fraction_matrix ctmi;
    unsigned int g2o_fraction_bits;
    unsigned long max_import_coord;
    int32_t g2o_fraction;
    t1_glyph_space_coord orig_gx, orig_gy; /* glyph origin in glyph space */
    t1_glyph_space_coord subglyph_orig_gx, subglyph_orig_gy; /* glyph origin in glyph space */
    fixed orig_dx, orig_dy; /* glyph origin in device space */
    fixed orig_ox, orig_oy; /* glyph origin in hinter space */
    t1_glyph_space_coord width_gx, width_gy; /* glyph space coords of the glyph origin */
    t1_glyph_space_coord cx, cy; /* current point */
    t1_glyph_space_coord bx, by; /* starting point of a contour */
    uint log2_pixels_x, log2_pixels_y; /* log2 of the number of pixels in unit (by an axis) */
    int log2_subpixels_x, log2_subpixels_y; /* log2 of the number of subpixels in unit (may be negative) */
    bool transposed;
    bool align_to_pixels; /* false == "align to (integral) pixels" */
    bool disable_hinting;
    bool pass_through;
    bool grid_fit_x, grid_fit_y;
    bool charpath_flag;
    bool path_opened;
    bool autohinting;
    bool fix_contour_sign;
    t1_glyph_space_coord blue_shift, blue_fuzz;
    t1_pole pole0[T1_MAX_POLES], *pole;
    t1_hint hint0[T1_MAX_HINTS], *hint;
    t1_zone zone0[T1_MAX_ALIGNMENT_ZONES], *zone;
    int contour0[T1_MAX_CONTOURS], *contour;
    int subglyph0[T1_MAX_CONTOURS], *subglyph;
    t1_glyph_space_coord stem_snap0[2][T1_MAX_STEM_SNAPS + 1]; /* StdWH + StemSnapH, StdWV + StemSnapV */
    t1_glyph_space_coord *stem_snap[2];
    int stem_snap_vote0[T1_MAX_STEM_SNAPS + 1];
    int *stem_snap_vote;
    t1_hint_range hint_range0[T1_MAX_HINTS], *hint_range;
    t1_hint_applying hint_applying0[T1_MAX_HINTS * 4], *hint_applying;
    int stem_snap_count[2], max_stem_snap_count[2]; /* H, V */
    int stem_snap_vote_count, max_stem_snap_vote_count;
    int subglyph_count, max_subglyph_count;
    int contour_count, max_contour_count;
    int zone_count, max_zone_count;
    int pole_count, max_pole_count;
    int hint_count, max_hint_count;
    int hint_range_count, max_hint_range_count;
    int hint_applying_count, max_hint_applying_count;
    int primary_hint_count;
    int flex_count;
    int FontType; /* 1 or 2 or 42 */
    bool have_flex;
    bool ForceBold;
    bool keep_stem_width;
    bool suppress_overshoots;
    double BlueScale;
    double font_size;
    double resolution;
    double heigt_transform_coef;
    double width_transform_coef;
    double base_font_scale;
    int19 width_transform_coef_rat;
    int19 heigt_transform_coef_rat;
    int19 width_transform_coef_inv;
    int19 heigt_transform_coef_inv;
    int32_t pixel_o_x, pixel_o_y; /* pixel size in the outline space. */
    t1_glyph_space_coord pixel_gw, pixel_gh; /* pixel size in the glyph space (maybe transpozed). */
    t1_glyph_space_coord overshoot_threshold;
    t1_glyph_space_coord ymin, ymax, ymid;
    gx_path *output_path;
    gs_memory_t *memory;
} t1_hinter;

void t1_hinter__init(t1_hinter * this, gx_path *output_path);
int  t1_hinter__set_mapping(t1_hinter * this, gs_matrix_fixed * ctm,
			gs_matrix * FontMatrix, gs_matrix * baseFontMatrix,
			int log2_pixels_x, int log2_pixels_y,
			int log2_subpixels_x, int log2_subpixels_y,
			fixed origin_x, fixed origin_y, bool align_to_pixels);
int  t1_hinter__set_font_data(t1_hinter * this, int FontType, gs_type1_data *pdata, 
			bool no_grid_fitting, bool is_resource);
int  t1_hinter__set_font42_data(t1_hinter * this, int FontType, gs_type42_data *pdata, 
			bool no_grid_fitting);

int  t1_hinter__sbw(t1_hinter * this, fixed sbx, fixed sby, fixed wx,  fixed wy);
int  t1_hinter__sbw_seac(t1_hinter * this, fixed sbx, fixed sby);
int  t1_hinter__rmoveto(t1_hinter * this, fixed xx, fixed yy);
int  t1_hinter__rlineto(t1_hinter *, fixed xx, fixed yy);
int  t1_hinter__rcurveto(t1_hinter * this, fixed xx0, fixed yy0, fixed xx1, fixed yy1, fixed xx2, fixed yy2);
void t1_hinter__setcurrentpoint(t1_hinter * this, fixed xx, fixed yy);
int  t1_hinter__closepath(t1_hinter * this);
int  t1_hinter__end_subglyph(t1_hinter * this);

int  t1_hinter__flex_beg(t1_hinter * this);
int  t1_hinter__flex_end(t1_hinter * this, fixed flex_height);
int  t1_hinter__flex_point(t1_hinter * this);

int  t1_hinter__hint_mask(t1_hinter * this, byte *mask);
int  t1_hinter__drop_hints(t1_hinter * this);
int  t1_hinter__dotsection(t1_hinter * this);
int  t1_hinter__hstem(t1_hinter * this, fixed x0, fixed x1);
int  t1_hinter__vstem(t1_hinter * this, fixed y0, fixed y1);
int  t1_hinter__overall_hstem(t1_hinter * this, fixed x0, fixed x1, int side_mask);
int  t1_hinter__hstem3(t1_hinter * this, fixed x0, fixed y1, fixed x2, fixed y3, fixed x4, fixed y5);
int  t1_hinter__vstem3(t1_hinter * this, fixed y0, fixed y1, fixed y2, fixed y3, fixed y4, fixed y5);

int  t1_hinter__endglyph(t1_hinter * this);
int  t1_hinter__is_x_fitting(t1_hinter * this);
void t1_hinter__fix_contour_signs(t1_hinter * this);

#endif /* gxhintn_INCLUDED */
