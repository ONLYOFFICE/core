#ifdef WIN32
#include "./XmlSigner_mscrypto.h"
#define CCertificate CCertificate_mscrypto

namespace NSOpenSSL
{
    int LoadKey(std::wstring file, std::string password)
    {
        return 0;
    }

    int LoadCert(std::wstring file, std::string password)
    {
        return 0;
    }
}

#endif

#include "../../../common/File.h"

#if defined(_LINUX) && !defined(_MAC)
#include "./XmlSigner_openssl.h"
#define XML_CERTIFICATE_USE_OPENSSL
#define CCertificate CCertificate_openssl
#endif

#ifdef _MAC
#include "./XmlSigner_openssl.h"
#define XML_CERTIFICATE_USE_OPENSSL
#define CCertificate CCertificate_openssl
#endif

int ICertificate::GetOOXMLHashAlg(const std::string& sAlg)
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

std::string ICertificate::GetDigestMethodA(const int& nAlg)
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
    default:
        break;
    }
    return "http://www.w3.org/2000/09/xmldsig#sha1";
}
std::wstring ICertificate::GetDigestMethod(const int& nAlg)
{
    std::string sTmp = GetDigestMethodA(nAlg);
    return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(sTmp);
}
std::string ICertificate::GetSignatureMethodA(const int& nAlg)
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
    default:
        break;
    }
    return "http://www.w3.org/2000/09/xmldsig#rsa-sha1";
}
std::wstring ICertificate::GetSignatureMethod(const int& nAlg)
{
    std::string sTmp = GetSignatureMethodA(nAlg);
    return NSFile::CUtf8Converter::GetUnicodeFromCharPtr(sTmp);
}

ICertificate* ICertificate::CreateInstance()
{
    ICertificate* pCert = new CCertificate();
    return pCert;
}

CCertificateInfo ICertificate::GetInfo()
{
    CCertificateInfo info;
    info.SetName(GetSignerName());
    info.SetDate(GetDate());
    info.SetId(GetId());
    info.SetRawBase64(GetCertificateBase64());
    return info;
}

bool ICertificate::FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
{
    return false;
}

CCertificateInfo ICertificate::GetDefault()
{
    CCertificateInfo info;

#ifdef WIN32
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
            info = _cert.GetInfo();
        }
    }

    CertCloseStore(hStoreHandle, 0);
#endif

    return info;
}

ICertificate* ICertificate::GetById(const std::string& id)
{
#ifdef WIN32
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
#endif

#ifdef XML_CERTIFICATE_USE_OPENSSL

    CCertificate_openssl* pCertificate = (CCertificate_openssl*)CreateInstance();
    if (pCertificate->FromId(id))
        return pCertificate;
    delete pCertificate;

#endif

    return NULL;
}
