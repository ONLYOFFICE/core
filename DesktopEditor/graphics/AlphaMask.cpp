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

	CSoftMask::CSoftMask(unsigned int unWidth, unsigned int unHeight) : m_unWidth(unWidth), m_unHeight(unHeight) {}
	unsigned int CSoftMask::GetStep() const { return 4; }
	unsigned int CSoftMask::GetWidth() const { return m_unWidth; }
	unsigned int CSoftMask::GetHeight() const { return m_unHeight; }

	template<> EMaskDataType _CSoftMask<agg::pixfmt_rgba32, agg::alpha_mask_rgba32a>::GetDataType() const { return EMaskDataType::Alpha4Buffer; }
	template<> EMaskDataType _CSoftMask<agg::pixfmt_rgba32, agg::alpha_mask_rgba32gray>::GetDataType() const { return EMaskDataType::ImageBuffer; }
	template<> EMaskDataType _CSoftMask<agg::pixfmt_bgra32, agg::alpha_mask_bgra32a>::GetDataType() const { return EMaskDataType::Alpha4Buffer; }
	template<> EMaskDataType _CSoftMask<agg::pixfmt_bgra32, agg::alpha_mask_bgra32gray>::GetDataType() const { return EMaskDataType::ImageBuffer; }
}
