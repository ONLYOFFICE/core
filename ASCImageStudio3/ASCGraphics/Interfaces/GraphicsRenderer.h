#pragma once
#include "..\stdafx.h"
#include "..\..\..\..\Common\mediaformatdefine.h"
#include "..\agg\ASCWrapper\Graphics.h"
#include "StructuresASC.h"
#include "..\DocumentViewer\ViewerCommon.h"
#include "ImageFilesCache.h"

class CGraphicsRenderer
{
	friend class CAVSMetafile;
	friend class CAVSGraphicsRenderer;

protected:

	inline BOOL COM_GetBGRAImageFromInterface(IUnknown* pSource, BYTE*& pBGRA, int& nWidth, int& nHeight, int* pStride = NULL, long* pAspectX = NULL, long* pAspectY = NULL)
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
		long lWidth = 0; pUncompressedFrame->get_Width(&lWidth);
		long lHeight = 0; pUncompressedFrame->get_Height(&lHeight);
		long lColorSpace = 0; pUncompressedFrame->get_ColorSpace(&lColorSpace);
		long lStride = 0; pUncompressedFrame->get_Stride(0, &lStride);
		long lBufferSize = 0; pUncompressedFrame->get_BufferSize(&lBufferSize);
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
		pBGRA = pSourceBuffer;
		nWidth = lWidth;
		nHeight = lHeight;

		pUncompressedFrame->Release();

		return TRUE;
	}


protected:

	IAVSFontManager*						m_pFontManager;
	IAVSRenderer*							m_pAVSRenderer;

	Aggplus::CGraphics*						m_pRenderer;
	Aggplus::CGraphicsPath*					m_pPath;

public:
	NSStructuresAVS::CPen					m_oPen;
	NSStructuresAVS::CBrush					m_oBrush;
	NSStructuresAVS::CFont					m_oFont;
	NSStructuresAVS::CShadow				m_oShadow;
	NSStructuresAVS::CEdgeText				m_oEdge;

protected:

	double m_dWidth;
	double m_dHeight;

	double m_dPixelsWidth;
	double m_dPixelsHeight;

	DWORD m_lCurrentCommandType;

	LONG  m_lCurrentClipMode;
	BOOL  m_bIsSetupClip;

	// область отсечения
	LONG	m_lClipLeft;
	LONG	m_lClipTop;
	LONG	m_lClipWidth;
	LONG	m_lClipHeight;

	// пикселы
	BYTE*	m_pPixels;

	NSStructures::CFont m_oInstalledFont;

	NSDocumentViewer::CDIB* m_pDIB;
	NSDocumentViewer::CDoubleRect m_oRectBlt;
	double m_dBltAngle;

	CImageFilesCache* m_pCache;

public:

	CGraphicsRenderer(IAVSFontManager* pManager = NULL)
	{
		m_pRenderer = NULL;
		m_pPath = NULL;

		m_dWidth = 210;
		m_dHeight = 190;

		m_dPixelsWidth = 720;
		m_dPixelsHeight = 576;

		m_lCurrentCommandType	= 0;

		m_lCurrentClipMode	= 0;
		m_bIsSetupClip		= FALSE;

		m_pFontManager = NULL;

		m_oInstalledFont.Name = _T("");

		m_pDIB = NULL;
		m_dBltAngle = 0.0;

		m_pCache = NULL;

		m_pAVSRenderer = NULL;
	}

	void SetFontManager(IAVSFontManager* pManager = NULL)
	{
		RELEASEINTERFACE(m_pFontManager);
		
		if (NULL == pManager)
		{
			CoCreateInstance(__uuidof(CAVSFontManager), NULL, CLSCTX_INPROC, __uuidof(IAVSFontManager), (void**)&m_pFontManager);
			m_pFontManager->Initialize(L"");
			m_pFontManager->SetDefaultFont( L"Arial" );
		}
		else
		{
			m_pFontManager = pManager;
			ADDREFINTERFACE(m_pFontManager);
		}
	}
	void CheckFontManager()
	{
		if (NULL == m_pFontManager)
		{
			SetFontManager(NULL);
		}
	}
	void SetImageCache(CImageFilesCache* pCache)
	{
		RELEASEINTERFACE(m_pCache);
		m_pCache = pCache;
		ADDREFINTERFACE(m_pCache);
	}

	inline IAVSFontManager* GetFontManager()
	{
		return m_pFontManager;
	}

	BYTE* GetPixels(LONG& lWidth, LONG& lHeight)
	{
		lWidth  = (LONG)m_dPixelsWidth;
		lHeight = (LONG)m_dPixelsHeight;
		return m_pPixels;
	}

	~CGraphicsRenderer()
	{
		Clear();

		RELEASEOBJECT(m_pDIB);
		RELEASEINTERFACE(m_pFontManager);
	}
	inline void SetRenderer(IAVSRenderer* pRenderer)
	{
		m_pAVSRenderer = pRenderer;
	}

	inline void ClearInstallFont()
	{
		m_oInstalledFont.Name = _T("");
		m_oInstalledFont.Path = _T("");
	}
	inline void SetClipRect(double x, double y, double w, double h)
	{
		m_pRenderer->SetClipRect3(x, y, w, h);
	}

