/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
//#include <QCoreApplication>

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../PdfWriter/PdfRenderer.h"
#include "../include/HTMLRenderer3.h"

#include "../include/ASCSVGWriter.h"
#include "../../DesktopEditor/raster/Metafile/MetaFile.h"

//#define METAFILE_TEST
//#define ONLINE_WORD_TO_PDF
//#define TO_PDF
#define TO_HTML_RENDERER
#define ONLY_TEXT

int main(int argc, char *argv[])
{
    CApplicationFonts oFonts;
    oFonts.Initialize();

#ifdef METAFILE_TEST

    NSHtmlRenderer::CASCSVGWriter oWriterSVG;
    oWriterSVG.SetFontManager(oFonts.GenerateFontManager());

    MetaFile::CMetaFile oMetafile(&oFonts);

    //oMetafile.LoadFromFile(L"D:\\2\\ppt\\media\\image4.wmf");
    oMetafile.LoadFromFile(L"/home/oleg/activex/1/image2.wmf");

    double x = 0, y = 0, w = 0, h = 0;
    oMetafile.GetBounds(&x, &y, &w, &h);

    double _max = (w >= h) ? w : h;
    double dKoef = 100000.0 / _max;

    int WW = (int)(dKoef * w + 0.5);
    int HH = (int)(dKoef * h + 0.5);

    oWriterSVG.put_Width(WW);
    oWriterSVG.put_Height(HH);
    oMetafile.DrawOnRenderer(&oWriterSVG, 0, 0, WW, HH);

    oWriterSVG.SaveFile(L"/home/oleg/activex/1/oleg.svg");

    return 0;

#endif

#ifdef ONLINE_WORD_TO_PDF
    CPdfRenderer oPdfW(&oFonts);
    oPdfW.SetTempFolder(sDst);
    oPdfW.OnlineWordToPdf(L"D:\\test\\123.txt", L"D:\\test\\123.pdf");
    return 0;
#endif


    //std::wstring sFile = L"\\\\KIRILLOV8\\_Office\\PDF\\Android intro(2p).pdf";
    //std::wstring sFile = L"D:\\activex\\Pi(1p).pdf";
    //std::wstring sFile = L"\\\\192.168.3.208\\allusers\\Files\\PDF\\AllPDF\\asia.pdf";
    //std::wstring sFile = L"D:\\knut.djvu";
    //std::wstring sFile = L"D:\\bankomats.xps";
    //std::wstring sFile = L"\\\\kirillov8\\_Office\\DJVU\\Основы разработки приложений на платформе Microsoft .NET Framework. Учебный курс Microsoft экзамен 70-536.djvu";
    //std::wstring sFile = L"D:\\TESTFILES\\Алгоритмы - построение и анализ.djvu";
    std::wstring sFile = L"D:\\TESTFILES\\PDF 1-7 (756p).pdf";
    std::wstring sDst = L"D:\\test\\Document";

    //std::wstring sFile = L"/home/oleg/activex/Android intro(2p).pdf";
    //std::wstring sFile = L"/home/oleg/activex/Pi(1p).pdf";
    //std::wstring sFile = L"/home/oleg/activex/knut.djvu";
    //std::wstring sFile = L"/home/oleg/activex/bankomats.xps";
    //std::wstring sDst = L"/home/oleg/activex/1";

    IOfficeDrawingFile* pReader = NULL;
    pReader = new PdfReader::CPdfReader(&oFonts);
    //pReader = new CDjVuFile(&oFonts);
    //pReader = new CXpsFile(&oFonts);

    pReader->SetTempDirectory(sDst);
    pReader->LoadFromFile(sFile);

#ifdef TO_HTML_RENDERER
    NSHtmlRenderer::CASCHTMLRenderer3 oRenderer;
#ifdef ONLY_TEXT
    oRenderer.SetOnlyTextMode(true);
    oRenderer.CreateOfficeFile(L"temp/temp");
#else
    oRenderer.CreateOfficeFile(sDst);
#endif
#else
    CPdfRenderer oRenderer(&oFonts);
    oRenderer.SetTempFolder(sDst);
#endif

    int nPagesCount = pReader->GetPagesCount();
    for (int i = 0; i < nPagesCount; ++i)
    {
        oRenderer.NewPage();
        oRenderer.BeginCommand(c_nPageType);

        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        oRenderer.put_Width(dWidth);
        oRenderer.put_Height(dHeight);

#ifdef ONLY_TEXT
        oRenderer.SetAdditionalParam("DisablePageEnd", L"yes");
#endif
        pReader->DrawPageOnRenderer(&oRenderer, i, NULL);

#ifdef ONLY_TEXT
        oRenderer.SetAdditionalParam("DisablePageEnd", L"no");

        int paragraphs = 0;
        int words = 0;
        int symbols = 0;
        int spaces = 0;
        std::string info;
        oRenderer.GetLastPageInfo(paragraphs, words, symbols, spaces, info);
#endif

        oRenderer.EndCommand(c_nPageType);
    }

#ifdef TO_HTML_RENDERER
#ifndef ONLY_TEXT
    oRenderer.CloseFile();
#endif
#else
    oRenderer.SaveToFile(L"/home/oleg/activex/1/1.pdf");
#endif

    return 0;
}
