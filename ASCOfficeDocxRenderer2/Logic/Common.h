#pragma once

#ifndef AVSINLINE
#if defined(_MSC_VER)
#define AVSINLINE __forceinline
#else
#define AVSINLINE inline
#endif
#endif

#include "../../Common/TemporaryCS.h"
#include "../Graphics/Matrix.h"
#include "../Graphics/Structures.h"
#include "StringWriter.h"
#include "CalculatorCRC32.h"

namespace NSDocxRenderer
{
	AVSINLINE LONG ConvertColor(LONG lBGR)
	{
		return (0x00FFFFFF & (((lBGR & 0xFF) << 16) | (lBGR & 0x0000FF00) | ((lBGR >> 16) & 0xFF)));
	} 

	AVSINLINE void CorrectString(CString& strValue)
	{
		strValue.Replace(_T("&"),	_T("&amp;"));			
		strValue.Replace(_T("'"),	_T("&apos;"));
		strValue.Replace(_T("<"),	_T("&lt;"));
		strValue.Replace(_T(">"),	_T("&gt;"));
		strValue.Replace(_T("\""),	_T("&quot;"));
	}

	class CBaseItem
	{
	public:
		enum ElemType
		{
			etParagraph	= 0,
			etImage		= 1,
			etShape		= 2
		};

		ElemType m_eType;

		CBaseItem()
		{
			m_eType = etShape;
		}

		virtual void ToXml(NSDocxRenderer::CStringWriter& oWriter) = 0;
	};

	class CImageManager
	{
	public:
		CAtlMap<CString, CImageInfo>	m_mapImagesFile;
		CAtlMap<DWORD, CImageInfo>		m_mapImageData;

		CString							m_strDstMedia;

		LONG							m_lMaxSizeImage;
		LONG							m_lNextIDImage;

		CCalculatorCRC32				m_oCRC;

	public:

		CImageManager()
		{
			m_strDstMedia	= _T("");
			m_lMaxSizeImage = 800;
			m_lNextIDImage	= 0;
		}

		AVSINLINE void NewDocument()
		{
			m_strDstMedia	= _T("");
			m_lMaxSizeImage = 800;
			m_lNextIDImage	= 0;

			m_mapImageData.RemoveAll();
			m_mapImagesFile.RemoveAll();
		}

	public:
		CImageInfo WriteImage(IUnknown* punkImage, double& x, double& y, double& width, double& height)
		{
			if (height < 0)
			{
				FlipY(punkImage);
				height = -height;
				y -= height;
			}
			
			return GenerateImageID(punkImage);
		}
		CImageInfo WriteImage(CString& strFile, double& x, double& y, double& width, double& height)
		{
			return GenerateImageID(strFile);
		}
	
	protected:
		inline void CopyFile(CString& strFileSrc, CString& strFileDst)
		{
			CDirectory::CopyFile(strFileSrc, strFileDst, NULL, NULL);
		}
		void SaveImage(CString& strFileSrc, CImageInfo& oInfo)
		{
			CString strLoadXml = _T("<transforms><ImageFile-LoadImage sourcepath=\"") + strFileSrc + _T("\"/></transforms>");

			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT_BOOL vbRes = VARIANT_FALSE;
			BSTR bsLoad = strLoadXml.AllocSysString();
			pTransform->SetXml(bsLoad, &vbRes);
			SysFreeString(bsLoad);

			pTransform->Transform(&vbRes);

			VARIANT var;
			var.punkVal = NULL;
			pTransform->GetResult(0, &var);

			if (NULL == var.punkVal)
			{
				RELEASEINTERFACE(pTransform);
				return;
			}

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			var.punkVal->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			RELEASEINTERFACE((var.punkVal));

			if (NULL == pFrame)
			{
				RELEASEINTERFACE(pTransform);
				return;
			}

			LONG lWidth		= 0;
			LONG lHeight	= 0;
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);

			oInfo.m_eType = GetImageType(pFrame);

			RELEASEINTERFACE(pFrame);

