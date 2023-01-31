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

#include "../PdfFile/PdfFile.h"

#include "../DesktopEditor/graphics/pro/Fonts.h"
#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "../DesktopEditor/graphics/pro/Image.h"

#include "../DesktopEditor/common/StringExt.h"

#define VER_DPI		96
#define HOR_DPI		96

#include <vector>

#include "../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

#ifdef BUILDING_WASM_MODULE
#define DISABLE_TEMP_DIRECTORY
#endif

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
    static int GetInteger(const std::wstring& wsString)
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
    m_wsTempDirectory = L"";
    SetTempDirectory(L"");
    m_pApplicationFonts = pFonts;
}
CDjVuFileImplementation::~CDjVuFileImplementation()
{
#ifndef DISABLE_TEMP_DIRECTORY
	NSDirectory::DeleteDirectory(m_wsTempDirectory);
#endif
}
NSFonts::IApplicationFonts* CDjVuFileImplementation::GetFonts()
{
    return m_pApplicationFonts;
}

bool CDjVuFileImplementation::LoadFromFile(const std::wstring& wsSrcFileName, const std::wstring& wsXMLOptions)
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
bool CDjVuFileImplementation::LoadFromMemory(BYTE* data, DWORD length, const std::wstring& wsXmlOptions)
{
    m_pDoc = NULL;
    try
    {
        GP<ByteStream> stream = ByteStream::create(data, (size_t)length);
        m_pDoc = DjVuDocument::create(stream);
        m_pDoc->wait_get_pages_num();
    }
    catch (...)
    {
        return false;
    }

    return true;
}
void CDjVuFileImplementation::Close()
{
}
std::wstring CDjVuFileImplementation::GetTempDirectory() const
{
	return m_wsTempDirectory;
}
void CDjVuFileImplementation::SetTempDirectory(const std::wstring& wsDirectory)
{
#ifndef DISABLE_TEMP_DIRECTORY
    if (!m_wsTempDirectory.empty())
        NSDirectory::DeleteDirectory(m_wsTempDirectory);

    m_wsTempDirectory = wsDirectory;
    if (m_wsTempDirectory.empty())
        m_wsTempDirectory = NSFile::CFileBinary::GetTempPath();

    m_wsTempDirectory += L"/DJVU/";
	NSDirectory::CreateDirectory(m_wsTempDirectory);
#endif
}

int CDjVuFileImplementation::GetPagesCount() const
{
	if (!m_pDoc)
		return 0;

	return m_pDoc->get_pages_num();
}
void CDjVuFileImplementation::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY) const
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
void  CDjVuFileImplementation::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
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

#ifndef BUILDING_WASM_MODULE
        if (c_nPDFWriter == lRendererType)
		{
			XmlUtils::CXmlNode oText = ParseText(pPage);
			CreatePdfFrame(pRenderer, pPage, nPageIndex, oText);
		}
        else
#endif
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
void CDjVuFileImplementation::ConvertToPdf(const std::wstring& wsDstPath)
{
    CPdfFile oPdf(m_pApplicationFonts);
    oPdf.CreatePdf();
	
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
#ifdef _DEBUG
		printf("%d of %d pages\n", nPageIndex + 1, nPagesCount);
#endif
	}

	oPdf.SaveToFile(wsDstPath);
}
std::wstring CDjVuFileImplementation::GetInfo()
{
    std::wstring sRes = L"{";

    double nW = 0;
    double nH = 0;
    double nDpi = 0;
    GetPageInfo(0, &nW, &nH, &nDpi, &nDpi);
    sRes += L"\"PageWidth\":";
    sRes += std::to_wstring((int)(nW * 100));
    sRes += L",\"PageHeight\":";
    sRes += std::to_wstring((int)(nH * 100));
    sRes += L",\"NumberOfPages\":";
    sRes += std::to_wstring(GetPagesCount());
    sRes += L"}";

    return sRes;
}

