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
//#include <QCoreApplication>

#include "../../DesktopEditor/graphics/pro/Fonts.h"
#include "../../DesktopEditor/graphics/pro/Graphics.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../PdfWriter/PdfRenderer.h"
#include "../include/HTMLRenderer3.h"

#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../include/ASCSVGWriter.h"
#include "../../Common/FileDownloader/FileDownloader.h"

void Download_OnComplete(int error)
{
    int y = error;
    return;
}

//#define RASTER_TEST
//#define METAFILE_TEST
//#define METAFILE_TEST_RASTER
#define ONLINE_WORD_TO_PDF
//#define TO_PDF
//#define TO_HTML_RENDERER
//#define ONLY_TEXT
//#define DOWNLOADER_TEST

int main(int argc, char *argv[])
{
#ifdef DOWNLOADER_TEST
    CFileDownloader oDownloader(L"https://download.onlyoffice.com/assets/fb/fb_icon_325x325.jpg", false);
    oDownloader.SetFilePath(L"D:\\111.jpg");
    oDownloader.SetEvent_OnComplete(Download_OnComplete);
    oDownloader.DownloadSync();
#endif

#ifdef RASTER_TEST
    CBgraFrame oFrame;
    oFrame.OpenFile(L"D:\\22.png");
    oFrame.SaveFile(L"D:\\oleg.png", 4);
    return 0;
#endif

    NSFonts::IApplicationFonts* pFonts = NSFonts::NSApplication::Create();
    if (true)
    {
        pFonts->Initialize();
    }
    else
    {
        std::vector<std::wstring> arFiles = pFonts->GetSetupFontFiles();
        NSDirectory::GetFiles2(L"D:\\GIT\\core-fonts", arFiles, true);
        pFonts->InitializeFromArrayFiles(arFiles);
    }

#ifdef METAFILE_TEST

    NSFonts::IFontManager* pManager = pFonts->GenerateFontManager();

    NSHtmlRenderer::CASCSVGWriter oWriterSVG;
    oWriterSVG.SetFontManager(pManager);

    MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);

    //pMetafile->LoadFromFile(L"D:\\2\\ppt\\media\\image4.wmf");
    pMetafile->LoadFromFile(L"D:\\SVG\\Disigner 2.svg");

    double x = 0, y = 0, w = 0, h = 0;
    pMetafile->GetBounds(&x, &y, &w, &h);

    double _max = (w >= h) ? w : h;
    double dKoef = 100000.0 / _max;

    int WW = (int)(dKoef * w + 0.5);
    int HH = (int)(dKoef * h + 0.5);

    oWriterSVG.put_Width(WW);
    oWriterSVG.put_Height(HH);
    pMetafile->DrawOnRenderer(&oWriterSVG, 0, 0, WW, HH);

    oWriterSVG.SaveFile(L"D:\\SVG\\out.png");

    RELEASEOBJECT(pMetafile);
    RELEASEINTERFACE(pManager);
    RELEASEOBJECT(pFonts);
    return 0;

#endif

#ifdef METAFILE_TEST_RASTER

    NSFonts::IFontManager* pManager = pFonts->GenerateFontManager();

    NSGraphics::IGraphicsRenderer* pRasterRenderer = NSGraphics::Create();
    pRasterRenderer->SetFontManager(pManager);

    int nRasterW = 1000;
    int nRasterH = 1000;
    BYTE* pData = new BYTE[4 * nRasterW * nRasterH];
    //memset(pData, 255, 4 * nRasterW * nRasterH);

    unsigned int back = 0xffffff;
    unsigned int* pData32 = (unsigned int*)pData;
    unsigned int* pData32End = pData32 + nRasterW * nRasterH;
    //дефолтный тон должен быть прозрачным, а не белым
    while (pData32 < pData32End)
        *pData32++ = back;

    CBgraFrame oFrame;
    oFrame.put_Data(pData);
    oFrame.put_Width(nRasterW);
    oFrame.put_Height(nRasterH);
    oFrame.put_Stride(4 * nRasterW);

    pRasterRenderer->CreateFromBgraFrame(&oFrame);
    pRasterRenderer->SetSwapRGB(false);

    double dW_MM = nRasterW * 25.4 / 96;
    double dH_MM = nRasterH * 25.4 / 96;

    pRasterRenderer->put_Width(dW_MM);
    pRasterRenderer->put_Height(dH_MM);

    MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
    pMetafile->LoadFromFile(L"D:\\test\\123.svg");

    double x = 0, y = 0, w = 0, h = 0;
    pMetafile->GetBounds(&x, &y, &w, &h);

    pMetafile->DrawOnRenderer(pRasterRenderer, dW_MM / 4, dW_MM / 4, dW_MM / 2, dH_MM / 2);
    pMetafile->ConvertToRaster(L"D:\\SVG\\out2.png", 4, 1000);

    oFrame.SaveFile(L"D:\\SVG\\out.png", 4);

    RELEASEOBJECT(pMetafile);
    RELEASEINTERFACE(pManager);
    RELEASEINTERFACE(pRasterRenderer);
    return 0;

