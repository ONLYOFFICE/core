#pragma once
#include "Common.h"
#include "..\..\..\Common\MediaFormatDefine.h"
#include "..\Interfaces\ImageSerializeObjects2.h"
#include "..\Interfaces\ASCGraphicsRenderer.h"

[object, uuid("BAA18A14-0888-4ed7-9623-C39E4F273F05"), dual, pointer_default(unique)]
__interface IAVSEffectTransform : IDispatch
{
	[id(10)]		HRESULT OpenXml([in] BSTR bstrXml);
	[id(11)]		HRESULT Draw([in] IAVSRenderer* pRenderer, [in] double dFrame);
	[id(12)]		HRESULT DrawOnMediaData([in] IUnknown* punkFrame, [in] double dFrame);
	
	[id(1001)]		HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)]		HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

// CAVSEffectTransform
[coclass, uuid("2684F9B4-B08F-42fd-9286-F5169BC198ED"), threading(apartment), vi_progid("AVSGraphics.EffectTransform"), progid("AVSGraphics.EffectTransform"), version(1.0), support_error_info(IAVSEffectTransform), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSEffectTransform
	:	public IAVSEffectTransform
{
protected:
	class CPageInfo
	{
	public:

		CString m_strPageInfo;

		double m_dWidthMetric;
		double m_dHeightMetric;

	public:
		CPageInfo() : 
			m_strPageInfo(_T("")),
			m_dWidthMetric(0.0),
			m_dHeightMetric(0.0)
		{
		}
		CPageInfo(const CPageInfo& oSrc)
		{
			*this = oSrc;
		}
		CPageInfo& operator=(const CPageInfo& oSrc)
		{
			m_strPageInfo = oSrc.m_strPageInfo;

			m_dWidthMetric = oSrc.m_dWidthMetric;
			m_dHeightMetric = oSrc.m_dHeightMetric;

			return *this;
		}

	public:

		void Render(IAVSRenderer* pRenderer)
		{
			if (NULL == pRenderer)
				return;

			XmlUtils::CXmlNode oNodeVideoTransforms;
			oNodeVideoTransforms.FromXmlString(m_strPageInfo);

			m_dWidthMetric	= 100;
			m_dHeightMetric	= 100;

			if ( TRUE )
			{
				XmlUtils::CXmlNodes oEffects;
				if (oNodeVideoTransforms.GetNodes(_T("*"), oEffects))
				{
					int nEffectCount = oEffects.GetCount();

					for (int i = 0; i < nEffectCount; ++i)
					{
						XmlUtils::CXmlNode oNodeEffect;
						oEffects.GetAt(i, oNodeEffect);
						
						CString sNodeName = oNodeEffect.GetName();
						if (g_cpszXMLEffectText == sNodeName)
						{
							m_dWidthMetric	= XmlUtils::GetDouble(oNodeEffect.GetAttributeOrValue(_T("widthmm"),  _T("100")));
							m_dHeightMetric = XmlUtils::GetDouble(oNodeEffect.GetAttributeOrValue(_T("heightmm"), _T("100")));

							pRenderer->put_Width(m_dWidthMetric);
							pRenderer->put_Height(m_dHeightMetric);

							ImageStudio::Serialize::Paint::Common::CTextMeasurer oMeasurer;

							oMeasurer.InternalFromXmlNode(oNodeEffect);				
							oMeasurer.DrawText(pRenderer, 1.0);
						}
						else if (g_cpszXMLPath == sNodeName)
						{
							ImageStudio::Serialize::Paint::Common::CDrawGraphicPath oPath;
							oPath.InternalFromXmlNode(oNodeEffect);

							oPath.Draw(pRenderer);
							pRenderer->PathCommandEnd();
						}
						else if (g_cpszXMLImage == sNodeName)
						{
							ImageStudio::Serialize::Paint::Common::CDrawImageFromFile oImagePainter;
							oImagePainter.InternalFromXmlNode(oNodeEffect);

							BSTR bsFile = oImagePainter.FilePath.AllocSysString();

							double dLeft	= oImagePainter.Left;
							double dTop		= oImagePainter.Top;
							double dWidth	= oImagePainter.Right - oImagePainter.Left;
							double dHeight	= oImagePainter.Bottom - oImagePainter.Top;

							CImageExt oImageExt;
							oImageExt.DrawOnRenderer(pRenderer, bsFile, dLeft, dTop, dWidth, dHeight);
							SysFreeString(bsFile);
						}
					}
				}
			}
		}
	};

protected:

	CPageInfo m_oInfo;

	CAVSEffectTransform() : m_oInfo()
	{
	}

	~CAVSEffectTransform()
	{
	}

public:
	STDMETHOD(OpenXml)(BSTR bstrXml)
	{
		m_oInfo.m_strPageInfo = (CString)bstrXml;
		return S_OK;
	}
	STDMETHOD(Draw)(IAVSRenderer* pRenderer, double dFrame)
	{
		m_oInfo.Render(pRenderer);
		return S_OK;
	}
	STDMETHOD(DrawOnMediaData)(IUnknown* punkFrame, double dComplete)
	{
		if (NULL == punkFrame)
			return S_FALSE;

		int lWidth		= 0;
		int lHeight		= 0;
		int lStride		= 0;
		BYTE* pBuffer	= NULL;

		if (!COM_GetBGRAImageFromInterface(punkFrame, pBuffer, lWidth, lHeight, &lStride))
			return S_FALSE;

		NSDocumentViewer::CDIB m_oSection;
		m_oSection.Create(lWidth, lHeight);

		LONG lBufferSize = 4 * lWidth * lHeight;
		memcpy(m_oSection.m_pBits, pBuffer, lBufferSize);

		IAVSRenderer* pRenderer = NULL;
		CoCreateInstance(__uuidof(CAVSGraphicsRenderer), NULL, CLSCTX_ALL, __uuidof(IAVSRenderer), (void**)&pRenderer);

		// --- fonts -----
		IAVSFontManager* pFonts = NULL;
		CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&pFonts);
		pFonts->Initialize(L"");
		pFonts->SetDefaultFont( L"Arial" );

		VARIANT var;
		var.vt = VT_BOOL;
		var.boolVal = VARIANT_TRUE;
		pFonts->SetAdditionalParam(L"UseDefaultFont", var);

		// ---------------		

		CAVSGraphicsRenderer* p2DRenderer = (CAVSGraphicsRenderer*)pRenderer;
		p2DRenderer->SetFontManager(pFonts);

		NSDocumentViewer::CDoubleRect oRect;
		oRect.left		= 0;
		oRect.top		= 0;
		oRect.right		= lWidth;
		oRect.bottom	= lHeight;
						
		p2DRenderer->m_oRenderer.Create(m_oSection.m_pBits, oRect, lWidth, lHeight, &m_oSection);

		Draw(pRenderer, dComplete);

		// все, теперь надо скопировать биты

		if (lStride < 0)
		{
			LONG lSize = 4 * lWidth * lHeight;
			memcpy(pBuffer, m_oSection.m_pBits, lSize);
		}
		else
		{
			BYTE* pSrc = m_oSection.m_pBits;
			LONG lSize = 4 * lWidth;
			BYTE* pDst = pBuffer + lSize * (lHeight - 1);

			for (int i = 0; i < lHeight; ++i)
			{
				memcpy(pDst, pSrc, lSize);
				pSrc += lSize;
				pDst -= lSize;
			}
		}

		// метафайлы полтят альфу. Вот реализуем emf по-нормальному - надо будет 
		// убрать этот код
		LONG lCount = lWidth * lHeight;
		pBuffer += 3;
		for (LONG i = 0; i < lCount; ++i, pBuffer += 4)
		{
			*pBuffer = 0xFF;
		}

		RELEASEINTERFACE(pFonts);
		RELEASEINTERFACE(pRenderer);

		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

protected:
	BOOL COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight, int* pStride = NULL, long* pAspectX = NULL, long* pAspectY = NULL)
	{
		/* @desc:
		int COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight)
		{
			из указанного интерфейса (pSource) вытаскивается картинка (pBGRA, nWidth, nHeight)
			в формате BGRA; если формат интерфейса не CSP_BGRA - конвертация формата не производится
		}
		*/

		if (NULL == pSource)
			return FALSE;

		// вытаскиваем из интерфейса uncompressed frame
		MediaCore::IAVSUncompressedVideoFrame* pUncompressedFrame = NULL;
		pSource->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pUncompressedFrame));
		if (NULL == pUncompressedFrame)
			return FALSE;

		// запрашиваем настройки картинки
		long lWidth			= 0; pUncompressedFrame->get_Width(&lWidth);
		long lHeight		= 0; pUncompressedFrame->get_Height(&lHeight);
		long lColorSpace	= 0; pUncompressedFrame->get_ColorSpace(&lColorSpace);
		long lStride		= 0; pUncompressedFrame->get_Stride(0, &lStride);
		long lBufferSize	= 0; pUncompressedFrame->get_BufferSize(&lBufferSize);
		BYTE* pSourceBuffer = NULL; pUncompressedFrame->get_Buffer(&pSourceBuffer);

		// проверяем совместимость кадра и формата
		if (CSP_BGRA != (CSP_COLOR_MASK & lColorSpace) || 4*lWidth != lStride || lWidth < 1 || lHeight < 1 || lBufferSize != 4*lWidth*lHeight || !pSourceBuffer)
		{
			pUncompressedFrame->Release();

			return FALSE;
		}

		if( pStride )
			*pStride = (lColorSpace & CSP_VFLIP) ? -lStride : lStride;

		if( pAspectX ) pUncompressedFrame->get_AspectRatioX( pAspectX );
		if( pAspectY ) pUncompressedFrame->get_AspectRatioY( pAspectY );

		// все хорошо, озвращаем наверх нужные параметры
		pBGRA	= pSourceBuffer;
		nWidth	= lWidth;
		nHeight = lHeight;

		pUncompressedFrame->Release();

		return TRUE;
	}
};

