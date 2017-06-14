#include "./XmlSigner_openssl.h"

#include "../../../common/File.h"
#include "../../../common/String.h"
#include "../../../common/BigInteger.h"

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

void BIO_FREE(BIO*& bio)
{
    if (bio)
    {
        BIO_free(bio);
        bio = NULL;
    }
}
void EVP_PKEY_FREE(EVP_PKEY*& key)
{
    if (key)
    {
        EVP_PKEY_free(key);
        key = NULL;
    }
}
void X509_FREE(X509*& cert)
{
    if (cert)
    {
        X509_free(cert);
        cert = NULL;
    }
}

class CCertificate_openssl_private
{
protected:
    ICertificateSelectDialogOpenSsl* m_pDialog;

    X509*           m_cert;
    EVP_PKEY*       m_key;

public:
    ICertificate* m_pBase;

public:
    CCertificate_openssl_private()
    {
        m_pDialog = NULL;

        m_cert = NULL;
        m_key = NULL;

        m_pBase = NULL;
    }
    virtual ~CCertificate_openssl_private()
    {
        if (NULL != m_cert)
            X509_FREE(m_cert);
        if (NULL != m_key)
            EVP_PKEY_FREE(m_key);
    }

public:
    std::string GetNumber()
    {
        if (NULL == m_cert)
            return "";

        ASN1_INTEGER* asn1_serial = X509_get_serialNumber(m_cert);
        if (asn1_serial == NULL)
            return "";

        BIGNUM* bn = ASN1_INTEGER_to_BN(asn1_serial, NULL);
        if (!bn)
        {
            ASN1_INTEGER_free(asn1_serial);
            return "";
        }

        char *tmp = BN_bn2dec(bn);
        std::string sReturn(tmp);

        BN_free(bn);

        return sReturn;
    }

    std::wstring GetSignerName()
    {
        if (NULL == m_cert)
            return L"";

        X509_NAME* name = X509_get_issuer_name(m_cert);
        char buffer[1024];
        memset(buffer, 0, 1025);

        X509_NAME_oneline(name, buffer, 1024);

        std::string sName(buffer);
        std::wstring sNameW = UTF8_TO_U(sName);

        return sNameW;
    }

    std::string GetCertificateBase64()
    {
        if (NULL == m_cert)
            return "";

        BIO* bio = BIO_new(BIO_s_mem());
        PEM_write_bio_X509_AUX(bio, m_cert);

        unsigned char *data;
        unsigned int len = 0;
        len = BIO_get_mem_data(bio, &data);

        std::string sReturn((char*)data, (size_t)len);

        BIO_free(bio);
        return sReturn;
    }

    std::string GetCertificateHash()
    {
        std::string sBase64 = GetCertificateBase64();
        BYTE* pData = NULL;
        int nLen = 0;
        if (NSFile::CBase64Converter::Decode(sBase64.c_str(), (int)sBase64.length(), pData, nLen))
        {
            std::string sHash = GetHash(pData, (unsigned int)nLen, OOXML_HASH_ALG_SHA1);
            RELEASEARRAYOBJECTS(pData);
            return sHash;
        }

        return "";
    }

    std::string GetDate()
    {
        if (NULL == m_cert)
            return "";

        ASN1_TIME* _time1 = X509_get_notBefore(m_cert);
        struct tm t1 = this->ASN1_GetTimeT(_time1);

        ASN1_TIME* _time2 = X509_get_notAfter(m_cert);
        struct tm t2 = this->ASN1_GetTimeT(_time2);

        std::string sRet = std::to_string(t1.tm_mday) +
                "/" +
                std::to_string(t1.tm_mon + 1) +
                "/" +
                std::to_string(t1.tm_year + 1900) +
                " - " +
                std::to_string(t1.tm_mday) +
                "/" +
                std::to_string(t2.tm_mon + 1) +
                "/" +
                std::to_string(t2.tm_year + 1900);

        return sRet;
    }

