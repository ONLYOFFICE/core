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
#include "PdfFile.h"
#include "PdfWriter.h"
#include "PdfReader.h"

#include "../DesktopEditor/common/File.h"
#include "lib/xpdf/PDFDoc.h"

#ifndef BUILDING_WASM_MODULE
#include "OnlineOfficeBinToPdf.h"
#include "../DesktopEditor/common/Path.h"
#include "../DesktopEditor/common/StringExt.h"

#include "SrcReader/Adaptors.h"
#include "lib/xpdf/AcroForm.h"
#include "lib/xpdf/TextString.h"

#include "SrcWriter/Objects.h"
#include "SrcWriter/Document.h"
#include "SrcWriter/Pages.h"
#include "SrcWriter/Catalog.h"
#include "SrcWriter/EncryptDictionary.h"
#include "SrcWriter/Info.h"

#define AddToObject(oVal)\
{\
    if (pObj->GetType() == PdfWriter::object_type_DICT)\
        ((PdfWriter::CDictObject*)pObj)->Add(sKey, oVal);\
    else if (pObj->GetType() == PdfWriter::object_type_ARRAY)\
        ((PdfWriter::CArrayObject*)pObj)->Add(oVal);\
}

void DictToCDictObject(Object* obj, PdfWriter::CObjectBase* pObj, bool bBinary, const std::string& sKey)
{
    Object oTemp;
    switch (obj->getType())
    {
    case objBool:
    {
        AddToObject(obj->getBool())
        break;
    }
    case objInt:
    {
        AddToObject(obj->getInt())
        break;
    }
    case objReal:
    {
        AddToObject(obj->getReal())
        break;
    }
    case objString:
    {
        if (bBinary)
        {
            GString* str = obj->getString();
            int nLength = str->getLength();
            BYTE* arrId = new BYTE[nLength];
            for (int nIndex = 0; nIndex < nLength; ++nIndex)
                arrId[nIndex] = str->getChar(nIndex);
            AddToObject(new PdfWriter::CBinaryObject(arrId, nLength));
            RELEASEARRAYOBJECTS(arrId);
        }
        else
        {
            TextString* s = new TextString(obj->getString());
            std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
            AddToObject(new PdfWriter::CStringObject(sValue.c_str()))
            delete s;
        }
        break;
    }
    case objName:
    {
        AddToObject(obj->getName())
        break;
    }
    case objNull:
    {
        AddToObject(new PdfWriter::CNullObject())
        break;
    }
    case objArray:
    {
        PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
        AddToObject(pArray)

        for (int nIndex = 0; nIndex < obj->arrayGetLength(); ++nIndex)
        {
            obj->arrayGetNF(nIndex, &oTemp);
            DictToCDictObject(&oTemp, pArray, bBinary, "");
            oTemp.free();
        }
        break;
    }
    case objDict:
    {
        PdfWriter::CDictObject* pDict = new PdfWriter::CDictObject();
        AddToObject(pDict);

        for (int nIndex = 0; nIndex < obj->dictGetLength(); ++nIndex)
        {
            char* chKey = obj->dictGetKey(nIndex);
            obj->dictGetValNF(nIndex, &oTemp);
            DictToCDictObject(&oTemp, pDict, bBinary, chKey);
            oTemp.free();
        }
        break;
    }
    case objRef:
    {
        PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
        pBase->SetRef(obj->getRefNum(), obj->getRefGen());
        AddToObject(new PdfWriter::CProxyObject(pBase, true))
        break;
    }
    case objNone:
    {
        AddToObject("None")
        break;
    }
    case objStream:
    case objCmd:
    case objError:
    case objEOF:
        break;
    }
}
#endif // BUILDING_WASM_MODULE

class CPdfFile_Private
{
public:
    std::wstring wsSrcFile;
    std::wstring wsPassword;
    std::wstring wsTempFolder;
    NSFonts::IApplicationFonts* pAppFonts;

    CPdfReader* pReader;

    CPdfWriter* pWriter;
    LONG lClipMode;
    bool bEdit;
    bool bEditPage;

#ifndef BUILDING_WASM_MODULE
    void GetPageTree(XRef* xref, Object* pPagesRefObj)
    {
        PdfWriter::CDocument* pDoc = pWriter->m_pDocument;
        if (!pPagesRefObj || !xref || !pDoc)
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

        PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, topPagesRef.num);
        if (!pXref)
        {
            pagesObj.free();
            return;
        }

        PdfWriter::CPageTree* pPageT = new PdfWriter::CPageTree(pXref);
        if (!pPageT)
        {
            pagesObj.free();
            RELEASEOBJECT(pXref);
            return;
        }

        for (int nIndex = 0; nIndex < pagesObj.dictGetLength(); ++nIndex)
        {
            Object oTemp;
            char* chKey = pagesObj.dictGetKey(nIndex);
            pagesObj.dictGetValNF(nIndex, &oTemp);
            DictToCDictObject(&oTemp, pPageT, false, chKey);
            oTemp.free();
        }

