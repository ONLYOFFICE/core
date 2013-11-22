#include "stdafx.h"
#include "DocumentViewer.h"

#include "TimeMeasurer.h"

LRESULT CAVSDocumentViewer::OnMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	switch (uMsg)
	{
	case WM_ERASEBKGND:
		{
			bHandled = TRUE;
			break;
		}
	case WM_PAINT:
		{
			if (!m_oCacheDocument.IsGlobalRepaint())
			{
				BlitPixels();
			}
			break;
		}
	case WM_PRINT:
		{
			CTemporaryCS oCS (&m_oCS);
			BlitPixelsDC((HDC)wParam);
			break;
		}
	case WM_SIZE:
		{
			OnSize();
			break;
		}
	case WM_MOVE:
		{
			OnMove();
			break;
		}
	case WM_MOUSEWHEEL:
		{
			LONG lOffset = 50 * GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA;
			OnMouseWhell(lOffset);
			break;
		}
	case WM_LBUTTONDOWN:
		{
			long xPos = GET_X_LPARAM(lParam); 
			long yPos = GET_Y_LPARAM(lParam);
			OnLButtonDown(xPos, yPos);
			bHandled = TRUE;

			OnMouseDown(1, 0, (double)xPos, (double)yPos);
			return 0;
		}
	case WM_LBUTTONDBLCLK:
		{
			long xPos = GET_X_LPARAM(lParam); 
			long yPos = GET_Y_LPARAM(lParam);

			bHandled = TRUE;
			OnMouseDblClick(1, 0, (double)xPos, (double)yPos);
			return 0;
		}
	case WM_LBUTTONUP:
		{
			OnLButtonUp();
			bHandled = TRUE;

			OnMouseUp(1, 0, 0.0, 0.0);
			return 0;
		}
	case WM_MOUSEMOVE:
		{
			long xPos = GET_X_LPARAM(lParam); 
			long yPos = GET_Y_LPARAM(lParam);
			OnMouseMove(xPos, yPos);
			bHandled = TRUE;

			OnMouseMove(1, 0, (double)xPos, (double)yPos);
			return 0;
		}
	case WM_DESTROY:
		{
			OnDestroy();
			break;
		}
	case WM_KEYDOWN:
		{
			OnKeyDown((LONG)wParam, (DWORD)lParam);

			OnKeyDown((LONG)wParam, (LONG)lParam, 0.0, 0.0);
			return 0;
		}
	case WM_KEYUP:
		{
			OnKeyUp((LONG)wParam, (LONG)lParam, 0.0, 0.0);
			return 0;
		}
	}

	OnMessage(uMsg, wParam, lParam, &bHandled);
	return 0;
}

void CAVSDocumentViewer::OnKeyDown(LONG lKey, DWORD dwData)
{
}

void CAVSDocumentViewer::OnSize()
{
	CTemporaryCS oCS(&m_oCS);
	
	RECT rc;
	GetClientRect(&rc);

	if ((0 >= (rc.right - rc.left)) || (0  >= (rc.bottom - rc.top)))
	{
		// окно свернулось (???)
		return;
	}

	HDC hdc		= GetDC();

	m_dDpiX = (double)GetDeviceCaps(hdc, LOGPIXELSX);
	m_dDpiY = (double)GetDeviceCaps(hdc, LOGPIXELSY);

	ReleaseDC(hdc);

	UpdatePixels(rc.right - rc.left, rc.bottom - rc.top);

	CheckZoom(FALSE);

	CalculateDocumentSize();
	Calculate();

	m_oCacheDocument.SendRepaint();
}

void CAVSDocumentViewer::OnMove()
{
	//OnPaint();
	PostMessage(WM_PAINT, 0, 0);
}

void CAVSDocumentViewer::OnCreate()
{
	CheckZoom();

	CalculateDocumentSize();
	Calculate();
	
	//OnPaint();
	m_oCacheDocument.SendRepaint();
}

void CAVSDocumentViewer::OnDestroy()
{
	APITimerOff();
}

void CAVSDocumentViewer::OnPaint(BOOL bVal)
{
	if (NULL == m_pPixels)
		return;

	PaintPixels();

	HDC hdc = GetDC();
	RECT rc;
	GetClientRect(&rc);

	HDC memdc = CreateCompatibleDC(hdc);
	//HBITMAP hbit = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, m_pPixels->m_hBitmap);
	
	//m_oPainter.Render(memdc, rc, m_pPixels->m_pBits);

	m_oPainter.SetDC(hdc, memdc);
	m_oCacheDocument.Draw(&m_oPainter, m_oRenderingDocument, m_lBorderLeft, m_lBorderTop);

	Gdiplus::Graphics oGraphics(memdc);
	oGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	oGraphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	oGraphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

	Gdiplus::Rect oRect(0, 0, m_lWidth + m_lBorderLeft + m_lBorderRight, m_lBorderTop);
	
	Gdiplus::Color oShadowColor1(0xFF, 0x67, 0x67, 0x67);
	Gdiplus::Color oShadowColor2(0x10, 0x67, 0x67, 0x67);
	Gdiplus::LinearGradientBrush oShadowBrush(oRect, oShadowColor1, oShadowColor2, Gdiplus::LinearGradientModeVertical);

	oGraphics.FillRectangle(&oShadowBrush, oRect);

	oRect.Y = m_lHeight + m_lBorderTop + 2;
	oRect.Height = m_lBorderBottom;

	Gdiplus::LinearGradientBrush oShadowBrush2(oRect, oShadowColor2, oShadowColor1, Gdiplus::LinearGradientModeVertical);
	oGraphics.FillRectangle(&oShadowBrush2, oRect);

	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memdc, 0, 0, SRCCOPY);

	DeleteDC(memdc);
	DeleteObject(hOldBitmap);
	//DeleteObject(hbit);

	ReleaseDC(hdc);
}

