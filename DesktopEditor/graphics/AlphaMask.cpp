#include "AlphaMask.h"
#include <string.h>

#include "../agg-2.4/include/agg_rendering_buffer.h"
#include "../agg-2.4/include/agg_scanline_u.h"
#include "../agg-2.4/include/agg_alpha_mask_u8.h"

#include "Graphics.h"

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
		case EMaskDataType::Alpha4Buffer: return 4;
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

		virtual EMaskDataType GetDataType() const { return EMaskDataType::ImageBuffer; }
		virtual bool GetSwapRGB() const { return true; };

		template<class Rasterizer, class Renderer>
		void render_scanlines_2(CClipMulti& oClip, Rasterizer& ras, Renderer& ren) {}

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

	template<class Rasterizer, class Renderer, class Scanline>
	void _render_scanlines_3(CClipMulti& oClip, Rasterizer& ras, Renderer& ren, Scanline& sl)
	{
		if (!oClip.IsClip())
			agg::render_scanlines(ras, sl, ren);
		else
		{
			typedef agg::scanline_p8 sbool_scanline_type;
			sbool_scanline_type sl1;
			sbool_scanline_type sl2;

			if (!oClip.IsClip2())
				agg::sbool_combine_shapes_aa(agg::sbool_and, ras, oClip.m_rasterizer, sl1, sl2, sl, ren);
			else
				agg::sbool_combine_shapes_aa(agg::sbool_and, ras, (1 == oClip.m_lCurStorage) ? oClip.m_storage1 : oClip.m_storage2, sl1, sl2, sl, ren);
		}
	}

	class CSoftMaskBGRAgray : public CSoftMask_private
	{
	public:
		CSoftMaskBGRAgray(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
			: CSoftMask_private(pBuffer, unWidth, unHeight, bExternalBuffer, bFlip), m_oAlphaMask(m_oRenderingBuffer), m_oScanLine(m_oAlphaMask) {}

		agg::scanline_u8_am<agg::alpha_mask_bgra32gray>& GetScanline() { return m_oScanLine; }
		template<class Rasterizer, class Renderer>
		void render_scanlines_2(CClipMulti& oClip, Rasterizer& ras, Renderer& ren)
		{
			_render_scanlines_3(oClip, ras, ren, m_oScanLine);
		}

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
		template<class Rasterizer, class Renderer>
		void render_scanlines_2(CClipMulti& oClip, Rasterizer& ras, Renderer& ren)
		{
			_render_scanlines_3(oClip, ras, ren, m_oScanLine);
		}

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
		virtual EMaskDataType GetDataType() const override { return EMaskDataType::Alpha4Buffer; }
		template<class Rasterizer, class Renderer>
		void render_scanlines_2(CClipMulti& oClip, Rasterizer& ras, Renderer& ren)
		{
			_render_scanlines_3(oClip, ras, ren, m_oScanLine);
		}

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
	EMaskDataType CSoftMask::GetDataType() { return m_pInternal->GetDataType(); }

	// TODO Ошибки линковки - unresolved external symbol
	// тело шаблонных функций следует определять в заголовочных файлах...
	template<class Rasterizer, class Renderer>
	void CSoftMask::render_scanlines_2(CClipMulti& oClip, Rasterizer& ras, Renderer& ren)
	{
		m_pInternal->render_scanlines_2(oClip, ras, ren);
	}
}
