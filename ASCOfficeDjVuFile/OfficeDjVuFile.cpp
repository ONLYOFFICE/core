#include "stdafx.h"
#include "OfficeDjVuFile.h"
#include "Resource.h"
#include "math.h"
#include "TimeMeasurer.h"

#define VER_DPI		96
#define HOR_DPI		96


namespace NSDjvu
{
	static GUTF8String MakeUTF8String(const CString& strText)
	{
		int nSize;

	#ifdef _UNICODE
		LPCWSTR pszUnicodeText = strText;
	#else
		nSize = ::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strText, -1, NULL, 0);
		if (nSize == 0)
			return "";

		LPWSTR pszUnicodeText = new WCHAR[nSize];
		::MultiByteToWideChar(CP_ACP, 0, (LPCSTR)strText, -1, pszUnicodeText, nSize);
	#endif

		nSize = ::WideCharToMultiByte(CP_UTF8, 0, pszUnicodeText, -1, NULL, 0, NULL, NULL);
		if (nSize == 0)
		{
	#ifndef _UNICODE
			delete[] pszUnicodeText;
	#endif
			return "";
		}

		LPSTR pszTextUTF8 = new CHAR[nSize];
		::WideCharToMultiByte(CP_UTF8, 0, pszUnicodeText, -1, pszTextUTF8, nSize, NULL, NULL);

		GUTF8String utf8String(pszTextUTF8);
		delete[] pszTextUTF8;

	#ifndef _UNICODE
		delete[] pszUnicodeText;
	#endif

		return utf8String;
	}


	static CString MakeCString(GUTF8String& strText)
	{
		int nSize;

		LPSTR pszUtf8Text = strText.getbuf();

		//nSize = ::WideCharToMultiByte(CP_UTF8, 0, pszUtf8Text, -1, NULL, 0, NULL, NULL);
		nSize = ::MultiByteToWideChar(CP_UTF8, 0, pszUtf8Text, -1, NULL, 0);

		if (nSize == 0)
		{
			return _T("");
		}

		LPWSTR pszUnicodeText = new WCHAR[nSize];
		::MultiByteToWideChar(CP_UTF8, 0, pszUtf8Text, -1, pszUnicodeText, nSize);

		CString String(pszUnicodeText);
		delete[] pszUnicodeText;

		return String;
	}
}

CDjVuFile::CDjVuFile()
{
}


HRESULT CDjVuFile::FinalConstruct()
{
	CString sTempPath;
	GetTempPath(4096, sTempPath.GetBuffer(4096));
	sTempPath.ReleaseBuffer();
	
	sTempPath += L"DJVU\\";

	#ifdef _DEBUG
	sTempPath =  _T("C:\\DJVU\\");
	#endif

	m_strTempDirectory = sTempPath;
	CreateDirectory(m_strTempDirectory, NULL);
	
	m_pDocumentRenderer = FALSE;

	InitializeCriticalSection(&m_oCS);

	return S_OK;
}


void CDjVuFile::FinalRelease()
{
	if (NULL != m_pDocumentRenderer)
	{
		m_pDocumentRenderer->Release();
		m_pDocumentRenderer = NULL;
	}

	WIN32_FIND_DATA winFileData;
	HANDLE hFile;

	CString strFoundPreset = m_strTempDirectory + _T("\\*.*");
	
	hFile = FindFirstFile(strFoundPreset, &winFileData); 
	if (hFile != INVALID_HANDLE_VALUE)
	{ 
		DeleteFile(winFileData.cFileName);
		while (FALSE != FindNextFile(hFile, &winFileData))
		{
			DeleteFile(winFileData.cFileName);
		}
		FindClose(hFile);
	}
	RemoveDirectory(m_strTempDirectory);

	DeleteCriticalSection(&m_oCS);
}


STDMETHODIMP CDjVuFile::get_PagesCount(LONG* plCount)
{
	*plCount = m_pDoc->get_pages_num();	
	return S_OK;
}

STDMETHODIMP CDjVuFile::GetPageInfo(int nPage, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	GP<DjVuImage> pPage = m_pDoc->get_page(nPage);

	pPage->wait_for_complete_decode();
	pPage->set_rotate(0);

	*pdWidth = pPage->get_real_width();
	*pdHeight = pPage->get_real_height();

	*pdDpiX = pPage->get_dpi();
	*pdDpiY = pPage->get_dpi();

	return S_OK;
}


