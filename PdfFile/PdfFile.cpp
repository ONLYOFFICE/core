#include "PdfFile.h"
#include "../PdfReader/Src/Adaptors.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Path.h"
#include "../DesktopEditor/common/StringExt.h"

#include "../PdfReader/lib/xpdf/PDFDoc.h"
#include "../PdfReader/lib/xpdf/AcroForm.h"
#include "../PdfReader/lib/xpdf/TextString.h"

#include "../PdfWriter/Src/Objects.h"
#include "../PdfWriter/Src/Document.h"
#include "../PdfWriter/Src/Pages.h"
#include "../PdfWriter/Src/Catalog.h"
#include "../PdfWriter/Src/EncryptDictionary.h"
#include "../PdfWriter/Src/Info.h"

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

class CPdfFile_Private
{
public:
    CPdfRenderer* pWriter;
    PdfReader::CPdfReader* pReader;
    std::wstring wsSrcFile;
    std::wstring wsPassword;

    void GetPageTree(XRef* xref, Object* pPagesRefObj)
    {
        PdfWriter::CDocument* pDoc = pWriter->GetPDFDocument();
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
};

CPdfFile::CPdfFile(NSFonts::IApplicationFonts* pAppFonts, bool isPDFA)
{
    m_pInternal = new CPdfFile_Private();

    m_pInternal->pWriter = new CPdfRenderer (pAppFonts, isPDFA);
    m_pInternal->pReader = new PdfReader::CPdfReader(pAppFonts);
    m_pInternal->wsPassword = L"";
}

CPdfFile::~CPdfFile()
{
    RELEASEOBJECT(m_pInternal->pWriter);
    RELEASEOBJECT(m_pInternal->pReader);
}

PdfReader::CPdfReader* CPdfFile::GetReader()
{
    return m_pInternal->pReader;
}

CPdfRenderer* CPdfFile::GetWriter()
{
    return m_pInternal->pWriter;
}

NSFonts::IFontManager* CPdfFile::GetFontManager()
{
    return m_pInternal->pReader->GetFontManager();
}

void CPdfFile::SetTempDirectory(const std::wstring& wsPath)
{
    m_pInternal->pWriter->SetTempFolder   (wsPath);
    m_pInternal->pReader->SetTempDirectory(wsPath);
}

bool CPdfFile::LoadFromFile(const std::wstring& wsSrcFile, const std::wstring& wsPassword)
{
    m_pInternal->wsSrcFile  = wsSrcFile;
    m_pInternal->wsPassword = wsPassword;
    return m_pInternal->pReader->LoadFromFile(wsSrcFile, L"", wsPassword, wsPassword) &&
          (m_pInternal->pReader->GetError() == 0);
}

bool CPdfFile::LoadFromMemory(BYTE* pData, DWORD nLength, const std::wstring& wsPassword)
{
    m_pInternal->wsSrcFile  = L"";
    m_pInternal->wsPassword = wsPassword;
    return m_pInternal->pReader->LoadFromMemory(pData, nLength, L"", wsPassword, wsPassword) &&
          (m_pInternal->pReader->GetError() == 0);
}

int CPdfFile::GetPagesCount()
{
    return m_pInternal->pReader->GetPagesCount();
}

void CPdfFile::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
    m_pInternal->pReader->GetPageInfo(nPageIndex, pdWidth, pdHeight, pdDpiX, pdDpiY);
}

void CPdfFile::DrawPageOnRenderer(IRenderer* pRenderer, int nPageIndex, bool* pBreak)
{
    m_pInternal->pReader->DrawPageOnRenderer(pRenderer, nPageIndex, pBreak);
}

int CPdfFile::GetError()
{
    return m_pInternal->pReader->GetError();
}

