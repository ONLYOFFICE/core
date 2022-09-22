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
#include "../DesktopEditor/common/StringExt.h"
#include "../DesktopEditor/common/Path.h"
#include "../PdfWriter/PdfRenderer.h"

#include "lib/xpdf/PDFDoc.h"
#include "lib/xpdf/GlobalParams.h"
#include "lib/xpdf/ErrorCodes.h"
#include "lib/xpdf/ImageOutputDev.h"
#include "lib/xpdf/TextString.h"
#include "lib/xpdf/SecurityHandler.h"
#include "lib/xpdf/Lexer.h"
#include "lib/xpdf/Parser.h"
#include "lib/xpdf/AcroForm.h"
#include "Src/RendererOutputDev.h"
#include "Src/Adaptors.h"

#ifdef BUILDING_WASM_MODULE
#include "../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"
#include "lib/xpdf/Outline.h"
#include "lib/xpdf/Link.h"
#include "lib/xpdf/TextOutputDev.h"
#include "lib/goo/GList.h"
#include <vector>
#endif

namespace PdfReader
{
    class CPdfReader_Private
    {
    public:
        PDFDoc*            m_pPDFDocument;
        std::wstring       m_wsTempFolder;
        std::wstring       m_wsCMapFolder;
        std::wstring       m_wsSrcPath;
        NSFonts::IApplicationFonts* m_pAppFonts;
        NSFonts::IFontManager*      m_pFontManager;
        CFontList*         m_pFontList;
        CPdfRenderer*      m_pPdfWriter;
        DWORD              m_nFileLength;

        void GetPageTree(XRef* xref, Object* pPagesRefObj)
        {
            if (!pPagesRefObj || !xref)
                return;

            Object typeDict, pagesObj;
            if (!pPagesRefObj->isRef() || !pPagesRefObj->fetch(xref, &pagesObj)   ||
                !pagesObj.isDict()     || !pagesObj.dictLookup("Type", &typeDict) ||
                !typeDict.isName("Pages"))
            {
                pagesObj.free();
                typeDict.free();
                return;
            }
            typeDict.free();

            Ref topPagesRef = pPagesRefObj->getRef();
            std::wstring sPageTree = XMLConverter::DictToXml(L"PageTree", &pagesObj, topPagesRef.num, topPagesRef.gen);

            m_pPdfWriter->CreatePageTree(sPageTree, topPagesRef.num);

            Object kidsArrObj;
            if (!pagesObj.dictLookup("Kids", &kidsArrObj) || !kidsArrObj.isArray())
            {
                pagesObj.free();
                kidsArrObj.free();
                return;
            }
            pagesObj.free();

            for (int i = 0, count = kidsArrObj.arrayGetLength(); i < count; ++i)
            {
                Object kidRefObj;
                if (kidsArrObj.arrayGetNF(i, &kidRefObj))
                    GetPageTree(xref, &kidRefObj);
                kidRefObj.free();
            }
            kidsArrObj.free();
        }
    };

