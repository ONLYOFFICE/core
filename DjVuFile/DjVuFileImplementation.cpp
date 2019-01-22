/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "DjVuFileImplementation.h"

#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Directory.h"

#include "../PdfWriter/PdfRenderer.h"

#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "../DesktopEditor/graphics/pro/Image.h"

#include "../DesktopEditor/common/StringExt.h"

#define VER_DPI		96
#define HOR_DPI		96

#include <vector>

namespace NSDjvu
{
	static GUTF8String MakeUTF8String(const std::wstring& wsText)
	{
        std::string sText = NSFile::CUtf8Converter::GetUtf8StringFromUnicode(wsText);
        GUTF8String utf8String(sText.c_str());
        return utf8String;
	}
    static std::string MakeCString(GUTF8String& strText)
	{
		std::string sString(strText.getbuf());
        return sString;
	}
	static int     GetInteger(const std::wstring& wsString)
	{
		if (wsString.size() < 1)
			return 0;

		try
		{
            return std::stoi(wsString);
		}
		catch (...)
		{
		}

		try
		{
            return static_cast<int>(std::stol(wsString));
		}
		catch (...)
		{
			return 0;
		}
	}
}

CDjVuFileImplementation::CDjVuFileImplementation(NSFonts::IApplicationFonts* pFonts)
{
	m_pDoc = NULL;
	std::wstring wsTempPath = NSFile::CFileBinary::GetTempPath();
	wsTempPath += L"DJVU\\";
	m_wsTempDirectory = wsTempPath;
	NSDirectory::CreateDirectory(m_wsTempDirectory);

    m_pApplicationFonts = pFonts;
}
CDjVuFileImplementation::~CDjVuFileImplementation()
{
	NSDirectory::DeleteDirectory(m_wsTempDirectory);
}
bool               CDjVuFileImplementation::LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXMLOptions)
{
	m_pDoc = NULL;
	try
	{
		GUTF8String utf8;
		GURL url = GURL::Filename::UTF8(NSDjvu::MakeUTF8String(wsSrcFileName));
		m_pDoc = DjVuDocument::create(url);
		m_pDoc->wait_get_pages_num();
	}
	catch (...)
	{
		return false;
	}

	return true;
}
void               CDjVuFileImplementation::Close()
{
}
std::wstring       CDjVuFileImplementation::GetTempDirectory() const
{
	return m_wsTempDirectory;
}
void               CDjVuFileImplementation::SetTempDirectory(const std::wstring& wsDirectory)
{
	NSDirectory::DeleteDirectory(m_wsTempDirectory);

	m_wsTempDirectory = wsDirectory;
	m_wsTempDirectory += L"\\DJVU\\";
	NSDirectory::CreateDirectory(m_wsTempDirectory);
}
int                CDjVuFileImplementation::GetPagesCount() const
{
	if (!m_pDoc)
		return 0;

	return m_pDoc->get_pages_num();
}
void               CDjVuFileImplementation::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) const
{
	if (!m_pDoc)
	{
		*pdWidth  = 0;
		*pdHeight = 0;

		*pdDpiX = 96;
		*pdDpiY = 96;
	}

#if 0
	GP<DjVuImage> pPage = m_pDoc->get_page(nPageIndex);

	pPage->wait_for_complete_decode();
	pPage->set_rotate(0);

	*pdWidth = pPage->get_real_width();
	*pdHeight = pPage->get_real_height();

	*pdDpiX = pPage->get_dpi();
	*pdDpiY = pPage->get_dpi();
#endif
    int nW = 0;
    int nH = 0;
    int nDpi = 0;
    m_pDoc->ReadPageInfo(nPageIndex, nW, nH, nDpi);
    *pdWidth = nW;
    *pdHeight = nH;
    *pdDpiX = nDpi;
    *pdDpiY = nDpi;
}
void               CDjVuFileImplementation::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
	if (!m_pDoc)
		return;

	try
	{
		GP<DjVuImage> pPage = m_pDoc->get_page(nPageIndex);
		pPage->wait_for_complete_decode();
		pPage->set_rotate(0);

		long lRendererType = c_nUnknownRenderer;
		pRenderer->get_Type(&lRendererType);
		if (false)//c_nGrRenderer == lRendererType)
		{
			CreateGrFrame(pRenderer, pPage, pBreak);
		}
		else if (c_nPDFWriter == lRendererType)
		{
			XmlUtils::CXmlNode oText = ParseText(pPage);
			CreatePdfFrame(pRenderer, pPage, nPageIndex, oText);
		}
		else
		{
			XmlUtils::CXmlNode oText = ParseText(pPage);
			CreateFrame(pRenderer, pPage, nPageIndex, oText);
		}
	}
	catch (...)
	{
		// белая страница
	}
}
void               CDjVuFileImplementation::ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType, const int& nRasterW, const int& nRasterH)
{
    if (!m_pApplicationFonts)
        return;

    NSFonts::IFontManager *pFontManager = m_pApplicationFonts->GenerateFontManager();
    NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
    pFontCache->SetStreams(m_pApplicationFonts->GetStreams());
	pFontManager->SetOwnerCache(pFontCache);

    NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
    pRenderer->SetFontManager(pFontManager);

	double dPageDpiX, dPageDpiY;
	double dWidth, dHeight;
	GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiX);

    int nWidth  = (nRasterW > 0) ? nRasterW : ((int)dWidth * 96 / dPageDpiX);
    int nHeight = (nRasterH > 0) ? nRasterH : ((int)dHeight * 96 / dPageDpiX);

	BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
	if (!pBgraData)
		return;

	memset(pBgraData, 0xff, nWidth * nHeight * 4);
	CBgraFrame oFrame;
	oFrame.put_Data(pBgraData);
	oFrame.put_Width(nWidth);
	oFrame.put_Height(nHeight);
	oFrame.put_Stride(-4 * nWidth);

    pRenderer->CreateFromBgraFrame(&oFrame);
    pRenderer->SetSwapRGB(false);
    pRenderer->put_Width(dWidth);
    pRenderer->put_Height(dHeight);

	bool bBreak = false;
    DrawPageOnRenderer(pRenderer, nPageIndex, &bBreak);

	oFrame.SaveFile(wsDstPath, nImageType);
	RELEASEINTERFACE(pFontManager);
    RELEASEOBJECT(pRenderer);
}
void               CDjVuFileImplementation::ConvertToPdf(const std::wstring& wsDstPath)
{
    CPdfRenderer oPdf(m_pApplicationFonts);
	
	bool bBreak = false;
	for (int nPageIndex = 0, nPagesCount = GetPagesCount(); nPageIndex < nPagesCount; nPageIndex++)
	{
		oPdf.NewPage();

		double dPageDpiX, dPageDpiY;
		double dWidth, dHeight;
		GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
		dWidth  *= 25.4 / dPageDpiX;
		dHeight *= 25.4 / dPageDpiY;
		oPdf.put_Width(dWidth);
		oPdf.put_Height(dHeight);

		DrawPageOnRenderer(&oPdf, nPageIndex, &bBreak);
//#ifdef _DEBUG
		printf("%d of %d pages\n", nPageIndex + 1, nPagesCount);
//#endif
	}

	oPdf.SaveToFile(wsDstPath);
}
void               CDjVuFileImplementation::CreateFrame(IRenderer* pRenderer, GP<DjVuImage>& pPage, int nPage, XmlUtils::CXmlNode& text)
{
	int nWidth	= pPage->get_real_width();
	int nHeight	= pPage->get_real_height();

	int nDpi = pPage->get_dpi();

	double dPixToMM = 25.4;

	double dRendDpiX = 0;
	double dRendDpiY = 0;

	pRenderer->get_DpiX(&dRendDpiX);
	pRenderer->get_DpiY(&dRendDpiY);

	if (0 >= dRendDpiX)
		dRendDpiX = 72.0;
	if (0 >= dRendDpiY)
		dRendDpiY = 72.0;

	double dRendWidth = 0;
	double dRendHeight = 0;

	pRenderer->get_Width(&dRendWidth);
	pRenderer->get_Height(&dRendHeight);

	if (0 >= dRendWidth)
		dRendWidth = 200;
	if (0 >= dRendHeight)
		dRendHeight = 300;

	LONG lImageWidth	= (LONG)(dRendDpiX * dRendWidth / dPixToMM);
	LONG lImageHeight	= (LONG)(dRendDpiY * dRendHeight / dPixToMM);

	long lRendererType = c_nUnknownRenderer;
	pRenderer->get_Type(&lRendererType);
	if (c_nPDFWriter == lRendererType)
	{
		lImageWidth	 = pPage->get_real_width();
		lImageHeight = pPage->get_real_height();
	}
	else if (c_nHtmlRendrerer == lRendererType)
	{
		// TODO: Нужно реализовать функцию 
		// pRenderer->GetMaxImageSize();
		//VARIANT var;
		//renderer->GetAdditionalParam(L"MaxImageSize", &var);
		//LONG lMaxWidth = var.lVal;
		//if ((lImageWidth > lMaxWidth) || (lImageHeight > lMaxWidth))
		//{
		//	double dAspect = (double)(lImageWidth) / lImageHeight;
		//	if (lImageWidth > lImageHeight)
		//	{
		//		lImageWidth  = lMaxWidth;
		//		lImageHeight = (LONG)(lImageHeight / dAspect);
		//	}
		//	else
		//	{
		//		lImageHeight = lMaxWidth;
		//		lImageWidth  = (LONG)(dAspect * lImageHeight);
		//	}
		//}
	}

	BYTE* pBufferDst = new BYTE[4 * lImageHeight * lImageWidth];
	if (!pBufferDst)
		return;

	Aggplus::CImage oImage;
	oImage.Create(pBufferDst, lImageWidth, lImageHeight, 4 * lImageWidth);
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

        unsigned int* palette = new unsigned int[nPaletteEntries];

		// Create palette for the bitmap
		int color = 0xff0000;
		int decrement = color / (nPaletteEntries - 1);
		for (int i = 0; i < nPaletteEntries; ++i)
		{
			BYTE level = (BYTE)(color >> 16);
			palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
			color -= decrement;
		}

        unsigned int* pBuffer = (unsigned int*)pBufferDst;
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

                unsigned int* palette = new unsigned int[nPaletteEntries];

				// Create palette for the bitmap
				int color = 0xff0000;
				int decrement = color / (nPaletteEntries - 1);
				for (int i = 0; i < nPaletteEntries; ++i)
				{
					BYTE level = (BYTE)(color >> 16);
					palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
					color -= decrement;
				}

                unsigned int* pBuffer = (unsigned int*)pBufferDst;
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

	pRenderer->BeginCommand(c_nPageType);

	if (c_nGrRenderer != lRendererType && c_nHtmlRendrerer != lRendererType && c_nHtmlRendrerer2 != lRendererType)
	{
		TextToRenderer(pRenderer, text, dPixToMM / nDpi);
	}

	pRenderer->DrawImage((IGrObject*)&oImage, 0, 0, dRendWidth, dRendHeight);
	pRenderer->EndCommand(c_nPageType);
}
void               CDjVuFileImplementation::CreatePdfFrame(IRenderer* pRenderer, GP<DjVuImage>& pPage, int nPageIndex, XmlUtils::CXmlNode& oText)
{
	double dPageDpiX, dPageDpiY;
	double dWidth, dHeight;
	GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
	dWidth  *= 25.4 / dPageDpiX;
	dHeight *= 25.4 / dPageDpiY;

	pRenderer->BeginCommand(c_nPageType);

	TextToRenderer(pRenderer, oText, 25.4 / pPage->get_dpi());

	LONG lImageWidth  = pPage->get_real_width();
	LONG lImageHeight = pPage->get_real_height();

	CPdfRenderer* pPdf = (CPdfRenderer*)pRenderer;

	if (pPage->is_legal_photo())
	{
		BYTE* pBufferDst = new BYTE[4 * lImageHeight * lImageWidth];
		if (!pBufferDst)
			return;

		Aggplus::CImage oImage;
		oImage.Create(pBufferDst, lImageWidth, lImageHeight, 4 * lImageWidth);

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
		pRenderer->DrawImage((IGrObject*)&oImage, 0, 0, dWidth, dHeight);
	}
	else if (pPage->is_legal_compound())
	{
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);
		GP<IW44Image> pIW44Image = pPage->get_bg44();
		if (NULL != pIW44Image)
		{
			int nBgWidth  = pIW44Image->get_width();
			int nBgHeight = pIW44Image->get_height();

			GP<GPixmap> pBgImage = pIW44Image->get_pixmap();
			if (NULL != pBgImage)
			{
				BYTE* pBgBuffer = new BYTE[4 * nBgWidth * nBgHeight];
				if (!pBgBuffer)
					return;

				Aggplus::CImage oBgImage;
				oBgImage.Create(pBgBuffer, nBgWidth, nBgHeight, 4 * nBgWidth);

				BYTE* pBuffer = pBgBuffer;
				for (int j = nBgHeight - 1; j >= 0; --j)
				{
					GPixel* pLine = pBgImage->operator [](j);

					for (int i = 0; i < nBgWidth; ++i, pBuffer += 4, ++pLine)
					{
						pBuffer[0] = pLine->b;
						pBuffer[1] = pLine->g;
						pBuffer[2] = pLine->r;
						pBuffer[3] = 255;
					}
				}
				pRenderer->DrawImage((IGrObject*)&oBgImage, 0, 0, dWidth, dHeight);
			}
		}
		
		GP<GPixmap> pImage = pPage->get_fgpm();
		if (NULL == pImage)
			pImage = pPage->get_fg_pixmap(oRectAll);

		if (NULL != pImage)
		{
			unsigned int unPixmapH = pImage->rows();
			unsigned int unPixmapW = pImage->columns();
			BYTE* pBufferDst = new BYTE[4 * unPixmapH * unPixmapW];
			if (!pBufferDst)
				return;

			Aggplus::CImage oImage;
			oImage.Create(pBufferDst, unPixmapW, unPixmapH, 4 * unPixmapW);

			BYTE* pBuffer = pBufferDst;
			for (int j = unPixmapH - 1; j >= 0; --j)
			{
				GPixel* pLine = pImage->operator [](j);

				for (int i = 0; i < unPixmapW; ++i, pBuffer += 4, ++pLine)
				{
					pBuffer[0] = pLine->b;
					pBuffer[1] = pLine->g;
					pBuffer[2] = pLine->r;
					pBuffer[3] = 255;
				}
			}

			GP<GBitmap> pBitmap = pPage->get_bitmap(oRectAll, oRectAll, 4);

            NSImages::CPixJbig2 oPix;
            if (oPix.Create(lImageWidth, lImageHeight, 1))
			{
				for (int nY = 0; nY < lImageHeight; nY++)
				{
					BYTE* pLine = pBitmap->operator [](nY);
					for (int nX = 0; nX < lImageWidth; nX++, pLine++)
					{
                        oPix.SetPixel(nX, lImageHeight - 1 - nY, *pLine);
					}
				}

                pPdf->DrawImageWith1bppMask((IGrObject*)&oImage, &oPix, lImageWidth, lImageHeight, 0, 0, dWidth, dHeight);
                oPix.Destroy();
			}
		}		
	}	
	else if (pPage->is_legal_bilevel())
	{
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);
		GP<GBitmap> pBitmap = pPage->get_bitmap(oRectAll, oRectAll, 4);

        NSImages::CPixJbig2 oPix;
        if (oPix.Create(lImageWidth, lImageHeight, 1))
		{
			for (int nY = 0; nY < lImageHeight; nY++)
			{
				BYTE* pLine = pBitmap->operator [](nY);
				for (int nX = 0; nX < lImageWidth; nX++, pLine++)
				{
                    oPix.SetPixel(nX, lImageHeight - 1 - nY, *pLine);
				}
			}

            pPdf->DrawImage1bpp(&oPix, lImageWidth, lImageHeight, 0, 0, dWidth, dHeight);
            oPix.Destroy();
		}
	}
	else
	{
		// белый фрейм??
		//memset(pBufferDst, 0xFF, 4 * lImageWidth * lImageHeight);
		GRect oRectAll(0, 0, lImageWidth, lImageHeight);
		GP<GPixmap> pImage = pPage->get_pixmap(oRectAll, oRectAll);

		if (NULL != pImage)
		{
			BYTE* pBufferDst = new BYTE[4 * lImageHeight * lImageWidth];
			if (pBufferDst)
			{
				Aggplus::CImage oImage;
				oImage.Create(pBufferDst, lImageWidth, lImageHeight, 4 * lImageWidth);

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
				pRenderer->DrawImage((IGrObject*)&oImage, 0, 0, dWidth, dHeight);
			}
		}
		else
		{
			GP<GBitmap> pBitmap = pPage->get_bitmap(oRectAll, oRectAll, 4);
			if (NULL != pBitmap)
			{
				int nPaletteEntries = pBitmap->get_grays();
				if (nPaletteEntries <= 2)
				{
                    NSImages::CPixJbig2 oPix;
                    if (oPix.Create(lImageWidth, lImageHeight, 1))
					{
						for (int nY = 0; nY < lImageHeight; nY++)
						{
							BYTE* pLine = pBitmap->operator [](nY);
							for (int nX = 0; nX < lImageWidth; nX++, pLine++)
							{
                                oPix.Create(nX, lImageHeight - 1 - nY, *pLine);
							}
						}

                        pPdf->DrawImage1bpp(&oPix, lImageWidth, lImageHeight, 0, 0, dWidth, dHeight);
                        oPix.Destroy();
					}
				}
				else
				{
					BYTE* pBufferDst = new BYTE[4 * lImageHeight * lImageWidth];
					if (!pBufferDst)
						return;

					Aggplus::CImage oImage;
					oImage.Create(pBufferDst, lImageWidth, lImageHeight, 4 * lImageWidth);

                    unsigned int* palette = new unsigned int[nPaletteEntries];

					// Create palette for the bitmap
					int color = 0xff0000;
					int decrement = color / (nPaletteEntries - 1);
					for (int i = 0; i < nPaletteEntries; ++i)
					{
						BYTE level = (BYTE)(color >> 16);
						palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
						color -= decrement;
					}

                    unsigned int* pBuffer = (unsigned int*)pBufferDst;
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
					pRenderer->DrawImage((IGrObject*)&oImage, 0, 0, dWidth, dHeight);
				}
			}
		}
	}

	pRenderer->EndCommand(c_nPageType);
}
void               CDjVuFileImplementation::CreateGrFrame(IRenderer* pRenderer, GP<DjVuImage>& pPage, bool* pBreak)
{
	int nWidth	= pPage->get_real_width();
	int nHeight	= pPage->get_real_height();

	BYTE*	pBufferDst	 = NULL;
	LONG	lImageWidth	 = 0;
	LONG	lImageHeight = 0;

	// TODO: Реализовать для графического рендерера

	//VARIANT var;
	//renderer->GetAdditionalParam(L"Pixels", &var);
	//pBufferDst = (BYTE*)var.lVal;

	//renderer->GetAdditionalParam(L"PixelsWidth", &var);
	//lImageWidth  = var.lVal;
	//renderer->GetAdditionalParam(L"PixelsHeight", &var);
	//lImageHeight = var.lVal;

	volatile bool* pCancel = pBreak;

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

        unsigned int* palette = new unsigned int[nPaletteEntries];

		// Create palette for the bitmap
		int color = 0xff0000;
		int decrement = color / (nPaletteEntries - 1);
		for (int i = 0; i < nPaletteEntries; ++i)
		{
			BYTE level = (BYTE)(color >> 16);
			palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
			color -= decrement;
		}

        unsigned int* pBuffer = (unsigned int*)pBufferDst;
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

            unsigned int* palette = new unsigned int[nPaletteEntries];

			// Create palette for the bitmap
			int color = 0xff0000;
			int decrement = color / (nPaletteEntries - 1);
			for (int i = 0; i < nPaletteEntries; ++i)
			{
				BYTE level = (BYTE)(color >> 16);
				palette[i] = (0xFF000000 | level << 16 | level << 8 | level);
				color -= decrement;
			}

            unsigned int* pBuffer = (unsigned int*)pBufferDst;
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
XmlUtils::CXmlNode CDjVuFileImplementation::ParseText(GP<DjVuImage> pPage)
{
	XmlUtils::CXmlNode paragraph;
	const GP<DjVuText> text(DjVuText::create());
	const GP<ByteStream> text_str(pPage->get_text());
	if (text_str)
	{
		text->decode(text_str);
		GUTF8String pageText = text->get_xmlText(pPage->get_height());
		XmlUtils::CXmlNode hiddenText;
		XmlUtils::CXmlNode pageColumn;
		XmlUtils::CXmlNode region;
        hiddenText.FromXmlStringA(NSDjvu::MakeCString(pageText));
        hiddenText.GetNode(L"PAGECOLUMN", pageColumn);
        pageColumn.GetNode(L"REGION", region);
        region.GetNode(L"PARAGRAPH", paragraph);
	}
	return paragraph;
}
void               CDjVuFileImplementation::TextToRenderer(IRenderer* pRenderer, XmlUtils::CXmlNode oTextNode, double dKoef, bool isView)
{
	// Выставим шрифт пустой (чтобы растягивать по всему ректу)
	pRenderer->put_FontName(L"DjvuEmptyFont");
    //std::wstring csText = oTextNode.GetXml();
	XmlUtils::CXmlNodes oLinesNodes;
	oTextNode.GetNodes(L"LINE", oLinesNodes);
	for (int nLineIndex = 0; nLineIndex < oLinesNodes.GetCount(); ++nLineIndex)
	{
		XmlUtils::CXmlNode oLineNode;
		oLinesNodes.GetAt(nLineIndex, oLineNode);
		XmlUtils::CXmlNodes oWordsNodes;
		oLineNode.GetNodes(L"WORD", oWordsNodes);
		for (int nWordIndex = 0; nWordIndex < oWordsNodes.GetCount(); ++nWordIndex)
		{
			XmlUtils::CXmlNode oWordNode;
			oWordsNodes.GetAt(nWordIndex, oWordNode);
            std::wstring csWord   = oWordNode.GetText();
            std::wstring csCoords = oWordNode.GetAttribute(L"coords");
			double arrCoords[4];
            ParseCoords(csCoords, arrCoords, dKoef);
            DrawPageText(pRenderer, arrCoords, csWord);
		}
	}
}
void               CDjVuFileImplementation::DrawPageText(IRenderer* pRenderer, double* pdCoords, const std::wstring& wsText)
{
	pRenderer->put_FontSize(pdCoords[1] - pdCoords[3]);
	pRenderer->CommandDrawText(wsText,
							   (float)(pdCoords[0]),
							   (float)(pdCoords[3]),
							   (float)(pdCoords[2] - pdCoords[0]),
							   (float)(pdCoords[1] - pdCoords[3]));
}
void               CDjVuFileImplementation::ParseCoords(const std::wstring& wsCoordsStr, double* pdCoords, double dKoef)
{
    std::vector<std::wstring> vCoords = NSStringExt::Split(wsCoordsStr, L',');
	if (vCoords.size() >= 4)
	{
		for (int nIndex = 0; nIndex < 4; nIndex++)
		{
			pdCoords[nIndex] = NSDjvu::GetInteger(vCoords.at(nIndex)) * dKoef;
		}
	}
}
