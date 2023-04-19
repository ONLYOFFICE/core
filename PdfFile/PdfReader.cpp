/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "PdfReader.h"

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/common/StringExt.h"
#include "../../DesktopEditor/graphics/pro/js/wasm/src/serialize.h"

#include "lib/xpdf/PDFDoc.h"
#include "lib/xpdf/GlobalParams.h"
#include "lib/xpdf/ErrorCodes.h"
#include "lib/xpdf/TextString.h"
#include "lib/xpdf/Lexer.h"
#include "lib/xpdf/Parser.h"
#include "SrcReader/Adaptors.h"
#include "lib/xpdf/Outline.h"
#include "lib/xpdf/Link.h"
#include "lib/xpdf/TextOutputDev.h"
#include "lib/xpdf/AcroForm.h"
#include "lib/xpdf/Annot.h"
#include "lib/goo/GList.h"

#include <vector>

CPdfReader::CPdfReader(NSFonts::IApplicationFonts* pAppFonts, IOfficeDrawingFile* pRenderer)
{
    m_pRenderer    = pRenderer;
    m_wsTempFolder = L"";
    m_pPDFDocument = NULL;
    m_nFileLength  = 0;

    globalParams  = new GlobalParamsAdaptor(NULL);
#ifndef _DEBUG
    globalParams->setErrQuiet(gTrue);
#endif

    m_pFontList = new PdfReader::CFontList();

    // Создаем менеджер шрифтов с собственным кэшем
    m_pFontManager = pAppFonts->GenerateFontManager();
    NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
    pMeasurerCache->SetStreams(pAppFonts->GetStreams());
    m_pFontManager->SetOwnerCache(pMeasurerCache);
    pMeasurerCache->SetCacheSize(1);
    ((GlobalParamsAdaptor*)globalParams)->SetFontManager(m_pFontManager);
#ifndef BUILDING_WASM_MODULE
    globalParams->setupBaseFonts(NULL);
    SetCMapFile(NSFile::GetProcessDirectory() + L"/cmap.bin");
#else
    globalParams->setDrawFormFields(gFalse);
    SetCMapMemory(NULL, 0);
#endif

    m_eError = errNone;
}
CPdfReader::~CPdfReader()
{
    if (m_pFontList)
    {
        m_pFontList->Clear();
        delete m_pFontList;
    }

    if (!m_wsTempFolder.empty())
    {
        NSDirectory::DeleteDirectory(m_wsTempFolder);
        m_wsTempFolder = L"";
    }

    RELEASEOBJECT(m_pPDFDocument);
    RELEASEOBJECT(globalParams);
    RELEASEINTERFACE(m_pFontManager);
}

bool scanFonts(Dict *pResources, PDFDoc *pDoc, const std::vector<std::string>& arrCMap)
{
    Object oFonts;
    if (pResources->lookup("Font", &oFonts) && oFonts.isDict())
    {
        for (int i = 0, nLength = oFonts.dictGetLength(); i < nLength; ++i)
        {
            Object oFont, oEncoding;
            if (!oFonts.dictGetVal(i, &oFont) || !oFont.isDict() || !oFont.dictLookup("Encoding", &oEncoding) || !oEncoding.isName())
            {
                oFont.free(); oEncoding.free();
                continue;
            }

            char* sName = oEncoding.getName();
            if (std::find(arrCMap.begin(), arrCMap.end(), sName) != arrCMap.end())
            {
                oEncoding.free(); oFont.free(); oFonts.free();
                return true;
            }
            oEncoding.free(); oFont.free();
        }
    }
    oFonts.free();

#define SCAN_FONTS(sName)\
{\
    Object oObject;\
    if (pResources->lookup(sName, &oObject) && oObject.isDict())\
    {\
        for (int i = 0, nLength = oObject.dictGetLength(); i < nLength; ++i)\
        {\
            Object oXObj, oResources;\
            if (!oObject.dictGetVal(i, &oXObj) || !oXObj.isStream() || !oXObj.streamGetDict()->lookup("Resources", &oResources) || !oResources.isDict())\
            {\
                oXObj.free(); oResources.free();\
                continue;\
            }\
            oXObj.free();\
            if (scanFonts(oResources.getDict(), pDoc, arrCMap))\
            {\
                oResources.free(); oObject.free();\
                return true;\
            }\
            oResources.free();\
        }\
    }\
    oObject.free();\
}
    SCAN_FONTS("XObject");
    SCAN_FONTS("Pattern");

    Object oExtGState;
    if (pResources->lookup("ExtGState", &oExtGState) && oExtGState.isDict())
    {
        for (int i = 0, nLength = oExtGState.dictGetLength(); i < nLength; ++i)
        {
            Object oGS, oSMask, oSMaskGroup, oResources;
            if (!oExtGState.dictGetVal(i, &oGS) || !oGS.isDict() || !oGS.dictLookup("SMask", &oSMask) || !oSMask.isDict() || !oSMask.dictLookup("G", &oSMaskGroup) || !oSMaskGroup.isStream() ||
                    oSMaskGroup.streamGetDict()->lookup("Resources", &oResources) || !oResources.isDict())
            {
                oGS.free(); oSMask.free(); oSMaskGroup.free(); oResources.free();
                continue;
            }
            oGS.free(); oSMask.free(); oSMaskGroup.free();
            if (scanFonts(oResources.getDict(), pDoc, arrCMap))
            {
                oResources.free(); oExtGState.free();
                return true;
            }
            oResources.free();
        }
    }
    oExtGState.free();

    return false;
}
bool CPdfReader::IsNeedCMap()
{
    std::vector<std::string> arrCMap = {"GB-EUC-H", "GB-EUC-V", "GB-H", "GB-V", "GBpc-EUC-H", "GBpc-EUC-V", "GBK-EUC-H",
      "GBK-EUC-V", "GBKp-EUC-H", "GBKp-EUC-V", "GBK2K-H", "GBK2K-V", "GBT-H", "GBT-V", "GBTpc-EUC-H", "GBTpc-EUC-V",
      "UniGB-UCS2-H", "UniGB-UCS2-V", "UniGB-UTF8-H", "UniGB-UTF8-V", "UniGB-UTF16-H", "UniGB-UTF16-V", "UniGB-UTF32-H",
      "UniGB-UTF32-V", "B5pc-H", "B5pc-V", "B5-H", "B5-V", "HKscs-B5-H", "HKscs-B5-V", "HKdla-B5-H", "HKdla-B5-V",
      "HKdlb-B5-H", "HKdlb-B5-V", "HKgccs-B5-H", "HKgccs-B5-V", "HKm314-B5-H", "HKm314-B5-V", "HKm471-B5-H",
      "HKm471-B5-V", "ETen-B5-H", "ETen-B5-V", "ETenms-B5-H", "ETenms-B5-V", "ETHK-B5-H", "ETHK-B5-V", "CNS-EUC-H",
      "CNS-EUC-V", "CNS1-H", "CNS1-V", "CNS2-H", "CNS2-V", "UniCNS-UCS2-H", "UniCNS-UCS2-V", "UniCNS-UTF8-H",
      "UniCNS-UTF8-V", "UniCNS-UTF16-H", "UniCNS-UTF16-V", "UniCNS-UTF32-H", "UniCNS-UTF32-V", "78-EUC-H", "78-EUC-V",
      "78-H", "78-V", "78-RKSJ-H", "78-RKSJ-V", "78ms-RKSJ-H", "78ms-RKSJ-V","83pv-RKSJ-H", "90ms-RKSJ-H", "90ms-RKSJ-V",
      "90msp-RKSJ-H", "90msp-RKSJ-V", "90pv-RKSJ-H", "90pv-RKSJ-V", "Add-H", "Add-V", "Add-RKSJ-H", "Add-RKSJ-V",
      "EUC-H", "EUC-V", "Ext-RKSJ-H", "Ext-RKSJ-V", "H", "V", "NWP-H", "NWP-V", "RKSJ-H", "RKSJ-V", "UniJIS-UCS2-H",
      "UniJIS-UCS2-V", "UniJIS-UCS2-HW-H", "UniJIS-UCS2-HW-V", "UniJIS-UTF8-H", "UniJIS-UTF8-V", "UniJIS-UTF16-H",
      "UniJIS-UTF16-V", "UniJIS-UTF32-H", "UniJIS-UTF32-V", "UniJIS2004-UTF8-H", "UniJIS2004-UTF8-V", "UniJIS2004-UTF16-H",
      "UniJIS2004-UTF16-V", "UniJIS2004-UTF32-H", "UniJIS2004-UTF32-V", "UniJISPro-UCS2-V", "UniJISPro-UCS2-HW-V",
      "UniJISPro-UTF8-V", "UniJISX0213-UTF32-H", "UniJISX0213-UTF32-V", "UniJISX02132004-UTF32-H", "UniJISX02132004-UTF32-V",
      "WP-Symbol", "Hankaku", "Hiragana", "Katakana", "Roman", "KSC-EUC-H", "KSC-EUC-V", "KSC-H", "KSC-V", "KSC-Johab-H",
      "KSC-Johab-V", "KSCms-UHC-H", "KSCms-UHC-V", "KSCms-UHC-HW-H", "KSCms-UHC-HW-V", "KSCpc-EUC-H", "KSCpc-EUC-V",
      "UniKS-UCS2-H", "UniKS-UCS2-V", "UniKS-UTF8-H", "UniKS-UTF8-V", "UniKS-UTF16-H", "UniKS-UTF16-V", "UniKS-UTF32-H",
      "UniKS-UTF32-V", "UniAKR-UTF8-H", "UniAKR-UTF16-H", "UniAKR-UTF32-H"};

    if (!m_pPDFDocument || !m_pPDFDocument->getCatalog())
        return false;

    for (int nPage = 0, nLastPage = m_pPDFDocument->getNumPages(); nPage < nLastPage; ++nPage)
    {
        Page* pPage = m_pPDFDocument->getCatalog()->getPage(nPage + 1);
        Dict* pResources = pPage->getResourceDict();
        if (pResources && scanFonts(pResources, m_pPDFDocument, arrCMap))
            return true;
    }
    return false;
}
void CPdfReader::SetCMapMemory(BYTE* pData, DWORD nSizeData)
{
    ((GlobalParamsAdaptor*)globalParams)->SetCMapMemory(pData, nSizeData);
}
void CPdfReader::SetCMapFolder(const std::wstring& sFolder)
{
    ((GlobalParamsAdaptor*)globalParams)->SetCMapFolder(sFolder);
}
void CPdfReader::SetCMapFile(const std::wstring& sFile)
{
    ((GlobalParamsAdaptor*)globalParams)->SetCMapFile(sFile);
}
bool CPdfReader::LoadFromFile(NSFonts::IApplicationFonts* pAppFonts, const std::wstring& wsSrcPath, const std::wstring& wsOwnerPassword, const std::wstring& wsUserPassword)
{
    RELEASEINTERFACE(m_pFontManager);
    m_pFontManager = pAppFonts->GenerateFontManager();
    NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
    pMeasurerCache->SetStreams(pAppFonts->GetStreams());
    m_pFontManager->SetOwnerCache(pMeasurerCache);
    pMeasurerCache->SetCacheSize(1);
    ((GlobalParamsAdaptor*)globalParams)->SetFontManager(m_pFontManager);

    RELEASEOBJECT(m_pPDFDocument);

    if (m_wsTempFolder == L"")
        SetTempDirectory(NSDirectory::GetTempPath());

    m_eError = errNone;
    GString* owner_pswd = NSStrings::CreateString(wsOwnerPassword);
    GString* user_pswd  = NSStrings::CreateString(wsUserPassword);

    // конвертим путь в utf8 - под виндой они сконвертят в юникод, а на остальных - так и надо
    std::string sPathUtf8 = U_TO_UTF8(wsSrcPath);
    m_pPDFDocument = new PDFDoc((char*)sPathUtf8.c_str(), owner_pswd, user_pswd);

    delete owner_pswd;
    delete user_pswd;

    NSFile::CFileBinary oFile;
    if (oFile.OpenFile(wsSrcPath))
    {
        m_nFileLength = oFile.GetFileSize();
        oFile.CloseFile();
    }

    m_eError = m_pPDFDocument ? m_pPDFDocument->getErrorCode() : errMemory;

    if (!m_pPDFDocument || !m_pPDFDocument->isOk())
    {
        RELEASEOBJECT(m_pPDFDocument);
        return false;
    }

    m_pFontList->Clear();

    return true;
}
bool CPdfReader::LoadFromMemory(NSFonts::IApplicationFonts* pAppFonts, BYTE* data, DWORD length, const std::wstring& owner_password, const std::wstring& user_password)
{
    RELEASEINTERFACE(m_pFontManager);
    m_pFontManager = pAppFonts->GenerateFontManager();
    NSFonts::IFontsCache* pMeasurerCache = NSFonts::NSFontCache::Create();
    pMeasurerCache->SetStreams(pAppFonts->GetStreams());
    m_pFontManager->SetOwnerCache(pMeasurerCache);
    pMeasurerCache->SetCacheSize(1);
    ((GlobalParamsAdaptor*)globalParams)->SetFontManager(m_pFontManager);

    RELEASEOBJECT(m_pPDFDocument);
    m_eError = errNone;
    GString* owner_pswd = NSStrings::CreateString(owner_password);
    GString* user_pswd  = NSStrings::CreateString(user_password);

    Object obj;
    obj.initNull();
    // будет освобожден в деструкторе PDFDoc
    BaseStream *str = new MemStream((char*)data, 0, length, &obj);
    m_pPDFDocument  = new PDFDoc(str, owner_pswd, user_pswd);
    m_nFileLength = length;

    delete owner_pswd;
    delete user_pswd;

    m_eError = m_pPDFDocument ? m_pPDFDocument->getErrorCode() : errMemory;

    if (!m_pPDFDocument || !m_pPDFDocument->isOk())
    {
        RELEASEOBJECT(m_pPDFDocument);
        return false;
    }

    m_pFontList->Clear();

    return true;
}
void CPdfReader::Close()
{
    RELEASEOBJECT(m_pPDFDocument);
}

