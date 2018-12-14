#include "./XmlSigner_openssl.h"

#include "../../../common/File.h"
#include "../../../common/StringExt.h"
#include "../../../common/BigInteger.h"

#include <unistd.h>
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

const EVP_MD* Get_EVP_MD(int nAlg)
{
    switch (nAlg)
    {
        case OOXML_HASH_ALG_SHA1:
        {
            return EVP_sha1();
        }
        case OOXML_HASH_ALG_SHA224:
        {
            return EVP_sha224();
        }
        case OOXML_HASH_ALG_SHA256:
        {
            return EVP_sha256();
        }
        case OOXML_HASH_ALG_SHA384:
        {
            return EVP_sha384();
        }
        case OOXML_HASH_ALG_SHA512:
        {
            return EVP_sha512();
        }
    default:
        break;
    }
    return EVP_sha1();
}

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
    X509*           m_cert;
    EVP_PKEY*       m_key;

    std::string     m_separator;
    std::string     m_id;

    int m_alg;

public:
    ICertificate* m_pBase;

public:
    CCertificate_openssl_private()
    {
        m_cert = NULL;
        m_key = NULL;

        m_pBase = NULL;
        m_separator = ";;;;;;;ONLYOFFICE;;;;;;;";
    }
    virtual ~CCertificate_openssl_private()
    {
        if (NULL != m_cert)
            X509_FREE(m_cert);
        if (NULL != m_key)
            EVP_PKEY_FREE(m_key);
    }

protected:
    static void string_replace(std::string& text, const std::string& replaceFrom, const std::string& replaceTo)
    {
        size_t posn = 0;
        while (std::string::npos != (posn = text.find(replaceFrom, posn)))
        {
            text.replace(posn, replaceFrom.length(), replaceTo);
            posn += replaceTo.length();
        }
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
        char buffer[10000];
        memset(buffer, 0, 10000);

        X509_NAME_oneline(name, buffer, 10000);

        std::string sName(buffer);
        std::wstring sNameW = UTF8_TO_U(sName);

        std::wstring::size_type pos = sNameW.find(L"CN=");
        if (std::wstring::npos != pos)
        {
            sNameW = sNameW.substr(pos + 3);
            pos = sNameW.find(L"/");

            if (std::wstring::npos != pos)
            {
                sNameW = sNameW.substr(0, pos);
            }
        }
        return sNameW;
    }

    std::wstring GetIssuerName()
    {
        return GetSignerName();
    }

    std::string GetCertificateBase64()
    {
        if (NULL == m_cert)
            return "";

#if 0
        BIO* bio = BIO_new(BIO_s_mem());
        PEM_write_bio_X509_AUX(bio, m_cert);

        unsigned char *data;
        unsigned int len = 0;
        len = BIO_get_mem_data(bio, &data);

        std::string sReturn((char*)data, (size_t)len);

        std::string sFindFirst = "-----BEGIN TRUSTED CERTIFICATE-----";
        std::string::size_type nPos1 = sReturn.find(sFindFirst);
        std::string::size_type nPos2 = sReturn.find("-----END TRUSTED CERTIFICATE-----");
        if (std::string::npos != nPos1 && std::string::npos != nPos2)
        {
            std::string::size_type nStart = nPos1 + sFindFirst.length();
            sReturn = sReturn.substr(nStart, nPos2 - nStart);
        }
        string_replace(sReturn, "\n", "");

        BIO_free(bio);
        return sReturn;
#endif

        BIO* bio = BIO_new(BIO_s_mem());

        i2d_X509_bio(bio, m_cert);
        BIO_flush(bio);

        BYTE* data = NULL;
        int size = (int)BIO_get_mem_data(bio, &data);

        char* dataDst = NULL;
        int lenDst = 0;
        NSFile::CBase64Converter::Encode(data, size, dataDst, lenDst);

        std::string sReturn(dataDst);

        RELEASEARRAYOBJECTS(dataDst);

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
            std::string sHash = GetHash(pData, (unsigned int)nLen, GetHashAlg());
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
        if (!m_id.empty())
            return m_id;
        return GetNumber();
    }

