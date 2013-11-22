#include "stdafx.h"
#include "ViewerThumbnails.h"

#include "TimeMeasurer.h"

LRESULT CAVSThumbnailsViewer::OnMessages(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

			LONG lPage = -1;
			GetPagePointInside((double)xPos, (double)yPos, &lPage);

			LONG lShift = 0;
			if (-1 != lPage)
			{
				lShift += (0xFF0000 + lPage);
			}

			bHandled = TRUE;
			OnMouseDblClick(1, lShift, (double)xPos, (double)yPos);
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
			OnKeyDown((LONG)wParam, (DWORD)lParam);
			OnKeyUp((LONG)wParam, (LONG)lParam, 0.0, 0.0);
			return 0;
		}
	}

	OnMessage(uMsg, wParam, lParam, &bHandled);
	return 0;
}

void CAVSThumbnailsViewer::OnKeyDown(LONG lKey, DWORD dwData)
{
	switch (lKey)
	{
	case VK_LEFT:
	case VK_PRIOR:
		{
			CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

			if (0 == m_lSelectedPage)
			{
				RELEASEOBJECT(pCS);
				return;
			}

			--m_lSelectedPage;
			OnPaint();

			RELEASEOBJECT(pCS);

			if (NULL != m_pDocViewer)
			{
				m_pDocViewer->put_CurrentPage(m_lSelectedPage);
			}

			break;
		}
	case VK_RIGHT:
	case VK_NEXT:
		{
			CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

			if (m_lSelectedPage == ((LONG)m_oDocument.GetPagesCount() - 1))
			{
				RELEASEOBJECT(pCS);
				return;
			}

			++m_lSelectedPage;
			OnPaint();

			RELEASEOBJECT(pCS);

			if (NULL != m_pDocViewer)
			{
				m_pDocViewer->put_CurrentPage(m_lSelectedPage);
			}

			break;
		}
	case VK_UP:
		{
			CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

			double dScaleX = m_dDpiX / c_ag_Inch_to_MM;
			LONG lSizeOneX = (LONG)(m_dZoom * m_dFixWidth * dScaleX);

			LONG lCountLine = m_lWidth / lSizeOneX;

			LONG lNewPage = m_lSelectedPage - lCountLine;

			if (lNewPage >= 0)
			{
				m_lSelectedPage = lNewPage;
				OnPaint();

				RELEASEOBJECT(pCS);

				if (NULL != m_pDocViewer)
				{
					m_pDocViewer->put_CurrentPage(m_lSelectedPage);
				}
			}

			RELEASEOBJECT(pCS);

			break;
		}
	case VK_DOWN:
		{
			CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);

			double dScaleX = m_dDpiX / c_ag_Inch_to_MM;
			LONG lSizeOneX = (LONG)(m_dZoom * m_dFixWidth * dScaleX);

			LONG lCountLine = m_lWidth / lSizeOneX;

			LONG lNewPage = m_lSelectedPage + lCountLine;

			if (lNewPage < (LONG)m_oDocument.GetPagesCount())
			{
				m_lSelectedPage = lNewPage;
				OnPaint();

				RELEASEOBJECT(pCS);

				if (NULL != m_pDocViewer)
				{
					m_pDocViewer->put_CurrentPage(m_lSelectedPage);
				}
			}

			RELEASEOBJECT(pCS);

			break;
		}
	default:
		break;
	}
}

void CAVSThumbnailsViewer::OnSize()
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
}

void CAVSThumbnailsViewer::OnMove()
{
	OnPaint();
}

void CAVSThumbnailsViewer::OnMouseWhell(LONG lOffset)
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

void CAVSThumbnailsViewer::OnCreate()
{
	CheckZoom();

	CalculateDocumentSize();
	Calculate();
	
	OnPaint();
}

void CAVSThumbnailsViewer::OnDestroy()
{
	APITimerOff();
}