int CPdfReader::GetError()
{
    if (!m_pPDFDocument)
        return m_eError;

    if (m_pPDFDocument->isOk())
        return 0;

    return m_pPDFDocument->getErrorCode();
}
void CPdfReader::GetPageInfo(int _nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
    int nPageIndex = _nPageIndex + 1;

    if (!m_pPDFDocument)
        return;

    int nRotate = m_pPDFDocument->getPageRotate(nPageIndex);
    if (nRotate % 180 == 0)
    {
        *pdWidth  = m_pPDFDocument->getPageCropWidth(nPageIndex);
        *pdHeight = m_pPDFDocument->getPageCropHeight(nPageIndex);
    }
    else
    {
        *pdHeight = m_pPDFDocument->getPageCropWidth(nPageIndex);
        *pdWidth  = m_pPDFDocument->getPageCropHeight(nPageIndex);
    }

    *pdDpiX = 72.0;
    *pdDpiY = 72.0;
}
void CPdfReader::DrawPageOnRenderer(IRenderer* pRenderer, int _nPageIndex, bool* pbBreak)
{
    int nPageIndex = _nPageIndex + 1;

    if (m_pPDFDocument && pRenderer)
    {
        PdfReader::RendererOutputDev oRendererOut(pRenderer, m_pFontManager, m_pFontList);
        oRendererOut.NewPDF(m_pPDFDocument->getXRef());
        oRendererOut.SetBreak(pbBreak);
        m_pPDFDocument->displayPage(&oRendererOut, nPageIndex, 72.0, 72.0, 0, gFalse, gTrue, gFalse);
    }
}
void CPdfReader::SetTempDirectory(const std::wstring& wsTempFolder)
{
    if (!m_wsTempFolder.empty())
    {
        NSDirectory::DeleteDirectory(m_wsTempFolder);
        m_wsTempFolder = wsTempFolder;
    }

    if (!wsTempFolder.empty())
    {
        std::wstring wsFolderName = wsTempFolder + L"/pdftemp";
        std::wstring wsFolder = wsFolderName;
        int nCounter = 0;
        while (NSDirectory::Exists(wsFolder))
        {
            nCounter++;
            wsFolder = wsFolderName + L"_" + std::to_wstring(nCounter);
        }
        NSDirectory::CreateDirectory(wsFolder);
        m_wsTempFolder = wsFolder;
    }
    else
        m_wsTempFolder = L"";

    if (globalParams)
        ((GlobalParamsAdaptor*)globalParams)->SetTempFolder(m_wsTempFolder.c_str());
}
std::wstring CPdfReader::GetTempDirectory()
{
    return m_wsTempFolder;
}

void CPdfReader::ChangeLength(DWORD nLength)
{
    m_nFileLength = nLength;
}

