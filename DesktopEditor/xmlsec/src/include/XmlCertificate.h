#ifndef _XML_SERTIFICATE_BASE_H_
#define _XML_SERTIFICATE_BASE_H_

#include <string>
#include <vector>
#include "../../../common/base_export.h"

#define OOXML_HASH_ALG_SHA1         0
#define OOXML_HASH_ALG_INVALID      1

class ICertificate;
class Q_DECL_EXPORT ICertificateSelectDialogOpenSsl
{
public:
    ICertificateSelectDialogOpenSsl()
    {
    }
    virtual ~ICertificateSelectDialogOpenSsl()
    {
    }

public:
    virtual std::wstring GetCertificatePath() = 0;
    virtual std::wstring GetCertificatePassword() = 0;

    virtual std::wstring GetKeyPath() = 0;
    virtual std::wstring GetKeyPassword() = 0;

    virtual bool ShowSelectDialog() = 0;
    virtual int ShowCertificate(ICertificate* pCert) = 0;
};

class Q_DECL_EXPORT CCertificateInfo
{
public:
    std::wstring m_name;
    std::string m_date;
    std::string m_id;

public:
    CCertificateInfo()
    {
    }
    ~CCertificateInfo()
    {
    }
};

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

    virtual std::string GetDate()               = 0;
    virtual std::string GetId()                 = 0;

public:
    virtual std::string Sign(const std::string& sXml)                                   = 0;
    virtual std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg)     = 0;
    virtual std::string GetHash(const std::string& sXml, int nAlg)                      = 0;
    virtual std::string GetHash(const std::wstring& sXmlFile, int nAlg)                 = 0;
    virtual bool Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg)  = 0;

    virtual bool LoadFromBase64Data(const std::string& data)                            = 0;    

public:
    virtual bool ShowSelectDialog()             = 0;
    virtual int ShowCertificate()               = 0;

    virtual void SetOpenSslDialog(ICertificateSelectDialogOpenSsl* pDialog) {}

    static CCertificateInfo GetDefault();
    static ICertificate* GetById(const std::string& id);

    virtual CCertificateInfo GetInfo()
    {
        CCertificateInfo info;
        info.m_name = GetSignerName();
        info.m_date = GetDate();
        info.m_id = GetId();
        return info;
    }

public:
    static int GetOOXMLHashAlg(const std::string& sAlg);
    static ICertificate* CreateInstance();
};

#endif // _XML_SERTIFICATE_BASE_H_
