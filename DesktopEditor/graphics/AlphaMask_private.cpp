#include "AlphaMask_private.h"
#include "../raster/BgraFrame.h"

namespace Aggplus
{
	CAlphaMask_private::CAlphaMask_private()
		: m_enStatus(EmptyAlphaMask), m_pImageData(NULL), m_pAlphaBufferData(NULL)
	{}

	CAlphaMask_private::~CAlphaMask_private()
	{
		Clear();
	}

	StatusAlphaMask CAlphaMask_private::GetStatus() const
	{
		return m_enStatus;
	}

	AMaskDataType CAlphaMask_private::GetDataType() const
	{
		return m_enDataType;
	}

	void CAlphaMask_private::Clear()
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

	Status CAlphaMask_private::Create(UINT unWidth, UINT unHeight, AMaskDataType enDataType)
	{
		if (0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		BYTE* pAlphaBufffer = new BYTE[unWidth * unHeight * ((enDataType == ImageBuffer) ? 4 : 1)];

		if (NULL == pAlphaBufffer)
			return OutOfMemory;

		Set(pAlphaBufffer, unWidth, unHeight, enDataType);

		if (enDataType == ImageBuffer)
			m_pImageData->m_oRendererBase.clear(agg::rgba8(0, 0, 0));
		else if (enDataType == AlphaBuffer)
			m_pAlphaBufferData->m_oRendererBase.clear(agg::gray8(0));

		m_enStatus        = GenerationAlphaMask;
		m_bExternalBuffer = false;

		return Ok;
	}

	Status CAlphaMask_private::LoadFromBuffer(BYTE *pBuffer, UINT unWidth, UINT unHeight, AMaskDataType enBufferType, bool bExternalBuffer)
	{
		if (NULL == pBuffer || 0 == unWidth || 0 == unHeight)
			return InvalidParameter;

		Set(pBuffer, unWidth, unHeight, enBufferType);

		m_enStatus        = ApplyingAlphaMask;
		m_bExternalBuffer = bExternalBuffer;

		return Ok;
	}

	Status CAlphaMask_private::LoadFromFile(const std::wstring &wsFilePath)
	{
		if (wsFilePath.empty())
			return InvalidParameter;

		CBgraFrame oFrame;

		if (!oFrame.OpenFile(wsFilePath))
			return GenericError;

		UINT unWidth  = oFrame.get_Width();
		UINT unHeight = oFrame.get_Height();

		Set(oFrame.get_Data(), unWidth, unHeight, ImageBuffer);

		oFrame.put_Data(NULL);

		m_enStatus        = ApplyingAlphaMask;
		m_bExternalBuffer = false;

		return Ok;
	}

	Status CAlphaMask_private::LoadFromImage(IGrObject *pGrObject, bool bCopy)
	{
		if (NULL == pGrObject)
			return InvalidParameter;

		CImage *pImage = (CImage*)pGrObject;

		if (Ok != pImage->GetLastStatus())
			return GenericError;

		UINT unWidth  = pImage->GetWidth();
		UINT unHeight = pImage->GetHeight();

		BYTE *pBuffer = NULL;

		if (bCopy)
		{
			const UINT unSize = 4 * unWidth * unWidth;

			pBuffer = new BYTE[unSize];
			memcpy(pBuffer, pImage->GetData(), unSize);
		}
		else
			pBuffer = pImage->GetData();

		Set(pBuffer, unWidth, unHeight, ImageBuffer);

		return Ok;
	}

	RenBaseBGRA32 &CAlphaMask_private::GetRendererBaseImage()
	{
		return m_pImageData->m_oRendererBase;
	}

	ScanlineBGRA32Gray &CAlphaMask_private::GetScanlineImage()
	{
		return m_pImageData->m_oScanLine;
	}

	ScanlineGray8 &CAlphaMask_private::GetScanlineABuffer()
	{
		return m_pAlphaBufferData->m_oScanLine;
	}

	BYTE *CAlphaMask_private::GetMask()
	{
		return m_oRenderingBuffer.buf();
	}

	void CAlphaMask_private::StartApplying()
	{
		m_enStatus = ApplyingAlphaMask;
	}

	CAlphaMask_private &CAlphaMask_private::operator=(const CAlphaMask_private &oAlphaMask)
	{
		Clear();

		m_enDataType     = oAlphaMask.m_enDataType;
		m_enStatus       = oAlphaMask.m_enStatus;
		m_bExternalBuffer = false;

		if (EmptyAlphaMask == m_enStatus)
			return *this;

		const UINT unSize = oAlphaMask.m_oRenderingBuffer.width() * oAlphaMask.m_oRenderingBuffer.height() * ((m_enDataType == ImageBuffer) ? 4 : 1);

		BYTE* pBuffer = new BYTE[unSize];

		memcpy(pBuffer, oAlphaMask.m_oRenderingBuffer.buf(), unSize);

		Set(pBuffer, oAlphaMask.m_oRenderingBuffer.width(), oAlphaMask.m_oRenderingBuffer.height(), m_enDataType);

		return *this;
	}

	void CAlphaMask_private::Set(BYTE *pBuffer, UINT unWidth, UINT unHeight, AMaskDataType enDataType)
	{
		Clear();

		m_enDataType = enDataType;

		switch (enDataType)
		{
			case ImageBuffer:
			{
				m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, 4 * unWidth);

				m_pImageData = new AMaskFromImage(m_oRenderingBuffer);

				m_pImageData->m_oPixfmt.attach(m_oRenderingBuffer);
				m_pImageData->m_oRendererBase.attach(m_pImageData->m_oPixfmt);
				m_pImageData->m_oAlphaMask.attach(m_oRenderingBuffer);

				break;
			}
			case AlphaBuffer:
			{
				m_oRenderingBuffer.attach(pBuffer, unWidth, unHeight, unWidth);

				m_pAlphaBufferData = new AMaskFromABuffer(m_oRenderingBuffer);

				m_pAlphaBufferData->m_oPixfmt.attach(m_oRenderingBuffer);
				m_pAlphaBufferData->m_oRendererBase.attach(m_pAlphaBufferData->m_oPixfmt);
				m_pAlphaBufferData->m_oAlphaMask.attach(m_oRenderingBuffer);

				break;
			}
		}
	}

	agg::rendering_buffer &CAlphaMask_private::GetRenderingBuffer()
	{
		return m_oRenderingBuffer;
	}
}

