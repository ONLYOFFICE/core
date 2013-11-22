#include "basetransform.h"

CBaseTransform::CBaseTransform(void)
{
	static bool	init_Intel=false;

	if (!init_Intel)
	{
		ippStaticInitBest();
		init_Intel=true;
	}
	SetCPU(0);
}

CBaseTransform::~CBaseTransform(void)
{
}

void __fastcall CBaseTransform::SetCPU(unsigned int CPU)
{
	CPU_Flags = (CPU & CPU_FORCE) ? CPU : detect_cpu_flags();

	/* Restore FPU context : fastconvert_emms_c is a nop functions */
	fastconvert_emms = fastconvert_emms_c;

	/* Initialize internal colorspace transformation tables */
	fastconvert_colorspace_init();

	/* All colorspace transformation functions User Format->YV12 */
	fastconvert_yv12_to_yv12    = fastconvert_yv12_to_yv12_c;
	fastconvert_rgb555_to_yv12  = fastconvert_rgb555_to_yv12_c;
	fastconvert_rgb565_to_yv12  = fastconvert_rgb565_to_yv12_c;
	fastconvert_bgr_to_yv12     = fastconvert_bgr_to_yv12_c;
	fastconvert_bgra_to_yv12    = fastconvert_bgra_to_yv12_c;
	fastconvert_abgr_to_yv12    = fastconvert_abgr_to_yv12_c;
	fastconvert_rgba_to_yv12    = fastconvert_rgba_to_yv12_c;
	fastconvert_argb_to_yv12    = fastconvert_argb_to_yv12_c;
	fastconvert_yuyv_to_yv12    = fastconvert_yuyv_to_yv12_c;
	fastconvert_uyvy_to_yv12    = fastconvert_uyvy_to_yv12_c;

	fastconvert_rgb555i_to_yv12 = fastconvert_rgb555i_to_yv12_c;
	fastconvert_rgb565i_to_yv12 = fastconvert_rgb565i_to_yv12_c;
	fastconvert_bgri_to_yv12    = fastconvert_bgri_to_yv12_c;
	fastconvert_bgrai_to_yv12   = fastconvert_bgrai_to_yv12_c;
	fastconvert_abgri_to_yv12   = fastconvert_abgri_to_yv12_c;
	fastconvert_rgbai_to_yv12   = fastconvert_rgbai_to_yv12_c;
	fastconvert_argbi_to_yv12   = fastconvert_argbi_to_yv12_c;
	fastconvert_yuyvi_to_yv12   = fastconvert_yuyvi_to_yv12_c;
	fastconvert_uyvyi_to_yv12   = fastconvert_uyvyi_to_yv12_c;

	/* All colorspace transformation functions YV12->User format */
	fastconvert_yv12_to_rgb555  = fastconvert_yv12_to_rgb555_c;
	fastconvert_yv12_to_rgb565  = fastconvert_yv12_to_rgb565_c;
	fastconvert_yv12_to_bgr     = fastconvert_yv12_to_bgr_c;
	fastconvert_yv12_to_bgra    = fastconvert_yv12_to_bgra_c;
	fastconvert_yv12_to_abgr    = fastconvert_yv12_to_abgr_c;
	fastconvert_yv12_to_rgba    = fastconvert_yv12_to_rgba_c;
	fastconvert_yv12_to_argb    = fastconvert_yv12_to_argb_c;
	fastconvert_yv12_to_yuyv    = fastconvert_yv12_to_yuyv_c;
	fastconvert_yv12_to_uyvy    = fastconvert_yv12_to_uyvy_c;

	fastconvert_yv12_to_rgb555i = fastconvert_yv12_to_rgb555i_c;
	fastconvert_yv12_to_rgb565i = fastconvert_yv12_to_rgb565i_c;
	fastconvert_yv12_to_bgri    = fastconvert_yv12_to_bgri_c;
	fastconvert_yv12_to_bgrai   = fastconvert_yv12_to_bgrai_c;
	fastconvert_yv12_to_abgri   = fastconvert_yv12_to_abgri_c;
	fastconvert_yv12_to_rgbai   = fastconvert_yv12_to_rgbai_c;
	fastconvert_yv12_to_argbi   = fastconvert_yv12_to_argbi_c;
	fastconvert_yv12_to_yuyvi   = fastconvert_yv12_to_yuyvi_c;
	fastconvert_yv12_to_uyvyi   = fastconvert_yv12_to_uyvyi_c;

	if ((CPU_Flags & CPU_MMX) || (CPU_Flags & CPU_MMXEXT) ||
		(CPU_Flags & CPU_3DNOW) || (CPU_Flags & CPU_3DNOWEXT) ||
		(CPU_Flags & CPU_SSE) || (CPU_Flags & CPU_SSE2))
	{
		/* Restore FPU context : fastconvert_emms_c is a nop functions */
		fastconvert_emms = fastconvert_emms_mmx;
	}

	if ((CPU_Flags & CPU_MMX))
	{
		/* image input xxx_to_yv12 related functions */
		fastconvert_yv12_to_yv12  = fastconvert_yv12_to_yv12_mmx;
		fastconvert_bgr_to_yv12   = fastconvert_bgr_to_yv12_mmx;
		fastconvert_bgra_to_yv12  = fastconvert_bgra_to_yv12_mmx;
		fastconvert_yuyv_to_yv12  = fastconvert_yuyv_to_yv12_mmx;
		fastconvert_uyvy_to_yv12  = fastconvert_uyvy_to_yv12_mmx;

		/* image output yv12_to_xxx related functions */
		fastconvert_yv12_to_bgr   = fastconvert_yv12_to_bgr_mmx;
		fastconvert_yv12_to_bgra  = fastconvert_yv12_to_bgra_mmx;
		fastconvert_yv12_to_yuyv  = fastconvert_yv12_to_yuyv_mmx;
		fastconvert_yv12_to_uyvy  = fastconvert_yv12_to_uyvy_mmx;

		fastconvert_yv12_to_yuyvi = fastconvert_yv12_to_yuyvi_mmx;
		fastconvert_yv12_to_uyvyi = fastconvert_yv12_to_uyvyi_mmx;
	}

	/* these 3dnow functions are faster than mmx, but slower than xmm. */
	if ((CPU_Flags & CPU_3DNOW))
	{
		fastconvert_emms = fastconvert_emms_3dn;

		fastconvert_yuyv_to_yv12  = fastconvert_yuyv_to_yv12_3dn;
		fastconvert_uyvy_to_yv12  = fastconvert_uyvy_to_yv12_3dn;
	}


	if ((CPU_Flags & CPU_MMXEXT)) 
	{
		/* Colorspace transformation */
		fastconvert_yv12_to_yv12  = fastconvert_yv12_to_yv12_xmm;
		fastconvert_yuyv_to_yv12  = fastconvert_yuyv_to_yv12_xmm;
		fastconvert_uyvy_to_yv12  = fastconvert_uyvy_to_yv12_xmm;
	}

	if ((CPU_Flags & CPU_MMX))
	{
		blend_row=fastconvert_asm_blend_row_MMX;
		middle_row=fastconvert_asm_middle_row_MMX;
	} else
	{
		blend_row=fastconvert_asm_blend_row;
		middle_row=fastconvert_asm_middle_row;
	}
}