    CPdfReader::CPdfReader(NSFonts::IApplicationFonts* pAppFonts)
	{
        m_pInternal = new CPdfReader_Private();

        m_pInternal->m_wsTempFolder = L"";
        m_pInternal->m_wsCMapFolder = L"";
        m_pInternal->m_wsSrcPath    = L"";

        m_pInternal->m_pPDFDocument = NULL;
        m_pInternal->m_pFontManager = NULL;
        m_pInternal->m_pPdfWriter   = NULL;
        m_pInternal->m_nFileLength  = 0;

        globalParams  = new GlobalParamsAdaptor(NULL);
#ifndef _DEBUG
        globalParams->setErrQuiet(gTrue);
#endif

        m_pInternal->m_pFontList = new CFontList();

        m_pInternal->m_pAppFonts = pAppFonts;

		// Создаем менеджер шрифтов с собственным кэшем
        m_pInternal->m_pFontManager = pAppFonts->GenerateFontManager();
        NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
		pMeasurerCache->SetStreams(pAppFonts->GetStreams());
        m_pInternal->m_pFontManager->SetOwnerCache(pMeasurerCache);
        pMeasurerCache->SetCacheSize(1);
        ((GlobalParamsAdaptor*)globalParams)->SetFontManager(m_pInternal->m_pFontManager);
    #ifndef BUILDING_WASM_MODULE
        globalParams->setupBaseFonts(NULL);
    #endif

#ifdef CMAP_USE_MEMORY
		SetCMapMemory();
#endif

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

        if (m_pInternal->m_pPdfWriter)
        {
            m_pInternal->m_pPdfWriter->Release();
            m_pInternal->m_pPdfWriter = NULL;
        }
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
        m_pInternal->m_wsSrcPath = wsSrcPath;
        std::string sPathUtf8 = U_TO_UTF8(wsSrcPath);
        m_pInternal->m_pPDFDocument = new PDFDoc((char*)sPathUtf8.c_str(), owner_pswd, user_pswd);

        delete owner_pswd;
        delete user_pswd;

        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(wsSrcPath))
        {
            m_pInternal->m_nFileLength = oFile.GetFileSize();
            oFile.CloseFile();
        }

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

