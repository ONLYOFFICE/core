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

/*$Id: evenbetter-rll.c 8071 2007-06-20 22:46:08Z giles $ */
/* Testbed implementation of Even Better Screening. */

/*
 * Code in this module is covered by US Patents 5,055,942 and
 * 5,917,614, and corresponding international patents.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "evenbetter-rll.h"

/* Set this define if compiling with AltiVec optimizations. */
#define noUSE_AVEC

/* Set this define if compiling with SSE optimizations. */
#define noUSE_SSE2

#define EVENBETTER_VERSION 133

#define EVEN_SHIFT 16
#define IMO_SHIFT 14
#define EVEN_RB_CAP (1 << (EVEN_SHIFT - 2))

#define FANCY_COUPLING

#if defined(USE_AVEC) || defined(USE_SSE2)
#define USE_VECTOR
#endif

#ifdef USE_AVEC
#include "eb_avec.h"

#endif

#ifdef USE_SSE2
typedef struct _eb_ctx_sse2 eb_ctx_sse2;
typedef struct _eb_srcbuf eb_srcbuf;

int eb_test_sse2(void);
int eb_sse2_core(eb_ctx_sse2 *ctx, unsigned char **out, eb_srcbuf *in,
		 int offset);
int eb_sse2_rev_rs(eb_ctx_sse2 *ctx, int offset);
int eb_sse2_set_daz(void);
void eb_sse2_restore_daz(int save_mxcsr);

struct _eb_ctx_sse2 {
  int xs;
  int *iir_line;
  int *r_line;
  int *a_line;
  int *b_line;
  char *skip_line;
  int dummy[2];
  float *luts[4];
  float e[4];
  float e_i_1[4];
  int r[4];
  int a[4];
  int b[4];
  int ones[4];
  int twos[4];
  int aspect2[4];
  float ehi[4];
  float elo[4];
  float ohi[4];
  float r_mul[4];
  float kernel[4];
  unsigned int seed1[4];
  unsigned int seed2[4];
};

struct _eb_srcbuf {
  float im[64];
  float rb[64];
  float rs[64];
  int dummy[3];
};

#endif

typedef struct _EBPlaneCtx EBPlaneCtx;
typedef unsigned int uint32;

struct _EvenBetterCtx {
  int source_width;
  int dest_width;
  int n_planes;
  int levels; /* Number of levels on output, <= 256 */
  EBPlaneCtx **plane_ctx;
  int aspect;
  int *strengths;
  int even_elo;
  int even_ehi;
  int *c_line;

  int even_c1;
  int do_shadows;

  uint32  seed1;
  uint32  seed2;

  FILE *dump_file;
  EbDumpLevel dump_level;

#ifdef USE_SSE2
  eb_ctx_sse2 **sse2_ctx;
  int using_vectors;
#endif
#ifdef USE_AVEC
  eb_ctx_avec **avec_ctx;
  int using_vectors;
#endif
};

struct _EBPlaneCtx {
  int source_width;
  int dest_width;
  int *rb_line;
  int *iir_line;
  int *r_line;
  int *a_line;
  int *b_line;
  int *r_line_sh;
  int *a_line_sh;
  int *b_line_sh;
  int *lut;
  int *rb_lut;
  char *rs_lut;
  int *white_count_line;
};

void *
eb_malloc_aligned(int size, int align)
{
  void *result;
  void *alloced = malloc(size + align);
  int pad;

  if (alloced == 0)
    return 0;
  pad = (((int)alloced + 12) & 15) + 4;
  result = (void *)(pad + (char *)alloced);
  ((int *)result)[-1] = pad;   
  return result;
}

void
eb_free_aligned(void *p)
{
  int pad = ((int *)p)[-1];
  free((char*)p - pad);
}

static double
eb_compute_rbscale(const EvenBetterParams *params)
{
  double rbscale = params->rbscale;

  if (rbscale == 0.0)
    {
      rbscale = params->aspect == 1 ? 0.95 :
	params->aspect == 2 ? 1.8 :
	params->aspect == 4 ? 3.6 : 1;
    }
  return rbscale;
}

static int
eb_compute_randshift(int nl, int rs_base, int do_shadows, int levels)
{
  int rs = rs_base;
  if ((nl > (90 << (EVEN_SHIFT - 10)) &&
       nl < (129 << (EVEN_SHIFT - 10))) ||
      (nl > (162 << (EVEN_SHIFT - 10)) &&
       nl < (180 << (EVEN_SHIFT - 10))))
    rs--;
  else if (nl > (321 << (EVEN_SHIFT - 10)) &&
	   nl < (361 << (EVEN_SHIFT - 10)))
    {
      rs--;
      if (nl > (331 << (EVEN_SHIFT - 10)) &&
	  nl < (351 << (EVEN_SHIFT - 10)))
	rs--;
    }
  else if ((do_shadows ||
	    nl == (levels - 1) << EVEN_SHIFT) &&
	   nl > ((levels - 1) << EVEN_SHIFT) -
	   (1 << (EVEN_SHIFT - 2)))
    {
      /* don't add randomness in extreme shadows */
    }
  else if ((nl > (3 << (EVEN_SHIFT - 2))))
    {
      nl -= (nl + (1 << (EVEN_SHIFT - 2))) & -(1 << (EVEN_SHIFT - 1));
      if (nl < 0) nl = -nl;
      if (nl < (1 << (EVEN_SHIFT - 4))) rs--;
      if (nl < (1 << (EVEN_SHIFT - 5))) rs--;
      if (nl < (1 << (EVEN_SHIFT - 6))) rs--;
    }
  else
    {
      if (nl < (3 << (EVEN_SHIFT - 3))) nl += 1 << (EVEN_SHIFT - 2);
      nl = nl - (1 << (EVEN_SHIFT - 1));
      if (nl < 0) nl = -nl;
      if (nl < (1 << (EVEN_SHIFT - 4))) rs--;
      if (nl < (1 << (EVEN_SHIFT - 5))) rs--;
      if (nl < (1 << (EVEN_SHIFT - 6))) rs--;
    }
  return rs;
}