STDMETHODIMP CDjVuFile::DrawPage(int nPage, IUnknown* punkRenderer, BOOL* pBreak)
{
	//CTemporaryCS* pCS = new CTemporaryCS(&m_oCS);
	
	try
	{
		GP<DjVuImage> pPage = m_pDoc->get_page(nPage);		
		pPage->wait_for_complete_decode();
		pPage->set_rotate(0);
	
		//RELEASEOBJECT(pCS);

		AVSGraphics::IAVSGraphicsRenderer* pGrRenderer = NULL;
		punkRenderer->QueryInterface(__uuidof(AVSGraphics::IAVSGraphicsRenderer), (void**)&pGrRenderer);

		if (NULL != pGrRenderer)
		{
			CreateFrame3(pGrRenderer, pPage, pBreak);
			RELEASEINTERFACE(pGrRenderer);
			return S_OK;
		}

		XmlUtils::CXmlNode text = ParseText(pPage);

		AVSGraphics::IAVSRenderer* renderer;
		punkRenderer->QueryInterface(__uuidof(AVSGraphics::IAVSRenderer), (void**)&renderer);
		CreateFrame2(renderer, pPage, nPage, text);
		RELEASEINTERFACE(renderer);
	}
	catch(...)
	{
		// бела€ страница
		return S_OK;
	}

	return S_OK;
}

STDMETHODIMP CDjVuFile::LoadFromFile(BSTR sSrcFileName, BSTR sDstPath, BSTR sXMLOptions)
{
	//CTemporaryCS oCS(&m_oCS);
	
	if (NULL == m_pDocumentRenderer)
		return S_FALSE;

	m_pDoc = NULL;	
	try
	{
		GUTF8String utf8;
		GURL url = GURL::Filename::UTF8(NSDjvu::MakeUTF8String(CString(sSrcFileName)));
		m_pDoc = DjVuDocument::create(url);
		m_pDoc->wait_get_pages_num();
	}
	catch (...)
	{
		return S_FALSE;
	}
/*
	int nPages = pDoc->get_pages_num();
	pDoc->wait_for_complete_init();

	for (int page = 0; page < nPages; ++page)
	{
		GP<DjVuImage> pPage = pDoc->get_page(page);		
		pPage->wait_for_complete_decode();
		pPage->set_rotate(0);
		//
		XmlUtils::CXmlNode text = ParseText(pPage);
		//
		double dWidth	= 190;
		double dHeight	= 270;
		
		CreateFrame(pPage, page, text);

		SHORT nCancel = c_shProgressContinue;
		LONG lProgress = (c_nMaxProgressPercent * (page + 1)) / nPages;
		OnProgressEx(0, lProgress, &nCancel);

		if (c_shProgressCancel == nCancel)
			break;
	}
*/
	
	int nPagesCount = m_pDoc->get_pages_num();

	SAFEARRAYBOUND rgsab;
	rgsab.lLbound	= 0;
	rgsab.cElements	= 2 * nPagesCount;

	SAFEARRAY *psaPagesSizes = SafeArrayCreate( VT_R8, 1, &rgsab );
	double *pSizes = (double*)psaPagesSizes->pvData;

	for (int nPageIndex = 0; nPageIndex < nPagesCount; ++nPageIndex, pSizes += 2)
	{
		int nWidth	= 1000;
		int nHeight	= 1000;

		int nDpi = 300;

		m_pDoc->ReadPageInfo(nPageIndex, nWidth, nHeight, nDpi);

		double dPixToMM = 25.4;	

		double dW_MM = dPixToMM * nWidth  / nDpi;
		double dH_MM = dPixToMM * nHeight / nDpi;

		pSizes[0] = dW_MM;
		pSizes[1] = dH_MM;		

		/*m_pDocumentRenderer->NewPage();
		m_pDocumentRenderer->put_Width(dW_MM);
		m_pDocumentRenderer->put_Height(dH_MM);

		IUnknown* punkInst = NULL;
		this->QueryInterface(IID_IUnknown, (void**)&punkInst);

		m_pDocumentRenderer->SetPageInstance(punkInst);

		RELEASEINTERFACE(punkInst);
		m_pDocumentRenderer->EndCommand(0x0001);*/
	}

	IUnknown* punkInst = NULL;
	this->QueryInterface(IID_IUnknown, (void**)&punkInst);
	m_pDocumentRenderer->SetDocumentInstance( punkInst, psaPagesSizes );
	RELEASEINTERFACE(punkInst);

	SafeArrayDestroy( psaPagesSizes );

	// больше документ рендерер не нужен
	RELEASEINTERFACE(m_pDocumentRenderer);

	return S_OK;
}


STDMETHODIMP CDjVuFile::SaveToFile(BSTR sDstFileName, BSTR sSrcPath, BSTR sXMLOptions)
{
	return S_OK;
}