        return true;
	}
    bool CPdfReader::LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options,
                                    const std::wstring& owner_password, const std::wstring& user_password)
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

        RELEASEOBJECT(m_pInternal->m_pPDFDocument);
        m_eError = errNone;
        GString* owner_pswd = NSStrings::CreateString(owner_password);
        GString* user_pswd  = NSStrings::CreateString(user_password);

        Object obj;
        obj.initNull();
        // будет освобожден в деструкторе PDFDoc
        BaseStream *str = new MemStream((char*)data, 0, length, &obj);
        m_pInternal->m_pPDFDocument = new PDFDoc(str, owner_pswd, user_pswd);
        m_pInternal->m_nFileLength = length;

        delete owner_pswd;
        delete user_pswd;

        m_eError = m_pInternal->m_pPDFDocument ? m_pInternal->m_pPDFDocument->getErrorCode() : errMemory;

        if (!m_pInternal->m_pPDFDocument || !m_pInternal->m_pPDFDocument->isOk())
        {
            RELEASEOBJECT(m_pInternal->m_pPDFDocument);
            return false;
        }

        m_pInternal->m_pFontList->Clear();

        return true;
    }
    void CPdfReader::Close()
	{
        RELEASEOBJECT((m_pInternal->m_pPDFDocument));
	}
    NSFonts::IApplicationFonts* CPdfReader::GetFonts()
    {
        return m_pInternal->m_pAppFonts;
    }

    OfficeDrawingFileType CPdfReader::GetType()
    {
        return odftPDF;
    }
    int CPdfReader::GetError()
	{
        if (!m_pInternal->m_pPDFDocument)
            return m_eError;

        if (m_pInternal->m_pPDFDocument->isOk())
            return 0;

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
	void CPdfReader::SetCMapMemory()
	{
		if (globalParams)
			((GlobalParamsAdaptor*)globalParams)->SetCMapMemory();
	}
    NSFonts::IFontManager* CPdfReader::GetFontManager()
	{
        return m_pInternal->m_pFontManager;
	}
	std::wstring CPdfReader::ToXml(const std::wstring& wsFilePath, bool isPrintStream)
	{
		XMLConverter oConverter(m_pInternal->m_pPDFDocument->getXRef(), isPrintStream);
		std::wstring wsXml = oConverter.GetXml();

		if (wsFilePath != L"")
		{
			NSFile::CFileBinary oFile;
			if (!oFile.CreateFileW(wsFilePath))
				return wsXml;

			oFile.WriteStringUTF8(wsXml);
			oFile.CloseFile();
		}

		return wsXml;
	}
    bool CPdfReader::EditPdf(IRenderer* pPdfWriter, const std::wstring& wsPath, const std::wstring& sPassword)
    {
        if (!pPdfWriter)
            return false;

        long lRendererType;
        pPdfWriter->get_Type(&lRendererType);
        if (c_nPDFWriter != lRendererType || !m_pInternal->m_pPDFDocument)
            return false;

        std::string sPathUtf8New = U_TO_UTF8(wsPath);
        std::string sPathUtf8Old = U_TO_UTF8(m_pInternal->m_wsSrcPath);
        if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
        {
            GString* owner_pswd = NSStrings::CreateString(sPassword);
            GString* user_pswd  = NSStrings::CreateString(sPassword);
            bool bRes = m_pInternal->m_pPDFDocument->makeWritable(true, owner_pswd, user_pswd);
            delete owner_pswd;
            delete user_pswd;
            if (!bRes)
                return false;
        }
        else
        {
            NSFile::CFileBinary oFile;
            if (!oFile.OpenFile(wsPath, true))
                return false;
            oFile.CloseFile();
        }

        m_pInternal->m_pPdfWriter = (CPdfRenderer*)pPdfWriter;
        m_pInternal->m_pPdfWriter->AddRef();
        XRef* xref = m_pInternal->m_pPDFDocument->getXRef();
        if (!xref)
            return false;

        Object catDict, catRefObj, pagesRefObj;
        if (!xref->getCatalog(&catDict) || !catDict.isDict() || !catDict.dictLookupNF("Pages", &pagesRefObj))
        {
            pagesRefObj.free();
            catDict.free();
            return false;
        }
        Object* trailer = xref->getTrailerDict();
        if (!trailer || !trailer->isDict() || !trailer->dictLookupNF("Root", &catRefObj) || !catRefObj.isRef())
        {
            pagesRefObj.free();
            catDict.free();
            catRefObj.free();
            return false;
        }

        Ref catRef = catRefObj.getRef();
        std::wstring sCatalog = XMLConverter::DictToXml(L"Catalog", &catDict, catRef.num, catRef.gen);

        unsigned int nFormField = 0;
        AcroForm* form = m_pInternal->m_pPDFDocument->getCatalog()->getForm();
        if (form)
        {
            nFormField = form->getNumFields() + 1;
            std::wstring sSig = L"Sig" + std::to_wstring(nFormField);
            int i = 0, nFormFields = form->getNumFields();
            while (i < nFormFields)
            {
                int nLength;
                Unicode* uName = form->getField(i)->getName(&nLength);
                std::wstring sName = NSStringExt::CConverter::GetUnicodeFromUTF32(uName, nLength);
                RELEASEMEM(uName);
                if (sName == sSig)
                {
                    i = 0;
                    nFormField++;
                    sSig = L"Sig" + std::to_wstring(nFormField);
                }
                else
                    i++;
            }
            nFormField--;
        }

        int nCryptAlgorithm = -1;
        std::wstring sEncrypt;
        if (xref->isEncrypted())
        {
            CryptAlgorithm encAlgorithm;
            GBool ownerPasswordOk;
            int permFlags, keyLength, encVersion;
            xref->getEncryption(&permFlags, &ownerPasswordOk, &keyLength, &encVersion, &encAlgorithm);
            nCryptAlgorithm = encAlgorithm;

            Object* pTrailerDict = xref->getTrailerDict();
            if (pTrailerDict)
            {
                Object encrypt, ID, ID1;
                if (pTrailerDict->dictLookup("Encrypt", &encrypt) && encrypt.isDict())
                    sEncrypt = XMLConverter::DictToXml(L"Encrypt", &encrypt, 0, 0, true);
                encrypt.free();
                if (sEncrypt.length() > 10)
                    sEncrypt.erase(sEncrypt.length() - 10);

                if (pTrailerDict->dictLookup("ID", &ID) && ID.isArray() && ID.arrayGet(0, &ID1) && ID1.isString())
                    sEncrypt += XMLConverter::DictToXml(L"ID", &ID1, 0, 0, true);
                if (sEncrypt.length() > 10)
                    sEncrypt += L"</Encrypt>";
                ID.free();
                ID1.free();
            }
        }

        bool bRes = m_pInternal->m_pPdfWriter->EditPdf(wsPath, xref->getLastXRefPos(), xref->getNumObjects(), sCatalog, catRef.num, sEncrypt, sPassword, nCryptAlgorithm, nFormField);
        if (bRes)
            m_pInternal->GetPageTree(xref, &pagesRefObj);
        pagesRefObj.free();
        catDict.free();
        catRefObj.free();
        return bRes;
    }
    bool CPdfReader::EditPage(int nPageIndex)
    {
        if (!m_pInternal->m_pPdfWriter || !m_pInternal->m_pPDFDocument)
            return false;
        XRef* xref = m_pInternal->m_pPDFDocument->getXRef();
        Catalog* pCatalog = m_pInternal->m_pPDFDocument->getCatalog();
        if (!xref || !pCatalog)
            return false;
        std::pair<int, int> pPageRef = m_pInternal->m_pPdfWriter->GetPageRef(nPageIndex);
        if (pPageRef.first == 0)
            return false;

        // Получение объекта страницы
        Object pageRefObj, pageObj;
        pageRefObj.initRef(pPageRef.first, pPageRef.second);
        if (!pageRefObj.fetch(xref, &pageObj) || !pageObj.isDict())
        {
            pageObj.free();
            pageRefObj.free();
            return false;
        }
        std::wstring sPage = XMLConverter::DictToXml(L"Page", &pageObj, pPageRef.first, pPageRef.second);
        pageObj.free();
        pageRefObj.free();

        return m_pInternal->m_pPdfWriter->EditPage(sPage, pPageRef.first);
    }
    bool CPdfReader::DeletePage(int nPageIndex)
    {
        if (!m_pInternal->m_pPdfWriter || !m_pInternal->m_pPDFDocument)
            return false;

        return m_pInternal->m_pPdfWriter->DeletePage(nPageIndex);
    }
    bool CPdfReader::AddPage(int nPageIndex)
    {
        if (!m_pInternal->m_pPdfWriter || !m_pInternal->m_pPDFDocument)
            return false;

        return m_pInternal->m_pPdfWriter->AddPage(nPageIndex);
    }
    bool CPdfReader::EditClose(const std::wstring& sPassword)
    {
        if (!m_pInternal->m_pPdfWriter || !m_pInternal->m_pPDFDocument)
            return false;
        XRef* xref = m_pInternal->m_pPDFDocument->getXRef();
        if (!xref)
            return false;

        Object* trailerDict = xref->getTrailerDict();
        std::wstring sTrailer;
        if (trailerDict)
            sTrailer = XMLConverter::DictToXml(L"Trailer", trailerDict);

        std::wstring sInfo;
        Object info;
        m_pInternal->m_pPDFDocument->getDocInfo(&info);
        if (info.isDict())
            sInfo = XMLConverter::DictToXml(L"Info", &info);
        info.free();

        std::wstring wsPath = m_pInternal->m_pPdfWriter->GetEditPdfPath();
        bool bRes = m_pInternal->m_pPdfWriter->EditClose(sTrailer, sInfo);

        std::string sPathUtf8New = U_TO_UTF8(wsPath);
        std::string sPathUtf8Old = U_TO_UTF8(m_pInternal->m_wsSrcPath);
        if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
        {
            GString* owner_pswd = NSStrings::CreateString(sPassword);
            GString* user_pswd  = NSStrings::CreateString(sPassword);
            bRes &= m_pInternal->m_pPDFDocument->makeWritable(false, owner_pswd, user_pswd);
            delete owner_pswd;
            delete user_pswd;

            NSFile::CFileBinary oFile;
            if (oFile.OpenFile(m_pInternal->m_wsSrcPath))
            {
                m_pInternal->m_nFileLength = oFile.GetFileSize();
                oFile.CloseFile();
            }
        }

        return bRes;
    }

#define DICT_LOOKUP(sName, wsName) \
    if (info.dictLookup(sName, &obj1)->isString())\
    {\
        TextString* s = new TextString(obj1.getString());\
        sRes += L"\"";\
        sRes += wsName;\
        sRes += L"\":\"";\
        std::wstring sValue = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());\
        NSStringExt::Replace(sValue, L"\"", L"\\\"");\
        sRes += sValue;\
        sRes += L"\",";\
        delete s;\
    }\