#ifdef USE_SSE2
static eb_ctx_sse2 *
eb_ctx_sse2_new(const EvenBetterParams *params, int start_plane, int end_plane)
{
  int xs = params->source_width;
  int aspect2 = params->aspect * params->aspect;
  eb_ctx_sse2 *ctx;
  int i;
  double im_scale;
  float r_mul = 1.0 / (params->aspect * (1 << (6 - params->even_c1_scale)));
  double rbscale = eb_compute_rbscale(params);
  int rs_base;

  ctx = (eb_ctx_sse2 *)eb_malloc_aligned(sizeof(eb_ctx_sse2), 16);
  ctx->xs = xs;
  for (i = 0; i < 4; i++)
    {
      ctx->e[i] = 0.0;
      ctx->e_i_1[i] = 0.0;
      ctx->r[i] = 0;
      ctx->a[i] = 1;
      ctx->b[i] = aspect2;
      ctx->ones[i] = 1;
      ctx->twos[i] = 2;
      ctx->aspect2[i] = aspect2;
      ctx->ohi[i] = params->levels - 1;
      ctx->ehi[i] = 1.1;
      ctx->elo[i] = -0.1;
      ctx->r_mul[i] = r_mul;
      ctx->seed1[i] = (i << 8) + 0x7000;
      ctx->seed2[i] = (i << 16) + 0x9000;
    }
  ctx->kernel[0] = 1.0 / 16;
  ctx->kernel[1] = 3.0 / 16;
  ctx->kernel[2] = 5.0 / 16;
  ctx->kernel[3] = 7.0 / 16;

  im_scale = (params->levels - 1) * 1.0 / (1 << 24);
  rs_base = 35 - EVEN_SHIFT - params->rand_scale;

  for (i = start_plane; i < end_plane; i++)
    {
      float *lut = (float *)malloc((ET_SRC_MAX + 1) * sizeof(float) * 3);
      int j;
      ctx->luts[i - start_plane] = lut;

      for (j = 0; j < ET_SRC_MAX + 1; j++)
	{
	  double g = ((1 << 24) - params->luts[i][j]) * im_scale;
	  int nl, rs;

	  lut[j * 3] = g;
	  if (g == 0.0)
	    lut[j * 3 + 1] = 0.5;
	  else
	    lut[j * 3 + 1] = 0.5 - r_mul * rbscale / g;

	  nl = (params->levels - 1 - g) * (1 << EVEN_SHIFT);
	  rs = eb_compute_randshift(nl, rs_base,
				    params->do_shadows, params->levels);

	  lut[j * 3 + 2] = 1.0 / (1 << EVEN_SHIFT) / (1 << rs);
	}
    }
  for (i = i - start_plane; i < 4; i++)
    ctx->luts[i] = NULL;

  ctx->iir_line = (int *)eb_malloc_aligned(16 * (xs + 32), 16);
  ctx->a_line = (int *)eb_malloc_aligned(16 * (xs + 32), 16);
  ctx->b_line = (int *)eb_malloc_aligned(16 * (xs + 32), 16);
  ctx->r_line = (int *)eb_malloc_aligned(16 * (xs + 32), 16);
  for (i = 0; i < (xs + 32) * 4; i++)
    {
      ((float *)ctx->iir_line)[i] = 0;
      ctx->a_line[i] = 1;
      ctx->b_line[i] = aspect2;
      ctx->r_line[i] = 0;
    }

  ctx->skip_line = (char *)malloc((xs + 15) & -16);

  return ctx;
}

static void
eb_ctx_sse2_free(eb_ctx_sse2 *ctx)
{
  int i;

  for (i = 0; i < 4; i++)
    free(ctx->luts[i]);
  eb_free_aligned(ctx->iir_line);
  eb_free_aligned(ctx->a_line);
  eb_free_aligned(ctx->b_line);
  eb_free_aligned(ctx->r_line);
  free(ctx->skip_line);
  eb_free_aligned(ctx);
}
#endif

#ifdef USE_AVEC
static eb_ctx_avec *
eb_ctx_avec_new(const EvenBetterParams *params, int start_plane, int end_plane)
{
  int xs = params->source_width;
  int aspect2 = params->aspect * params->aspect;
  eb_ctx_avec *ctx;
  int i;
  double im_scale;
  double k;
  float imscale1, imscale2, rbmul, rsbase;
  float r_mul = 1.0 / (params->aspect * (1 << (6 - params->even_c1_scale)));
  double rbscale = eb_compute_rbscale(params);
  vector unsigned int zero = vec_splat_u32(0);
  const vector float kernel = { 1.0 / 16, 3.0 / 16, 5.0 / 16, 7.0 / 16 };
  vector float almostone = { 255.0/256, 255.0/256, 255.0/256, 255.0/256 };
  int rs_base;

  ctx = (eb_ctx_avec *)eb_malloc_aligned(sizeof(eb_ctx_avec), 16);
  ctx->xs = xs;

  ctx->e = (vector float) zero;
  ctx->e_i_1 = (vector float) zero;
  ctx->r = zero;
  ctx->a = zero;
  im_scale = (params->levels - 1) * (1.0 / (1 << 24));
  rs_base = 35 - EVEN_SHIFT - params->rand_scale;

  if (params->gamma == 1.0)
    k = 0;
  else if (params->gamma == 1.8)
    k = 0.835;
  else if (params->gamma == 2.0)
    k = 1.0;
  else
    /* this shouldn't happen! */
    k = 0;

  for (;;)
    {
      vector float foff, f0, f1;

      imscale1 = (1 - k) * (params->levels - 1) * (256.0 / 255.0);
      imscale2 = k * (params->levels - 1) * sqrt(256.0 / 255.0);
      for (i = 0; i < 4; i++)
	{
	  ((float *)&ctx->imscale1)[i] = imscale1;
	  ((float *)&ctx->imscale2)[i] = imscale2;
	}
      f0 = vec_rsqrte(almostone);
      f0 = vec_madd(f0, almostone, (vector float)zero);
      f1 = vec_madd(f0, ctx->imscale2, (vector float)zero);
      foff = vec_madd(almostone, ctx->imscale1, f1);
      f1 = vec_nmsub(f0, ctx->imscale2, foff);
      f1 = vec_nmsub(almostone, ctx->imscale1, f1);
      if (vec_all_eq(f1, (vector float)zero))
	{
	  ctx->foff = foff;
	  break;
	}
      k += 1e-5;
    }
  rbmul = -r_mul * rbscale;
  rsbase = 1.0 / (1 << EVEN_SHIFT) / (1 << rs_base);
  for (i = 0; i < 4; i++)
    {
      ((int *)&ctx->b)[i] = aspect2;
      ((int *)&ctx->aspect2)[i] = aspect2;
      ((int *)&ctx->seed1)[i] = (i << 8) + 0x7000;
      ((int *)&ctx->seed2)[i] = (i << 16) + 0x9000;
      ((float *)&ctx->ohi)[i] = params->levels - 1;
      ((float *)&ctx->ehi)[i] = 1.1;
      ((float *)&ctx->elo)[i] = -0.1;
      ((float *)&ctx->r_mul)[i] = r_mul;
      ((float *)&ctx->rsbase)[i] = rsbase;
      ((float *)&ctx->rbmul)[i] = rbmul;
    }
  ctx->kernel = kernel;

  rs_base = 35 - EVEN_SHIFT - params->rand_scale;

  for (i = start_plane; i < end_plane; i++)
    {
      float *lut = (float *)malloc((ET_SRC_MAX + 1) * sizeof(float) * 3);
      int j;
      ctx->luts[i - start_plane] = lut;

      for (j = 0; j < ET_SRC_MAX + 1; j++)
	{
	  double g = ((1 << 24) - params->luts[i][j]) * im_scale;
	  int nl, rs;

	  lut[j * 3] = g;
	  if (g == 0.0)
	    lut[j * 3 + 1] = 0.5;
	  else
	    lut[j * 3 + 1] = 0.5 - r_mul * rbscale / g;
	  nl = (params->levels - 1 - g) * (1 << EVEN_SHIFT);
	  rs = eb_compute_randshift(nl, rs_base,
				    params->do_shadows, params->levels);

	  lut[j * 3 + 2] = 1.0 / (1 << EVEN_SHIFT) / (1 << rs);
	}
    }
  for (i = i - start_plane; i < 4; i++)
    ctx->luts[i] = NULL;

  ctx->iir_line = (vector float *)eb_malloc_aligned(16 * (xs + 32), 16);
  ctx->a_line = (vector unsigned int *)eb_malloc_aligned(16 * (xs + 32), 16);
  ctx->b_line = (vector unsigned int *)eb_malloc_aligned(16 * (xs + 32), 16);
  ctx->r_line = (vector unsigned int *)eb_malloc_aligned(16 * (xs + 32), 16);
  for (i = 0; i < (xs + 32) * 4; i++)
    {
      ((float *)ctx->iir_line)[i] = 0;
      ((int *)ctx->a_line)[i] = 1;
      ((int *)ctx->b_line)[i] = aspect2;
      ((int *)ctx->r_line)[i] = 0;
    }

  ctx->skip_line = (char *)malloc((xs + 15) & -16);

  return ctx;
}

