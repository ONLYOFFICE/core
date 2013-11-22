#pragma once

#define int8_t   char
#define uint8_t  unsigned char
#define int16_t  short
#define uint16_t unsigned short
#define int32_t  int
#define uint32_t unsigned int
#define int64_t  __int64
#define uint64_t unsigned __int64

/* initialize tables */

void fastconvert_colorspace_init(void);



/* colorspace conversion function (encoder) */

typedef void (fastconvert_packedFunc) (uint8_t * x_ptr,
								 int x_stride,
								 uint8_t * y_src,
								 uint8_t * v_src,
								 uint8_t * u_src,
								 int y_stride,
								 int uv_stride,
								 int width,
								 int height,
								 int vflip);

typedef fastconvert_packedFunc *fastconvert_packedFuncPtr;


/* plain c */
fastconvert_packedFunc fastconvert_rgb555_to_yv12_c;
fastconvert_packedFunc fastconvert_rgb565_to_yv12_c;
fastconvert_packedFunc fastconvert_bgr_to_yv12_c;
fastconvert_packedFunc fastconvert_bgra_to_yv12_c;
fastconvert_packedFunc fastconvert_abgr_to_yv12_c;
fastconvert_packedFunc fastconvert_rgba_to_yv12_c;
fastconvert_packedFunc fastconvert_argb_to_yv12_c;
fastconvert_packedFunc fastconvert_yuyv_to_yv12_c;
fastconvert_packedFunc fastconvert_uyvy_to_yv12_c;

fastconvert_packedFunc fastconvert_rgb555i_to_yv12_c;
fastconvert_packedFunc fastconvert_rgb565i_to_yv12_c;
fastconvert_packedFunc fastconvert_bgri_to_yv12_c;
fastconvert_packedFunc fastconvert_bgrai_to_yv12_c;
fastconvert_packedFunc fastconvert_abgri_to_yv12_c;
fastconvert_packedFunc fastconvert_rgbai_to_yv12_c;
fastconvert_packedFunc fastconvert_argbi_to_yv12_c;
fastconvert_packedFunc fastconvert_yuyvi_to_yv12_c;
fastconvert_packedFunc fastconvert_uyvyi_to_yv12_c;

/* mmx */
fastconvert_packedFunc fastconvert_bgr_to_yv12_mmx;
fastconvert_packedFunc fastconvert_bgra_to_yv12_mmx;
fastconvert_packedFunc fastconvert_yuyv_to_yv12_mmx;
fastconvert_packedFunc fastconvert_uyvy_to_yv12_mmx;

/* 3dnow */
fastconvert_packedFunc fastconvert_yuyv_to_yv12_3dn;
fastconvert_packedFunc fastconvert_uyvy_to_yv12_3dn;

/* xmm */
fastconvert_packedFunc fastconvert_yuyv_to_yv12_xmm;
fastconvert_packedFunc fastconvert_uyvy_to_yv12_xmm;


/* plain c */
fastconvert_packedFunc fastconvert_yv12_to_rgb555_c;
fastconvert_packedFunc fastconvert_yv12_to_rgb565_c;
fastconvert_packedFunc fastconvert_yv12_to_bgr_c;
fastconvert_packedFunc fastconvert_yv12_to_bgra_c;
fastconvert_packedFunc fastconvert_yv12_to_abgr_c;
fastconvert_packedFunc fastconvert_yv12_to_rgba_c;
fastconvert_packedFunc fastconvert_yv12_to_argb_c;
fastconvert_packedFunc fastconvert_yv12_to_yuyv_c;
fastconvert_packedFunc fastconvert_yv12_to_uyvy_c;

fastconvert_packedFunc fastconvert_yv12_to_rgb555i_c;
fastconvert_packedFunc fastconvert_yv12_to_rgb565i_c;
fastconvert_packedFunc fastconvert_yv12_to_bgri_c;
fastconvert_packedFunc fastconvert_yv12_to_bgrai_c;
fastconvert_packedFunc fastconvert_yv12_to_abgri_c;
fastconvert_packedFunc fastconvert_yv12_to_rgbai_c;
fastconvert_packedFunc fastconvert_yv12_to_argbi_c;
fastconvert_packedFunc fastconvert_yv12_to_yuyvi_c;
fastconvert_packedFunc fastconvert_yv12_to_uyvyi_c;

/* mmx */
fastconvert_packedFunc fastconvert_yv12_to_bgr_mmx;
fastconvert_packedFunc fastconvert_yv12_to_bgra_mmx;
fastconvert_packedFunc fastconvert_yv12_to_yuyv_mmx;
fastconvert_packedFunc fastconvert_yv12_to_uyvy_mmx;

fastconvert_packedFunc fastconvert_yv12_to_yuyvi_mmx;
fastconvert_packedFunc fastconvert_yv12_to_uyvyi_mmx;


typedef void (fastconvert_planarFunc) (
				uint8_t * y_dst, uint8_t * u_dst, uint8_t * v_dst,
				int y_dst_stride, int uv_dst_stride,
				uint8_t * y_src, uint8_t * u_src, uint8_t * v_src,
				int y_src_stride, int uv_src_stride,
				int width, int height, int vflip);
typedef fastconvert_planarFunc *fastconvert_planarFuncPtr;

fastconvert_planarFunc fastconvert_yv12_to_yv12_c;

fastconvert_planarFunc fastconvert_yv12_to_yv12_mmx;
fastconvert_planarFunc fastconvert_yv12_to_yv12_xmm;
