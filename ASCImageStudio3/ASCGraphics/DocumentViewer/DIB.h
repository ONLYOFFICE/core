#pragma once 
#include "GdiplusEx.h"
#include "math.h"
#include "..\Interfaces\ASCRenderer.h"

namespace NSDocumentViewer
{
	const double c_dInch_to_MM					= 25.4;

	enum ViewMode
	{
		FitPage			= 0,			
		FitWidth		= 1,			
		FitHeight		= 2,			
		Custom			= 3
	};

	class CDoublePoint
	{
	public:
		double x;
		double y;
	public:
		CDoublePoint()
		{
			x = 0;
			y = 0;
		}
		CDoublePoint(double dx, double dy)
		{
			x = dx;
			y = dy;
		}
		CDoublePoint& operator=(const CDoublePoint& oSrc)
		{
			x = oSrc.x;
			y = oSrc.y;

			return *this;
		}
		CDoublePoint(const CDoublePoint& oSrc)
		{
			*this = oSrc;
		}
	};

	class CDoubleRect
	{
	public:
		double left;
		double top;
		double right;
		double bottom;

	public:
		CDoubleRect()
		{
			left	= 0;
			top		= 0;
			right	= 0;
			bottom	= 0;
		}
		CDoubleRect& operator=(const CDoubleRect& oSrc)
		{
			left	= oSrc.left;
			top		= oSrc.top;
			right	= oSrc.right;
			bottom	= oSrc.bottom;

			return *this;
		}
		CDoubleRect(const CDoubleRect& oSrc)
		{
			*this = oSrc;
		}

		inline double GetWidth() const
		{
			return right - left;
		}
		inline double GetHeight() const
		{
			return bottom - top;
		}

		inline void Offset(double dX, double dY)
		{
			left	+= dX;
			top		+= dY;
			right	+= dX;
			bottom	+= dY;
		}

		inline double GetCentreX() const
		{
			return (left + right) / 2.0;
		}

		inline double GetCentreY() const
		{
			return (top + bottom) / 2.0;
		}

		inline BOOL IsPointInside(const CDoublePoint& oPoint)
		{
			return IsPointInside(oPoint.x, oPoint.y);
		}
		BOOL IsPointInside(const double& pointX, const double& pointY)
		{
			return ((left <= pointX) && (right >= pointX) &&
					(top <= pointY) && (bottom >= pointY));
		}
	};

	class CPainter
	{
	public:
		BITMAPINFO m_infoDC;

		LONG m_lDisplayWidth;
		LONG m_lDisplayHeight;

		BYTE* m_pPixels;
		
		HDC m_hDC;
		HDC m_hMemDC;

		IAVSRenderer* m_pRenderer;
		double m_dKoefPixToMM_X;
		double m_dKoefPixToMM_Y;

	public:
		CPainter()
		{
			memset(&m_infoDC, 0, sizeof(BITMAPINFO));
			m_infoDC.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
			//m_infoDC.bmiHeader.biWidth		= 0;
			//m_infoDC.bmiHeader.biHeight		= 0;
			m_infoDC.bmiHeader.biPlanes			= 1;
			m_infoDC.bmiHeader.biBitCount		= 32;
			m_infoDC.bmiHeader.biCompression	= BI_RGB;
			//m_infoDC.bmiHeader.biSizeImage	= 0 * 0 * 4;

			m_pRenderer = NULL;
		}
		~CPainter()
		{
			RELEASEINTERFACE(m_pRenderer);
		}
		
	public:
		void SetDisplayInfo(BYTE* pPixels, LONG lWidth, LONG lHeight)
		{
			m_pPixels = pPixels;
			m_lDisplayWidth  = lWidth;
			m_lDisplayHeight = lHeight;
		}
		void SetDpi(double dDpiX, double dDpiY);
		void SetDC(HDC hDC, HDC hMemDC)
		{
			m_hDC		= hDC;
			m_hMemDC	= hMemDC;
		}

