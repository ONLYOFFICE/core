#pragma once
#include "Writer3.h"
#include "SVGWriter2.h"

namespace NSHtmlRenderer
{
	class CTileInfo
	{
		/*
		_T("<htmltiling x=\"%.2lf\" y=\"%.2lf\" countx=\"%.2lf\" county=\"%.2lf\" stepx=\"%.2lf\" stepy=\"%.2lf\">\
<bbox x=\".2lf\" y=\"%.2lf\" r=\"%.2lf\" b=\"%.2lf\" />\
<transform m1=\"%.2lf\" m2=\"%.2lf\" m3=\"%.2lf\" m4=\"%.2lf\" m5=\"%.2lf\" m6=\"%.2lf\" />\
</htmltiling>
		*/

	public:
		double x;
		double y;
		double countx;
		double county;
		double stepx;
		double stepy;

		double bbox_x;
		double bbox_y;
		double bbox_r;
		double bbox_b;

		double transform_1;
		double transform_2;
		double transform_3;
		double transform_4;
		double transform_5;
		double transform_6;

	public:
		CTileInfo()
		{
			Clear();
		}

	public:

		void Clear()
		{
			x = 0;
			y = 0;
			countx = 0;
			county = 0;
			stepx = 0;
			stepy = 0;

			bbox_x = 0;
			bbox_y = 0;
			bbox_r = 0;
			bbox_b = 0;

			transform_1 = 1;
			transform_2 = 0;
			transform_3 = 0;
			transform_4 = 1;
			transform_5 = 0;
			transform_6 = 0;
		}

		void LoadFromXml(const CString& strXml)
		{
			XmlUtils::CXmlNode oNode;
			if (oNode.FromXmlString(strXml))
			{
				x		= XmlUtils::GetDouble(oNode.GetAttribute(_T("x"), _T("0")));
				y		= XmlUtils::GetDouble(oNode.GetAttribute(_T("y"), _T("0")));
				countx	= XmlUtils::GetDouble(oNode.GetAttribute(_T("countx"), _T("0")));
				county	= XmlUtils::GetDouble(oNode.GetAttribute(_T("county"), _T("0")));
				stepx	= XmlUtils::GetDouble(oNode.GetAttribute(_T("stepx"), _T("0")));
				stepy	= XmlUtils::GetDouble(oNode.GetAttribute(_T("stepy"), _T("0")));

				XmlUtils::CXmlNode oNodeBox;
				if (oNode.GetNode(_T("bbox"), oNodeBox))
				{
					bbox_x = XmlUtils::GetDouble(oNodeBox.GetAttribute(_T("x"), _T("0")));
					bbox_y = XmlUtils::GetDouble(oNodeBox.GetAttribute(_T("y"), _T("0")));
					bbox_r = XmlUtils::GetDouble(oNodeBox.GetAttribute(_T("r"), _T("0")));
					bbox_b = XmlUtils::GetDouble(oNodeBox.GetAttribute(_T("b"), _T("0")));
				}

				XmlUtils::CXmlNode oNodeTr;
				if (oNode.GetNode(_T("transform"), oNodeTr))
				{
					transform_1 = XmlUtils::GetDouble(oNodeTr.GetAttribute(_T("m1"), _T("0")));
					transform_2 = XmlUtils::GetDouble(oNodeTr.GetAttribute(_T("m2"), _T("0")));
					transform_3 = XmlUtils::GetDouble(oNodeTr.GetAttribute(_T("m3"), _T("0")));
					transform_4 = XmlUtils::GetDouble(oNodeTr.GetAttribute(_T("m4"), _T("0")));
					transform_5 = XmlUtils::GetDouble(oNodeTr.GetAttribute(_T("m5"), _T("0")));
					transform_6 = XmlUtils::GetDouble(oNodeTr.GetAttribute(_T("m6"), _T("0")));
				}
			}
		}
	};

	class CGraphicsDumper
	{
	public:
		Graphics::IASCGraphicsRenderer*		m_pRenderer;
		MediaCore::IAVSUncompressedVideoFrame*	m_pFrame;

		double							m_dWidth;
		double							m_dHeight;

		LONG							m_lWidthPix;
		LONG							m_lHeightPix;

		RECT							m_oBounds;

		CTileInfo						m_oTile;
		
	public:
		CGraphicsDumper()
		{
			m_pRenderer			= NULL;			
			m_pFrame			= NULL;
			
			m_dWidth			= -1;
			m_dHeight			= -1;

			m_lWidthPix			= -1;
			m_lHeightPix		= -1;

			m_oBounds.left		= 0;
			m_oBounds.top		= 0;
			m_oBounds.right		= 0;
			m_oBounds.bottom	= 0;
		}
		~CGraphicsDumper()
		{
			RELEASEINTERFACE(m_pRenderer);
			RELEASEINTERFACE(m_pFrame);
		}

		void NewPage(double dWidth, double dHeight)
		{
			if (dWidth != m_dWidth || dHeight != m_dHeight)
			{
				RELEASEINTERFACE(m_pFrame);
				
				m_dWidth	= dWidth;
				m_dHeight	= dHeight;				
			}

			RELEASEINTERFACE(m_pRenderer);

			m_lWidthPix		= (LONG)(96 * dWidth / 25.4);
			m_lHeightPix	= (LONG)(96 * dHeight / 25.4);

			if (NULL == m_pFrame)
			{
				CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pFrame);
				
				m_pFrame->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
				m_pFrame->put_Width( m_lWidthPix );
				m_pFrame->put_Height( m_lHeightPix );
				m_pFrame->put_AspectRatioX( m_lWidthPix );
				m_pFrame->put_AspectRatioY( m_lHeightPix );
				m_pFrame->put_Interlaced( VARIANT_FALSE );
				m_pFrame->put_Stride( 0, 4 * m_lWidthPix );
				m_pFrame->AllocateBuffer( -1 );
			}

			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);

			memset(pBuffer, 0xFF, 4 * m_lWidthPix * m_lHeightPix);

