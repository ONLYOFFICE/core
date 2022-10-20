#ifdef _WIN32
#include "./Certificate_mscrypto.h"
#endif

#include "./Certificate_openssl.h"

#include "./../include/CertificateCommon.h"

namespace NSOpenSSL
{
    int LoadKey(std::wstring file, std::string password)
    {
        return CCertificate_openssl::LoadKey(file, password, NULL);
    }

    int LoadCert(std::wstring file, std::string password)
    {
        return CCertificate_openssl::LoadCert(file, password, NULL);
    }

    int LoadKeyRaw(unsigned char* data, unsigned int len, std::string password)
    {
        return CCertificate_openssl::LoadKey(data, (DWORD)len, password, NULL);
    }
    int LoadCertRaw(unsigned char* data, unsigned int len, std::string password)
    {
        return CCertificate_openssl::LoadCert(data, (DWORD)len, password, NULL);
    }
}

namespace NSCertificate
{
    CCertificateInfo GetDefault()
    {
        CCertificateInfo info;

    #ifdef _WIN32
        // detect user name
        std::wstring sUserName;

        DWORD dwUserNameLen = 1024;
        wchar_t* _name = new wchar_t[dwUserNameLen + 1];
        GetUserNameW(_name, &dwUserNameLen);
        sUserName = std::wstring(_name);
        delete []_name;

        HANDLE hStoreHandle = CertOpenSystemStoreA(NULL, "MY");
        if (!hStoreHandle)
            return info;

        PCCERT_CONTEXT pCertContext = NULL;

        while (pCertContext = CertEnumCertificatesInStore(hStoreHandle, pCertContext))
        {
            CCertificate_mscrypto _cert(pCertContext);
            if (sUserName == _cert.GetSignerName())
            {
                info = NSCertificate::GetInfo(&_cert);
            }
        }

        CertCloseStore(hStoreHandle, 0);
    #endif

        return info;
    }
    ICertificate* GetById(const std::string& id)
    {
    #ifdef _WIN32
        HANDLE hStoreHandle = CertOpenSystemStoreA(NULL, "MY");
        if (!hStoreHandle)
            return NULL;

        PCCERT_CONTEXT pCertContext = NULL;

        while (pCertContext = CertEnumCertificatesInStore(hStoreHandle, pCertContext))
        {
            CCertificate_mscrypto* _cert = new CCertificate_mscrypto(pCertContext);
            if (id == _cert->GetId())
            {
                _cert->m_release = true;
                return _cert;
            }

            RELEASEOBJECT(_cert);
        }

        CertCloseStore(hStoreHandle, 0);
    #else

        CCertificate_openssl* pCertificate = (CCertificate_openssl*)CreateInstance(CERTIFICATE_ENGINE_TYPE_OPENSSL);
        if (pCertificate->FromKey(id))
            return pCertificate;
        delete pCertificate;

    #endif

        return NULL;
    }

    CCertificateInfo GetInfo(ICertificate* pCert)
    {
        CCertificateInfo info;
        info.Name    = pCert->GetSignerName();
        info.Date    = pCert->GetDate();
        info.Id      = pCert->GetId();
        info.RawData = pCert->GetCertificateBase64();
        return info;
    }

    ICertificate* FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
    {
        CCertificate_openssl* pCert = new CCertificate_openssl();
        if (pCert->FromFiles(keyPath, keyPassword, certPath, certPassword))
            return pCert;
        RELEASEOBJECT(pCert);
        return NULL;
    }

    ICertificate* FromFilesRaw(unsigned char* key, unsigned int keyLen, const std::string& keyPassword,
                               unsigned char* cert, unsigned int certLen, const std::string& certPassword)
    {
        CCertificate_openssl* pCert = new CCertificate_openssl();
        if (pCert->FromFilesRaw(key, keyLen, keyPassword, cert, certLen, certPassword))
            return pCert;
        RELEASEOBJECT(pCert);
        return NULL;
    }