[object, uuid("3FD49373-02BA-46b7-A3CA-0AAF651BC1A0"), dual, pointer_default(unique)]
__interface IAVSImageFilter : IDispatch
{
	[id(10)]		HRESULT Filter([in] LONG lType, [in] IUnknown* punkSrc, [out, retval] IUnknown** ppunkDst);
	[id(11)]		HRESULT FilterEx([in] BSTR bsXml, [in] IUnknown* punkSrc, [out, retval] IUnknown** ppunkDst);
		
	[id(1001)]		HRESULT SetAdditionalParam([in] BSTR ParamName, [in] VARIANT ParamValue);
	[id(1002)]		HRESULT GetAdditionalParam([in] BSTR ParamName, [out, retval] VARIANT* ParamValue);
};

[coclass, uuid("78D262BC-3C39-4b53-982B-69BA8BED7678"), threading(apartment), vi_progid("AVSGraphics.ImageFilter"), progid("AVSGraphics.ImageFilter"), version(1.0), support_error_info(IAVSImageFilter), registration_script("control.rgs")]
class ATL_NO_VTABLE CAVSImageFilter
	:	public IAVSImageFilter
{
public:
	CAVSImageFilter()
	{
	}

	~CAVSImageFilter()
	{
	}

	STDMETHOD(Filter)(LONG lType, IUnknown* punkSrc, IUnknown** ppunkDst)
	{
		if (NULL == ppunkDst)
			return S_FALSE;

		if (0 == lType)
		{
			MediaCore::IAVSUncompressedVideoFrame* pSrcFrame = NULL;
			punkSrc->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pSrcFrame);

			if (NULL == pSrcFrame)
				return S_FALSE;

			LONG lWidth = 0;
			LONG lHeight = 0;
			BYTE* pBuffer = NULL;

			pSrcFrame->get_Width(&lWidth);
			pSrcFrame->get_Height(&lHeight);
			pSrcFrame->get_Buffer(&pBuffer);

			LONG lColorSpace = 0;
			pSrcFrame->get_ColorSpace(&lColorSpace);

			if (lWidth < 1 || lHeight < 1)
			{
				RELEASEINTERFACE(pSrcFrame);
			}
			
			LONG lLeft = 0;
			LONG lTop = 0;
			LONG lRight = lWidth - 1;
			LONG lBottom = lHeight - 1;

			LONG lStride = 4 * lWidth;

			// hors -----------------------------------------------------------------------------------
			while (lLeft <= lRight)
			{
				BYTE* pBufferMem = pBuffer + 4 * lLeft;
				bool bIsEmptyCur = true;
				for (long i = lTop; i <= lBottom; ++i, pBufferMem += lStride)
				{
					if (pBufferMem[3] != 0x00)
					{
						bIsEmptyCur = false;
						break;
					}
				}
				if (!bIsEmptyCur)
					break;
				++lLeft;
			}
			while (lRight >= lLeft)
			{
				BYTE* pBufferMem = pBuffer + 4 * lRight;
				bool bIsEmptyCur = true;
				for (long i = lTop; i <= lBottom; ++i, pBufferMem += lStride)
				{
					if (pBufferMem[3] != 0x00)
					{
						bIsEmptyCur = false;
						break;
					}
				}
				if (!bIsEmptyCur)
					break;
				--lRight;
			}
			// ---------------------------------------------------------------------------------------
			// vers ----------------------------------------------------------------------------------
			while (lTop <= lBottom)
			{
				BYTE* pBufferMem = pBuffer + lStride * lTop;
				bool bIsEmptyCur = true;
				for (long i = 0; i < lWidth; ++i, pBufferMem += 4)
				{
					if (pBufferMem[3] != 0x00)
					{
						bIsEmptyCur = false;
						break;
					}
				}
				if (!bIsEmptyCur)
					break;
				++lTop;
			}
			while (lBottom >= lTop)
			{
				BYTE* pBufferMem = pBuffer + lStride * lBottom;
				bool bIsEmptyCur = true;
				for (long i = 0; i < lWidth; ++i, pBufferMem += 4)
				{
					if (pBufferMem[3] != 0x00)
					{
						bIsEmptyCur = false;
						break;
					}
				}
				if (!bIsEmptyCur)
					break;
				--lBottom;
			}
			// ---------------------------------------------------------------------------------------

			MediaCore::IAVSUncompressedVideoFrame* pDstFrame = NULL;
			CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pDstFrame);
			if (NULL == pDstFrame)
			{
				RELEASEINTERFACE(pSrcFrame);
				return S_FALSE;
			}

			LONG lDstW = lRight - lLeft + 1;
			LONG lDstH = lBottom - lTop + 1;
			
			pDstFrame->put_ColorSpace(lColorSpace);
			pDstFrame->put_Width(lDstW);
			pDstFrame->put_Height(lDstH);
			pDstFrame->put_AspectRatioX(lDstW);
			pDstFrame->put_AspectRatioY(lDstH);
			pDstFrame->put_Interlaced(VARIANT_FALSE);
			pDstFrame->put_Stride(0, 4 * lDstW);
			pDstFrame->AllocateBuffer(-1);

			BYTE* pDstBuffer = NULL;
			pDstFrame->get_Buffer(&pDstBuffer);

			BYTE* pDstMem = pDstBuffer;
			LONG lStrideD = 4 * lDstW;
			for (LONG lLine = lTop; lLine <= lBottom; ++lLine, pDstMem += lStrideD)
			{
				memcpy(pDstMem, pBuffer + lStride * lLine + 4 * lLeft, lStrideD);
			}

			RELEASEINTERFACE(pSrcFrame);

			pDstFrame->QueryInterface(IID_IUnknown, (void**)ppunkDst);
			RELEASEINTERFACE(pDstFrame);
		}
		return S_OK;
	}
	STDMETHOD(FilterEx)(BSTR bsXml, IUnknown* punkSrc, IUnknown** punkDst)
	{
		return S_OK;
	}

	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}
};