        pDoc->CreatePageTree(pXref, pPageT);
        pPageT->SetRef(topPagesRef.num, topPagesRef.gen);
        pPageT->Fix();

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
#endif
};

// ------------------------------------------------------------------------

CPdfFile::CPdfFile(NSFonts::IApplicationFonts* pAppFonts)
{
    m_pInternal = new CPdfFile_Private();

    m_pInternal->pAppFonts = pAppFonts;
    m_pInternal->pWriter = NULL;
    m_pInternal->pReader = NULL;
    m_pInternal->wsPassword = L"";
    m_pInternal->bEdit     = false;
    m_pInternal->bEditPage = false;
}
CPdfFile::~CPdfFile()
{
    RELEASEOBJECT(m_pInternal->pWriter);
    RELEASEOBJECT(m_pInternal->pReader);
}
NSFonts::IFontManager* CPdfFile::GetFontManager()
{
    if (!m_pInternal->pReader)
        return NULL;
    return m_pInternal->pReader->GetFontManager();
}

void CPdfFile::Close()
{
    if (!m_pInternal->bEdit)
    {
        if (m_pInternal->pReader)
            m_pInternal->pReader->Close();
        return;
    }
#ifndef BUILDING_WASM_MODULE
    if (!m_pInternal->pWriter || !m_pInternal->pReader)
        return;
    PDFDoc* pPDFDocument = m_pInternal->pReader->GetPDFDocument();
    PdfWriter::CDocument* pDoc = m_pInternal->pWriter->m_pDocument;
    if (!pPDFDocument || !pDoc)
        return;

    XRef* xref = pPDFDocument->getXRef();
    if (!xref)
        return;

    // Добавляем первый элемент в таблицу xref
    // он должен иметь вид 0000000000 65535 f
    PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, 0, 65535);
    if (!pXref)
        return;

    PdfWriter::CDictObject* pTrailer = NULL;
    Object* trailerDict = xref->getTrailerDict();
    if (trailerDict)
    {
        pTrailer = pXref->GetTrailer();

        for (int nIndex = 0; nIndex < trailerDict->dictGetLength(); ++nIndex)
        {
            Object oTemp;
            char* chKey = trailerDict->dictGetKey(nIndex);
            trailerDict->dictGetValNF(nIndex, &oTemp);
            DictToCDictObject(&oTemp, pTrailer, true, chKey);
            oTemp.free();
        }
    }

    Object info;
    pPDFDocument->getDocInfo(&info);
    PdfWriter::CXref* pInfoXref = NULL;
    PdfWriter::CInfoDict* pInfoDict = NULL;
    if (info.isDict())
    {
        // Обновление Info
        PdfWriter::CObjectBase* pInfo = pTrailer->Get("Info");
        pInfoXref = new PdfWriter::CXref(pDoc, pInfo ? pInfo->GetObjId() : 0);
        if (!pInfoXref)
        {
            RELEASEOBJECT(pXref);
            return;
        }
        pInfoDict = new PdfWriter::CInfoDict(pInfoXref);
        if (!pInfoDict)
        {
            RELEASEOBJECT(pXref);
            RELEASEOBJECT(pInfoXref);
            return;
        }

        for (int nIndex = 0; nIndex < info.dictGetLength(); ++nIndex)
        {
            Object oTemp;
            char* chKey = info.dictGetKey(nIndex);
            info.dictGetValNF(nIndex, &oTemp);
            DictToCDictObject(&oTemp, pInfoDict, true, chKey);
            oTemp.free();
        }

        if (pInfo)
            pInfoDict->SetRef(pInfo->GetObjId(), pInfo->GetGenNo());
        pInfoDict->SetTime(PdfWriter::InfoModaDate);
    }
    info.free();

    if (!m_pInternal->pWriter->EditClose() || !pDoc->AddToFile(pXref, pTrailer, pInfoXref, pInfoDict))
    {
        RELEASEOBJECT(pXref);
        return;
    }

    std::wstring wsPath = pDoc->GetEditPdfPath();
    std::string sPathUtf8New = U_TO_UTF8(wsPath);
    std::string sPathUtf8Old = U_TO_UTF8(m_pInternal->wsSrcFile);
    if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
    {
        GString* owner_pswd = NSStrings::CreateString(m_pInternal->wsPassword);
        GString* user_pswd  = NSStrings::CreateString(m_pInternal->wsPassword);
        pPDFDocument->makeWritable(false, owner_pswd, user_pswd);
        delete owner_pswd;
        delete user_pswd;

        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(m_pInternal->wsSrcFile))
        {
            m_pInternal->pReader->ChangeLength(oFile.GetFileSize());
            oFile.CloseFile();
        }
    }

    m_pInternal->bEdit     = false;
    m_pInternal->bEditPage = false;
