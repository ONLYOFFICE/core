#pragma once
#include "StructuresGDI.h"
#include "GdiplusEx.h"
#include "..\..\AVSVideoStudio3\Common\MediaFormatDefine.h"

class CGdiplusRenderer
{
public:
	// отрисовщик
	Gdiplus::Graphics* m_pRenderer;

	// контейнер примитивов
	Gdiplus::GraphicsPath* m_pPath;

	// состояние
	NSStructuresGDI::CPen		m_oPen;
	NSStructuresGDI::CBrush		m_oBrush;
	NSStructuresGDI::CFont		m_oFont;
	NSStructuresGDI::CShadow	m_oShadow;
	NSStructuresGDI::CEdgeText	m_oEdge;

	// формат для рисования
	Gdiplus::StringFormat m_oFormat;

	double m_dWidth;
	double m_dHeight;

	double m_dPixelsWidth;
	double m_dPixelsHeight;

	// картинка - на которой все рисуется...
	// нужна только для Addref - Release ( а то вдруг кто удалит картиночку, пока мы тут рисуем )
	MediaCore::IAVSUncompressedVideoFrame* m_pImage;
	Gdiplus::Bitmap* m_pBitmap;

	DWORD m_lCurrentCommandType;

	LONG m_lCurrentClipMode;

	Gdiplus::Matrix m_oBaseMatrix;
	BOOL			m_bBaseMatrixUse;

	CGdiPlusInit m_oInit;

	BOOL			m_bIsSetupClip;

public:

	CGdiplusRenderer() : m_oFormat(StringFormat::GenericTypographic()), m_oBaseMatrix(1, 0, 0, 1, 0, 0)
	{
		m_pRenderer = NULL;
		m_pPath = NULL;

		m_oInit.Init();

		m_oFormat.SetFormatFlags(StringFormatFlagsNoClip | StringFormatFlagsMeasureTrailingSpaces | StringFormatFlagsNoFitBlackBox);
		m_oFormat.SetAlignment(StringAlignmentNear);
		m_oFormat.SetLineAlignment(StringAlignmentNear);

		m_pImage = NULL;
		m_pBitmap = NULL;

		m_dWidth = 210;
		m_dHeight = 190;

		m_dPixelsWidth = 720;
		m_dPixelsHeight = 576;

		m_lCurrentCommandType	= 0;

		m_lCurrentClipMode	= 0;

		m_bBaseMatrixUse	= FALSE;

		m_bIsSetupClip		= FALSE;
	}

	~CGdiplusRenderer()
	{
		Clear();
	}

public:
	inline BOOL CheckValidate(BOOL bOnlyGraphics = FALSE)
	{
		if (NULL == m_pRenderer)
			return FALSE;

		if (!bOnlyGraphics)
		{
			return (NULL != m_pPath);
		}

		return TRUE;
	}

