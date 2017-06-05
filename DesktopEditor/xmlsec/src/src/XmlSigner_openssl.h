#ifndef _XMLSIGNER_OPENSSL_H_
#define _XMLSIGNER_OPENSSL_H_

#include "./include/XmlCertificate.h"

#include "../../../common/File.h"
#include "../../../common/BigInteger.h"

class CCertificate_openssl : public ICertificate
{
public:

protected:
    BYTE*           m_rawData;
    int             m_rawDataLen;

public:
    CCertificate_openssl() : ICertificate()
    {
        m_rawData = NULL;
        m_rawDataLen = 0;
    }
    virtual ~CCertificate_openssl()
    {        
    }

public:
    virtual std::string GetNumber()
    {
        return "";
    }

    virtual std::wstring GetSignerName()
    {
        return L"";
    }

    virtual std::string GetCertificateBase64()
    {
        return "";
    }

    virtual std::string GetCertificateHash()
    {
        return "";
    }

    virtual std::string GetDate()
    {
        return "";
    }

    virtual std::string GetId()
    {
        return "";
    }

public:
    virtual std::string Sign(const std::string& sXml)
    {
        return "";
    }

    virtual std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg)
    {
        return "";
    }

    virtual std::string GetHash(const std::string& sXml, int nAlg)
    {
        return GetHash((BYTE*)sXml.c_str(), (DWORD)sXml.length(), nAlg);
    }

    virtual std::string GetHash(const std::wstring& sXmlFile, int nAlg)
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

    virtual bool Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg)
    {
        return false;
    }

    virtual bool LoadFromBase64Data(const std::string& data)
    {
        return false;
    }

    virtual int ShowCertificate()
    {
        return 1;
    }

public:
    virtual bool ShowSelectDialog()
    {
        return false;
    }
};

#endif // _XMLSIGNER_OPENSSL_H_