#endif
}
void CPdfFile::Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate)
{
    if (!m_pInternal->pWriter)
        return;
    m_pInternal->pWriter->Sign(dX, dY, dW, dH, wsPicturePath, pCertificate);
}
void CPdfFile::RotatePage(int nRotate)
{
    if (!m_pInternal->pWriter)
        return;
    // Применение поворота страницы для writer
    m_pInternal->pWriter->PageRotate(nRotate);
}
#ifndef BUILDING_WASM_MODULE
bool CPdfFile::EditPdf(const std::wstring& wsDstFile)
{
    if (!m_pInternal->pReader)
        return false;
    // Создание writer для редактирования
    RELEASEOBJECT(m_pInternal->pWriter);
    m_pInternal->pWriter = new CPdfWriter(m_pInternal->pAppFonts);
    if (!wsDstFile.empty())
        NSFile::CFileBinary::Copy(m_pInternal->wsSrcFile, wsDstFile);

    PDFDoc* pPDFDocument = m_pInternal->pReader->GetPDFDocument();
    if (!pPDFDocument)
        return false;

    // Если результат редактирования будет сохранен в тот же файл, что открыт для чтения, то файл необходимо сделать редактируемым
    std::string sPathUtf8New = U_TO_UTF8(wsDstFile);
    std::string sPathUtf8Old = U_TO_UTF8(m_pInternal->wsSrcFile);
    if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
    {
        GString* owner_pswd = NSStrings::CreateString(m_pInternal->wsPassword);
        GString* user_pswd  = NSStrings::CreateString(m_pInternal->wsPassword);
        GBool bRes = pPDFDocument->makeWritable(true, owner_pswd, user_pswd);
        delete owner_pswd;
        delete user_pswd;
        if (!bRes)
            return false;
    }
    else
    {
        NSFile::CFileBinary oFile;
        if (!oFile.OpenFile(wsDstFile, true))
            return false;
        oFile.CloseFile();
    }

    XRef* xref = pPDFDocument->getXRef();
    PdfWriter::CDocument* pDoc = m_pInternal->pWriter->m_pDocument;
    if (!xref || !pDoc)
        return false;

    // Получение каталога и дерева страниц из reader
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
    catRefObj.free();

    // Создание каталога для writer
    PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, catRef.num);
    if (!pXref)
    {
        pagesRefObj.free();
        catDict.free();
        return false;
    }
    PdfWriter::CCatalog* pCatalog = new PdfWriter::CCatalog(pXref, true);
    if (!pCatalog)
    {
        pagesRefObj.free();
        catDict.free();
        RELEASEOBJECT(pXref);
        return false;
    }
    for (int nIndex = 0; nIndex < catDict.dictGetLength(); ++nIndex)
    {
        Object oTemp;
        char* chKey = catDict.dictGetKey(nIndex);
        catDict.dictGetValNF(nIndex, &oTemp);
        DictToCDictObject(&oTemp, pCatalog, false, chKey);
        oTemp.free();
    }
    pCatalog->SetRef(catRef.num, catRef.gen);
    catDict.free();

    // Проверка уникальности имён текущих цифровых подписей pdf
    unsigned int nFormField = 0;
    AcroForm* form = pPDFDocument->getCatalog()->getForm();
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

    // Получение шифрования из reader и применения для writer
    int nCryptAlgorithm = -1;
    PdfWriter::CEncryptDict* pEncryptDict = NULL;
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
            pEncryptDict = new PdfWriter::CEncryptDict();

            Object encrypt, ID, ID1;
            if (pTrailerDict->dictLookup("Encrypt", &encrypt) && encrypt.isDict())
            {
                for (int nIndex = 0; nIndex < encrypt.dictGetLength(); ++nIndex)
                {
                    Object oTemp;
                    char* chKey = encrypt.dictGetKey(nIndex);
                    encrypt.dictGetValNF(nIndex, &oTemp);
                    DictToCDictObject(&oTemp, pEncryptDict, true, chKey);
                    oTemp.free();
                }

                pEncryptDict->SetRef(0, 0);
                pEncryptDict->Fix();
            }
            encrypt.free();

            if (pTrailerDict->dictLookup("ID", &ID) && ID.isArray() && ID.arrayGet(0, &ID1) && ID1.isString())
            {
                for (int nIndex = 0; nIndex < ID1.dictGetLength(); ++nIndex)
                {
                    Object oTemp;
                    char* chKey = ID1.dictGetKey(nIndex);
                    ID1.dictGetValNF(nIndex, &oTemp);
                    DictToCDictObject(&oTemp, pEncryptDict, true, chKey);
                    oTemp.free();
                }
            }

            pEncryptDict->SetRef(0, 0);
            pEncryptDict->Fix();
            pEncryptDict->SetPasswords(m_pInternal->wsPassword, m_pInternal->wsPassword);
            pEncryptDict->UpdateKey(nCryptAlgorithm);

            ID.free();
            ID1.free();
        }
    }

    // Применение редактирования для writer
    bool bRes = pDoc->EditPdf(wsDstFile, xref->getLastXRefPos(), xref->getNumObjects(), pXref, pCatalog, pEncryptDict, nFormField);
    if (bRes)
    {
        // Воспроизведение дерева страниц во writer
        m_pInternal->GetPageTree(xref, &pagesRefObj);
        m_pInternal->bEdit = true;
    }
    pagesRefObj.free();
    return bRes;
}
bool CPdfFile::EditPage(int nPageIndex)
{
    // Проверка режима редактирования
    if (!m_pInternal->pWriter || !m_pInternal->pReader)
        return false;
    PDFDoc* pPDFDocument = m_pInternal->pReader->GetPDFDocument();
    PdfWriter::CDocument* pDoc = m_pInternal->pWriter->m_pDocument;
    if (!pPDFDocument || !pDoc || !m_pInternal->bEdit)
        return false;

    XRef* xref = pPDFDocument->getXRef();
    Catalog* pCatalog = pPDFDocument->getCatalog();
    if (!xref || !pCatalog)
        return false;
    std::pair<int, int> pPageRef = pDoc->GetPageRef(nPageIndex);
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
    pageRefObj.free();

    // Воспроизведение словаря страницы из reader для writer
    PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, pPageRef.first);
    if (!pXref)
    {
        pageObj.free();
        return false;
    }
    PdfWriter::CPage* pPage = new PdfWriter::CPage(pXref, pDoc);
    if (!pPage)
    {
        pageObj.free();
        RELEASEOBJECT(pXref);
        return false;
    }
    for (int nIndex = 0; nIndex < pageObj.dictGetLength(); ++nIndex)
    {
        Object oTemp;
        char* chKey = pageObj.dictGetKey(nIndex);
        if (strcmp("Resources", chKey) == 0 || strcmp("AcroForm", chKey) == 0)
            pageObj.dictGetVal(nIndex, &oTemp);
        else
            pageObj.dictGetValNF(nIndex, &oTemp);
        DictToCDictObject(&oTemp, pPage, true, chKey);
        oTemp.free();
    }
    pPage->SetRef(pPageRef.first, pPageRef.second);
    pPage->Fix();
    pageObj.free();

    // Применение редактирования страницы для writer
    m_pInternal->bEditPage = true;
    if (m_pInternal->pWriter->EditPage(pPage) && pDoc->EditPage(pXref, pPage))
        return true;

    RELEASEOBJECT(pXref);
    return false;
}
bool CPdfFile::DeletePage(int nPageIndex)
{
    // Проверка режима редактирования
    if (!m_pInternal->pWriter || !m_pInternal->pWriter->m_pDocument || !m_pInternal->bEdit)
        return false;
    // Применение удаления страницы для writer
    return m_pInternal->pWriter->m_pDocument->DeletePage(nPageIndex);
}
bool CPdfFile::AddPage(int nPageIndex)
{
    // Проверка режима редактирования
    if (!m_pInternal->pWriter || !m_pInternal->bEdit)
        return false;
    // Применение добавления страницы для writer
    bool bRes = m_pInternal->pWriter->AddPage(nPageIndex);
    // По умолчанию выставляются размеры первой страницы, в дальнейшем размеры можно изменить
    if (bRes)
    {
        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        m_pInternal->pReader->GetPageInfo(0, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        m_pInternal->pWriter->put_Width(dWidth);
        m_pInternal->pWriter->put_Height(dHeight);
    }
    return bRes;
}
#endif // BUILDING_WASM_MODULE

// ------------------------------------------------------------------------

int CPdfFile::GetError()
{
    if (!m_pInternal->pReader)
        return 1;
    return m_pInternal->pReader->GetError();
}
bool CPdfFile::IsNeedCMap()
{
    if (!m_pInternal->pReader)
        return false;
    return m_pInternal->pReader->IsNeedCMap();
}
void CPdfFile::SetCMapMemory(BYTE* pData, DWORD nSizeData)
{
    if (!m_pInternal->pReader)
        return;
    m_pInternal->pReader->SetCMapMemory(pData, nSizeData);
}
void CPdfFile::SetCMapFolder(const std::wstring& sFolder)
{
    if (!m_pInternal->pReader)
        return;
    m_pInternal->pReader->SetCMapFolder(sFolder);
}
void CPdfFile::SetCMapFile(const std::wstring& sFile)
{
    if (!m_pInternal->pReader)
        return;
    m_pInternal->pReader->SetCMapFile(sFile);
}

bool CPdfFile::LoadFromFile(const std::wstring& file, const std::wstring& options, const std::wstring& owner_password, const std::wstring& user_password)
{
    m_pInternal->pReader = new CPdfReader(m_pInternal->pAppFonts);
    if (!m_pInternal->pReader)
        return false;
    m_pInternal->wsSrcFile  = file;
    m_pInternal->wsPassword = owner_password;
    if (!m_pInternal->wsTempFolder.empty())
        m_pInternal->pReader->SetTempDirectory(m_pInternal->wsTempFolder);
    return m_pInternal->pReader->LoadFromFile(m_pInternal->pAppFonts, file, owner_password, user_password) && (m_pInternal->pReader->GetError() == 0);
}
bool CPdfFile::LoadFromMemory(BYTE* data, DWORD length, const std::wstring& options, const std::wstring& owner_password, const std::wstring& user_password)
{
    m_pInternal->pReader = new CPdfReader(m_pInternal->pAppFonts);
    if (!m_pInternal->pReader)
        return false;
    m_pInternal->wsSrcFile  = L"";
    m_pInternal->wsPassword = owner_password;
    return m_pInternal->pReader->LoadFromMemory(m_pInternal->pAppFonts, data, length, owner_password, user_password) && (m_pInternal->pReader->GetError() == 0);
}
NSFonts::IApplicationFonts* CPdfFile::GetFonts()
{
    return m_pInternal->pAppFonts;
}
OfficeDrawingFileType CPdfFile::GetType()
{
    return odftPDF;
}
std::wstring CPdfFile::GetTempDirectory()
{
    if (!m_pInternal->pReader)
        return std::wstring();
    return m_pInternal->pReader->GetTempDirectory();
}
void CPdfFile::SetTempDirectory(const std::wstring& wsPath)
{
    m_pInternal->wsTempFolder = wsPath;
    if (m_pInternal->pReader)
        m_pInternal->pReader->SetTempDirectory(wsPath);
}
int CPdfFile::GetPagesCount()
{
    if (!m_pInternal->pReader)
        return 0;
    PDFDoc* pPdfDoc = m_pInternal->pReader->GetPDFDocument();
    int nPages = pPdfDoc ? pPdfDoc->getNumPages() : 0;
#ifndef BUILDING_WASM_MODULE
    if (m_pInternal->bEdit && m_pInternal->pWriter && m_pInternal->pWriter->m_pDocument)
    {
        int nWPages = m_pInternal->pWriter->m_pDocument->GetPagesCount();
        if (nWPages > 0)
            nPages = nWPages;
    }
#endif
    return nPages;
}
void CPdfFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
    if (!m_pInternal->pReader)
        return;
#ifndef BUILDING_WASM_MODULE
    if (m_pInternal->bEdit && m_pInternal->pWriter && m_pInternal->pWriter->m_pDocument)
    {
        PdfWriter::CPage* pPage = m_pInternal->pWriter->m_pDocument->GetPage(nPageIndex);
        if (!pPage)
            return;

        int nRotate = pPage->GetRotate();
        if (nRotate % 180 == 0)
        {
            *pdWidth  = pPage->GetWidth();
            *pdHeight = pPage->GetHeight();
        }
        else
        {
            *pdWidth  = pPage->GetHeight();
            *pdHeight = pPage->GetWidth();
        }

        *pdDpiX = 72.0;
        *pdDpiY = 72.0;
    }
    else
#endif
        m_pInternal->pReader->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
}
void CPdfFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
    if (!m_pInternal->pReader)
        return;
    m_pInternal->pReader->DrawPageOnRenderer(pRenderer, nPageIndex, pBreak);
}
std::wstring CPdfFile::GetInfo()
{
    if (!m_pInternal->pReader)
        return std::wstring();
    return m_pInternal->pReader->GetInfo();
}
BYTE* CPdfFile::GetStructure()
{
    if (!m_pInternal->pReader)
        return NULL;
    return m_pInternal->pReader->GetStructure();
}
BYTE* CPdfFile::GetLinks(int nPageIndex)
{
    if (!m_pInternal->pReader)
        return NULL;
    return m_pInternal->pReader->GetLinks(nPageIndex);
}

