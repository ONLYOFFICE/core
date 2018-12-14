#ifndef _XMLSIGNER_OPENSSL_H_
#define _XMLSIGNER_OPENSSL_H_

#include "./../include/XmlCertificate.h"

class CCertificate_openssl_private;
class CCertificate_openssl : public ICertificate
{
protected:
    CCertificate_openssl_private* m_internal;

public:
    CCertificate_openssl();
    virtual ~CCertificate_openssl();

public:
    virtual std::string GetNumber();

    virtual std::wstring GetSignerName();

    virtual std::wstring GetIssuerName();

    virtual std::string GetCertificateBase64();

    virtual std::string GetCertificateHash();

    virtual std::string GetDate();

    virtual std::string GetId();

    virtual int VerifySelf();

public:
    virtual std::string Sign(const std::string& sXml);

    virtual std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg);

    virtual std::string GetHash(const std::string& sXml, int nAlg);

    virtual std::string GetHash(const std::wstring& sXmlFile, int nAlg);

    virtual bool Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg);

    virtual bool LoadFromBase64Data(const std::string& data);

    virtual std::vector<int> GetHashAlgs();
    virtual int GetHashAlg();

    virtual bool IsGOST();

public:
    virtual int ShowSelectDialog(void* parent = NULL);
    virtual int ShowCertificate(void* parent = NULL);

    virtual bool FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword);
    bool FromId(const std::string& id);
};

#endif // _XMLSIGNER_OPENSSL_H_