void CAVSThumbnailsViewer::OnPaint(BOOL bVal)
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
	
	//m_oPainter.Render(memdc, rc, m_pPixels);

	m_oPainter.SetDC(hdc, memdc);
	m_oCacheDocument.Draw(&m_oPainter, m_oRenderingDocument, m_lBorderLeft, m_lBorderTop);

	// видимая часть страницы
	if (NULL != m_pDocViewer && m_bIsRedBounds)
	{
		size_t nPages = m_oRenderingDocument.m_arPages.GetCount();
		for (size_t nIndex = 0; nIndex < nPages; ++nIndex)
		{
			if (m_lSelectedPage == m_oRenderingDocument.m_arPages[nIndex].m_lIndexInSource)
			{
				NSDocumentViewer::CDoubleRect oRect = m_oRenderingDocument.m_arPages[nIndex].m_oGlobalBounds;

				double dW = oRect.GetWidth();
				double dH = oRect.GetHeight();

				oRect.left += m_dPercentCurrentPageLeft * dW;
				oRect.top  += m_dPercentCurrentPageTop * dH;

				oRect.right  = oRect.left + m_dPercentCurrentPageWidth * dW;
				oRect.bottom = oRect.top + m_dPercentCurrentPageHeight * dH;

				// теперь просто рисуем этот рект на hdc
				Gdiplus::Graphics oGr(memdc);
				Gdiplus::Pen oPen(Gdiplus::Color::Red, 2);
				oGr.DrawRectangle(&oPen, (float)(oRect.left + m_lBorderLeft), (float)(oRect.top + m_lBorderTop), 
					(float)oRect.GetWidth(), (float)oRect.GetHeight());
			}
		}
	}

	Gdiplus::Graphics oGraphics(memdc);
	Gdiplus::Rect oRect(0, 0, m_lWidth + m_lBorderLeft + m_lBorderRight, m_lBorderTop);
	
	Gdiplus::Color oShadowColor1(0xFF, 0x67, 0x67, 0x67);
	Gdiplus::Color oShadowColor2(0x10, 0x00, 0x00, 0x00);
	//Gdiplus::SolidBrush oShadowBrush(oShadowColor);
	Gdiplus::LinearGradientBrush oShadowBrush(oRect, oShadowColor1, oShadowColor2, Gdiplus::LinearGradientModeVertical);

	oGraphics.FillRectangle(&oShadowBrush, oRect);

	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memdc, 0, 0, SRCCOPY);

	DeleteDC(memdc);
	DeleteObject(hOldBitmap);
	//DeleteObject(hbit);

	ReleaseDC(hdc);
}

void CAVSThumbnailsViewer::OnPaintDC(HDC hdc)
{
	if (NULL == m_pPixels)
		return;

	PaintPixels();

	RECT rc;
	GetClientRect(&rc);

	HDC memdc = CreateCompatibleDC(hdc);
	//HBITMAP hbit = CreateCompatibleBitmap(hdc, rc.right - rc.left, rc.bottom - rc.top);
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, m_pPixels->m_hBitmap);
	
	//m_oPainter.Render(memdc, rc, m_pPixels);

	m_oPainter.SetDC(hdc, memdc);
	m_oCacheDocument.Draw(&m_oPainter, m_oRenderingDocument, m_lBorderLeft, m_lBorderTop);

	// видимая часть страницы
	if (NULL != m_pDocViewer && m_bIsRedBounds)
	{
		size_t nPages = m_oRenderingDocument.m_arPages.GetCount();
		for (size_t nIndex = 0; nIndex < nPages; ++nIndex)
		{
			if (m_lSelectedPage == m_oRenderingDocument.m_arPages[nIndex].m_lIndexInSource)
			{
				NSDocumentViewer::CDoubleRect oRect = m_oRenderingDocument.m_arPages[nIndex].m_oGlobalBounds;

				double dW = oRect.GetWidth();
				double dH = oRect.GetHeight();

				oRect.left += m_dPercentCurrentPageLeft * dW;
				oRect.top  += m_dPercentCurrentPageTop * dH;

				oRect.right  = oRect.left + m_dPercentCurrentPageWidth * dW;
				oRect.bottom = oRect.top + m_dPercentCurrentPageHeight * dH;

				// теперь просто рисуем этот рект на hdc
				Gdiplus::Graphics oGr(memdc);
				Gdiplus::Pen oPen(Gdiplus::Color::Red, 2);
				oGr.DrawRectangle(&oPen, (float)(oRect.left + m_lBorderLeft), (float)(oRect.top + m_lBorderTop), 
					(float)oRect.GetWidth(), (float)oRect.GetHeight());
			}
		}
	}

	Gdiplus::Graphics oGraphics(memdc);
	oGraphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
	oGraphics.SetCompositingQuality(Gdiplus::CompositingQualityHighQuality);
	oGraphics.SetPixelOffsetMode(Gdiplus::PixelOffsetModeHighQuality);

	Gdiplus::Rect oRect(0, 0, m_lWidth + m_lBorderLeft + m_lBorderRight, m_lBorderTop);
	
	Gdiplus::Color oShadowColor1(0xFF, 0x67, 0x67, 0x67);
	Gdiplus::Color oShadowColor2(0x10, 0x67, 0x67, 0x67);
	Gdiplus::LinearGradientBrush oShadowBrush(oRect, oShadowColor1, oShadowColor2, Gdiplus::LinearGradientModeVertical);

	oGraphics.FillRectangle(&oShadowBrush, oRect);

	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memdc, 0, 0, SRCCOPY);

	DeleteDC(memdc);
	DeleteObject(hOldBitmap);
	//DeleteObject(hbit);
}


