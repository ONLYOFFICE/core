#include "CAlphaMask.h"

namespace Aggplus
{
	CAlphaMask::CAlphaMask()
		: m_enStatus(EmptyAlphaMask), m_pImageData(NULL), m_pAlphaBufferData(NULL)
	{}

	CAlphaMask::~CAlphaMask()
	{
		Clear();
	}

	StatusAlphaMask CAlphaMask::GetStatus() const
	{
		return m_enStatus;
	}

	DataType CAlphaMask::GetDataType() const
	{
		return m_enDataType;
	}

	void CAlphaMask::Clear()
	{		
		if (NULL != m_oRenderingBuffer.buf())
		{
			if (!m_bExternalBuffer)
				delete [] m_oRenderingBuffer.buf();
			m_oRenderingBuffer.attach(NULL, 0, 0, 0);
		}

		RELEASEOBJECT(m_pImageData)
		RELEASEOBJECT(m_pAlphaBufferData)

		m_enStatus = EmptyAlphaMask;
	}

	void CAlphaMask::Set(BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight, DataType enDataType)
	{
		Clear();

		m_enDataType = enDataType;

		switch (enDataType)
		{
			case FromImage:
			{
				m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, -4 * unWidth);

				m_pImageData = new AMaskFromImage(m_oRenderingBuffer);

				m_pImageData->m_oPixfmt.attach(m_oRenderingBuffer);
				m_pImageData->m_oRendererBase.attach(m_pImageData->m_oPixfmt);
				m_pImageData->m_oAlphaMask.attach(m_oRenderingBuffer);

				break;
			}
			case FromAlphaBuffer:
			{
				m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, -unWidth);

				m_pAlphaBufferData = new AMaskFromABuffer(m_oRenderingBuffer);

				m_pAlphaBufferData->m_oPixfmt.attach(m_oRenderingBuffer);
				m_pAlphaBufferData->m_oRendererBase.attach(m_pAlphaBufferData->m_oPixfmt);
				m_pAlphaBufferData->m_oAlphaMask.attach(m_oRenderingBuffer);

				break;
			}
		}
	}

	Status CAlphaMask::Create(unsigned int unWidth, unsigned int unHeight, DataType enDataType)
	{
		if (0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		agg::int8u* pAlpha_buf = new agg::int8u[unWidth * unHeight * ((enDataType == FromImage) ? 4 : 1)];

		if (NULL == pAlpha_buf)
			return OutOfMemory;

		Set(pAlpha_buf, unWidth, unHeight, enDataType);

		if (enDataType == FromImage)
			m_pImageData->m_oRendererBase.clear(agg::rgba8(0, 0, 0));
		else if (enDataType == FromAlphaBuffer)
			m_pAlphaBufferData->m_oRendererBase.clear(agg::gray8(0));

		m_enStatus        = GenerationAlphaMask;
		m_bExternalBuffer = false;

		return Ok;
	}

	Status CAlphaMask::LoadAlphaChennel(BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer)
	{
		if (NULL == pBuffer || 0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		Set(pBuffer, unWidth, unHeight, FromAlphaBuffer);

		m_enStatus        = ApplyingAlphaMask;
		m_bExternalBuffer = bExternalBuffer;

		return Ok;
	}

	Status CAlphaMask::LoadFromImageBuffer(BYTE *pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer)
	{
		if (NULL == pBuffer || 0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		Set(pBuffer, unWidth, unHeight, FromImage);

		m_enStatus        = ApplyingAlphaMask;
		m_bExternalBuffer = bExternalBuffer;

		return Ok;
	}

	agg::rendering_buffer &CAlphaMask::GetRenderingBuffer()
	{
		return m_oRenderingBuffer;
	}

	agg::renderer_base<agg::pixfmt_bgra32> &CAlphaMask::GetRendererBaseImage()
	{
		return m_pImageData->m_oRendererBase;
	}

	agg::scanline_u8_am<agg::alpha_mask_bgra32gray> &CAlphaMask::GetScanlineImage()
	{
		return m_pImageData->m_oScanLine;
	}

	agg::scanline_u8_am<agg::alpha_mask_gray8> &CAlphaMask::GetScanlineABuffer()
	{
		return m_pAlphaBufferData->m_oScanLine;
	}

	void CAlphaMask::StartApplying()
	{
		m_enStatus = ApplyingAlphaMask;
	}
}
