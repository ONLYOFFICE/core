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
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../PdfWriter/PdfRenderer.h"
#include "../include/HTMLRenderer3.h"

#include "../../DesktopEditor/raster/BgraFrame.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../include/ASCSVGWriter.h"

#include "../../Common/Network/FileTransporter/include/FileTransporter.h"


void Download_OnComplete(int error)
{
    int y = error;
    return;
}

//#define RASTER_TEST
//#define METAFILE_TEST
//#define METAFILE_TEST_X2T
//#define METAFILE_TEST_RASTER
//#define ONLINE_WORD_TO_PDF
//#define TO_PDF
//#define TO_HTML_RENDERER
//#define ONLY_TEXT
#define DOWNLOADER_TEST

#include "Windows.h"
#include <wininet.h>
#undef CreateDirectory

bool DownloadFilePS(const std::wstring& sFileURL, const std::wstring& strFileOutput)
{
	STARTUPINFO sturtupinfo;
	ZeroMemory(&sturtupinfo,sizeof(STARTUPINFO));
	sturtupinfo.cb = sizeof(STARTUPINFO);

	std::wstring sFileDst = strFileOutput;
	size_t posn = 0;
	while (std::wstring::npos != (posn = sFileDst.find('\\', posn)))
	{
		sFileDst.replace(posn, 1, L"/");
		posn += 1;
	}

	std::wstring sApp = L"powershell.exe –c \"(new-object System.Net.WebClient).DownloadFile('" + sFileURL + L"','" + sFileDst + L"')\"";

	wchar_t* pCommandLine = new wchar_t[sApp.length() + 1];
	memcpy(pCommandLine, sApp.c_str(), sApp.length() * sizeof(wchar_t));
	pCommandLine[sApp.length()] = (wchar_t)'\0';

	HANDLE ghJob = CreateJobObject(NULL, NULL);

	if (ghJob)
	{
		JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli = { 0 };

		// Configure all child processes associated with the job to terminate when the
		jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
		if ( 0 == SetInformationJobObject( ghJob, JobObjectExtendedLimitInformation, &jeli, sizeof(jeli)))
		{
			CloseHandle(ghJob);
			ghJob = NULL;
		}
	}

	PROCESS_INFORMATION processinfo;
	ZeroMemory(&processinfo,sizeof(PROCESS_INFORMATION));
	BOOL bResult = CreateProcessW(NULL, pCommandLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &sturtupinfo, &processinfo);

	if (bResult && ghJob)
	{
		AssignProcessToJobObject(ghJob, processinfo.hProcess);
	}

	::WaitForSingleObject(processinfo.hProcess, INFINITE);

	RELEASEARRAYOBJECTS(pCommandLine);

	return NSFile::CFileBinary::Exists(sFileDst);
}

int main(int argc, char *argv[])
{
	DownloadFilePS(L"https://natworld.info/wp-content/uploads/2018/02/vodosvinka-ili-kapibara.jpg", L"D:/222.jpg");

#ifdef DOWNLOADER_TEST
	NSNetwork::NSFileTransport::CFileDownloader oDownloader(L"https://natworld.info/wp-content/uploads/2018/02/vodosvinka-ili-kapibara.jpg", false);
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

    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    //oWorker.m_arAdditionalFolders.push_back(L"D:\\GIT\\core-fonts");
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pFonts = oWorker.Check();

#ifdef METAFILE_TEST_X2T

    MetaFile::IMetaFile* pMetafile = MetaFile::Create(pFonts);
    if (pMetafile->LoadFromFile(L"image.emf"))
    {
        NSFonts::IFontManager* pFontManager = pFonts->GenerateFontManager();

        double x = 0, y = 0, w = 0, h = 0;
        pMetafile->GetBounds(&x, &y, &w, &h);

        double _max = (w >= h) ? w : h;
        double dKoef = 1000.0 / _max;

        int WW = (int)(dKoef * w + 0.5);
        int HH = (int)(dKoef * h + 0.5);

        NSHtmlRenderer::CASCSVGWriter oWriterSVG;
        oWriterSVG.SetFontManager(pFontManager);
        oWriterSVG.put_Width(WW);
        oWriterSVG.put_Height(HH);

        bool bRes = true;
        bool bIsBigestSVG = false;
        bool bIsRaster = true;
        try
        {
            bRes = pMetafile->DrawOnRenderer(&oWriterSVG, 0, 0, WW, HH);
        }
        catch (...)
        {
            bRes = false;
        }

        if (bRes)
        {
            oWriterSVG.IsRaster(&bIsRaster);

            LONG lSvgDataSize = 0;
            oWriterSVG.GetSVGDataSize(&lSvgDataSize);

            bIsBigestSVG = (lSvgDataSize > 5 * 1024 * 1024);
        }

        if (bIsRaster || bIsBigestSVG || !bRes)
        {
            int nWidth = 0;
            int nHeight = 0;

            int nMaxPixSize = 1000;

            double dKoef = nMaxPixSize / _max;

            nWidth = (int)(dKoef * w + 0.5);
            nHeight = (int)(dKoef * h + 0.5);

            pMetafile->ConvertToRaster(L"image.png", 4 /*CXIMAGE_FORMAT_PNG*/,  nWidth, nHeight);
        }
        else
        {
            oWriterSVG.SaveFile(L"image.svg");
        }

        RELEASEINTERFACE(pFontManager);
    }
    RELEASEOBJECT(pMetafile);

#endif

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
    std::wstring sFile = L"D:\\OoPdfFormExample2.pdf";
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

    //NSFonts::NSApplicationFontStream::SetGlobalMemoryStorage(NSFonts::NSApplicationFontStream::CreateDefaultGlobalMemoryStorage());

    IOfficeDrawingFile* pReader = NULL;
    pReader = new PdfReader::CPdfReader(pFonts);
    //pReader = new CDjVuFile(pFonts);
    //pReader = new CXpsFile(pFonts);

    pReader->SetTempDirectory(sDst);
    pReader->LoadFromFile(sFile);

    pReader->ConvertToRaster(0, L"D:\\111.png", 4);

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
