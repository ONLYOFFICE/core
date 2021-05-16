#include "../../../common/File.h"

#include "../include/XmlCertificate.h"
#include "../src/XmlSigner_openssl.h"

#include "../include/OOXMLSigner.h"
#include "../include/OOXMLVerifier.h"

#ifdef _WIN32
#define WASM_EXPORT __declspec(dllexport)
#else
#define WASM_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

WASM_EXPORT void* XmlSirnature_Malloc(unsigned int size)
{
    return ::malloc(size);
}
WASM_EXPORT void XmlSirnature_Free(void* p)
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
    CCertificate_openssl* m_pCert;

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
        m_pCert = new CCertificate_openssl();
    }
    ~CCertificate()
    {
        delete m_pCert;
    }

public:
    int LoadCert(unsigned char* data, unsigned int len, char* pass)
    {
        m_cert = NULL; m_certLen = 0; m_certPass = "";
        int nRes = NSOpenSSL::LoadCertRaw(data, len, pass);

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
        int nRes = NSOpenSSL::LoadKeyRaw(data, len, pass);

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
        m_pCert->FromFilesRaw(m_key, m_keyLen, m_keyPass, m_cert, m_certLen, m_certPass);
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
WASM_EXPORT int XmlSignature_LoadCert(CCertificate* cert, unsigned char* data, unsigned int len, char* pass)
{
    return cert->LoadCert(data, len, pass);
}
WASM_EXPORT int XmlSignature_LoadKey(CCertificate* cert, unsigned char* data, unsigned int len, char* pass)
{
    return cert->LoadKey(data, len, pass);
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
WASM_EXPORT void XmlSignature_DestroyFile(void* file)
{
    CMemoryFile* pZipFile = (CMemoryFile*)file;
    delete pZipFile;
}

WASM_EXPORT void* XmlSignature_Sign(CMemoryFile* file, CCertificate* cert)
{
    cert->Load();
    COOXMLSigner oSigner(file->Data, file->Length, cert->GetCertificate());

    BYTE* pDataDst = NULL;
    unsigned long lDataDstLen = 0;

    oSigner.Sign(pDataDst, lDataDstLen);

    CMemoryFile* fileDst = new CMemoryFile(pDataDst, (unsigned int)lDataDstLen);
    return fileDst;
}

#ifdef __cplusplus
}
#endif

#ifdef TEST_AS_EXECUTABLE
int main()
{
    return 0;
}
#endif