	public:
		inline void SetSize(const LONG& lWidth, const LONG& lHeight)
		{
			m_infoDC.bmiHeader.biWidth			= lWidth;
			m_infoDC.bmiHeader.biHeight			= lHeight;

			m_infoDC.bmiHeader.biSizeImage		= lWidth * lHeight * 4;
		}

		inline void Render(HDC hDC, RECT rect, BYTE* pBuffer)
		{
			LONG lWidth		= rect.right - rect.left;
			LONG lHeight	= rect.bottom - rect.top;
			
			SetSize(lWidth, lHeight);
			SetDIBitsToDevice(hDC, rect.left, rect.top, lWidth, lHeight, 0, 0, 0, lHeight,
				(void*)pBuffer, &m_infoDC, DIB_RGB_COLORS);
		}
		inline void RenderStretch(HDC hDC, RECT rect, BYTE* pBufferBGRA, LONG lSrcWidth, LONG lSrcHeight, LONG lSrcX = 0, LONG lSrcY = 0)
		{
			//LONG lWidth	= rect.right - rect.left;
			//LONG lHeight	= rect.bottom - rect.top;

			SetSize(lSrcWidth, lSrcHeight);			
			
			StretchDIBits(hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
				lSrcX, lSrcY, lSrcWidth, lSrcHeight, pBufferBGRA, &m_infoDC, DIB_RGB_COLORS, SRCCOPY);
		}

	public:

		void BlitPixels()
		{
			SetSize(m_lDisplayWidth, m_lDisplayHeight);
			SetDIBitsToDevice(m_hDC, 0, 0, m_lDisplayWidth, m_lDisplayHeight, 0, 0, 0, m_lDisplayHeight,
				(void*)m_pPixels, &m_infoDC, DIB_RGB_COLORS);
		}
		void BlitPage(BYTE* pPixelsPage, RECT rect, LONG lSrcWidth, LONG lSrcHeight)
		{
			LONG lDrawWidth  = rect.right - rect.left;
			LONG lDrawHeight = rect.bottom - rect.top;

			if ((lDrawWidth == lSrcWidth) && (lDrawHeight == lSrcHeight))
			{
				// просто копируем пикселы
				LONG leftBuffer  = 0;
				LONG leftDisplay = rect.left;
				if (rect.left < 0)
				{
					leftBuffer  = -leftDisplay;
					leftDisplay = 0;
				}
				
				LONG topBuffer  = 0;
				LONG topDisplay = rect.top;
				if (rect.top < 0)
				{
					topBuffer  = -topDisplay;
					topDisplay = 0;
				}

				LONG bottomDisplay = min(rect.bottom, m_lDisplayHeight - 1);
				LONG lCopy = 4 * min(m_lDisplayWidth - leftDisplay, lSrcWidth);
				
				//BYTE* pDisplayMem = m_pPixels    + 4 * (leftDisplay + topDisplay * m_lDisplayWidth);
				BYTE* pDisplayMem = m_pPixels + 4 * (m_lDisplayHeight - 1) * m_lDisplayWidth;
				pDisplayMem +=  4 * (leftDisplay - topDisplay * m_lDisplayWidth);
				//BYTE* pBufferMem  = pPixelsPage  + 4 * (leftBuffer  + topBuffer  * lSrcWidth);
				BYTE* pBufferMem  = pPixelsPage  + 4 * (lSrcHeight - 1) * lSrcWidth;
				pBufferMem  += 4 * (leftBuffer  - topBuffer  * lSrcWidth);

				
				for (LONG i = topDisplay; i < bottomDisplay; ++i)
				{
					memcpy(pDisplayMem, pBufferMem, lCopy);

					pDisplayMem -= 4 * m_lDisplayWidth;
					pBufferMem  -= 4 * lSrcWidth;
				}
			}
			else
			{
				// нужно растягивать. сделаем через гди+
				Gdiplus::Bitmap oDisplayImage(m_lDisplayWidth, m_lDisplayHeight, 4 * m_lDisplayWidth, PixelFormat32bppARGB, m_pPixels);
				Gdiplus::Bitmap oBufferImage(lSrcWidth, lSrcHeight, 4 * lSrcWidth, PixelFormat32bppARGB, pPixelsPage);

				Gdiplus::Graphics oRender(&oDisplayImage);
				oRender.DrawImage(&oBufferImage, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
			}
		}

		void BlitPage2(BYTE* pPixelsPage, RECT rect, LONG lSrcWidth, LONG lSrcHeight)
		{
			SetSize(lSrcWidth, lSrcHeight);			
			
			SetStretchBltMode(m_hDC, COLORONCOLOR);
			StretchDIBits(m_hDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
				0, 0, lSrcWidth, lSrcHeight, pPixelsPage, &m_infoDC, DIB_RGB_COLORS, SRCCOPY);
		}

		void BlitPage3(BYTE* pPixelsPage, RECT rect, LONG lSrcWidth, LONG lSrcHeight)
		{
			SetSize(lSrcWidth, lSrcHeight);			
			
			SetStretchBltMode(m_hMemDC, COLORONCOLOR);
			StretchDIBits(m_hMemDC, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
				0, 0, lSrcWidth, lSrcHeight, pPixelsPage, &m_infoDC, DIB_RGB_COLORS, SRCCOPY);
		}
	};

	
	class CDIB
	{
	public:
		BYTE* m_pBits;
		LONG m_lWidth;
		LONG m_lHeight;