			CString strSaveItem = _T("");
			strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
			if (itJPG == oInfo.m_eType)
			{
				strSaveItem = _T("<ImageFile-SaveAsJpeg destinationpath=\"") + m_strDstMedia + strSaveItem + _T("jpg\" format=\"888\"/>");
			}
			else
			{
				strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + m_strDstMedia + strSaveItem + _T("png\" format=\"888\"/>");
			}

			CString strXml = _T("");

			if ((lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
			{
				strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
			}
			else
			{
				LONG lW = 0;
				LONG lH = 0;
				double dAspect = (double)lWidth / lHeight;

				if (lWidth >= lHeight)
				{
					lW = m_lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = m_lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\"/>"), lW, lH);

				strXml = _T("<transforms>") + strResize + strSaveItem + _T("</transforms>");
			}
			
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);
		}
		void SaveImage(IUnknown* punkImage, CImageInfo& oInfo)
		{
			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			LONG lWidth		= 0;
			LONG lHeight	= 0;
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);

			oInfo.m_eType = GetImageType(pFrame);

			RELEASEINTERFACE(pFrame);
			
			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = punkImage;
			pTransform->SetSource(0, var);

			CString strSaveItem = _T("");
			strSaveItem.Format(_T("\\image%d."), oInfo.m_lID);
			if (itJPG == oInfo.m_eType)
			{
				strSaveItem = _T("<ImageFile-SaveAsJpeg destinationpath=\"") + m_strDstMedia + strSaveItem + _T("jpg\" format=\"888\"/>");
			}
			else
			{
				strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + m_strDstMedia + strSaveItem + _T("png\" format=\"888\"/>");
			}

			CString strXml = _T("");
			if ((lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
			{
				strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
			}
			else
			{
				LONG lW = 0;
				LONG lH = 0;
				double dAspect = (double)lWidth / lHeight;

				if (lWidth >= lHeight)
				{
					lW = m_lMaxSizeImage;
					lH = (LONG)((double)lW / dAspect);
				}
				else
				{
					lH = m_lMaxSizeImage;
					lW = (LONG)(dAspect * lH);
				}

				CString strResize = _T("");
				strResize.Format(_T("<ImageTransform-TransformResize width=\"%d\" height=\"%d\"/>"), lW, lH);

				strXml = _T("<transforms>") + strResize + strSaveItem + _T("</transforms>");
			}
			
			VARIANT_BOOL vbSuccess = VARIANT_FALSE;
			BSTR bsXml = strXml.AllocSysString();
			pTransform->SetXml(bsXml, &vbSuccess);
			SysFreeString(bsXml);

			pTransform->Transform(&vbSuccess);

			RELEASEINTERFACE(pTransform);
		}

		CImageInfo GenerateImageID(IUnknown* punkData)
		{
			CImageInfo oInfo;

			if (NULL == punkData)
				return oInfo;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkData->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			BYTE* pBuffer = NULL;
			LONG lLen = 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_BufferSize(&lLen);

			DWORD dwSum = m_oCRC.Calc(pBuffer, lLen);

			CAtlMap<DWORD, CImageInfo>::CPair* pPair = m_mapImageData.Lookup(dwSum);
			if (NULL == pPair)
			{
				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(punkData, oInfo);
				m_mapImageData.SetAt(dwSum, oInfo);
			}
			else
			{
				oInfo = pPair->m_value;
			}

			RELEASEINTERFACE(pFrame);

			return oInfo;
		}

		CImageInfo GenerateImageID(CString& strFileName)
		{
			CImageInfo oInfo;
			CAtlMap<CString, CImageInfo>::CPair* pPair = m_mapImagesFile.Lookup(strFileName);

			if (NULL == pPair)
			{
				// нужно добавить
				++m_lNextIDImage;
				
				oInfo.m_lID = m_lNextIDImage;
				SaveImage(strFileName, oInfo);
				m_mapImagesFile.SetAt(strFileName, oInfo);
			}
			else
			{
				oInfo = pPair->m_value;
			}

			return oInfo;
		}

		ImageType GetImageType(MediaCore::IAVSUncompressedVideoFrame* pFrame)
		{
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			BYTE* pBuffer	= NULL;

			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Buffer(&pBuffer);

			BYTE* pBufferMem = pBuffer + 3;
			LONG lCountPix = lWidth * lHeight;

			for (LONG i = 0; i < lCountPix; ++i, pBufferMem += 4)
			{
				if (255 != *pBufferMem)
					return itPNG;
			}
			return itJPG;
		}

		void FlipY(IUnknown* punkImage)
		{
			if (NULL == punkImage)
				return;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			BYTE* pBuffer	= NULL;
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			LONG lStride	= 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Stride(0, &lStride);

			if (lStride < 0)
				lStride = -lStride;
			
			if ((lWidth * 4) != lStride)
			{
				RELEASEINTERFACE(pFrame);
				return;
			}

			BYTE* pBufferMem = new BYTE[lStride];

			BYTE* pBufferEnd = pBuffer + lStride * (lHeight - 1);

			LONG lCountV = lHeight / 2;

			for (LONG lIndexV = 0; lIndexV < lCountV; ++lIndexV)
			{
				memcpy(pBufferMem, pBuffer, lStride);
				memcpy(pBuffer, pBufferEnd, lStride);
				memcpy(pBufferEnd, pBufferMem, lStride);
				
				pBuffer		+= lStride;
				pBufferEnd	-= lStride;
			}

			RELEASEARRAYOBJECTS(pBufferMem);
			RELEASEINTERFACE(pFrame);
		}

		void FlipX(IUnknown* punkImage)
		{
			if (NULL == punkImage)
				return;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			punkImage->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

			if (NULL == pFrame)
				return;

			BYTE* pBuffer	= NULL;
			LONG lWidth		= 0;
			LONG lHeight	= 0;
			LONG lStride	= 0;

			pFrame->get_Buffer(&pBuffer);
			pFrame->get_Width(&lWidth);
			pFrame->get_Height(&lHeight);
			pFrame->get_Stride(0, &lStride);

			if (lStride < 0)
				lStride = -lStride;
			
			if ((lWidth * 4) != lStride)
			{
				RELEASEINTERFACE(pFrame);
				return;
			}

			DWORD* pBufferDWORD	= (DWORD*)pBuffer;

			LONG lW2 = lWidth / 2;
			for (LONG lIndexV = 0; lIndexV < lHeight; ++lIndexV)
			{
				DWORD* pMem1 = pBufferDWORD;
				DWORD* pMem2 = pBufferDWORD + lWidth - 1;
				
				LONG lI = 0;
				while (lI < lW2)
				{
					DWORD dwMem = *pMem1;
					*pMem1++ = *pMem2;
					*pMem2-- = dwMem;
				}
			}

			RELEASEINTERFACE(pFrame);
		}

	};
}