void CAVSDocumentViewer::OnPaintDC(HDC hdc)
{
	PaintPixels();

	RECT rc;
	GetClientRect(&rc);

	HDC memdc = CreateCompatibleDC(hdc);
	//HBITMAP hbit = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, m_pPixels->m_hBitmap);
	
	//m_oPainter.Render(memdc, rc, m_pPixels->m_pBits);

	m_oPainter.SetDC(hdc, memdc);
	m_oCacheDocument.Draw(&m_oPainter, m_oRenderingDocument, m_lBorderLeft, m_lBorderTop);

	Gdiplus::Graphics oGraphics(memdc);
	Gdiplus::Rect oRect(0, 0, m_lWidth + m_lBorderLeft + m_lBorderRight, m_lBorderTop);
	
	Gdiplus::Color oShadowColor1(0xFF, 0x67, 0x67, 0x67);
	Gdiplus::Color oShadowColor2(0x10, 0x00, 0x00, 0x00);
	//Gdiplus::SolidBrush oShadowBrush(oShadowColor);
	Gdiplus::LinearGradientBrush oShadowBrush(oRect, oShadowColor1, oShadowColor2, Gdiplus::LinearGradientModeVertical);

	oGraphics.FillRectangle(&oShadowBrush, oRect);

	oRect.Y = m_lHeight + m_lBorderTop + 3;
	oRect.Height = m_lBorderBottom;

	Gdiplus::LinearGradientBrush oShadowBrush2(oRect, oShadowColor2, oShadowColor1, Gdiplus::LinearGradientModeVertical);
	oGraphics.FillRectangle(&oShadowBrush2, oRect);

	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memdc, 0, 0, SRCCOPY);

	DeleteDC(memdc);
	DeleteObject(hOldBitmap);
	//DeleteObject(hbit);
}

void CAVSDocumentViewer::BlitPixels()
{
	HDC hdc	= GetDC();
	BlitPixelsDC(hdc);
	ReleaseDC(hdc);
}

void CAVSDocumentViewer::BlitPixelsDC(HDC hdc)
{
	HDC memdc			= CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(&rc);

	HBITMAP hOldBitmap	= (HBITMAP)SelectObject(memdc, m_pPixels->m_hBitmap);
	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memdc, 0, 0, SRCCOPY);

	DeleteDC(memdc);
	DeleteObject(hOldBitmap);
}

HRESULT CAVSDocumentViewer::OnLButtonDown(long xPos, long yPos)
{
	m_bIsMove = true;

	m_lOldCursorX = xPos;
	m_lOldCursorY = yPos;

	m_dOldPositionHor = m_dPercentHor;
	m_dOldPositionVer = m_dPercentVer;

	m_lOldPositionVer = m_lPositionVer;

	m_dOldPositionVer2 = m_dOldPositionVer;

	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
	SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND_CLOSE)));
	
	return S_OK;
}

HRESULT CAVSDocumentViewer::OnLButtonUp()
{
	m_bIsMove = false;	
	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
	SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));

	ReleaseCapture();
	return S_OK;
}

HRESULT CAVSDocumentViewer::OnMouseMove(long xPos, long yPos)
{
	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
	if (!m_bIsMove)
	{
		if (ActivateHyperlink(xPos, yPos))
		{
			SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_LINK)));
			return S_OK;
		}
		
		SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		return S_OK;
	}

	SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND_CLOSE)));
	SetCapture();
	
	double dChangeHor = (double)(xPos - m_lOldCursorX) * m_dKoefMousePositionHor;
	double dNewPosHor = m_dOldPositionHor - dChangeHor;

	dNewPosHor = min(1.0, max(0.0, dNewPosHor));

	double dChangeVer = (double)(yPos - m_lOldCursorY) * m_dKoefMousePositionVer;
	double dNewPosVer = m_dOldPositionVer - dChangeVer;

	dNewPosVer = min(1.0, max(0.0, dNewPosVer));
	m_dOldPositionVer2 = dNewPosVer;

	m_bIsSendScrollPosition = TRUE;

	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexPositionHor, dNewPosHor);
	m_oPropertyManager.SetProperty(g_lPropertyIndexPositionVer, dNewPosVer);
	
	return S_OK;
}

void CAVSDocumentViewer::OnMouseWhell(LONG lOffset)
{
	if (m_bIsMove)
		return;

	double dChangeVer = (double)(lOffset) * m_dKoefMousePositionVer;
	double dNewPosVer = m_dOldPositionVer2 - dChangeVer;

	dNewPosVer = min(1.0, max(0.0, dNewPosVer));

	m_dOldPositionVer2 = dNewPosVer;
	m_dOldPositionVer  = m_dOldPositionVer2;

	m_bIsSendScrollPosition = TRUE;

	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexPositionVer, dNewPosVer);
}