// ------------------------------------------------------------------------

void CPdfFile::CreatePdf(bool isPDFA)
{
    RELEASEOBJECT(m_pInternal->pWriter);
    m_pInternal->pWriter = new CPdfWriter(m_pInternal->pAppFonts, isPDFA);
}
int CPdfFile::SaveToFile(const std::wstring& wsPath)
{
    if (!m_pInternal->pWriter)
        return 0;
    return m_pInternal->pWriter->SaveToFile(wsPath);
}
void CPdfFile::SetPassword(const std::wstring& wsPassword)
{
    if (!m_pInternal->pWriter)
        return;
    m_pInternal->pWriter->SetPassword(wsPassword);
}
void CPdfFile::SetDocumentID(const std::wstring& wsDocumentID)
{
    if (!m_pInternal->pWriter)
        return;
    m_pInternal->pWriter->SetDocumentID(wsDocumentID);
}
HRESULT CPdfFile::OnlineWordToPdf(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams)
{
#ifndef BUILDING_WASM_MODULE
    if (!m_pInternal->pWriter || !NSOnlineOfficeBinToPdf::ConvertBinToPdf(this, wsSrcFile, wsDstFile, false, pParams))
        return S_FALSE;
#endif
    return S_OK;
}
HRESULT CPdfFile::OnlineWordToPdfFromBinary(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams)
{
#ifndef BUILDING_WASM_MODULE
    if (!m_pInternal->pWriter || !NSOnlineOfficeBinToPdf::ConvertBinToPdf(this, wsSrcFile, wsDstFile, true, pParams))
        return S_FALSE;
#endif
    return S_OK;
}
HRESULT CPdfFile::DrawImageWith1bppMask(IGrObject* pImage, NSImages::CPixJbig2* pMaskBuffer, const unsigned int& unMaskWidth, const unsigned int& unMaskHeight, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->DrawImageWith1bppMask(pImage, pMaskBuffer, unMaskWidth, unMaskHeight, dX, dY, dW, dH);
}
HRESULT CPdfFile::DrawImage1bpp(NSImages::CPixJbig2* pImageBuffer, const unsigned int& unWidth, const unsigned int& unHeight, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->DrawImage1bpp(pImageBuffer, unWidth, unHeight, dX, dY, dW, dH);
}
HRESULT CPdfFile::SetLinearGradient(const double& dX1, const double& dY1, const double& dX2, const double& dY2)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->SetLinearGradient(dX1, dY1, dX2, dY2);
}
HRESULT CPdfFile::SetRadialGradient(const double& dX1, const double& dY1, const double& dR1, const double& dX2, const double& dY2, const double& dR2)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->SetRadialGradient(dX1, dY1, dR1, dX2, dY2, dR2);
}