protected:
	BOOL CheckValidate(BOOL bOnlyGraphics = FALSE)
	{
		if (NULL == m_pRenderer)
			return FALSE;

		if (!bOnlyGraphics)
		{
			return (NULL != m_pPath);
		}

		return TRUE;
	}

	void Clear()
	{
		RELEASEOBJECT(m_pPath);
		RELEASEOBJECT(m_pRenderer);
	}

	inline void UpdateSize()
	{
		if (NULL == m_pRenderer)
			return;

		m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);

		m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
	}

public:
	// создание рендерера из пикселов
	inline LONG GetCurrentType()
	{
		return m_lCurrentCommandType;
	}
	void CreateFromMediaData(IUnknown* punkData)
	{
		BYTE* pPixels = NULL;
		int nWidth = 0;
		int nHeight = 0;

		int nStride = 0;
		if (COM_GetBGRAImageFromInterface(punkData, pPixels, nWidth, nHeight, &nStride))
		{
			NSDocumentViewer::CDoubleRect oRect;
			oRect.left		= 0;
			oRect.top		= 0;
			oRect.right		= nWidth;
			oRect.bottom	= nHeight;

			if (nStride > 0)
			{
				CreateFlip(pPixels, oRect, nWidth, nHeight);
			}
			else
			{
				Create(pPixels, oRect, nWidth, nHeight);
			}
		}
	}
	BOOL CreateFromHDC(HDC hDC, IAVSFontManager* pManager, double dWidthMM, double dHeightMM, double dLeft, double dTop, double dWidth, double dHeight, double dAngle)
	{
		RELEASEOBJECT(m_pDIB);

		m_oRectBlt.left		= dLeft;
		m_oRectBlt.top		= dTop;
		m_oRectBlt.right	= dLeft + dWidth;
		m_oRectBlt.bottom	= dTop + dHeight;

		LONG l1 = GetDeviceCaps(hDC, HORZRES);
		LONG l2 = GetDeviceCaps(hDC, VERTRES);

		m_dBltAngle			= dAngle;

		LONG lPixWidthDIB	= (LONG)(m_oRectBlt.GetWidth());
		LONG lPixHeightDIB	= (LONG)(m_oRectBlt.GetHeight());
		
		m_pDIB = new NSDocumentViewer::CDIB();
		BOOL bRes = FALSE;

		int nCount = 20;
		while (!bRes && (nCount > 0))
		{
			bRes = m_pDIB->Create(lPixWidthDIB, lPixHeightDIB);
			--nCount;
		}
	
		if (!bRes)
		{
			RELEASEOBJECT(m_pDIB);			
			return FALSE;
		}

		SetFontManager(pManager);

		NSDocumentViewer::CDoubleRect _oRect;
		_oRect.left		= 0;
		_oRect.top		= 0;
		_oRect.right	= lPixWidthDIB;
		_oRect.bottom	= lPixHeightDIB;

		memset(m_pDIB->m_pBits, 0xFF, 4 * lPixWidthDIB * lPixHeightDIB);
						
		Create(m_pDIB->m_pBits, _oRect, lPixWidthDIB, lPixHeightDIB);

		return TRUE;
	}

	BOOL BitBlt(HDC hDC)
	{
		if (NULL != m_pDIB)
		{
			double dAngle = m_dBltAngle * 180.0 / M_PI;
			if ((abs(dAngle - 90) < 1.0) || (abs(dAngle - 270) < 1.0))
			{
				Gdiplus::Graphics oRender(hDC);
				Gdiplus::Bitmap oImage(m_pDIB->m_lWidth, m_pDIB->m_lHeight, -4 * m_pDIB->m_lWidth, PixelFormat32bppARGB, 
					m_pDIB->m_pBits + 4 * ((m_pDIB->m_lHeight - 1) * m_pDIB->m_lWidth));

				Gdiplus::PointF pointCentre((float)m_oRectBlt.GetCentreX(), (float)m_oRectBlt.GetCentreY());

				oRender.SetPageUnit(Gdiplus::UnitPixel);

				oRender.TranslateTransform(-pointCentre.X, -pointCentre.Y, Gdiplus::MatrixOrderAppend);
				oRender.RotateTransform(-90, Gdiplus::MatrixOrderAppend);
				oRender.TranslateTransform(pointCentre.X, pointCentre.Y, Gdiplus::MatrixOrderAppend);

				oRender.DrawImage(&oImage, (float)m_oRectBlt.left, (float)m_oRectBlt.top, (float)m_oRectBlt.GetWidth(), (float)m_oRectBlt.GetHeight());
				return TRUE;
			}

			NSDocumentViewer::CPainter oPainter;
			oPainter.m_hDC = hDC;
			
			m_pDIB->Draw3(&oPainter, m_oRectBlt);

			return TRUE;
		}
		return FALSE;
	}

	void Create(BYTE* pPixels, const NSDocumentViewer::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, NSDocumentViewer::CDIB* pDib = NULL)
	{
		LONG lRectLeft	= (LONG)oRect.left;
		LONG lRectTop	= (LONG)oRect.top;
		LONG lWidth		= (LONG)oRect.GetWidth();
		LONG lHeight	= (LONG)oRect.GetHeight();

		m_dPixelsWidth	= lWidth;
		m_dPixelsHeight = lHeight;

		LONG lStride	= 4 * lWidthControl;

		m_pPixels = pPixels + (4 * lRectLeft);
		m_pPixels += lStride * lRectTop;

		RELEASEOBJECT(m_pRenderer);
		RELEASEOBJECT(m_pPath);

		m_pRenderer = new Aggplus::CGraphics();
		m_pPath		= new Aggplus::CGraphicsPath();


		m_lClipLeft		= max(0, lRectLeft);
		m_lClipTop		= max(0, lRectTop);

		//LONG lClipRight		= min(lWidth, -lRectLeft + lWidthControl);
		//LONG lClipBottom	= min(lHeight, -lRectTop + lHeightControl);

		LONG lClipRight		= min((LONG)oRect.right, lWidthControl);
		LONG lClipBottom	= min((LONG)oRect.bottom, lHeightControl);

		m_lClipWidth	= lClipRight - m_lClipLeft;
		m_lClipHeight	= lClipBottom - m_lClipTop;

		//m_pRenderer->Create(m_pPixels, lWidth, lHeight, -4 * lWidthControl, 0, colorModeRGBA8);
		m_pRenderer->Create2(pPixels, lWidthControl, lHeightControl, -4 * lWidthControl, 0, 
			m_lClipLeft, m_lClipTop, m_lClipWidth, m_lClipHeight, oRect.GetWidth(), oRect.GetHeight(), pDib);
	
		Aggplus::CMatrix oBase(1, 0, 0, 1, oRect.left, oRect.top);
		m_pRenderer->SetBaseTransform(&oBase);

		m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);

		//m_pRenderer->SetClipRect3(m_lClipLeft, m_lClipTop, m_lClipWidth, m_lClipHeight);
		//m_pRenderer->SetClipRect3(0, 0, lWidthControl, lHeightControl);
	}
	void CreateFlip(BYTE* pPixels, const NSDocumentViewer::CDoubleRect& oRect, LONG lWidthControl, LONG lHeightControl, NSDocumentViewer::CDIB* pDib = NULL)
	{
		LONG lRectLeft	= (LONG)oRect.left;
		LONG lRectTop	= (LONG)oRect.top;
		LONG lWidth		= (LONG)oRect.GetWidth();
		LONG lHeight	= (LONG)oRect.GetHeight();

		m_dPixelsWidth	= lWidth;
		m_dPixelsHeight = lHeight;

		LONG lStride	= 4 * lWidthControl;

		RELEASEOBJECT(m_pRenderer);
		RELEASEOBJECT(m_pPath);

		m_pRenderer = new Aggplus::CGraphics();
		m_pPath		= new Aggplus::CGraphicsPath();


		m_lClipLeft		= max(0, lRectLeft);
		m_lClipTop		= max(0, lRectTop);

		//LONG lClipRight		= min(lWidth, -lRectLeft + lWidthControl);
		//LONG lClipBottom	= min(lHeight, -lRectTop + lHeightControl);

		LONG lClipRight		= min((LONG)oRect.right, lWidthControl);
		LONG lClipBottom	= min((LONG)oRect.bottom, lHeightControl);

		m_lClipWidth	= lClipRight - m_lClipLeft;
		m_lClipHeight	= lClipBottom - m_lClipTop;

		//m_pRenderer->Create(m_pPixels, lWidth, lHeight, -4 * lWidthControl, 0, colorModeRGBA8);
		m_pRenderer->Create2(pPixels, lWidthControl, lHeightControl, 4 * lWidthControl, 0, 
			m_lClipLeft, m_lClipTop, m_lClipWidth, m_lClipHeight, oRect.GetWidth(), oRect.GetHeight(), pDib);
	
		Aggplus::CMatrix oBase(1, 0, 0, 1, oRect.left, oRect.top);
		m_pRenderer->SetBaseTransform(&oBase);

		m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
		m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);

		//m_pRenderer->SetClipRect3(m_lClipLeft, m_lClipTop, m_lClipWidth, m_lClipHeight);
		//m_pRenderer->SetClipRect3(0, 0, lWidthControl, lHeightControl);
	}

	// ---------------------------------------------
	// функции для работы со страницей (здесь - только лишь для вычисления dpi)
	inline STDMETHOD(NewPage)()
	{
		// ну не влезло так не влезло
		return S_OK;
	}
	inline STDMETHOD(put_Height)(double dHeight)
	{
		m_dHeight = dHeight;
		if (NULL != m_pRenderer)
		{
			m_pRenderer->SetPageHeight(m_dHeight, Aggplus::UnitMillimeter);
			m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
		}
		return S_OK;
	}
	inline STDMETHOD(get_Height)(double* dHeight)
	{
		if (NULL == dHeight)
			return S_FALSE;

		*dHeight = m_dHeight;
		return S_OK;
	}
	inline STDMETHOD(put_Width)(double dWidth)
	{
		m_dWidth = dWidth;
		if (NULL != m_pRenderer)
		{
			m_pRenderer->SetPageWidth(m_dWidth, Aggplus::UnitMillimeter);
			m_pRenderer->SetPageUnit(Aggplus::UnitMillimeter);
		}
		return S_OK;
	}
	inline STDMETHOD(get_Width)(double* dWidth)
	{
		if (NULL == dWidth)
			return S_FALSE;

		*dWidth = m_dWidth;
		return S_OK;
	}
	inline STDMETHOD(get_DpiX)(double* dDpiX)
	{
		if (NULL == dDpiX)
			return S_FALSE;

		*dDpiX = 25.4 * m_dPixelsWidth / m_dWidth;
		return S_OK;
	}
	inline STDMETHOD(get_DpiY)(double* dDpiY)
	{
		if (NULL == dDpiY)
			return S_FALSE;

		*dDpiY = 25.4 * m_dPixelsHeight / m_dHeight;
		return S_OK;
	}
	
	// ---------------------------------------------
	// функции состояния откисовщика ---------------
	inline STDMETHOD(SetPen)(BSTR bsXML)
	{
		CString str = (CString)bsXML;
		m_oPen.FromXmlString(str);
		return S_OK;
	}
	PROPERTY_RENDERER(Pen, Color, LONG)
	PROPERTY_RENDERER(Pen, Alpha, LONG)
	PROPERTY_RENDERER(Pen, Size, double)
	PROPERTY_RENDERER(Pen, DashStyle, BYTE)
	PROPERTY_RENDERER(Pen, LineStartCap, BYTE)
	PROPERTY_RENDERER(Pen, LineEndCap, BYTE)
	PROPERTY_RENDERER(Pen, LineJoin, BYTE)
	PROPERTY_RENDERER(Pen, DashOffset, double)
	PROPERTY_RENDERER(Pen, Align, LONG)
	PROPERTY_RENDERER(Pen, MiterLimit, double)

	STDMETHOD(PenDashPattern)(SAFEARRAY* pPattern)
	{
		if (NULL == pPattern)
			return NULL;

		m_oPen.SetDashPattern((double*)pPattern->pvData, pPattern->rgsabound[0].cElements);
		return S_OK;
	}

	STDMETHOD(SetBrush)(BSTR bsXML)
	{
		CString str = (CString)bsXML;
		m_oBrush.FromXmlString(str);
		return S_OK;
	}

	PROPERTY_RENDERER(Brush, Type, LONG)
	PROPERTY_RENDERER(Brush, Color1, LONG)
	PROPERTY_RENDERER(Brush, Color2, LONG)
	PROPERTY_RENDERER(Brush, Alpha1, LONG)
	PROPERTY_RENDERER(Brush, Alpha2, LONG)
	PROPERTY_RENDERER(Brush, TextureMode, LONG)
	PROPERTY_RENDERER(Brush, TextureAlpha, LONG)
	PROPERTY_RENDERER(Brush, LinearAngle, double)

	STDMETHOD(get_BrushTexturePath)(BSTR* bsPath)
	{
		*bsPath = m_oBrush.TexturePath.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_BrushTexturePath)(BSTR bsPath)
	{
		m_oBrush.TexturePath = (CString)bsPath;
		return S_OK;
	}

	STDMETHOD(BrushRect)(BOOL val, double left, double top, double width, double height)
	{
		m_oBrush.Rectable = val;
		m_oBrush.Rect.X = (float)left;
		m_oBrush.Rect.Y = (float)top;
		m_oBrush.Rect.Width = (float)width;
		m_oBrush.Rect.Height = (float)height;
		return S_OK;
	}

	STDMETHOD(SetFont)(BSTR bsXML)
	{
		CString str = (CString)bsXML;
		m_oFont.FromXmlString(str);
		return S_OK;
	}

	STDMETHOD(get_FontName)(BSTR* bsName)
	{
		*bsName = m_oFont.Name.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_FontName)(BSTR bsName)
	{
		m_oFont.Name = (CString)bsName;
		return S_OK;
	}
	STDMETHOD(get_FontPath)(BSTR* bsPath)
	{
		*bsPath = m_oFont.Path.AllocSysString();
		return S_OK;
	}
	STDMETHOD(put_FontPath)(BSTR bsPath)
	{
		m_oFont.Path = (CString)bsPath;
		return S_OK;
	}
	PROPERTY_RENDERER(Font, Size, double)
	STDMETHOD(get_FontStyle)(LONG* val)
	{
		*val = m_oFont.GetStyle();
		return S_OK;
	}
	STDMETHOD(put_FontStyle)(LONG val)
	{
		m_oFont.SetStyle(val);
		return S_OK;
	}
	PROPERTY_RENDERER(Font, StringGID, BOOL)
	PROPERTY_RENDERER(Font, CharSpace, double)
	
	STDMETHOD(SetShadow)(BSTR bsXML)
	{
		CString str = (CString)bsXML;
		m_oShadow.FromXmlString(str);
		return S_OK;
	}
	PROPERTY_RENDERER(Shadow, DistanceX, double)
	PROPERTY_RENDERER(Shadow, DistanceY, double)
	PROPERTY_RENDERER(Shadow, BlurSize, double)
	PROPERTY_RENDERER(Shadow, Color, LONG)
	PROPERTY_RENDERER(Shadow, Alpha, LONG)
	PROPERTY_RENDERER(Shadow, Visible, BOOL)
	
	STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		CString str = (CString)bsXML;
		m_oEdge.FromXmlString(str);
		return S_OK;
	}
	PROPERTY_RENDERER(Edge, Visible, LONG)
	PROPERTY_RENDERER(Edge, Color, LONG)
	PROPERTY_RENDERER(Edge, Alpha, LONG)
	PROPERTY_RENDERER(Edge, Dist, double)

	// ---------------------------------------------
	// отрисовка текста
	STDMETHOD(CommandDrawText)(BSTR bsText, double dX, double dY, double dWidth, double dHeight, double dBaseLineOffset)
	{
		if (c_nHyperlinkType == m_lCurrentCommandType)
			return S_OK;
		
		_SetFont();

		Aggplus::CBrush* pBrush = Aggplus::CreateBrush(&m_oBrush);				
		m_pRenderer->DrawString(bsText, m_pFontManager, pBrush, dX, dY + dBaseLineOffset);
		
		RELEASEOBJECT(pBrush);

		return S_OK;
	}
	// ---------------------------------------------
	// GraphicPath Methods
	STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->MoveTo((double)fX, (double)fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->LineTo((double)fX, (double)fY);
		return S_OK;
	}
	STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints)
	{
		if (!CheckValidate() || (NULL == pPoints))
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		m_pPath->AddLines((double*)pPoints->pvData, lCount);
		return S_OK;
	}

	STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->CurveTo(fX1, fY1, fX2, fY2, fX3, fY3);
		return S_OK;
	}
	STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints)
	{
		if (!CheckValidate())
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		m_pPath->AddBeziers((double*)pPoints->pvData, lCount);
		return S_OK;
	}
	STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->AddArc2(fX, fY, fWidth, fHeight, -fStartAngle, -fSweepAngle);
		return S_OK;
	}
	STDMETHOD(PathCommandClose)()
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->CloseFigure();
		return S_OK;
	}
	STDMETHOD(PathCommandEnd)()
	{
		m_pPath->Reset();
		return S_OK;
	}

	STDMETHOD(PathCommandStart)()
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->StartFigure();
		return S_OK;
	}
	STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY)
	{
		m_pPath->GetLastPoint(*fX, *fY);
		return S_OK;
	}
	
	STDMETHOD(DrawPath)(long nType)
	{
		if (!CheckValidate(TRUE))
			return S_FALSE;

		LONG lFillType = (nType & 0xFF00);
		BOOL bIsStroke = (0x01 == (nType & 0x01));

		if ((lFillType != 0) && (m_oBrush.Type == c_BrushTypeTexture || m_oBrush.Type == c_BrushTypePattern) && (NULL != m_pAVSRenderer))
		{
			CImageExt oExt;
			
			LONG lType = oExt.GetImageType(m_oBrush.TexturePath);
			if (0 != lType)
			{
				double x = 0;
				double y = 0;
				double w = 0;
				double h = 0;
				m_pPath->GetBounds(x, y, w, h);

				BSTR bsPath = m_oBrush.TexturePath.AllocSysString();
				oExt.DrawOnRenderer(m_pAVSRenderer, bsPath, x, y, w, h);
				SysFreeString(bsPath);			
				
				if (bIsStroke)
				{
					m_pRenderer->DrawPath(&m_oPen, m_pPath);
				}				
				return S_OK;
			}
		}

		switch (lFillType)
		{
		case c_nWindingFillMode:
			{
				m_pPath->SetRuler(false);

				CCacheImage* pCacheImage	= NULL;
				Aggplus::CBrush* pBrush		= NULL;
				
				if (m_oBrush.Type == c_BrushTypeTexture || m_oBrush.Type == c_BrushTypePattern)
				{
					Aggplus::WrapMode oMode = Aggplus::WrapModeClamp;
					switch (m_oBrush.TextureMode)
					{
					case c_BrushTextureModeTile:
						oMode = Aggplus::WrapModeTile;
						break;
					case c_BrushTextureModeTileCenter:
						oMode = Aggplus::WrapModeTile;
						break;
					default: 
						break;
					}

					Aggplus::CBrushTexture* pTextureBrush = NULL;
					
					if (NULL != m_pCache)
					{
						pCacheImage = m_pCache->Lock(m_oBrush.TexturePath);

						pTextureBrush = new Aggplus::CBrushTexture(pCacheImage->GetImage(), oMode);
					}
					else
					{
						pTextureBrush = new Aggplus::CBrushTexture(m_oBrush.TexturePath, oMode);
					}

					if( pTextureBrush )
					{
						if( m_oBrush.Type == c_BrushTypePattern )
						{
							pTextureBrush->m_bUsePattern = TRUE;
							pTextureBrush->m_colors[0] = Aggplus::CColor((BYTE)m_oBrush.Alpha1, m_oBrush.Color1);
							pTextureBrush->m_colors[1] = Aggplus::CColor((BYTE)m_oBrush.Alpha2, m_oBrush.Color2);
						}
					}

					pBrush = pTextureBrush;
				}
				else
				{
					pBrush = m_oBrush.GetBrush();
				}

				m_pRenderer->FillPath(pBrush, m_pPath);
				RELEASEOBJECT(pBrush);
				RELEASEINTERFACE(pCacheImage);
				break;
			}
		case c_nEvenOddFillMode:
			{
				m_pPath->SetRuler(true);

				CCacheImage* pCacheImage	= NULL;
				Aggplus::CBrush* pBrush		= NULL;
				
				if (m_oBrush.Type == c_BrushTypeTexture || m_oBrush.Type == c_BrushTypePattern)
				{
					Aggplus::CBrushTexture* pTextureBrush = NULL;
					
					if (NULL != m_pCache)
					{
						pCacheImage = m_pCache->Lock(m_oBrush.TexturePath);

						pTextureBrush = new Aggplus::CBrushTexture(pCacheImage->GetImage(), /*(Aggplus::WrapMode)TextureMode*/Aggplus::WrapModeClamp);
					}
					else
					{
						pTextureBrush = new Aggplus::CBrushTexture(m_oBrush.TexturePath, /*(Aggplus::WrapMode)TextureMode*/Aggplus::WrapModeClamp);
					}

					if( pTextureBrush )
					{
						if( m_oBrush.Type == c_BrushTypePattern )
						{
							pTextureBrush->m_bUsePattern = TRUE;
							pTextureBrush->m_colors[0] = Aggplus::CColor((BYTE)m_oBrush.Alpha1, m_oBrush.Color1);
							pTextureBrush->m_colors[1] = Aggplus::CColor((BYTE)m_oBrush.Alpha2, m_oBrush.Color2);
						}
					}

					pBrush = pTextureBrush;
				}
				else
				{
					pBrush = m_oBrush.GetBrush();
				}

				//Aggplus::CBrush* pBrush = m_oBrush.GetBrush();
				m_pRenderer->FillPath(pBrush, m_pPath);
				RELEASEOBJECT(pBrush);
				RELEASEINTERFACE(pCacheImage);

				break;
			}
		default:
			break;
		};

		if (bIsStroke)
		{
			m_pRenderer->DrawPath(&m_oPen, m_pPath);
		}
		
		return S_OK;
	}
	// ---------------------------------------------
	STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
	{
		if (!CheckValidate(TRUE) || NULL == pInterface)
			return S_FALSE;

		BYTE* pBGRA = NULL;
		int nWidth = 0;
		int nHeight = 0;

		int lStride = 0;
		BOOL bRes = COM_GetBGRAImageFromInterface(pInterface, pBGRA, nWidth, nHeight, &lStride);
		if (!bRes)
			return S_FALSE;

		if (m_pRenderer->IsClip())
		{
			Aggplus::CBitmap oImage( nWidth, nHeight, lStride, PixelFormat32bppARGB, pBGRA );
			m_pRenderer->DrawImage(&oImage, fX, fY, fWidth, fHeight);
			return S_OK;
		}

		// здесь мы внутри картинки перевернем
		if (0 > lStride)
		{
			// переворачиваем изображение для GDI+
			pBGRA += 4 * nWidth * (nHeight - 1);
		}
		Gdiplus::Bitmap oImageGP( nWidth, nHeight, lStride, PixelFormat32bppARGB, pBGRA );

		m_pRenderer->DrawGdiplusImage(&oImageGP, fX, fY, fWidth, fHeight);
		return S_OK;
	}
	//----------------------------------------------
	STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	// new
	STDMETHOD(BeginCommand)(DWORD lType)
	{
		m_lCurrentCommandType = lType;
		
		switch (lType)
		{
		case c_nPageType:
			{
				return NewPage();
			}
		default:
			break;
		};
		
		return S_OK;
	}
	STDMETHOD(EndCommand)(DWORD lType)
	{
		switch (lType)
		{
		case c_nClipType:
			{
				BOOL bIsIn = (c_nClipRegionTypeWinding == (0x0001 & m_lCurrentClipMode));

				m_pPath->SetRuler(bIsIn ? false : true);

				BOOL bIsIntersect = (c_nClipRegionIntersect == (0x0100 & m_lCurrentClipMode));
				m_pRenderer->CombineClip(m_pPath, bIsIntersect ? agg::sbool_and : agg::sbool_or);

				//m_pRenderer->SetClip(m_pPath);
				break;
			}
		case c_nResetClipType:
			{
				m_pRenderer->ResetClip();
				m_bIsSetupClip = FALSE;
				break;
			}
		default:
			break;
		};

		m_lCurrentCommandType = 0;
		return S_OK;
	}
	STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		if (NULL != bsGidText)
		{
			m_oFont.StringGID = TRUE;
			return CommandDrawText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}
		
		m_oFont.StringGID = FALSE;
		return CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
	{
		if (m_pRenderer->IsDib())
		{
			CImageExt oImageExt;
			LONG lType = oImageExt.GetImageType((CString)bstrVal);
			if (0 != lType)
			{
				if (NULL != m_pAVSRenderer)
				{
					oImageExt.DrawOnRenderer(m_pAVSRenderer, bstrVal, fX, fY, fWidth, fHeight);
				}
				else
				{
					m_pRenderer->DrawMeta(bstrVal, fX, fY, fWidth, fHeight);
				}
				return S_OK;
			}
		}
		
		CCacheImage* pCacheImage = NULL;
		if (NULL != m_pCache)
		{
			pCacheImage = m_pCache->Lock((CString)bstrVal);
		}

		if (NULL != pCacheImage)
		{
			Aggplus::CImage* pImage = pCacheImage->GetImage();
		
			//m_pRenderer->DrawImage(&oImage, fX, fY, fWidth, fHeight);
			BYTE* pBGRA = pImage->m_pImgData;

			if (0 > pImage->m_nStride)
			{
				// переворачиваем изображение для GDI+
				pBGRA += 4 * pImage->m_dwWidth * (pImage->m_dwHeight - 1);
			}
			Gdiplus::Bitmap oImageGdi( pImage->m_dwWidth, pImage->m_dwHeight, pImage->m_nStride, PixelFormat32bppARGB, pBGRA );

			m_pRenderer->DrawGdiplusImage(&oImageGdi, fX, fY, fWidth, fHeight);

			RELEASEINTERFACE(pCacheImage);
		}
		else
		{
			Aggplus::CBitmap oImage(bstrVal);
			
			//m_pRenderer->DrawImage(&oImage, fX, fY, fWidth, fHeight);
			BYTE* pBGRA = oImage.m_pImgData;

			if (0 > oImage.m_nStride)
			{
				// переворачиваем изображение для GDI+
				pBGRA += 4 * oImage.m_dwWidth * (oImage.m_dwHeight - 1);
			}
			Gdiplus::Bitmap oImageGdi( oImage.m_dwWidth, oImage.m_dwHeight, oImage.m_nStride, PixelFormat32bppARGB, pBGRA );

			m_pRenderer->DrawGdiplusImage(&oImageGdi, fX, fY, fWidth, fHeight);
		}

		return S_OK;
	}

	STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return S_OK;
	}
	STDMETHOD(SetCommandParams)(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		ApplyTransform2(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);
		return S_OK;
	}

	STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		if (!CheckValidate())
			return S_FALSE;

		ApplyTransform(dA, dB, dC, dD, dE, dF);
		
		return S_OK;
	}
	STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		if (!CheckValidate())
			return S_FALSE;

		double buffer[6];
		Aggplus::CMatrix oMatrix;
		m_pRenderer->GetTransform()->GetElements(buffer);

		*pdA = (double)buffer[0];
		*pdB = (double)buffer[1];
		*pdC = (double)buffer[2];
		*pdD = (double)buffer[3];
		*pdE = (double)buffer[4];
		*pdF = (double)buffer[5];

		return S_OK;
	}
	STDMETHOD(ResetTransform)(void)
	{
		_ResetTransform();
		return S_OK;
	}

	STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		if ((!CheckValidate()) || (NULL == plMode))
			return S_FALSE;

		*plMode = m_lCurrentClipMode;
		return S_OK;
	}
	STDMETHOD(put_ClipMode)(LONG lMode)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_lCurrentClipMode = lMode;
		return S_OK;
	}

	STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		if (!CheckValidate())
			return S_FALSE;

		_SetFont();

		// так как пат рисуется в миллиметрах - надо перевести пункты в миллиметры
		float fSize = (float)(0.3528 * m_oFont.Size);
		m_pPath->AddString(bsText, m_pFontManager, fX, fY);

		return S_OK;
	}
	STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		if (NULL != bsGidText)
		{
			m_oFont.StringGID = TRUE;
			return PathCommandText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}
		
		m_oFont.StringGID = FALSE;
		return PathCommandText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}