	inline void Clear()
	{
		RELEASEOBJECT(m_pPath);
		RELEASEOBJECT(m_pRenderer);
		
		RELEASEINTERFACE(m_pImage);
		RELEASEOBJECT(m_pBitmap);
	}

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

	
	inline void UpdateSize()
	{
		if (NULL == m_pBitmap)
			return;

		RELEASEOBJECT(m_pRenderer);

		double MMToInch = 25.4;

		float fDpiX = (float)(m_dPixelsWidth / (m_dWidth / MMToInch));
		float fDpiY = (float)(m_dPixelsHeight / (m_dHeight / MMToInch));
		m_pBitmap->SetResolution(fDpiX, fDpiY);

		if( Gdiplus::Ok != m_pBitmap->GetLastStatus() )
		{
			RELEASEINTERFACE(m_pImage);
			RELEASEOBJECT(m_pBitmap);
			return;
		}

		m_pRenderer = new Gdiplus::Graphics(m_pBitmap);
		m_pRenderer->SetSmoothingMode(SmoothingModeAntiAlias);
		m_pRenderer->SetTextRenderingHint(TextRenderingHintAntiAlias);

		m_pRenderer->SetPageUnit(UnitMillimeter);
	}

public:
	// создание рендерера из картинки
	STDMETHOD(CreateFromMediaData)(IUnknown* punkMediaData)
	{
		Clear();

		if (NULL == punkMediaData)
			return S_FALSE;

		punkMediaData->QueryInterface(MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&m_pImage);
		if (NULL == m_pImage)
			return S_FALSE;

		BYTE* pBGRA = NULL;
		int nWidth = 0;
		int nHeight = 0;

		int nStride = 0;

		// вытаскиваем из интерфейса указатель на память в формате BGRA
		BOOL bRes = COM_GetBGRAImageFromInterface(punkMediaData, pBGRA, nWidth, nHeight, &nStride);
		if (!bRes)
		{
			RELEASEINTERFACE(m_pImage);
			return S_FALSE;
		}

		m_dPixelsWidth = max(1.0, (double)nWidth);
		m_dPixelsHeight = max(1.0, (double)nHeight);

		if (nStride < 0)
		{
			// переворачиваем изображение для GDI+
			pBGRA += 4 * nWidth * (nHeight - 1);
		}

		m_pBitmap = new Gdiplus::Bitmap( nWidth, nHeight, /*-4*nWidth*/nStride, PixelFormat32bppARGB, pBGRA );

		double MMToInch = 25.4;

		float fDpiX = (float)(nWidth / (m_dWidth / MMToInch));
		float fDpiY = (float)(nHeight / (m_dHeight / MMToInch));
		m_pBitmap->SetResolution(fDpiX, fDpiY);

		if( Gdiplus::Ok != m_pBitmap->GetLastStatus() )
		{
			RELEASEINTERFACE(m_pImage);
			RELEASEOBJECT(m_pBitmap);
			return S_FALSE;
		}

		m_pRenderer = new Gdiplus::Graphics(m_pBitmap);
		m_pRenderer->SetSmoothingMode(SmoothingModeAntiAlias);
		m_pRenderer->SetTextRenderingHint(TextRenderingHintAntiAlias);

		m_pRenderer->SetPageUnit(UnitMillimeter);

		m_pPath = new Gdiplus::GraphicsPath(FillModeWinding);
		return S_OK;
	}
	STDMETHOD(CreateFromHDC)(HDC* phDC)
	{
		Clear();

		m_pRenderer = new Gdiplus::Graphics(*phDC);
		m_pRenderer->SetSmoothingMode(SmoothingModeAntiAlias);
		m_pRenderer->SetTextRenderingHint(TextRenderingHintAntiAlias);

		m_pRenderer->SetPageUnit(UnitMillimeter);

		m_pPath = new Gdiplus::GraphicsPath(FillModeWinding);
		return S_OK;
	}
	// ---------------------------------------------
	// функции для работы со страницей (здесь - только лишь для вычисления dpi)
	inline STDMETHOD(NewPage)()
	{
		// ну не влезло так не влезло
		return S_OK;
	}
	inline STDMETHOD(put_Height)(double fHeight)
	{
		double dOldH = m_dHeight;
		m_dHeight = max(10.0, fHeight);

		if (dOldH != m_dHeight)
		{
			UpdateSize();
		}

		return S_OK;
	}
	inline STDMETHOD(get_Height)(double* fHeight)
	{
		*fHeight = m_dHeight;
		return S_OK;
	}
	inline STDMETHOD(put_Width)(double fWidth)
	{
		double dOldW = m_dWidth;
		m_dWidth = max(10.0, fWidth);

		if (dOldW != m_dWidth)
		{
			UpdateSize();
		}

		return S_OK;
	}
	inline STDMETHOD(get_Width)(double* fWidth)
	{
		*fWidth = m_dWidth;
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
		m_oPen.FromXmlString((CString)bsXML);
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


	inline STDMETHOD(SetBrush)(BSTR bsXML)
	{
		m_oBrush.FromXmlString(CString(bsXML));
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

	inline STDMETHOD(SetFont)(BSTR bsXML)
	{
		m_oFont.FromXmlString(CString(bsXML));
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
		*bsPath = m_oFont.Name.AllocSysString();
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

	inline STDMETHOD(SetShadow)(BSTR bsXML)
	{
		m_oShadow.FromXmlString(CString(bsXML));
		return S_OK;
	}

	PROPERTY_RENDERER(Shadow, DistanceX, double)
	PROPERTY_RENDERER(Shadow, DistanceY, double)
	PROPERTY_RENDERER(Shadow, BlurSize, double)
	PROPERTY_RENDERER(Shadow, Color, LONG)
	PROPERTY_RENDERER(Shadow, Alpha, LONG)
	PROPERTY_RENDERER(Shadow, Visible, BOOL)

	inline STDMETHOD(SetEdgeText)(BSTR bsXML)
	{
		m_oEdge.FromXmlString(CString(bsXML));
		return S_OK;
	}
	PROPERTY_RENDERER(Edge, Visible, LONG)
	PROPERTY_RENDERER(Edge, Color, LONG)
	PROPERTY_RENDERER(Edge, Alpha, LONG)
	PROPERTY_RENDERER(Edge, Dist, double)

	// ---------------------------------------------
	// отрисовка текста
	inline STDMETHOD(CommandDrawText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		if (!CheckValidate(TRUE))
			return S_FALSE;

		if (c_nHyperlinkType == m_lCurrentCommandType)
			return S_OK;

		CString strText = (CString)(bsText);
		int nLength = strText.GetLength();

		Gdiplus::Font* pFont = m_oFont.GetFont(UnitPoint);

		RectF oRect((float)fX, (float)fY, (float)fWidth, (float)fHeight);

		if (m_oShadow.Visible)
		{
			m_oShadow.Draw(m_pRenderer, pFont, &m_oFormat, bsText, nLength, oRect);
		}
		
		m_oBrush.Rect = oRect;
		Gdiplus::Brush* pBrush = m_oBrush.GetBrush();
		m_pRenderer->DrawString(bsText, nLength, pFont, oRect, &m_oFormat, pBrush);

		if (m_oEdge.Visible)
		{
			m_oEdge.Draw(m_pRenderer, pFont, &m_oFormat, bsText, nLength, oRect);
		}

		RELEASEOBJECT(pBrush);
		RELEASEOBJECT(pFont);

		return S_OK;
	}
	// ---------------------------------------------
	// GraphicPath Methods
	inline STDMETHOD(PathCommandMoveTo)(double fX, double fY)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->StartFigure();
		m_pPath->AddLine((float)fX, (float)fY, (float)fX, (float)fY);
		return S_OK;
	}
	inline STDMETHOD(PathCommandLineTo)(double fX, double fY)
	{
		if (!CheckValidate())
			return S_FALSE;

		PointF pointCur(0, 0);
		CheckLastPoint(pointCur);
		m_pPath->AddLine(pointCur.X, pointCur.Y, (float)fX, (float)fY);

		return S_OK;
	}
	inline STDMETHOD(PathCommandLinesTo)(SAFEARRAY* pPoints)
	{
		if (!CheckValidate() || (NULL == pPoints))
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		lCount /= 2;

		double* points = (double*)pPoints->pvData; 

		CSimpleArray<Gdiplus::PointF> arPoints;
		
		for (int nIndex = 0; nIndex < lCount; ++nIndex)
		{
			Gdiplus::PointF oPoint((float)points[0], (float)points[1]);
			points += 2;
			arPoints.Add(oPoint);
		}
		
		if (1 < lCount)
		{
			m_pPath->AddLines(arPoints.m_aT, lCount);
		}

		return S_OK;
	}

	inline STDMETHOD(PathCommandCurveTo)(double fX1, double fY1, double fX2, double fY2, double fX3, double fY3)
	{
		if (!CheckValidate())
			return S_FALSE;

		PointF pointCur(0, 0);

		CheckLastPoint(pointCur);

		CSimpleArray<PointF> arPoints;
		
		arPoints.Add(pointCur);
		
		pointCur.X = (float)fX1; pointCur.Y = (float)fY1;
		arPoints.Add(pointCur);
		pointCur.X = (float)fX2; pointCur.Y = (float)fY2;
		arPoints.Add(pointCur);
		pointCur.X = (float)fX3; pointCur.Y = (float)fY3;
		arPoints.Add(pointCur);
		
		//m_pPath->AddCurve(arPoints.m_aT, 4);
		m_pPath->AddBeziers(arPoints.m_aT, 4);

		return S_OK;
	}
	inline STDMETHOD(PathCommandCurvesTo)(SAFEARRAY* pPoints)
	{
		if (!CheckValidate())
			return S_FALSE;

		LONG lCount = pPoints->rgsabound[0].cElements;
		lCount /= 2;

		double* points = (double*)pPoints->pvData; 

		CSimpleArray<Gdiplus::PointF> arPoints;
		
		for (int nIndex = 0; nIndex < lCount; ++nIndex)
		{
			Gdiplus::PointF oPoint((float)points[0], (float)points[1]);
			points += 2;
			arPoints.Add(oPoint);
		}

		int nSize = arPoints.GetSize();
		if (1 < nSize)
		{
			//m_pPath->AddCurve(arPoints.m_aT, nSize);
			m_pPath->AddBeziers(arPoints.m_aT, nSize);
		}

		return S_OK;
	}
	inline STDMETHOD(PathCommandArcTo)(double fX, double fY, double fWidth, double fHeight, double fStartAngle, double fSweepAngle)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->AddArc((float)fX, (float)fY, (float)fWidth, (float)fHeight, (float)fStartAngle, (float)fSweepAngle);
		return S_OK;
	}
	inline STDMETHOD(PathCommandClose)()
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->CloseFigure();
		return S_OK;
	}
	inline STDMETHOD(PathCommandEnd)()
	{
		//RELEASEOBJECT(m_pPath);
		//m_pPath = new Gdiplus::GraphicsPath(FillModeWinding);
		m_pPath->Reset();
		return S_OK;
	}

