#ifndef _BUILD_ALPHAMASK_H_
#define _BUILD_ALPHAMASK_H_

#include <string>
#include "aggplustypes.h"
#include "../common/IGrObject.h"
#include "./config.h"

#include "../agg-2.4/include/agg_rendering_buffer.h"
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

	class GRAPHICS_DECL CSoftMask : public IGrObject
	{
	public:
		virtual ~CSoftMask();

		unsigned int GetStep() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;
		BYTE* GetBuffer();

		virtual EMaskDataType GetDataType() const = 0;

	protected:
		CSoftMask(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip);

		agg::rendering_buffer m_oRenderingBuffer;
		bool                  m_bExternalBuffer;
		unsigned int          m_unWidth;
		unsigned int          m_unHeight;
	};

	class GRAPHICS_DECL CSoftMaskBGRAgray : public CSoftMask
	{
	public:
		CSoftMaskBGRAgray(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_bgra32gray>& GetScanline() { return m_oScanLine; }
		virtual EMaskDataType GetDataType() const override { return EMaskDataType::ImageBuffer; }

	private:
		agg::alpha_mask_bgra32gray                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_bgra32gray> m_oScanLine;
	};

	class GRAPHICS_DECL CSoftMaskRGBAgray : public CSoftMask
	{
	public:
		CSoftMaskRGBAgray(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_rgba32gray>& GetScanline() { return m_oScanLine; }
		virtual EMaskDataType GetDataType() const override { return EMaskDataType::ImageBuffer; }

	private:
		agg::alpha_mask_rgba32gray                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_rgba32gray> m_oScanLine;
	};

	class GRAPHICS_DECL CSoftMaskAlpha : public CSoftMask
	{
	public:
		CSoftMaskAlpha(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_rgba32a>& GetScanline() { return m_oScanLine; }
		virtual EMaskDataType GetDataType() const override { return EMaskDataType::Alpha4Buffer; }

	private:
		agg::alpha_mask_rgba32a                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_rgba32a> m_oScanLine;
	};
}

#endif // _BUILD_ALPHAMASK_H_