void CDjVuFile::CreateFrame(GP<DjVuImage>& pPage, int nPage, XmlUtils::CXmlNode& text)
{
	FILE* file;
	file = fopen("C:\\times.txt", "a+");
	
	CTimeMeasurer getPixmapTimer;
	CTimeMeasurer getBitmapTimer;
	CTimeMeasurer save24bitTimer;
	CTimeMeasurer save8bitTimer;
	CTimeMeasurer drawTextTimer;
			
	double time;

	CString strNum;
	strNum.Format(_T("%d"), nPage);

	CString strFile = m_strTempDirectory + strNum;

	int nWidth	= pPage->get_real_width();
	int nHeight	= pPage->get_real_height();

	int nDpi = pPage->get_dpi();	

	double dW_MM;
	double dH_MM;

	double dPixToMM = 25.4;	

	dW_MM = dPixToMM * nWidth  / nDpi;
	dH_MM = dPixToMM * nHeight / nDpi;

	GRect oRect(0, 0, nWidth, nHeight);

	if (pPage->is_legal_photo() || pPage->is_legal_compound())
	{
		GRect oRectAll(0, 0, nWidth * HOR_DPI / nDpi, nHeight * VER_DPI / nDpi);
		getPixmapTimer.Reset();//TIMER
		GP<GPixmap> pImage = pPage->get_pixmap(/*oRect*/oRectAll, oRectAll);
		time = getPixmapTimer.GetTimeInterval();//TIMER
		fprintf(file, "GetPixmap:%lf\n", time);//TIMER

		strFile += _T(".png");		
		save24bitTimer.Reset();//TIMER
		m_oImage.SaveImage24bit(pImage->operator [](0), nWidth * HOR_DPI / nDpi, nHeight * VER_DPI / nDpi, strFile);
		time = save24bitTimer.GetTimeInterval();//TIMER
		fprintf(file, "Save24bit:%lf\n", time);//TIMER
	}
	else if (pPage->is_legal_bilevel())
	{
		getBitmapTimer.Reset();//TIMER
		GP<GBitmap> pBitmap = pPage->get_bitmap(oRect, oRect, 4);
		time = getBitmapTimer.GetTimeInterval();//TIMER
		fprintf(file, "GetBitmap:%lf\n", time);//TIMER
		int nPaletteEntries = pBitmap->get_grays();

		LPBITMAPINFO pBMI;
		int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nPaletteEntries;

		pBMI = (LPBITMAPINFO) malloc(nSize);
		ZeroMemory(pBMI, nSize);

		BITMAPINFOHEADER& bmih	= pBMI->bmiHeader;
		bmih.biSize				= sizeof(bmih);
		bmih.biPlanes			= 1;
		bmih.biWidth			= nWidth;
		bmih.biHeight			= nHeight;
		bmih.biBitCount			= 8;
		bmih.biCompression		= BI_RGB;
		bmih.biClrUsed			= nPaletteEntries;

		// Create palette for the bitmap
		int color = 0xff0000;
		int decrement = color/(nPaletteEntries - 1);
		for (int i = 0; i < nPaletteEntries; ++i)
		{
			int level = color >> 16;
			pBMI->bmiColors[i].rgbBlue = level;
			pBMI->bmiColors[i].rgbGreen = level;
			pBMI->bmiColors[i].rgbRed = level;
			color -= decrement;
		}

		strFile += _T(".bmp");
		save8bitTimer.Reset();//TIMER
		m_oImage.SaveImage8bit(pBMI, pBitmap, strFile);
		time = save8bitTimer.GetTimeInterval();//TIMER
		fprintf(file, "Save8bit:%lf\n", time);//TIMER
	}
	else
	{
		// белый фрейм??
	}

	PageToRenderer(dW_MM, dH_MM);	
		
	drawTextTimer.Reset();//TIMER
	TextToRenderer(text, dPixToMM / nDpi);
	time = drawTextTimer.GetTimeInterval();//TIMER
	fprintf(file, "Drawtext:%lf\n", time);//TIMER	

	ImageToRenderer(strFile, dW_MM, dH_MM);	

	m_pDocumentRenderer->EndCommand(0x0001); // CompletePage
	fprintf(file, "--------------\n");
	fclose(file);//TIMER
}



