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

#define errMemory 12

#include "../DesktopEditor/graphics/pro/Graphics.h"
#include "PdfReader.h"

#include "Src/Adaptors.h"
#include "lib/xpdf/ErrorCodes.h"

#include "../Common/OfficeDefines.h"
#include "../DesktopEditor/raster/BgraFrame.h"
#include "../DesktopEditor/graphics/IRenderer.h"
#include "../DesktopEditor/common/Directory.h"

//#include "Src/StringExt.h"
#include "lib/xpdf/PDFDoc.h"
#include "lib/xpdf/GlobalParams.h"
#include "lib/xpdf/ErrorCodes.h"
#include "lib/xpdf/ImageOutputDev.h"
#include "Src/RendererOutputDev.h"
//#include "Src/PageLabels.h"

namespace PdfReader
{
    class CPdfReader_Private
    {
    public:
        PDFDoc*            m_pPDFDocument;
        std::wstring       m_wsTempFolder;
        std::wstring       m_wsCMapFolder;
        NSFonts::IApplicationFonts* m_pAppFonts;
        NSFonts::IFontManager*      m_pFontManager;
        CFontList*         m_pFontList;
    };

    CPdfReader::CPdfReader(NSFonts::IApplicationFonts* pAppFonts)
	{
        m_pInternal = new CPdfReader_Private();

        m_pInternal->m_wsTempFolder = L"";
        m_pInternal->m_wsCMapFolder = L"";

        m_pInternal->m_pPDFDocument = NULL;
        m_pInternal->m_pFontManager = NULL;

        globalParams  = new GlobalParamsAdaptor(NULL);
        m_pInternal->m_pFontList = new CFontList();

        m_pInternal->m_pAppFonts = pAppFonts;

		// Создаем менеджер шрифтов с собственным кэшем
        m_pInternal->m_pFontManager = pAppFonts->GenerateFontManager();
        NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
		pMeasurerCache->SetStreams(pAppFonts->GetStreams());
        m_pInternal->m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(1);
        ((GlobalParamsAdaptor*)globalParams)->SetFontManager(m_pInternal->m_pFontManager);
        globalParams->setupBaseFonts(NULL);

        m_eError = errNone;
	}
	CPdfReader::~CPdfReader()
	{
        if (m_pInternal->m_pFontList)
		{
            m_pInternal->m_pFontList->Clear();
            delete m_pInternal->m_pFontList;
		}

        m_pInternal->m_wsCMapFolder = L"";

        if (!m_pInternal->m_wsTempFolder.empty())
		{
            NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);
            m_pInternal->m_wsTempFolder = L"";
		}

