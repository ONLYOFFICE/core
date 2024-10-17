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



	CSoftMask::CSoftMask() : m_unWidth(0), m_unHeight(0), m_pImageData(NULL), m_pAlphaBufferData(NULL) {}
	CSoftMask::CSoftMask(BYTE* pBuffer, UINT unWidth, UINT unHeight, EMaskDataType enDataType, bool bExternalBuffer, bool bFlip) : m_pImageData(NULL), m_pAlphaBufferData(NULL)
	{
		LoadFromBuffer(pBuffer, unWidth, unHeight, enDataType, bExternalBuffer, bFlip);
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

		RELEASEOBJECT(m_pImageData);
		RELEASEOBJECT(m_pAlphaBufferData);
	}

	EMaskDataType CSoftMask::GetDataType() const { return m_enDataType; }
	unsigned int CSoftMask::GetStep() const
	{
		switch(m_enDataType)
		{
		case EMaskDataType::ImageBuffer: return 4;
		case EMaskDataType::AlphaBuffer: return 1;
		case EMaskDataType::Alpha4Buffer: return 4;
		}
	}
	unsigned int CSoftMask::GetWidth() const { return m_unWidth; }
	unsigned int CSoftMask::GetHeight() const { return m_unHeight; }

	void CSoftMask::SetType(EMaskDataType enDataType)
	{
		m_enDataType = enDataType;

		RELEASEOBJECT(m_pImageData);
		RELEASEOBJECT(m_pAlphaBufferData);

		switch (enDataType)
		{
		case EMaskDataType::ImageBuffer:
		{
			m_pImageData = new AMaskFromImage(m_oRenderingBuffer);
			m_pImageData->m_oRendererBase.attach(m_pImageData->m_oPixfmt);
			m_pImageData->m_oAlphaMask.attach(m_oRenderingBuffer);
			break;
		}
		case EMaskDataType::Alpha4Buffer:
		{
			m_pAlphaBufferData = new AMaskFromABuffer(m_oRenderingBuffer);
			m_pAlphaBufferData->m_oRendererBase.attach(m_pAlphaBufferData->m_oPixfmt);
			m_pAlphaBufferData->m_oAlphaMask.attach(m_oRenderingBuffer);
			break;
		}
		}
	}

	Status CSoftMask::Create(UINT unWidth, UINT unHeight, EMaskDataType enDataType)
	{
		if (0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		m_bExternalBuffer = false;

		UINT unSize = unWidth * unHeight * GetStep();
		BYTE* pAlphaBufffer = new BYTE[unSize];
		if (!pAlphaBufffer)
			return OutOfMemory;

		memset(pAlphaBufffer, 0x00, unSize);

		Set(pAlphaBufffer, unWidth, unHeight, enDataType);

		return Ok;
	}

	Status CSoftMask::LoadFromBuffer(BYTE *pBuffer, UINT unWidth, UINT unHeight, EMaskDataType enDataType, bool bExternalBuffer, bool bFlip)
	{
		if (NULL == pBuffer || 0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		m_bExternalBuffer = bExternalBuffer;

		Set(pBuffer, unWidth, unHeight, enDataType, bFlip);

		return Ok;
	}

	RenBaseBGRA32& CSoftMask::GetRendererBaseImage()
	{
		return m_pImageData->m_oRendererBase;
	}

	ScanlineBGRA32Gray& CSoftMask::GetScanlineImage()
	{
		return m_pImageData->m_oScanLine;
	}

	ScanlineBGRA32A& CSoftMask::GetScanlineABuffer()
	{
		return m_pAlphaBufferData->m_oScanLine;
	}

	BYTE* CSoftMask::GetBuffer()
	{
		return m_oRenderingBuffer.buf();
	}

	agg::rendering_buffer& CSoftMask::GetRenderingBuffer()
	{
		return m_oRenderingBuffer;
	}

	void CSoftMask::Set(BYTE* pBuffer, UINT unWidth, UINT unHeight, EMaskDataType enDataType, bool bFlip)
	{
		m_enDataType = enDataType;
		m_unWidth    = unWidth;
		m_unHeight   = unHeight;
		m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, (bFlip ? -1 : 1) * GetStep() * unWidth);

		SetType(enDataType);
	}
}
