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