			CoCreateInstance(Graphics::CLSID_CASCGraphicsRenderer, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicsRenderer, (void**)&m_pRenderer);
			//ставим FontManager
			VARIANT vtVariant;
			vtVariant.vt = VT_UNKNOWN;
			vtVariant.punkVal = NULL;
			m_pRenderer->SetAdditionalParam( L"FontManager", vtVariant );
			
			m_pRenderer->put_Width(m_dWidth);
			m_pRenderer->put_Height(m_dHeight);
			m_pRenderer->CreateFromMediaData((IUnknown*)m_pFrame, 0, 0, m_lWidthPix, m_lHeightPix);
		}

		IUnknown* ConvertVectorGraphics()
		{
			BYTE* pBuffer = NULL;
			m_pFrame->get_Buffer(&pBuffer);

			BYTE* pBufferSrcMem = pBuffer + 4 * m_oBounds.top * m_lWidthPix + 4 * m_oBounds.left;
			LONG lWidthShape	= m_oBounds.right - m_oBounds.left + 1;
			LONG lHeightShape	= m_oBounds.bottom - m_oBounds.top + 1;

			MediaCore::IAVSUncompressedVideoFrame* pShapePicture = NULL;
			CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, 
									MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pShapePicture);

			pShapePicture->put_ColorSpace( ( 1 << 6) | ( 1 << 31) ); // CPS_BGRA | CPS_FLIP
			pShapePicture->put_Width( lWidthShape );
			pShapePicture->put_Height( lHeightShape );
			pShapePicture->put_AspectRatioX( lWidthShape );
			pShapePicture->put_AspectRatioY( lHeightShape );
			pShapePicture->put_Interlaced( VARIANT_FALSE );
			pShapePicture->put_Stride( 0, 4 * lWidthShape );
			pShapePicture->AllocateBuffer( -1 );

			BYTE* pBufferDst = NULL;
			pShapePicture->get_Buffer(&pBufferDst);
				
			for (LONG lLine = 0; lLine < lHeightShape; ++lLine)
			{
				memcpy(pBufferDst, pBufferSrcMem, 4 * lWidthShape);
				
				pBufferDst		+= 4 * lWidthShape;
				pBufferSrcMem	+= 4 * m_lWidthPix;
			}

			IUnknown* pReturn = NULL;
			pShapePicture->QueryInterface(IID_IUnknown, (void**)&pReturn);
			RELEASEINTERFACE(pShapePicture);

			return pReturn;
		}

	public:
		// pen --------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetPen(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetPen(bsXML);				
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenColor(LONG lColor)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenColor(lColor);				
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlpha(LONG lAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenAlpha(lAlpha);				
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenSize(double dSize)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenSize(dSize);				
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashStyle(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenDashStyle(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineStartCap(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenLineStartCap(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineEndCap(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenLineEndCap(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenLineJoin(BYTE val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenLineJoin(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenDashOffset(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenDashOffset(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenAlign(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenAlign(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_PenMiterLimit(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_PenMiterLimit(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PenDashPattern(SAFEARRAY* pPattern)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PenDashPattern(pPattern);
			}
			return S_OK;
		}
		// brush ------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetBrush(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetBrush(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushType(LONG lType)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushType(lType);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor1(LONG lColor)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushColor1(lColor);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha1(LONG lAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushAlpha1(lAlpha);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushColor2(LONG lColor)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushColor2(lColor);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushAlpha2(LONG lAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushAlpha2(lAlpha);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTexturePath(BSTR bsPath)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushTexturePath(bsPath);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureMode(LONG lMode)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushTextureMode(lMode);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushTextureAlpha(LONG lTxAlpha)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushTextureAlpha(lTxAlpha);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_BrushLinearAngle(double dAngle)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_BrushLinearAngle(dAngle);
			}
			return S_OK;
		}
		AVSINLINE HRESULT BrushRect(BOOL val, double left, double top, double width, double height)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->BrushRect(val, left, top, width, height);
			}
			return S_OK;
		}
		// font -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetFont(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetFont(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontName(BSTR bsName)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontName(bsName);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontPath(BSTR bsName)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontPath(bsName);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontSize(double dSize)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontSize(dSize);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStyle(LONG lStyle)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontStyle(lStyle);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontStringGID(BOOL bGID)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontStringGID(bGID);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_FontCharSpace(double dSpace)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_FontCharSpace(dSpace);
			}
			return S_OK;
		}
		// shadow -----------------------------------------------------------------------------------
		AVSINLINE HRESULT SetShadow(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetShadow(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceX(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowDistanceX(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowDistanceY(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowDistanceY(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowBlurSize(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowBlurSize(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowColor(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowColor(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowAlpha(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowAlpha(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ShadowVisible(BOOL val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ShadowVisible(val);
			}
			return S_OK;
		}
		// edge -------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetEdgeText(BSTR bsXML)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetEdgeText(bsXML);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeVisible(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeVisible(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeColor(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeColor(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeAlpha(LONG val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeAlpha(val);
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_EdgeDist(double val)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_EdgeDist(val);
			}
			return S_OK;
		}

		//-------- Функции для вывода текста --------------------------------------------------------
		AVSINLINE HRESULT CommandDrawText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
			}
			return S_OK;
		}
		AVSINLINE HRESULT CommandDrawTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->CommandDrawTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
			}
			return S_OK;
		}
		//-------- Маркеры для команд ---------------------------------------------------------------
		AVSINLINE HRESULT BeginCommand(DWORD lType)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->BeginCommand(lType);				
			}
			return S_OK;
		}
		AVSINLINE HRESULT EndCommand(DWORD lType)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->EndCommand(lType);
			}
			return S_OK;
		}
		//-------- Функции для работы с Graphics Path -----------------------------------------------
		AVSINLINE HRESULT PathCommandMoveTo(double fX, double fY)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandMoveTo(fX, fY);				
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLineTo(double fX, double fY)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandLineTo(fX, fY);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandLinesTo(SAFEARRAY* pPoints)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandLinesTo(pPoints);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurveTo(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandCurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandCurvesTo(SAFEARRAY* pPoints)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandCurvesTo(pPoints);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandArcTo(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandArcTo(fX, fY, fWidth, fHeight, fStartAngle, fSweepAngle);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandClose()
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandClose();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandEnd()
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandEnd();
			}
			return S_OK;
		}
		AVSINLINE HRESULT DrawPath(long nType)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->DrawPath(nType);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandStart()
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandStart();
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandText(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
			}
			return S_OK;
		}
		AVSINLINE HRESULT PathCommandTextEx(BSTR bsUnicodeText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->PathCommandTextEx(bsUnicodeText, bsGidText, bsSourceCodeText, fX, fY, fWidth, fHeight, fBaseLineOffset, lFlags);
			}
			return S_OK;
		}
		AVSINLINE HRESULT SetCommandParams(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetCommandParams(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
			}
			return S_OK;
		}
		//-------- Функции для вывода изображений --------------------------------------------------
		AVSINLINE HRESULT DrawImage(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->DrawImage(pInterface, fX, fY, fWidth, fHeight);
			}
			return S_OK;
		}
		AVSINLINE HRESULT DrawImageFromFile(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->DrawImageFromFile(bstrVal, fX, fY, fWidth, fHeight);
			}
			return S_OK;
		}
		//------------------------------------------------------------------------------------------
		AVSINLINE HRESULT SetAdditionalParam(BSTR ParamName, VARIANT ParamValue)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetAdditionalParam(ParamName, ParamValue);
			}
			return S_OK;
		}
		AVSINLINE HRESULT SetTransform(double dA, double dB, double dC, double dD, double dE, double dF)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->SetTransform(dA, dB, dC, dD, dE, dF);
			}
			return S_OK;
		}
		AVSINLINE HRESULT ResetTransform(void)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->ResetTransform();
			}
			return S_OK;
		}
		AVSINLINE HRESULT put_ClipMode(LONG lMode)
		{
			if (NULL != m_pRenderer)
			{
				m_pRenderer->put_ClipMode(lMode);
			}
			return S_OK;
		}
	};

	using namespace NSStrings;
	class CWriter4
	{
	public:
		CMetafile		m_oPage;
		CSVGWriter2		m_oSVGWriter;

		CAtlArray<CPageMeta>	m_arrPages;		
		bool					m_bIsBigPicture;

		LONG					m_lTilingCounter;

	public:
		NSStructures::CPen*		m_pPen;
		NSStructures::CBrush*	m_pBrush;
		NSStructures::CFont*	m_pFont;

		NSStructures::CPen		m_oLastPen;
		NSStructures::CBrush	m_oLastBrush;
		NSStructures::CFont		m_oLastFont;

		LONG					m_lCurrentFont;
		double					m_dCurrentFontSize;

		CFontDstGenerator		m_oDstFontGenerator;
		CFontManager			m_oFontManager;

		CHText					m_oSmartText;

		double m_dDpiX;
		double m_dDpiY;

		BOOL					m_bPathClosed;

		CAtlMap<CString, CImageInfo>	m_mapImagesFile;
		CAtlMap<DWORD, CImageInfo>		m_mapImageData;

		CCalculatorCRC32		m_oCRC;

		double m_dWidthDocMM;
		double m_dHeightDocMM;
		double m_dHeightPageMM;

		Graphics::IASCGraphicSimpleComverter*	m_pSimpleConverter;
		CFile m_oFileWriter;

		bool m_bIsGids;

	public:

		double					m_dWidth;
		double					m_dHeight;
		bool					m_bIsImageFromVectors;

	private:

		LONG					m_lNextIDShape;
		LONG					m_lNextIDImage;

		LONG					m_lCurrentPage;
		LONG					m_lCurrentFunctionPage;
		LONG					m_lPagesCount;

		LONG					m_lMaxSizeImage;

	public:
		CString					m_strDstDirectory;
		CString					m_strDstMedia;
		CString					m_strDstDirectoryFiles;
		CString					m_strFileName;

		CMatrix*				m_pTransform;
		CMatrix					m_oLastTransform;

		LONG					m_lCurrentDumpSize;

		LONG					m_lSrcFileType;

		bool					m_bIsClipping;
		bool					m_bIsSimpleGraphics;		

	public:
		CWriter4()
		{
			m_dDpiX = 96.0;
			m_dDpiY = 96.0;

			m_dWidth	= 0;
			m_dHeight	= 0;

			m_lNextIDImage	= 0;
			m_lNextIDShape	= 0;

			m_lCurrentPage	= -1;

			m_bPathClosed	= TRUE;

			m_pSimpleConverter = NULL;

			m_lMaxSizeImage = 800;

			m_dWidthDocMM	= 0;
			m_dHeightDocMM	= 0;
			m_dHeightPageMM	= 0;

			m_lPagesCount		= 0;

			m_bIsImageFromVectors = false;

			m_lSrcFileType = 0;

			m_bIsClipping = false;
			m_bIsSimpleGraphics = false;
			m_lTilingCounter = 0;
		}


		void CreateFile(CString& strDir, CString& strFileName)
		{
			m_lPagesCount		= 0;
			m_lCurrentPage		= -1;

			m_strDstDirectory		= strDir;
			m_strDstDirectoryFiles	= m_strDstDirectory + _T("\\") + strFileName;// + _T("_files");
			m_strFileName			= strFileName;

			CDirectory::CreateDirectory(m_strDstDirectoryFiles);
			m_strDstMedia = m_strDstDirectoryFiles + _T("\\media");
			CDirectory::CreateDirectory(m_strDstMedia);

			CString strFileFonts = m_strDstDirectoryFiles + _T("\\fonts");
			CDirectory::CreateDirectory(strFileFonts);

			CString strDocRendererS = m_strDstDirectoryFiles + _T("\\document_temp.bin");
			m_oFileWriter.CreateFile(strDocRendererS);

			m_oPage.Clear();
			m_arrPages.RemoveAll();
			m_bIsGids = false;
			m_lCurrentDumpSize = 0;

			m_oSmartText.SetParams(this);

			m_oSmartText.m_lCountParagraphs = 0;
			m_oSmartText.m_lCountWords = 0;
			m_oSmartText.m_lCountSymbols = 0;
			m_oSmartText.m_lCountSpaces = 0;
		}

		void SetSimpleConverter(Graphics::IASCGraphicSimpleComverter* pSimpleConverter, CMatrix* pMatrix)
		{
			m_pSimpleConverter = pSimpleConverter;
			m_pTransform = pMatrix;

			m_oSVGWriter.m_pTransform = m_pTransform;

			m_oSVGWriter.m_pPen = m_pPen;
			m_oSVGWriter.m_pBrush = m_pBrush;

			m_oSVGWriter.m_pLastPen = &m_oLastPen;
			m_oSVGWriter.m_pLastBrush = &m_oLastBrush;
		}
				
		AVSINLINE void WriteText(BSTR bsText, BSTR bsGid, double x, double y, double width, double height, double baselineoffset, const bool& bIsChangedFontParamBetweenDrawText)
		{
			if (m_lSrcFileType == AVS_OFFICESTUDIO_FILE_CROSSPLATFORM_DJVU)
			{
				// не нужно ничего писать из djvu
				return;
			}

			CheckVectorGraphics();
			CheckTectClipRect();

			bool bIsT = true;
			if (NULL == bsText || bsText[0] == 0)
				bIsT = false;
			bool bIsG = true;
			if (NULL == bsGid || bsGid[0] == 0)
				bIsG = false;
			
			if (!bIsT && !bIsG)
				return;

			bool bIsDumpFont = false;
		
			if (m_lCurrentFont == -1 || bIsChangedFontParamBetweenDrawText)
			{
				BSTR bsFontsSymbols = (NULL == bsGid) ? bsText : bsGid;
				LONG lCurrentFontIndex = m_oDstFontGenerator.AddFont(m_pFont, &m_oFontManager, bsFontsSymbols);
				if ((lCurrentFontIndex != m_lCurrentFont) || (m_pFont->Size != m_dCurrentFontSize))
				{
					m_lCurrentFont		= lCurrentFontIndex;
					m_dCurrentFontSize	= m_pFont->Size;

					bIsDumpFont = true;				
				}
			}
			else
			{
				BSTR bsFontsSymbols = (NULL == bsGid) ? bsText : bsGid;
				m_oDstFontGenerator.m_pFonts[m_lCurrentFont].AddBSTR(bsFontsSymbols);
			}

			if (NULL != bsGid)
				m_bIsGids = true;

			if (!bsText)
				bsText = bsGid;
			m_oSmartText.CommandText(bsText, bsGid, x, y, width, height, bIsDumpFont, this);
			return;
		}
		AVSINLINE void WriteImage(IUnknown* punkImage, double x, double y, double width, double height)
		{
			/*
			if (m_lTilingCounter > 0)
			{
				NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(punkImage);
				m_oSVGWriter.WriteImage(oInfo, x, y, width, height);
				return;
			}
			*/

			CheckVectorGraphics();
			m_oSmartText.DumpLine();
			SetTransformToDocument(true);

			bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
			if (bIsClip)
				m_oPage.Write(m_oSVGWriter.m_oClipMetafile);

			NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(punkImage);			
			//m_oSVGWriter.WriteImage(oInfo, x, y, width, height);
			WriteImage2(oInfo, x, y, width, height);

			if (bIsClip)
				m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
		}
		
		AVSINLINE void WritePattern(IUnknown* pPattern, CTileInfo& oTile)
		{
			CheckVectorGraphics();
			m_oSmartText.DumpLine();
			SetTransformToDocument(true);

			NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(pPattern);
			
			bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
			if (bIsClip)
				m_oPage.Write(m_oSVGWriter.m_oClipMetafile);

			WriteImagePattrern(oInfo, oTile);

			if (bIsClip)
				m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
		}

		AVSINLINE void WriteImage(CString& strFile, double x, double y, double width, double height)
		{
			if (m_lTilingCounter > 0)
			{
				NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(strFile);
				m_oSVGWriter.WriteImage(oInfo, x, y, width, height);
				return;
			}

			CheckVectorGraphics();
			m_oSmartText.DumpLine();
			SetTransformToDocument(true);

			bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
			if (bIsClip)
				m_oPage.Write(m_oSVGWriter.m_oClipMetafile);

			NSHtmlRenderer::CImageInfo oInfo = GenerateImageID(strFile);
			//m_oSVGWriter.WriteImage(oInfo, x, y, width, height);
			WriteImage2(oInfo, x, y, width, height);

			if (bIsClip)
				m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
		}

		AVSINLINE void WriteImage2(NSHtmlRenderer::CImageInfo& oID, const double& x, const double& y, const double& w, const double& h)
		{
			SetTransformToDocument(true);

			if (fabs(m_pTransform->m_agg_mtx.shx) < 0.0000001 && fabs(m_pTransform->m_agg_mtx.shy) < 0.0000001 && 
				m_pTransform->m_agg_mtx.sx >= 0 && m_pTransform->m_agg_mtx.sy >= 0)
			{
				double xx = x;
				double yy = y;
				double rr = x + w;
				double bb = y + h;
				m_pTransform->TransformPoint(xx, yy);
				m_pTransform->TransformPoint(rr, bb);
				
				if (oID.m_eType == itJPG)
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(0);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(xx);
					m_oPage.WriteDouble(yy);
					m_oPage.WriteDouble(rr - xx);
					m_oPage.WriteDouble(bb - yy);
				}
				else
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(1);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(xx);
					m_oPage.WriteDouble(yy);
					m_oPage.WriteDouble(rr - xx);
					m_oPage.WriteDouble(bb - yy);
				}
			}
			else 
			{
				if (oID.m_eType == itJPG)
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(10);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(x);
					m_oPage.WriteDouble(y);
					m_oPage.WriteDouble(w);
					m_oPage.WriteDouble(h);

					agg::trans_affine* t = &m_pTransform->m_agg_mtx;
					m_oPage.WriteDouble(t->sx);
					m_oPage.WriteDouble(t->shy);
					m_oPage.WriteDouble(t->shx);
					m_oPage.WriteDouble(t->sy);
					m_oPage.WriteDouble(t->tx);
					m_oPage.WriteDouble(t->ty);
				}
				else
				{
					m_oPage.WriteCommandType(CMetafile::ctDrawImage);
					m_oPage.WriteBYTE(11);
					m_oPage.WriteLONG(oID.m_lID);
					m_oPage.WriteDouble(x);
					m_oPage.WriteDouble(y);
					m_oPage.WriteDouble(w);
					m_oPage.WriteDouble(h);

					agg::trans_affine* t = &m_pTransform->m_agg_mtx;
					m_oPage.WriteDouble(t->sx);
					m_oPage.WriteDouble(t->shy);
					m_oPage.WriteDouble(t->shx);
					m_oPage.WriteDouble(t->sy);
					m_oPage.WriteDouble(t->tx);
					m_oPage.WriteDouble(t->ty);
				}
			}			
		}

		AVSINLINE void WriteImagePattrern(NSHtmlRenderer::CImageInfo& oID, CTileInfo& oTile)
		{
			SetTransformToDocument(true);
			double dKoef = 25.4 / m_dDpiX;

			double xx = oTile.bbox_x * dKoef;
			double yy = oTile.bbox_y * dKoef;
			double rr = oTile.bbox_r * dKoef;
			double bb = oTile.bbox_b * dKoef;
			
			if (oID.m_eType == itJPG)
			{
				m_oPage.WriteCommandType(CMetafile::ctDrawImage);
				m_oPage.WriteBYTE(0);
				m_oPage.WriteLONG(oID.m_lID);
				m_oPage.WriteDouble(xx);
				m_oPage.WriteDouble(yy);
				m_oPage.WriteDouble(rr - xx);
				m_oPage.WriteDouble(bb - yy);
			}
			else
			{
				m_oPage.WriteCommandType(CMetafile::ctDrawImage);
				m_oPage.WriteBYTE(1);
				m_oPage.WriteLONG(oID.m_lID);
				m_oPage.WriteDouble(xx);
				m_oPage.WriteDouble(yy);
				m_oPage.WriteDouble(rr - xx);
				m_oPage.WriteDouble(bb - yy);
			}
		}
		
		AVSINLINE void WriteBeginPath()
		{
			//m_oVectors.WriteCommandType(CMetafile::ctPathCommandStart);			
		}

		AVSINLINE void WriteEndPath()
		{	
			//m_oVectors.WriteCommandType(CMetafile::ctPathCommandStart);
			m_oSVGWriter.WritePathEnd();
		}		
		
		AVSINLINE void WritePathStart()
		{
		}
		AVSINLINE void WritePathClose()
		{
			//m_oVectors.WriteCommandType(CMetafile::ctPathCommandClose);
			m_oSVGWriter.WritePathClose();
		}

		AVSINLINE void WritePathMoveTo(const double& x, const double& y)
		{
			/*
			m_oVectors.WriteCommandType(CMetafile::ctPathCommandMoveTo);
			m_oVectors.WriteDouble(x);
			m_oVectors.WriteDouble(y);
			*/
			m_oSVGWriter.WritePathMoveTo(x, y);
		}
		AVSINLINE void WritePathLineTo(const double& x, const double& y)
		{
			/*
			m_oVectors.WriteCommandType(CMetafile::ctPathCommandLineTo);
			m_oVectors.WriteDouble(x);
			m_oVectors.WriteDouble(y);
			*/
			m_oSVGWriter.WritePathLineTo(x, y);
		}
		AVSINLINE void WritePathCurveTo(const double& x1, const double& y1, const double& x2, const double& y2, const double& x3, const double& y3)
		{
			/*
			m_oVectors.WriteCommandType(CMetafile::ctPathCommandCurveTo);
			m_oVectors.WriteDouble(x1);
			m_oVectors.WriteDouble(y1);
			m_oVectors.WriteDouble(x2);
			m_oVectors.WriteDouble(y2);
			m_oVectors.WriteDouble(x3);
			m_oVectors.WriteDouble(y3);
			*/
			m_oSVGWriter.WritePathCurveTo(x1, y1, x2, y2, x3, y3);
		}
		AVSINLINE void WriteDrawPath(LONG lType)
		{
			/*
			if (lType > 0xFF)
			{
				if (!m_oLastBrush.IsEqual(m_pBrush))
				{
					m_oLastBrush = *m_pBrush;
					SetFillColor(true);
				}
			}
			if (lType & 0x01)
			{
				if (!m_oLastPen.IsEqual(m_pPen))
				{
					m_oLastPen = *m_pPen;
					SetStrokeColor(true);
					SetLineWidth(true);
				}
			}

			m_oVectors.WriteCommandType(CMetafile::ctDrawPath);
			m_oVectors.WriteLONG(lType);
			*/

			CImageInfo oInfo;
			if ((lType > 0xFF) && (c_BrushTypeTexture == m_pBrush->Type))
			{
				oInfo = GenerateImageID(m_pBrush->TexturePath);

				if (TRUE)
				{
					// пока делаем так
					double x = 0;
					double y = 0;
					double w = 0;
					double h = 0;
					m_pSimpleConverter->PathCommandGetBounds(&x, &y, &w, &h);

					CheckVectorGraphics();

					/*
					bool bIsClip = (m_oSVGWriter.m_oClipMetafile.GetPosition() > 0) ? true : false;
					if (bIsClip)
						m_oPage.Write(m_oSVGWriter.m_oClipMetafile);
					*/
					LONG _oldPos = m_oSVGWriter.WriteTempClip();
					m_oPage.Write(m_oSVGWriter.m_oClipMetafile);
					
					WriteImage2(oInfo, x, y, w, h);

					m_oSVGWriter.m_oClipMetafile.Seek(_oldPos);

					/*
					if (bIsClip)
						m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);
					*/
					m_oPage.WriteLONG(CMetafile::ctBeginCommand, c_nResetClipType);

					lType &= 0xFF;
				}
			}

			m_oSVGWriter.WriteDrawPath(lType, m_pSimpleConverter, oInfo);
		}

		AVSINLINE void WritePathClip()
		{
			m_oSVGWriter.WritePathClip();
		}
		AVSINLINE void WritePathClipEnd()
		{
			m_oSVGWriter.WritePathClipEnd();
		}		
		AVSINLINE void WritePathResetClip()
		{
			m_oSVGWriter.WritePathResetClip();
		}

		AVSINLINE void SetFontStyle()
		{
			m_oPage.WriteCommandType(CMetafile::ctFontName);
			m_oPage.WriteLONG(m_lCurrentFont);
			m_oPage.WriteLONG(m_pFont->GetStyle());
			m_oPage.WriteDouble(m_dCurrentFontSize);
		}
		AVSINLINE void SetTransformToDocument(bool bIsFromGraphics)
		{
			if (NULL == m_pTransform)
				return;

			if (bIsFromGraphics)
			{
				if ((fabs(1 - m_oLastTransform.m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(1 - m_oLastTransform.m_agg_mtx.sy) < __g_matrix_eps))
				{
					// ничего делать не нужно
				}
				else
				{
					m_oPage.WriteCommandType(CMetafile::ctResetTransform);
					m_oLastTransform.Reset();
				}
			}
			else
			{
				if (true)
				{
					m_oLastTransform = *m_pTransform;

					agg::trans_affine* m = &m_pTransform->m_agg_mtx;
					//m_oPage.Write(CMetafile::ctSetTransform, m->sx, m->shy, m->shx, m->sy, m->tx, m->ty);
					m_oPage.WriteCommandType(CMetafile::ctSetTransform);
					m_oPage.WriteDouble(m->sx);
					m_oPage.WriteDouble(m->shy);
					m_oPage.WriteDouble(m->shx);
					m_oPage.WriteDouble(m->sy);
					m_oPage.WriteDouble(m->tx);
					m_oPage.WriteDouble(m->ty);
					return;
				}

				if ((fabs(m_pTransform->m_agg_mtx.sx - m_oLastTransform.m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.shx - m_oLastTransform.m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.shy - m_oLastTransform.m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.sy - m_oLastTransform.m_agg_mtx.sy) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.tx - m_oLastTransform.m_agg_mtx.tx) < __g_matrix_eps) &&
					(fabs(m_pTransform->m_agg_mtx.ty - m_oLastTransform.m_agg_mtx.ty) < __g_matrix_eps))
				{
					// ничего делать не нужно
					return;
				}

				if ((fabs(1 - m_pTransform->m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(0 - m_pTransform->m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(0 - m_pTransform->m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(1 - m_pTransform->m_agg_mtx.sy) < __g_matrix_eps) &&
					(fabs(1 - m_oLastTransform.m_agg_mtx.sx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shx) < __g_matrix_eps) &&
					(fabs(0 - m_oLastTransform.m_agg_mtx.shy) < __g_matrix_eps) &&
					(fabs(1 - m_oLastTransform.m_agg_mtx.sy) < __g_matrix_eps))
				{
					// ничего делать не нужно
					m_oLastTransform.Reset();
				}
				else
				{
					if ((fabs(1 - m_pTransform->m_agg_mtx.sx) < __g_matrix_eps) &&
						(fabs(0 - m_pTransform->m_agg_mtx.shx) < __g_matrix_eps) &&
						(fabs(0 - m_pTransform->m_agg_mtx.shy) < __g_matrix_eps) &&
						(fabs(1 - m_pTransform->m_agg_mtx.sy) < __g_matrix_eps))
					{
						// ничего делать не нужно
						m_oPage.WriteCommandType(CMetafile::ctResetTransform);
						m_oLastTransform.Reset();
						return;
					}

					m_oLastTransform = *m_pTransform;

					agg::trans_affine* m = &m_pTransform->m_agg_mtx;
					//m_oPage.Write(CMetafile::ctSetTransform, m->sx, m->shy, m->shx, m->sy, m->tx, m->ty);
					m_oPage.WriteCommandType(CMetafile::ctSetTransform);
					m_oPage.WriteDouble(m->sx);
					m_oPage.WriteDouble(m->shy);
					m_oPage.WriteDouble(m->shx);
					m_oPage.WriteDouble(m->sy);
					m_oPage.WriteDouble(m->tx);
					m_oPage.WriteDouble(m->ty);
				}
			}
		}

		void NewPage(const double& dWidthMM, const double& dHeightMM)
		{	
			++m_lPagesCount;

			CPageMeta oInfo;
			oInfo.m_dWidth = dWidthMM;
			oInfo.m_dHeight = dHeightMM;
			oInfo.m_lStart = m_lCurrentDumpSize;
			oInfo.m_lEnd = 0;

			m_dWidth = dWidthMM;
			m_dHeight = dHeightMM;

			m_arrPages.Add(oInfo);			

			m_oLastBrush.Color1 = -1;
			m_oLastPen.Color	= -1;
			m_oLastFont.Name	= _T("");

			m_lCurrentFont		= -1;
			m_dCurrentFontSize	= 0.0;

			m_oLastTransform.Reset();
			m_pTransform->Reset();

			m_bIsBigPicture = false;

			m_oSVGWriter.NewDocument(m_dWidth, m_dHeight, m_lPagesCount - 1);
		}
		void EndPage()
		{
			m_oSmartText.ClosePage();
			CheckVectorGraphics();

			m_arrPages[m_arrPages.GetCount() - 1].m_lEnd = m_lCurrentDumpSize + m_oPage.GetPosition();
			m_lCurrentDumpSize += m_oPage.GetPosition();
		
			m_oFileWriter.WriteFile((void*)m_oPage.GetData(), m_oPage.GetPosition());
			m_oPage.Clear();
		}

		void WriteStartDocument()
		{
			m_bIsGids = false;
		}
		
		void WriteEndDocument(CDocument& oDocument)
		{
			CMetafile oDocInfo;
			oDocInfo.WriteLONG(m_lPagesCount);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountParagraphs);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountWords);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountSymbols);
			oDocInfo.WriteLONG(m_oSmartText.m_lCountSpaces);

			oDocInfo.WriteLONG(m_oDstFontGenerator.m_lCountFonts);

			LONG lOffset = (6 + 4 * m_lPagesCount) * sizeof(LONG);
			for (LONG i = 0; i < m_lPagesCount; i++)
			{
				CPageMeta& oMeta = m_arrPages[i];
				oDocInfo.WriteDouble(oMeta.m_dWidth);
				oDocInfo.WriteDouble(oMeta.m_dHeight);
				oDocInfo.WriteLONG(oMeta.m_lStart + lOffset);
				oDocInfo.WriteLONG(oMeta.m_lEnd + lOffset);
			}

			m_oFileWriter.CloseFile();

			CFile oFilePages;
			oFilePages.OpenFile(m_strDstDirectoryFiles + _T("\\document_temp.bin"));

			LONG lMetaSize = (LONG)oFilePages.GetFileSize();
			LONG lSizeAll = oDocInfo.GetPosition() + lMetaSize;

			BYTE* pData = new BYTE[lSizeAll];
			memcpy(pData, oDocInfo.GetData(), oDocInfo.GetPosition());
			
			oFilePages.ReadFile(pData + oDocInfo.GetPosition(), lMetaSize);

			oFilePages.CloseFile();
			::DeleteFile(m_strDstDirectoryFiles + _T("\\document_temp.bin"));

			int nOutputLen = Base64EncodeGetRequiredLength(lSizeAll, ATL_BASE64_FLAG_NOCRLF);
			BYTE* pOutput = new BYTE[nOutputLen];
			Base64Encode(pData, lSizeAll, (LPSTR)pOutput, &nOutputLen, ATL_BASE64_FLAG_NOCRLF);
			
			CFile _file;
			_file.CreateFile(m_strDstDirectoryFiles + _T("\\document.js"));

			CStringA sDstLen = "";
			//#ifdef _DEBUG
			//sDstLen.Format("window[\"document_base64\"] = \"%d;", lSizeAll);
			//#else
			sDstLen.Format("%d;", lSizeAll);
			//#endif
			_file.WriteFile((void*)sDstLen.GetBuffer(), sDstLen.GetLength());
			
			_file.WriteFile((void*)pOutput, nOutputLen);

			//#ifdef _DEBUG
			//sDstLen = "\";";
			//_file.WriteFile((void*)sDstLen.GetBuffer(), sDstLen.GetLength());
			//#endif

			_file.CloseFile();
			
			m_oDstFontGenerator.WriteFonts(m_strDstDirectoryFiles + _T("\\fonts"), m_bIsGids);
		}

		AVSINLINE void CheckVectorGraphics()
		{
			/*
			if (0 < m_oVectors.GetPosition() && m_bIsSimpleGraphics && !m_oSVGWriter.m_bIsClipping)
			{
				m_oPage.Write(m_oVectors);
			}
			else if (m_oSVGWriter.m_oDocument.GetCurSize() > 0)
			{
				WriteImageID_SVG();
			}

			m_oVectors.ClearNoAttack();
			*/
			if (m_oSVGWriter.m_lEmtyDocChecker < (ULONG)m_oSVGWriter.m_oDocument.GetCurSize())
				WriteImageID_SVG();
		}

		AVSINLINE void CheckTectClipRect()
		{
			// смотрим, нужно ли обновить рект для клиппирования текста
			if (m_oSVGWriter.m_bIsTextClipWriteCleared)
			{
				if (!m_oSVGWriter.m_oTextClipBounds.IsCleared)
				{
					m_oSmartText.DumpLine();
					// записать клип
					m_oPage.WriteCommandType(CMetafile::ctCommandTextClipRect);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.x);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.y);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.r);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.b);
					m_oSVGWriter.m_bIsTextClipWriteCleared = false;
				}
			}
			else
			{
				if (m_oSVGWriter.m_oTextClipBounds.IsCleared)
				{
					m_oSmartText.DumpLine();
					// записать команду сброса клипа текстректа
					m_oPage.WriteCommandType(CMetafile::ctCommandTextClipRectReset);
					m_oSVGWriter.m_bIsTextClipWriteCleared = true;
					return;
				}
				else if (m_oSVGWriter.m_bIsIntersectNewClipRect)
				{
					m_oSmartText.DumpLine();
					// записать клип
					m_oPage.WriteCommandType(CMetafile::ctCommandTextClipRect);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.x);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.y);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.r);
					m_oPage.WriteDouble(m_oSVGWriter.m_oTextClipBounds.b);
					m_oSVGWriter.m_bIsIntersectNewClipRect = false;
				}
			}
		}