	inline STDMETHOD(PathCommandStart)()
	{
		if (!CheckValidate())
			return S_FALSE;

		m_pPath->StartFigure();
		return S_OK;
	}
	inline STDMETHOD(PathCommandGetCurrentPoint)(double* fX, double* fY)
	{
		if (!CheckValidate())
			return S_FALSE;

		if ((NULL == fX) || (NULL == fY))
			return S_FALSE;

		PointF oPoint(0, 0);
		CheckLastPoint(oPoint);

		*fX = (double)oPoint.X;
		*fY = (double)oPoint.Y;

		return S_OK;
	}
	
	inline STDMETHOD(DrawPath)(long nType)
	{
		if (!CheckValidate(TRUE))
			return S_FALSE;

		LONG lFillType = (nType & 0xFF00);
		BOOL bIsStroke = (0x01 == (nType & 0x01));

		REAL fLeft = 0;
		REAL fTop = 0;
		REAL fRight = 0;
		REAL fBottom = 0;

		/*Painter::CTextureManager oManager;

		if (0 < lFillType)
		{
			int nCount = m_pPath->GetPointCount();
			if (0 < nCount)
			{
				PointF* pPoints = new PointF[nCount];
				m_pPath->GetPathPoints(pPoints, nCount);
				
				fLeft = pPoints[0].X;
				fTop = pPoints[0].Y;
				fRight = pPoints[0].X;
				fBottom = pPoints[0].Y;

				for (int nIndex = 1; nIndex < nCount; ++nIndex)
				{
					fLeft = min(fLeft, pPoints[nIndex].X);
					fTop = min(fTop, pPoints[nIndex].Y);
					fRight = max(fRight, pPoints[nIndex].X);
					fBottom = max(fBottom, pPoints[nIndex].Y);
				}

				delete []pPoints;
			}

			if (Painter::c_BrushTypeTexture == m_oBrush.Type)
			{
				oManager.Add(m_oBrush.TexturePath);
			}
		}*/

		RectF oRect(0, 0, 1, 1);		

		switch (lFillType)
		{
		case c_nWindingFillMode:
			{
				/*m_pPath->SetFillMode(FillModeWinding);
				RectF oRect(fLeft, fTop, fRight - fLeft, fBottom - fTop);

				SmoothingMode oOldMode = m_pRenderer->GetSmoothingMode();
				m_pRenderer->SetSmoothingMode(SmoothingModeNone);
				m_pRenderer->FillPath(m_oBrush.GetBrush(&oManager, &oRect), m_pPath);
				m_pRenderer->SetSmoothingMode(oOldMode);*/

				m_pPath->SetFillMode(Gdiplus::FillModeWinding);
				m_pPath->GetBounds(&oRect);

				Gdiplus::Brush* pBrush = m_oBrush.GetBrush();
				m_pRenderer->FillPath(pBrush, m_pPath);
				
				RELEASEOBJECT(pBrush);
				break;
			}
		case c_nEvenOddFillMode:
			{
				/*m_pPath->SetFillMode(FillModeAlternate);
				RectF oRect(fLeft, fTop, fRight - fLeft, fBottom - fTop);
				
				SmoothingMode oOldMode = m_pRenderer->GetSmoothingMode();
				m_pRenderer->SetSmoothingMode(SmoothingModeNone);
				m_pRenderer->FillPath(m_oBrush.GetBrush(&oManager, &oRect), m_pPath);
				m_pRenderer->SetSmoothingMode(oOldMode);*/

				m_pPath->SetFillMode(Gdiplus::FillModeAlternate);
				m_pPath->GetBounds(&oRect);

				Gdiplus::Brush* pBrush = m_oBrush.GetBrush();
				m_pRenderer->FillPath(pBrush, m_pPath);
				
				RELEASEOBJECT(pBrush);
				break;
			}
		default:
			break;
		};

		if (bIsStroke)
		{
			//m_pRenderer->DrawPath(m_oPen.GetPen(), m_pPath);
			
			Gdiplus::Pen* pPen = m_oPen.GetPen();
			m_pRenderer->DrawPath(pPen, m_pPath);
			RELEASEOBJECT(pPen);
		}
		
		return S_OK;
	}
	// ---------------------------------------------
	inline STDMETHOD(DrawImage)(IUnknown* pInterface, double fX, double fY, double fWidth, double fHeight)
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

