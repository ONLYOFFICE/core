#ifndef _BUILD_CALPHAMASK_H_
#define _BUILD_CALPHAMASK_H_

#include "aggplustypes.h"

#include "../agg-2.4/include/agg_alpha_mask_u8.h"
#include "../agg-2.4/include/agg_pixfmt_gray.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/svg/agg_svg_rasterizer.h"

namespace Aggplus
{
	enum StatusAlphaMask
	{
		EmptyAlphaMask,
		GenerationAlphaMask,
		ApplyingAlphaMask
	};

	enum DataType
	{
		FromImage,
		FromAlphaBuffer
	} ;

	template <class PixelFormat, class AlphaMask>
	struct CAlphaMaskData
	{
		CAlphaMaskData(agg::rendering_buffer& oRenderingBuffer)
			: m_oPixfmt(oRenderingBuffer), m_oScanLine(m_oAlphaMask)
		{};

		PixelFormat                      m_oPixfmt;
		agg::renderer_base<PixelFormat>  m_oRendererBase;
		AlphaMask                        m_oAlphaMask;
		agg::scanline_u8_am<AlphaMask>   m_oScanLine; // Используется для применения альфа маски
	};

	class CAlphaMask
	{
		StatusAlphaMask                 m_enStatus;
		bool                            m_bExternalBuffer;

		agg::rendering_buffer           m_oRenderingBuffer;

		typedef CAlphaMaskData<agg::pixfmt_bgra32, agg::alpha_mask_bgra32gray> AMaskFromImage;
		typedef CAlphaMaskData<agg::pixfmt_gray8,  agg::alpha_mask_gray8>      AMaskFromABuffer;

		AMaskFromImage   *m_pImageData;
		AMaskFromABuffer *m_pAlphaBufferData;

		DataType m_enDataType;

		void Set(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, DataType enDataType);

	public:
		CAlphaMask();
		~CAlphaMask();

		StatusAlphaMask GetStatus() const;
		DataType        GetDataType() const;
		void            Clear();

		Status Create(unsigned int unWidth, unsigned int unHeight, DataType enDataType = FromImage);
		Status LoadAlphaChennel(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer = true);
		Status LoadFromImageBuffer(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer = true);

		agg::rendering_buffer& GetRenderingBuffer();

		agg::renderer_base<agg::pixfmt_bgra32>& GetRendererBaseImage();

		agg::scanline_u8_am<agg::alpha_mask_bgra32gray>& GetScanlineImage();
		agg::scanline_u8_am<agg::alpha_mask_gray8>&      GetScanlineABuffer();

		void StartApplying();
	};
}

#endif // _BUILD_CALPHAMASK_H_
