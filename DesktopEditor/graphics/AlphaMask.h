#ifndef _BUILD_ALPHAMASK_H_
#define _BUILD_ALPHAMASK_H_

#include <string>
#include "aggplustypes.h"
#include "../common/IGrObject.h"
#include "./config.h"

#include "../agg-2.4/include/agg_renderer_base.h"
#include "../agg-2.4/include/agg_pixfmt_rgba.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/include/agg_alpha_mask_u8.h"

namespace Aggplus
{
	enum class EMaskDataType
	{
		ImageBuffer,
		AlphaBuffer,
		Alpha4Buffer
	};

	class GRAPHICS_DECL CAlphaMask : public IGrObject
	{
	public:
		CAlphaMask();
		CAlphaMask(BYTE* pBuffer, EMaskDataType eDataType, bool bExternalBuffer = true);
		virtual ~CAlphaMask();

		BYTE*         GetBuffer();
		EMaskDataType GetDataType() const;
		UINT          GetStep() const;

		Status Create(UINT unWidth, UINT unHeight, EMaskDataType eDataType);
		Status LoadFromBuffer(BYTE* pBuffer, EMaskDataType eDataType, bool bExternalBuffer = true);
	private:
		BYTE         *m_pBuffer;
		EMaskDataType m_enDataType;
		bool          m_bExternalBuffer;
	};

	template <class PixelFormat, class AlphaMask>
	struct TAlphaMaskData
	{
		TAlphaMaskData(agg::rendering_buffer& oRenderingBuffer) : m_oPixfmt(oRenderingBuffer), m_oScanLine(m_oAlphaMask) {};

		PixelFormat                     m_oPixfmt;
		agg::renderer_base<PixelFormat> m_oRendererBase;
		AlphaMask                       m_oAlphaMask;
		agg::scanline_u8_am<AlphaMask>  m_oScanLine;
	};

	typedef agg::renderer_base<agg::pixfmt_bgra32>          RenBaseBGRA32;
	typedef agg::scanline_u8_am<agg::alpha_mask_bgra32gray> ScanlineBGRA32Gray;
	typedef agg::scanline_u8_am<agg::alpha_mask_bgra32a>    ScanlineBGRA32A;

	class GRAPHICS_DECL CSoftMask : public IGrObject
	{
	public:
		CSoftMask();
		CSoftMask(BYTE* pBuffer, UINT unWidth, UINT unHeight, EMaskDataType enDataType, bool bExternalBuffer = true, bool bFlip = false);
		virtual ~CSoftMask();

		EMaskDataType GetDataType() const;
		unsigned int GetStep() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		void SetType(EMaskDataType enDataType);
		Status Create(UINT unWidth, UINT unHeight, EMaskDataType enDataType);
		Status LoadFromBuffer(BYTE* pBuffer, UINT unWidth, UINT unHeight, EMaskDataType enDataType, bool bExternalBuffer = true, bool bFlip = false);

		agg::rendering_buffer& GetRenderingBuffer();
		RenBaseBGRA32&         GetRendererBaseImage();
		ScanlineBGRA32Gray&    GetScanlineImage();
		ScanlineBGRA32A&       GetScanlineABuffer();
		BYTE*                  GetBuffer();
	private:
		void Set(BYTE* pBuffer, UINT unWidth, UINT unHeight, EMaskDataType enDataType, bool bFlip = false);

		agg::rendering_buffer m_oRenderingBuffer;
		EMaskDataType         m_enDataType;
		bool                  m_bExternalBuffer;
		unsigned int          m_unWidth;
		unsigned int          m_unHeight;

		typedef TAlphaMaskData<agg::pixfmt_bgra32, agg::alpha_mask_bgra32gray> AMaskFromImage;
		typedef TAlphaMaskData<agg::pixfmt_bgra32, agg::alpha_mask_bgra32a>    AMaskFromABuffer;

		AMaskFromImage*   m_pImageData;
		AMaskFromABuffer* m_pAlphaBufferData;
	};
}

#endif // _BUILD_ALPHAMASK_H_