static void
eb_ctx_avec_free(eb_ctx_avec *ctx)
{
  int i;

  for (i = 0; i < 4; i++)
    free(ctx->luts[i]);
  eb_free_aligned(ctx->iir_line);
  eb_free_aligned(ctx->a_line);
  eb_free_aligned(ctx->b_line);
  eb_free_aligned(ctx->r_line);
  free(ctx->skip_line);
  eb_free_aligned(ctx);
}

#endif

#ifdef USE_VECTOR
static int
even_better_line_vector(EvenBetterCtx *ebc, uchar **dest,
		      const ET_Rll *const *src)
{
  int n_planes = ebc->n_planes;
  int xd = ebc->dest_width;
  int strip;
  eb_srcbuf sb_alloc;
  eb_srcbuf *srcbuf;
  uchar dummy_a[32];
  uchar *dummy_dst = (uchar *)(((int)dummy_a + 15) & -16);
#ifdef USE_SSE2
  int save_mxcsr = eb_sse2_set_daz();
#endif

  srcbuf = (eb_srcbuf *)(((int)&sb_alloc + 12) & -16);


  for (strip = 0; strip < n_planes; strip += 4)
    {
#ifdef USE_AVEC
      eb_ctx_avec *ctx = ebc->avec_ctx[strip >> 2];
#endif
#ifdef USE_SSE2
      eb_ctx_sse2 *ctx = ebc->sse2_ctx[strip >> 2];
#endif
      uchar *destbufs[4];
      const ET_Rll *const *sbuf = src + strip;
      int count[4];
      int src_idx[4];
      int plane_idx, last_plane;
      float im[4], rb[4], rs[4];
      int i;

      last_plane = n_planes - strip < 4 ? n_planes - strip : 4;
      for (plane_idx = 0; plane_idx < last_plane; plane_idx++)
	{
	  count[plane_idx] = 0;
	  src_idx[plane_idx] = 0;
	  destbufs[plane_idx] = dest[plane_idx + strip];
	}
      for (; plane_idx < 4; plane_idx++)
	{
	  int j;

	  for (j = 0; j < 16; j++)
	    {
	      ((float *)&srcbuf->im)[j * 4 + plane_idx] = 0.0;
	      ((float *)&srcbuf->rb)[j * 4 + plane_idx] = 0.0;
	      ((float *)&srcbuf->rs)[j * 4 + plane_idx] = 0.0;
	    }
	}
      for (i = 0; i < xd; i += 16)
	{
	  int jmax = (xd - i) > 16 ? 16 : xd - i;
	  int skip = 1;
	  int j;

	  for (plane_idx = 0; plane_idx < last_plane; plane_idx++)
	    {
	      if (count[plane_idx] < 16 || im[plane_idx] != 0.0)
		{
		  skip = 0;
		  break;
		}
	    }
	  ctx->skip_line[i >> 4] = skip;

	  if (skip)
	    {
	      /* all white */

	      for (plane_idx = 0; plane_idx < last_plane; plane_idx++)
		{
		  uchar *dst_ptr = destbufs[plane_idx];
		  if (jmax == 16)
		    {
		      ((uint32 *)dst_ptr)[(i >> 2) + 0] = 0;
		      ((uint32 *)dst_ptr)[(i >> 2) + 1] = 0;
		      ((uint32 *)dst_ptr)[(i >> 2) + 2] = 0;
		      ((uint32 *)dst_ptr)[(i >> 2) + 3] = 0;
		    }
		  else
		    {
		      for (j = 0; j < jmax; j++)
			dst_ptr[i + j] = 0;
		    }
		  count[plane_idx] -= jmax;
		}
	    }
	  else
	    {
	      for (plane_idx = 0; plane_idx < last_plane; plane_idx++)
		{
		  const float *lut = ctx->luts[plane_idx];
		  float imp = im[plane_idx];
		  float rbp = rb[plane_idx];
		  float rsp = rs[plane_idx];
 		  for (j = 0; j < jmax; j++)
		    {
		      if (count[plane_idx] == 0)
			{
			  const ET_Rll *src_p = sbuf[plane_idx] +
			    src_idx[plane_idx]++;
			  ET_SrcPixel src_pixel = src_p->value;
			  count[plane_idx] = src_p->length;
			  imp = lut[src_pixel * 3];
			  rbp = lut[src_pixel * 3 + 1];
			  rsp = lut[src_pixel * 3 + 2];
			}
		      ((float *)&srcbuf->im)[j * 4 + plane_idx] = imp;
		      ((float *)&srcbuf->rb)[j * 4 + plane_idx] = rbp;
		      ((float *)&srcbuf->rs)[j * 4 + plane_idx] = rsp;
		      count[plane_idx]--;
		    }
		  im[plane_idx] = imp;
		  rb[plane_idx] = rbp;
		  rs[plane_idx] = rsp;
		}
	      for (; plane_idx < 4; plane_idx++)
		{
		  destbufs[plane_idx] = dummy_dst - i;
		}
#ifdef USE_AVEC
	      eb_avec_core(ctx, (vector unsigned char **)destbufs, srcbuf, i);
#endif
#ifdef USE_SSE2
	      eb_sse2_core(ctx, destbufs, srcbuf, i);
#endif
	    }
	}

      for (i = xd & -16; i >= 0; i -= 16)
	{
	  if (!ctx->skip_line[i >> 4])
	    {
#ifdef USE_AVEC
	      eb_avec_rev_rs(ctx, i + 15);
#endif
#ifdef USE_SSE2
	      eb_sse2_rev_rs(ctx, i + 15);
#endif
	    }
	}
    }
#ifdef USE_SSE2
  eb_sse2_restore_daz(save_mxcsr);
#endif
  return 0;
}
#endif