protected:
		AVSINLINE void CopyFile(CString& strFileSrc, CString& strFileDst)
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

			// не ресайзим картинки. Они рисуются теперь у нас с dpi 96
			if (true || (lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
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
			// не ресайзим картинки. Они рисуются теперь у нас с dpi 96
			if (true || (lWidth <= m_lMaxSizeImage) && (lHeight <= m_lMaxSizeImage))
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

		void SaveImage2(IUnknown* punkImage, CString strDst)
		{
			ImageStudio::IImageTransforms* pTransform = NULL;
			CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL ,CLSCTX_INPROC_SERVER, ImageStudio::IID_IImageTransforms, (void**)&pTransform);

			VARIANT var;
			var.vt = VT_UNKNOWN;
			var.punkVal = punkImage;
			pTransform->SetSource(0, var);

			CString strSaveItem = _T("<ImageFile-SaveAsPng destinationpath=\"") + strDst + _T(".png\" format=\"888\"/>");
			
			CString strXml = _T("<transforms>") + strSaveItem + _T("</transforms>");
						
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

		void WriteImageID_SVG()
		{
			m_oSmartText.DumpLine();
			SetTransformToDocument(true);

			bool bIsBIG = false;

#ifdef USE_SIMPLE_GRAPHICS_NOSVG

			if (m_oSVGWriter.m_bIsSimpleGraphics)
			{
				if (m_oSVGWriter.m_oVectors.GetPosition() > SVG_TO_RASTER_MIN_SIZE)
					bIsBIG = true;

				if (!bIsBIG)
				{
					m_oPage.Write(m_oSVGWriter.m_oVectors);
					m_oSVGWriter.NewSVG();
					return;
				}
			}

#endif

			if (!bIsBIG)
			{
				if (m_oSVGWriter.m_oDocument.GetCurSize() > SVG_TO_RASTER_MIN_SIZE)
					bIsBIG = true;
			}

			bool bIsNeedWriteEndSVG = true;
			if (bIsBIG)
			{
#ifdef USE_BIG_GRAPHICS_TORASTER
				bool bIsBIGWrite = WriteSVGAsBitmap();
				bIsNeedWriteEndSVG = false;
				if (bIsBIGWrite)
				{
					m_oSVGWriter.NewSVG();
					return;
				}
#endif
			}

			if ((m_oSVGWriter.m_oDocument.GetCurSize() > SVG_INLINE_MAX_SIZE))
			{			
				++m_lNextIDImage;

				CString strSaveItem = _T("");
				strSaveItem.Format(_T("\\image%d.svg"), m_lNextIDImage);

				strSaveItem = m_strDstMedia + strSaveItem;
				m_oSVGWriter.CloseFile2(strSaveItem, bIsNeedWriteEndSVG);
				
				m_oPage.WriteCommandType(CMetafile::ctDrawImage);
				m_oPage.WriteBYTE(2);
				m_oPage.WriteLONG(m_lNextIDImage);
			}
			else
			{
				m_oPage.WriteCommandType(CMetafile::ctDrawImage);
				m_oPage.WriteBYTE(3);
				m_oSVGWriter.CloseFile3(&m_oPage, bIsNeedWriteEndSVG);
			}

			m_oSVGWriter.NewSVG();
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

		bool WriteSVGAsBitmap()
		{
			BSTR bsXml = m_oSVGWriter.GetSVGXml();

			LONG nWidth = m_oSVGWriter.m_lWidth;
			LONG nHeight = m_oSVGWriter.m_lHeight;

			MediaCore::IAVSUncompressedVideoFrame* pFrame = NULL;
			CoCreateInstance( MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_ALL, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)(&pFrame) );
			if( !pFrame )
			{
				SysFreeString(bsXml);
				return false;
			}

			pFrame->put_Width( nWidth );
			pFrame->put_Height( nHeight );
			pFrame->put_ColorSpace( CSP_BGRA );
			pFrame->SetDefaultStrides();
			pFrame->AllocateBuffer( -1 );
			
			BYTE* pBuffer = NULL;
			pFrame->get_Buffer( &pBuffer );
			if( !pBuffer )
			{
				pFrame->Release();
				SysFreeString(bsXml);
				return false;
			}

			DWORD* dst = (DWORD*)pBuffer;
			DWORD dwColor = 0x00FFFFFF;
			LONG nCountPixels = nWidth * nHeight;
			for(LONG i = 0; i < nCountPixels; ++i)
			{
				*dst++ = dwColor;
			}

			long lBufSize;
			pFrame->get_BufferSize( &lBufSize );
			pFrame->put_DataSize( lBufSize );
			
			pFrame->put_AspectRatioX( nWidth );
			pFrame->put_AspectRatioY( nHeight );

			Graphics::IASCGraphicsRenderer* pGrRenderer;
			CoCreateInstance( Graphics::CLSID_CASCGraphicsRenderer, NULL, CLSCTX_ALL, Graphics::IID_IASCGraphicsRenderer, (void**)(&pGrRenderer) );
			if( !pGrRenderer )
			{
				pFrame->Release();
				SysFreeString(bsXml);
				return false;
			}
			
			pGrRenderer->put_Width ( (double)nWidth  );
			pGrRenderer->put_Height( (double)nHeight );
			
			HRESULT hr = pGrRenderer->CreateFromMediaData( (IUnknown*)pFrame, 0, 0, nWidth, nHeight );
			if ( FAILED(hr) )
			{
				pFrame->Release();
				pGrRenderer->Release();
				SysFreeString(bsXml);
				return false;
			}

			Graphics::ISVGTransformer* pTransformer;
			CoCreateInstance( Graphics::CLSID_SVGTransformer, NULL, CLSCTX_ALL, Graphics::IID_ISVGTransformer, (void**)(&pTransformer) );
			if( !pTransformer )
			{
				pFrame->Release();
				pGrRenderer->Release();
				SysFreeString(bsXml);
				return false;
			}

			hr = pTransformer->Load(bsXml);
			SysFreeString(bsXml);
			if ( FAILED(hr) )
			{
				pFrame->Release();
				pGrRenderer->Release();
				pTransformer->Release();
				return false;
			}

			hr = pTransformer->Draw((IUnknown*)pGrRenderer, 0, 0, nWidth, nHeight);

			if ( FAILED(hr) )
			{
				pFrame->Release();
				pGrRenderer->Release();
				pTransformer->Release();
				return false;
			}

			NSHtmlRenderer::CImageInfo oInfo = GenerateImageID((IUnknown*)pFrame);

			agg::trans_affine* m = &m_pTransform->m_agg_mtx;
			double _sx = m->sx;
			double _sy = m->sy;
			double _shx = m->shx;
			double _shy = m->shy;
			double _tx = m->tx;
			double _ty = m->ty;

			m->sx = 1;
			m->sy = 1;
			m->shx = 0;
			m->shy = 0;
			m->tx = 0;
			m->ty = 0;

			WriteImage2(oInfo, 0, 0, m_dWidth, m_dHeight);

			m->sx = _sx;
			m->sy = _sy;
			m->shx = _shx;
			m->shy = _shy;
			m->tx = _tx;
			m->ty = _ty;

			pFrame->Release();
			pGrRenderer->Release();
			pTransformer->Release();

			return true;
		}
	};
}
