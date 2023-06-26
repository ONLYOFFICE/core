#ifndef _BUILD_ALPHAMASK_PRIVATE_H_
#define _BUILD_ALPHAMASK_PRIVATE_H_

#include <string>
#include "aggplustypes.h"
#include "../common/IGrObject.h"

#include "../agg-2.4/include/agg_alpha_mask_u8.h"
#include "../agg-2.4/include/agg_renderer_base.h"
#include "../agg-2.4/include/agg_pixfmt_gray.h"
#include "../agg-2.4/include/agg_pixfmt_rgba.h"
#include "../agg-2.4/include/agg_scanline_u.h"

namespace Aggplus
{
	enum StatusAlphaMask
	{
		EmptyAlphaMask,
		GenerationAlphaMask,
		ApplyingAlphaMask
	};

	enum AMaskDataType
	{
		ImageBuffer,
		AlphaBuffer
	} ;

	template <class PixelFormat, class AlphaMask>
	struct TAlphaMaskData
	{
		TAlphaMaskData(agg::rendering_buffer& oRenderingBuffer)
			: m_oPixfmt(oRenderingBuffer), m_oScanLine(m_oAlphaMask)
		{};

		PixelFormat                      m_oPixfmt;
		agg::renderer_base<PixelFormat>  m_oRendererBase;
		AlphaMask                        m_oAlphaMask;
		agg::scanline_u8_am<AlphaMask>   m_oScanLine; // Используется для применения альфа маски
	};

	typedef agg::renderer_base<agg::pixfmt_bgra32>          RenBaseBGRA32;
	typedef agg::scanline_u8_am<agg::alpha_mask_bgra32gray> ScanlineBGRA32Gray;
	typedef agg::scanline_u8_am<agg::alpha_mask_gray8>      ScanlineGray8;

	class CAlphaMask_private
	{
	public:
		CAlphaMask_private();
		~CAlphaMask_private();

		StatusAlphaMask GetStatus()   const;
		AMaskDataType   GetDataType() const;

		void Clear();

		Status Create(UINT unWidth, UINT unHeight, AMaskDataType enDataType);

		Status LoadFromBuffer(BYTE* pBuffer, UINT unWidth, UINT unHeight, AMaskDataType enBufferType, bool bExternalBuffer = true);

		Status LoadFromFile(const std::wstring& wsFilePath);
		Status LoadFromImage(IGrObject* pGrObject, bool bCopy = true);

		agg::rendering_buffer& GetRenderingBuffer();
		RenBaseBGRA32&         GetRendererBaseImage();
		ScanlineBGRA32Gray&    GetScanlineImage();
		ScanlineGray8&         GetScanlineABuffer();

		BYTE* GetMask();

		void StartApplying();

		CAlphaMask_private& operator=(const CAlphaMask_private& oAlphaMask);
	private:
		void Set(BYTE* pBuffer, UINT unWidth, UINT unHeight, AMaskDataType enDataType);

		agg::rendering_buffer           m_oRenderingBuffer;
		StatusAlphaMask                 m_enStatus;
		AMaskDataType                   m_enDataType;

		bool                            m_bExternalBuffer;
	public:
		typedef TAlphaMaskData<agg::pixfmt_bgra32, agg::alpha_mask_bgra32gray> AMaskFromImage;
		typedef TAlphaMaskData<agg::pixfmt_gray8,  agg::alpha_mask_gray8>      AMaskFromABuffer;

		AMaskFromImage   *m_pImageData;
		AMaskFromABuffer *m_pAlphaBufferData;
	};
}

#endif // _BUILD_ALPHAMASK_PRIVATE_H_
