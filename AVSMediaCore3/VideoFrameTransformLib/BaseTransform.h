#pragma once

#include <ippcore.h>
#include <ippdefs.h>
#include <ippi.h>

#pragma comment(lib, "ippcorel.lib")
#pragma comment(lib, "ippsemerged.lib")
#pragma comment(lib, "ippsmerged.lib")
#pragma comment(lib, "ippiemerged.lib")
#pragma comment(lib, "ippimerged.lib")

#include "windows.h"
#include "Common.h"
#include "DefTypes.h"

class CBaseTransform
{
private:
	unsigned int detect_cpu_flags();
	bool sigill_check(int proc_type);
protected:
	/* input */
	fastconvert_packedFuncPtr fastconvert_rgb555_to_yv12;
	fastconvert_packedFuncPtr fastconvert_rgb565_to_yv12;
	fastconvert_packedFuncPtr fastconvert_bgr_to_yv12;
	fastconvert_packedFuncPtr fastconvert_bgra_to_yv12;
	fastconvert_packedFuncPtr fastconvert_abgr_to_yv12;
	fastconvert_packedFuncPtr fastconvert_rgba_to_yv12;
	fastconvert_packedFuncPtr fastconvert_argb_to_yv12;
	fastconvert_packedFuncPtr fastconvert_yuv_to_yv12;
	fastconvert_packedFuncPtr fastconvert_yuyv_to_yv12;
	fastconvert_packedFuncPtr fastconvert_uyvy_to_yv12;

	fastconvert_packedFuncPtr fastconvert_rgb555i_to_yv12;
	fastconvert_packedFuncPtr fastconvert_rgb565i_to_yv12;
	fastconvert_packedFuncPtr fastconvert_bgri_to_yv12;
	fastconvert_packedFuncPtr fastconvert_bgrai_to_yv12;
	fastconvert_packedFuncPtr fastconvert_abgri_to_yv12;
	fastconvert_packedFuncPtr fastconvert_rgbai_to_yv12;
	fastconvert_packedFuncPtr fastconvert_argbi_to_yv12;
	fastconvert_packedFuncPtr fastconvert_yuyvi_to_yv12;
	fastconvert_packedFuncPtr fastconvert_uyvyi_to_yv12;

	/* output */
	fastconvert_packedFuncPtr fastconvert_yv12_to_rgb555;
	fastconvert_packedFuncPtr fastconvert_yv12_to_rgb565;
	fastconvert_packedFuncPtr fastconvert_yv12_to_bgr;
	fastconvert_packedFuncPtr fastconvert_yv12_to_bgra;
	fastconvert_packedFuncPtr fastconvert_yv12_to_abgr;
	fastconvert_packedFuncPtr fastconvert_yv12_to_rgba;
	fastconvert_packedFuncPtr fastconvert_yv12_to_argb;
	fastconvert_packedFuncPtr fastconvert_yv12_to_yuv;
	fastconvert_packedFuncPtr fastconvert_yv12_to_yuyv;
	fastconvert_packedFuncPtr fastconvert_yv12_to_uyvy;

	fastconvert_packedFuncPtr fastconvert_yv12_to_rgb555i;
	fastconvert_packedFuncPtr fastconvert_yv12_to_rgb565i;
	fastconvert_packedFuncPtr fastconvert_yv12_to_bgri;
	fastconvert_packedFuncPtr fastconvert_yv12_to_bgrai;
	fastconvert_packedFuncPtr fastconvert_yv12_to_abgri;
	fastconvert_packedFuncPtr fastconvert_yv12_to_rgbai;
	fastconvert_packedFuncPtr fastconvert_yv12_to_argbi;
	fastconvert_packedFuncPtr fastconvert_yv12_to_yuyvi;
	fastconvert_packedFuncPtr fastconvert_yv12_to_uyvyi;

	fastconvert_planarFuncPtr fastconvert_yv12_to_yv12;

	fastconvert_emmsFuncPtr fastconvert_emms;

	fastconvert_FuncPtr	blend_row;
	fastconvert_FuncPtr	middle_row;

	void fastconvert_RGB32CopyMem(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror);
	void fastconvert_RGB24CopyMem(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror);
        void fastconvert_YUY2CopyMem(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame, bool Mirror);

	void fastconvert_RGB32BlendDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void fastconvert_RGB24BlendDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void fastconvert_YV12BlendDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void fastconvert_YUY2BlendDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);

	void fastconvert_RGB32MiddleDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void fastconvert_RGB24MiddleDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void fastconvert_YV12MiddleDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);
	void fastconvert_YUY2MiddleDeinterlace(SUncompressedVideoFrame* InpFrame, SUncompressedVideoFrame* OutFrame);

	unsigned int CPU_Flags;					//Тип процессора (0 - автоопределение)		(по умолчанию 0)
public:
	unsigned int __fastcall GetCPU(){return CPU_Flags;}
	void __fastcall SetCPU(unsigned int CPU);

	__declspec(property (get = GetCPU, put=SetCPU))	unsigned int	m_CPU_Flags;

	CBaseTransform(void);
	virtual ~CBaseTransform(void);
};