#define DICT_LOOKUP_DATE(sName, wsName) \
    if (info.dictLookup(sName, &obj1)->isString())\
    {\
        char* str = obj1.getString()->getCString();\
        int length = obj1.getString()->getLength();\
        if (str && length > 21)\
        {\
            TextString* s = new TextString(obj1.getString());\
            std::wstring sNoDate = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());\
            if (sNoDate.length() > 21)\
            {\
                std::wstring sDate = sNoDate.substr(2,  4) + L'-' + sNoDate.substr(6,  2) + L'-' + sNoDate.substr(8,  2) + L'T' +\
                                     sNoDate.substr(10, 2) + L':' + sNoDate.substr(12, 2) + L':' + sNoDate.substr(14, 2) + L".000+" +\
                                     sNoDate.substr(17, 2) + L':' + sNoDate.substr(20, 2);\
                NSStringExt::Replace(sDate, L"\"", L"\\\"");\
                sRes += L"\"";\
                sRes += wsName;\
                sRes += L"\":\"";\
                sRes += sDate;\
                sRes += L"\",";\
            }\
            delete s;\
        }\
    }\

    std::wstring CPdfReader::GetInfo()
    {
        if (!m_pInternal->m_pPDFDocument)
            return NULL;
        XRef* xref = m_pInternal->m_pPDFDocument->getXRef();
        BaseStream* str = m_pInternal->m_pPDFDocument->getBaseStream();
        if (!xref || !str)
            return NULL;

        std::wstring sRes = L"{";

        Object info, obj1;
        m_pInternal->m_pPDFDocument->getDocInfo(&info);
        if (info.isDict())
        {
            DICT_LOOKUP("Title",    L"Title");
            DICT_LOOKUP("Author",   L"Author");
            DICT_LOOKUP("Subject",  L"Subject");
            DICT_LOOKUP("Keywords", L"Keywords");
            DICT_LOOKUP("Creator",  L"Creator");
            DICT_LOOKUP("Producer", L"Producer");

            DICT_LOOKUP_DATE("CreationDate", L"CreationDate");
            DICT_LOOKUP_DATE("ModDate", L"ModDate");
        }

        info.free();
        obj1.free();

        std::wstring version = std::to_wstring(GetVersion());
        std::wstring::size_type posDot = version.find('.');
        if (posDot != std::wstring::npos)
            version = version.substr(0, posDot + 2);

        sRes += L"\"Version\":";
        sRes += version;
        double nW = 0;
        double nH = 0;
        double nDpi = 0;
        GetPageInfo(0, &nW, &nH, &nDpi, &nDpi);
        sRes += L",\"PageWidth\":";
        sRes += std::to_wstring((int)(nW * 100));
        sRes += L",\"PageHeight\":";
        sRes += std::to_wstring((int)(nH * 100));
        sRes += L",\"NumberOfPages\":";
        sRes += std::to_wstring(GetPagesCount());
        sRes += L",\"FastWebView\":";

        Object obj2, obj3, obj4, obj5, obj6;
        bool bLinearized = false;
        obj1.initNull();
        Parser* parser = new Parser(xref, new Lexer(xref, str->makeSubStream(str->getStart(), gFalse, 0, &obj1)), gTrue);
        parser->getObj(&obj1);
        parser->getObj(&obj2);
        parser->getObj(&obj3);
        parser->getObj(&obj4);
        if (obj1.isInt() && obj2.isInt() && obj3.isCmd("obj") && obj4.isDict())
        {
            obj4.dictLookup("Linearized", &obj5);
            obj4.dictLookup("L", &obj6);
            if (obj5.isNum() && obj5.getNum() > 0 && obj6.isNum())
            {
                unsigned long size = obj6.getNum();
                bLinearized = size == m_pInternal->m_nFileLength;
            }
            obj6.free();
            obj5.free();
        }
        obj4.free();
        obj3.free();
        obj2.free();
        obj1.free();
        delete parser;

        sRes += bLinearized ? L"true" : L"false";
        sRes += L",\"Tagged\":";

        bool bTagged = false;
        Object catDict, markInfoObj;
        if (xref->getCatalog(&catDict) && catDict.isDict() && catDict.dictLookup("MarkInfo", &markInfoObj) && markInfoObj.isDict())
        {
            Object marked, suspects;
            if (markInfoObj.dictLookup("Marked", &marked) && marked.isBool() && marked.getBool())
            {
                bTagged = true;
                // If Suspects is true, the document may not completely conform to Tagged PDF conventions.
                if (markInfoObj.dictLookup("Suspects", &suspects) && suspects.isBool() && suspects.getBool())
                    bTagged = false;
            }
            marked.free();
            suspects.free();
        }
        markInfoObj.free();
        catDict.free();

        sRes += bTagged ? L"true" : L"false";
        sRes += L"}";

        return sRes;
    }