		HBITMAP m_hBitmap;
	
	public:
		
		HANDLE m_hFile;
		HANDLE m_hSection;

		BITMAPINFO* m_pInfo;

	public:
		CDIB() : m_hBitmap(NULL), m_hFile(NULL), m_hSection(NULL)
		{
			UINT nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 0;
			m_pInfo = (LPBITMAPINFO)malloc(nSize);

			ZeroMemory(&m_pInfo->bmiHeader, sizeof(m_pInfo->bmiHeader));

			m_pInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
			m_pInfo->bmiHeader.biWidth			= 0;
			m_pInfo->bmiHeader.biHeight			= 0;
			m_pInfo->bmiHeader.biBitCount		= 32;
			m_pInfo->bmiHeader.biCompression	= BI_RGB;
			m_pInfo->bmiHeader.biClrUsed		= 0;
			m_pInfo->bmiHeader.biPlanes			= 1;
			m_pInfo->bmiHeader.biSizeImage		= 0;

			m_pBits		= NULL;
			m_lWidth	= 0;
			m_lHeight	= 0;
		}
		~CDIB()
		{
			free(m_pInfo);
			m_pInfo = NULL;
			
			if (NULL != m_hBitmap)
				DeleteObject(m_hBitmap);			
			if (m_hSection != NULL)
				CloseHandle(m_hSection);
			if (m_hFile != NULL)
				CloseHandle(m_hFile);
		}