void CDjVuFile::CreateFrame2(AVSGraphics::IAVSRenderer* renderer, GP<DjVuImage>& pPage, int nPage, XmlUtils::CXmlNode& text)
{			
	int nWidth	= pPage->get_real_width();
	int nHeight	= pPage->get_real_height();

	int nDpi = pPage->get_dpi();	

	double dPixToMM = 25.4;

	double dRendDpiX = 0;
	double dRendDpiY = 0;
	renderer->get_DpiX(&dRendDpiX);
	renderer->get_DpiY(&dRendDpiY);

	if (0 >= dRendDpiX)
		dRendDpiX = 72.0;
	if (0 >= dRendDpiY)
		dRendDpiY = 72.0;

	double dRendWidth = 0;
	double dRendHeight = 0;
	renderer->get_Width(&dRendWidth);
	renderer->get_Height(&dRendHeight);

	if (0 >= dRendWidth)
		dRendWidth = 200;
	if (0 >= dRendHeight)
		dRendHeight = 300;

	MediaCore::IAVSUncompressedVideoFrame* pFrame;
	CoCreateInstance(MediaCore::CLSID_CAVSUncompressedVideoFrame, NULL, CLSCTX_INPROC, MediaCore::IID_IAVSUncompressedVideoFrame, (void**)&pFrame);

	LONG lImageWidth	= (LONG)(dRendDpiX * dRendWidth  / dPixToMM);
	LONG lImageHeight	= (LONG)(dRendDpiY * dRendHeight / dPixToMM);

	long lRendererType = 0x00; // Unknown renderer
	renderer->get_Type( &lRendererType );
	if ( 0x01 == lRendererType ) // PDF
	{
		// забиваем на dpi
		lImageWidth	 = pPage->get_real_width();
		lImageHeight = pPage->get_real_height();
	}
	else if (0x02 == lRendererType)
	{
		// html
		VARIANT var;
		renderer->GetAdditionalParam(L"MaxImageSize", &var);

		LONG lMaxWidth = var.lVal;
		if ((lImageWidth > lMaxWidth) || (lImageHeight > lMaxWidth))
		{
			double dAspect = (double)(lImageWidth) / lImageHeight;
			if (lImageWidth > lImageHeight)
			{
				lImageWidth  = lMaxWidth;
				lImageHeight = (LONG)(lImageHeight / dAspect);
 			}
			else
			{
				lImageHeight = lMaxWidth;
				lImageWidth  = (LONG)(dAspect * lImageHeight);		
			}
		}
	}

	pFrame->put_Width(lImageWidth);
	pFrame->put_Height(lImageHeight);
	pFrame->put_ColorSpace(CSP_BGRA);
	pFrame->SetDefaultStrides();
	pFrame->AllocateBuffer(-1);

	BYTE* pBufferDst = NULL;
	pFrame->get_Buffer(&pBufferDst);
	
	if (pPage->is_legal_photo() || pPage->is_legal_compound())
	{
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);		
		GP<GPixmap> pImage = pPage->get_pixmap(oRectAll, oRectAll);

		BYTE* pBuffer = pBufferDst;
		for (int j = lImageHeight - 1; j >= 0; --j)
		{
			GPixel* pLine = pImage->operator [](j);

			for (int i = 0; i < lImageWidth; ++i, pBuffer += 4, ++pLine)
			{
				pBuffer[0] = pLine->b;
				pBuffer[1] = pLine->g;
				pBuffer[2] = pLine->r;
				pBuffer[3] = 255;
			}
		}
	}
	else if (pPage->is_legal_bilevel())
	{
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);
		GP<GBitmap> pBitmap = pPage->get_bitmap(oRectAll, oRectAll, 4);
		int nPaletteEntries = pBitmap->get_grays();

		DWORD* palette = new DWORD[nPaletteEntries];

		// Create palette for the bitmap
		int color = 0xff0000;
		int decrement = color/(nPaletteEntries - 1);
		for (int i = 0; i < nPaletteEntries; ++i)
		{
			BYTE level = (BYTE)(color >> 16);
			palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
			color -= decrement;
		}

		DWORD* pBuffer = (DWORD*)pBufferDst;
		for (int j = lImageHeight - 1; j >= 0; --j)
		{
			BYTE* pLine = pBitmap->operator [](j);

			for (int i = 0; i < lImageWidth; ++i, ++pBuffer, ++pLine)
			{
				if (*pLine < nPaletteEntries)
				{
					*pBuffer = palette[*pLine];
				}
				else
				{
					*pBuffer = palette[0];
				}
			}
		}

		RELEASEARRAYOBJECTS(palette);
	}
	else
	{
		// белый фрейм??
		//memset(pBufferDst, 0xFF, 4 * lImageWidth * lImageHeight);
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);		
		GP<GPixmap> pImage = pPage->get_pixmap(oRectAll, oRectAll);

		if (NULL != pImage)
		{
			BYTE* pBuffer = pBufferDst;
			for (int j = lImageHeight - 1; j >= 0; --j)
			{
				GPixel* pLine = pImage->operator [](j);

				for (int i = 0; i < lImageWidth; ++i, pBuffer += 4, ++pLine)
				{
					pBuffer[0] = pLine->b;
					pBuffer[1] = pLine->g;
					pBuffer[2] = pLine->r;
					pBuffer[3] = 255;
				}
			}
		}
		else
		{
			GP<GBitmap> pBitmap = pPage->get_bitmap(oRectAll, oRectAll, 4);

			if (NULL != pBitmap)
			{
				int nPaletteEntries = pBitmap->get_grays();

				DWORD* palette = new DWORD[nPaletteEntries];

				// Create palette for the bitmap
				int color = 0xff0000;
				int decrement = color/(nPaletteEntries - 1);
				for (int i = 0; i < nPaletteEntries; ++i)
				{
					BYTE level = (BYTE)(color >> 16);
					palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
					color -= decrement;
				}

				DWORD* pBuffer = (DWORD*)pBufferDst;
				for (int j = lImageHeight - 1; j >= 0; --j)
				{
					BYTE* pLine = pBitmap->operator [](j);

					for (int i = 0; i < lImageWidth; ++i, ++pBuffer, ++pLine)
					{
						if (*pLine < nPaletteEntries)
						{
							*pBuffer = palette[*pLine];
						}
						else
						{
							*pBuffer = palette[0];
						}
					}
				}

				RELEASEARRAYOBJECTS(palette);
			}
		}
	}

	// здесь - не GraphicsRenderer (дл€ него друга€ функци€)	
	if (0x02 != lRendererType && 0x06 != lRendererType)
	{
		// дл€ html текст не нужен
		TextToRenderer(renderer, text, dPixToMM / nDpi);
	}
	
	renderer->DrawImage((IUnknown*)pFrame, 0, 0, dRendWidth, dRendHeight);

	RELEASEINTERFACE(pFrame);

	renderer->EndCommand(0x0001); // CompletePage
}