#endif

#ifdef ONLINE_WORD_TO_PDF
    CPdfRenderer oPdfW(pFonts);
    oPdfW.SetTempFolder(L"D:\\test\\Document");
    oPdfW.OnlineWordToPdf(L"D:\\test\\123.txt", L"D:\\test\\123.pdf");
    RELEASEOBJECT(pFonts);
    return 0;
#endif


    //std::wstring sFile = L"\\\\KIRILLOV8\\_Office\\PDF\\Android intro(2p).pdf";
    //std::wstring sFile = L"D:\\activex\\Pi(1p).pdf";
    //std::wstring sFile = L"\\\\192.168.3.208\\allusers\\Files\\PDF\\AllPDF\\asia.pdf";
    //std::wstring sFile = L"D:\\knut.djvu";
    //std::wstring sFile = L"D:\\bankomats.xps";
    //std::wstring sFile = L"\\\\kirillov8\\_Office\\DJVU\\Основы разработки приложений на платформе Microsoft .NET Framework. Учебный курс Microsoft экзамен 70-536.djvu";
    //std::wstring sFile = L"D:\\TESTFILES\\Алгоритмы - построение и анализ.djvu";
#ifndef WIN32
    std::wstring sFile = L"/home/oleg/GIT/ddd/ZfAvCwDsowJALpClgmE_/source/ZfAvCwDsowJALpClgmE_.pdf";
#else
    //std::wstring sFile = L"D:\\ddd\\ZfAvCwDsowJALpClgmE_\\source\\ZfAvCwDsowJALpClgmE_.pdf";
    //std::wstring sFile = L"D:\\2.pdf";
    std::wstring sFile = L"D:\\5.xps";
#endif

#ifdef WIN32
    std::wstring sDst = L"D:\\test\\Document";
#else
    std::wstring sDst = L"/home/oleg/test/Document";
#endif

    //std::wstring sFile = L"/home/oleg/activex/Android intro(2p).pdf";
    //std::wstring sFile = L"/home/oleg/activex/Pi(1p).pdf";
    //std::wstring sFile = L"/home/oleg/activex/knut.djvu";
    //std::wstring sFile = L"/home/oleg/activex/bankomats.xps";
    //std::wstring sDst = L"/home/oleg/activex/1";

    IOfficeDrawingFile* pReader = NULL;
    //pReader = new PdfReader::CPdfReader(pFonts);
    //pReader = new CDjVuFile(pFonts);
    pReader = new CXpsFile(pFonts);

    pReader->SetTempDirectory(sDst);
    pReader->LoadFromFile(sFile);

    //pReader->ConvertToRaster(0, L"D:\\111.png", 4);

#ifdef TO_HTML_RENDERER
    NSHtmlRenderer::CASCHTMLRenderer3 oRenderer;
#ifdef ONLY_TEXT
    oRenderer.SetOnlyTextMode(true);
    oRenderer.CreateOfficeFile(L"temp/temp");
#else
    oRenderer.CreateOfficeFile(sDst);
#endif
#else
    CPdfRenderer oRenderer(pFonts);
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
    oRenderer.SaveToFile(L"D:/11.pdf");
#endif

    RELEASEOBJECT(pFonts);
    return 0;
}