std::wstring CPdfReader::GetInfo()
{
    if (!m_pPDFDocument)
        return NULL;
    XRef* xref = m_pPDFDocument->getXRef();
    BaseStream* str = m_pPDFDocument->getBaseStream();
    if (!xref || !str)
        return NULL;

    std::wstring sRes = L"{";

    Object info, obj1;
    m_pPDFDocument->getDocInfo(&info);
    if (info.isDict())
    {
#define DICT_LOOKUP(sName, wsName) \
if (info.dictLookup(sName, &obj1)->isString())\
{\
    TextString* s = new TextString(obj1.getString());\
    std::wstring sValue = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());\
    delete s;\
    NSStringExt::Replace(sValue, L"\"", L"\\\"");\
    size_t nFind = sValue.find(L'\000');\
    while (nFind != std::wstring::npos)\
    {\
        sValue.erase(nFind, 1);\
        nFind = sValue.find(L'\000');\
    }\
    if (!sValue.empty())\
    {\
        sRes += L"\"";\
        sRes += wsName;\
        sRes += L"\":\"";\
        sRes += sValue;\
        sRes += L"\",";\
    }\
}
        DICT_LOOKUP("Title",    L"Title");
        DICT_LOOKUP("Author",   L"Author");
        DICT_LOOKUP("Subject",  L"Subject");
        DICT_LOOKUP("Keywords", L"Keywords");
        DICT_LOOKUP("Creator",  L"Creator");
        DICT_LOOKUP("Producer", L"Producer");
#define DICT_LOOKUP_DATE(sName, wsName) \
if (info.dictLookup(sName, &obj1)->isString())\
{\
    char* str = obj1.getString()->getCString();\
    if (str)\
    {\
        TextString* s = new TextString(obj1.getString());\
        std::wstring sNoDate = NSStringExt::CConverter::GetUnicodeFromUTF32(s->getUnicode(), s->getLength());\
        if (sNoDate.length() > 16)\
        {\
            std::wstring sDate = sNoDate.substr(2,  4) + L'-' + sNoDate.substr(6,  2) + L'-' + sNoDate.substr(8,  2) + L'T' +\
                                 sNoDate.substr(10, 2) + L':' + sNoDate.substr(12, 2) + L':' + sNoDate.substr(14, 2);\
            if (sNoDate.length() > 21)\
                sDate += (L".000" + sNoDate.substr(16, 3) + L':' + sNoDate.substr(20, 2));\
            else\
                sDate += L"Z";\
            NSStringExt::Replace(sDate, L"\"", L"\\\"");\
            sRes += L"\"";\
            sRes += wsName;\
            sRes += L"\":\"";\
            sRes += sDate;\
            sRes += L"\",";\
        }\
        delete s;\
    }\
}
        DICT_LOOKUP_DATE("CreationDate", L"CreationDate");
        DICT_LOOKUP_DATE("ModDate", L"ModDate");
    }

    info.free();
    obj1.free();

    std::wstring version = std::to_wstring(m_pPDFDocument->getPDFVersion());
    std::wstring::size_type posDot = version.find('.');
    if (posDot != std::wstring::npos)
        version = version.substr(0, posDot + 2);
    if (!version.empty())
        sRes += (L"\"Version\":" + version + L",");

    double nW = 0;
    double nH = 0;
    double nDpi = 0;
    GetPageInfo(0, &nW, &nH, &nDpi, &nDpi);
    sRes += L"\"PageWidth\":";
    sRes += std::to_wstring((int)(nW * 100));
    sRes += L",\"PageHeight\":";
    sRes += std::to_wstring((int)(nH * 100));
    sRes += L",\"NumberOfPages\":";
    sRes += std::to_wstring(m_pPDFDocument->getNumPages());
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
            unsigned long size = (unsigned long)obj6.getNum();
            bLinearized = size == m_nFileLength;
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
        if (markInfoObj.dictLookup("Marked", &marked) && marked.isBool() && marked.getBool() == gTrue)
        {
            bTagged = true;
            // If Suspects is true, the document may not completely conform to Tagged PDF conventions.
            if (markInfoObj.dictLookup("Suspects", &suspects) && suspects.isBool() && suspects.getBool() == gTrue)
                bTagged = false;
        }
        marked.free();
        suspects.free();
    }
    markInfoObj.free();
    catDict.free();

    sRes += bTagged ? L"true}" : L"false}";

    return sRes;
}

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
    out.WriteString((BYTE*)sTitle.c_str(), (unsigned int)sTitle.length());

    pOutlineItem->open();
    GList* pList = pOutlineItem->getKids();
    if (!pList)
        return;
    for (int i = 0, num = pList->getLength(); i < num; i++)
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
    if (!m_pPDFDocument)
        return NULL;
    Outline* pOutline = m_pPDFDocument->getOutline();
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
            getBookmars(m_pPDFDocument, pOutlineItem, oRes, 1);
    }
    oRes.WriteLen();

    BYTE* bRes = oRes.GetBuffer();
    oRes.ClearWithoutAttack();
    return bRes;
}
BYTE* CPdfReader::GetLinks(int nPageIndex)
{
    if (!m_pPDFDocument)
        return NULL;

    nPageIndex++;

    NSWasm::CPageLink oLinks;
    double height = m_pPDFDocument->getPageCropHeight(nPageIndex);

    // Гиперссылка
    Links* pLinks = m_pPDFDocument->getLinks(nPageIndex);
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
            LinkDest* pLinkDest = str ? m_pPDFDocument->findDest(str) : ((LinkGoTo*)pLinkAction)->getDest()->copy();
            if (pLinkDest)
            {
                int pg;
                if (pLinkDest->isPageRef())
                {
                    Ref pageRef = pLinkDest->getPageRef();
                    pg = m_pPDFDocument->findPage(pageRef.num, pageRef.gen);
                }
                else
                    pg = pLinkDest->getPageNum();
                std::string sLink = "#" + std::to_string(pg - 1);
                str = new GString(sLink.c_str());
                dy  = m_pPDFDocument->getPageCropHeight(pg) - pLinkDest->getTop();
            }
            else
                str = NULL;
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
    m_pPDFDocument->displayPage(pTextOut, nPageIndex, 72.0, 72.0, 0, gFalse, gTrue, gFalse);
    m_pPDFDocument->processLinks(pTextOut, nPageIndex);
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

void getAction(PDFDoc* pdfDoc, NSWasm::CData& oRes, Object* oAction, int nAnnot)
{
    AcroForm* pAcroForms = pdfDoc->getCatalog()->getForm();

    Object oActType;
    std::string sSName;
    if (oAction->dictLookup("S", &oActType)->isName())
    {
        sSName = oActType.getName();
        oRes.WriteString((BYTE*)sSName.c_str(), (unsigned int)sSName.length());
    }
    else
        oRes.WriteString(NULL, 0);
    oActType.free();

    LinkAction* oAct = LinkAction::parseAction(oAction);
    if (!oAct)
        return;

    LinkActionKind kind = oAct->getKind();
    switch (kind)
    {
    // Переход внутри файла
    case actionGoTo:
    {
        GString* str = ((LinkGoTo*)oAct)->getNamedDest();
        LinkDest* pLinkDest = str ? pdfDoc->findDest(str) : ((LinkGoTo*)oAct)->getDest();
        if (!pLinkDest)
        {
            oRes.WriteString(NULL, 0);
            oRes.AddDouble(0.0);
            break;
        }
        int pg;
        if (pLinkDest->isPageRef())
        {
            Ref pageRef = pLinkDest->getPageRef();
            pg = pdfDoc->findPage(pageRef.num, pageRef.gen);
        }
        else
            pg = pLinkDest->getPageNum();
        std::string sLink = "#" + std::to_string(pg - 1);
        double dy = pdfDoc->getPageCropHeight(pg) - pLinkDest->getTop();
        oRes.WriteString((BYTE*)sLink.c_str(), (unsigned int)sLink.length());
        oRes.AddDouble(dy);
        break;
    }
    // Переход к внешнему файлу
    case actionGoToR:
    {
        break;
    }
    // Запуск стороннего приложения
    case actionLaunch:
    {
        break;
    }
    // Внешняя ссылка
    case actionURI:
    {
        TextString* s = new TextString(((LinkURI*)oAct)->getURI());
        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
        delete s;
        break;
    }
    // Нестандартно именованные действия
    case actionNamed:
    {
        TextString* s = new TextString(((LinkNamed*)oAct)->getName());
        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
        delete s;
        break;
    }
    // Воспроизведение фильма
    case actionMovie:
    {
        break;
    }
    // JavaScript
    case actionJavaScript:
    {
        TextString* s = new TextString(((LinkJavaScript*)oAct)->getJS());
        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
        delete s;
        break;
    }
    // Отправка формы
    case actionSubmitForm:
    {
        break;
    }
    // Скрытие аннотаций
    case actionHide:
    {
        oRes.AddInt(((LinkHide*)oAct)->getHideFlag());
        Object* oHideObj = ((LinkHide*)oAct)->getFields();
        int nHide = 1, k = 0;
        if (oHideObj->isArray())
            nHide = oHideObj->arrayGetLength();
        int nFieldsPos = oRes.GetSize();
        int nFields = 0;
        oRes.AddInt(nFields);

        Object oHide;
        oHideObj->copy(&oHide);
        do
        {
            if (oHideObj->isArray())
            {
                oHide.free();
                oHideObj->arrayGetNF(k, &oHide);
            }
            if (oHide.isString())
            {
                GString* sField = oHide.getString();
                GList* arrFind = pAcroForms->findFieldIdx(sField);
                if (arrFind)
                {
                    for (int i = 0; i < arrFind->getLength(); ++i)
                    {
                        nFields++;
                        oRes.AddInt(*((int*)arrFind->get(i)));
                    }
                }
                deleteGList(arrFind, int);
            }
            else if (oHide.isRef())
            {
                int nFind = pAcroForms->findFieldIdx(&oHide);
                if (nFind >= 0)
                {
                    nFields++;
                    oRes.AddInt(nFind);
                }
            }
            k++;
        } while (k < nHide);
        oHide.free();

        oRes.AddInt(nFields, nFieldsPos);
        break;
    }
    // Неизвестное действие
    case actionUnknown:
    default:
    {
        if (sSName == "ResetForm")
        {
            Object oObj;
            if (oAction->dictLookup("Flags", &oObj)->isInt())
                oRes.AddInt(oObj.getInt());
            else
                oRes.AddInt(0);
            oObj.free();

            if (oAction->dictLookup("Fields", &oObj)->isArray())
            {
                int nFieldsPos = oRes.GetSize();
                int nFields = 0;
                oRes.AddInt(nFields);
                for (int j = 0; j < oObj.arrayGetLength(); ++j)
                {
                    Object oField;
                    oObj.arrayGetNF(j, &oField);
                    if (oField.isString())
                    {
                        GString* sField = oField.getString();
                        GList* arrFind = pAcroForms->findFieldIdx(sField);
                        if (arrFind)
                        {
                            for (int i = 0; i < arrFind->getLength(); ++i)
                            {
                                nFields++;
                                oRes.AddInt(*((int*)arrFind->get(i)));
                            }
                        }
                        deleteGList(arrFind, int);
                    }
                    else if (oField.isRef())
                    {
                        int nFind = pAcroForms->findFieldIdx(&oField);
                        if (nFind >= 0)
                        {
                            nFields++;
                            oRes.AddInt(nFind);
                        }
                    }
                    oField.free();
                }
                oRes.AddInt(nFields, nFieldsPos);
            }
            else
                oRes.AddInt(0);
            oObj.free();
        }
        break;
    }
    }

    Object oNextAction;
    if (oAction->dictLookup("Next", &oNextAction)->isDict())
    {
        oRes.WriteBYTE(1);
        getAction(pdfDoc, oRes, &oNextAction, nAnnot);
    }
    else
        oRes.WriteBYTE(0);

    RELEASEOBJECT(oAct);
};
BYTE* CPdfReader::GetWidgets()
{
    if (!m_pPDFDocument || !m_pPDFDocument->getCatalog())
        return NULL;

    AcroForm* pAcroForms = m_pPDFDocument->getCatalog()->getForm();
    if (!pAcroForms)
        return NULL;

    NSWasm::CData oRes;
    oRes.SkipLen();

    for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
    {
        AcroFormField* pField = pAcroForms->getField(i);
        Object oFieldRef, oField;
        XRef* xref = m_pPDFDocument->getXRef();
        if (!xref || !pField->getFieldRef(&oFieldRef)->isRef() || !oFieldRef.fetch(xref, &oField)->isDict())
        {
            // TODO Если ошибочная аннотация
            oRes.AddInt(0xFFFFFFFF);
            oFieldRef.free(); oField.free();
            continue;
        }

        // Номер аннотации для сопоставления с AP
        oRes.AddInt(i);

        Object oObj;
        // Флаг аннотации - F
        int nAnnotFlag = 0;
        if (pField->fieldLookup("F", &oObj)->isInt())
            nAnnotFlag = oObj.getInt();
        oObj.free();
        oRes.AddInt(nAnnotFlag);

        // Полное имя поля - T (parent_full_name.child_name)
        int nLengthName;
        Unicode* uName = pField->getName(&nLengthName);
        std::string sTName = NSStringExt::CConverter::GetUtf8FromUTF32(uName, nLengthName);
        oRes.WriteString((BYTE*)sTName.c_str(), (unsigned int)sTName.length());
        gfree(uName);

        // Номер страницы - P
        int nPage = pField->getPageNum();
        oRes.AddInt(nPage - 1);

        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        GetPageInfo(nPage - 1, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        // Координаты - BBox
        double dx1, dy1, dx2, dy2;
        pField->getBBox(&dx1, &dy1, &dx2, &dy2);
        double dTemp = dy1;
        dy1 = dHeight - dy2;
        dy2 = dHeight - dTemp;
        std::string sX1 = std::to_string(dx1);
        std::string sY1 = std::to_string(dy1);
        std::string sX2 = std::to_string(dx2);
        std::string sY2 = std::to_string(dy2);
        oRes.WriteString((BYTE*)sX1.c_str(), (unsigned int)sX1.length());
        oRes.WriteString((BYTE*)sY1.c_str(), (unsigned int)sY1.length());
        oRes.WriteString((BYTE*)sX2.c_str(), (unsigned int)sX2.length());
        oRes.WriteString((BYTE*)sY2.c_str(), (unsigned int)sY2.length());

        // Выравнивание текста - Q
        int nQ = 0;
        if (pField->fieldLookup("Q", &oObj)->isInt())
            nQ = oObj.getInt();
        oObj.free();
        oRes.AddInt(nQ);

        // Тип - FT + флаги
        AcroFormFieldType oType = pField->getAcroFormFieldType();
        std::string sType;
        switch (oType)
        {
        case acroFormFieldPushbutton:    sType = "button";        break;
        case acroFormFieldRadioButton:   sType = "radiobutton";   break;
        case acroFormFieldCheckbox:      sType = "checkbox";      break;
        case acroFormFieldFileSelect:    sType = "text"/*"fileselect"*/;    break;
        case acroFormFieldMultilineText: sType = "text"/*"multilinetext"*/; break;
        case acroFormFieldText:          sType = "text";          break;
        case acroFormFieldBarcode:       sType = "text"/*"barcode"*/;       break;
        case acroFormFieldComboBox:      sType = "combobox";      break;
        case acroFormFieldListBox:       sType = "listbox";       break;
        case acroFormFieldSignature:     sType = "signature";     break;
        default:                         sType = "";              break;
        }
        oRes.WriteString((BYTE*)sType.c_str(), (unsigned int)sType.length());

        // Флаг - Ff
        unsigned int nFieldFlag = pField->getFlags();
        oRes.AddInt(nFieldFlag);

        int nFlags = 0;
        int nFlagPos = oRes.GetSize();
        oRes.AddInt(nFlags);

#define DICT_LOOKUP_STRING(func, sName, byte) \
{\
if (func(sName, &oObj)->isString())\
{\
    TextString* s = new TextString(oObj.getString());\
    std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());\
    nFlags |= (1 << byte);\
    oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());\
    delete s;\
}\
oObj.free();\
}
        // 1 - Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
        DICT_LOOKUP_STRING(pField->fieldLookup, "TU", 0);

        // 2 - Строка стиля по умолчанию - DS
        DICT_LOOKUP_STRING(pField->fieldLookup, "DS", 1);

        // 3 - Эффекты границы - BE
        Object oBorderBE, oBorderBEI;
        if (pField->fieldLookup("BE", &oObj)->isDict() && oObj.dictLookup("S", &oBorderBE)->isName("C") && oObj.dictLookup("I", &oBorderBEI)->isNum())
        {
            nFlags |= (1 << 2);
            oRes.AddDouble(oBorderBEI.getNum());
        }
        oObj.free(); oBorderBE.free(); oBorderBEI.free();

        // 4 - Режим выделения - H
        if (pField->fieldLookup("H", &oObj)->isName())
        {
            nFlags |= (1 << 3);
            std::string sName(oObj.getName());
            oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
        }
        oObj.free();

        // 5 - Границы и Dash Pattern - Border/BS
        Object oBorder;
        unsigned int nBorderType = 5;
        double dBorderWidth = 1, dDashesAlternating = 3, dGaps = 3;
        if (pField->fieldLookup("BS", &oBorder)->isDict())
        {
            nBorderType = annotBorderSolid;
            Object oV;
            if (oBorder.dictLookup("S", &oV)->isName())
            {
                if (oV.isName("S"))
                    nBorderType = annotBorderSolid;
                else if (oV.isName("D"))
                    nBorderType = annotBorderDashed;
                else if (oV.isName("B"))
                    nBorderType = annotBorderBeveled;
                else if (oV.isName("I"))
                    nBorderType = annotBorderInset;
                else if (oV.isName("U"))
                    nBorderType = annotBorderUnderlined;
            }
            oV.free();
            if (oBorder.dictLookup("W", &oV)->isNum())
                dBorderWidth = oV.getNum();
            oV.free();
            if (oBorder.dictLookup("D", &oV)->isArray())
            {
                Object oV1;
                if (oV.arrayGet(0, &oV1)->isNum())
                    dDashesAlternating = oV1.getNum();
                oV1.free();
                if (oV.arrayGet(1, &oV1)->isNum())
                    dGaps = oV1.getNum();
                else
                    dGaps = dDashesAlternating;
                oV1.free();
            }
            oV.free();
        }
        else
        {
            oBorder.free();
            if (pField->fieldLookup("Border", &oBorder)->isArray() && oBorder.arrayGetLength() > 2)
            {
                nBorderType = annotBorderSolid;
                Object oV;
                if (oBorder.arrayGet(2, &oV) && oV.isNum())
                    dBorderWidth = oV.getNum();
                oV.free();

                if (oBorder.arrayGetLength() > 3 && oBorder.arrayGet(3, &oV)->isArray() && oV.arrayGetLength() > 1)
                {
                    nBorderType = annotBorderDashed;
                    Object oV1;
                    if (oV.arrayGet(0, &oV1)->isNum())
                        dDashesAlternating = oV1.getNum();
                    oV1.free();
                    if (oV.arrayGet(1, &oV1)->isNum())
                        dGaps = oV1.getNum();
                    oV1.free();
                }
                oV.free();
            }
        }
        oBorder.free();
        if (nBorderType != 5)
        {
            nFlags |= (1 << 4);
            oRes.AddInt(nBorderType);
            oRes.AddDouble(dBorderWidth);
            if (nBorderType == annotBorderDashed)
            {
                oRes.AddDouble(dDashesAlternating);
                oRes.AddDouble(dGaps);
            }
        }

        Object oMK;
        if (pField->fieldLookup("MK", &oMK)->isDict())
        {
            // 6 - Цвет границ - BC. Даже если граница не задана BS/Border, то при наличии BC предоставляется граница по-умолчанию (сплошная, толщиной 1)
            if (oMK.dictLookup("BC", &oObj)->isArray())
            {
                nFlags |= (1 << 5);
                int nBCLength = oObj.arrayGetLength();
                oRes.AddInt(nBCLength);
                for (int j = 0; j < nBCLength; ++j)
                {
                    Object oBCj;
                    oRes.AddDouble(oObj.arrayGet(j, &oBCj)->isNum() ? oBCj.getNum() : 0.0);
                    oBCj.free();
                }
            }
            oObj.free();

            // 7 - Поворот аннотации относительно страницы - R
            if (oMK.dictLookup("R", &oObj)->isInt())
            {
                nFlags |= (1 << 6);
                oRes.AddInt(oObj.getInt());
            }
            oObj.free();

            // 8 - Цвет фона - BG
            if (oMK.dictLookup("BG", &oObj)->isArray())
            {
                nFlags |= (1 << 7);
                int nBGLength = oObj.arrayGetLength();
                oRes.AddInt(nBGLength);
                for (int j = 0; j < nBGLength; ++j)
                {
                    Object oBGj;
                    oRes.AddDouble(oObj.arrayGet(j, &oBGj)->isNum() ? oBGj.getNum() : 0.0);
                    oBGj.free();
                }
            }
            oObj.free();
        }
        oMK.free();

        // 9 - Значение по-умолчанию
        DICT_LOOKUP_STRING(pField->fieldLookup, "DV", 8);

        // Значение поля - V
        int nValueLength;
        Unicode* pValue = pField->getValue(&nValueLength);
        std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(pValue, nValueLength);
        gfree(pValue);

        // Запись данных необходимых каждому типу
        switch (oType)
        {
        case acroFormFieldPushbutton:
        case acroFormFieldRadioButton:
        case acroFormFieldCheckbox:
        {
            if (pField->fieldLookup("AS", &oObj)->isName())
                sValue = oObj.getName();
            oObj.free();

            // 10 - Включено
            if (sValue != "Off")
                nFlags |= (1 << 9);

            // Если Checkbox/RadioButton наследует Opt, то состояние соответствует порядковому в массиве Kids из Opt
            /*
            int nDepth = 0;
            Object oParentObj;
            oField.dictLookup("Parent", &oParentObj);
            while (oParentObj.isDict() && nDepth < 50)
            {
                Object oOpt, oKids;
                if (oParentObj.dictLookup("Opt", &oOpt) && oOpt.isArray() && oParentObj.dictLookup("Kids", &oKids) && oKids.isArray() && oOpt.arrayGetLength() == oKids.arrayGetLength())
                {
                    for (int j = 0, nLength = oOpt.arrayGetLength(); j < nLength; ++j)
                    {
                        Object oOptJ, oOptI;
                        if (oKids.arrayGetNF(j, &oOptJ) && oOptJ.isRef() && oOptJ.getRefGen() == oFieldRef.getRefGen() && oOptJ.getRefNum() == oFieldRef.getRefNum() &&
                                oOpt.arrayGet(j, &oOptI) && oOptI.isString())
                        {
                            TextString* s = new TextString(oOptI.getString());
                            sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                            if (sValue != "Off")
                                nFlags |= (1 << 9);
                            delete s;
                        }
                        oOptJ.free(); oOptI.free();
                    }
                }
                oOpt.free(); oKids.free();

                Object oParentObj2;
                oParentObj.dictLookup("Parent", &oParentObj2);
                oParentObj.free();
                oParentObj = oParentObj2;
                ++nDepth;
            }
            oParentObj.free();
            */

            int nIFPos = oRes.GetSize();
            unsigned int nIFFlag = 0;
            oRes.AddInt(nIFFlag);

            Object oMK;
            if (pField->fieldLookup("MK", &oMK)->isDict())
            {
                // 11 - Заголовок - СА
                // 12 - Заголовок прокрутки - RC
                // 13 - Альтернативный заголовок - AC
                if (oType == acroFormFieldPushbutton)
                {
                    DICT_LOOKUP_STRING(oMK.dictLookup, "CA", 10);
                    DICT_LOOKUP_STRING(oMK.dictLookup, "RC", 11);
                    DICT_LOOKUP_STRING(oMK.dictLookup, "AC", 12);
                }
                else
                {
                    unsigned int nStyle = (oType == acroFormFieldRadioButton ? 0 : 1);
                    if (oMK.dictLookup("CA", &oObj)->isString())
                    {
                        std::string sCA(oObj.getString()->getCString());

                        if (sCA == "l")
                            nStyle = 0;
                        else if (sCA == "4")
                            nStyle = 1;
                        else if (sCA == "8")
                            nStyle = 2;
                        else if (sCA == "u")
                            nStyle = 3;
                        else if (sCA == "n")
                            nStyle = 4;
                        else if (sCA == "H")
                            nStyle = 5;
                    }
                    oObj.free();
                    oRes.AddInt(nStyle);
                }

                // 14 - Положение заголовка - TP
                if (oMK.dictLookup("TP", &oObj)->isInt())
                {
                    nFlags |= (1 << 13);
                    oRes.AddInt(oObj.getInt());
                }
                oObj.free();

                Object oIF;
                if (oMK.dictLookup("IF", &oIF)->isDict())
                {
                    nIFFlag = 1;
                    // 2 - Масштабирование - SW
                    if (oIF.dictLookup("SW", &oObj)->isName())
                    {
                        nIFFlag |= (1 << 1);
                        std::string sName(oObj.getName());
                        oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                    }
                    oObj.free();
                    // 3 - Тип масштабирования - S
                    if (oIF.dictLookup("S", &oObj)->isName())
                    {
                        nIFFlag |= (1 << 2);
                        std::string sName(oObj.getName());
                        oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                    }
                    oObj.free();
                    // 4 - Смещение - A
                    if (oIF.dictLookup("A", &oObj)->isArray())
                    {
                        Object oObj1;
                        nIFFlag |= (1 << 3);
                        oRes.AddDouble(oObj.arrayGet(0, &oObj1)->isNum() ? oObj1.getNum() : 0.5);
                        oObj1.free();
                        oRes.AddDouble(oObj.arrayGet(1, &oObj1)->isNum() ? oObj1.getNum() : 0.5);
                        oObj1.free();
                    }
                    oObj.free();
                    // 5 - Полное соответствие - FB
                    if (oIF.dictLookup("FB", &oObj)->isBool() && oObj.getBool())
                        nIFFlag |= (1 << 4);
                    oObj.free();
                }
                oIF.free();
            }
            oMK.free();
            oRes.AddInt(nIFFlag, nIFPos);

            // 15 - Имя вкл состояния - AP - N - Yes
            Object oNorm;
            if (pField->fieldLookup("AP", &oObj)->isDict() && oObj.dictLookup("N", &oNorm)->isDict())
            {
                for (int j = 0, nNormLength = oNorm.dictGetLength(); j < nNormLength; ++j)
                {
                    std::string sNormName(oNorm.dictGetKey(j));
                    if (sNormName != "Off")
                    {
                        nFlags |= (1 << 14);
                        oRes.WriteString((BYTE*)sNormName.c_str(), (unsigned int)sNormName.length());
                        break;
                    }
                }
            }
            oNorm.free(); oObj.free();

            break;
        }
        case acroFormFieldFileSelect:
        case acroFormFieldMultilineText:
        case acroFormFieldText:
        case acroFormFieldBarcode:
        {
            // 10 - Значение
            // 11 - Максимальное количество символов
            // 12 - Расширенный текст RV

            if (!sValue.empty())
            {
                nFlags |= (1 << 9);
                oRes.WriteString((BYTE*)sValue.c_str(), (unsigned int)sValue.length());
            }

            // Максимальное количество символов в Tx - MaxLen
            int nMaxLen = pField->getMaxLen();
            if (nMaxLen > 0)
            {
                nFlags |= (1 << 10);
                oRes.AddInt(nMaxLen);
            }

            // RichText
            if (nFieldFlag & (1 << 25))
            {
                DICT_LOOKUP_STRING(pField->fieldLookup, "RV", 11);
            }

            break;
        }
        case acroFormFieldComboBox:
        case acroFormFieldListBox:
        {
            // 10 - Значение
            if (!sValue.empty())
            {
                nFlags |= (1 << 9);
                oRes.WriteString((BYTE*)sValue.c_str(), (unsigned int)sValue.length());
            }

            Object oOpt;
            // 11 - Список значений
            if (pField->fieldLookup("Opt", &oOpt)->isArray())
            {
                nFlags |= (1 << 10);
                int nOptLength = oOpt.arrayGetLength();
                oRes.AddInt(nOptLength);
                for (int j = 0; j < nOptLength; ++j)
                {
                    Object oOptJ;
                    if (!oOpt.arrayGet(j, &oOptJ) || !(oOptJ.isString() || oOptJ.isArray()))
                    {
                        oOptJ.free();
                        oRes.WriteString(NULL, 0);
                        oRes.WriteString(NULL, 0);
                        continue;
                    }

                    std::string sOpt1, sOpt2;
                    if (oOptJ.isArray() && oOptJ.arrayGetLength() > 1)
                    {
                        Object oOptJ2;
                        if (oOptJ.arrayGet(0, &oOptJ2)->isString())
                        {
                            TextString* s = new TextString(oOptJ2.getString());
                            sOpt1 = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                            delete s;
                        }
                        oOptJ2.free();
                        if (oOptJ.arrayGet(1, &oOptJ2)->isString())
                        {
                            TextString* s = new TextString(oOptJ2.getString());
                            sOpt2 = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                            delete s;
                        }
                        oOptJ2.free();
                    }
                    else if (oOptJ.isString())
                    {
                        TextString* s = new TextString(oOptJ.getString());
                        sOpt2 = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                        delete s;
                    }
                    oRes.WriteString((BYTE*)sOpt1.c_str(), (unsigned int)sOpt1.length());
                    oRes.WriteString((BYTE*)sOpt2.c_str(), (unsigned int)sOpt2.length());
                    oOptJ.free();
                }
            }
            oOpt.free();

            // 12 - Индекс верхнего элемента - TI
            if (pField->fieldLookup("TI", &oObj)->isInt())
            {
                nFlags |= (1 << 11);
                oRes.AddInt(oObj.getInt());
            }
            oObj.free();

            break;
        }
        case acroFormFieldSignature:
        {
            int nSigFlag = 0;
            if (pField->fieldLookup("SigFlags", &oObj)->isInt())
                nSigFlag = oObj.getInt();
            oObj.free();
            oRes.AddInt(nSigFlag);

            Object oObj1;
            // 10 - Action Lock
            // 11 - Fields
            if (pField->fieldLookup("Lock", &oObj)->isDict() && oObj.dictLookup("Type", &oObj1)->isName("SigFieldLock"))
            {
                oObj1.free();
                if (oObj.dictLookup("Action", &oObj1)->isName())
                {
                    nFlags |= (1 << 9);
                    std::string sName(oObj1.getName());
                    oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                }
                oObj1.free();

                if (oObj.dictLookup("Fields", &oObj1)->isArray())
                {
                    nFlags |= (1 << 10);
                    int nFields = oObj1.arrayGetLength();
                    oRes.AddInt(nFields);
                    for (int j = 0; j < nFields; ++j)
                    {
                        Object oObj2;
                        if (oObj1.arrayGet(j, &oObj2)->isString())
                        {
                            TextString* s = new TextString(oObj2.getString());
                            std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                            oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
                            delete s;
                        }
                        else
                            oRes.WriteString(NULL, 0);
                        oObj2.free();
                    }
                }
                oObj1.free();
            }
            oObj.free(); oObj1.free();

            if (pField->fieldLookup("SV", &oObj)->isDict() && oObj.dictLookup("Type", &oObj1)->isName("SV"))
            {
                oObj1.free();
                int nSVFlag = 0;
                int nSVPos = oRes.GetSize();
                oRes.AddInt(nSVFlag);

                // 1 - Ff
                if (oObj.dictLookup("Ff", &oObj1)->isInt())
                {
                    nSVFlag |= (1 << 0);
                    oRes.AddInt(oObj1.getInt());
                }
                oObj1.free();
                // 2 - Filter
                if (oObj.dictLookup("Filter", &oObj1)->isName())
                {
                    nSVFlag |= (1 << 1);
                    std::string sName(oObj1.getName());
                    oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                }
                oObj1.free();
                // 3 - SubFilter
                if (oObj.dictLookup("SubFilter", &oObj1)->isArray())
                {
                    nSVFlag |= (1 << 2);
                    int nFields = oObj1.arrayGetLength();
                    oRes.AddInt(nFields);
                    for (int j = 0; j < nFields; ++j)
                    {
                        Object oObj2;
                        if (oObj1.arrayGet(j, &oObj2)->isName())
                        {
                            std::string sName(oObj2.getName());
                            oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                        }
                        else
                            oRes.WriteString(NULL, 0);
                        oObj2.free();
                    }
                }
                oObj1.free();
                // 4 - DigestMethod
                if (oObj.dictLookup("DigestMethod", &oObj1)->isArray())
                {
                    nSVFlag |= (1 << 3);
                    int nFields = oObj1.arrayGetLength();
                    oRes.AddInt(nFields);
                    for (int j = 0; j < nFields; ++j)
                    {
                        Object oObj2;
                        if (oObj1.arrayGet(j, &oObj2)->isName())
                        {
                            std::string sName(oObj2.getName());
                            oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                        }
                        else
                            oRes.WriteString(NULL, 0);
                        oObj2.free();
                    }
                }
                oObj1.free();
                // 5 - V
                if (oObj.dictLookup("V", &oObj1)->isNum())
                {
                    nSVFlag |= (1 << 4);
                    oRes.AddDouble(oObj1.getNum());
                }
                oObj1.free();
                // 6 - Cert
                Object oObj2;
                if (oObj.dictLookup("Cert", &oObj1)->isDict() && oObj1.dictLookup("Type", &oObj2)->isName("SVCert"))
                {
                    nSVFlag |= (1 << 5);
                    oObj2.free();

                    // 12 - Ff
                    if (oObj1.dictLookup("Ff", &oObj2)->isInt())
                    {
                        nSVFlag |= (1 << 11);
                        oRes.AddInt(oObj2.getInt());
                    }
                    oObj2.free();
                    // 13 - Subject
                    if (oObj1.dictLookup("Subject", &oObj2)->isArray())
                    {
                        nSVFlag |= (1 << 12);
                        int nFields = oObj2.arrayGetLength();
                        oRes.AddInt(nFields);
                        for (int j = 0; j < nFields; ++j)
                        {
                            Object oObj3;
                            if (oObj2.arrayGet(j, &oObj3)->isString())
                            {
                                TextString* s = new TextString(oObj3.getString());
                                std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                                oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
                                delete s;
                            }
                            else
                                oRes.WriteString(NULL, 0);
                            oObj3.free();
                        }
                    }
                    oObj2.free();
                    // 14 - SubjectDN TODO
                    // 15 - KeyUsage TODO
                    // 16 - Issuer
                    if (oObj1.dictLookup("Issuer", &oObj2)->isArray())
                    {
                        nSVFlag |= (1 << 15);
                        int nFields = oObj2.arrayGetLength();
                        oRes.AddInt(nFields);
                        for (int j = 0; j < nFields; ++j)
                        {
                            Object oObj3;
                            if (oObj2.arrayGet(j, &oObj3)->isString())
                            {
                                TextString* s = new TextString(oObj3.getString());
                                std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                                oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
                                delete s;
                            }
                            else
                                oRes.WriteString(NULL, 0);
                            oObj3.free();
                        }
                    }
                    oObj2.free();
                    // 17 - OID
                    if (oObj1.dictLookup("OID", &oObj2)->isArray())
                    {
                        nSVFlag |= (1 << 16);
                        int nFields = oObj2.arrayGetLength();
                        oRes.AddInt(nFields);
                        for (int j = 0; j < nFields; ++j)
                        {
                            Object oObj3;
                            if (oObj2.arrayGet(j, &oObj3)->isString())
                            {
                                TextString* s = new TextString(oObj3.getString());
                                std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                                oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
                                delete s;
                            }
                            else
                                oRes.WriteString(NULL, 0);
                            oObj3.free();
                        }
                    }
                    oObj2.free();
                    // 18 - URL
                    if (oObj1.dictLookup("URL", &oObj2)->isString())
                    {
                        nSVFlag |= (1 << 17);
                        TextString* s = new TextString(oObj2.getString());
                        std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                        oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());
                        delete s;
                    }
                    oObj2.free();
                    // 19 - URLType
                    if (oObj1.dictLookup("URLType", &oObj2)->isName())
                    {
                        nSVFlag |= (1 << 1);
                        std::string sName(oObj2.getName());
                        oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                    }
                    oObj2.free();
                }
                oObj1.free(); oObj2.free();
                // 7 - Reasons
                if (oObj.dictLookup("Reasons", &oObj1)->isArray())
                {
                    nSVFlag |= (1 << 6);
                    int nFields = oObj1.arrayGetLength();
                    oRes.AddInt(nFields);
                    for (int j = 0; j < nFields; ++j)
                    {
                        Object oObj2;
                        if (oObj1.arrayGet(j, &oObj2)->isName())
                        {
                            std::string sName(oObj2.getName());
                            oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                        }
                        else
                            oRes.WriteString(NULL, 0);
                        oObj2.free();
                    }
                }
                oObj1.free();
                // 8 - MDP
                if (oObj.dictLookup("MDP", &oObj1)->isDict())
                {
                    nSVFlag |= (1 << 7);
                    // TODO
                }
                oObj1.free();
                // 9 - TimeStamp
                if (oObj.dictLookup("TimeStamp", &oObj1)->isDict())
                {
                    nSVFlag |= (1 << 8);
                    // TODO
                }
                oObj1.free();
                // 10 - LegalAttestation
                if (oObj.dictLookup("LegalAttestation", &oObj1)->isArray())
                {
                    nSVFlag |= (1 << 9);
                    int nFields = oObj1.arrayGetLength();
                    oRes.AddInt(nFields);
                    for (int j = 0; j < nFields; ++j)
                    {
                        Object oObj2;
                        if (oObj1.arrayGet(j, &oObj2)->isName())
                        {
                            std::string sName(oObj2.getName());
                            oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
                        }
                        else
                            oRes.WriteString(NULL, 0);
                        oObj2.free();
                    }
                }
                oObj1.free();
                // 11 - AddRevInfo
                if (oObj.dictLookup("TimeStamp", &oObj1)->isBool() && oObj1.getBool())
                {
                    nSVFlag |= (1 << 10);
                }
                oObj1.free();

                oRes.AddInt(nSVFlag, nSVPos);
            }
            oObj.free(); oObj1.free();

            break;
        }
        default:
            break;
        }
        oFieldRef.free(); oField.free();

        // 16 - Альтернативный текст - Contents
        DICT_LOOKUP_STRING(pField->fieldLookup, "Contents", 15);

        // 17 - Специальный цвет для аннотации - C
        if (pField->fieldLookup("C", &oObj)->isArray())
        {
            nFlags |= (1 << 16);
            int nCLength = oObj.arrayGetLength();
            oRes.AddInt(nCLength);
            for (int j = 0; j < nCLength; ++j)
            {
                Object oCj;
                oRes.AddDouble(oObj.arrayGet(j, &oCj)->isNum() ? oCj.getNum() : 0.0);
                oCj.free();
            }
        }
        oObj.free();

        int nActionPos = oRes.GetSize();
        unsigned int nActionLength = 0;
        oRes.AddInt(nActionLength);

        // Action - A
        Object oAction;
        if (pField->fieldLookup("A", &oAction)->isDict())
        {
            nActionLength++;

            std::string sAA = "A";
            oRes.WriteString((BYTE*)sAA.c_str(), (unsigned int)sAA.length());
            getAction(m_pPDFDocument, oRes, &oAction, i);
        }
        oAction.free();

        // Actions - AA
        Object oAA;
        if (pField->fieldLookup("AA", &oAA)->isDict())
        {
            int nLength = oAA.dictGetLength();
            nActionLength += nLength;
            for (int j = 0; j < nLength; ++j)
            {
                std::string sAA(oAA.dictGetKey(j));
                oRes.WriteString((BYTE*)sAA.c_str(), (unsigned int)sAA.length());

                if (!oAA.dictGetVal(j, &oAction)->isDict())
                {
                    oRes.WriteString(NULL, 0);
                    oAction.free();
                    continue;
                }

                getAction(m_pPDFDocument, oRes, &oAction, i);
                oAction.free();
            }
        }
        oAA.free();

        oRes.AddInt(nActionLength, nActionPos);
        oRes.AddInt(nFlags, nFlagPos);
    }

    oRes.WriteLen();
    BYTE* bRes = oRes.GetBuffer();
    oRes.ClearWithoutAttack();
    return bRes;
}
void WriteAppearance(int nWidth, int nHeight, BYTE* pBgraData, unsigned int nColor, NSWasm::CData& oRes)
{
    BYTE* pSubMatrix = new BYTE[nWidth * nHeight * 4];
    int p = 0;
    unsigned int* pTemp = (unsigned int*)pBgraData;
    unsigned int* pSubTemp = (unsigned int*)pSubMatrix;
    for (int y = 0; y < nHeight; ++y)
    {
        for (int x = 0; x < nWidth; ++x)
        {
            pSubTemp[p++] = pTemp[y * nWidth + x];
            pTemp[y * nWidth + x] = 0xFF000000 | nColor;
        }
    }

    unsigned long long npSubMatrix = (unsigned long long)pSubMatrix;
    unsigned int npSubMatrix1 = npSubMatrix & 0xFFFFFFFF;
    oRes.AddInt(npSubMatrix1);
    oRes.AddInt(npSubMatrix >> 32);

    BYTE* pTextFormField = ((GlobalParamsAdaptor*)globalParams)->GetTextFormField();
    BYTE* x = pTextFormField;
    unsigned int nLength = x ? (x[0] | x[1] << 8 | x[2] << 16 | x[3] << 24) : 4;
    nLength -= 4;
    oRes.Write(pTextFormField + 4, nLength);
    RELEASEARRAYOBJECTS(pTextFormField);
}
void DrawAppearance(PDFDoc* pdfDoc, int nPage, AcroFormField* pField, Gfx* gfx, const char* sAPName, const char* sASName)
{
    XRef* xref = pdfDoc->getXRef();
    Object oFieldRef, oField;
    pField->getFieldRef(&oFieldRef);
    oFieldRef.fetch(xref, &oField);

    Object kidsObj, annotRef, annotObj;
    if (oField.dictLookup("Kids", &kidsObj)->isArray())
    {
        for (int j = 0; j < kidsObj.arrayGetLength(); ++j)
        {
            kidsObj.arrayGetNF(j, &annotRef);
            annotRef.fetch(xref, &annotObj);
            pField->drawAnnot(nPage, gfx, gFalse, &annotRef, &annotObj, sAPName, sASName, gFalse);
            annotObj.free();
            annotRef.free();
        }
    }
    else
        pField->drawAnnot(nPage, gfx, gFalse, &oFieldRef, &oField, sAPName, sASName, gFalse);
    kidsObj.free();

    oFieldRef.free(); oField.free();
}
BYTE* CPdfReader::GetAPWidget(int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nWidget, const char* sView, const char* sButtonView)
{
    if (!m_pPDFDocument || !m_pPDFDocument->getCatalog())
        return NULL;

    AcroForm* pAcroForms = m_pPDFDocument->getCatalog()->getForm();
    Page* pPage = m_pPDFDocument->getCatalog()->getPage(nPageIndex + 1);
    if (!pAcroForms || !pPage)
        return NULL;

    double dPageDpiX, dPageDpiY;
    double dWidth, dHeight;
    GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

    NSWasm::CData oRes;
    oRes.SkipLen();

    for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
    {
        AcroFormField* pField = pAcroForms->getField(i);
        if (pField->getPageNum() != nPageIndex + 1 || (nWidget >= 0 && i != nWidget))
            continue;

        // Номер аннотации для сопоставления с AP
        oRes.AddInt(i);

        // Координаты - BBox
        double dx1, dy1, dx2, dy2;
        pField->getBBox(&dx1, &dy1, &dx2, &dy2);

        int nWidth  = (int)round((dx2 - dx1) * (double)nRasterW / dWidth);
        int nHeight = (int)round((dy2 - dy1) * (double)nRasterH / dHeight);

        double dWTale = nWidth  - (dx2 - dx1) * (double)nRasterW / dWidth;
        double dHTale = nHeight - (dy2 - dy1) * (double)nRasterH / dHeight;

        nWidth  += 2;
        nHeight += 2;

        BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
        unsigned int nColor = (unsigned int)nBackgroundColor;
        unsigned int nSize = (unsigned int)(nWidth * nHeight);
        unsigned int* pTemp = (unsigned int*)pBgraData;
        for (unsigned int i = 0; i < nSize; ++i)
            *pTemp++ = 0xFF000000 | nColor;

        CBgraFrame* pFrame = new CBgraFrame();
        pFrame->put_Data(pBgraData);
        pFrame->put_Width(nWidth);
        pFrame->put_Height(nHeight);
        pFrame->put_Stride(4 * nWidth);

        NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
        pRenderer->SetFontManager(m_pFontManager);
        pRenderer->CreateFromBgraFrame(pFrame);
        pRenderer->SetSwapRGB(true);
        pRenderer->put_Width ((dx2 - dx1 + (2 + dWTale) * dWidth / (double)nRasterW) * 25.4 / dPageDpiX);
        pRenderer->put_Height((dy2 - dy1 + (2 + dHTale) * dWidth / (double)nRasterW) * 25.4 / dPageDpiX);
        if (nBackgroundColor != 0xFFFFFF)
            pRenderer->CommandLong(c_nDarkMode, 1);

        PdfReader::RendererOutputDev oRendererOut(pRenderer, m_pFontManager, m_pFontList);
        oRendererOut.NewPDF(m_pPDFDocument->getXRef());

        // Создание Gfx
        GBool crop = gTrue;
        PDFRectangle box;
        int rotate = pPage->getRotate();
        if (rotate >= 360)
            rotate -= 360;
        else if (rotate < 0)
            rotate += 360;
        pPage->makeBox(72.0, 72.0, rotate, gFalse, oRendererOut.upsideDown(), -1, -1, -1, -1, &box, &crop);
        PDFRectangle* cropBox = pPage->getCropBox();

        Gfx* gfx = new Gfx(m_pPDFDocument, &oRendererOut, nPageIndex + 1, pPage->getAttrs()->getResourceDict(), 72.0, 72.0, &box, crop ? cropBox : (PDFRectangle *)NULL, rotate, NULL, NULL);

        // Координаты и размеры внешнего вида
        int nRx1 = (int)round(dx1 * (double)nRasterW / dWidth);
        int nRy2 = (int)round(dy2 * (double)nRasterH / dHeight);
        oRes.AddInt(nRx1);
        oRes.AddInt(nRasterH - nRy2);
        oRes.AddInt(nWidth);
        oRes.AddInt(nHeight);

        int nAPPos = oRes.GetSize();
        unsigned int nAPLength = 0;
        oRes.AddInt(nAPLength);

        // Отрисовка внешних видов аннотации
        Object oAP;
        AcroFormFieldType oType = pField->getAcroFormFieldType();
        ((GlobalParamsAdaptor*)globalParams)->setDrawFormField(true);

        if (pField->fieldLookup("AP", &oAP)->isDict())
        {
            std::vector<const char*> arrAPName { "N", "D", "R" };
            for (unsigned int j = 0; j < arrAPName.size(); ++j)
            {
                if (sView && strcmp(sView, arrAPName[j]) != 0)
                    continue;
                std::string sAPName(arrAPName[j]);
                Object oObj;
                if (oAP.dictLookup(sAPName.c_str(), &oObj)->isDict())
                {
                    for (int k = 0; k < oObj.dictGetLength(); ++k)
                    {
                        if (sButtonView)
                        {
                            if (strcmp(sButtonView, "Off") == 0 && strcmp(oObj.dictGetKey(k), "Off") != 0)
                                continue;
                            if (strcmp(sButtonView, "Yes") == 0 && strcmp(oObj.dictGetKey(k), "Off") == 0)
                                continue;
                        }
                        std::string sASName(oObj.dictGetKey(k));
                        oRes.WriteString((BYTE*)sAPName.c_str(), sAPName.length());
                        if ((oType == acroFormFieldRadioButton || oType == acroFormFieldCheckbox) && sASName != "Off")
                        {
                            std::string sYes = "Yes";
                            oRes.WriteString((BYTE*)sYes.c_str(), sYes.length());
                        }
                        else
                            oRes.WriteString((BYTE*)sASName.c_str(), sASName.length());
                        pRenderer->SetCoordTransformOffset(-dx1 * (double)nRasterW / dWidth + 1 + dWTale / 2, dy2 * (double)nRasterH / dHeight - nRasterH + 1 + dHTale / 2);
                        DrawAppearance(m_pPDFDocument, nPageIndex + 1, pField, gfx, sAPName.c_str(), sASName.c_str());
                        nAPLength++;
                        WriteAppearance(nWidth, nHeight, pBgraData, nColor, oRes);
                    }
                }
                else if (!oObj.isNull())
                {
                    oRes.WriteString((BYTE*)sAPName.c_str(), sAPName.length());
                    oRes.WriteString(NULL, 0);
                    pRenderer->SetCoordTransformOffset(-dx1 * (double)nRasterW / dWidth + 1 + dWTale / 2, dy2 * (double)nRasterH / dHeight - nRasterH + 1 + dHTale / 2);
                    DrawAppearance(m_pPDFDocument, nPageIndex + 1, pField, gfx, sAPName.c_str(), NULL);
                    nAPLength++;
                    WriteAppearance(nWidth, nHeight, pBgraData, nColor, oRes);
                }
                oObj.free();
            }
        }
        oAP.free();
        ((GlobalParamsAdaptor*)globalParams)->setDrawFormField(false);
        oRes.AddInt(nAPLength, nAPPos);

        delete gfx;
        RELEASEOBJECT(pFrame);
        RELEASEOBJECT(pRenderer);
    }


    oRes.WriteLen();
    BYTE* bRes = oRes.GetBuffer();
    oRes.ClearWithoutAttack();
    return bRes;
}
BYTE* CPdfReader::GetButtonIcon(int nRasterW, int nRasterH, int nBackgroundColor, int nPageIndex, int nButtonWidget, const char* sIconView)
{
    if (!m_pPDFDocument || !m_pPDFDocument->getCatalog())
        return NULL;

    AcroForm* pAcroForms = m_pPDFDocument->getCatalog()->getForm();
    Page* pPage = m_pPDFDocument->getCatalog()->getPage(nPageIndex + 1);
    if (!pAcroForms || !pPage)
        return NULL;

    double dPageDpiX, dPageDpiY;
    double dWidth, dHeight;
    GetPageInfo(nPageIndex, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

    NSWasm::CData oRes;
    oRes.SkipLen();

    for (int i = 0, nNum = pAcroForms->getNumFields(); i < nNum; ++i)
    {
        AcroFormField* pField = pAcroForms->getField(i);
        AcroFormFieldType oType = pField->getAcroFormFieldType();
        if (pField->getPageNum() != nPageIndex + 1 || oType != acroFormFieldPushbutton || (nButtonWidget >= 0 && i != nButtonWidget))
            continue;

        // Номер аннотации для сопоставления с AP
        oRes.AddInt(i);

        int nMKPos = oRes.GetSize();
        unsigned int nMKLength = 0;
        oRes.AddInt(nMKLength);
        Object oMK;
        if (pField->fieldLookup("MK", &oMK)->isDict())
        {
            std::vector<const char*> arrMKName { "I", "RI", "IX" };
            for (unsigned int j = 0; j < arrMKName.size(); ++j)
            {
                if (sIconView && strcmp(sIconView, arrMKName[j]) != 0)
                    continue;
                std::string sMKName(arrMKName[j]);
                Object oStr;
                if (!oMK.dictLookup(sMKName.c_str(), &oStr)->isStream())
                {
                    oStr.free();
                    continue;
                }
                oRes.WriteString((BYTE*)sMKName.c_str(), sMKName.size());

                Dict *dict = oStr.streamGetDict();

                // BBox
                Object bboxObj;
                double bbox[4];
                dict->lookup("BBox", &bboxObj);
                if (bboxObj.isArray())
                {
                    for (i = 0; i < 4; ++i)
                    {
                        Object obj1;
                        bboxObj.arrayGet(i, &obj1);
                        bbox[i] = obj1.getNum();
                        obj1.free();
                    }
                }
                else
                {
                    bbox[0] = 0; bbox[1] = 0;
                    bbox[2] = 0; bbox[3] = 0;
                }
                bboxObj.free();

                // Matrix
                double m[6];
                Object matrixObj;
                dict->lookup("Matrix", &matrixObj);
                if (false && matrixObj.isArray())
                {
                    for (i = 0; i < 6; ++i)
                    {
                        Object obj1;
                        matrixObj.arrayGet(i, &obj1);
                        m[i] = obj1.getNum();
                        obj1.free();
                    }
                }
                else
                {
                    m[0] = 1; m[1] = 0;
                    m[2] = 0; m[3] = 1;
                    m[4] = 0; m[5] = 0;
                }
                matrixObj.free();

                // Resources
                Object resObj;
                dict->lookup("Resources", &resObj);
                Dict *resDict = resObj.isDict() ? resObj.getDict() : (Dict *)NULL;

                int nWidth  = (bbox[2] > 0) ? (int)round(bbox[2] * (double)nRasterW / dWidth) : (int)((int)dWidth  * 96 / dPageDpiX);
                int nHeight = (bbox[3] > 0) ? (int)round(bbox[3] * (double)nRasterH / dHeight): (int)((int)dHeight * 96 / dPageDpiY);
                oRes.AddInt(nWidth);
                oRes.AddInt(nHeight);

                BYTE* pBgraData = new BYTE[nWidth * nHeight * 4];
                unsigned int nColor = (unsigned int)nBackgroundColor;
                unsigned int nSize = (unsigned int)(nWidth * nHeight);
                unsigned int* pTemp = (unsigned int*)pBgraData;
                for (unsigned int i = 0; i < nSize; ++i)
                    *pTemp++ = nColor;

                CBgraFrame* pFrame = new CBgraFrame();
                pFrame->put_Data(pBgraData);
                pFrame->put_Width(nWidth);
                pFrame->put_Height(nHeight);
                pFrame->put_Stride(4 * nWidth);

                NSGraphics::IGraphicsRenderer* pRenderer = NSGraphics::Create();
                pRenderer->SetFontManager(m_pFontManager);
                pRenderer->CreateFromBgraFrame(pFrame);
                pRenderer->SetSwapRGB(true);
                pRenderer->put_Width(bbox[2] * 25.4 / dPageDpiX);
                pRenderer->put_Height(bbox[3] * 25.4 / dPageDpiX);
                if (nBackgroundColor != 0xFFFFFF)
                    pRenderer->CommandLong(c_nDarkMode, 1);

                PdfReader::RendererOutputDev oRendererOut(pRenderer, m_pFontManager, m_pFontList);
                oRendererOut.NewPDF(m_pPDFDocument->getXRef());

                // Создание Gfx
                GBool crop = gTrue;
                PDFRectangle box;
                int rotate = pPage->getRotate();
                if (rotate >= 360)
                    rotate -= 360;
                else if (rotate < 0)
                    rotate += 360;
                pPage->makeBox(72.0, 72.0, rotate, gFalse, oRendererOut.upsideDown(), -1, -1, -1, -1, &box, &crop);
                PDFRectangle* cropBox = pPage->getCropBox();

                Gfx* gfx = new Gfx(m_pPDFDocument, &oRendererOut, nPageIndex + 1, pPage->getAttrs()->getResourceDict(), 72.0, 72.0, &box, cropBox ? cropBox : (PDFRectangle *)NULL, rotate, NULL, NULL);

                Object oStrRef;
                oMK.dictLookupNF(sMKName.c_str(), &oStrRef);
                pRenderer->SetCoordTransformOffset(0, bbox[3] * (double)nRasterH / dHeight - nRasterH);
                gfx->drawForm(&oStrRef, resDict, m, bbox, gFalse, gFalse, gFalse, gFalse);
                oStr.free(); oStrRef.free(); resObj.free();

                nMKLength++;
                unsigned long long npSubMatrix = (unsigned long long)pBgraData;
                unsigned int npSubMatrix1 = npSubMatrix & 0xFFFFFFFF;
                oRes.AddInt(npSubMatrix1);
                oRes.AddInt(npSubMatrix >> 32);

                delete gfx;
                pFrame->ClearNoAttack();
                RELEASEOBJECT(pFrame);
                RELEASEOBJECT(pRenderer);
            }
        }
        oMK.free();
        oRes.AddInt(nMKLength, nMKPos);
    }

    oRes.WriteLen();
    BYTE* bRes = oRes.GetBuffer();
    oRes.ClearWithoutAttack();
    return bRes;
}
