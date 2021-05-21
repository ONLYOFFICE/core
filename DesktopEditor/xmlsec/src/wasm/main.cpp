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

class CData
{
protected:
    unsigned char* m_pData;
    size_t m_lSize;

    unsigned char* m_pDataCur;
    size_t m_lSizeCur;

public:
    CData()
    {
        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = m_lSize;
    }
    CData(size_t nLen)
    {
        m_lSize = nLen;
        m_pData = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));

        m_lSizeCur = 0;
        m_pDataCur = m_pData;
    }
    CData(unsigned char* value, unsigned int len)
    {
        m_lSize = len;
        m_pData = value;

        m_lSizeCur = m_lSize;
        m_pDataCur = m_pData + m_lSizeCur;
    }
    virtual ~CData()
    {
        Clear();
    }

    inline void AddSize(size_t nSize)
    {
        if (NULL == m_pData)
        {
            m_lSize = 1000;
            if (nSize > m_lSize)
                m_lSize = nSize;

            m_pData = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));

            m_lSizeCur = 0;
            m_pDataCur = m_pData;
            return;
        }

        if ((m_lSizeCur + nSize) > m_lSize)
        {
            while ((m_lSizeCur + nSize) > m_lSize)
                m_lSize *= 2;

            unsigned char* pRealloc = (unsigned char*)realloc(m_pData, m_lSize * sizeof(unsigned char));
            if (NULL != pRealloc)
            {
                m_pData    = pRealloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
            else
            {
                unsigned char* pMalloc = (unsigned char*)malloc(m_lSize * sizeof(unsigned char));
                memcpy(pMalloc, m_pData, m_lSizeCur * sizeof(unsigned char));

                free(m_pData);
                m_pData    = pMalloc;
                m_pDataCur = m_pData + m_lSizeCur;
            }
        }
    }

public:
    void AddInt(unsigned int value)
    {
        AddSize(4);
        memcpy(m_pDataCur, &value, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
    }
    void WriteString(unsigned char* value, unsigned int len)
    {
        AddSize(len + 4);
        memcpy(m_pDataCur, &len, sizeof(unsigned int));
        m_pDataCur += 4;
        m_lSizeCur += 4;
        memcpy(m_pDataCur, value, len);
        m_pDataCur += len;
        m_lSizeCur += len;
    }
    unsigned char* GetBuffer()
    {
        return m_pData;
    }

    void Clear()
    {
        if (m_pData) free(m_pData);

        m_pData = NULL;
        m_lSize = 0;

        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    void ClearNoAttack()
    {
        m_pDataCur = m_pData;
        m_lSizeCur = 0;
    }
    unsigned int GetSize()
    {
        return (unsigned int)m_lSizeCur;
    }

    void SkipLen()
    {
        AddInt(0);
    }
    void WriteLen()
    {
        unsigned int len = (unsigned int)m_lSizeCur;
        memcpy(m_pData, &len, sizeof(unsigned int));
    }
};

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

    BYTE* pDataDst = NULL;
    unsigned long lDataDstLen = 0;

    oSigner.Sign(pDataDst, lDataDstLen);

    CData* oData = new CData();
    oData->WriteString(pDataDst, lDataDstLen);
    return oData->GetBuffer();
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
    void* file = XmlSignature_CreateFile(pData, nLen);
    void* res  = XmlSignature_Sign(file, cert);
    BYTE* pRes = (BYTE*)res;

    NSFile::CFileBinary oFileDst;
    oFileDst.CreateFileW(sTestDir + L"/file2.docx");
    oFileDst.WriteFile(pRes + 4, pRes[0] | pRes[1] << 8 | pRes[2] << 16 | pRes[3] << 24);
    oFileDst.CloseFile();

    XmlSignature_DestroyFile(file);
    XmlSirnature_Free(res);
    XmlSignature_DestroyCertificate(cert);

    return 0;
}
#endif