    std::string GetId()
    {
        // TODO: + public key?
        return GetNumber();
    }

public:
    std::string Sign(const std::string& sXml)
    {
        EVP_MD_CTX* pCtx = EVP_MD_CTX_create();
        const EVP_MD* pDigest = EVP_sha1();

        int n1 = EVP_SignInit(pCtx, pDigest);
        n1 = n1;

        int n2 = EVP_SignUpdate(pCtx, sXml.c_str(), sXml.length());
        n2 = n2;

        BYTE pSignature[4096];
        unsigned int nSignatureLen = 0;

        int n3 = EVP_SignFinal(pCtx, pSignature, &nSignatureLen, m_key);
        n3 = n3;

        EVP_MD_CTX_destroy(pCtx);

        return std::string((char*)pSignature, (size_t)nSignatureLen);
    }

    std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg)
    {
        if (nAlg == OOXML_HASH_ALG_SHA1)
        {
            unsigned char obuf[20];
            SHA1(pData, (size_t)nSize, obuf);

            char* pBase64_hash = NULL;
            int nBase64Len_hash = 0;
            NSFile::CBase64Converter::Encode(obuf, 20, pBase64_hash, nBase64Len_hash, NSBase64::B64_BASE64_FLAG_NOCRLF);

            std::string sReturn(pBase64_hash, nBase64Len_hash);
            delete [] pBase64_hash;

            return sReturn;
        }
        return "";
    }

    std::string GetHash(const std::string& sXml, int nAlg)
    {
        return GetHash((BYTE*)sXml.c_str(), (DWORD)sXml.length(), nAlg);
    }

    std::string GetHash(const std::wstring& sXmlFile, int nAlg)
    {
        BYTE* pFileData = NULL;
        DWORD dwFileDataLen = 0;
        NSFile::CFileBinary::ReadAllBytes(sXmlFile, &pFileData, dwFileDataLen);

        if (0 == dwFileDataLen)
            return "";

        std::string sReturn = GetHash(pFileData, dwFileDataLen, nAlg);

        RELEASEARRAYOBJECTS(pFileData);
        return sReturn;
    }

    bool Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg)
    {
        EVP_MD_CTX* pCtx = EVP_MD_CTX_create();
        const EVP_MD* pDigest = EVP_sha1();

        int n1 = EVP_VerifyInit(pCtx, pDigest);
        n1 = n1;

        BYTE* pDigestValue = NULL;
        int nDigestLen = 0;
        NSFile::CBase64Converter::Decode(sXmlSignature.c_str(), (int)sXmlSignature.length(), pDigestValue, nDigestLen);

        int n2 = EVP_VerifyUpdate(pCtx, pDigestValue, (size_t)nDigestLen);
        n2 = n2;

        EVP_PKEY* pubkey = X509_get_pubkey(m_cert);

        int n3 = EVP_VerifyFinal(pCtx, (BYTE*)sXml.c_str(), (unsigned int)sXml.length(), pubkey);
        n3 = n3;

        EVP_MD_CTX_destroy(pCtx);
        EVP_PKEY_FREE(pubkey);

        RELEASEARRAYOBJECTS(pDigestValue);

        return (1 == n3) ? true : false;
    }

    bool LoadFromBase64Data(const std::string& data)
    {
        BYTE* pData = NULL;
        int nLen = 0;
        if (NSFile::CBase64Converter::Decode(data.c_str(), (int)data.length(), pData, nLen))
        {
            X509* pCert = NULL;
            int nErr = LoadCert(pData, (DWORD)nLen, "", &pCert);

            if (nErr == OPEN_SSL_WARNING_OK || nErr == OPEN_SSL_WARNING_ALL_OK)
            {
                m_cert = pCert;
            }
            else
            {
                X509_FREE(pCert);
                m_cert = NULL;
            }

            RELEASEARRAYOBJECTS(pData);
            return (NULL == m_cert) ? false : true;
        }
        return false;
    }