BOOL CAVSDocumentViewer::UpdatePixels(LONG lSizeX, LONG lSizeY)
{
	CTemporaryCS oCS(&m_oCS);

	LONG lWidth		= m_lWidth + m_lBorderLeft + m_lBorderRight;
	LONG lHeigth	= m_lHeight + m_lBorderTop + m_lBorderBottom;

	if ((lWidth != lSizeX) || (lHeigth != lSizeY) || (NULL == m_pPixels))
	{
		m_lWidth	= lSizeX - m_lBorderLeft - m_lBorderRight;
		m_lHeight	= lSizeY - m_lBorderTop - m_lBorderBottom;

		RELEASEOBJECT(m_pPixels);

		m_pPixels = new NSDocumentViewer::CDIB();
		m_pPixels->Create(lSizeX, lSizeY);

		// создаем для секции рендерер --------------------------------------------------
		RELEASEINTERFACE((m_oPainter.m_pRenderer));

		CoCreateInstance(__uuidof(CAVSGraphicsRenderer), NULL, CLSCTX_ALL, __uuidof(IAVSRenderer), (void**)&m_oPainter.m_pRenderer);
		CAVSGraphicsRenderer* p2DRenderer = (CAVSGraphicsRenderer*)m_oPainter.m_pRenderer;
		//p2DRenderer->SetFontManager(NULL); - сделаем это, когда понадобится

		NSDocumentViewer::CDoubleRect oRect;
		oRect.left		= 0;
		oRect.top		= 0;
		oRect.right		= lSizeX;
		oRect.bottom	= lSizeY;
								
		p2DRenderer->m_oRenderer.Create(m_pPixels->m_pBits, oRect, lSizeX, lSizeY, m_pPixels);
		// ------------------------------------------------------------------------------

		m_oGraphics.Create(m_pPixels->m_pBits, lSizeX, lSizeY, -4 * lSizeX, 0);

		Aggplus::CColor oBack(m_dwBackgroundColor);
		m_oGraphics.Clear(oBack);

		m_oPainter.SetDisplayInfo(m_pPixels->m_pBits, lSizeX, lSizeY);

		return TRUE;
	}

	return FALSE;
}

void CAVSDocumentViewer::OnTimer(void)
{
	_OnTimer();
}