HRESULT CAVSThumbnailsViewer::OnLButtonDown(long xPos, long yPos)
{
	m_bIsMove = true;

	m_lOldCursorX = xPos;
	m_lOldCursorY = yPos;

	m_dOldPositionHor = m_dPercentHor;
	m_dOldPositionVer = m_dPercentVer;

	m_dOldPositionVer2 = m_dOldPositionVer;

	// другая обработка
	CTemporaryCS oCS (&m_oCS);
	size_t nPagesCount = m_oCalculateDocument.m_arPages.GetCount();

	for (size_t i = 0; i < nPagesCount; ++i)
	{
		if (m_oCalculateDocument.m_arPages[i].m_oGlobalBounds.IsPointInside(xPos, yPos))
		{
			m_lSelectedPage = m_oCalculateDocument.m_arPages[i].m_lIndexInSource;
			OnPaint();

			if (NULL != m_pDocViewer)
			{
				m_pDocViewer->put_CurrentPage(m_lSelectedPage);
			}
		}
	}

	return S_OK;

	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
	SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND_CLOSE)));
	
	return S_OK;
}

HRESULT CAVSThumbnailsViewer::OnLButtonUp()
{
	m_bIsMove = false;	
	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
	SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
	return S_OK;
}

HRESULT CAVSThumbnailsViewer::OnMouseMove(long xPos, long yPos)
{
	// никаких тут движений
	return S_OK;
	
	HINSTANCE hInst = _AtlBaseModule.GetModuleInstance();
	if (!m_bIsMove)
	{
		SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND)));
		return S_OK;
	}

	SetCursor(LoadCursor(hInst, MAKEINTRESOURCE(IDC_CURSOR_HAND_CLOSE)));

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

BOOL CAVSThumbnailsViewer::UpdatePixels(LONG lSizeX, LONG lSizeY)
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

		// как научимся рисовать по формату текст - убрать весь гди ---------------------
		RELEASEOBJECT(m_pGdiImage);
		RELEASEOBJECT(m_pGdiGraphics);

		BYTE* pGdiPixels = m_pPixels->m_pBits + (4 * lSizeX * (lSizeY - 1));

		m_pGdiImage = new Gdiplus::Bitmap(lSizeX, lSizeY, -4 * lSizeX, PixelFormat32bppARGB, pGdiPixels);
		m_pGdiGraphics = new Gdiplus::Graphics(m_pGdiImage);
		// ------------------------------------------------------------------------------

		m_oGraphics.Create(m_pPixels->m_pBits, lSizeX, lSizeY, -4 * lSizeX, 0);

		Aggplus::CColor oBack(m_dwBackgroundColor);
		m_oGraphics.Clear(oBack);

		m_oPainter.SetDisplayInfo(m_pPixels->m_pBits, lSizeX, lSizeY);

		return TRUE;
	}

	return FALSE;
}

void CAVSThumbnailsViewer::OnTimer(void)
{
	_OnTimer();
}

