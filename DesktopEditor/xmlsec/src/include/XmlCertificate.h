#ifndef _XML_SERTIFICATE_BASE_H_
#define _XML_SERTIFICATE_BASE_H_

#include <string>
#include <vector>
#include "../../../common/base_export.h"

#define OOXML_HASH_ALG_SHA1         0
#define OOXML_HASH_ALG_INVALID      1
#define OOXML_HASH_ALG_SHA256       2
#define OOXML_HASH_ALG_SHA224       3
#define OOXML_HASH_ALG_SHA384       4
#define OOXML_HASH_ALG_SHA512       5

#define OOXML_HASH_ALG_GOST_GR3411          10
#define OOXML_HASH_ALG_GOST_GR3411_2012_256 11
#define OOXML_HASH_ALG_GOST_GR3411_2012_512 12

#define OPEN_SSL_WARNING_OK         0
#define OPEN_SSL_WARNING_ERR        1
#define OPEN_SSL_WARNING_ALL_OK     2
#define OPEN_SSL_WARNING_PASS       4
#define OPEN_SSL_WARNING_NOVERIFY   8

namespace NSOpenSSL
{
    Q_DECL_EXPORT int LoadKey(std::wstring file, std::string password);
    Q_DECL_EXPORT int LoadCert(std::wstring file, std::string password);
}

class Q_DECL_EXPORT CCertificateInfo
{
private:
    std::wstring m_name;
    std::string m_date;
    std::string m_id;
    std::string m_rawData;

public:
    CCertificateInfo()
    {
    }
    ~CCertificateInfo()
    {
    }

    std::wstring GetName()
    {
        return m_name;
    }
    void SetName(const std::wstring& name)
    {
        m_name = name;
    }

    std::string GetDate()
    {
        return m_date;
    }
    void SetDate(const std::string& date)
    {
        m_date = date;
    }

    std::string GetId()
    {
        return m_id;
    }
    void SetId(const std::string& id)
    {
        m_id = id;
    }

    std::string GetRawBase64()
    {
        return m_rawData;
    }
    void SetRawBase64(const std::string& data)
    {
        m_rawData = data;
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
    virtual std::wstring GetIssuerName()        = 0;

    virtual std::string GetCertificateBase64()  = 0;
    virtual std::string GetCertificateHash()    = 0;

    virtual std::string GetDate()               = 0;
    virtual std::string GetId()                 = 0;
    virtual int VerifySelf()                    = 0;

public:
    virtual std::string Sign(const std::string& sXml)                                   = 0;
    virtual std::string GetHash(unsigned char* pData, unsigned int nSize, int nAlg)     = 0;
    virtual std::string GetHash(const std::string& sXml, int nAlg)                      = 0;
    virtual std::string GetHash(const std::wstring& sXmlFile, int nAlg)                 = 0;
    virtual bool Verify(const std::string& sXml, std::string& sXmlSignature, int nAlg)  = 0;

    virtual bool LoadFromBase64Data(const std::string& data)                            = 0;

    virtual std::vector<int> GetHashAlgs()                                              = 0;
    virtual int GetHashAlg()                                                            = 0;

    virtual bool IsGOST()                                                               = 0;

public:
    virtual int ShowSelectDialog(void* parent = NULL)   = 0;
    virtual int ShowCertificate(void* parent = NULL)    = 0;

    static CCertificateInfo GetDefault();
    static ICertificate* GetById(const std::string& id);

    virtual CCertificateInfo GetInfo();

    virtual bool FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword);

public:
    static int GetOOXMLHashAlg(const std::string& sAlg);
    static std::string GetDigestMethodA(const int& nAlg);
    static std::wstring GetDigestMethod(const int& nAlg);
    static std::string GetSignatureMethodA(const int& nAlg);
    static std::wstring GetSignatureMethod(const int& nAlg);

    static ICertificate* CreateInstance();
};

#endif // _XML_SERTIFICATE_BASE_H_
