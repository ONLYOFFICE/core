#include "../../../../common/File.h"

#include "../../include/Certificate.h"
#include "../../include/CertificateCommon.h"

#include "../../include/OOXMLSigner.h"
#include "../../include/OOXMLVerifier.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT void* XmlSignature_Malloc(unsigned int size)
{
    return ::malloc(size);
}
WASM_EXPORT void XmlSignature_Free(void* p)
{
    if (p) ::free(p);
}

class CMemoryFile
{
public:
    unsigned int Length;
    BYTE* Data;

public:
    CMemoryFile(BYTE* data = NULL, unsigned int len = 0) { Length = len; Data = data; }
    ~CMemoryFile() { if (Data) delete [] Data; }
};

class CCertificate
{
public:
    ICertificate* m_pCert;

private:
    unsigned char* m_key;
    unsigned int m_keyLen;
    std::string m_keyPass;

    unsigned char* m_cert;
    unsigned int m_certLen;
    std::string m_certPass;

public:
    CCertificate()
    {
        m_pCert = NULL;
    }
    ~CCertificate()
    {
        delete m_pCert;
    }

public:
    int LoadCert(unsigned char* data, unsigned int len, char* pass)
    {
        m_cert = NULL; m_certLen = 0; m_certPass = "";
        int nRes = NSOpenSSL::LoadCertRaw(data, len, (NULL != pass) ? std::string(pass) : "");

        if (OPEN_SSL_WARNING_ALL_OK == nRes || OPEN_SSL_WARNING_OK == nRes)
        {
            m_cert = data;
            m_certLen = len;
            m_certPass = (NULL != pass) ? std::string(pass) : "";
        }
        if (OPEN_SSL_WARNING_ALL_OK == nRes)
        {
            m_key = m_cert;
            m_keyLen = m_certLen;
            m_keyPass = m_certPass;
        }

        return nRes;
    }
    int LoadKey(unsigned char* data, unsigned int len, char* pass)
    {
        m_key = NULL; m_keyLen = 0; m_keyPass = "";
        int nRes = NSOpenSSL::LoadKeyRaw(data, len, (NULL != pass) ? std::string(pass) : "");

        if (OPEN_SSL_WARNING_ALL_OK == nRes || OPEN_SSL_WARNING_OK == nRes)
        {
            m_key = data;
            m_keyLen = len;
            m_keyPass = (NULL != pass) ? std::string(pass) : "";
        }
        if (OPEN_SSL_WARNING_ALL_OK == nRes)
        {
            m_cert = m_key;
            m_certLen = m_keyLen;
            m_certPass = m_keyPass;
        }

        return nRes;
    }

    void Load()
    {
        m_pCert = NSCertificate::FromFilesRaw(m_key, m_keyLen, m_keyPass, m_cert, m_certLen, m_certPass);
    }

    ICertificate* GetCertificate()
    {
        return m_pCert;
    }
};

WASM_EXPORT void* XmlSignature_CreateCertificate()
{
    return new CCertificate();
}
WASM_EXPORT int XmlSignature_LoadCert(void* cert, unsigned char* data, unsigned int len, char* pass)
{
    CCertificate* pCert = (CCertificate*)cert;
    return pCert->LoadCert(data, len, pass);
}
WASM_EXPORT int XmlSignature_LoadKey(void* cert, unsigned char* data, unsigned int len, char* pass)
{
    CCertificate* pCert = (CCertificate*)cert;
    return pCert->LoadKey(data, len, pass);
}
WASM_EXPORT void XmlSignature_DestroyCertificate(void* cert)
{
    CCertificate* pCert = (CCertificate*)cert;
    delete pCert;
}

WASM_EXPORT void* XmlSignature_CreateFile(unsigned char* file, unsigned int len)
{
    return new CMemoryFile(file, len);
}
WASM_EXPORT unsigned int XmlSignature_GetFileSize(void* file)
{
    CMemoryFile* pZipFile = (CMemoryFile*)file;
    return pZipFile->Length;
}
WASM_EXPORT void* XmlSignature_GetFileRaw(void* file)
{
    CMemoryFile* pZipFile = (CMemoryFile*)file;
    return pZipFile->Data;
}
WASM_EXPORT void XmlSignature_DestroyFile(void* file)
{
    CMemoryFile* pZipFile = (CMemoryFile*)file;
    delete pZipFile;
}

WASM_EXPORT void* XmlSignature_Sign(void* file, void* cert)
{
    CCertificate* pCert   = (CCertificate*)cert;
    CMemoryFile* pZipFile = (CMemoryFile*)file;

    pCert->Load();
    COOXMLSigner oSigner(pZipFile->Data, pZipFile->Length, pCert->GetCertificate());

    CMemoryFile* pFile = new CMemoryFile();
    unsigned long lDataDstLen = 0;

    oSigner.Sign(pFile->Data, lDataDstLen);
    pFile->Length = (unsigned int)lDataDstLen;

    return pFile;
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
int main()
{
    std::wstring sTestDir = NSFile::GetProcessDirectory() + L"/../../../../test/";
    void* cert = XmlSignature_CreateCertificate();

    BYTE* pCertData = NULL;
    DWORD nCertLen  = 0;
    NSFile::CFileBinary::ReadAllBytes(sTestDir + L"keys/cert.crt", &pCertData, nCertLen);
    XmlSignature_LoadCert(cert, pCertData, nCertLen, NULL);

    BYTE* pKeyData = NULL;
    DWORD nKeyLen  = 0;
    NSFile::CFileBinary::ReadAllBytes(sTestDir + L"keys/key.key", &pKeyData, nKeyLen);
    XmlSignature_LoadKey(cert, pKeyData, nKeyLen, NULL);

    BYTE* pData = NULL;
    DWORD nLen  = 0;
    NSFile::CFileBinary::ReadAllBytes(sTestDir + L"file.docx", &pData, nLen);
    CMemoryFile* file = (CMemoryFile*)XmlSignature_CreateFile(pData, nLen);
    CMemoryFile* res  = (CMemoryFile*)XmlSignature_Sign(file, cert);

    NSFile::CFileBinary oFileDst;
    oFileDst.CreateFileW(sTestDir + L"/file2.docx");
    oFileDst.WriteFile(res->Data, (DWORD)res->Length);
    oFileDst.CloseFile();

    XmlSignature_DestroyFile(file); // RELEASEARRAYOBJECTS(pData);
    XmlSignature_DestroyFile(res);
    XmlSignature_DestroyCertificate(cert);

    RELEASEARRAYOBJECTS(pCertData);
    RELEASEARRAYOBJECTS(pKeyData);

    return 0;
}
#endif