public:
    bool ShowSelectDialog()
    {
        if (!m_pDialog)
            return false;

        bool bResult = m_pDialog->ShowSelectDialog();

        std::wstring sKeyPath = m_pDialog->GetKeyPath();
        std::wstring sKeyPasswordW = m_pDialog->GetKeyPassword();
        std::string sKeyPassword = U_TO_UTF8(sKeyPasswordW);

        std::wstring sCertPath = m_pDialog->GetCertificatePath();
        std::wstring sCertPasswordW = m_pDialog->GetCertificatePassword();
        std::string sCertPassword = U_TO_UTF8(sCertPasswordW);

        if (sCertPath.empty())
        {
            sCertPath = sKeyPath;
            sCertPassword = sKeyPassword;
        }

        int nErr = LoadKey(sKeyPath, sKeyPassword, &m_key);

        if (nErr != OPEN_SSL_WARNING_OK && nErr != OPEN_SSL_WARNING_ALL_OK)
            return false;

        nErr = LoadCert(sCertPath, sCertPassword, &m_cert);

        if (nErr != OPEN_SSL_WARNING_OK && nErr != OPEN_SSL_WARNING_ALL_OK)
            return false;

        return true;
    }
    int ShowCertificate()
    {
        if (m_pDialog)
            return m_pDialog->ShowCertificate(m_pBase);
        return 1;
    }

    void SetOpenSslDialog(ICertificateSelectDialogOpenSsl* pDialog)
    {
        m_pDialog = pDialog;
    }

protected:
    tm ASN1_GetTimeT(ASN1_TIME* time)
    {
        struct tm t;
        const char* str = (const char*) time->data;
        size_t i = 0;

        memset(&t, 0, sizeof(t));

        if (time->type == V_ASN1_UTCTIME)
        {
            /* two digit year */
            t.tm_year = (str[i++] - '0') * 10;
            t.tm_year += (str[i++] - '0');
            if (t.tm_year < 70)
                t.tm_year += 100;
        }
        else if (time->type == V_ASN1_GENERALIZEDTIME)
        {
            /* four digit year */
            t.tm_year = (str[i++] - '0') * 1000;
            t.tm_year+= (str[i++] - '0') * 100;
            t.tm_year+= (str[i++] - '0') * 10;
            t.tm_year+= (str[i++] - '0');
            t.tm_year -= 1900;
        }
        t.tm_mon  = (str[i++] - '0') * 10;
        t.tm_mon += (str[i++] - '0') - 1; // -1 since January is 0 not 1.
        t.tm_mday = (str[i++] - '0') * 10;
        t.tm_mday+= (str[i++] - '0');
        t.tm_hour = (str[i++] - '0') * 10;
        t.tm_hour+= (str[i++] - '0');
        t.tm_min  = (str[i++] - '0') * 10;
        t.tm_min += (str[i++] - '0');
        t.tm_sec  = (str[i++] - '0') * 10;
        t.tm_sec += (str[i++] - '0');

        /* Note: we did not adjust the time based on time zone information */
        return t;
    }