void CDjVuFile::CreateFrame3(AVSGraphics::IAVSGraphicsRenderer* renderer, GP<DjVuImage>& pPage, BOOL* pBreak)
{			
	int nWidth	= pPage->get_real_width();
	int nHeight	= pPage->get_real_height();

	BYTE*	pBufferDst	 = NULL;
	LONG	lImageWidth	 = 0;
	LONG	lImageHeight = 0;

	VARIANT var;
	renderer->GetAdditionalParam(L"Pixels", &var);
	pBufferDst = (BYTE*)var.lVal;

	renderer->GetAdditionalParam(L"PixelsWidth", &var);
	lImageWidth  = var.lVal;
	renderer->GetAdditionalParam(L"PixelsHeight", &var);
	lImageHeight = var.lVal;
	
	volatile BOOL* pCancel = pBreak;

	if ((NULL != pCancel) && (TRUE == *pCancel))
		return;

	if (pPage->is_legal_photo() || pPage->is_legal_compound())
	{
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);		
		GP<GPixmap> pImage = pPage->get_pixmap(oRectAll, oRectAll);

		BYTE* pBuffer = pBufferDst;
		for (int j = 0; j < lImageHeight; ++j)
		{
			GPixel* pLine = pImage->operator [](j);

			if ((NULL != pCancel) && (TRUE == *pCancel))
				return;

			for (int i = 0; i < lImageWidth; ++i, pBuffer += 4, ++pLine)
			{
				pBuffer[0] = pLine->b;
				pBuffer[1] = pLine->g;
				pBuffer[2] = pLine->r;
				pBuffer[3] = 255;
			}
		}
	}
	else if (pPage->is_legal_bilevel())
	{
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);
		GP<GBitmap> pBitmap = pPage->get_bitmap(oRectAll, oRectAll, 4);
		int nPaletteEntries = pBitmap->get_grays();

		DWORD* palette = new DWORD[nPaletteEntries];

		// Create palette for the bitmap
		int color = 0xff0000;
		int decrement = color/(nPaletteEntries - 1);
		for (int i = 0; i < nPaletteEntries; ++i)
		{
			BYTE level = (BYTE)(color >> 16);
			palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
			color -= decrement;
		}

		DWORD* pBuffer = (DWORD*)pBufferDst;
		for (int j = 0; j < lImageHeight; ++j)
		{
			BYTE* pLine = pBitmap->operator [](j);

			if ((NULL != pCancel) && (TRUE == *pCancel))
				return;

			for (int i = 0; i < lImageWidth; ++i, ++pBuffer, ++pLine)
			{
				if (*pLine < nPaletteEntries)
				{
					*pBuffer = palette[*pLine];
				}
				else
				{
					*pBuffer = palette[0];
				}
			}
		}

		RELEASEARRAYOBJECTS(palette);
	}
	else
	{
		// белый фрейм??
		//memset(pBufferDst, 0xFF, 4 * lImageWidth * lImageHeight);
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);		
		GP<GPixmap> pImage = pPage->get_pixmap(oRectAll, oRectAll);

		if (NULL != pImage)
		{
			BYTE* pBuffer = pBufferDst;
			for (int j = 0; j < lImageHeight; ++j)
			{
				GPixel* pLine = pImage->operator [](j);

				if ((NULL != pCancel) && (TRUE == *pCancel))
					return;

				for (int i = 0; i < lImageWidth; ++i, pBuffer += 4, ++pLine)
				{
					pBuffer[0] = pLine->b;
					pBuffer[1] = pLine->g;
					pBuffer[2] = pLine->r;
					pBuffer[3] = 255;
				}
			}

			return;
		}

		GP<GBitmap> pBitmap = pPage->get_bitmap(oRectAll, oRectAll, 4);

		if (NULL != pBitmap)
		{
			int nPaletteEntries = pBitmap->get_grays();

			DWORD* palette = new DWORD[nPaletteEntries];

			// Create palette for the bitmap
			int color = 0xff0000;
			int decrement = color/(nPaletteEntries - 1);
			for (int i = 0; i < nPaletteEntries; ++i)
			{
				BYTE level = (BYTE)(color >> 16);
				palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
				color -= decrement;
			}

			DWORD* pBuffer = (DWORD*)pBufferDst;
			for (int j = 0; j < lImageHeight; ++j)
			{
				BYTE* pLine = pBitmap->operator [](j);

				if ((NULL != pCancel) && (TRUE == *pCancel))
					return;

				for (int i = 0; i < lImageWidth; ++i, ++pBuffer, ++pLine)
				{
					if (*pLine < nPaletteEntries)
					{
						*pBuffer = palette[*pLine];
					}
					else
					{
						*pBuffer = palette[0];
					}
				}
			}

			RELEASEARRAYOBJECTS(palette);
		}
	}
}

