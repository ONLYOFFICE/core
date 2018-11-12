#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/pkcs12.h>

#include <openssl/sha.h>
#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/engine.h>
#include <openssl/evp.h>
#include <openssl/conf.h>

#include "../../../common/File.h"
#include "../../../common/StringExt.h"
#include "opensslwrapper.h"

class COpenssl_library_private
{
private:
    static bool m_bInit;

public:
    EVP_PKEY* m_key;

public:
    COpenssl_library_private()
    {
        if (!m_bInit)
        {
            SSL_library_init();
            OpenSSL_add_all_algorithms();
            ERR_load_BIO_strings();
            ERR_load_crypto_strings();
            m_bInit = true;
        }
    }
    ~COpenssl_library_private()
    {
        ENGINE_cleanup();
        CONF_modules_unload(1);
        ERR_free_strings();
        EVP_cleanup();
        CRYPTO_cleanup_all_ex_data();
    }
};

bool COpenssl_library_private::m_bInit = false;

std::string GetOpenSslErrors()
{
    BIO* bio = BIO_new(BIO_s_mem());
    ERR_print_errors(bio);
    char *buf = NULL;
    size_t len = BIO_get_mem_data(bio, &buf);
    std::string sRet((char*)buf, len);
    NSStringExt::ToLower(sRet);
    BIO_free (bio);
    return sRet;
}
bool IsOpenSslPasswordError(const std::string& str)
{
    if (std::string::npos != str.find("mac verify error"))
        return true;
    if (std::string::npos != str.find("mac verify failure"))
        return true;
    if (std::string::npos != str.find("password"))
        return true;
    return false;
}

COpenssl_library::COpenssl_library()
{
    m_internal = new COpenssl_library_private();
}

COpenssl_library::~COpenssl_library()
{
    RELEASEOBJECT(m_internal);
}

int COpenssl_library::LoadKey(std::wstring file, std::string password)
{
    BYTE* pData = NULL;
    DWORD dwDataLen;
    if (!NSFile::CFileBinary::ReadAllBytes(file, &pData, dwDataLen))
        return OPEN_SSL_WARNING_ERR;

    EVP_PKEY* pKey = NULL;
    char* pPassword = (password.empty()) ? NULL : (char*)password.c_str();

    BIO* bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
    if (PEM_read_bio_PrivateKey(bio, &pKey, NULL, (void*)pPassword))
    {
        EVP_PKEY_free(pKey);
        BIO_free(bio);
        return OPEN_SSL_WARNING_OK;
    }
    std::string sError = GetOpenSslErrors();
    if (IsOpenSslPasswordError(sError))
    {
        EVP_PKEY_free(pKey);
        BIO_free(bio);
        return OPEN_SSL_WARNING_PASS;
    }

    BIO_free(bio);
    bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
    if (d2i_PrivateKey_bio(bio, &pKey))
    {
        EVP_PKEY_free(pKey);
        BIO_free(bio);
        return OPEN_SSL_WARNING_OK;
    }
    sError = GetOpenSslErrors();
    if (IsOpenSslPasswordError(sError))
    {
        EVP_PKEY_free(pKey);
        BIO_free(bio);
        return OPEN_SSL_WARNING_PASS;
    }

    BIO_free(bio);
    bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
    if (d2i_PKCS8PrivateKey_bio(bio, &pKey, NULL, (void*)pPassword))
    {
        EVP_PKEY_free(pKey);
        BIO_free(bio);
        return OPEN_SSL_WARNING_OK;
    }
    sError = GetOpenSslErrors();
    if (IsOpenSslPasswordError(sError))
    {
        EVP_PKEY_free(pKey);
        BIO_free(bio);
        return OPEN_SSL_WARNING_PASS;
    }

    BIO_free(bio);
    bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);

    PKCS12* p12 = d2i_PKCS12_bio(bio, NULL);
    if (p12)
    {
        X509* pCert = NULL;
        STACK_OF(X509)* pCa = NULL;

        if (PKCS12_parse(p12, pPassword, &pKey, &pCert, &pCa))
        {
            sk_X509_pop_free(pCa, X509_free);
            X509_free(pCert);
            EVP_PKEY_free(pKey);
            PKCS12_free(p12);
            BIO_free(bio);
            return OPEN_SSL_WARNING_ALL_OK;
        }

        sError = GetOpenSslErrors();
        if (IsOpenSslPasswordError(sError))
        {
            PKCS12_free(p12);
            BIO_free(bio);
            return OPEN_SSL_WARNING_PASS;
        }

        PKCS12_free(p12);
    }

    BIO_free(bio);

    return OPEN_SSL_WARNING_ERR;
}

int COpenssl_library::LoadCert(std::wstring file, std::string password)
{
    BYTE* pData = NULL;
    DWORD dwDataLen;
    if (!NSFile::CFileBinary::ReadAllBytes(file, &pData, dwDataLen))
        return OPEN_SSL_WARNING_ERR;

    X509* pCert = NULL;
    char* pPassword = (password.empty()) ? NULL : (char*)password.c_str();

    BIO* bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
    if (PEM_read_bio_X509(bio, &pCert, NULL, (void*)pPassword))
    {
        X509_free(pCert);
        BIO_free(bio);
        return OPEN_SSL_WARNING_OK;
    }
    std::string sError = GetOpenSslErrors();
    if (IsOpenSslPasswordError(sError))
    {
        X509_free(pCert);
        BIO_free(bio);
        return OPEN_SSL_WARNING_PASS;
    }

    BIO_free(bio);
    bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
    if (d2i_X509_bio(bio, &pCert))
    {
        X509_free(pCert);
        BIO_free(bio);
        return OPEN_SSL_WARNING_OK;
    }
    sError = GetOpenSslErrors();
    if (IsOpenSslPasswordError(sError))
    {
        X509_free(pCert);
        BIO_free(bio);
        return OPEN_SSL_WARNING_PASS;
    }

    BIO_free(bio);
    bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);

    PKCS12* p12 = d2i_PKCS12_bio(bio, NULL);
    if (p12)
    {
        EVP_PKEY* pKey = NULL;
        STACK_OF(X509)* pCa = NULL;

        if (PKCS12_parse(p12, pPassword, &pKey, &pCert, &pCa))
        {
            sk_X509_pop_free(pCa, X509_free);
            X509_free(pCert);
            EVP_PKEY_free(pKey);
            PKCS12_free(p12);
            BIO_free(bio);
            return OPEN_SSL_WARNING_ALL_OK;
        }

        sError = GetOpenSslErrors();
        if (IsOpenSslPasswordError(sError))
        {
            PKCS12_free(p12);
            BIO_free(bio);
            return OPEN_SSL_WARNING_PASS;
        }

        PKCS12_free(p12);
    }

    BIO_free(bio);
    return OPEN_SSL_WARNING_ERR;
}