public:
    static std::string GetOpenSslErrors()
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
    static bool IsOpenSslPasswordError(const std::string& str)
    {
        if (std::string::npos != str.find("mac verify error"))
            return true;
        if (std::string::npos != str.find("mac verify failure"))
            return true;
        if (std::string::npos != str.find("password"))
            return true;
        return false;
    }

    static int LoadKey(BYTE* pData, DWORD dwDataLen, std::string password, EVP_PKEY** ppKey)
    {
        int nErr = OPEN_SSL_WARNING_ERR;
        std::string sError = "";

        PKCS12* p12 = NULL;
        EVP_PKEY* pKey = NULL;
        char* pPassword = (password.empty()) ? NULL : (char*)password.c_str();

        BIO* bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
        if (PEM_read_bio_PrivateKey(bio, &pKey, NULL, (void*)pPassword))
        {
            nErr = OPEN_SSL_WARNING_OK;
            goto end;
        }
        sError = GetOpenSslErrors();
        if (IsOpenSslPasswordError(sError))
        {
            nErr = OPEN_SSL_WARNING_PASS;
            goto end;
        }

        BIO_FREE(bio);
        bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
        if (d2i_PrivateKey_bio(bio, &pKey))
        {
            nErr = OPEN_SSL_WARNING_OK;
            goto end;
        }
        sError = GetOpenSslErrors();
        if (IsOpenSslPasswordError(sError))
        {
            nErr = OPEN_SSL_WARNING_PASS;
            goto end;
        }

        BIO_FREE(bio);
        bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
        if (d2i_PKCS8PrivateKey_bio(bio, &pKey, NULL, (void*)pPassword))
        {
            nErr = OPEN_SSL_WARNING_OK;
            goto end;
        }
        sError = GetOpenSslErrors();
        if (IsOpenSslPasswordError(sError))
        {
            nErr = OPEN_SSL_WARNING_PASS;
            goto end;
        }

        BIO_FREE(bio);
        bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);

        p12 = d2i_PKCS12_bio(bio, NULL);
        if (p12)
        {
            X509* pCert = NULL;
            STACK_OF(X509)* pCa = NULL;

            if (PKCS12_parse(p12, pPassword, &pKey, &pCert, &pCa))
            {
                sk_X509_pop_free(pCa, X509_free);
                X509_FREE(pCert);
                PKCS12_free(p12);
                nErr = OPEN_SSL_WARNING_ALL_OK;
                goto end;
            }
            PKCS12_free(p12);

            sError = GetOpenSslErrors();
            if (IsOpenSslPasswordError(sError))
            {
                nErr = OPEN_SSL_WARNING_PASS;
                goto end;
            }
        }

end:
        if (NULL == ppKey)
            EVP_PKEY_FREE(pKey);
        else
            *ppKey = pKey;

        BIO_FREE(bio);
        return nErr;
    }

    static int LoadKey(std::wstring file, std::string password, EVP_PKEY** ppKey)
    {
        BYTE* pData = NULL;
        DWORD dwDataLen;
        if (!NSFile::CFileBinary::ReadAllBytes(file, &pData, dwDataLen))
            return OPEN_SSL_WARNING_ERR;

        int nErr = LoadKey(pData, dwDataLen, password, ppKey);
        RELEASEARRAYOBJECTS(pData);
        return nErr;
    }

    static int LoadCert(BYTE* pData, DWORD dwDataLen, std::string password, X509** ppCert)
    {
        int nErr = OPEN_SSL_WARNING_ERR;
        std::string sError = "";

        PKCS12* p12 = NULL;
        X509* pCert = NULL;
        char* pPassword = (password.empty()) ? NULL : (char*)password.c_str();

        BIO* bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
        if (PEM_read_bio_X509(bio, &pCert, NULL, (void*)pPassword))
        {
            nErr = OPEN_SSL_WARNING_OK;
            goto end;
        }
        sError = GetOpenSslErrors();
        if (IsOpenSslPasswordError(sError))
        {
            nErr = OPEN_SSL_WARNING_PASS;
            goto end;
        }

        BIO_FREE(bio);
        bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);
        if (d2i_X509_bio(bio, &pCert))
        {
            nErr = OPEN_SSL_WARNING_OK;
            goto end;
        }
        sError = GetOpenSslErrors();
        if (IsOpenSslPasswordError(sError))
        {
            nErr = OPEN_SSL_WARNING_PASS;
            goto end;
        }

        BIO_FREE(bio);
        bio = BIO_new_mem_buf((void*)pData, (int)dwDataLen);

        p12 = d2i_PKCS12_bio(bio, NULL);
        if (p12)
        {
            EVP_PKEY* pKey = NULL;
            STACK_OF(X509)* pCa = NULL;

            if (PKCS12_parse(p12, pPassword, &pKey, &pCert, &pCa))
            {
                sk_X509_pop_free(pCa, X509_free);
                EVP_PKEY_FREE(pKey);
                PKCS12_free(p12);
                BIO_FREE(bio);
                nErr = OPEN_SSL_WARNING_ALL_OK;
                goto end;
            }

            PKCS12_free(p12);
            sError = GetOpenSslErrors();
            if (IsOpenSslPasswordError(sError))
            {
                nErr = OPEN_SSL_WARNING_PASS;
                goto end;
            }
        }

