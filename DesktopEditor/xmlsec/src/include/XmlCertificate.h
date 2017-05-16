#ifndef _XML_SERTIFICATE_BASE_H_
#define _XML_SERTIFICATE_BASE_H_

#include <string>
#include <vector>
#include "../../../common/base_export.h"

#define OOXML_HASH_ALG_SHA1         0
#define OOXML_HASH_ALG_INVALID      1

class Q_DECL_EXPORT ICertificate
{
public:
    ICertificate()
    {
    }

    virtual ~ICertificate()
    {
    }

public:
    virtual std::string GetNumber()             = 0;
    virtual std::wstring GetSignerName()        = 0;

    virtual std::string GetCertificateBase64()  = 0;
    virtual std::string GetCertificateHash()    = 0;

public:
    virtual std::string Sign(std::string sXml)                                          = 0;
    virtual std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg)     = 0;
    virtual std::string GetHash(std::string& sXml, int nAlg)                            = 0;
    virtual std::string GetHash(std::wstring& sXmlFile, int nAlg)                       = 0;
    virtual bool Verify(std::string& sXml, std::string& sXmlSignature, int nAlg)        = 0;

    virtual bool LoadFromBase64Data(const std::string& data)                            = 0;    

public:
    virtual bool ShowSelectDialog()             = 0;
    virtual int ShowCertificate()               = 0;

public:
    static int GetOOXMLHashAlg(const std::string& sAlg);
    static ICertificate* CreateInstance();
};

#endif // _XML_SERTIFICATE_BASE_H_
