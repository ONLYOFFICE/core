#ifndef _XMLSIGNER_BASE_H_
#define _XMLSIGNER_BASE_H_

#include "../../common/File.h"
#include "../../common/BigInteger.h"

#include <string>
#include <vector>
#include <map>

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
    virtual std::string Sign(std::string sXml)                              = 0;
    virtual std::string GetHash(unsigned char* pData, unsigned int nSize)   = 0;
    virtual std::string GetHash(std::string& sXml)                          = 0;
    virtual std::string GetHash(std::wstring& sXmlFile)                     = 0;
    virtual bool Verify(std::string& sXml, std::string& sXmlSignature)      = 0;

    virtual bool LoadFromBase64Data(const std::string& data)                = 0;
    virtual int ShowCertificate()                                           = 0;

public:
    virtual bool ShowSelectDialog()             = 0;
};

#endif // _XMLSIGNER_BASE_H_