#ifdef USE_AVEC
static int
even_better_line_fastprep(EvenBetterCtx *ebc, uchar **dest,
			  const ET_SrcPixel *const *src)
{
  int n_planes = ebc->n_planes;
  int xd = ebc->dest_width;
  int strip;
  eb_srcbuf sb_alloc;
  eb_srcbuf *srcbuf;
  uchar dummy_a[32];
  uchar *dummy_dst = (uchar *)(((int)dummy_a + 15) & -16);

  srcbuf = (eb_srcbuf *)(((int)&sb_alloc + 12) & -16);

  for (strip = 0; strip < n_planes; strip += 4)
    {
#ifdef USE_AVEC
      eb_ctx_avec *ctx = ebc->avec_ctx[strip >> 2];
#endif
#ifdef USE_SSE2
      eb_ctx_sse2 *ctx = ebc->sse2_ctx[strip >> 2];
#endif
      uchar *destbufs[4];
      const ET_SrcPixel *const *sbuf = src + strip;
      int plane_idx, last_plane;
      int i;

      last_plane = n_planes - strip < 4 ? n_planes - strip : 4;
      for (plane_idx = 0; plane_idx < last_plane; plane_idx++)
	{
	  destbufs[plane_idx] = dest[plane_idx + strip];
	}
      for (i = 0; i < xd; i += 16)
	{
	  int noskip;
	  noskip = eb_avec_prep_srcbuf(ctx, last_plane, srcbuf, sbuf, i);
	  ctx->skip_line[i >> 4] = noskip;
	  if (noskip)
	    {
	      for (plane_idx = last_plane; plane_idx < 4; plane_idx++)
		destbufs[plane_idx] = dummy_dst - i;
	      eb_avec_core(ctx, (vector unsigned char **)destbufs, srcbuf, i);
	    }
	  else
	    {
	      /* all white */

	      for (plane_idx = 0; plane_idx < last_plane; plane_idx++)
		{
		  uchar *dst_ptr = destbufs[plane_idx];
		  ((uint32 *)dst_ptr)[(i >> 2) + 0] = 0;
		  ((uint32 *)dst_ptr)[(i >> 2) + 1] = 0;
		  ((uint32 *)dst_ptr)[(i >> 2) + 2] = 0;
		  ((uint32 *)dst_ptr)[(i >> 2) + 3] = 0;
		}
	    }
	}

      for (i = xd & -16; i >= 0; i -= 16)
	{
	  if (ctx->skip_line[i >> 4])
	    {
#ifdef USE_AVEC
	      eb_avec_rev_rs(ctx, i + 15);
#endif
#ifdef USE_SSE2
	      eb_sse2_rev_rs(ctx, i + 15);
#endif
	    }
	}
    }
  return 0;
}
#endif

/* Maximum number of planes, but actually we want to dynamically
   allocate all scratch buffers that depend on this. */
#define M 16

static void
even_better_line_hi (EvenBetterCtx *ebc, uchar **dest,
		     const ET_Rll *const *src)
{
  int a[M], b[M];
  int e_1_0[M], e_m1_1[M], e_0_1[M], e_1_1[M];
  int iml[M], rbl[M];
  int i, j;
  int im;
  int *pa, *pb, *piir, *pr;
  int r[M], rg;
  int xd, xs;
  uint32 seed1 = ebc->seed1;
  uint32 seed2 = ebc->seed2;
  uint32 sum;
  int plane_idx;
  int r_scratch[M];
  int n_planes = ebc->n_planes;
  int levels = ebc->levels;
#ifdef OLD_QUANT
  int dith_mul = levels << 8;
#else
  int dith_mul = (levels - 1) << 8;
#endif
  int imo_mul = (1 << (EVEN_SHIFT + IMO_SHIFT)) / (levels - 1);
  int aspect2 = ebc->aspect * ebc->aspect;
  int *strengths = ebc->strengths;
  int even_elo = ebc->even_elo;
  int even_ehi = ebc->even_ehi;
  int coupling;
  int *c_line = ebc->c_line;
  int even_c1 = ebc->even_c1;
  int rand_shift;
  int even_rlimit = 1 << (30 - EVEN_SHIFT + even_c1);
  int count[M], src_idx[M];
  int rs[M];

  xs = ebc->source_width;
  xd = ebc->dest_width;

  for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
    {
      a[plane_idx] = 1;
      b[plane_idx] = aspect2;
      r[plane_idx] = 0;
      e_0_1[plane_idx] = 0;
      e_1_0[plane_idx] = 0;
      e_1_1[plane_idx] = 0;
      count[plane_idx] = 0;
      src_idx[plane_idx] = 0;
    }

  coupling = 0;

  for (i = 0; i < xd;)
    {
      int work_planes[M];
      int n_work = 0;
      int work_idx;
      int jmax;

      jmax = (xd - i) > 16 ? 16 : xd - i;

      for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
	{
	  EBPlaneCtx *ctx = ebc->plane_ctx[plane_idx];
	  int *wcl = ctx->white_count_line;
	  if (count[plane_idx] >= 16 && iml[plane_idx] == 0)
	    wcl[i >> 4]++;
	  else
	    wcl[i >> 4] = 0;
	  if (wcl[i >> 4] > 15)
	    {
	      uchar *dst_ptr = dest[plane_idx];
	      if (jmax == 16)
		{
		  ((uint32 *)dst_ptr)[(i >> 2) + 0] = 0;
		  ((uint32 *)dst_ptr)[(i >> 2) + 1] = 0;
		  ((uint32 *)dst_ptr)[(i >> 2) + 2] = 0;
		  ((uint32 *)dst_ptr)[(i >> 2) + 3] = 0;
		}
	      else
		{
		  for (j = 0; j < jmax; j++)
		    dst_ptr[i + j] = 0;
		}
	      count[plane_idx] -= jmax;
	    }
	  else
	    {
	      work_planes[n_work++] = plane_idx;
	    }
	}

      if (n_work == 0)
	{
	  /* all planes were white */
	  i += jmax;
	  continue;
	}

      for (j = 0; j < jmax; j++)
	{
#ifdef FANCY_COUPLING
	  coupling += c_line[i];
#else
	  coupling = 0;
#endif
	  /* Lookup image data and compute R for all planes. */
	  for (work_idx = 0; work_idx < n_work; work_idx++)
	    {
	      int plane_idx = work_planes[work_idx];
	      EBPlaneCtx *ctx = ebc->plane_ctx[plane_idx];
	      ET_SrcPixel src_pixel;
	      int new_r;

	      pr = ctx->r_line;
	      pa = ctx->a_line;
	      pb = ctx->b_line;
	      if (count[plane_idx] == 0)
		{
		  const ET_Rll *src_p = src[plane_idx] + src_idx[plane_idx]++;
		  int *lut = ctx->lut;
		  int *rblut = ctx->rb_lut;
		  char *rslut = ctx->rs_lut;

		  count[plane_idx] = src_p->length;
		  src_pixel = src_p->value;
		  iml[plane_idx] = lut[src_pixel];
		  rbl[plane_idx] = rblut[src_pixel];
		  rs[plane_idx] = rslut[src_pixel];
		}
	      count[plane_idx]--;

	      if (r[plane_idx] + a[plane_idx] < pr[i])
		{
		  r[plane_idx] += a[plane_idx];
		  a[plane_idx] += 2;
		}
	      else
		{
		  a[plane_idx] = pa[i];
		  b[plane_idx] = pb[i];
		  r[plane_idx] = pr[i];
		}
	      if (iml[plane_idx] == 0)
		{
		  r_scratch[plane_idx] = 0;
		}
	      else
		{
		  int r_tmp;
		  const int r_max = 0;
		  new_r = r[plane_idx];
		  if (new_r > even_rlimit)
		    new_r = even_rlimit;
		  /* Should we store back with the limit? */

		  rg = new_r << (EVEN_SHIFT - even_c1);
		  r_tmp = rg - rbl[plane_idx];
		  if (r_tmp > r_max) r_tmp >>= 3;
		  r_scratch[plane_idx] = r_tmp;
		}
	    }

	  /* Dither each plane. */
	  for (work_idx = 0; work_idx < n_work; work_idx++)
	    {
	      int plane_idx = work_planes[work_idx];
	      EBPlaneCtx *ctx = ebc->plane_ctx[plane_idx];
	      uchar *dst_ptr = dest[plane_idx];
	      int new_e_1_0;
	      int coupling_contribution;

	      pr = ctx->r_line;
	      pa = ctx->a_line;
	      pb = ctx->b_line;
	      piir = ctx->iir_line;

	      im = iml[plane_idx];
	      e_m1_1[plane_idx] = e_0_1[plane_idx];
	      e_0_1[plane_idx] = e_1_1[plane_idx];
	      e_1_1[plane_idx] = i == xd - 1 ? 0 : piir[i + 1];
	      new_e_1_0 = ((e_1_0[plane_idx] * 7 + e_m1_1[plane_idx] * 3 +
			    e_0_1[plane_idx] * 5 + e_1_1[plane_idx] * 1) >> 4);
	      if (im == 0)
		{
		  dst_ptr[i] = 0;
		}
	      else
		{
		  int err;
		  int imo;

		  err = new_e_1_0;

		  err += r_scratch[plane_idx];

		  /* Add the two seeds together */
		  sum = seed1 + seed2;

		  /* If the add generated a carry, increment
		   * the result of the addition.
		   */
		  if (sum < seed1 || sum < seed2) sum++;

		  /* Seed2 becomes old seed1, seed1 becomes result */
		  seed2 = seed1;
		  seed1 = sum;

		  rand_shift = rs[plane_idx];
		  err -= (sum >> rand_shift) - (0x80000000 >> rand_shift);

		  if (err < even_elo)
		    err = even_elo;

		  else if (err > even_ehi)
		    err = even_ehi;

#if 1
		  err += coupling;
#endif

#ifdef OLD_QUANT
		  imo = ((err + im) * dith_mul) >> (EVEN_SHIFT + 8);
#else
		  imo = ((err + im) * dith_mul + (1 << (EVEN_SHIFT + 7))) >> (EVEN_SHIFT + 8);
#endif
		  if (imo < 0) imo = 0;
		  else if (imo > levels - 1) imo = levels - 1;
		  dst_ptr[i] = imo;
		  coupling_contribution = im - ((imo * imo_mul) >> IMO_SHIFT);
		  new_e_1_0 += coupling_contribution;
		  coupling += (coupling_contribution * strengths[plane_idx]) >> 8;
		}
	      if (dst_ptr[i] != 0)
		{
		  a[plane_idx] = 1;
		  b[plane_idx] = aspect2;
		  r[plane_idx] = 0;
		}
	      pa[i] = a[plane_idx];
	      pb[i] = b[plane_idx];
	      pr[i] = r[plane_idx];
	      piir[i] = new_e_1_0;
	      e_1_0[plane_idx] = new_e_1_0;
	    }
#ifdef FANCY_COUPLING
	  coupling = coupling >> 1;
	  c_line[i] = coupling;
#endif
	  i++;
	}
    }

  /* Note: this isn't white optimized, but the payoff is probably not
     that important. */
#ifdef FANCY_COUPLING
  coupling = 0;
  for (i = xd - 1; i >= 0; i--)
    {
      coupling = (coupling + c_line[i]) >> 1;
      c_line[i] = (coupling - (coupling >> 4));
    }
#endif

  /* Update distances. */
  for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
    {
      EBPlaneCtx *ctx = ebc->plane_ctx[plane_idx];
      int *wcl = ctx->white_count_line;
      int av, bv, rv;
      int jmax;

      pr = ctx->r_line;
      pa = ctx->a_line;
      pb = ctx->b_line;

      av = 1;
      bv = 1;
      rv = 0;
      jmax = ((xd - 1) & 15) + 1;
      for (i = xd - 1; i >= 0;)
	{
	  if (wcl[i >> 4] < 16)
	    {
	      for (j = 0; j < jmax; j++)
		{
		  if (rv + bv + av < pr[i] + pb[i])
		    {
		      rv += av;
		      av += 2;
		    }
		  else
		    {
		      rv = pr[i];
		      av = pa[i];
		      bv = pb[i];
		    }
		  if (rv > even_rlimit) rv = even_rlimit;
		  pa[i] = av;
		  pb[i] = bv + (aspect2 << 1);
		  pr[i] = rv + bv;
		  i--;
		}
	    }
	  else
	    i -= jmax;
	  jmax = 16;
	}
    }

   ebc->seed1 = seed1;
   ebc->seed2 = seed2;
}