void CAVSThumbnailsViewer::Calculate()
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
		m_dPercentVer = 0;
	}
	if (m_lDocumentWidth < m_lWidth)
	{
		// тогда все видно по горизонтали. 
		// сбросим параметр
		m_dPercentHor = 0;
	}

	ULONGLONG lWindowWidth	= (ULONGLONG)m_lWidth;
	ULONGLONG lWindowHeight	= (ULONGLONG)m_lHeight;

	ULONGLONG lStartSectorVer = 0;
	if (m_lDocumentHeight > lWindowHeight)
	{
		lStartSectorVer = (ULONGLONG)((m_lDocumentHeight - lWindowHeight) * m_dPercentVer);
		m_dKoefMousePositionVer = 1.0 / double(m_lDocumentHeight - lWindowHeight);
	}

	ULONGLONG lStartSectorHor = 0;
	if (m_lDocumentWidth > lWindowWidth)
	{
		lStartSectorHor = (ULONGLONG)((m_lDocumentWidth - lWindowWidth) * m_dPercentVer);
		m_dKoefMousePositionHor = 1.0 / double(m_lDocumentWidth - lWindowWidth);
	}

	double dScaleX = m_dDpiX / c_ag_Inch_to_MM;
	double dScaleY = m_dDpiY / c_ag_Inch_to_MM;

	LONG lSizeOneX = (LONG)(m_dZoom * m_dFixWidth * dScaleX);
	LONG lSizeOneY = (LONG)(m_dZoom * m_dFixHeight * dScaleY);

	LONG lCountLine = m_lWidth / lSizeOneX;
	
	double dStartLineX	= 0;
	double dBetweenLine	= 0;

	if (lCountLine > 0)
	{
		dBetweenLine = (double)m_lWidth - lSizeOneX * lCountLine;
		dBetweenLine = dBetweenLine / (lCountLine + 1);
	}
	else
	{
		m_lDocumentWidth = lSizeOneX;
		lCountLine = 1;
	}
	dStartLineX = dBetweenLine;

	LONG lPagesCount = (LONG)m_oDocument.GetPagesCount();
	LONG lLinesCount = (lPagesCount + lCountLine - 1) / lCountLine;

	LONG lStartLine = (LONG)(lStartSectorVer / lSizeOneY);
	double dTop = lSizeOneY * lStartLine - (double)lStartSectorVer;

	LONG lPageFirst = lStartLine * lCountLine;

	double dBorderThumbnailPixX = m_dDpiX * m_dBorderThumbnail / c_dInch_to_MM;
	double dBorderThumbnailPixY = m_dDpiY * m_dBorderThumbnail / c_dInch_to_MM;
	double dHeightTitle			= m_dDpiY * m_dHeightTitle / c_dInch_to_MM;

	for (LONG lIndexLine = lStartLine; lIndexLine < lLinesCount; ++lIndexLine)
	{
		double dBottom = dTop + lSizeOneY;

		double dLeft = dStartLineX;

		for (LONG i = 0; ((i < lCountLine) && (lPageFirst < lPagesCount)); ++i, ++lPageFirst)
		{
			CPage* pPage = m_oDocument.GetPage(lPageFirst);

			CDrawingPage oPage(pPage);
			oPage.m_oPageBounds.left		= dLeft + dBorderThumbnailPixX;
			oPage.m_oPageBounds.right		= dLeft + lSizeOneX - dBorderThumbnailPixX;

			oPage.m_oPageBounds.top			= dTop + dBorderThumbnailPixY;
			oPage.m_oPageBounds.bottom		= dBottom - dHeightTitle;

			oPage.m_lIndexInSource			= lPageFirst;

			oPage.Inscribed();

			dLeft += (dBetweenLine + lSizeOneX);

			m_oCalculateDocument.m_arPages.Add(oPage);
		}


		dTop = dBottom;
		if (dTop >= m_lHeight)
			break;
	}

	// заодно нужно посмотреть какая страница является текущей
	// (т.е. nIndexStart или nIndexStart + 1.
	// определить очень просто - какую страницу больше видно по вертикали
	//size_t nDrawPagesCount = m_oCalculateDocument.m_arPages.GetCount();
	//if (1 >= nDrawPagesCount)
	//{
	//	if (m_lCurrentPage != lIndexCurrentPage)
	//	{
	//		m_lCurrentPage = lIndexCurrentPage;
	//		OnDocumentPageChanged(m_lCurrentPage);
	//	}
	//}
	//else
	//{
	//	double d1 = m_oCalculateDocument.m_arPages[0].GetPercentVisibleVer(m_lHeight);
	//	double d2 = m_oCalculateDocument.m_arPages[1].GetPercentVisibleVer(m_lHeight);

	//	if (d2 > d1)
	//		++lIndexCurrentPage;

	//	if (m_lCurrentPage != lIndexCurrentPage)
	//	{
	//		m_lCurrentPage = lIndexCurrentPage;
	//		OnDocumentPageChanged(m_lCurrentPage);
	//	}
	//}

	if (m_oCalculateDocument == m_oRenderingDocument)
		return;

	m_oRenderingDocument = m_oCalculateDocument;
	if (0 != m_oRenderingDocument.m_arPages.GetCount())
	{
		m_oCacheDocument.Calculate(m_oCalculateDocument, m_oRenderingDocument);
	}

	if (!m_oCacheDocument.IsRepaint())
	{
		// вся отрисовка готова...
		OnPaint();
	}
}

