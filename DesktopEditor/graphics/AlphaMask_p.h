#ifndef _BUILD_ALPHAMASK_P_H_
#define _BUILD_ALPHAMASK_P_H_

#include "AlphaMask.h"
#include <string>
#include <string.h>

#include "../agg-2.4/include/agg_rendering_buffer.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/include/agg_alpha_mask_u8.h"

namespace Aggplus
{
	class CSoftMask_private
	{
	public:
		virtual ~CSoftMask_private()
		{
			BYTE* pBuffer = m_oRenderingBuffer.buf();
			if (NULL != pBuffer)
			{
				if (!m_bExternalBuffer)
					RELEASEARRAYOBJECTS(pBuffer);

				m_oRenderingBuffer.attach(NULL, 0, 0, 0);
			}
		}

		unsigned int GetStep() const { return 4; }
		unsigned int GetWidth() const { return m_unWidth; }
		unsigned int GetHeight() const { return m_unHeight; }
		BYTE* GetBuffer() { return m_oRenderingBuffer.buf(); }

		virtual ESoftMaskType GetDataType() const = 0;
		virtual bool GetSwapRGB() const { return true; };

	protected:
		CSoftMask_private(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
		{
			m_bExternalBuffer = bExternalBuffer;
			m_unWidth         = unWidth;
			m_unHeight        = unHeight;
			m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, (bFlip ? -1 : 1) * GetStep() * unWidth);
		}

		agg::rendering_buffer m_oRenderingBuffer;
		bool                  m_bExternalBuffer;
		unsigned int          m_unWidth;
		unsigned int          m_unHeight;
	};

	class CSoftMaskBGRAgray : public CSoftMask_private
	{
	public:
		CSoftMaskBGRAgray(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask_private(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_bgra32gray>& GetScanline() { return m_oScanLine; }
		virtual ESoftMaskType GetDataType() const override { return ESoftMaskType::BGRGrayBuffer; }

	private:
		agg::alpha_mask_bgra32gray                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_bgra32gray> m_oScanLine;
	};

	class CSoftMaskRGBAgray : public CSoftMask_private
	{
	public:
		CSoftMaskRGBAgray(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask_private(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_rgba32gray>& GetScanline() { return m_oScanLine; }
		virtual bool GetSwapRGB() const override { return false; };
		virtual ESoftMaskType GetDataType() const override { return ESoftMaskType::RGBGrayBuffer; }

	private:
		agg::alpha_mask_rgba32gray                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_rgba32gray> m_oScanLine;
	};

	class CSoftMaskAlpha : public CSoftMask_private
	{
	public:
		CSoftMaskAlpha(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask_private(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_rgba32a>& GetScanline() { return m_oScanLine; }
		virtual ESoftMaskType GetDataType() const override { return ESoftMaskType::Alpha4Buffer; }

	private:
		agg::alpha_mask_rgba32a                      m_oAlphaMask;
		agg::scanline_u8_am<agg::alpha_mask_rgba32a> m_oScanLine;
	};
}

#endif // _BUILD_ALPHAMASK_P_H_