static void
even_better_line_both (EvenBetterCtx *ebc, uchar **dest,
		       const ET_Rll *const *src)
{
#if 0
  int a[M], b[M];
  int a_sh[M], b_sh[M];
  int e_1_0[M], e_m1_1[M], e_0_1[M], e_1_1[M];
  int imraw[M];
  int iml[M];
  int i;
  int im;
  int *lut;
  const ET_SrcPixel *ps;
  int *pa, *pb, *piir, *pr;
  int *pa_sh, *pb_sh, *pr_sh;
  int r[M], rb, rg;
  int r_sh[M];
  int *rblut;
  int xd, xrem, xs;
  uint32 seed1 = ebc->seed1;
  uint32 seed2 = ebc->seed2;
  uint32 sum;
  int plane_idx;
  int r_scratch[M];
  int src_idx;
  int n_planes = ebc->n_planes;
  int levels = ebc->levels;
#ifdef OLD_QUANT
  int dith_mul = levels << 8;
#else
  int dith_mul = (levels - 1) << 8;
#endif
  int imo_mul = (1 << (EVEN_SHIFT + IMO_SHIFT)) / (levels - 1);
  int aspect2 = ebc->aspect * ebc->aspect;
  int *strengths = ebc->strengths;
  int even_elo= ebc->even_elo;
  int even_ehi= ebc->even_ehi;
  int coupling;
  int *c_line = ebc->c_line;
  int even_c1 = ebc->even_c1;
  int rand_shift = ebc->rand_shift;
  int even_rlimit = 1 << (30 - EVEN_SHIFT + even_c1);

  xs = ebc->source_width;
  xd = ebc->dest_width;
  xrem = xd - xs;

  for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
    {
      a[plane_idx] = 1;
      b[plane_idx] = aspect2;
      a_sh[plane_idx] = 1;
      b_sh[plane_idx] = aspect2;
      r[plane_idx] = 0;
      r_sh[plane_idx] = 0;
      e_0_1[plane_idx] = 0;
      e_1_0[plane_idx] = 0;
      e_1_1[plane_idx] = 0;
    }

  coupling = 0;

  src_idx = 0;
  for (i = 0; i < xd; i++)
    {
#ifdef FANCY_COUPLING
      coupling += c_line[i];
#else
      coupling = 0;
#endif

      xrem += xs;
      if (xrem >= xd)
	{
	  for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
	    {
	      ps = src[plane_idx];
	      imraw[plane_idx] = ps[src_idx];
	    }
	  src_idx++;
	  xrem -= xd;
	}

      /* Lookup image data and compute R for all planes. */
      for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
	{
	  EBPlaneCtx *ctx = ebc->plane_ctx[plane_idx];
	  ET_SrcPixel src_pixel;
	  int new_r;

	  pr = ctx->r_line;
	  pa = ctx->a_line;
	  pb = ctx->b_line;
	  pr_sh = ctx->r_line_sh;
	  pa_sh = ctx->a_line_sh;
	  pb_sh = ctx->b_line_sh;
	  lut = ctx->lut;
	  rblut = ctx->rb_lut;
	  src_pixel = imraw[plane_idx];

	  im = lut[src_pixel];
	  iml[plane_idx] = im;
	  rb = rblut[src_pixel];
	  if (r[plane_idx] + a[plane_idx] < pr[i])
	    {
	      r[plane_idx] += a[plane_idx];
	      a[plane_idx] += 2;
	    }
	  else
	    {
	      a[plane_idx] = pa[i];
	      b[plane_idx] = pb[i];
	      r[plane_idx] = pr[i];
	    }
	  if (r_sh[plane_idx] + a_sh[plane_idx] < pr_sh[i])
	    {
	      r_sh[plane_idx] += a_sh[plane_idx];
	      a_sh[plane_idx] += 2;
	    }
	  else
	    {
	      a_sh[plane_idx] = pa_sh[i];
	      b_sh[plane_idx] = pb_sh[i];
	      r_sh[plane_idx] = pr_sh[i];
	    }
	  if (im == 0 || im == (1 << EVEN_SHIFT))
	    {
	      r_scratch[plane_idx] = 0;
	    }
	  else
	    {
	      new_r = r[plane_idx];
	      if (new_r > even_rlimit)
		new_r = even_rlimit;
	      /* Should we store back with the limit? */
	      rg = new_r << (EVEN_SHIFT - even_c1);

	      new_r = r_sh[plane_idx];
	      if (new_r > even_rlimit)
		new_r = even_rlimit;
	      rg -= new_r << (EVEN_SHIFT - even_c1);
	      r_scratch[plane_idx] = rg - rb;
	    }
	}

      /* Dither each plane. */
      for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
	{
	  EBPlaneCtx *ctx = ebc->plane_ctx[plane_idx];
	  uchar *dst_ptr = dest[plane_idx];
	  int new_e_1_0;
	  int coupling_contribution;

	  pr = ctx->r_line;
	  pa = ctx->a_line;
	  pb = ctx->b_line;
	  pr_sh = ctx->r_line_sh;
	  pa_sh = ctx->a_line_sh;
	  pb_sh = ctx->b_line_sh;
	  piir = ctx->iir_line;

	  im = iml[plane_idx];
	  e_m1_1[plane_idx] = e_0_1[plane_idx];
	  e_0_1[plane_idx] = e_1_1[plane_idx];
	  e_1_1[plane_idx] = i == xd - 1 ? 0 : piir[i + 1];
	  new_e_1_0 = ((e_1_0[plane_idx] * 7 + e_m1_1[plane_idx] * 3 +
			e_0_1[plane_idx] * 5 + e_1_1[plane_idx] * 1) >> 4);
	  if (im == 0)
	    {
	      dst_ptr[i] = 0;
	    }
	  else
	    {
	      int err;
	      int imo;

	      err = new_e_1_0;

	      err += r_scratch[plane_idx];

	      /* Add the two seeds together */
	      sum = seed1 + seed2;

	      /* If the add generated a carry, increment
	       * the result of the addition.
	       */
	      if (sum < seed1 || sum < seed2) sum++;

	      /* Seed2 becomes old seed1, seed1 becomes result */
	      seed2 = seed1;
	      seed1 = sum;

	      err -= (sum >> rand_shift) - (0x80000000 >> rand_shift);

	      if (err < even_elo)
		err = even_elo;

	      else if (err > even_ehi)
		err = even_ehi;

#if 1
	      err += coupling;
#endif

#ifdef OLD_QUANT
	      imo = ((err + im) * dith_mul) >> (EVEN_SHIFT + 8);
#else
	      imo = ((err + im) * dith_mul + (1 << (EVEN_SHIFT + 7))) >> (EVEN_SHIFT + 8);
#endif
	      if (imo < 0) imo = 0;
	      else if (imo > levels - 1) imo = levels - 1;
	      dst_ptr[i] = imo;
	      coupling_contribution = im - ((imo * imo_mul) >> IMO_SHIFT);
	      new_e_1_0 += coupling_contribution;
	      coupling += (coupling_contribution * strengths[plane_idx]) >> 8;
	    }
	  if (dst_ptr[i] != 0)
	    {
	      a[plane_idx] = 1;
	      b[plane_idx] = aspect2;
	      r[plane_idx] = 0;
	    }
	  if (dst_ptr[i] != levels - 1)
	    {
	      a_sh[plane_idx] = 1;
	      b_sh[plane_idx] = aspect2;
	      r_sh[plane_idx] = 0;
	    }
	  pa[i] = a[plane_idx];
	  pb[i] = b[plane_idx];
	  pr[i] = r[plane_idx];
	  pa_sh[i] = a_sh[plane_idx];
	  pb_sh[i] = b_sh[plane_idx];
	  pr_sh[i] = r_sh[plane_idx];
	  piir[i] = new_e_1_0;
	  e_1_0[plane_idx] = new_e_1_0;
	}
#ifdef FANCY_COUPLING
      coupling = coupling >> 1;
      c_line[i] = coupling;
#endif
    }

#ifdef FANCY_COUPLING
  coupling = 0;
  for (i = xd - 1; i >= 0; i--)
    {
      if (plane_idx == 0)
	{
	  coupling = (coupling + c_line[i]) >> 1;
	  c_line[i] = (coupling - (coupling >> 4));
	}
    }
#endif

  /* Update distances. */
  for (plane_idx = 0; plane_idx < n_planes; plane_idx++)
    {
      EBPlaneCtx *ctx = ebc->plane_ctx[plane_idx];
      int av, bv, rv;
      int av_sh, bv_sh, rv_sh;

      pr = ctx->r_line;
      pa = ctx->a_line;
      pb = ctx->b_line;
      pr_sh = ctx->r_line_sh;
      pa_sh = ctx->a_line_sh;
      pb_sh = ctx->b_line_sh;

      av = 1;
      bv = 1;
      rv = 0;
      av_sh = 1;
      bv_sh = 1;
      rv_sh = 0;
      for (i = xd - 1; i >= 0; i--)
	{
	  if (rv + bv + av < pr[i] + pb[i])
	    {
	      rv += av;
	      av += 2;
	    }
	  else
	    {
	      rv = pr[i];
	      av = pa[i];
	      bv = pb[i];
	    }
	  if (rv > even_rlimit) rv = even_rlimit;
	  pa[i] = av;
	  pb[i] = bv + (aspect2 << 1);
	  pr[i] = rv + bv;

	  if (rv_sh + bv_sh + av_sh < pr_sh[i] + pb_sh[i])
	    {
	      rv_sh += av_sh;
	      av_sh += 2;
	    }
	  else
	    {
	      rv_sh = pr_sh[i];
	      av_sh = pa_sh[i];
	      bv_sh = pb_sh[i];
	    }
	  if (rv_sh > even_rlimit) rv_sh = even_rlimit;
	  pa_sh[i] = av_sh;
	  pb_sh[i] = bv_sh + (aspect2 << 1);
	  pr_sh[i] = rv_sh + bv_sh;
	}
    }

   ebc->seed1 = seed1;
   ebc->seed2 = seed2;
#endif
}