void CAVSThumbnailsViewer::CalculateDocumentSize()
{
	CTemporaryCS oCS (&m_oCS);
	
	double dScaleX = m_dDpiX / c_ag_Inch_to_MM;
	double dScaleY = m_dDpiY / c_ag_Inch_to_MM;

	LONG lSizeOneX = (LONG)(m_dZoom * m_dFixWidth * dScaleX);
	LONG lSizeOneY = (LONG)(m_dZoom * m_dFixHeight * dScaleY);

	LONG lCountLine = m_lWidth / lSizeOneX;
	
	double dStartLineX	= 0;
	double dBetweenLine	= 0;

	m_lDocumentWidth  = m_lWidth; 

	if (lCountLine > 0)
	{
		dBetweenLine = (double)m_lWidth - lSizeOneX * lCountLine;
		dBetweenLine = dBetweenLine / (lCountLine + 1);
	}
	else
	{
		m_lDocumentWidth = lSizeOneX;
		lCountLine = 1;
	}
	dStartLineX = dBetweenLine;

	LONG lPagesCount = (LONG)m_oDocument.GetPagesCount();
	LONG lLinesCount = (lPagesCount + lCountLine - 1) / lCountLine;

	m_lDocumentHeight = lLinesCount * lSizeOneY;

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
	size_t nCountBlocks = m_oDocument.m_arBlocks.GetCount();
	if (m_lDocumentHeight <= m_lHeight)
	{
		for (size_t i = 0; i < nCountBlocks; ++i)
		{
			//m_oDocument.m_arPages[i]->m_dPositionVer = 0;
			m_oDocument.m_arBlocks[i].SetPositionVer(0);
			m_oDocument.m_arBlocks[i].SetPositionVer2(0);
		}
	}
	else
	{
		ULONGLONG lStart = m_lHeight / 2;
		ULONGLONG lEnd	 = m_lDocumentHeight - lStart;

		ULONGLONG lDist = lEnd - lStart;

		ULONGLONG lMemoryHeight = 0;

		double dPosVerPlus = max(0, (double)(m_lHeight - lSizeOneY));
		if (dPosVerPlus > 0)
		{
			dPosVerPlus /= (m_lDocumentHeight - m_lHeight);
		}

		/*for (size_t i = 0; i < nCountBlocks; ++i)
		{
			ULONGLONG lPos = lMemoryHeight + lStart;
			if (lPos > lEnd)
				lPos = lEnd;

			double dPositionVer = (double)(lPos - lStart) / lDist;
			
			m_oDocument.m_arBlocks[i].SetPositionVer(dPositionVer);
			m_oDocument.m_arBlocks[i].SetPositionVer2(max(0, min(1, dPositionVer - dPosVerPlus)));
			
			lMemoryHeight += lSizeOneY;
		}*/

		LONG lBlock = 0;
		LONG lBlocks = (LONG)nCountBlocks;
		for (LONG j = 0; j < lLinesCount; ++j)
		{
			ULONGLONG lPos = lMemoryHeight + lStart;
			if (lPos > lEnd)
				lPos = lEnd;

			double dPositionVer  = (double)(lPos - lStart) / lDist;
			double dPositionVer2 = max(0, min(1, dPositionVer - dPosVerPlus));

			if (dPositionVer == 1.0)
				dPositionVer2 = 1.0;
			
			for (LONG i = 0; (i < lCountLine) && (lBlock < lBlocks); ++i, ++lBlock)
			{
				m_oDocument.m_arBlocks[lBlock].SetPositionVer(dPositionVer);
				m_oDocument.m_arBlocks[lBlock].SetPositionVer2(dPositionVer2);
			}
			
			lMemoryHeight += lSizeOneY;

			if (lMemoryHeight > lEnd)
				lMemoryHeight = lEnd;
		}
	}
}