public:     
    std::string Sign(const std::string& sXml)
    {
        EVP_MD_CTX* pCtx = EVP_MD_CTX_create();
        const EVP_MD* pDigest = Get_EVP_MD(this->GetHashAlg());

        int n1 = EVP_SignInit(pCtx, pDigest);
        n1 = n1;

        int n2 = EVP_SignUpdate(pCtx, sXml.c_str(), sXml.length());
        n2 = n2;

        BYTE pSignature[4096];
        unsigned int nSignatureLen = 0;

        int n3 = EVP_SignFinal(pCtx, pSignature, &nSignatureLen, m_key);
        n3 = n3;

        EVP_MD_CTX_destroy(pCtx);

        char* pBase64 = NULL;
        int nBase64Len = 0;
        NSFile::CBase64Converter::Encode(pSignature, (int)nSignatureLen, pBase64, nBase64Len, NSBase64::B64_BASE64_FLAG_NONE);

        std::string sReturn(pBase64, nBase64Len);
        delete[] pBase64;

        return sReturn;
    }

    std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg)
    {
        int nBufLen = 0;
        unsigned char* pBufData = NULL;

        switch (nAlg)
        {
            case OOXML_HASH_ALG_SHA1:
            {
                nBufLen = 20;
                pBufData = new unsigned char[nBufLen];
                SHA1(pData, (size_t)nSize, pBufData);
                break;
            }
            case OOXML_HASH_ALG_SHA224:
            {
                nBufLen = 28;
                pBufData = new unsigned char[nBufLen];
                SHA224(pData, (size_t)nSize, pBufData);
                break;
            }
            case OOXML_HASH_ALG_SHA256:
            {
                nBufLen = 32;
                pBufData = new unsigned char[nBufLen];
                SHA256(pData, (size_t)nSize, pBufData);
                break;
            }
            case OOXML_HASH_ALG_SHA384:
            {
                nBufLen = 48;
                pBufData = new unsigned char[nBufLen];
                SHA384(pData, (size_t)nSize, pBufData);
                break;
            }
            case OOXML_HASH_ALG_SHA512:
            {
                nBufLen = 64;
                pBufData = new unsigned char[nBufLen];
                SHA512(pData, (size_t)nSize, pBufData);
                break;
            }
        default:
            break;
        }

        if (0 == nBufLen)
            return "";

        char* pBase64_hash = NULL;
        int nBase64Len_hash = 0;
        NSFile::CBase64Converter::Encode(pBufData, nBufLen, pBase64_hash, nBase64Len_hash, NSBase64::B64_BASE64_FLAG_NOCRLF);

        std::string sReturn(pBase64_hash, nBase64Len_hash);
        delete [] pBase64_hash;

        delete [] pBufData;

        return sReturn;
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
        const EVP_MD* pDigest = Get_EVP_MD(this->GetHashAlg());

        int n1 = EVP_VerifyInit(pCtx, pDigest);
        n1 = n1;

        BYTE* pDigestValue = NULL;
        int nDigestLen = 0;
        NSFile::CBase64Converter::Decode(sXmlSignature.c_str(), (int)sXmlSignature.length(), pDigestValue, nDigestLen);

        int n2 = EVP_VerifyUpdate(pCtx, (BYTE*)sXml.c_str(), (size_t)sXml.length());
        n2 = n2;

        EVP_PKEY* pubkey = X509_get_pubkey(m_cert);

        int n3 = EVP_VerifyFinal(pCtx, pDigestValue, (unsigned int)nDigestLen, pubkey);
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

    std::vector<int> GetHashAlgs()
    {
        std::vector<int> algs;
        if (!m_cert)
            return algs;

        // TODO:
        // Check algs in cert

        if (algs.empty())
            m_alg = OOXML_HASH_ALG_SHA1;
        else
            m_alg = algs[0];

        return algs;
    }
    int GetHashAlg()
    {
        if (m_alg == OOXML_HASH_ALG_INVALID)
            GetHashAlgs();
        return m_alg;
    }

public:
    int ShowSelectDialog()
    {
        return -1;
    }

    bool FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
    {
        std::wstring sKeyPath = keyPath;
        std::string sKeyPassword = keyPassword;
        std::wstring sCertPath = certPath;
        std::string sCertPassword = certPassword;

        if (sCertPath.empty())
        {
            sCertPath = sKeyPath;
            sCertPassword = sKeyPassword;
        }
        else if (sKeyPath.empty())
        {
            sKeyPath = sCertPath;
            sKeyPassword = sCertPassword;
        }

        int nErr = LoadKey(sKeyPath, sKeyPassword, &m_key);

        if (nErr != OPEN_SSL_WARNING_OK && nErr != OPEN_SSL_WARNING_ALL_OK)
            return false;

        nErr = LoadCert(sCertPath, sCertPassword, &m_cert);

        if (nErr != OPEN_SSL_WARNING_OK && nErr != OPEN_SSL_WARNING_ALL_OK)
            return false;

        m_id = U_TO_UTF8(sKeyPath);
        m_id += m_separator;
        m_id += sKeyPassword;
        m_id += m_separator;
        m_id += U_TO_UTF8(sCertPath);
        m_id += m_separator;
        m_id += sCertPassword;

        GetHashAlgs();
        return true;
    }

    bool FromKey(const std::string& sId)
    {
        std::string id = sId;
        std::vector<std::string> arr;
        size_t pos = 0;
        while ((pos = id.find(m_separator)) != std::string::npos)
        {
            arr.push_back(id.substr(0, pos));
            id.erase(0, pos + m_separator.length());
        }

        if (!id.empty())
            arr.push_back(id);
        else if (3 == arr.size())
            arr.push_back("");

        if (4 != arr.size())
            return false;

        std::string sKeyPathA = arr[0];
        std::string sKeyPasswordA = arr[1];
        std::string sCertPathA = arr[2];
        std::string sCertPasswordA = arr[3];

        std::wstring sKeyPath = UTF8_TO_U(sKeyPathA);
        std::wstring sCertPath = UTF8_TO_U(sCertPathA);

        return FromFiles(sKeyPath, sKeyPasswordA, sCertPath, sCertPasswordA);
    }

    int ShowCertificate()
    {
        return -1;
    }

    int VerifySelf()
    {
        return OPEN_SSL_WARNING_OK;

        if (NULL == m_cert)
            return OPEN_SSL_WARNING_NOVERIFY;

        X509_STORE_CTX* ctx = X509_STORE_CTX_new();
        X509_STORE* store = X509_STORE_new();

        X509_STORE_add_cert(store, m_cert);
        X509_STORE_CTX_init(ctx, store, m_cert, NULL);

        int status = X509_verify_cert(ctx);
        int nErr = X509_STORE_CTX_get_error(ctx);
        std::string sErr(X509_verify_cert_error_string(nErr));

        X509_STORE_free(store);
        X509_STORE_CTX_free(ctx);

        return (1 == status) ? OPEN_SSL_WARNING_OK : OPEN_SSL_WARNING_NOVERIFY;
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

std::wstring CCertificate_openssl::GetIssuerName()
{
    return m_internal->GetIssuerName();
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

int CCertificate_openssl::VerifySelf()
{
    return m_internal->VerifySelf();
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

int CCertificate_openssl::ShowSelectDialog(void* parent)
{
    return m_internal->ShowSelectDialog();
}

int CCertificate_openssl::ShowCertificate(void* parent)
{
    return m_internal->ShowCertificate();
}

bool CCertificate_openssl::FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
{
    return m_internal->FromFiles(keyPath, keyPassword, certPath, certPassword);
}

bool CCertificate_openssl::FromId(const std::string& id)
{
    return m_internal->FromKey(id);
}

std::vector<int> CCertificate_openssl::GetHashAlgs()
{
    return m_internal->GetHashAlgs();
}
int CCertificate_openssl::GetHashAlg()
{
    return m_internal->GetHashAlg();
}

bool CCertificate_openssl::IsGOST()
{
    return false;
}

namespace NSOpenSSL
{
    int LoadKey(std::wstring file, std::string password)
    {
        return CCertificate_openssl_private::LoadKey(file, password, NULL);
    }

    int LoadCert(std::wstring file, std::string password)
    {
        return CCertificate_openssl_private::LoadCert(file, password, NULL);
    }
}