/**
 * even_better_line_rll: Screen a line using Even ToneFS screeing.
 * @ctx: An #EBPlaneCtx context.
 * @dest: Array of destination buffers, 8 bpp pixels each.
 * @src: Array of source buffers, runlength encoded.
 *
 * Screens a single line using Even ToneFS screening.
 **/
void
even_better_line_rll (EvenBetterCtx *ebc, uchar **dest,
		      const ET_Rll *const *src)
{

  if (ebc->dump_file && ebc->dump_level >= EB_DUMP_INPUT)
    {
      int i;

      /* Note: we should calculate the actual number of runlength
	 codes here. As it is, it will just waste storage a bit. */
      for (i = 0; i < ebc->n_planes; i++)
	fwrite (src[i], sizeof(ET_Rll), ebc->source_width,
		ebc->dump_file);
    }
#ifdef USE_VECTOR
  if (ebc->using_vectors)
    even_better_line_vector(ebc, dest, src);
  else
#endif
  if (ebc->do_shadows)
    even_better_line_both (ebc, dest, src);
  else
    even_better_line_hi (ebc, dest, src);
  if (ebc->dump_file && ebc->dump_level >= EB_DUMP_INPUT)
    {
      int i;

      for (i = 0; i < ebc->n_planes; i++)
	fwrite (dest[i], 1, ebc->dest_width,
		ebc->dump_file);
    }
}

