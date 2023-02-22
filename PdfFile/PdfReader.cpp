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

CPdfReader::CPdfReader(NSFonts::IApplicationFonts* pAppFonts)
{
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
}\

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
}\

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

#define DICT_LOOKUP_STRING(field, sName, byte) \
{\
if (field.dictLookup(sName, &oObj) && oObj.isString())\
{\
    TextString* s = new TextString(oObj.getString());\
    std::string sStr = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());\
    nFlags |= (1 << byte);\
    oRes.WriteString((BYTE*)sStr.c_str(), (unsigned int)sStr.length());\
    delete s;\
}\
oObj.free();\
}

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
        if (!xref || !pField->getFieldRef(&oFieldRef) || !oFieldRef.isRef() || !oFieldRef.fetch(xref, &oField) || !oField.isDict())
        {
            oFieldRef.free(); oField.free();
            continue;
        }

        // Флаг аннотации - F
        Object oFlag;
        int nAnnotFlag = 0;
        if (oField.dictLookup("F", &oFlag) && oFlag.isInt())
            nAnnotFlag = oFlag.getInt();
        oFlag.free();
        oRes.AddInt(nAnnotFlag);

        // Полное имя поля - T (parent_full_name.child_name)
        int nLengthName;
        Unicode* uName = pField->getName(&nLengthName);
        std::string sName = NSStringExt::CConverter::GetUtf8FromUTF32(uName, nLengthName);
        oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
        gfree(uName);

        // Номер страницы - P
        int nPage = pField->getPageNum();
        oRes.AddInt(nPage - 1);

        // Координаты - BBox
        double dx1, dy1, dx2, dy2;
        pField->getBBox(&dx1, &dy1, &dx2, &dy2);
        double height = m_pPDFDocument->getPageCropHeight(nPage);
        oRes.AddDouble(dx1);
        oRes.AddDouble(height - dy1);
        oRes.AddDouble(dx2);
        oRes.AddDouble(height - dy2);

        // Выравнивание текста - Q
        Object oQ;
        pField->fieldLookup("Q", &oQ);
        int nQ = 0;
        if (oQ.isInt())
            nQ = oQ.getInt();
        oQ.free();
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

        // Цвет - оператор g/rg в DA
        // double r, g, b;
        // pField->getColor(&r, &g, &b);

        // Шрифт и размер шрифта - операторы Tf и Tm в DA
        // Ref pFontRef; double dFontSize;
        // pField->getFont(&pFontRef, &dFontSize);

        // Флаг - Ff
        unsigned int nFieldFlag = pField->getFlags();
        oRes.AddInt(nFieldFlag);

        int nFlags = 0;
        int nFlagPos = oRes.GetSize();
        oRes.AddInt(nFlags);

        // 1 - Альтернативное имя поля, используется во всплывающей подсказке и сообщениях об ошибке - TU
        Object oObj;
        DICT_LOOKUP_STRING(oField, "TU", 0);

        // 2 - Строка стиля по умолчанию - DS
        DICT_LOOKUP_STRING(oField, "DS", 1);

        // Границы и Dash Pattern - Border/BS
        Object oBorder;
        AnnotBorderType nBorderType = annotBorderSolid;
        double dBorderWidth = 1, dDashesAlternating = 3, dGaps = 3;
        if (oField.dictLookup("BS", &oBorder) && oBorder.isDict())
        {
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
            if (oField.dictLookup("Border", &oBorder) && oBorder.isArray() && oBorder.arrayGetLength() > 2)
            {
                Object oV;
                if (oBorder.arrayGet(2, &oV) && oV.isNum())
                    dBorderWidth = oV.getNum();
                oV.free();

                if (oBorder.arrayGetLength() > 3 && oBorder.arrayGet(3, &oV) && oV.isArray() && oV.arrayGetLength() > 1)
                {
                    nBorderType = annotBorderDashed;
                    Object oV1;
                    if (oV.arrayGet(0, &oV1) && oV1.isNum())
                        dDashesAlternating = oV1.getNum();
                    oV1.free();
                    if (oV.arrayGet(1, &oV1) && oV1.isNum())
                        dGaps = oV1.getNum();
                    oV1.free();
                }
                oV.free();
            }
        }
        oBorder.free();
        oRes.AddInt(nBorderType);
        oRes.AddDouble(dBorderWidth);
        if (nBorderType == annotBorderDashed)
        {
            oRes.AddDouble(dDashesAlternating);
            oRes.AddDouble(dGaps);
        }

        // 3 - Эффекты границы - BE
        Object oBorderBE, oBorderBEI;
        if (oField.dictLookup("BE", &oBorder) && oBorder.isDict() && oBorder.dictLookup("S", &oBorderBE)->isName("C") && oBorder.dictLookup("I", &oBorderBEI)->isNum())
        {
            nFlags |= (1 << 2);
            oRes.AddDouble(oBorderBEI.getNum());
        }
        oBorder.free(); oBorderBE.free(); oBorderBEI.free();

        // 4 - Режим выделения - H
        Object oHighlighting;
        if (oField.dictLookup("H", &oHighlighting) && oBorder.isName())
        {
            nFlags |= (1 << 3);
            std::string sName(oBorder.getName());
            oRes.WriteString((BYTE*)sName.c_str(), (unsigned int)sName.length());
        }
        oHighlighting.free();

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
            if (oField.dictLookup("AS", &oObj) && oObj.isName())
                sValue = oObj.getName();
            oObj.free();

            // 10 - Включено
            if (sValue != "Off")
                nFlags |= (1 << 9);

            // Если Checkbox/RadioButton наследует Opt, то состояние соответствует порядковому в массиве Kids из Opt
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

            Object oMK;
            if (oField.dictLookup("MK", &oMK) && oMK.isDict())
            {
                // 11 - Заголовок - СА
                DICT_LOOKUP_STRING(oMK, "CA", 10);

                // 12 - Заголовок прокрутки - RC
                // 13 - Альтернативный заголовок - AC
                if (oType == acroFormFieldPushbutton)
                {
                    DICT_LOOKUP_STRING(oMK, "RC", 11);
                    DICT_LOOKUP_STRING(oMK, "AC", 12);
                }

                // 14 - Положение заголовка - TP
                if (oMK.dictLookup("TP", &oObj) && oObj.isInt())
                {
                    nFlags |= (1 << 13);
                    oRes.AddInt(oObj.getInt());
                }
                oObj.free();

                // TODO Обычный значок - I Значок прокрутки - RI Альтернативный значок - IX Значок соответствия - IF
            }
            oMK.free();

            // 15 - Имя вкл состояния - AP - N - Yes
            Object oNorm;
            if (oField.dictLookup("AP", &oObj) && oObj.isDict() && oObj.dictLookup("N", &oNorm) && oNorm.isDict())
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
                DICT_LOOKUP_STRING(oField, "RV", 11);
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
            if (!oField.dictLookup("Opt", &oOpt) || !oOpt.isArray())
            {
                int nDepth = 0;
                Object oParentObj;
                oField.dictLookup("Parent", &oParentObj);
                while (oParentObj.isDict() && nDepth < 50)
                {
                    if (oParentObj.dictLookup("Opt", &oOpt) && oOpt.isArray())
                        break;

                    Object oParentObj2;
                    oParentObj.dictLookup("Parent", &oParentObj2);
                    oParentObj.free();
                    oParentObj = oParentObj2;
                    ++nDepth;
                }
                oParentObj.free();
            }
            // 11 - Список значений
            if (oOpt.isArray())
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
                        if (oOptJ.arrayGet(0, &oOptJ2) && oOptJ2.isString())
                        {
                            TextString* s = new TextString(oOptJ2.getString());
                            sOpt1 = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                            delete s;
                        }
                        oOptJ2.free();
                        if (oOptJ.arrayGet(1, &oOptJ2) && oOptJ2.isString())
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

            break;
        }
        case acroFormFieldSignature:
        {
            break;
        }
        default:
            break;
        }
        oFieldRef.free(); oField.free();

        // 20 - Action - A
        Object oAA;
        if (oField.dictLookup("AA", &oAA) && oAA.isDict())
        {
            nFlags |= (1 << 19);
            int nLength = oAA.dictGetLength();
            oRes.AddInt(nLength);
            for (int j = 0; j < nLength; ++j)
            {
                std::string sAA(oAA.dictGetKey(j));
                oRes.WriteString((BYTE*)sAA.c_str(), sAA.length());

                Object oAction, oType;
                if (!oAA.dictGetVal(j, &oAction) || !oAction.isDict() || !oAction.dictLookup("S", &oType) || !oType.isName())
                {
                    oRes.WriteString(NULL, 0);
                    oAction.free();
                    oType.free();
                    continue;
                }

                std::string sName(oType.getName());
                oRes.WriteString((BYTE*)sName.c_str(), sName.length());
                oType.free();
                if (sName == "JavaScript")
                {
                    // 21 - JS
                    Object oJS;
                    if (oAction.dictLookup("JS", &oJS) && oJS.isString())
                    {
                        TextString* s = new TextString(oJS.getString());
                        std::string sJS = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
                        delete s;
                        nFlags |= (1 << 20);
                        oRes.WriteString((BYTE*)sJS.c_str(), sJS.length());
                    }
                    oJS.free();
                }
                oAction.free();
            }
        }
        oAA.free();

        oRes.AddInt(nFlags, nFlagPos);
    }

    oRes.WriteLen();
    BYTE* bRes = oRes.GetBuffer();
    oRes.ClearWithoutAttack();
    return bRes;
}
