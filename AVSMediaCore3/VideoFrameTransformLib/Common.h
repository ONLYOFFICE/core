#pragma once

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (fastconvert_packedFunc)(void* x_ptr, int x_stride,
									  void* y_src, void* v_src, void* u_src, int y_stride, int uv_stride,
                                      int width, int height, int vflip);
typedef fastconvert_packedFunc *fastconvert_packedFuncPtr;
/* plain c */
extern fastconvert_packedFunc fastconvert_rgb555_to_yv12_c;
extern fastconvert_packedFunc fastconvert_rgb565_to_yv12_c;
extern fastconvert_packedFunc fastconvert_bgr_to_yv12_c;
extern fastconvert_packedFunc fastconvert_bgra_to_yv12_c;
extern fastconvert_packedFunc fastconvert_abgr_to_yv12_c;
extern fastconvert_packedFunc fastconvert_rgba_to_yv12_c;
extern fastconvert_packedFunc fastconvert_argb_to_yv12_c;
extern fastconvert_packedFunc fastconvert_yuyv_to_yv12_c;
extern fastconvert_packedFunc fastconvert_uyvy_to_yv12_c;

extern fastconvert_packedFunc fastconvert_rgb555i_to_yv12_c;
extern fastconvert_packedFunc fastconvert_rgb565i_to_yv12_c;
extern fastconvert_packedFunc fastconvert_bgri_to_yv12_c;
extern fastconvert_packedFunc fastconvert_bgrai_to_yv12_c;
extern fastconvert_packedFunc fastconvert_abgri_to_yv12_c;
extern fastconvert_packedFunc fastconvert_rgbai_to_yv12_c;
extern fastconvert_packedFunc fastconvert_argbi_to_yv12_c;
extern fastconvert_packedFunc fastconvert_yuyvi_to_yv12_c;
extern fastconvert_packedFunc fastconvert_uyvyi_to_yv12_c;

/* mmx */
extern fastconvert_packedFunc fastconvert_bgr_to_yv12_mmx;
extern fastconvert_packedFunc fastconvert_bgra_to_yv12_mmx;
extern fastconvert_packedFunc fastconvert_yuyv_to_yv12_mmx;
extern fastconvert_packedFunc fastconvert_uyvy_to_yv12_mmx;

/* 3dnow */
extern fastconvert_packedFunc fastconvert_yuyv_to_yv12_3dn;
extern fastconvert_packedFunc fastconvert_uyvy_to_yv12_3dn;

/* xmm */
extern fastconvert_packedFunc fastconvert_yuyv_to_yv12_xmm;
extern fastconvert_packedFunc fastconvert_uyvy_to_yv12_xmm;


/* plain c */
extern fastconvert_packedFunc fastconvert_yv12_to_rgb555_c;
extern fastconvert_packedFunc fastconvert_yv12_to_rgb565_c;
extern fastconvert_packedFunc fastconvert_yv12_to_bgr_c;
extern fastconvert_packedFunc fastconvert_yv12_to_bgra_c;
extern fastconvert_packedFunc fastconvert_yv12_to_abgr_c;
extern fastconvert_packedFunc fastconvert_yv12_to_rgba_c;
extern fastconvert_packedFunc fastconvert_yv12_to_argb_c;
extern fastconvert_packedFunc fastconvert_yv12_to_yuyv_c;
extern fastconvert_packedFunc fastconvert_yv12_to_uyvy_c;

extern fastconvert_packedFunc fastconvert_yv12_to_rgb555i_c;
extern fastconvert_packedFunc fastconvert_yv12_to_rgb565i_c;
extern fastconvert_packedFunc fastconvert_yv12_to_bgri_c;
extern fastconvert_packedFunc fastconvert_yv12_to_bgrai_c;
extern fastconvert_packedFunc fastconvert_yv12_to_abgri_c;
extern fastconvert_packedFunc fastconvert_yv12_to_rgbai_c;
extern fastconvert_packedFunc fastconvert_yv12_to_argbi_c;
extern fastconvert_packedFunc fastconvert_yv12_to_yuyvi_c;
extern fastconvert_packedFunc fastconvert_yv12_to_uyvyi_c;

/* mmx */
extern fastconvert_packedFunc fastconvert_yv12_to_bgr_mmx;
extern fastconvert_packedFunc fastconvert_yv12_to_bgra_mmx;
extern fastconvert_packedFunc fastconvert_yv12_to_yuyv_mmx;
extern fastconvert_packedFunc fastconvert_yv12_to_uyvy_mmx;

extern fastconvert_packedFunc fastconvert_yv12_to_yuyvi_mmx;
extern fastconvert_packedFunc fastconvert_yv12_to_uyvyi_mmx;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (fastconvert_planarFunc)(void* y_dst, void* u_dst, void* v_dst, int y_dst_stride, int uv_dst_stride,
                                      void* y_src, void* u_src, void* v_src, int y_src_stride, int uv_src_stride,
                                      int width, int height, int vflip);
typedef fastconvert_planarFunc *fastconvert_planarFuncPtr;

extern fastconvert_planarFunc fastconvert_yv12_to_yv12_c;
extern fastconvert_planarFunc fastconvert_yv12_to_yv12_mmx;
extern fastconvert_planarFunc fastconvert_yv12_to_yv12_xmm;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (fastconvet_emmsFunc) ();
typedef fastconvet_emmsFunc *fastconvert_emmsFuncPtr;

extern fastconvet_emmsFunc fastconvert_emms_c;
extern fastconvet_emmsFunc fastconvert_emms_mmx;
extern fastconvet_emmsFunc fastconvert_emms_3dn;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void (fastconvert_Func)(void* dst, void* src, int w, int srcpitch);
typedef fastconvert_Func *fastconvert_FuncPtr;

extern fastconvert_Func fastconvert_asm_blend_row_clipped;
extern fastconvert_Func fastconvert_asm_blend_row;
extern fastconvert_Func fastconvert_asm_blend_row_MMX;
extern fastconvert_Func fastconvert_asm_middle_row;
extern fastconvert_Func fastconvert_asm_middle_row_MMX;


///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void fastconvert_colorspace_init(void);
///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern int fastconvert_check_cpu_features(void);
extern void fastconvert_sse_os_trigger(void);
extern void fastconvert_sse2_os_trigger(void);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
extern void fastconvert_safe_packed_conv(void* x_ptr, int x_stride,
                                         void* y_ptr, void* u_ptr, void* v_ptr, int y_stride, int uv_stride,
                                         int width, int height, int vflip,
                                         fastconvert_packedFunc* func_opt, fastconvert_packedFunc func_c, int size);

#ifdef __cplusplus
}
#endif