void CDjVuFile::CreateFrame(AVSGraphics::IAVSRenderer* renderer, GP<DjVuImage>& pPage, int nPage, XmlUtils::CXmlNode& text)
{			
	CString strNum;
	strNum.Format(_T("%d"), nPage);

	CString strFile = m_strTempDirectory + strNum;

	int nWidth	= pPage->get_real_width();
	int nHeight	= pPage->get_real_height();

	int nDpi = pPage->get_dpi();	

	double dW_MM;
	double dH_MM;

	double dPixToMM = 25.4;	

	dW_MM = dPixToMM * nWidth  / nDpi;
	dH_MM = dPixToMM * nHeight / nDpi;

	GRect oRect(0, 0, nWidth, nHeight);

	if (pPage->is_legal_photo() || pPage->is_legal_compound())
	{
		GRect oRectAll(0, 0, nWidth * HOR_DPI / nDpi, nHeight * VER_DPI / nDpi);		
		GP<GPixmap> pImage = pPage->get_pixmap(/*oRect*/oRectAll, oRectAll);

		strFile += _T(".png");	
		m_oImage.SaveImage24bit(pImage->operator [](0), nWidth * HOR_DPI / nDpi, nHeight * VER_DPI / nDpi, strFile);
		
	}
	else if (pPage->is_legal_bilevel())
	{
		GP<GBitmap> pBitmap = pPage->get_bitmap(oRect, oRect, 4);
		int nPaletteEntries = pBitmap->get_grays();

		LPBITMAPINFO pBMI;
		int nSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*nPaletteEntries;

		pBMI = (LPBITMAPINFO) malloc(nSize);
		ZeroMemory(pBMI, nSize);

		BITMAPINFOHEADER& bmih	= pBMI->bmiHeader;
		bmih.biSize				= sizeof(bmih);
		bmih.biPlanes			= 1;
		bmih.biWidth			= nWidth;
		bmih.biHeight			= nHeight;
		bmih.biBitCount			= 8;
		bmih.biCompression		= BI_RGB;
		bmih.biClrUsed			= nPaletteEntries;

		// Create palette for the bitmap
		int color = 0xff0000;
		int decrement = color/(nPaletteEntries - 1);
		for (int i = 0; i < nPaletteEntries; ++i)
		{
			int level = color >> 16;
			pBMI->bmiColors[i].rgbBlue = level;
			pBMI->bmiColors[i].rgbGreen = level;
			pBMI->bmiColors[i].rgbRed = level;
			color -= decrement;
		}

		strFile += _T(".bmp");
		m_oImage.SaveImage8bit(pBMI, pBitmap, strFile);
	}
	else
	{
		// белый фрейм??
	}

	PageToRenderer(renderer, dW_MM, dH_MM);		
	TextToRenderer(renderer, text, dPixToMM / nDpi);
	ImageToRenderer(renderer, strFile, dW_MM, dH_MM);	

	renderer->EndCommand(0x0001); // CompletePage
}