    int GetOOXMLHashAlg(const std::string& sAlg)
    {
        if ("http://www.w3.org/2000/09/xmldsig#rsa-sha1" == sAlg ||
            "http://www.w3.org/2000/09/xmldsig#sha1" == sAlg)
            return OOXML_HASH_ALG_SHA1;

        /*
        if ("http://www.w3.org/2001/04/xmldsig-more#rsa-sha224" == sAlg ||
            "http://www.w3.org/2001/04/xmldsig-more#sha224" == sAlg)
            return OOXML_HASH_ALG_SHA224;
        */

        if ("http://www.w3.org/2001/04/xmldsig-more#rsa-sha256" == sAlg ||
            "http://www.w3.org/2001/04/xmldsig-more#sha256" == sAlg ||
            "http://www.w3.org/2001/04/xmlenc#sha256" == sAlg)
            return OOXML_HASH_ALG_SHA256;

        if ("http://www.w3.org/2001/04/xmldsig-more#rsa-sha384" == sAlg ||
            "http://www.w3.org/2001/04/xmldsig-more#sha384" == sAlg)
            return OOXML_HASH_ALG_SHA384;

        if ("http://www.w3.org/2001/04/xmldsig-more#rsa-sha512" == sAlg ||
            "http://www.w3.org/2001/04/xmldsig-more#sha512" == sAlg ||
            "http://www.w3.org/2001/04/xmlenc#sha512" == sAlg)
            return OOXML_HASH_ALG_SHA512;

        if ("urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr3411" == sAlg ||
            "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34102001-gostr3411" == sAlg)
            return OOXML_HASH_ALG_GOST_GR3411;

        if ("urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34112012-256" == sAlg ||
            "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34102012-gostr34112012-256" == sAlg)
            return OOXML_HASH_ALG_GOST_GR3411_2012_256;

        if ("urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34112012-512" == sAlg ||
            "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34102012-gostr34112012-512" == sAlg)
            return OOXML_HASH_ALG_GOST_GR3411_2012_512;

        return OOXML_HASH_ALG_INVALID;
    }
    std::string GetDigestMethodA(const int& nAlg)
    {
        switch (nAlg)
        {
        case OOXML_HASH_ALG_SHA1:
            return "http://www.w3.org/2000/09/xmldsig#sha1";
        case OOXML_HASH_ALG_SHA224:
            return "http://www.w3.org/2001/04/xmldsig-more#sha224";
        case OOXML_HASH_ALG_SHA256:
            return "http://www.w3.org/2001/04/xmlenc#sha256";
        case OOXML_HASH_ALG_SHA384:
            return "http://www.w3.org/2001/04/xmldsig-more#sha384";
        case OOXML_HASH_ALG_SHA512:
            return "http://www.w3.org/2001/04/xmlenc#sha512";
        case OOXML_HASH_ALG_GOST_GR3411:
            return "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr3411";
        case OOXML_HASH_ALG_GOST_GR3411_2012_256:
            return "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34112012-256";
        case OOXML_HASH_ALG_GOST_GR3411_2012_512:
            return "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34112012-512";
        case OOXML_HASH_ALG_ED25519:
        case OOXML_HASH_ALG_ED448:
            return "http://www.w3.org/2001/04/xmlenc#sha512";
        default:
            break;
        }
        return "http://www.w3.org/2000/09/xmldsig#sha1";
    }
    std::wstring GetDigestMethod(const int& nAlg)
    {
        std::string sTmp = GetDigestMethodA(nAlg);
        return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(sTmp);
    }
    std::string GetSignatureMethodA(const int& nAlg)
    {
        switch (nAlg)
        {
        case OOXML_HASH_ALG_SHA1:
            return "http://www.w3.org/2000/09/xmldsig#rsa-sha1";
        case OOXML_HASH_ALG_SHA224:
            return "http://www.w3.org/2001/04/xmldsig-more#rsa-sha224";
        case OOXML_HASH_ALG_SHA256:
            return "http://www.w3.org/2001/04/xmldsig-more#rsa-sha256";
        case OOXML_HASH_ALG_SHA384:
            return "http://www.w3.org/2001/04/xmldsig-more#rsa-sha384";
        case OOXML_HASH_ALG_SHA512:
            return "http://www.w3.org/2001/04/xmldsig-more#rsa-sha512";
        case OOXML_HASH_ALG_GOST_GR3411:
            return "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34102001-gostr3411";
        case OOXML_HASH_ALG_GOST_GR3411_2012_256:
            return "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34102012-gostr34112012-256";
        case OOXML_HASH_ALG_GOST_GR3411_2012_512:
            return "urn:ietf:params:xml:ns:cpxmlsec:algorithms:gostr34102012-gostr34112012-512";
        case OOXML_HASH_ALG_ED25519:
        case OOXML_HASH_ALG_ED448:
            return "http://www.w3.org/2001/04/xmldsig-more#rsa-sha512";
        default:
            break;
        }
        return "http://www.w3.org/2000/09/xmldsig#rsa-sha1";
    }
    std::wstring GetSignatureMethod(const int& nAlg)
    {
        std::string sTmp = GetSignatureMethodA(nAlg);
        return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(sTmp);
    }

    ICertificate* CreateInstance(const int& type)
    {
        int typeCreate = type;

        if (typeCreate == CERTIFICATE_ENGINE_TYPE_DEFAULT)
        {
#ifdef _WIN32
            typeCreate = CERTIFICATE_ENGINE_TYPE_MSCRYPTO;
#else
            typeCreate = CERTIFICATE_ENGINE_TYPE_OPENSSL;
#endif
        }

        if (typeCreate == CERTIFICATE_ENGINE_TYPE_OPENSSL)
            return new CCertificate_openssl();

#ifdef _WIN32
        if (typeCreate == CERTIFICATE_ENGINE_TYPE_MSCRYPTO)
            return new CCertificate_mscrypto();
#endif

        return NULL;
    }
};

bool CCertificate_openssl::g_is_initialize = false;
