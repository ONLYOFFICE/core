#include "AlphaMask_p.h"

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
}