namespace NSDocxRenderer
{
	// 2-byte number
    inline short little_endian_2_big_endian( short s )
    {
		return ( ( s >> 8) & 0xff ) + ( ( s << 8 ) & 0xff00 );
    }

    /*========================================================================================================*/

    // 4-byte number
    inline int little_endian_2_big_endian( int i )
    {
		return ( ( i & 0xff ) << 24 ) + ( ( i & 0xff00 ) << 8 ) + ( ( i & 0xff0000 ) >> 8 ) + ( ( i >> 24 ) & 0xff );
    }

	AVSINLINE CString ToHexString( unsigned int ui )
    {
		CString strRes = _T("");
		strRes.Format(_T("%08X"), ui);
		return strRes;
    }

	/*========================================================================================================*/

    AVSINLINE CString ToHexString( short s )
    {
		CString strRes = _T("");
		strRes.Format(_T("%04X"), s);
		return strRes;
    }

	/*========================================================================================================*/
	
	AVSINLINE CString ToHexString( unsigned short us )
    {
		CString strRes = _T("");
		strRes.Format(_T("%04X"), us);
		return strRes;
    }

	/*========================================================================================================*/

    AVSINLINE CString ToHexString( char c )
    {
		CString strRes = _T("");
		strRes.Format(_T("%02X"), c);
		return strRes;
    }

	/*========================================================================================================*/
	
	AVSINLINE CString ToHexString( BYTE uc )
    {
		CString strRes = _T("");
		strRes.Format(_T("%02X"), uc);
		return strRes;
    }

	/*========================================================================================================*/
}