/**
 * even_better_compress_rll: Compress a single scan line to RLL format.
 * @dst: Destination buffer.
 * @src: Source buffer.
 * @width: Number of source pixels.
 *
 * Return value: number of runlength codes.
 **/
static int
even_better_compress_rll (ET_Rll *dst, const ET_SrcPixel *src,
			  int src_width, int dst_width)
{
  int rll_idx;
  int i;
  int count;
  ET_SrcPixel last_val;
  int whole = dst_width / src_width;
  int frac = dst_width % src_width;
  int rem;

  rll_idx = 0;
  last_val = src[0];
  count = whole;
  if (frac == 0)
    {
      for (i = 1; i < src_width; i++)
	{
	  ET_SrcPixel val = src[i];
	  
	  if (count > 0xffff - whole || val != last_val)
	    {
	      dst[rll_idx].length = count;
	      dst[rll_idx].value = last_val;
	      rll_idx++;
	      last_val = val;
	      count = 0;
	    }
	  count += whole;
	}
    }
  else
    {
      rem = frac;
      for (i = 1; i < src_width; i++)
	{
	  ET_SrcPixel val = src[i];
	  
	  if (count >= 0xffff - whole || val != last_val)
	    {
	      dst[rll_idx].length = count;
	      dst[rll_idx].value = last_val;
	      rll_idx++;
	      last_val = val;
	      count = 0;
	    }
	  count += whole;
	  rem += frac;
	  if (rem >= src_width)
	    {
	      count++;
	      rem -= src_width;
	    }
	}
    }
  dst[rll_idx].length = count;
  dst[rll_idx].value = last_val;
  rll_idx++;
  return rll_idx;
}

/**
 * even_better_line: Screen a line using Even TonenFS screeing.
 * @ctx: An #EBPlaneCtx context.
 * @dest: Array of destination buffers, 8 bpp pixels each.
 * @src: Array of source buffer, ET_SrcPixel pixels each.
 *
 * Screens a single line using Even ToneFS screening.
 **/
void
even_better_line (EvenBetterCtx *ebc, uchar **dest,
		      const ET_SrcPixel *const *src)
{
  ET_Rll *rll_buf[M];
  int i;
  int source_width = ebc->source_width;
  int dest_width = ebc->dest_width;

#ifdef USE_AVEC
  if (ebc->using_vectors == 2)
    {
      even_better_line_fastprep (ebc, dest, src);
    }
  else
#endif
    {
      for (i = 0; i < ebc->n_planes; i++)
	{
	  rll_buf[i] = (ET_Rll *)malloc (source_width * sizeof(ET_Rll));
	  even_better_compress_rll (rll_buf[i], src[i], source_width, dest_width);
	}
      even_better_line_rll (ebc, dest, (const ET_Rll * const *)rll_buf);
      for (i = 0; i < ebc->n_planes; i++)
	free (rll_buf[i]);
    }
}

/**
 * even_better_plane_free: Free an #EBPlaneCtx context.
 * @ctx: The #EBPlaneCtx context to free.
 *
 * Frees @ctx.
 **/
static void
even_better_plane_free (EBPlaneCtx *ctx)
{
  free (ctx->rb_line);
  free (ctx->iir_line);
  free (ctx->r_line);
  free (ctx->a_line);
  free (ctx->b_line);
  free (ctx->lut);
  free (ctx->rb_lut);
  free (ctx->rs_lut);
  free (ctx->white_count_line);
  free (ctx);
}

static int
even_log2 (int x)
{
  int y = 0;
  int z;

  for (z = x; z > 1; z = z >> 1)
    y++;
  return y;
}

/**
 * even_better_new: Create new Even ToneFS screening context.
 * @source_width: Width of source buffer.
 * @dest_width: Width of destination buffer, in pixels.
 * @lut: Lookup table for gray values.
 *
 * Creates a new context for Even ToneFS screening.
 *
 * If @dest_width is larger than @source_width, then input lines will
 * be expanded using nearest-neighbor sampling.
 *
 * @lut should be an array of 256 values, one for each possible input
 * gray value. @lut is a lookup table for gray values. Each value
 * ranges from 0 (black) to 2^24 (white).
 *
 * Return value: The new #EBPlaneCtx context.
 **/
