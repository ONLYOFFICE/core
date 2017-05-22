#ifdef WIN32
#include "./XmlSigner_mscrypto.h"
#define CCertificate CCertificate_mscrypto
#endif

#if defined(_LINUX) && !defined(_MAC)
#include "./XmlSigner_openssl.h"
#define CCertificate CCertificate_openssl
#endif

#ifdef _MAC
#include "./XmlSigner_openssl.h"
#define CCertificate CCertificate_openssl
#endif

int ICertificate::GetOOXMLHashAlg(const std::string& sAlg)
{
    if ("http://www.w3.org/2000/09/xmldsig#rsa-sha1" == sAlg ||
        "http://www.w3.org/2000/09/xmldsig#sha1" == sAlg)
        return OOXML_HASH_ALG_SHA1;

    return OOXML_HASH_ALG_INVALID;
}

ICertificate* ICertificate::CreateInstance()
{
    return new CCertificate();
}

CCertificateInfo ICertificate::GetDefault()
{
    CCertificateInfo info;

    // detect user name
    std::wstring sUserName;
#ifdef WIN32
    DWORD dwUserNameLen = 1024;
    wchar_t* _name = new wchar_t[dwUserNameLen + 1];
    GetUserNameW(_name, &dwUserNameLen);
    sUserName = std::wstring(_name);
    delete []_name;
#endif
    ////////////////////

#ifdef WIN32
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

    return NULL;
}
