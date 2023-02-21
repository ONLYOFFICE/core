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

    XRef* xref = m_pPDFDocument->getXRef();
    if (!xref)
        return false;

    for (int nNum = 0, nCount = xref->getSize(); nNum < nCount; ++nNum)
    {
        XRefEntry *pEntry = xref->getEntry(nNum);
        if (!pEntry || xrefEntryFree == pEntry->type)
            continue;

        Object oTemp;
        if (!xref->fetch(nNum, pEntry->gen, &oTemp))
            continue;

        if (!oTemp.isDict())
        {
            oTemp.free();
            continue;
        }

        Object oFont;
        if (!oTemp.dictLookup("Type", &oFont) || !oFont.isName() || strcmp(oFont.getName(), "Font"))
        {
            oTemp.free();
            oFont.free();
            continue;
        }
        oFont.free();

        Object oEncoding;
        if (!oTemp.dictLookup("Encoding", &oEncoding) || !oEncoding.isName())
        {
            oTemp.free();
            oEncoding.free();
            continue;
        }
        oTemp.free();

        char* sName = oEncoding.getName();
        if (std::find(arrCMap.begin(), arrCMap.end(), sName) != arrCMap.end())
        {
            oEncoding.free();
            return true;
        }
        oEncoding.free();
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
