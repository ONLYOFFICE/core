#include "AlphaMask.h"
#include <string.h>

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

	CSoftMask::CSoftMask(BYTE* pBuffer, unsigned int unWidth, unsigned int unHeight, bool bExternalBuffer, bool bFlip)
	{
		m_bExternalBuffer = bExternalBuffer;
		m_unWidth         = unWidth;
		m_unHeight        = unHeight;
		m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, (bFlip ? -1 : 1) * GetStep() * unWidth);
	}
	CSoftMask::~CSoftMask()
	{
		BYTE* pBuffer = m_oRenderingBuffer.buf();
		if (NULL != pBuffer)
		{
			if (!m_bExternalBuffer)
				RELEASEARRAYOBJECTS(pBuffer);

			m_oRenderingBuffer.attach(NULL, 0, 0, 0);
		}
	}
	unsigned int CSoftMask::GetStep() const { return 4; }
	unsigned int CSoftMask::GetWidth() const { return m_unWidth; }
	unsigned int CSoftMask::GetHeight() const { return m_unHeight; }
	BYTE* CSoftMask::GetBuffer() { return m_oRenderingBuffer.buf(); }
}