		if (0 > lStride)
		{
			// переворачиваем изображение для GDI+
			pBGRA += 4 * nWidth * (nHeight - 1);
		}
		
		Bitmap oImage( nWidth, nHeight, lStride, PixelFormat32bppARGB, pBGRA );
		
		m_pRenderer->DrawImage(&oImage, (float)fX, (float)fY, (float)fWidth, (float)fHeight);
		
		return S_OK;
	}
	//----------------------------------------------
	inline STDMETHOD(SetAdditionalParam)(BSTR ParamName, VARIANT ParamValue)
	{
		return S_OK;
	}
	inline STDMETHOD(GetAdditionalParam)(BSTR ParamName, VARIANT* ParamValue)
	{
		return S_OK;
	}

	// new
	inline STDMETHOD(BeginCommand)(DWORD lType)
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
	inline STDMETHOD(EndCommand)(DWORD lType)
	{
		switch (lType)
		{
		case c_nClipType:
			{
				Gdiplus::Region oCurrentRegion;
				m_pRenderer->GetClip(&oCurrentRegion);

				BOOL bIsIn = (c_nClipRegionTypeWinding == (0x0001 & m_lCurrentClipMode));

				m_pPath->SetFillMode(bIsIn ? Gdiplus::FillModeWinding : Gdiplus::FillModeAlternate);
				
				Region oNewRegion(m_pPath);

				if (!m_bIsSetupClip)
				{
					m_pRenderer->SetClip(&oNewRegion);
					m_bIsSetupClip = TRUE;
				}
				else
				{
					BOOL bIsIntersect = (c_nClipRegionIntersect == (0x0100 & m_lCurrentClipMode));
					if (bIsIntersect)
					{
						oCurrentRegion.Intersect(&oNewRegion);
					}
					else
					{
						oCurrentRegion.Union(&oNewRegion);
					}

					m_pRenderer->SetClip(&oCurrentRegion);
				}

				RELEASEOBJECT(m_pPath);
				m_pPath = new Gdiplus::GraphicsPath(FillModeWinding);

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
	inline STDMETHOD(CommandDrawTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
	{
		if (NULL != bsGidText)
		{
			m_oFont.StringGID = TRUE;
			return CommandDrawText(bsGidText, fX, fY, fWidth, fHeight, fBaseLineOffset);
		}
		
		m_oFont.StringGID = FALSE;
		return CommandDrawText(bsText, fX, fY, fWidth, fHeight, fBaseLineOffset);
	}
	inline STDMETHOD(DrawImageFromFile)(BSTR bstrVal, double fX, double fY, double fWidth, double fHeight)
	{
		return S_OK;
	}

	inline STDMETHOD(GetCommandParams)(double* dAngle, double* dLeft, double* dTop, double* dWidth, double* dHeight, DWORD* lFlags)
	{
		return S_OK;
	}
	inline STDMETHOD(SetCommandParams)(double dAngle,  double dLeft,  double dTop,  double dWidth,  double dHeight,  DWORD lFlags)
	{
		ApplyTransform2(dAngle, dLeft, dTop, dWidth, dHeight, lFlags);

		return S_OK;
	}

	inline STDMETHOD(SetTransform)(double dA, double dB, double dC, double dD, double dE, double dF)
	{
		if (!CheckValidate())
			return S_FALSE;

		ApplyTransform(dA, dB, dC, dD, dE, dF);
		
		return S_OK;
	}
	inline STDMETHOD(GetTransform)(double *pdA, double *pdB, double *pdC, double *pdD, double *pdE, double *pdF)
	{
		if (!CheckValidate())
			return S_FALSE;

		Gdiplus::Matrix oMatrix;
		m_pRenderer->GetTransform(&oMatrix);

		REAL buffer[10];
		oMatrix.GetElements(buffer);

		*pdA = (double)buffer[0];
		*pdB = (double)buffer[1];
		*pdC = (double)buffer[2];
		*pdD = (double)buffer[3];
		*pdE = (double)buffer[4];
		*pdF = (double)buffer[5];

		return S_OK;
	}
	inline STDMETHOD(ResetTransform)(void)
	{
		_ResetTransform();
		return S_OK;
	}

	inline STDMETHOD(get_ClipMode)(LONG* plMode)
	{
		if ((!CheckValidate()) || (NULL == plMode))
			return S_FALSE;

		*plMode = m_lCurrentClipMode;
		return S_OK;
	}
	inline STDMETHOD(put_ClipMode)(LONG lMode)
	{
		if (!CheckValidate())
			return S_FALSE;

		m_lCurrentClipMode = lMode;
		return S_OK;
	}

	inline STDMETHOD(PathCommandText)(BSTR bsText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset)
	{
		if (!CheckValidate())
			return S_FALSE;

		CString strText = (CString)(bsText);
		int nLength = strText.GetLength();

		Gdiplus::Font* pFont = m_oFont.GetFont(UnitPoint);
		PointF pointOrigin((float)fX, (float)fY);

		Gdiplus::FontFamily oFamily;
		pFont->GetFamily(&oFamily);

		// так как пат рисуется в миллиметрах - надо перевести пункты в миллиметры
		Gdiplus::REAL fSize = (Gdiplus::REAL)(0.3528 * pFont->GetSize());
		m_pPath->AddString(bsText, nLength, &oFamily, pFont->GetStyle(), fSize, pointOrigin, &m_oFormat);

		RELEASEOBJECT(pFont);

		return S_OK;
	}
	inline STDMETHOD(PathCommandTextEx)(BSTR bsText, BSTR bsGidText, BSTR bsSourceCodeText, double fX, double fY, double fWidth, double fHeight, double fBaseLineOffset, DWORD lFlags)
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

	void ApplyTransform(double d1, double d2, double d3, double d4, double d5, double d6)
	{
		Gdiplus::Matrix oMatrix((REAL)d1, (REAL)d2, (REAL)d3, (REAL)d4, (REAL)d5, (REAL)d6);
		ApplyTransform(&oMatrix);
	}

	void ApplyTransform2(double dAngle, double dLeft, double dTop, double dWidth, double dHeight, DWORD lFlags)
	{
		if ((dWidth <= 1) || (dHeight <= 1))
			lFlags = 0;

		BOOL bFlipX = (0 != (c_nParamFlipX & lFlags));
		BOOL bFlipY = (0 != (c_nParamFlipY & lFlags));
		
		REAL m11 = bFlipX ? -1.0f : 1.0f;
		REAL m22 = bFlipY ? -1.0f : 1.0f;
		
		Gdiplus::Matrix oMatrix(1, 0, 0, 1, 0, 0);

		if ((0 != dAngle) || (0 != lFlags))
		{
			REAL dCentreX = (REAL)(dLeft + dWidth / 2.0);
			REAL dCentreY = (REAL)(dTop + dHeight / 2.0);
			
			oMatrix.Translate(-dCentreX, -dCentreY	, MatrixOrderAppend);
			
			oMatrix.Rotate((REAL)dAngle	, MatrixOrderAppend);
			oMatrix.Scale(m11, m22					, MatrixOrderAppend);
			
			oMatrix.Translate(dCentreX, dCentreY	, MatrixOrderAppend);
		}

		ApplyTransform(&oMatrix);
	}

	inline void ApplyTransform(Gdiplus::Matrix* pMatrix)
	{
		if (!CheckValidate())
			return;

		if (m_bBaseMatrixUse)
		{
			pMatrix->Multiply(&m_oBaseMatrix, MatrixOrderPrepend);
		}
		m_pRenderer->SetTransform(pMatrix);
	}

	inline void _ResetTransform()
	{
		if (!CheckValidate())
			return;
		
		ApplyTransform(1, 0, 0, 1, 0, 0);
	}

	void CheckLastPoint(PointF& pointCur)
	{
		if (!CheckValidate())
			return;
		
		int nCount = m_pPath->GetPointCount();
		if (0 < nCount)
		{
			PointF* pPoints = new PointF[nCount];
			m_pPath->GetPathPoints(pPoints, nCount);

			pointCur.X = pPoints[nCount - 1].X;
			pointCur.Y = pPoints[nCount - 1].Y;

			delete []pPoints;
		}
	}

public:
	inline void CalculatePoint(PointF& oPoint)
	{
		CalculatePoint(oPoint.X, oPoint.Y);
	}
	inline void CalculatePoints(CSimpleArray<Gdiplus::PointF>& arPoints)
	{
		for (int nIndex = 0; nIndex < arPoints.GetSize(); ++nIndex)
		{
			CalculatePoint(arPoints[nIndex]);
		}
	}
	inline void CalculatePoint(float& fX, float& fY)
	{
		fX = (float)(fX * m_dPixelsWidth / m_dWidth);
		fY = (float)(fY * m_dPixelsHeight / m_dHeight);
	}

public:

	void SetBaseMatrix(const Gdiplus::Matrix& oMatrix)
	{
		REAL elements[6];
		oMatrix.GetElements(elements);
		m_oBaseMatrix.SetElements(elements[0], elements[1], elements[2], elements[3], elements[4], elements[5]);
		m_bBaseMatrixUse = TRUE;
	}
	void ResetBaseMatrix()
	{
		m_oBaseMatrix.SetElements(1, 0, 0, 1, 0, 0);
		m_bBaseMatrixUse = FALSE;
	}
	void ApplyBaseMatrix()
	{
		if (!CheckValidate())
			return;

		m_pRenderer->SetTransform(&m_oBaseMatrix);
	}


};