#include "stdafx.h"
#include "Image.h"
#include "../ap_Mem.h"

#ifdef AVS_CXIMAGE_USE
#include "../X__CacheFile.h"
#include "../X__CacheFile.cpp"

inline LPVOID ZC_GetCreateItemBuff(LPCWSTR szwFilename, DWORD *pdwRetWidth, DWORD *pdwRetHeight) 
{ 
	return Z__CacheFile.GetCreateItemBuff(szwFilename, pdwRetWidth, pdwRetHeight); 
}
inline LPVOID ZC_GetCreateCopy(LPVOID pSourceBuff, DWORD dwWidth, DWORD dwHeight) 
{ 
	return Z__CacheFile.GetCreateCopy(pSourceBuff, dwWidth, dwHeight); 
}
inline void   ZC_Release(LPVOID pBuff) 
{ 
	Z__CacheFile.Release(pBuff); 
}
#else

inline LPVOID ZC_GetCreateItemBuff(LPCWSTR szwFilename, DWORD *pdwRetWidth, DWORD *pdwRetHeight) 
{ 
	return NULL; 
}
inline LPVOID ZC_GetCreateCopy(LPVOID pSourceBuff, DWORD dwWidth, DWORD dwHeight) 
{ 
	return NULL; 
}
inline void   ZC_Release(LPVOID pBuff) 
{ 
	return;
}

#endif

void Aggplus::CImage::Create(const WCHAR *filename)
{
	Destroy();
	m_pImgData	= (BYTE*)ZC_GetCreateItemBuff(filename, &m_dwWidth, &m_dwHeight);
	m_nStride	= 4 * m_dwWidth;
	if(NULL == m_pImgData) 
	{ 
		Destroy(); 
	}
	else 
	{
		m_Status = Ok;
	}
}

void Aggplus::CImage::CreateFromImageStudio(const WCHAR *filename)
{
	Destroy();
	
	ImageStudio::IImageTransforms* pTransforms = NULL;
	CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&pTransforms); 

	if (NULL == pTransforms)
		return;

	CStringW strXml = L"<ImageFile-LoadImage sourcepath='";
	strXml += CStringW(filename);
	strXml += L"'/>";

	VARIANT_BOOL vbRes = VARIANT_FALSE;
	BSTR bsXml = strXml.AllocSysString();
	pTransforms->SetXml(bsXml, &vbRes);
	pTransforms->Transform(&vbRes);

	VARIANT var;
	pTransforms->GetResult(0, &var);

	if (NULL != var.punkVal)
	{
		var.punkVal->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrame);
		RELEASEINTERFACE((var.punkVal));
	}

	if (NULL != m_pFrame)
	{
		LONG lW = 0;
		LONG lH = 0;
		m_pFrame->get_Buffer(&m_pImgData);
		m_pFrame->get_Width(&lW);
		m_pFrame->get_Height(&lH);

		m_dwWidth = lW;
		m_dwHeight = lH;
		
		m_pFrame->get_Stride(0, &m_nStride);

		m_Status = Ok;
	}

	RELEASEINTERFACE(pTransforms);
}

void Aggplus::CImage::CopyFrom(const CImage *pSource)
{
	Destroy();
	if(pSource && pSource->m_Status == Ok)
	{
		m_Status	= Ok;
		m_dwWidth	= pSource->m_dwWidth;
		m_dwHeight	= pSource->m_dwHeight;
		m_nStride	= pSource->m_nStride;
		
		m_bExternalBuffer	= false;
		
		if(pSource->m_bExternalBuffer)
		{
			assert((unsigned)abs(m_nStride) == 4 * m_dwWidth);
			
			LONG lSize = 4 * m_dwWidth * m_dwHeight;

			m_pImgData = (BYTE*)Aggplus::ap_malloc16(lSize);
			memcpy(m_pImgData, pSource->m_pImgData, lSize);
		}
		else
		{
			m_pImgData = (BYTE*)ZC_GetCreateCopy(pSource->m_pImgData, m_dwWidth, m_dwHeight);
			if (NULL == m_pImgData) 
			{ 
				Destroy(); 
			}
		}
	}
}

void Aggplus::CImage::Destroy()
{
	if (NULL != m_pFrame)
	{
		RELEASEINTERFACE(m_pFrame);
		m_pImgData = NULL;
	}
	
	if (NULL != m_pImgData)
	{
		if (!m_bExternalBuffer)
		{
			ZC_Release(m_pImgData);
		}
	}

	m_Status	= WrongState;
	m_pImgData	= NULL;
	m_dwWidth	= 0;
	m_dwHeight	= 0;
	m_nStride	= 0;
	
	m_bExternalBuffer=false;
}