HRESULT CPdfFile::get_Type(LONG* lType)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    *lType = c_nPDFWriter;
    return S_OK;
}
HRESULT CPdfFile::NewPage()
{
    if (!m_pInternal->pWriter || m_pInternal->bEdit)
        return S_FALSE;
    return m_pInternal->pWriter->NewPage();
}
HRESULT CPdfFile::get_Height(double* dHeight)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_Height(dHeight);
}
HRESULT CPdfFile::put_Height(const double& dHeight)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    if (m_pInternal->bEdit && m_pInternal->bEditPage)
        return S_OK;
    return m_pInternal->pWriter->put_Height(dHeight);
}
HRESULT CPdfFile::get_Width(double* dWidth)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_Width(dWidth);
}
HRESULT CPdfFile::put_Width(const double& dWidth)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    if (m_pInternal->bEdit && m_pInternal->bEditPage)
        return S_OK;
    return m_pInternal->pWriter->put_Width(dWidth);
}
HRESULT CPdfFile::get_DpiX(double* dDpiX)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    *dDpiX = 72;
    return S_OK;
}
HRESULT CPdfFile::get_DpiY(double* dDpiY)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    *dDpiY = 72;
    return S_OK;
}
HRESULT CPdfFile::get_PenColor(LONG* lColor)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenColor(lColor);
}
HRESULT CPdfFile::put_PenColor(const LONG& lColor)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenColor(lColor);
}
HRESULT CPdfFile::get_PenAlpha(LONG* lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenAlpha(lAlpha);
}
HRESULT CPdfFile::put_PenAlpha(const LONG& lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenAlpha(lAlpha);
}
HRESULT CPdfFile::get_PenSize(double* dSize)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenSize(dSize);
}
HRESULT CPdfFile::put_PenSize(const double& dSize)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenSize(dSize);
}
HRESULT CPdfFile::get_PenDashStyle(BYTE* nDashStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenDashStyle(nDashStyle);
}
HRESULT CPdfFile::put_PenDashStyle(const BYTE& nDashStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenDashStyle(nDashStyle);
}
HRESULT CPdfFile::get_PenLineStartCap(BYTE* nCapStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenLineStartCap(nCapStyle);
}
HRESULT CPdfFile::put_PenLineStartCap(const BYTE& nCapStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenLineStartCap(nCapStyle);
}
HRESULT CPdfFile::get_PenLineEndCap(BYTE* nCapStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenLineEndCap(nCapStyle);
}
HRESULT CPdfFile::put_PenLineEndCap(const BYTE& nCapStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenLineEndCap(nCapStyle);
}
HRESULT CPdfFile::get_PenLineJoin(BYTE* nJoinStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenLineJoin(nJoinStyle);
}
HRESULT CPdfFile::put_PenLineJoin(const BYTE& nJoinStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenLineJoin(nJoinStyle);
}
HRESULT CPdfFile::get_PenDashOffset(double* dOffset)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenDashOffset(dOffset);
}
HRESULT CPdfFile::put_PenDashOffset(const double& dOffset)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenDashOffset(dOffset);
}
HRESULT CPdfFile::get_PenAlign(LONG* lAlign)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenAlign(lAlign);
}
HRESULT CPdfFile::put_PenAlign(const LONG& lAlign)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenAlign(lAlign);
}
HRESULT CPdfFile::get_PenMiterLimit(double* dMiter)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_PenMiterLimit(dMiter);
}
HRESULT CPdfFile::put_PenMiterLimit(const double& dMiter)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_PenMiterLimit(dMiter);
}
HRESULT CPdfFile::PenDashPattern(double* pPattern, LONG lCount)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PenDashPattern(pPattern, lCount);
}
HRESULT CPdfFile::get_BrushType(LONG* lType)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushType(lType);
}
HRESULT CPdfFile::put_BrushType(const LONG& lType)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushType(lType);
}
HRESULT CPdfFile::get_BrushColor1(LONG* lColor)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushColor1(lColor);
}
HRESULT CPdfFile::put_BrushColor1(const LONG& lColor)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushColor1(lColor);
}
HRESULT CPdfFile::get_BrushAlpha1(LONG* lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushAlpha1(lAlpha);
}
HRESULT CPdfFile::put_BrushAlpha1(const LONG& lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushAlpha1(lAlpha);
}
HRESULT CPdfFile::get_BrushColor2(LONG* lColor)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushColor2(lColor);
}
HRESULT CPdfFile::put_BrushColor2(const LONG& lColor)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushColor2(lColor);
}
HRESULT CPdfFile::get_BrushAlpha2(LONG* lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushAlpha2(lAlpha);
}
HRESULT CPdfFile::put_BrushAlpha2(const LONG& lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushAlpha2(lAlpha);
}
HRESULT CPdfFile::get_BrushTexturePath(std::wstring* wsPath)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushTexturePath(wsPath);
}
HRESULT CPdfFile::put_BrushTexturePath(const std::wstring& wsPath)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushTexturePath(wsPath);
}
HRESULT CPdfFile::get_BrushTextureMode(LONG* lMode)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushTextureMode(lMode);
}
HRESULT CPdfFile::put_BrushTextureMode(const LONG& lMode)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushTextureMode(lMode);
}
HRESULT CPdfFile::get_BrushTextureAlpha(LONG* lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushTextureAlpha(lAlpha);
}
HRESULT CPdfFile::put_BrushTextureAlpha(const LONG& lAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushTextureAlpha(lAlpha);
}
HRESULT CPdfFile::get_BrushLinearAngle(double* dAngle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_BrushLinearAngle(dAngle);
}
HRESULT CPdfFile::put_BrushLinearAngle(const double& dAngle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushLinearAngle(dAngle);
}
HRESULT CPdfFile::BrushRect(const INT& nVal, const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->BrushRect(nVal, dLeft, dTop, dWidth, dHeight);
}
HRESULT CPdfFile::BrushBounds(const double& dLeft, const double& dTop, const double& dWidth, const double& dHeight)
{
    return m_pInternal ? S_OK : S_FALSE;
}
HRESULT CPdfFile::put_BrushGradientColors(LONG* pColors, double* pPositions, LONG lCount)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_BrushGradientColors(pColors, pPositions, lCount);
}
HRESULT CPdfFile::get_FontName(std::wstring* wsName)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_FontName(wsName);
}
HRESULT CPdfFile::put_FontName(const std::wstring& wsName)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_FontName(wsName);
}
HRESULT CPdfFile::get_FontPath(std::wstring* wsPath)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_FontPath(wsPath);
}
HRESULT CPdfFile::put_FontPath(const std::wstring& wsPath)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_FontPath(wsPath);
}
HRESULT CPdfFile::get_FontSize(double* dSize)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_FontSize(dSize);
}
HRESULT CPdfFile::put_FontSize(const double& dSize)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_FontSize(dSize);
}
HRESULT CPdfFile::get_FontStyle(LONG* lStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_FontStyle(lStyle);
}
HRESULT CPdfFile::put_FontStyle(const LONG& lStyle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_FontStyle(lStyle);
}
HRESULT CPdfFile::get_FontStringGID(INT* bGid)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_FontStringGID(bGid);
}
HRESULT CPdfFile::put_FontStringGID(const INT& bGid)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_FontStringGID(bGid);
}
HRESULT CPdfFile::get_FontCharSpace(double* dSpace)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_FontCharSpace(dSpace);
}
HRESULT CPdfFile::put_FontCharSpace(const double& dSpace)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_FontCharSpace(dSpace);
}
HRESULT CPdfFile::get_FontFaceIndex(int* lFaceIndex)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->get_FontFaceIndex(lFaceIndex);
}
HRESULT CPdfFile::put_FontFaceIndex(const int& lFaceIndex)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->put_FontFaceIndex(lFaceIndex);
}
HRESULT CPdfFile::CommandDrawTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->CommandDrawTextCHAR(lUnicode, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->CommandDrawTextExCHAR(lUnicode, lGid, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->CommandDrawText(wsUnicodeText, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->CommandDrawTextEx(wsUnicodeText, pGids, nGidsCount, dX, dY, dW, dH);
}
HRESULT CPdfFile::CommandDrawTextCHAR2(unsigned int* unUnicode, const unsigned int& unUnicodeCount, const unsigned int& unGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->CommandDrawTextCHAR2(unUnicode, unUnicodeCount, unGid, dX, dY, dW, dH);
}
HRESULT CPdfFile::BeginCommand(const DWORD& lType)
{
    return m_pInternal->pWriter ? S_OK : S_FALSE;
}
HRESULT CPdfFile::EndCommand(const DWORD& lType)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->EndCommand(lType, m_pInternal->lClipMode);
}
HRESULT CPdfFile::PathCommandMoveTo(const double& dX, const double& dY)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandMoveTo(dX, dY);
}
HRESULT CPdfFile::PathCommandLineTo(const double& dX, const double& dY)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandLineTo(dX, dY);
}
HRESULT CPdfFile::PathCommandLinesTo(double* pPoints, const int& nCount)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandLinesTo(pPoints, nCount);
}
HRESULT CPdfFile::PathCommandCurveTo(const double& dX1, const double& dY1, const double& dX2, const double& dY2, const double& dXe, const double& dYe)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandCurveTo(dX1, dY1, dX2, dY2, dXe, dYe);
}
HRESULT CPdfFile::PathCommandCurvesTo(double* pPoints, const int& nCount)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandCurvesTo(pPoints, nCount);
}
HRESULT CPdfFile::PathCommandArcTo(const double& dX, const double& dY, const double& dW, const double& dH, const double& dStartAngle, const double& dSweepAngle)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandArcTo(dX, dY, dW, dH, dStartAngle, dSweepAngle);
}
HRESULT CPdfFile::PathCommandClose()
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandClose();
}
HRESULT CPdfFile::PathCommandEnd()
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandEnd();
}
HRESULT CPdfFile::DrawPath(const LONG& lType)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->DrawPath(m_pInternal->pAppFonts, m_pInternal->wsTempFolder, lType);
}
HRESULT CPdfFile::PathCommandStart()
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandStart();
}
HRESULT CPdfFile::PathCommandGetCurrentPoint(double* dX, double* dY)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandGetCurrentPoint(dX, dY);
}
HRESULT CPdfFile::PathCommandTextCHAR(const LONG& lUnicode, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandTextCHAR(lUnicode, dX, dY, dW, dH);
}
HRESULT CPdfFile::PathCommandTextExCHAR(const LONG& lUnicode, const LONG& lGid, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandTextExCHAR(lUnicode, lGid, dX, dY, dW, dH);
}
HRESULT CPdfFile::PathCommandText(const std::wstring& wsUnicodeText, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandText(wsUnicodeText, dX, dY, dW, dH);
}
HRESULT CPdfFile::PathCommandTextEx(const std::wstring& wsUnicodeText, const unsigned int* pGids, const unsigned int nGidsCount, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->PathCommandTextEx(wsUnicodeText, pGids, nGidsCount, dX, dY, dW, dH);
}
HRESULT CPdfFile::DrawImage(IGrObject* pImage, const double& dX, const double& dY, const double& dW, const double& dH)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->DrawImage(pImage, dX, dY, dW, dH);
}
HRESULT CPdfFile::DrawImageFromFile(const std::wstring& wsImagePath, const double& dX, const double& dY, const double& dW, const double& dH, const BYTE& nAlpha)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->DrawImageFromFile(m_pInternal->pAppFonts, m_pInternal->wsTempFolder, wsImagePath, dX, dY, dW, dH, nAlpha);
}
HRESULT CPdfFile::SetTransform(const double& dM11, const double& dM12, const double& dM21, const double& dM22, const double& dX, const double& dY)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->SetTransform(dM11, dM12, dM21, dM22, dX, dY);
}
HRESULT CPdfFile::GetTransform(double* dM11, double* dM12, double* dM21, double* dM22, double* dX, double* dY)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->GetTransform(dM11, dM12, dM21, dM22, dX, dY);
}
HRESULT CPdfFile::ResetTransform()
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->ResetTransform();
}
HRESULT CPdfFile::get_ClipMode(LONG* lMode)
{
    *lMode = m_pInternal->lClipMode;
    return S_OK;
}
HRESULT CPdfFile::put_ClipMode(const LONG& lMode)
{
    m_pInternal->lClipMode = lMode;
    return S_OK;
}
HRESULT CPdfFile::CommandLong(const LONG& lType, const LONG& lCommand)
{
    return m_pInternal->pWriter ? S_OK : S_FALSE;
}
HRESULT CPdfFile::CommandDouble(const LONG& lType, const double& dCommand)
{
    return m_pInternal->pWriter ? S_OK : S_FALSE;
}
HRESULT CPdfFile::CommandString(const LONG& lType, const std::wstring& sCommand)
{
    return m_pInternal->pWriter ? S_OK : S_FALSE;
}
HRESULT CPdfFile::AddHyperlink(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsUrl, const std::wstring& wsTooltip)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->AddHyperlink(dX, dY, dW, dH, wsUrl, wsTooltip);
}
HRESULT CPdfFile::AddLink(const double& dX, const double& dY, const double& dW, const double& dH, const double& dDestX, const double& dDestY, const int& nPage)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->AddLink(dX, dY, dW, dH, dDestX, dDestY, nPage);
}
HRESULT CPdfFile::AddFormField(IFormField* pFieldInfo)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    return m_pInternal->pWriter->AddFormField(m_pInternal->pAppFonts, pFieldInfo);
}
HRESULT CPdfFile::DocInfo(const std::wstring& wsTitle, const std::wstring& wsCreator, const std::wstring& wsSubject, const std::wstring& wsKeywords)
{
    if (!m_pInternal->pWriter)
        return S_FALSE;
    m_pInternal->pWriter->SetDocumentInfo(wsTitle, wsCreator, wsSubject, wsKeywords);
    return S_OK;
}