CString CDjVuFile::SavePage(int nPageNum, IUnknown* pPage)
{
	CString strNum;
	strNum.Format(_T("%d"), nPageNum);
	
	CString strFile = m_strTempDirectory + strNum + _T(".jpg");
		
	CString strXml = _T("<transforms><ImageFile-SaveAsJpeg destinationpath='") + 
		strFile + _T("' format='888' quality='100' /></transforms>");

	ImageStudio::IImageTransforms* pTransforms = NULL;
	CoCreateInstance(ImageStudio::CLSID_ImageTransforms, NULL, CLSCTX_INPROC, ImageStudio::IID_IImageTransforms, (void**)&pTransforms);

	if (NULL == pTransforms)
		return _T("");

	VARIANT var;
	var.vt = VT_UNKNOWN;
	var.punkVal = pPage;

	pTransforms->SetSource(0, var);
	var.punkVal = NULL;

	VARIANT_BOOL vbRes = VARIANT_FALSE;
	BSTR bsXml = strXml.AllocSysString();
	pTransforms->SetXml(bsXml, &vbRes);
	pTransforms->Transform(&vbRes);

	pTransforms->Release();

	return strFile;
}


XmlUtils::CXmlNode CDjVuFile::ParseText(GP<DjVuImage> pPage)
{
	XmlUtils::CXmlNode paragraph;
	const GP<DjVuText> text(DjVuText::create());
	const GP<ByteStream> text_str(pPage->get_text());
	if(text_str)
	{		
		text->decode(text_str);
		GUTF8String pageText = text->get_xmlText(pPage->get_height());
		XmlUtils::CXmlNode hiddenText;
		XmlUtils::CXmlNode pageColumn;
		XmlUtils::CXmlNode region;
		hiddenText.FromXmlString(NSDjvu::MakeCString(pageText));
		hiddenText.GetNode(_T("PAGECOLUMN"), pageColumn);
		pageColumn.GetNode(_T("REGION"), region);
		region.GetNode(_T("PARAGRAPH"), paragraph);
	}
	return paragraph;
}


void CDjVuFile::PageToRenderer(double width, double height)
{
	m_pDocumentRenderer->NewPage();
	m_pDocumentRenderer->put_Width(width);
	m_pDocumentRenderer->put_Height(height);
}


void CDjVuFile::PageToRenderer(AVSGraphics::IAVSRenderer* renderer, double width, double height)
{
	renderer->NewPage();
	renderer->put_Width(width);
	renderer->put_Height(height);
}


void CDjVuFile::TextToRenderer(XmlUtils::CXmlNode text, double koef, bool isView)
{
	
	m_pDocumentRenderer->SetShadow(L"<shadow visible='0' />");
	CString p = text.GetXml();
	XmlUtils::CXmlNodes lines_node;
	text.GetNodes(_T("LINE"), lines_node);
	for(int i = 0; i < lines_node.GetCount(); ++i)
	{
		XmlUtils::CXmlNode line_node;
		lines_node.GetAt(i, line_node);
		XmlUtils::CXmlNodes words_node;
		line_node.GetNodes(_T("WORD"), words_node);
		for(int j = 0; j < words_node.GetCount(); ++j)
		{
			XmlUtils::CXmlNode word_node;
			words_node.GetAt(j, word_node);
			CString word = word_node.GetText();
			CString coordsStr = word_node.GetAttribute(_T("coords"));
			double coords[4];
			ParseCoords(coordsStr, coords, koef);		
			DrawText(coords, word);		
		}
	}
}