void CAVSDocumentViewer::Calculate(BOOL bIsUseRepaint)
{
	CTemporaryCS oCS (&m_oCS);
	
	m_oCalculateDocument.Clear();

	if (0 == m_oDocument.m_arBlocks.GetCount())
	{
		return;
	}

	// размеры должны быть уже посчитаны

	if (m_lDocumentHeight < m_lHeight)
	{
		// отдельный случай - тут видно все, нужно только 
		// посмотреть на горизонтальное смещение
		m_lPositionVer	= 0;
		m_dPercentVer	= 0;
	}
	if (m_lDocumentWidth < m_lWidth)
	{
		// тогда все видно по горизонтали. 
		// сбросим параметр
		m_dPercentHor = 0;
	}

	// сначала разберемся с вертикалью
	ULONGLONG lWindowWidth	= (ULONGLONG)m_lWidth;
	ULONGLONG lWindowHeight	= (ULONGLONG)m_lHeight;

	ULONGLONG lStart	= lWindowHeight / 2;
	ULONGLONG lEnd		= m_lDocumentHeight - lWindowHeight / 2;

	m_dKoefMousePositionVer = 0;
	if (m_lDocumentHeight > m_lHeight)
	{
		m_dKoefMousePositionVer = 1.0 / double(lEnd - lStart);
	} 

	m_lPositionVer = (ULONGLONG)((lEnd - lStart) * m_dPercentVer);
	ULONGLONG lPosition = (ULONGLONG)(lStart + m_lPositionVer);

	ULONGLONG lStartSector = 0;
	if (lPosition > lStart)
	{
		lStartSector = lPosition - lStart;
	}

	// теперь с горизонталью
	// нужна координата оси симметрии
	ULONGLONG lStartSectorHor	= m_lWidth / 2;
	ULONGLONG lEndSectorHor		= lStartSectorHor;

	LONGLONG lCenterHor			= m_lWidth / 2;
	ULONGLONG lPositionCenter	= m_lWidth / 2;

	m_dKoefMousePositionHor = 0;
	if (m_lDocumentWidth > m_lWidth)
	{
		lEndSectorHor	= m_lDocumentWidth - lStartSectorHor;
		lPositionCenter = m_lDocumentWidth / 2;

		m_dKoefMousePositionHor = 1.0 / double(lEndSectorHor - lStartSectorHor);
	}

	ULONGLONG lPositionHor = (ULONGLONG)(lStartSectorHor + (lEndSectorHor - lStartSectorHor) * m_dPercentHor);
	lCenterHor += (lPositionCenter - lPositionHor);
	//lCenterHor = lPositionCenter - lPositionHor;

	// итак, начало и конец по вертикали нам известны.
	// также известно расположение оси симметрии
	
	// осталось сформировать сами страницы для отрисовки
	//size_t nCountPages = m_oDocument.m_arPages.GetCount();
	size_t nCountPages = m_oDocument.m_arBlocks.GetCount();
	size_t nIndexStart = 0;

	ULONGLONG lCurrentHeight	= 0;
	ULONGLONG lCurrentHeightOld = 0;
	for (; nIndexStart < nCountPages; ++nIndexStart)
	{
		lCurrentHeightOld = lCurrentHeight;
		
		//lCurrentHeight += m_oDocument.m_arPages[nIndexStart]->Height();
		//lCurrentHeight += m_oDocument.m_arPages[nIndexStart]->m_lPixHeight;
		lCurrentHeight += m_oDocument.m_arBlocks[nIndexStart].GetHeight();
		lCurrentHeight += m_lPageBetweenOffset;

		if (lCurrentHeight > lStartSector)
			break;
	}

	LONG lPagesWindowHeight = 0;
	
	// делаем страницы по центру (если документ влезает)
	if (m_lDocumentHeight < m_lHeight)
	{
		lPagesWindowHeight = (LONG)(m_lHeight - m_lDocumentHeight) / 2;
	}
	
	// добавляем первую страницу (посмотрим, нужно ли ее добавлять)
	// может быть от нее нужно только область между ней и следующей страницей

	LONG lIndexCurrentPage = (LONG)(nIndexStart + 1);
	if (nIndexStart < nCountPages)
	{
		/*NSDocumentViewer::CPage* pPage = m_oDocument.m_arPages[nIndexStart];
		
		CDrawingPage oPage(pPage);
		oPage.m_oGlobalBounds.left		= lCenterHor - (double)(pPage->m_lPixWidth) / 2.0;
		oPage.m_oGlobalBounds.right		= lCenterHor + (double)(pPage->m_lPixWidth) / 2.0;

		oPage.m_oGlobalBounds.top		= -(double)(lStartSector - lCurrentHeightOld);
		oPage.m_oGlobalBounds.bottom	= (double)(oPage.m_oGlobalBounds.top + pPage->m_lPixHeight);

		if (oPage.m_oGlobalBounds.bottom > -m_lBorderTop)
		{
			m_arPages.Add(oPage);
			lIndexCurrentPage = nIndexStart;
		}

		lPagesWindowHeight = (LONG)(oPage.m_oGlobalBounds.bottom + m_lPageBetweenOffset);*/

		if (lPagesWindowHeight > 0)
		{
			// значит весь документ / страница влез
			lPagesWindowHeight = m_oDocument.m_arBlocks[nIndexStart].AppendDrawingPages(&m_oCalculateDocument.m_arPages, lCenterHor, 
				(double)(lPagesWindowHeight), (double)m_lBorderTop, m_lPageBetweenOffset);
		}
		else
		{
			lPagesWindowHeight = m_oDocument.m_arBlocks[nIndexStart].AppendDrawingPages(&m_oCalculateDocument.m_arPages, lCenterHor, 
				-(double)(lStartSector - lCurrentHeightOld), (double)m_lBorderTop, m_lPageBetweenOffset);
		}
	}

	if (0 != m_oCalculateDocument.m_arPages.GetCount())
	{
		lIndexCurrentPage = (LONG)nIndexStart;
	}
	
	for (size_t nIndexPage = nIndexStart + 1; nIndexPage < nCountPages; ++nIndexPage)
	{
		if (lPagesWindowHeight > m_lHeight)
			break;

		/*NSDocumentViewer::CPage* pPage = m_oDocument.m_arPages[nIndexPage];
		
		CDrawingPage oPage(pPage);
		oPage.m_oGlobalBounds.left		= lCenterHor - (double)(pPage->m_lPixWidth) / 2.0;
		oPage.m_oGlobalBounds.right		= lCenterHor + (double)(pPage->m_lPixWidth) / 2.0;

		oPage.m_oGlobalBounds.top		= (double)lPagesWindowHeight;
		oPage.m_oGlobalBounds.bottom	= (double)(oPage.m_oGlobalBounds.top + pPage->m_lPixHeight);

		m_arPages.Add(oPage);

		lPagesWindowHeight += (LONG)(m_oDocument.m_arPages[nIndexPage]->m_lPixHeight + m_lPageBetweenOffset);*/

		m_oDocument.m_arBlocks[nIndexPage].AppendDrawingPages(&m_oCalculateDocument.m_arPages, lCenterHor, 
			(double)(lPagesWindowHeight), 0.0, m_lPageBetweenOffset);

		lPagesWindowHeight += (m_oDocument.m_arBlocks[nIndexPage].GetHeight() + m_lPageBetweenOffset);
	}

	// заодно нужно посмотреть какая страница является текущей
	// (т.е. nIndexStart или nIndexStart + 1.
	// определить очень просто - какую страницу больше видно по вертикали
	size_t nDrawPagesCount = m_oCalculateDocument.m_arPages.GetCount();
	if (1 >= nDrawPagesCount)
	{
		if ((NULL != m_pThumbnailsViewer) && (0 != nDrawPagesCount))
		{
			double __dLeft   = 0;
			double __dTop    = 0;
			double __dWidth  = 0;
			double __dHeight = 0;
		
			CDrawingPage* __pPage = &m_oCalculateDocument.m_arPages[0];
			__pPage->GetPercentsVisiblePage(__dLeft, __dTop, __dWidth, __dHeight, m_lWidth, m_lHeight);

			m_pThumbnailsViewer->SetCurrentPageInfo(lIndexCurrentPage, __dLeft, __dTop, __dWidth, __dHeight);
		}
		
		if (m_lCurrentPage != lIndexCurrentPage)
		{
			m_lCurrentPage = lIndexCurrentPage;
			OnDocumentPageChanged(m_lCurrentPage);
		}
	}
	else
	{
		double d1 = m_oCalculateDocument.m_arPages[0].GetPercentVisibleVer(m_lHeight);
		double d2 = m_oCalculateDocument.m_arPages[1].GetPercentVisibleVer(m_lHeight);

		CDrawingPage* __pPage = &m_oCalculateDocument.m_arPages[0];

		if (d2 > d1)
		{
			__pPage = &m_oCalculateDocument.m_arPages[1];
			++lIndexCurrentPage;
		}

		if (NULL != m_pThumbnailsViewer)
		{
			double __dLeft   = 0;
			double __dTop    = 0;
			double __dWidth  = 0;
			double __dHeight = 0;
		
			__pPage->GetPercentsVisiblePage(__dLeft, __dTop, __dWidth, __dHeight, m_lWidth, m_lHeight);
			m_pThumbnailsViewer->SetCurrentPageInfo(lIndexCurrentPage, __dLeft, __dTop, __dWidth, __dHeight);
		}

		if (m_lCurrentPage != lIndexCurrentPage)
		{
			m_lCurrentPage = lIndexCurrentPage;
			OnDocumentPageChanged(m_lCurrentPage);
		}
	}

	// все, все сформировано. Можно делать отрисовку.
	//SyncDrawingRestart();

	if (m_oCalculateDocument == m_oRenderingDocument)
		return;

	m_oRenderingDocument = m_oCalculateDocument;
	if (0 != m_oRenderingDocument.m_arPages.GetCount())
	{
		m_oCacheDocument.Calculate(m_oCalculateDocument, m_oRenderingDocument);
	}

	if (bIsUseRepaint && !m_oCacheDocument.IsRepaintOrWorked())
	{
		// вся отрисовка готова...
		OnPaint();
	}
}