end:
        if (NULL == ppCert)
            X509_FREE(pCert);
        else
            *ppCert = pCert;

        BIO_FREE(bio);
        return nErr;
    }

    static int LoadCert(std::wstring file, std::string password, X509** ppCert)
    {
        BYTE* pData = NULL;
        DWORD dwDataLen;
        if (!NSFile::CFileBinary::ReadAllBytes(file, &pData, dwDataLen))
            return OPEN_SSL_WARNING_ERR;

        int nErr = LoadCert(pData, dwDataLen, password, ppCert);
        RELEASEARRAYOBJECTS(pData);
        return nErr;
    }
};

// REALIZE
CCertificate_openssl::CCertificate_openssl() : ICertificate()
{
    m_internal = new CCertificate_openssl_private();
    m_internal->m_pBase = this;
}

CCertificate_openssl::~CCertificate_openssl()
{
    RELEASEOBJECT(m_internal);
}

std::string CCertificate_openssl::GetNumber()
{
    return m_internal->GetNumber();
}

std::wstring CCertificate_openssl::GetSignerName()
{
    return m_internal->GetSignerName();
}

std::string CCertificate_openssl::GetCertificateBase64()
{
    return m_internal->GetCertificateBase64();
}

std::string CCertificate_openssl::GetCertificateHash()
{
    return m_internal->GetCertificateHash();
}

std::string CCertificate_openssl::GetDate()
{
    return m_internal->GetDate();
}

std::string CCertificate_openssl::GetId()
{
    return m_internal->GetId();
}

std::string CCertificate_openssl::Sign(const std::string& sXml)
{
    return m_internal->Sign(sXml);
}

std::string CCertificate_openssl::GetHash(unsigned char* pData, unsigned int nSize, int nAlg)
{
    return m_internal->GetHash(pData, nSize, nAlg);
}

std::string CCertificate_openssl::GetHash(const std::string& sXml, int nAlg)
{
    return m_internal->GetHash(sXml, nAlg);
}

std::string CCertificate_openssl::GetHash(const std::wstring& sXmlFile, int nAlg)
{
    return m_internal->GetHash(sXmlFile, nAlg);
}

bool CCertificate_openssl::Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg)
{
    return m_internal->Verify(sXml, sXmlSignature, nAlg);
}

bool CCertificate_openssl::LoadFromBase64Data(const std::string& data)
{
    return m_internal->LoadFromBase64Data(data);
}

bool CCertificate_openssl::ShowSelectDialog()
{
    return m_internal->ShowSelectDialog();
}

int CCertificate_openssl::ShowCertificate()
{
    return m_internal->ShowCertificate();
}

void CCertificate_openssl::SetOpenSslDialog(ICertificateSelectDialogOpenSsl* pDialog)
{
    return m_internal->SetOpenSslDialog(pDialog);
}

int ICertificateSelectDialogOpenSsl::LoadKey(std::wstring file, std::string password)
{
    return CCertificate_openssl_private::LoadKey(file, password, NULL);
}

int ICertificateSelectDialogOpenSsl::LoadCert(std::wstring file, std::string password)
{
    return CCertificate_openssl_private::LoadCert(file, password, NULL);
}