#ifdef BUILDING_WASM_MODULE    
    void getBookmars(PDFDoc* pdfDoc, OutlineItem* pOutlineItem, NSWasm::CData& out, int level)
    {
        int nLengthTitle = pOutlineItem->getTitleLength();
        Unicode* pTitle = pOutlineItem->getTitle();
        std::string sTitle = NSStringExt::CConverter::GetUtf8FromUTF32(pTitle, nLengthTitle);

        LinkAction* pLinkAction = pOutlineItem->getAction();
        if (!pLinkAction)
            return;
        LinkActionKind kind = pLinkAction->getKind();
        if (kind != actionGoTo)
            return;

        GString* str = ((LinkGoTo*)pLinkAction)->getNamedDest();
        LinkDest* pLinkDest = str ? pdfDoc->findDest(str) : ((LinkGoTo*)pLinkAction)->getDest();
        if (!pLinkDest)
            return;
        int pg;
        if (pLinkDest->isPageRef())
        {
            Ref pageRef = pLinkDest->getPageRef();
            pg = pdfDoc->findPage(pageRef.num, pageRef.gen);
        }
        else
            pg = pLinkDest->getPageNum();
        if (pg == 0)
            pg = 1;
        double dy = 0;
        double dTop = pLinkDest->getTop();
        double dHeight = pdfDoc->getPageCropHeight(pg);
        if (dTop > 0 && dTop < dHeight)
            dy = dHeight - dTop;
        if (str)
            RELEASEOBJECT(pLinkDest);

        out.AddInt(pg - 1);
        out.AddInt(level);
        out.AddDouble(dy);
        out.WriteString((BYTE*)sTitle.c_str(), sTitle.length());

        pOutlineItem->open();
        GList* pList = pOutlineItem->getKids();
        if (!pList)
            return;
        int num = pList->getLength();
        for (int i = 0; i < num; i++)
        {
            OutlineItem* pOutlineItemKid = (OutlineItem*)pList->get(i);
            if (!pOutlineItemKid)
                continue;
            getBookmars(pdfDoc, pOutlineItemKid, out, level + 1);
        }
        pOutlineItem->close();
    }
    BYTE* CPdfReader::GetStructure()
    {
        if (!m_pInternal->m_pPDFDocument)
            return NULL;
        Outline* pOutline = m_pInternal->m_pPDFDocument->getOutline();
        if (!pOutline)
            return NULL;
        GList* pList = pOutline->getItems();
        if (!pList)
            return NULL;

        NSWasm::CData oRes;
        oRes.SkipLen();
        int num = pList->getLength();
        for (int i = 0; i < num; i++)
        {
            OutlineItem* pOutlineItem = (OutlineItem*)pList->get(i);
            if (pOutlineItem)
                getBookmars(m_pInternal->m_pPDFDocument, pOutlineItem, oRes, 1);
        }
        oRes.WriteLen();

        BYTE* bRes = oRes.GetBuffer();
        oRes.ClearWithoutAttack();
        return bRes;
    }
    BYTE* CPdfReader::GetLinks(int nPageIndex)
    {
        if (!m_pInternal->m_pPDFDocument)
            return NULL;

        nPageIndex++;

        NSWasm::CPageLink oLinks;
        double height = m_pInternal->m_pPDFDocument->getPageCropHeight(nPageIndex);

        // Гиперссылка
        Links* pLinks = m_pInternal->m_pPDFDocument->getLinks(nPageIndex);
        if (!pLinks)
            return NULL;

        int num = pLinks->getNumLinks();
        for (int i = 0; i < num; i++)
        {
            Link* pLink = pLinks->getLink(i);
            if (!pLink)
                continue;

            GString* str = NULL;
            double x1 = 0.0, y1 = 0.0, x2 = 0.0, y2 = 0.0, dy = 0.0;
            pLink->getRect(&x1, &y1, &x2, &y2);
            y1 = height - y1;
            y2 = height - y2;

            LinkAction* pLinkAction = pLink->getAction();
            if (!pLinkAction)
                continue;
            LinkActionKind kind = pLinkAction->getKind();
            if (kind == actionGoTo)
            {
                str = ((LinkGoTo*)pLinkAction)->getNamedDest();
                LinkDest* pLinkDest = str ? m_pInternal->m_pPDFDocument->findDest(str) : ((LinkGoTo*)pLinkAction)->getDest()->copy();
                if (pLinkDest)
                {
                    int pg;
                    if (pLinkDest->isPageRef())
                    {
                        Ref pageRef = pLinkDest->getPageRef();
                        pg = m_pInternal->m_pPDFDocument->findPage(pageRef.num, pageRef.gen);
                    }
                    else
                        pg = pLinkDest->getPageNum();
                    std::string sLink = "#" + std::to_string(pg - 1);
                    str = new GString(sLink.c_str());
                    dy  = m_pInternal->m_pPDFDocument->getPageCropHeight(pg) - pLinkDest->getTop();
                }
                RELEASEOBJECT(pLinkDest);
            }
            else if (kind == actionURI)
                str = ((LinkURI*)pLinkAction)->getURI()->copy();

            oLinks.m_arLinks.push_back({str ? std::string(str->getCString(), str->getLength()) : "", dy, x1, y2, x2 - x1, y1 - y2});
            RELEASEOBJECT(str);
        }
        RELEASEOBJECT(pLinks);

        // Текст-ссылка
        TextOutputControl textOutControl;
        textOutControl.mode = textOutReadingOrder;
        TextOutputDev* pTextOut = new TextOutputDev(NULL, &textOutControl, gFalse);
        m_pInternal->m_pPDFDocument->displayPage(pTextOut, nPageIndex, 72, 72, 0, gFalse, gTrue, gFalse);
        m_pInternal->m_pPDFDocument->processLinks(pTextOut, nPageIndex);
        TextWordList* pWordList = pTextOut->makeWordList();
        for (int i = 0; i < pWordList->getLength(); i++)
        {
            TextWord* pWord = pWordList->get(i);
            if (!pWord)
                continue;
            GString* sLink = pWord->getText();
            if (!sLink)
                continue;
            std::string link(sLink->getCString(), sLink->getLength());
            size_t find = link.find("http://");
            if (find == std::string::npos)
                find = link.find("https://");
            if (find == std::string::npos)
                find = link.find("www.");
            if (find != std::string::npos)
            {
                link.erase(0, find);
                double x1, y1, x2, y2;
                pWord->getBBox(&x1, &y1, &x2, &y2);
                oLinks.m_arLinks.push_back({link, 0, x1, y1, x2 - x1, y2 - y1});
            }
        }
        RELEASEOBJECT(pTextOut);

        return oLinks.Serialize();
    }
#endif
}