void CAVSDocumentViewer::CalculateDocumentSize()
{
	CTemporaryCS oCS (&m_oCS);
	
	double dScaleX = m_dDpiX / c_ag_Inch_to_MM;
	double dScaleY = m_dDpiY / c_ag_Inch_to_MM;

	dScaleX *= m_dZoom;
	dScaleY *= m_dZoom;

	m_oDocument.CalculatePix(dScaleX, dScaleY);

	m_lDocumentWidth	= m_oDocument.GetWidth(m_lPageBetweenOffset);
	m_lDocumentHeight	= m_oDocument.GetHeight();

	size_t nCountPages = m_oDocument.m_arBlocks.GetCount();
	if (nCountPages > 0)
	{
		m_lDocumentHeight += ((nCountPages - 1) * m_lPageBetweenOffset);
	}

	if (m_dPercentHor != 0)
	{
		// пересчет, чтобы все осталось на своих местах
	}
	if (m_dPercentVer != 0)
	{
		// пересчет, чтобы все осталось на своих местах
	}
	
	double _width	= (double)m_lDocumentWidth  / 1000;
	double _height	= (double)m_lDocumentHeight / 1000;

	OnDocumentSizeChanged(_width, _height, m_dPercentHor, m_dPercentVer);

	// теперь расчитаем вертикальные позиции страниц
	if (m_lDocumentHeight <= m_lHeight)
	{
		for (size_t i = 0; i < nCountPages; ++i)
		{
			//m_oDocument.m_arPages[i]->m_dPositionVer = 0;
			m_oDocument.m_arBlocks[i].SetPositionVer(0);
		}
	}
	else
	{
		ULONGLONG lStart = m_lHeight / 2;
		ULONGLONG lEnd	 = m_lDocumentHeight - lStart;

		ULONGLONG lDist = lEnd - lStart;

		ULONGLONG lMemoryHeight = 0;
		for (size_t i = 0; i < nCountPages; ++i)
		{
			ULONGLONG lPos = lMemoryHeight + lStart;
			if (lPos > lEnd)
				lPos = lEnd;

			/*m_oDocument.m_arPages[i]->m_dPositionVer = (double)(lPos - lStart) / lDist;
			
			lMemoryHeight += m_oDocument.m_arPages[i]->m_lPixHeight;
			lMemoryHeight += m_lPageBetweenOffset;*/

			m_oDocument.m_arBlocks[i].SetPositionVer((double)(lPos - lStart) / lDist);
			
			lMemoryHeight += m_oDocument.m_arBlocks[i].GetHeight();
			lMemoryHeight += m_lPageBetweenOffset;
		}
	}
}


void CAVSDocumentViewer::CheckZoom(BOOL bPutZoom)
{
	CTemporaryCS oCS (&m_oCS);
	
	double dZoom = m_dZoom;
	switch (m_eViewMode)
	{
	case FitWidth:
		{
			//double dPlus	= m_lPageBetweenOffset * c_dInch_to_MM / m_dDpiX;
			
			LONG lOffset	= m_oDocument.GetPixelsBetween(m_lPageBetweenOffset);
			
			double widthMM	= m_oDocument.GetWidthMM(/*dPlus*/0);
			
			if (0 == widthMM)
				return;

			LONG lWidthPix	= (LONG)(m_dDpiX * widthMM  / c_dInch_to_MM);
			dZoom			= (double)(m_lWidth - lOffset) / lWidthPix;
			break;
		}
	case FitHeight:
		{
			double heightMM	= m_oDocument.GetHeightMM();

			if (0 == heightMM)
				return;

			LONG lHeightPix	= (LONG)(m_dDpiY * heightMM  / c_dInch_to_MM);
			dZoom			= (double)m_lHeight / lHeightPix;
			break;
		}
	case FitPage:
		{
			double dPlus	= m_lPageBetweenOffset * c_dInch_to_MM / m_dDpiX;
			
			double widthMM	= m_oDocument.GetWidthMM(dPlus);

			if (0 == widthMM)
				return;

			LONG lWidthPix	= (LONG)(m_dDpiX * widthMM  / c_dInch_to_MM);
			double dZoom1	= (double)m_lWidth / lWidthPix;

			double heightMM	= m_oDocument.GetHeightMM();
			
			if (0 == heightMM)
				return;

			LONG lHeightPix	= (LONG)(m_dDpiY * heightMM  / c_dInch_to_MM);
			double dZoom2	= (double)m_lHeight / lHeightPix;

			dZoom			= min(dZoom1, dZoom2);
			break;
		}
	default:
		break;
	};
	
	if (bPutZoom)
	{
		put_Zoom(dZoom);
	}
	else
	{
		m_dZoom = dZoom;
	}
}

BOOL CAVSDocumentViewer::CheckZoom2()
{
	double dOldZoom = m_dZoom;

	double dZoom = m_dZoom;
	switch (m_eViewMode)
	{
	case FitWidth:
		{
			//double dPlus	= m_lPageBetweenOffset * c_dInch_to_MM / m_dDpiX;
			
			LONG lOffset	= m_oDocument.GetPixelsBetween(m_lPageBetweenOffset);
			
			double widthMM	= m_oDocument.GetWidthMM(/*dPlus*/0);
			LONG lWidthPix	= (LONG)(m_dDpiX * widthMM  / c_dInch_to_MM);

			if (0 != lWidthPix)
			{
				dZoom			= (double)(m_lWidth - lOffset) / lWidthPix;
			}
			break;
		}
	case FitHeight:
		{
			double heightMM	= m_oDocument.GetHeightMM();
			LONG lHeightPix	= (LONG)(m_dDpiY * heightMM  / c_dInch_to_MM);
			
			if (0 != lHeightPix)
			{
				dZoom			= (double)m_lHeight / lHeightPix;
			}
			break;
		}
	case FitPage:
		{
			double dPlus	= m_lPageBetweenOffset * c_dInch_to_MM / m_dDpiX;
			
			double widthMM	= m_oDocument.GetWidthMM(dPlus);
			LONG lWidthPix	= (LONG)(m_dDpiX * widthMM  / c_dInch_to_MM);
			double dZoom1	= (0 != lWidthPix) ? ((double)m_lWidth / lWidthPix) : m_dZoom;

			double heightMM	= m_oDocument.GetHeightMM();
			LONG lHeightPix	= (LONG)(m_dDpiY * heightMM  / c_dInch_to_MM);
			double dZoom2	= (0 != lHeightPix) ? ((double)m_lHeight / lHeightPix) : m_dZoom;

			dZoom			= min(dZoom1, dZoom2);
			break;
		}
	default:
		break;
	};
	
	m_dZoom = dZoom;

	return (m_dZoom != dOldZoom);
}

