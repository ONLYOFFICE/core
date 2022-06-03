#include "./../include/PDFSigner.h"
#include "../../../common/File.h"

#include <openssl/crypto.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/err.h>
#include <openssl/md5.h>

class CPDFSigner_private
{
private:
    std::wstring m_sCertFile;
    std::string m_sCertPassword;

public:
    // PFX сертификат и закрытый ключ/пароль (публичный ключ встроен в сертификат)
    CPDFSigner_private(const std::wstring& sCertFile, const std::string& sCertPassword)
    {
        m_sCertFile = sCertFile;
        m_sCertPassword = sCertPassword;
    }
    // CRT сертификат и ключ
    CPDFSigner_private(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
    {
    }

    void Sign(BYTE* pDataForSignature, DWORD dwLenDataForSignature, BYTE*& pDataForWrite, DWORD& dwLenDataForWrite)
    {
        FILE* pfx_file = NSFile::CFileBinary::OpenFileNative(m_sCertFile, L"rb");
        if (!pfx_file)
            return;

        // TODO разобраться, изучить и понять

        ERR_load_crypto_strings();
        OpenSSL_add_all_algorithms();

        PKCS12* pkcs12 = d2i_PKCS12_fp(pfx_file, NULL);
        fclose(pfx_file);
        if (!pkcs12)
            return;

        EVP_PKEY* pkey = NULL;
        X509* cert = NULL;
        STACK_OF(X509)* ca = NULL;
        if (!PKCS12_parse(pkcs12, m_sCertPassword.c_str(), &pkey, &cert, &ca))
        {
            PKCS12_free(pkcs12);
            return;
        }
        PKCS12_free(pkcs12);

        dwLenDataForWrite = 15000;
        DWORD dsLenSignedData = 0;
        pDataForWrite = new BYTE[dwLenDataForWrite];
        memset(pDataForWrite, 0, dwLenDataForWrite);

        BIO* inputbio = BIO_new(BIO_s_mem());
        BIO_write(inputbio, pDataForSignature, dwLenDataForSignature);
        PKCS7* pkcs7 = PKCS7_sign(cert, pkey, NULL, inputbio, PKCS7_DETACHED | PKCS7_BINARY);
        BIO_free(inputbio);
        if (!pkcs7)
            return;

        BIO* outputbio = BIO_new(BIO_s_mem());
        i2d_PKCS7_bio(outputbio, pkcs7);
        BUF_MEM* mem = NULL;
        BIO_get_mem_ptr(outputbio, &mem);
        if (mem && mem->data && mem->length)
        {
            dsLenSignedData = mem->length;
            memcpy(pDataForWrite, mem->data, dsLenSignedData);
        }
        BIO_free(outputbio);
        PKCS7_free(pkcs7);

        EVP_cleanup();
    }
};

CPDFSigner::CPDFSigner(const std::wstring& sCertFile, const std::string& sCertPassword)
{
    m_internal = new CPDFSigner_private(sCertFile, sCertPassword);
}

CPDFSigner::CPDFSigner(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
{
    m_internal = new CPDFSigner_private(keyPath, keyPassword, certPath, certPassword);
}

CPDFSigner::~CPDFSigner()
{
    RELEASEOBJECT(m_internal);
}

void CPDFSigner::Sign(BYTE* pDataForSignature, DWORD dwLenDataForSignature, BYTE*& pDataForWrite, DWORD& dwLenDataForWrite)
{
    m_internal->Sign(pDataForSignature, dwLenDataForSignature, pDataForWrite, dwLenDataForWrite);
}
