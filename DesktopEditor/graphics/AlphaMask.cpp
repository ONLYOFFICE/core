#include "AlphaMask.h"
#include <string.h>

#include "../agg-2.4/include/agg_rendering_buffer.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/include/agg_alpha_mask_u8.h"

namespace Aggplus
{
	CAlphaMask::CAlphaMask()
		: m_pBuffer(NULL)
	{}

	CAlphaMask::CAlphaMask(BYTE *pBuffer, EMaskDataType eDataType, bool bExternalBuffer)
		: m_pBuffer(pBuffer), m_enDataType(eDataType), m_bExternalBuffer(bExternalBuffer)
	{}

	CAlphaMask::~CAlphaMask()
	{
		if (!m_bExternalBuffer)
			RELEASEARRAYOBJECTS(m_pBuffer);
	}

	BYTE *CAlphaMask::GetBuffer()
	{
		return m_pBuffer;
	}

	EMaskDataType CAlphaMask::GetDataType() const
	{
		return m_enDataType;
	}

	UINT CAlphaMask::GetStep() const
	{
		switch(m_enDataType)
		{
		case EMaskDataType::ImageBuffer: return 4;
		case EMaskDataType::AlphaBuffer: return 1;
		}
	}

	Status CAlphaMask::Create(UINT unWidth, UINT unHeight, EMaskDataType eDataType)
	{
		if (0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		m_enDataType      = eDataType;
		m_bExternalBuffer = false;

		UINT unSize = unWidth * unHeight * GetStep();
		
		m_pBuffer = new BYTE[unSize];

		if (NULL == m_pBuffer)
			return OutOfMemory;

		memset(m_pBuffer, 0x00, unSize);

		return Ok;
	}

	Status CAlphaMask::LoadFromBuffer(BYTE *pBuffer, EMaskDataType eDataType, bool bExternalBuffer)
	{
		m_pBuffer = pBuffer;
		m_enDataType = eDataType;
		m_bExternalBuffer = bExternalBuffer;
		return Ok;
	}


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

	CSoftMask::CSoftMask(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bFlip, bool bRGB, bool bAlpha)
	{
		if (bAlpha)
			m_pInternal = new CSoftMaskAlpha(pBuffer, unWidth, unHeight, false, bFlip);
		else
		{
			if (bRGB)
				m_pInternal = new CSoftMaskBGRAgray(pBuffer, unWidth, unHeight, false, bFlip);
			else
				m_pInternal = new CSoftMaskRGBAgray(pBuffer, unWidth, unHeight, false, bFlip);
		}
	}
	CSoftMask::~CSoftMask()
	{
		RELEASEOBJECT(m_pInternal);
	}

	unsigned int CSoftMask::GetStep() const { return m_pInternal->GetStep(); }
	unsigned int CSoftMask::GetWidth() const { return m_pInternal->GetWidth(); }
	unsigned int CSoftMask::GetHeight() const { return m_pInternal->GetHeight(); }
	BYTE* CSoftMask::GetBuffer() { return m_pInternal->GetBuffer(); }
	ESoftMaskType CSoftMask::GetDataType() { return m_pInternal->GetDataType(); }

	agg::scanline_u8_am<agg::alpha_mask_bgra32gray>& CSoftMask::GetScanlineBGRGray()
	{
		return ((CSoftMaskBGRAgray*)m_pInternal)->GetScanline();
	}
	agg::scanline_u8_am<agg::alpha_mask_rgba32gray>& CSoftMask::GetScanlineRGBGray()
	{
		return ((CSoftMaskRGBAgray*)m_pInternal)->GetScanline();
	}
	agg::scanline_u8_am<agg::alpha_mask_rgba32a>& CSoftMask::GetScanlineAlpha4()
	{
		return ((CSoftMaskAlpha*)m_pInternal)->GetScanline();
	}
}
