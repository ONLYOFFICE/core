#include "../../../common/File.h"
#include "../include/XmlCertificate.h"
#include "../include/OOXMLSigner.h"
#include "../include/OOXMLVerifier.h"

#ifdef _WIN32
//#define USE_MS_CRYPTO
#endif

#define USE_SIGN
//#define USE_VERIFY

int main()
{
    std::wstring sTestDir = NSFile::GetProcessDirectory() + L"/../../";
    ICertificate* pCertificate = NULL;

#ifdef USE_MS_CRYPTO

    // TODO: Load sertificate from store

#else

    pCertificate = ICertificate::CreateInstance();
    pCertificate->FromFiles(sTestDir + L"keys/key.key", "", sTestDir + L"keys/cert.crt", "");
#endif

    BYTE* pDataDst = NULL;
    unsigned long nLenDst = 0;

#ifdef USE_SIGN
#if 0
    COOXMLSigner oSigner(sTestDir + L"file", pCertificate);
    oSigner.Sign(pDataDst, nLenDst);
#else
    BYTE* pDataSrc = NULL;
    unsigned long nLenSrc = 0;
    NSFile::CFileBinary::ReadAllBytes(sTestDir + L"/file.docx", &pDataSrc, nLenSrc);

    COOXMLSigner oSigner(pDataSrc, nLenSrc, pCertificate);
    oSigner.Sign(pDataDst, nLenDst);
    RELEASEARRAYOBJECTS(pDataSrc);

    NSFile::CFileBinary oFileDst;
    oFileDst.CreateFileW(sTestDir + L"/file2.docx");
    oFileDst.WriteFile(pDataDst, nLenDst);
    oFileDst.CloseFile();
#endif
#endif

    RELEASEARRAYOBJECTS(pDataDst);

#ifdef USE_VERIFY
#if 1
    COOXMLVerifier oVerifier(sTestDir + L"file");
    int nCount = oVerifier.GetSignatureCount();
    for (int i = 0; i < nCount; i++)
    {
        COOXMLSignature* pSign = oVerifier.GetSignature(i);
        pSign->Check();
    }
#endif
#endif

    delete pCertificate;
    return 0;
}