protected:
	void _SetFont()
	{
		if (m_oInstalledFont.IsEqual(&m_oFont))
		{
			if (1 < m_dWidth)
			{
				double dPix = m_oFont.CharSpace * m_dPixelsWidth / m_dWidth;
				m_pFontManager->SetCharSpacing(dPix);
			}
			return;
		}

		m_pFontManager->SetStringGID(m_oFont.StringGID);
		if (1 < m_dWidth)
		{
			double dPix = m_oFont.CharSpace * m_dPixelsWidth / m_dWidth;
			m_pFontManager->SetCharSpacing(dPix);
		}

		if (_T("") == m_oFont.Path)
		{
			BSTR bsName = m_oFont.Name.AllocSysString();
			m_pFontManager->LoadFontByName(bsName, (float)m_oFont.Size, m_oFont.GetStyle(), m_pRenderer->GetDpiX(), m_pRenderer->GetDpiY());
			SysFreeString(bsName);
		}
		else
		{
			BSTR bsName = m_oFont.Path.AllocSysString();
			m_pFontManager->LoadFontFromFile(bsName, (float)m_oFont.Size, m_pRenderer->GetDpiX(), m_pRenderer->GetDpiY());
			SysFreeString(bsName);
		}

		m_oInstalledFont = m_oFont;
	}

	void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
	{
		Aggplus::CMatrix oMatrix(d1, d2, d3, d4, d5, d6);
		ApplyTransform(&oMatrix);
	}

	void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		if ((dWidth <= 1) || (dHeight <= 1))
			lFlags = 0;

		BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
		BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
		
		double m11 = bFlipX ? -1.0 : 1.0;
		double m22 = bFlipY ? -1.0 : 1.0;
		
		Aggplus::CMatrix oMatrix(1, 0, 0, 1, 0, 0);

		if ((0 != dAngle) || (0 != lFlags))
		{
			double dCentreX = (dLeft + dWidth / 2.0);
			double dCentreY = (dTop + dHeight / 2.0);
			
			oMatrix.Translate(-dCentreX, -dCentreY	, Aggplus::MatrixOrderAppend);
			
			oMatrix.Rotate(dAngle			, Aggplus::MatrixOrderAppend);
			oMatrix.Scale(m11, m22					, Aggplus::MatrixOrderAppend);
			
			oMatrix.Translate(dCentreX, dCentreY	, Aggplus::MatrixOrderAppend);
		}

		ApplyTransform(&oMatrix);
	}

	inline void ApplyTransform(Aggplus::CMatrix* pMatrix)
	{
		if (!CheckValidate())
			return;

		m_pRenderer->SetTransform(pMatrix);
		m_pRenderer->CalculateFullTransform();
	}

	inline void _ResetTransform()
	{
		if (!CheckValidate())
			return;
		
		ApplyTransform(1, 0, 0, 1, 0, 0);
	}	
};