void getBookmars(const GP<DjVmNav>& nav, int& pos, int count, NSWasm::CData& out, int level)
{
    while (count > 0 && pos < nav->getBookMarkCount())
    {
        GP<DjVmNav::DjVuBookMark> gpBookMark;
        nav->getBookMark(gpBookMark, pos++);

        GUTF8String str = gpBookMark->url;
        int endpos;
        DWORD nPage = str.toULong(1, endpos) - 1;
        if (endpos == (int)str.length())
        {
            out.AddInt(nPage);
            out.AddInt(level);
            out.AddDouble(0.0);
            GUTF8String description = gpBookMark->displayname;
            out.WriteString((BYTE*)description.getbuf(), description.length());
        }

        getBookmars(nav, pos, gpBookMark->count, out, level + 1);
        count--;
    }
}
BYTE* CDjVuFileImplementation::GetStructure()
{
    GP<DjVmNav> nav = m_pDoc->get_djvm_nav();
    if (!nav)
        return NULL;

    int pos = 0;
    int count = nav->getBookMarkCount();
    if (count <= 0)
        return NULL;

    NSWasm::CData oRes;
    oRes.SkipLen();
    getBookmars(nav, pos, count, oRes, 1);
    oRes.WriteLen();
    BYTE* bRes = oRes.GetBuffer();
    oRes.ClearWithoutAttack();
    return bRes;
}
BYTE* CDjVuFileImplementation::GetPageGlyphs(int nPageIndex)
{
    return NULL;
    try
    {
        GP<DjVuImage> pPage = m_pDoc->get_page(nPageIndex);
        const GP<DjVuText> text(DjVuText::create());
        const GP<ByteStream> text_str(pPage->get_text());
        if (!text_str)
            return NULL;
        text->decode(text_str);

        GUTF8String pageText = text->get_xmlText(pPage->get_height());
        XmlUtils::CXmlNode hiddenText;
        XmlUtils::CXmlNode pageColumn;
        XmlUtils::CXmlNode region;
        hiddenText.FromXmlStringA(NSDjvu::MakeCString(pageText));
        hiddenText.GetNode(L"PAGECOLUMN", pageColumn);
        pageColumn.GetNode(L"REGION", region);

        NSWasm::CData oRes;
        oRes.SkipLen();
        XmlUtils::CXmlNodes oParagraphsNodes;
        region.GetNodes(L"PARAGRAPH", oParagraphsNodes);
        for (int nParagraphIndex = 0; nParagraphIndex < oParagraphsNodes.GetCount(); nParagraphIndex++)
        {
            XmlUtils::CXmlNode oParagraphNode;
            oParagraphsNodes.GetAt(nParagraphIndex, oParagraphNode);
            XmlUtils::CXmlNodes oLinesNodes;
            oParagraphNode.GetNodes(L"LINE", oLinesNodes);
            for (int nLineIndex = 0; nLineIndex < oLinesNodes.GetCount(); nLineIndex++)
            {
                XmlUtils::CXmlNode oLineNode;
                oLinesNodes.GetAt(nLineIndex, oLineNode);
                XmlUtils::CXmlNodes oWordsNodes;
                oLineNode.GetNodes(L"WORD", oWordsNodes);
                for (int nWordIndex = 0; nWordIndex < oWordsNodes.GetCount(); nWordIndex++)
                {
                    XmlUtils::CXmlNode oWordNode;
                    oWordsNodes.GetAt(nWordIndex, oWordNode);
                    std::wstring csWord   = oWordNode.GetText();
                    std::wstring csCoords = oWordNode.GetAttribute(L"coords");
                    double arrCoords[4];
                    ParseCoords(csCoords, arrCoords, 1);

                    std::string sText = U_TO_UTF8(csWord);
                    oRes.WriteString((BYTE*)sText.c_str(), sText.length());
                    oRes.AddDouble(arrCoords[0]);
                    oRes.AddDouble(arrCoords[3]);
                    oRes.AddDouble(arrCoords[2] - arrCoords[0]);
                    oRes.AddDouble(arrCoords[1] - arrCoords[3]);
                }
            }
        }
        oRes.WriteLen();

        BYTE* res = oRes.GetBuffer();
        oRes.ClearWithoutAttack();
        return res;
    }
    catch (...) {}
    return NULL;
}
BYTE* CDjVuFileImplementation::GetPageLinks(int nPageIndex)
{
    double dPageDpiX, dPageDpiY;
    double dWidth, dHeight;
    GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

    try
    {
        GP<DjVuImage> pPage = m_pDoc->get_page(nPageIndex);
        pPage->wait_for_complete_decode();
        GP<DjVuAnno> pAnno = pPage->get_decoded_anno();
        if (!pAnno)
            return NULL;
        GPList<GMapArea> map_areas = pAnno->ant->map_areas;

        NSWasm::CData oRes;
        oRes.SkipLen();
        for (GPosition pos(map_areas); pos; ++pos)
        {
            GUTF8String str = map_areas[pos]->url;
            oRes.WriteString((BYTE*)str.getbuf(), str.length());
            // Верхний левый угол
            double x = map_areas[pos]->get_xmin();
            double y = dHeight - map_areas[pos]->get_ymax();
            oRes.AddDouble(0.0);
            oRes.AddDouble(x);
            oRes.AddDouble(y);
            oRes.AddDouble(map_areas[pos]->get_xmax() - x);
            oRes.AddDouble(map_areas[pos]->get_ymax() - map_areas[pos]->get_ymin());
        }
        oRes.WriteLen();

        BYTE* res = oRes.GetBuffer();
        oRes.ClearWithoutAttack();
        return res;
    }
    catch (...) {}
    return NULL;
}

void CDjVuFileImplementation::CreateFrame(IRenderer* pRenderer, GP<DjVuImage>& pPage, int nPage, XmlUtils::CXmlNode& text)
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
void CDjVuFileImplementation::CreatePdfFrame(IRenderer* pRenderer, GP<DjVuImage>& pPage, int nPageIndex, XmlUtils::CXmlNode& oText)
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

	CPdfFile* pPdf = (CPdfFile*)pRenderer;

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
void CDjVuFileImplementation::TextToRenderer(IRenderer* pRenderer, XmlUtils::CXmlNode oTextNode, double dKoef, bool isView)
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
void CDjVuFileImplementation::DrawPageText(IRenderer* pRenderer, double* pdCoords, const std::wstring& wsText)
{
	pRenderer->put_FontSize(pdCoords[1] - pdCoords[3]);
	pRenderer->CommandDrawText(wsText,
							   (float)(pdCoords[0]),
							   (float)(pdCoords[3]),
							   (float)(pdCoords[2] - pdCoords[0]),
							   (float)(pdCoords[1] - pdCoords[3]));
}
void CDjVuFileImplementation::ParseCoords(const std::wstring& wsCoordsStr, double* pdCoords, double dKoef)
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
