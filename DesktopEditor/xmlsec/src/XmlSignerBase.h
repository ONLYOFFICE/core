#ifndef _XMLSIGNER_BASE_H_
#define _XMLSIGNER_BASE_H_

#include "../../common/File.h"
#include "../../common/BigInteger.h"

#include <string>
#include <vector>
#include <map>

#define OOXML_HASH_ALG_SHA1         0
#define OOXML_HASH_ALG_INVALID      1

class ICertificate
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
    virtual int ShowCertificate()                                                       = 0;

public:
    virtual bool ShowSelectDialog()             = 0;

    static int GetOOXMLHashAlg(const std::string& sAlg)
    {
        if ("http://www.w3.org/2000/09/xmldsig#rsa-sha1" == sAlg ||
            "http://www.w3.org/2000/09/xmldsig#sha1" == sAlg)
            return OOXML_HASH_ALG_SHA1;

        return OOXML_HASH_ALG_INVALID;
    }
};

#endif // _XMLSIGNER_BASE_H_