        RELEASEOBJECT((m_pInternal->m_pPDFDocument));
        RELEASEOBJECT((globalParams));
        RELEASEINTERFACE((m_pInternal->m_pFontManager));
	}
    bool CPdfReader::LoadFromFile(const std::wstring& wsSrcPath, const std::wstring& wsOptions,
                                    const std::wstring& wsOwnerPassword, const std::wstring& wsUserPassword)
	{
// TODO: Сейчас при загрузке каждой новой картинки мы пересоздаем 
//       FontManager, потому что сейчас в нем кэш без ограничения.
//------------------------------------------------------
        RELEASEINTERFACE((m_pInternal->m_pFontManager));
        m_pInternal->m_pFontManager = m_pInternal->m_pAppFonts->GenerateFontManager();
        NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
        pMeasurerCache->SetStreams(m_pInternal->m_pAppFonts->GetStreams());
        m_pInternal->m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(1);
        ((GlobalParamsAdaptor*)globalParams)->SetFontManager(m_pInternal->m_pFontManager);
//------------------------------------------------------

        if (m_pInternal->m_pPDFDocument)
            delete m_pInternal->m_pPDFDocument;

        if (GetTempDirectory() == L"")
        {
            SetTempDirectory(NSDirectory::GetTempPath());
        }

        m_eError = errNone;
        GString* owner_pswd = NSStrings::CreateString(wsOwnerPassword);
        GString* user_pswd = NSStrings::CreateString(wsUserPassword);

        // конвертим путь в utf8 - под виндой они сконвертят в юникод, а на остальных - так и надо
        std::string sPathUtf8 = U_TO_UTF8(wsSrcPath);
        m_pInternal->m_pPDFDocument = new PDFDoc((char*)sPathUtf8.c_str(), owner_pswd, user_pswd);

        delete owner_pswd;
        delete user_pswd;

        if (m_pInternal->m_pPDFDocument)
            m_eError = m_pInternal->m_pPDFDocument->getErrorCode();
        else
            m_eError = errMemory;

        if (!m_pInternal->m_pPDFDocument || !m_pInternal->m_pPDFDocument->isOk())
		{
            RELEASEOBJECT(m_pInternal->m_pPDFDocument);
			return false;
		}

        m_pInternal->m_pFontList->Clear();

        return (errNone == m_eError);
	}
    void CPdfReader::Close()
	{
        RELEASEOBJECT((m_pInternal->m_pPDFDocument));
	}
    OfficeDrawingFileType CPdfReader::GetType()
    {
        return odftPDF;
    }
    int CPdfReader::GetError()
	{
        if (!m_pInternal->m_pPDFDocument)
            return m_eError;

        return m_pInternal->m_pPDFDocument->getErrorCode();
	}
    int CPdfReader::GetPagesCount()
	{
        if (!m_pInternal->m_pPDFDocument)
			return 0;

        return m_pInternal->m_pPDFDocument->getNumPages();
	}
    double CPdfReader::GetVersion()
	{
        if (!m_pInternal->m_pPDFDocument)
			return 0;

        return m_pInternal->m_pPDFDocument->getPDFVersion();
	}
    int CPdfReader::GetPermissions()
	{
        if (!m_pInternal->m_pPDFDocument)
			return 0;

		int nPermissions = 0;

        if (m_pInternal->m_pPDFDocument->okToPrint())
			nPermissions += PERMISSION_PRINT;
        if (m_pInternal->m_pPDFDocument->okToCopy())
			nPermissions += PERMISSION_COPY;
        if (m_pInternal->m_pPDFDocument->okToChange())
			nPermissions += PERMISSION_CHANGE;

		return nPermissions;
	}
	std::wstring CPdfReader::GetPageLabel(int nPageIndex)
	{
		if (!m_pInternal->m_pPDFDocument)
			return std::wstring();
// todo label
//		StringExt* seLabel = m_pInternal->m_pPDFDocument->GetPageLabels()->GetLabel(nPageIndex);
//		if (seLabel)
//		{
//			std::wstring wsResult(seLabel->GetWString());
//			delete seLabel;
//			return wsResult;
//		}

		return std::wstring();
	}
    bool CPdfReader::ExtractAllImages(const wchar_t* wsDstPath, const wchar_t* wsPrefix)
	{
        std::wstring sDstPath(wsDstPath);
        if (sDstPath.empty())
            return false;

        // check last symbol (directory)
        wchar_t nLastSymbol = sDstPath[sDstPath.length() - 1];
        if ('\\' != nLastSymbol && '/' != nLastSymbol)
            sDstPath += '/';
        // prefix for each file
        if (NULL != wsPrefix)
            sDstPath += std::wstring(wsPrefix);

        std::string sDstPathA = U_TO_UTF8(sDstPath);
        ImageOutputDev *pOutputDev = new ImageOutputDev((char*)sDstPathA.c_str(), true, false, false);
		if (!pOutputDev)
			return false;

        int nPagesCount = GetPagesCount();
        for (int nIndex = 1; nIndex <= nPagesCount; nIndex++)
		{
            m_pInternal->m_pPDFDocument->displayPage(pOutputDev, nIndex, 72, 72, 0, false, false, false);
		}

		delete pOutputDev;

        return true;
	}
    void CPdfReader::GetPageInfo(int _nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
	{
		int nPageIndex = _nPageIndex + 1;

        if (!m_pInternal->m_pPDFDocument)
			return;

		const double c_dInch = 25.399; // Миллиметров в дюйме
		const double c_dXResolution = 154.0;
		const double c_dYResolution = 154.0;

		double dKoefX = c_dInch / c_dXResolution;
		double dKoefY = c_dInch / c_dYResolution;

        int nRotate = m_pInternal->m_pPDFDocument->getPageRotate(nPageIndex);

		while (nRotate >= 360)
			nRotate -= 360;

		while (nRotate < 0)
			nRotate += 360;

		if (0 != nRotate && 180 != nRotate)
		{
            *pdHeight = m_pInternal->m_pPDFDocument->getPageCropWidth(nPageIndex);
            *pdWidth  = m_pInternal->m_pPDFDocument->getPageCropHeight(nPageIndex);
		}
		else
		{
            *pdWidth  = m_pInternal->m_pPDFDocument->getPageCropWidth(nPageIndex);
            *pdHeight = m_pInternal->m_pPDFDocument->getPageCropHeight(nPageIndex);
		}

		*pdDpiX   = 72;
		*pdDpiY   = 72;
	}
    void CPdfReader::DrawPageOnRenderer(IRenderer* pRenderer, int _nPageIndex, bool* pbBreak)
	{
		int nPageIndex = _nPageIndex + 1;

        if (m_pInternal->m_pPDFDocument && pRenderer)
		{
            RendererOutputDev oRendererOut(pRenderer, m_pInternal->m_pFontManager, m_pInternal->m_pFontList);
            oRendererOut.NewPDF(m_pInternal->m_pPDFDocument->getXRef());
			oRendererOut.SetBreak(pbBreak);
            m_pInternal->m_pPDFDocument->displayPage(&oRendererOut, nPageIndex, 72.0, 72.0, 0, false, true, false);
		}
	}
    void CPdfReader::ConvertToRaster(int nPageIndex, const std::wstring& wsDstPath, int nImageType, const int nRasterW, const int nRasterH)
	{
        NSFonts::IFontManager *pFontManager = m_pInternal->m_pAppFonts->GenerateFontManager();
        NSFonts::IFontsCache* pFontCache = NSFonts::NSFontCache::Create();
		pFontCache->SetStreams(m_pInternal->m_pAppFonts->GetStreams());
		pFontManager->SetOwnerCache(pFontCache);

        NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
        pRenderer->SetFontManager(pFontManager);

		double dWidth, dHeight;
        double dDpiX, dDpiY;
        GetPageInfo(nPageIndex, &dWidth, &dHeight, &dDpiX, &dDpiY);

        int nWidth  = (nRasterW > 0) ? nRasterW : ((int)dWidth  * 72 / 25.4);
        int nHeight = (nRasterH > 0) ? nRasterH : ((int)dHeight * 72 / 25.4);

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

        dWidth  *= 25.4 / dDpiX;
        dHeight *= 25.4 / dDpiY;

        pRenderer->put_Width(dWidth);
        pRenderer->put_Height(dHeight);

		bool bBreak = false;
        DrawPageOnRenderer(pRenderer, nPageIndex, &bBreak);

		oFrame.SaveFile(wsDstPath, nImageType);
		RELEASEINTERFACE(pFontManager);
        RELEASEOBJECT(pRenderer);
	}
    int CPdfReader::GetImagesCount()
	{
//        ImageOutputDev *pOutputDev = new ImageOutputDev(NULL, true, true, false);
//		if (!pOutputDev)
//			return 0;
//
//        for (int nIndex = 1; nIndex <= m_pInternal->m_pPDFDocument->GetPagesCount(); nIndex++)
//		{
//            m_pInternal->m_pPDFDocument->displayPage(pOutputDev, nIndex, 72, 72, 0, false, false, false);
//		}
//
//		return pOutputDev->
return 0;
	}	
    void CPdfReader::SetTempDirectory(const std::wstring& wsTempFolder)
	{		
        if (!m_pInternal->m_wsTempFolder.empty())
		{
            NSDirectory::DeleteDirectory(m_pInternal->m_wsTempFolder);
            m_pInternal->m_wsTempFolder = wsTempFolder;
		}

        if (!wsTempFolder.empty())
		{
			std::wstring wsFolderName = std::wstring(wsTempFolder) + L"//pdftemp";
			std::wstring wsFolder = wsFolderName;
			int nCounter = 0;
			while (NSDirectory::Exists(wsFolder))
			{
				nCounter++;
				wsFolder = wsFolderName + L"_" + std::to_wstring(nCounter);
			}
			NSDirectory::CreateDirectory(wsFolder);
            m_pInternal->m_wsTempFolder = wsFolder;
		}
		else
            m_pInternal->m_wsTempFolder = L"";

        if (globalParams)
            ((GlobalParamsAdaptor*)globalParams)->SetTempFolder(m_pInternal->m_wsTempFolder.c_str());
	}
    std::wstring CPdfReader::GetTempDirectory()
    {
        return m_pInternal->m_wsTempFolder;
    }

    void CPdfReader::SetCMapFolder(const wchar_t* wsCMapFolder)
	{
        m_pInternal->m_wsCMapFolder = std::wstring(wsCMapFolder);

        if (globalParams)
            ((GlobalParamsAdaptor*)globalParams)->SetCMapFolder(m_pInternal->m_wsCMapFolder.c_str());
	}
    NSFonts::IFontManager* CPdfReader::GetFontManager()
	{
        return m_pInternal->m_pFontManager;
	}
	std::wstring CPdfReader::ToXml(const std::wstring& wsFilePath)
	{
        //todo
//		std::wstring wsXml = m_pInternal->m_pPDFDocument->ToXml();
//
//		if (wsFilePath != L"")
//		{
//			NSFile::CFileBinary oFile;
//			if (!oFile.CreateFileW(wsFilePath))
//				return wsXml;
//
//			oFile.WriteStringUTF8(wsXml);
//			oFile.CloseFile();
//		}
//
//		return wsXml;
        return L"";
	}
}