bool CBaseTransform::sigill_check(int proc_type)
{
	_try
	{
		if (proc_type==0)
			fastconvert_sse_os_trigger();
		else
			fastconvert_sse2_os_trigger();
	} _except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION)
			return true;
	}
	return false;
}

unsigned int CBaseTransform::detect_cpu_flags()
{
	unsigned int cpu_flags = CPU_ASM;

	cpu_flags |= fastconvert_check_cpu_features();
	if ((cpu_flags & CPU_SSE) && sigill_check(0))
		cpu_flags &= ~CPU_SSE;

	if ((cpu_flags & CPU_SSE2) && sigill_check(1))
		cpu_flags &= ~CPU_SSE2;

	return cpu_flags;
}

void CBaseTransform::fastconvert_RGB32CopyMem(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror)
{
	long	lHeight		= InpFrame->Height;
	BYTE*	pSrc		= (BYTE*)InpFrame->Plane[0];
	long	lSrcStride	= InpFrame->Stride[0];
	long	lWidth		= InpFrame->Width;

	BYTE*	pDst		= (BYTE*)OutFrame->Plane[0];
	long	lDstStride	= OutFrame->Stride[0];

	if (Mirror)
	{
		pDst += lDstStride*(lHeight-1);
		for (int y = 0; y<lHeight; y++, pSrc += lSrcStride, pDst -= lDstStride)
			memcpy(pDst, pSrc, 4*lWidth);
	}
	else
	{
		for (int y = 0; y<lHeight; y++, pSrc += lSrcStride, pDst += lDstStride)
			memcpy(pDst, pSrc, 4*lWidth);
	}
}

void CBaseTransform::fastconvert_RGB24CopyMem(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror)
{
	if (Mirror)
	{
		IppiSize	ROI={InpFrame->Width, InpFrame->Height};
		ippiMirror_8u_C3R((BYTE*)InpFrame->Plane[0], InpFrame->Stride[0], (BYTE*)OutFrame->Plane[0], OutFrame->Stride[0], ROI, ippAxsHorizontal);
	} else
	{
		BYTE*	src=(BYTE*)InpFrame->Plane[0];
		int		src_stride=InpFrame->Stride[0];
		BYTE*	dst=(BYTE*)OutFrame->Plane[0];
		int		y;

		for (y = 0; y<InpFrame->Height; y++)
		{
			memcpy(dst, src, 3*InpFrame->Width);
			src += src_stride;
			dst += OutFrame->Stride[0];
		}
	}
}

void CBaseTransform::fastconvert_YUY2CopyMem(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror)
{   
	IppStatus status;
	if (Mirror)
	{
		IppiSize	ROI={InpFrame->Width/2, InpFrame->Height};
		status = ippiMirror_8u_C4R((BYTE*)InpFrame->Plane[0], InpFrame->Stride[0],
			               (BYTE*)OutFrame->Plane[0], OutFrame->Stride[0], 
						    ROI, ippAxsHorizontal);
	} 
	else
	{
		BYTE*	src=(BYTE*)InpFrame->Plane[0];
		int		src_stride=InpFrame->Stride[0];
		BYTE*	dst=(BYTE*)OutFrame->Plane[0];
		int		y;

		for (y = 0; y<InpFrame->Height; y++)
		{
			memcpy(dst, src, 2*InpFrame->Width);
			src += src_stride;
			dst += OutFrame->Stride[0];
		}
	}
}