void CDjVuFile::TextToRenderer(AVSGraphics::IAVSRenderer* renderer, XmlUtils::CXmlNode text, double koef, bool isView)
{
	renderer->put_ShadowVisible(0);

	// выставим шрифт пустой (чтобы раст€гивать по всему ректу)
	renderer->put_FontName(L"AVSEmptyFont");

	CString p = text.GetXml();
	XmlUtils::CXmlNodes lines_node;
	text.GetNodes(_T("LINE"), lines_node);
	for(int i = 0; i < lines_node.GetCount(); ++i)
	{
		XmlUtils::CXmlNode line_node;
		lines_node.GetAt(i, line_node);
		XmlUtils::CXmlNodes words_node;
		line_node.GetNodes(_T("WORD"), words_node);
		for(int j = 0; j < words_node.GetCount(); ++j)
		{
			XmlUtils::CXmlNode word_node;
			words_node.GetAt(j, word_node);
			CString word = word_node.GetText();
			CString coordsStr = word_node.GetAttribute(_T("coords"));
			double coords[4];
			ParseCoords(coordsStr, coords, koef);		
			DrawText(renderer, coords, word);		
		}
	}
}


void CDjVuFile::ImageToRenderer(const CString& strName, double width, double height)
{
	BSTR bsPath = strName.AllocSysString();
	m_pDocumentRenderer->DrawImageFromFile(bsPath, 0, 0, (float)width, (float)height);
	SysFreeString(bsPath);
}


void CDjVuFile::ImageToRenderer(AVSGraphics::IAVSRenderer* renderer, const CString& strName, double width, double height)
{
	BSTR bsPath = strName.AllocSysString();
	renderer->DrawImageFromFile(bsPath, 0, 0, (float)width, (float)height);
	SysFreeString(bsPath);
}


void CDjVuFile::ParseCoords(CString& coordsStr, double* coords, double koef)
{
	CString trimStr = coordsStr + _T(",");
	for(int k = 0; k < 4; ++k)
	{
		coords[k] = XmlUtils::GetInteger(trimStr.Mid(0, trimStr.Find(_T(",")))) * koef;
		if (k != 3)
		{
			trimStr = trimStr.Mid(trimStr.Find(_T(",")) + 1);
		}
	}
}


void CDjVuFile::DrawRect(double* coords)
{
	m_pDocumentRenderer->PathCommandMoveTo(coords[0], coords[3]);
	m_pDocumentRenderer->PathCommandLineTo(coords[2], coords[3]);
	m_pDocumentRenderer->PathCommandLineTo(coords[2], coords[1]);
	m_pDocumentRenderer->PathCommandLineTo(coords[0], coords[1]);
	m_pDocumentRenderer->PathCommandClose();
	m_pDocumentRenderer->DrawPath(1);
	m_pDocumentRenderer->PathCommandEnd();
}


void CDjVuFile::DrawRect(AVSGraphics::IAVSRenderer* renderer, double* coords)
{
	renderer->PathCommandMoveTo(coords[0], coords[3]);
	renderer->PathCommandLineTo(coords[2], coords[3]);
	renderer->PathCommandLineTo(coords[2], coords[1]);
	renderer->PathCommandLineTo(coords[0], coords[1]);
	renderer->PathCommandClose();
	renderer->DrawPath(1);
	renderer->PathCommandEnd();
}


void CDjVuFile::DrawText(double* coords, CString text)
{
	CString strFont;
	strFont.Format(_T("<font name='Arial' size='%lf'/>"), min( 100 , max( 1, fabs((float)(coords[3] - coords[1]))) * 72 / 25.4));
	BSTR bsFont = strFont.AllocSysString();
	m_pDocumentRenderer->SetFont(bsFont);
	SysFreeString(bsFont);
	BSTR bsText = text.AllocSysString();
	m_pDocumentRenderer->CommandDrawText(bsText, 
		(float)coords[0], 
		(float)coords[3], 
		(float)(coords[2] - coords[0]),
		(float)(coords[1] - coords[3]), 
		(float)(coords[1] - coords[3]));
	SysFreeString(bsText);
}

void CDjVuFile::DrawText(AVSGraphics::IAVSRenderer* renderer, double* coords, CString text)
{
	/*CString strFont;
	strFont.Format(_T("<font name='Arial' size='%lf'/>"), min( 100 , max( 1, fabs((float)(coords[3] - coords[1]))) * 72 / 25.4));
	BSTR bsFont = strFont.AllocSysString();
	renderer->SetFont(bsFont);
	SysFreeString(bsFont);*/
	renderer->put_FontSize(coords[1] - coords[3]);
	BSTR bsText = text.AllocSysString();
	renderer->CommandDrawText(bsText, 
		(float)coords[0], 
		(float)coords[3], 
		(float)(coords[2] - coords[0]),
		(float)(coords[1] - coords[3]), 
		(float)(coords[1] - coords[3]));
	SysFreeString(bsText);
}