void CPdfFile::ConvertToRaster(int nPageIndex, const std::wstring& path, int nImageType, const int nRasterW, const int nRasterH,
                               bool bIsFlip, NSFonts::IFontManager* pFonts, int nBackgroundColor, bool bIsDarkMode)
{
    m_pInternal->pReader->ConvertToRaster(nPageIndex, path, nImageType, nRasterW, nRasterH, bIsFlip, pFonts, nBackgroundColor, bIsDarkMode);
}

bool CPdfFile::EditPdf(const std::wstring& wsDstFile)
{
    if (!wsDstFile.empty())
        NSFile::CFileBinary::Copy(m_pInternal->wsSrcFile, wsDstFile);

    PDFDoc* pPDFDocument = m_pInternal->pReader->GetPDFDocument();
    if (!pPDFDocument)
        return false;

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
    PdfWriter::CDocument* pDoc = m_pInternal->pWriter->GetPDFDocument();
    if (!xref || !pDoc)
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
    catRefObj.free();

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

    bool bRes = pDoc->EditPdf(wsDstFile, xref->getLastXRefPos(), xref->getNumObjects(), pXref, pCatalog, pEncryptDict, nFormField);
    if (bRes)
    {
        m_pInternal->GetPageTree(xref, &pagesRefObj);
        m_pInternal->pWriter->EditPdf();
    }
    pagesRefObj.free();
    return bRes;
}

bool CPdfFile::EditClose()
{
    PDFDoc* pPDFDocument = m_pInternal->pReader->GetPDFDocument();
    PdfWriter::CDocument* pDoc = m_pInternal->pWriter->GetPDFDocument();
    if (!pPDFDocument || !pDoc)
        return false;

    XRef* xref = pPDFDocument->getXRef();
    if (!xref)
        return false;

    // Добавляем первый элемент в таблицу xref
    // он должен иметь вид 0000000000 65535 f
    PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, 0, 65535);
    if (!pXref)
        return false;

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
            return false;
        pInfoDict = new PdfWriter::CInfoDict(pInfoXref);
        if (!pInfoDict)
        {
            RELEASEOBJECT(pInfoXref);
            return false;
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


    bool bRes = m_pInternal->pWriter->EditClose();
    if (!bRes)
        return false;
    bRes = pDoc->AddToFile(pXref, pTrailer, pInfoXref, pInfoDict);
    if (!bRes)
        return false;

    std::wstring wsPath = m_pInternal->pWriter->GetEditPdfPath();
    std::string sPathUtf8New = U_TO_UTF8(wsPath);
    std::string sPathUtf8Old = U_TO_UTF8(m_pInternal->wsSrcFile);
    if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
    {
        GString* owner_pswd = NSStrings::CreateString(m_pInternal->wsPassword);
        GString* user_pswd  = NSStrings::CreateString(m_pInternal->wsPassword);
        bRes &= (pPDFDocument->makeWritable(false, owner_pswd, user_pswd) != 0);
        delete owner_pswd;
        delete user_pswd;

        NSFile::CFileBinary oFile;
        if (oFile.OpenFile(m_pInternal->wsSrcFile))
        {
            m_pInternal->pReader->ChangeLength(oFile.GetFileSize());
            oFile.CloseFile();
        }
    }

    return bRes;
}

bool CPdfFile::EditPage(int nPageIndex)
{
    PDFDoc* pPDFDocument = m_pInternal->pReader->GetPDFDocument();
    PdfWriter::CDocument* pDoc = m_pInternal->pWriter->GetPDFDocument();
    if (!pPDFDocument || !pDoc)
        return false;

    XRef* xref = pPDFDocument->getXRef();
    Catalog* pCatalog = pPDFDocument->getCatalog();
    if (!xref || !pCatalog)
        return false;
    std::pair<int, int> pPageRef = m_pInternal->pWriter->GetPageRef(nPageIndex);
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

    PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, pPageRef.first);
    if (!pXref)
    {
        pageObj.free();
        return false;
    }

    PdfWriter::CPage* pNewPage = new PdfWriter::CPage(pXref, pDoc);
    if (!pNewPage)
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
        DictToCDictObject(&oTemp, pNewPage, true, chKey);
        oTemp.free();
    }
    pNewPage->SetRef(pPageRef.first, pPageRef.second);
    pNewPage->Fix();
    pageObj.free();

    if (m_pInternal->pWriter->EditPage(pNewPage) && pDoc->EditPage(pXref, pNewPage))
        return true;

    RELEASEOBJECT(pXref);
    return false;
}

