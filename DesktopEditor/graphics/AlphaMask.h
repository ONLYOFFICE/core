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

	class GRAPHICS_DECL CSoftMask : public IGrObject
	{
	public:
		unsigned int GetStep() const;
		unsigned int GetWidth() const;
		unsigned int GetHeight() const;

		virtual EMaskDataType GetDataType() const = 0;
		virtual BYTE* GetBuffer() = 0;

	protected:
		CSoftMask(unsigned int unWidth, unsigned int unHeight);

		unsigned int m_unWidth;
		unsigned int m_unHeight;
	};

	template <class PixelFormat, class AlphaMask>
	class GRAPHICS_DECL _CSoftMask : public CSoftMask
	{
	public:
		_CSoftMask(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer = true, bool bFlip = false) : CSoftMask(unWidth, unHeight), m_oScanLine(m_oAlphaMask)
		{
			m_bExternalBuffer = bExternalBuffer;
			m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, (bFlip ? -1 : 1) * GetStep() * unWidth);
			m_oPixfmt.attach(m_oRenderingBuffer);
			m_oRendererBase.attach(m_oPixfmt);
			m_oAlphaMask.attach(m_oRenderingBuffer);
		}
		virtual ~_CSoftMask()
		{
			BYTE* pBuffer = m_oRenderingBuffer.buf();
			if (NULL != pBuffer)
			{
				if (!m_bExternalBuffer)
					RELEASEARRAYOBJECTS(pBuffer);

				m_oRenderingBuffer.attach(NULL, 0, 0, 0);
			}
		}

		agg::scanline_u8_am<AlphaMask>& GetScanline() { return m_oScanLine; }
		virtual EMaskDataType GetDataType() const override;
		virtual BYTE* GetBuffer() override { return m_oRenderingBuffer.buf(); }

	private:
		bool                            m_bExternalBuffer;

		agg::rendering_buffer           m_oRenderingBuffer;
		PixelFormat                     m_oPixfmt;
		agg::renderer_base<PixelFormat> m_oRendererBase;
		AlphaMask                       m_oAlphaMask;
		agg::scanline_u8_am<AlphaMask>  m_oScanLine;
	};
}

#endif // _BUILD_ALPHAMASK_H_