double CAVSDocumentViewer::GetCurrentZoom()
{
	NSDocumentViewer::ViewMode eMode = m_eViewMode;
	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexViewMode])
	{
		eMode = (NSDocumentViewer::ViewMode)m_oPropertyManager.m_pValues[g_lPropertyIndexViewMode].lVal;
	}
	
	double dZoom = m_dZoom;
	switch (eMode)
	{
	case FitWidth:
		{
			//double dPlus	= m_lPageBetweenOffset * c_dInch_to_MM / m_dDpiX;
			
			LONG lOffset	= m_oDocument.GetPixelsBetween(m_lPageBetweenOffset);
			
			double widthMM	= m_oDocument.GetWidthMM(/*dPlus*/0);
			LONG lWidthPix	= (LONG)(m_dDpiX * widthMM  / c_dInch_to_MM);

			if (0 != lWidthPix)
			{
				dZoom			= (double)(m_lWidth - lOffset) / lWidthPix;
			}
			break;
		}
	case FitHeight:
		{
			double heightMM	= m_oDocument.GetHeightMM();
			LONG lHeightPix	= (LONG)(m_dDpiY * heightMM  / c_dInch_to_MM);
			
			if (0 != lHeightPix)
			{
				dZoom			= (double)m_lHeight / lHeightPix;
			}
			break;
		}
	case FitPage:
		{
			double dPlus	= m_lPageBetweenOffset * c_dInch_to_MM / m_dDpiX;
			
			double widthMM	= m_oDocument.GetWidthMM(dPlus);
			LONG lWidthPix	= (LONG)(m_dDpiX * widthMM  / c_dInch_to_MM);
			double dZoom1	= (0 != lWidthPix) ? ((double)m_lWidth / lWidthPix) : m_dZoom;

			double heightMM	= m_oDocument.GetHeightMM();
			LONG lHeightPix	= (LONG)(m_dDpiY * heightMM  / c_dInch_to_MM);
			double dZoom2	= (0 != lHeightPix) ? ((double)m_lHeight / lHeightPix) : m_dZoom;

			dZoom			= min(dZoom1, dZoom2);
			break;
		}
	default:
		break;
	};

	return dZoom;
}

void CAVSDocumentViewer::PaintPixels()
{
	m_oGraphics.SetClipRect(0, 0, m_lWidth, m_lHeight);

	Aggplus::CColor oColor(m_dwBackgroundColor);
	m_oGraphics.Clear(oColor);

	size_t nCount = m_oCalculateDocument.m_arPages.GetCount();
	for (size_t i = 0; i < nCount; ++i)
	{
		DrawPage(i);
	}
}

void CAVSDocumentViewer::DrawPage(size_t nIndexPage)
{
	Aggplus::CColor oShadowColor(0xFF000000);
	Aggplus::CColor oPageColor(0xFFFFFFFF);

	Aggplus::CBrushSolid oBrush(oShadowColor);
	
	CDrawingPage* pPage = &m_oCalculateDocument.m_arPages[nIndexPage];

	double dLeft	= m_lBorderLeft + pPage->m_oGlobalBounds.left;
	double dTop		= m_lBorderTop + pPage->m_oGlobalBounds.top;
	double dWidth	= pPage->m_oGlobalBounds.GetWidth();
	double dHeight	= pPage->m_oGlobalBounds.GetHeight();

	double dOffsetX = pPage->m_pPage->m_lShadowOffsetHor;
	double dOffsetY = pPage->m_pPage->m_lShadowOffsetVer;

	m_oGraphics.FillRectangle(&oBrush, dLeft + dOffsetX, dTop + dOffsetY, dWidth, dHeight);

	oBrush.SetColor(oPageColor);
	m_oGraphics.FillRectangle(&oBrush, dLeft, dTop, dWidth, dHeight);
}
void CAVSDocumentViewer::AddPage()
{
	CTemporaryCS oCS(&m_oCS);
	
	CPage* pPage = new CPage();
	pPage->m_dWidthMM	= m_dLastPageWidth;
	pPage->m_dHeightMM	= m_dLastPageHeight;

	pPage->SetDpi(m_dDpiX, m_dDpiY);

	pPage->m_pPage = m_pCurrentPage;
	
	m_pCurrentPage = NULL;
	CoCreateInstance(__uuidof(CAVSMetafile), NULL, CLSCTX_INPROC, __uuidof(IAVSMetafile), (void**)&m_pCurrentPage);

	m_oDocument.AddPage(pPage);

	OnDocumentCountPagesChanged((LONG)m_oDocument.GetPagesCount());

	m_oPropertyManager.SetProperty(g_lPropertyIndexAddPage, (LONG)0);

	/*CheckZoom();

	CalculateDocumentSize();
	Calculate();*/
	//PaintPixels();
	//OnPaint();
}