static EBPlaneCtx *
even_better_plane_new (const EvenBetterParams *params, EvenBetterCtx *ebc,
		       int plane_idx)
{
  int source_width = params->source_width;
  int dest_width = params->dest_width;
  int *lut = params->luts[plane_idx];
  EBPlaneCtx *result;
  int i;
  int *new_lut;
  int *rb_lut;
  char *rs_lut;
  double rbscale = eb_compute_rbscale(params);
  int even_c1 = ebc->even_c1;
  int even_rlimit = 1 << (30 - EVEN_SHIFT + even_c1);
  int do_shadows = params->do_shadows;
  int log2_levels;
  int rs_base;

  result = (EBPlaneCtx *)malloc (sizeof(EBPlaneCtx));

  result->source_width = source_width;
  result->dest_width = dest_width;

  new_lut = (int *)malloc ((ET_SRC_MAX + 1) * sizeof(int));
  for (i = 0; i < ET_SRC_MAX + 1; i++)
    {
      int nli;

      if (lut == NULL)
	{
#if ET_SRC_MAX == 255
	  nli = (i * 65793 + (i >> 7)) >> (24 - EVEN_SHIFT);
#else
	  nli = (i * ((double) (1 << EVEN_SHIFT)) / ET_SRC_MAX) + 0.5;
#endif
	}
      else
	nli = lut[i] >> (24 - EVEN_SHIFT);
      new_lut[i] = (1 << EVEN_SHIFT) - nli;
    }

  rb_lut = (int *)malloc ((ET_SRC_MAX + 1) * sizeof(int));
  rs_lut = (char *)malloc ((ET_SRC_MAX + 1) * sizeof(int));

  log2_levels = even_log2 (params->levels);
  rs_base = 35 - EVEN_SHIFT + log2_levels - params->rand_scale;

  for (i = 0; i <= ET_SRC_MAX; i++)
    {
      double rb;
      int nl = new_lut[i] * (params->levels - 1);
      int rs;

      if (nl == 0)
	rb = 0;
      else
	{
	  rb = (rbscale * (1 << (2 * EVEN_SHIFT - even_c1))) / nl;
	  if (rb > even_rlimit << (EVEN_SHIFT - even_c1))
	    rb = even_rlimit << (EVEN_SHIFT - even_c1);
	}

      rs = eb_compute_randshift(nl, rs_base, do_shadows, params->levels);
      rs_lut[i] = rs;

      if (params->do_shadows)
	{
	  nl = ((1 << EVEN_SHIFT) - new_lut[i]) * (params->levels - 1);

	  if (nl == 0)
	    rb = 0;
	  else
	    {
	      int rb_sh;
	      rb_sh = (rbscale * (1 << (2 * EVEN_SHIFT - even_c1))) / nl;
	      if (rb_sh > even_rlimit << (EVEN_SHIFT - even_c1))
		rb_sh = even_rlimit << (EVEN_SHIFT - even_c1);
	      rb -= rb_sh;
	    }
	}
      rb_lut[i] = rb;

    }

  result->lut = new_lut;
  result->rb_lut = rb_lut;
  result->rs_lut = rs_lut;

  result->rb_line = (int *)calloc (dest_width, sizeof(int));
  result->iir_line = (int *)calloc (dest_width, sizeof(int));
  result->r_line = (int *)calloc (dest_width, sizeof(int));
  result->a_line = (int *)calloc (dest_width, sizeof(int));
  result->b_line = (int *)calloc (dest_width, sizeof(int));
  result->white_count_line = (int *)calloc ((dest_width + 15) >> 4, sizeof(int));
  if (do_shadows)
    {
      result->r_line_sh = (int *)calloc (dest_width, sizeof(int));
      result->a_line_sh = (int *)calloc (dest_width, sizeof(int));
      result->b_line_sh = (int *)calloc (dest_width, sizeof(int));
    }
  else
    {
      result->r_line_sh = NULL;
      result->a_line_sh = NULL;
      result->b_line_sh = NULL;
    }
  for (i = 0; i < dest_width; i++)
    {
      result->a_line[i] = 1;
      result->b_line[i] = 1;
      result->iir_line[i] = -((rand () & 0x7fff) << 6) >> (24 - EVEN_SHIFT);
      if (do_shadows)
	{
	  result->a_line_sh[i] = 1;
	  result->b_line_sh[i] = 1;
	}
    }

  return result;
}

EvenBetterCtx *
even_better_new (const EvenBetterParams *params)
{
  EvenBetterCtx *result = (EvenBetterCtx *)malloc (sizeof(EvenBetterCtx));
  int n_planes = params->n_planes;
  int i;
  int log2_levels, log2_aspect;
  int using_vectors = 0;

  if (params->dump_file)
    {
      int header[5];

      header[0] = 0x70644245;
      header[1] = 'M' * 0x1010000 + 'I' * 0x101;
      header[2] = EVENBETTER_VERSION;
      header[3] = ET_SRC_MAX;
      header[4] = sizeof(ET_SrcPixel);
      fwrite (header, sizeof(int), sizeof(header) / sizeof(header[0]),
	      params->dump_file);
      if (params->dump_level >= EB_DUMP_PARAMS)
	{

	  fwrite (params, 1, sizeof(EvenBetterParams), params->dump_file);
	}
      if (params->dump_level >= EB_DUMP_LUTS)
	{
	  int i;
	  for (i = 0; i < params->n_planes; i++)
	    fwrite (params->luts[i], sizeof(int), ET_SRC_MAX + 1,
		    params->dump_file);
	}
    }

  result->source_width = params->source_width;
  result->dest_width = params->dest_width;
  result->n_planes = n_planes;
  result->levels = params->levels;

  result->aspect = params->aspect;

  result->even_ehi = 0.6 * (1 << EVEN_SHIFT) / (params->levels - 1);
  result->even_elo = -result->even_ehi;

  result->strengths = (int *)malloc (sizeof(int) * n_planes);
  memcpy (result->strengths, params->strengths,
	  sizeof(int) * n_planes);

  log2_levels = even_log2 (params->levels);
  log2_aspect = even_log2 (params->aspect);
  result->even_c1 = 6 + log2_aspect + log2_levels - params->even_c1_scale;
  result->do_shadows = params->do_shadows;

  result->c_line = (int *)calloc (params->dest_width, sizeof(int));

  result->seed1 = 0x5324879f;
  result->seed2 = 0xb78d0945;

  result->dump_file = params->dump_file;
  result->dump_level = params->dump_level;

#ifdef USE_SSE2
  using_vectors = eb_test_sse2();
#endif
#ifdef USE_AVEC
  using_vectors = 1; /* todo: Altivec sensing */

  /* select fastprep */
  if (sizeof(ET_SrcPixel) == 1 && using_vectors && params->gamma != 0)
    using_vectors = 2;

#endif

#ifdef USE_VECTOR
  result->using_vectors = using_vectors;
#endif
  if (using_vectors)
    {
#ifdef USE_SSE2
      result->sse2_ctx = (eb_ctx_sse2 **)malloc(sizeof(eb_ctx_sse2 *) *
						((n_planes + 3) >> 2));
      for (i = 0; i < n_planes; i += 4)
	{
	  int end_plane = i + 4 < n_planes ? i + 4 : n_planes;
	  result->sse2_ctx[i >> 2] = eb_ctx_sse2_new(params, i, end_plane);
	}
#endif
#ifdef USE_AVEC
      result->avec_ctx = (eb_ctx_avec **)malloc(sizeof(eb_ctx_avec *) *
						((n_planes + 3) >> 2));
      for (i = 0; i < n_planes; i += 4)
	{
	  int end_plane = i + 4 < n_planes ? i + 4 : n_planes;
	  result->avec_ctx[i >> 2] = eb_ctx_avec_new(params, i, end_plane);
	}
#endif
      result->plane_ctx = NULL;
    }
  else
    {
      result->plane_ctx = (EBPlaneCtx **)malloc(sizeof(EBPlaneCtx *) * n_planes);
      for (i = 0; i < n_planes; i++)
	result->plane_ctx[i] = even_better_plane_new (params, result, i);
    }
  return result;
}

/**
 * even_better_free: Free an #EvenBetterCtx context.
 * @ctx: The #EvenBetterCtx context to free.
 *
 * Frees @ctx.
 **/
void
even_better_free (EvenBetterCtx *ctx)
{
  int i;
  int n_planes = ctx->n_planes;

  if (ctx->dump_file)
    fclose (ctx->dump_file);

#ifdef USE_VECTOR
  if (ctx->using_vectors)
    {
#ifdef USE_SSE2
      for (i = 0; i < n_planes; i += 4)
	eb_ctx_sse2_free(ctx->sse2_ctx[i >> 2]);
      free(ctx->sse2_ctx);
#endif
#ifdef USE_AVEC
      for (i = 0; i < n_planes; i += 4)
	eb_ctx_avec_free(ctx->avec_ctx[i >> 2]);
      free(ctx->avec_ctx);
#endif
    }
  else
#endif
    {
      for (i = 0; i < n_planes; i++)
	even_better_plane_free (ctx->plane_ctx[i]);
      free(ctx->plane_ctx);
    }
  free (ctx->strengths);
  free (ctx->c_line);

  free (ctx);
}