bool CPdfFile::DeletePage(int nPageIndex)
{
    return m_pInternal->pWriter->DeletePage(nPageIndex);
}

bool CPdfFile::AddPage(int nPageIndex)
{
    bool bRes = m_pInternal->pWriter->AddPage(nPageIndex);
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

void CPdfFile::Sign(const double& dX, const double& dY, const double& dW, const double& dH, const std::wstring& wsPicturePath, ICertificate* pCertificate)
{
    m_pInternal->pWriter->Sign(dX, dY, dW, dH, wsPicturePath, pCertificate);
}

void CPdfFile::PageRotate(int nRotate)
{
    m_pInternal->pWriter->PageRotate(nRotate);
}

HRESULT CPdfFile::OnlineWordToPdf(const std::wstring& wsSrcFile, const std::wstring& wsDstFile, CConvertFromBinParams* pParams, bool bBinary)
{
    if (bBinary)
        return m_pInternal->pWriter->OnlineWordToPdfFromBinary(wsSrcFile, wsDstFile, pParams);
    return m_pInternal->pWriter->OnlineWordToPdf(wsSrcFile, wsDstFile, pParams);
}

void CPdfFile::SetPassword(const std::wstring& wsPassword)
{
    m_pInternal->pWriter->SetPassword(wsPassword);
}

void CPdfFile::SetDocumentID(const std::wstring& wsDocumentID)
{
    m_pInternal->pWriter->SetPassword(wsDocumentID);
}

int  CPdfFile::SaveToFile(const std::wstring& wsPath)
{
    int nPagesCount = m_pInternal->pReader->GetPagesCount();
    for (int i = 0; i < nPagesCount; ++i)
    {
        m_pInternal->pWriter->NewPage();
        m_pInternal->pWriter->BeginCommand(c_nPageType);

        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        m_pInternal->pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        m_pInternal->pWriter->put_Width(dWidth);
        m_pInternal->pWriter->put_Height(dHeight);

        m_pInternal->pReader->DrawPageOnRenderer(m_pInternal->pWriter, i, NULL);

        m_pInternal->pWriter->EndCommand(c_nPageType);
    }
    return m_pInternal->pWriter->SaveToFile(wsPath);
}

void CPdfFile::TEST(int i)
{
    if (i == 1)
    {
        m_pInternal->pWriter->PathCommandStart();
        m_pInternal->pWriter->PathCommandMoveTo(10, 10);
        m_pInternal->pWriter->PathCommandLineTo(20, 20);
        m_pInternal->pWriter->PathCommandCurveTo(70, 30, 30, 20, 50, 50);
        m_pInternal->pWriter->PathCommandClose();
        m_pInternal->pWriter->put_BrushColor1(0xFF0000);
        m_pInternal->pWriter->put_PenColor(0x0000FF);
        m_pInternal->pWriter->put_PenSize(1);
        m_pInternal->pWriter->DrawPath(c_nStroke | c_nWindingFillMode);
        m_pInternal->pWriter->PathCommandEnd();
    }
    else if (i == 2)
    {
        m_pInternal->pWriter->OnlineWordToPdf(NSFile::GetProcessDirectory() + L"/../../../PdfWriter/test/example/pdf.bin", L"");
    }
    else if (i == 3)
    {
        m_pInternal->pWriter->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/../../../PdfWriter/test/example1/1/pdf.bin", L"");
    }
}