// интерфейс
STDMETHODIMP CAVSDocumentViewer::get_ViewMode(LONG* lMode)
{
	if (NULL == lMode)
		return S_FALSE;

	*lMode = (LONG)m_eViewMode;
	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::put_ViewMode(LONG lMode)
{
	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexViewMode, lMode);

	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_Zoom(double* dZoom)
{
	if (NULL == dZoom)
		return S_FALSE;

	CTemporaryCS oCS( &m_oCS );

	*dZoom = GetCurrentZoom();
	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::put_Zoom(double dZoom)
{
	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexZoom, dZoom);

	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_PositionHor(double* dPositionHor)
{
	if (NULL == dPositionHor)
		return S_FALSE;
	
	*dPositionHor = m_dPercentHor;
	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::put_PositionHor(double dPositionHor)
{
	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexPositionHor, dPositionHor);
		
	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_PositionVer(double* dPositionVer)
{
	if (NULL == dPositionVer)
		return S_FALSE;
	
	*dPositionVer = m_dPercentVer;

	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::put_PositionVer(double dPositionVer)
{
	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexPositionVer, dPositionVer);
	m_bIsSendScrollPosition = FALSE;

	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_ControlWidth(LONG* lWidth)
{
	if (NULL == lWidth)
		return S_FALSE;

	*lWidth = m_lWidth;
	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::get_ControlHeight(LONG* lHeight)
{
	if (NULL == lHeight)
		return S_FALSE;

	*lHeight = m_lHeight;
	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_DocumentWidth(double* dWidth)
{
	if (NULL == dWidth)
		return S_FALSE;
	
	*dWidth = (double)m_lDocumentWidth  / 1000;
	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::get_DocumentHeight(double* dHeight)
{
	if (NULL == dHeight)
		return S_FALSE;
	
	*dHeight = (double)m_lDocumentHeight  / 1000;
	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_CountPages(LONG* lCountPages)
{
	if (NULL == lCountPages)
		return S_FALSE;

	*lCountPages = (LONG)m_oDocument.GetPagesCount();
	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_CurrentPage(LONG* lCurrentPage)
{
	if (NULL == lCurrentPage)
		return S_FALSE;

	*lCurrentPage = m_lCurrentPage;	
	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::put_CurrentPage(LONG lCurrentPage)
{
	CTemporaryCS oCS(&m_oCS);
	
	if ((lCurrentPage < 0) || (lCurrentPage == m_lCurrentPage) || (lCurrentPage >= (LONG)m_oDocument.GetPagesCount()))
		return S_OK;

	//m_oPropertyManager.SetProperty(g_lPropertyIndexCurrentPage, lCurrentPage);

	m_lCurrentPage = lCurrentPage;
	NSDocumentViewer::CPage* pPage = m_oDocument.GetPage(m_lCurrentPage);

	put_PositionVer(pPage->m_dPositionVer);
	m_bIsSendScrollPosition = TRUE;

	if (m_lCurrentPage == lCurrentPage)
	{
		// значит евент не отправился
		OnDocumentPageChanged(m_lCurrentPage);
	}

	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::get_PagesInBlock(LONG* lPages)
{
	if (NULL == lPages)
		return S_FALSE;

	*lPages = m_oDocument.m_lCountPagesInBlock;
	return S_OK;
}
STDMETHODIMP CAVSDocumentViewer::put_PagesInBlock(LONG lPages)
{
	CTemporaryCS oCS(&m_oCS);

	m_oPropertyManager.SetProperty(g_lPropertyIndexPagesInBlock, lPages);

	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::CloseFile()
{
	CTemporaryCS oCS (&m_oCS);

	m_oDocument.CloseFile();

	m_oCalculateDocument.Clear();
	m_oRenderingDocument.Clear();

	m_oCacheDocument.Stop();

	m_dDocumentWidthMM	= 0;
	m_dDocumentHeightMM	= 0;

	m_lDocumentWidth	= 0;
	m_lDocumentHeight	= 0;

	m_dLastPageWidth	= 0;
	m_dLastPageHeight	= 0;

	m_dPercentHor		= 0;
	m_dPercentVer		= 0;

	m_lCurrentPage		= -1;
	
	OnDocumentCountPagesChanged(0);

	m_oSource.SetSource(NULL);	

	m_bIsSetSource = FALSE;

	Calculate();
	OnPaint();
	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::Init()
{
	m_oSource.m_oEvents.m_pSource = this;
	APITimerOn(10);
	return S_OK;
}

STDMETHODIMP CAVSDocumentViewer::_OnTimer()
{
	if (m_bIsDeleted)
		return S_OK;

	CTemporaryCS oCS(&m_oCS_ChangeSource);
	
	BOOL bIsCalculate			= FALSE;
	BOOL bIsCalculateDocSize	= FALSE;
	BOOL bIsCheckZoom			= FALSE;

	BOOL bIsSendEventPosition		= FALSE;
	BOOL bIsSendEventCurrentPage	= FALSE;
	
	LONG lNewWidth  = m_lWidth	+ m_lBorderLeft + m_lBorderRight;
	LONG lNewHeight = m_lHeight	+ m_lBorderTop + m_lBorderBottom;

	BOOL bIsSavePosVer = FALSE;

	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPagesInBlock])
	{
		bool bAttack = m_oDocument.SetCountPagesInBlock(m_oPropertyManager.m_pValues[g_lPropertyIndexPagesInBlock].lVal);

		if (bAttack)
		{
			bIsCheckZoom		= TRUE;

			bIsCalculateDocSize = TRUE;
			bIsCalculate		= TRUE;
		}
		
		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPagesInBlock] = FALSE;
	}
	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexCurrentPage])
	{
		LONG lPageNumber = m_oPropertyManager.m_pValues[g_lPropertyIndexCurrentPage].lVal;
		if (m_lCurrentPage != lPageNumber)
		{
			m_lCurrentPage = lPageNumber;

			bIsCalculate = TRUE;
		}

		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexCurrentPage] = FALSE;
	}
	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexViewMode])
	{
		m_eViewMode = (NSDocumentViewer::ViewMode)m_oPropertyManager.m_pValues[g_lPropertyIndexViewMode].lVal;
		
		bIsCheckZoom = TRUE;
		bIsCalculateDocSize = TRUE;
		bIsCalculate		= TRUE;
		
		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexViewMode] = FALSE;
	}
	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexZoom])
	{
		double dNewZoom = m_oPropertyManager.m_pValues[g_lPropertyIndexZoom].dblVal;

		if (abs(m_dZoom - dNewZoom) > 0.001)
		{
			m_dZoom = dNewZoom;

			bIsCalculateDocSize = TRUE;
			bIsCalculate		= TRUE;
		}

		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexZoom] = FALSE;
	}
	if ((m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPositionHor]) && (++m_lCurrentVerCount == m_lMaxVerCount))
	{
		double dPositionHor = m_oPropertyManager.m_pValues[g_lPropertyIndexPositionHor].dblVal;
		m_lCurrentVerCount	= 0;

		if (m_dPercentHor != dPositionHor)
		{
			m_dPercentHor	= dPositionHor;
			bIsCalculate	= TRUE;

			bIsSendEventPosition	= TRUE;
			m_bIsSendScrollPosition = TRUE;
		}
		
		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPositionHor] = FALSE;
	}
	if ((m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPositionVer]) && (++m_lCurrentHorCount == m_lMaxHorCount))
	{
		double dPositionVer = m_oPropertyManager.m_pValues[g_lPropertyIndexPositionVer].dblVal;
		m_lCurrentHorCount	= 0;

		if (m_dPercentVer != dPositionVer)
		{
			m_dPercentVer	= dPositionVer;
			m_dOldPositionVer2 = m_dPercentVer;

			bIsCalculate	= TRUE;

			bIsSendEventPosition = TRUE;
		}
		
		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPositionVer] = FALSE;
	}
	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexAddPage])
	{
		bIsCheckZoom			= TRUE;
		bIsCalculateDocSize		= TRUE;
		bIsCalculate			= TRUE;
		
		bIsSavePosVer			= (1 == m_oPropertyManager.m_pValues[g_lPropertyIndexAddPage].lVal);
		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexAddPage] = FALSE;

		if (m_bIsSetSource)
		{
			bIsSavePosVer = FALSE;
			m_bIsSetSource = FALSE;
		}
	}

	/*BOOL bSize = UpdatePixels(lNewWidth, lNewHeight);
	if (bSize)
	{
		bIsCheckZoom		= TRUE;
		bIsCalculateDocSize = TRUE;
		bIsCalculate		= TRUE;
	}*/

	if (bIsCheckZoom)
	{
		CheckZoom2();
	}
	if (bIsCalculateDocSize)
	{
		CalculateDocumentSize();
	}

	if (bIsSavePosVer)
	{
		// пересчитаем m_dPositionVer так, чтобы на экране
		// ничего не поменялось. (если только снизу добавилось)

		CTemporaryCS oCS (&m_oCS);
		ULONGLONG lNewDocSize = m_oDocument.GetHeight();
		
		double dOldPercent = m_dPercentVer;
		
		ULONGLONG __lOldHeight = 0;
		if (m_lOldDocumentHeight > m_lHeight)
		{
			__lOldHeight = m_lOldDocumentHeight - m_lHeight;
		}
		ULONGLONG __lNewHeight = 0;
		if (lNewDocSize > m_lHeight)
		{
			__lNewHeight = lNewDocSize - m_lHeight;
		}

		//m_dPercentVer = (double)m_lPositionVer / __lOldHeight;
		
		double dOldULONGPos		= (double)m_dPercentVer * __lOldHeight;
		double dOldOldULONGPOs  = (double)m_dOldPositionVer * __lOldHeight;

		if (0 == lNewDocSize)
		{
			m_dPercentVer = 0.0;
		}
		else
		{
			m_dPercentVer = dOldULONGPos / __lNewHeight;

			// чтобы мышка не отвалилась
			m_dOldPositionVer = dOldOldULONGPOs / __lNewHeight;
			m_dKoefMousePositionVer = 1.0 / __lNewHeight;
		}

		if (dOldPercent != m_dPercentVer)
			bIsSendEventPosition = TRUE;
	}

	if (bIsCalculate)
	{
		Calculate(TRUE);
	}

	if (bIsSendEventPosition && m_bIsSendScrollPosition)
	{
		OnDocumentPositionChanged(m_dPercentHor, m_dPercentVer);
	}
	if (bIsSendEventCurrentPage)
	{
		OnDocumentPageChanged(m_lCurrentPage);
	}

	if (m_oCacheDocument.IsRepaintReset())
	{
		CTemporaryCS oCS(&m_oCS);
		OnPaint();
	}

	//OnPaint();
	return S_OK;
}

BOOL CAVSDocumentViewer::ActivateHyperlink(long x, long y)
{
	CTemporaryCS oCS (&m_oCS);
	double dX = (double)x;
	double dY = (double)y;

	size_t nCount = m_oCalculateDocument.m_arPages.GetCount();
	for (size_t i = 0; i < nCount; ++i)
	{
		CDrawingPage* pPage = &m_oCalculateDocument.m_arPages[i];
		
		if (pPage->m_oGlobalBounds.IsPointInside(x, y))
		{
			double dPositionPixX = (dX - pPage->m_oGlobalBounds.left) / pPage->m_oGlobalBounds.GetWidth();
			double dPositionPixY = (dY - pPage->m_oGlobalBounds.top)  / pPage->m_oGlobalBounds.GetHeight();

			double dPosMM_X = dPositionPixX * c_dInch_to_MM / m_dDpiX;
			double dPosMM_Y = dPositionPixY * c_dInch_to_MM / m_dDpiY;

			return pPage->m_pPage->m_oLinks.IsActivate(dPosMM_X, dPosMM_Y);
		}
	}

	return FALSE;
}

