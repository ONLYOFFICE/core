#include "PdfFile.h"
#include "../PdfReader/Src/Adaptors.h"
#include "../DesktopEditor/common/File.h"
#include "../DesktopEditor/common/Path.h"
#include "../DesktopEditor/common/StringExt.h"

#include "../PdfReader/lib/xpdf/PDFDoc.h"
#include "../PdfReader/lib/xpdf/AcroForm.h"

class CPdfFile_Private
{
public:
    CPdfRenderer* pWriter;
    PdfReader::CPdfReader* pReader;
    std::wstring wsSrcFile;
    std::wstring wsPassword;

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

        pWriter->CreatePageTree(sPageTree, topPagesRef.num);

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

    // PdfReader

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

    // TODO Dict to PdfWriter::CDictObject
    Ref catRef = catRefObj.getRef();
    std::wstring sCatalog = XMLConverter::DictToXml(L"Catalog", &catDict, catRef.num, catRef.gen);

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

    // PdfWriter

    bool bRes = m_pInternal->pWriter->EditPdf(wsDstFile, xref->getLastXRefPos(), xref->getNumObjects(), sCatalog, catRef.num, sEncrypt, m_pInternal->wsPassword, nCryptAlgorithm, nFormField);
    if (bRes)
        m_pInternal->GetPageTree(xref, &pagesRefObj);
    pagesRefObj.free();
    catDict.free();
    catRefObj.free();
    return bRes;
}

bool CPdfFile::EditClose()
{
    PDFDoc* pPDFDocument = m_pInternal->pReader->GetPDFDocument();
    if (!pPDFDocument)
        return false;

    XRef* xref = pPDFDocument->getXRef();
    if (!xref)
        return false;

    Object* trailerDict = xref->getTrailerDict();
    std::wstring sTrailer;
    if (trailerDict)
        sTrailer = XMLConverter::DictToXml(L"Trailer", trailerDict);

    std::wstring sInfo;
    Object info;
    pPDFDocument->getDocInfo(&info);
    if (info.isDict())
        sInfo = XMLConverter::DictToXml(L"Info", &info);
    info.free();

    std::wstring wsPath = m_pInternal->pWriter->GetEditPdfPath();
    bool bRes = m_pInternal->pWriter->EditClose(sTrailer, sInfo);

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
    if (!pPDFDocument)
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
    std::wstring sPage = XMLConverter::DictToXml(L"Page", &pageObj, pPageRef.first, pPageRef.second);
    pageObj.free();
    pageRefObj.free();

    return m_pInternal->pWriter->EditPage(sPage, pPageRef.first);
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
        m_pInternal->pWriter->OnlineWordToPdf(NSFile::GetProcessDirectory() + L"/../example/pdf.bin", L"");
    }
    else if (i == 3)
    {
        m_pInternal->pWriter->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/../example1/1/pdf.bin", L"");
    }
}