	public:
		BOOL Create(LONG lWidth, LONG lHeight, double dDPIX = 0, double dDPIY = 0)
		{
			m_lWidth	= lWidth;
			m_lHeight	= lHeight;

			m_pInfo->bmiHeader.biWidth		= m_lWidth;
			m_pInfo->bmiHeader.biHeight		= m_lHeight;

			if (dDPIX > 1 && dDPIY > 1)
			{
				m_pInfo->bmiHeader.biXPelsPerMeter = (LONG)(dDPIX / 0.0254);
				m_pInfo->bmiHeader.biYPelsPerMeter = (LONG)(dDPIY / 0.0254);
			}

			m_hBitmap = ::CreateDIBSection(NULL, m_pInfo, DIB_RGB_COLORS, (VOID**)&m_pBits, NULL, 0);

			DWORD dwError = GetLastError();

			if (NULL == m_hBitmap)
			{
				CString strTempPath;
				if (::GetTempPath(_MAX_PATH, strTempPath.GetBuffer(_MAX_PATH)) != 0)
					strTempPath.ReleaseBuffer();
				else
					strTempPath = _T(".");

				CString strTempFile;
				if (::GetTempFileName(strTempPath, _T("dib"), 0, strTempFile.GetBuffer(_MAX_PATH)) != 0)
				{
					DWORD nLineLength = static_cast<DWORD>(ceil((double)(1.0 * m_lWidth * 32 / 8.0)));
					while ((nLineLength % 4) != 0)
						++nLineLength;

					DWORD nBitsSize = nLineLength * m_lHeight;

					// Try to create a mapped file section
					m_hFile = ::CreateFile(strTempFile, GENERIC_READ | GENERIC_WRITE,
						0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
					if (m_hFile != NULL && m_hFile != INVALID_HANDLE_VALUE)
					{
						m_hSection = ::CreateFileMapping(m_hFile, NULL,
							PAGE_READWRITE, 0, nBitsSize, NULL);
						if (m_hSection != NULL)
						{
							m_hBitmap = ::CreateDIBSection(NULL, m_pInfo, DIB_RGB_COLORS,
									(VOID**)&m_pBits, m_hSection, 0);
						}
					}

					if (NULL == m_hBitmap)
					{
						if (m_hSection != NULL)
							::CloseHandle(m_hSection);
						if (m_hFile != NULL && m_hFile != INVALID_HANDLE_VALUE)
							::CloseHandle(m_hFile);
						m_hSection = NULL;
						m_hFile = NULL;
					}
				}
			}

			return (NULL != m_hBitmap);
		}

		void Draw(CPainter* pPainter, CDoubleRect& oRectDraw)
		{
			if (NULL == m_hBitmap)
				return;

			LONG lDrawWidth  = (LONG)oRectDraw.GetWidth();
			LONG lDrawHeight = (LONG)oRectDraw.GetHeight();

			if ((lDrawWidth == m_lWidth) && (lDrawHeight == m_lHeight))
			{
				// просто копируем пикселы
				LONG leftBuffer  = 0;
				LONG leftDisplay = (LONG)oRectDraw.left;
				if (leftDisplay < 0)
				{
					leftBuffer  = -leftDisplay;
					leftDisplay = 0;
				}
				
				LONG topBuffer  = 0;
				LONG topDisplay = (LONG)oRectDraw.top;
				if (topDisplay < 0)
				{
					topBuffer  = -topDisplay;
					topDisplay = 0;
				}

				LONG bottomDisplay = min((LONG)oRectDraw.bottom, pPainter->m_lDisplayHeight - 1);
				LONG rightDisplay  = min((LONG)oRectDraw.right,  pPainter->m_lDisplayWidth - 1);
				
				HDC memdc = CreateCompatibleDC(pPainter->m_hMemDC);				
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, m_hBitmap);
			
				BitBlt(pPainter->m_hMemDC, leftDisplay, topDisplay, rightDisplay - leftDisplay + 1, bottomDisplay - topDisplay + 1, 
					memdc, leftBuffer, topBuffer, SRCCOPY);

				DeleteObject(hOldBitmap);
				DeleteDC(memdc);
				//SelectObject(pPainter->m_hMemDC, hOldBitmap);
				//DeleteObject(hOldBitmap);
			}
			else
			{
				SetStretchBltMode(pPainter->m_hMemDC, COLORONCOLOR);
				StretchDIBits(pPainter->m_hMemDC, (LONG)oRectDraw.left, (LONG)oRectDraw.top, (LONG)oRectDraw.GetWidth(), (LONG)oRectDraw.GetHeight(),
					0, 0, m_lWidth, m_lHeight, m_pBits, m_pInfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}

		void Draw2(CPainter* pPainter, CDoubleRect& oRectDraw)
		{
			if (NULL == m_hBitmap)
				return;

			LONG lDrawWidth  = (LONG)oRectDraw.GetWidth();
			LONG lDrawHeight = (LONG)oRectDraw.GetHeight();

			if ((lDrawWidth == m_lWidth) && (lDrawHeight == m_lHeight))
			{
				// просто копируем пикселы
				LONG leftBuffer  = 0;
				LONG leftDisplay = (LONG)oRectDraw.left;
				if (leftDisplay < 0)
				{
					leftBuffer  = -leftDisplay;
					leftDisplay = 0;
				}
				
				LONG topBuffer  = 0;
				LONG topDisplay = (LONG)oRectDraw.top;
				if (topDisplay < 0)
				{
					topBuffer  = -topDisplay;
					topDisplay = 0;
				}

				LONG bottomDisplay = min((LONG)oRectDraw.bottom, pPainter->m_lDisplayHeight - 1);
				LONG rightDisplay  = min((LONG)oRectDraw.right,  pPainter->m_lDisplayWidth - 1);
				
				HDC memdc = CreateCompatibleDC(pPainter->m_hDC);				
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, m_hBitmap);
			
				BitBlt(pPainter->m_hDC, leftDisplay, topDisplay, rightDisplay - leftDisplay + 1, bottomDisplay - topDisplay + 1, 
					memdc, leftBuffer, topBuffer, SRCCOPY);

				DeleteObject(hOldBitmap);
				DeleteDC(memdc);
				//SelectObject(pPainter->m_hMemDC, hOldBitmap);
				//DeleteObject(hOldBitmap);
			}
			else
			{
				SetStretchBltMode(pPainter->m_hDC, COLORONCOLOR);
				StretchDIBits(pPainter->m_hDC, (LONG)oRectDraw.left, (LONG)oRectDraw.top, (LONG)oRectDraw.GetWidth(), (LONG)oRectDraw.GetHeight(),
					0, 0, m_lWidth, m_lHeight, m_pBits, m_pInfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}

		void Draw3(CPainter* pPainter, CDoubleRect& oRectDraw)
		{
			if (NULL == m_hBitmap)
				return;

			LONG lDrawWidth  = (LONG)oRectDraw.GetWidth();
			LONG lDrawHeight = (LONG)oRectDraw.GetHeight();

			SetDIBitsToDevice(pPainter->m_hDC, (LONG)oRectDraw.left, (LONG)oRectDraw.top, m_lWidth, m_lHeight,
				0, 0, 0, m_lHeight, m_pBits, m_pInfo, DIB_RGB_COLORS);
			return;
			
			
			if ((lDrawWidth == m_lWidth) && (lDrawHeight == m_lHeight))
			{
				// просто копируем пикселы
				LONG leftBuffer  = 0;
				LONG leftDisplay = (LONG)oRectDraw.left;
				if (leftDisplay < 0)
				{
					leftBuffer  = -leftDisplay;
					leftDisplay = 0;
				}
				
				LONG topBuffer  = 0;
				LONG topDisplay = (LONG)oRectDraw.top;
				if (topDisplay < 0)
				{
					topBuffer  = -topDisplay;
					topDisplay = 0;
				}

				LONG bottomDisplay = min((LONG)oRectDraw.bottom, pPainter->m_lDisplayHeight - 1);
				LONG rightDisplay  = min((LONG)oRectDraw.right,  pPainter->m_lDisplayWidth - 1);
				
				HDC memdc = CreateCompatibleDC(pPainter->m_hDC);				
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, m_hBitmap);
			
				BitBlt(pPainter->m_hDC, leftDisplay, topDisplay, m_lWidth, m_lHeight, 
					memdc, leftBuffer, topBuffer, SRCCOPY);

				DeleteObject(hOldBitmap);
				DeleteDC(memdc);
				//SelectObject(pPainter->m_hMemDC, hOldBitmap);
				//DeleteObject(hOldBitmap);
			}
			else
			{
				SetStretchBltMode(pPainter->m_hDC, COLORONCOLOR);
				StretchDIBits(pPainter->m_hDC, (LONG)oRectDraw.left, (LONG)oRectDraw.top, (LONG)oRectDraw.GetWidth(), (LONG)oRectDraw.GetHeight(),
					0, 0, m_lWidth, m_lHeight, m_pBits, m_pInfo, DIB_RGB_COLORS, SRCCOPY);
			}
		}
	};
}