#ifdef WIN32
#include "./XmlSigner_mscrypto.h"
#define CCertificate CCertificate_mscrypto

int ICertificateSelectDialogOpenSsl::LoadKey(std::wstring file, std::string password)
{
    return 0;
}

int ICertificateSelectDialogOpenSsl::LoadCert(std::wstring file, std::string password)
{
    return 0;
}

#endif

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

namespace
{
    ICertificateSelectDialogOpenSsl* g_application_openssl_gialog = NULL;
}

void ICertificateSelectDialogOpenSsl::SetOpenSslDialogApplication(ICertificateSelectDialogOpenSsl* pDialog)
{
    g_application_openssl_gialog = pDialog;
}

int ICertificate::GetOOXMLHashAlg(const std::string& sAlg)
{
    if ("http://www.w3.org/2000/09/xmldsig#rsa-sha1" == sAlg ||
        "http://www.w3.org/2000/09/xmldsig#sha1" == sAlg)
        return OOXML_HASH_ALG_SHA1;

    return OOXML_HASH_ALG_INVALID;
}

ICertificate* ICertificate::CreateInstance()
{
    ICertificate* pCert = new CCertificate();
    pCert->SetOpenSslDialog(g_application_openssl_gialog);
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