void CAVSThumbnailsViewer::CheckZoom(BOOL bPutZoom)
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

BOOL CAVSThumbnailsViewer::CheckZoom2()
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

void CAVSThumbnailsViewer::PaintPixels()
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

void CAVSThumbnailsViewer::BlitPixels()
{
	HDC hdc	= GetDC();
	BlitPixelsDC(hdc);
	ReleaseDC(hdc);
}

void CAVSThumbnailsViewer::BlitPixelsDC(HDC hdc)
{
	HDC memdc			= CreateCompatibleDC(hdc);

	RECT rc;
	GetClientRect(&rc);

	HBITMAP hOldBitmap	= (HBITMAP)SelectObject(memdc, m_pPixels->m_hBitmap);
	BitBlt(hdc, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, memdc, 0, 0, SRCCOPY);

	DeleteDC(memdc);
	DeleteObject(hOldBitmap);
}

void CAVSThumbnailsViewer::DrawPage(size_t nIndexPage)
{
	Aggplus::CColor oShadowColor(0xFF000000);
	Aggplus::CColor oPageColor(0xFFFFFFFF);

	Aggplus::CBrushSolid oBrush(oShadowColor);
	
	CDrawingPage* pPage = &m_oCalculateDocument.m_arPages[nIndexPage];

	double dBorderThumbnailPixX = m_dDpiX * m_dBorderThumbnail / c_dInch_to_MM;
	double dBorderThumbnailPixY = m_dDpiY * m_dBorderThumbnail / c_dInch_to_MM;
	double dHeightTitle			= m_dDpiY * m_dHeightTitle / c_dInch_to_MM;

	if (pPage->m_lIndexInSource == m_lSelectedPage)
	{
		Aggplus::CColor oSelectedColor(m_dwSelectedColor);
		Aggplus::CBrushSolid oActive(oSelectedColor);

		m_oGraphics.FillRectangle(&oActive, m_lBorderLeft + pPage->m_oPageBounds.left - dBorderThumbnailPixX, 
											m_lBorderTop + pPage->m_oPageBounds.top - dBorderThumbnailPixY, 
											pPage->m_oPageBounds.GetWidth() + 2 * dBorderThumbnailPixX, 
											pPage->m_oPageBounds.GetHeight() + dBorderThumbnailPixY + dHeightTitle);
	}

	double dLeft	= m_lBorderLeft + pPage->m_oGlobalBounds.left;
	double dTop		= m_lBorderTop + pPage->m_oGlobalBounds.top;
	double dWidth	= pPage->m_oGlobalBounds.GetWidth();
	double dHeight	= pPage->m_oGlobalBounds.GetHeight();

	double dOffsetX = 1.5;//pPage->m_pPage->m_lShadowOffsetHor;
	double dOffsetY = 1.5;//pPage->m_pPage->m_lShadowOffsetVer;

	m_oGraphics.FillRectangle(&oBrush, dLeft + dOffsetX, dTop + dOffsetY, dWidth, dHeight);

	double dHeightPix = m_dDpiY * m_dHeightTitle / c_dInch_to_MM;	
	Gdiplus::RectF oRectText((float)pPage->m_oPageBounds.left, (float)(pPage->m_oPageBounds.top + pPage->m_oPageBounds.GetHeight() + 2), 
								(float)pPage->m_oPageBounds.GetWidth(), (float)dHeightPix);
	
	Gdiplus::SolidBrush oSolidBrush(Gdiplus::Color::Black);
	
	CString strTitle = _T("");
	//strTitle.Format(_T("Page %d"), pPage->m_lIndexInSource + 1);
	strTitle.Format(_T("%d"), pPage->m_lIndexInSource + 1);

	BSTR bsText = strTitle.AllocSysString();	
	m_pGdiGraphics->DrawString(bsText, strTitle.GetLength(), m_pGdiFont, oRectText, &m_oGdiStringFormat, &oSolidBrush);
	SysFreeString(bsText);

	oBrush.SetColor(oPageColor);
	m_oGraphics.FillRectangle(&oBrush, dLeft, dTop, dWidth, dHeight);
	
}
void CAVSThumbnailsViewer::AddPage()
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
STDMETHODIMP CAVSThumbnailsViewer::get_Zoom(double* dZoom)
{
	if (NULL == dZoom)
		return S_FALSE;

	*dZoom = m_dZoom;
	return S_OK;
}
STDMETHODIMP CAVSThumbnailsViewer::put_Zoom(double dZoom)
{
	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexZoom, dZoom);

	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::get_PositionHor(double* dPositionHor)
{
	if (NULL == dPositionHor)
		return S_FALSE;
	
	*dPositionHor = m_dPercentHor;
	return S_OK;
}
STDMETHODIMP CAVSThumbnailsViewer::put_PositionHor(double dPositionHor)
{
	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexPositionHor, dPositionHor);
		
	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::get_PositionVer(double* dPositionVer)
{
	if (NULL == dPositionVer)
		return S_FALSE;
	
	*dPositionVer = m_dPercentVer;

	return S_OK;
}
STDMETHODIMP CAVSThumbnailsViewer::put_PositionVer(double dPositionVer)
{
	CTemporaryCS oCS(&m_oCS);
	m_oPropertyManager.SetProperty(g_lPropertyIndexPositionVer, dPositionVer);
	m_bIsSendScrollPosition = FALSE;

	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::get_ControlWidth(LONG* lWidth)
{
	if (NULL == lWidth)
		return S_FALSE;

	*lWidth = m_lWidth;
	return S_OK;
}
STDMETHODIMP CAVSThumbnailsViewer::get_ControlHeight(LONG* lHeight)
{
	if (NULL == lHeight)
		return S_FALSE;

	*lHeight = m_lHeight;
	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::get_DocumentWidth(double* dWidth)
{
	if (NULL == dWidth)
		return S_FALSE;
	
	*dWidth = (double)m_lDocumentWidth  / 1000;
	return S_OK;
}
STDMETHODIMP CAVSThumbnailsViewer::get_DocumentHeight(double* dHeight)
{
	if (NULL == dHeight)
		return S_FALSE;
	
	*dHeight = (double)m_lDocumentHeight  / 1000;
	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::get_CountPages(LONG* lCountPages)
{
	if (NULL == lCountPages)
		return S_FALSE;

	*lCountPages = (LONG)m_oDocument.GetPagesCount();
	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::get_CurrentPage(LONG* lCurrentPage)
{
	if (NULL == lCurrentPage)
		return S_FALSE;

	*lCurrentPage = m_lCurrentPage;	
	return S_OK;
}
STDMETHODIMP CAVSThumbnailsViewer::put_CurrentPage(LONG lCurrentPage)
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

STDMETHODIMP CAVSThumbnailsViewer::CloseFile()
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

	Calculate();
	OnPaint();
	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::Init()
{
	m_oSource.m_oEvents.m_pSource = this;
	APITimerOn(40);
	return S_OK;
}

STDMETHODIMP CAVSThumbnailsViewer::_OnTimer()
{
	if (m_bIsDeleted)
		return S_OK;
	
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
	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPositionHor])
	{
		double dPositionHor = m_oPropertyManager.m_pValues[g_lPropertyIndexPositionHor].dblVal;

		if (m_dPercentHor != dPositionHor)
		{
			m_dPercentHor	= dPositionHor;
			bIsCalculate	= TRUE;

			bIsSendEventPosition = TRUE;
		}
		
		m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPositionHor] = FALSE;
	}
	if (m_oPropertyManager.m_pIsChanged[g_lPropertyIndexPositionVer])
	{
		double dPositionVer = m_oPropertyManager.m_pValues[g_lPropertyIndexPositionVer].dblVal;

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
		Calculate();
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
		CTemporaryCS oCS (&m_oCS);
		OnPaint();
		//SendMessage(WM_PAINT, 0, 0);
		//PostMessage(WM_PAINT, 0, 0);
	}

	//OnPaint();
	return S_OK;
}

