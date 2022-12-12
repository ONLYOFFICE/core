#ifndef _XML_CERTIFICATE_BASE_H_
#define _XML_CERTIFICATE_BASE_H_

#include "./openssl_config.h"

#define OOXML_HASH_ALG_SHA1         0
#define OOXML_HASH_ALG_INVALID      1
#define OOXML_HASH_ALG_SHA256       2
#define OOXML_HASH_ALG_SHA224       3
#define OOXML_HASH_ALG_SHA384       4
#define OOXML_HASH_ALG_SHA512       5
#define OOXML_HASH_ALG_ED25519      6
#define OOXML_HASH_ALG_ED448        7
#define OOXML_HASH_ALG_SHA3_512     8

#define OOXML_HASH_ALG_GOST_GR3411          10
#define OOXML_HASH_ALG_GOST_GR3411_2012_256 11
#define OOXML_HASH_ALG_GOST_GR3411_2012_512 12

#define OPEN_SSL_WARNING_OK         0
#define OPEN_SSL_WARNING_ERR        1
#define OPEN_SSL_WARNING_ALL_OK     2
#define OPEN_SSL_WARNING_PASS       4
#define OPEN_SSL_WARNING_NOVERIFY   8

#define CERTIFICATE_ENGINE_TYPE_DEFAULT     0
#define CERTIFICATE_ENGINE_TYPE_OPENSSL     1
#define CERTIFICATE_ENGINE_TYPE_MSCRYPTO    2
#define CERTIFICATE_ENGINE_TYPE_OFORM       3

class OPENSSL_DECL ICertificate
{
public:
	ICertificate();
	virtual ~ICertificate();

public:
	virtual int GetType()                       = 0;

	virtual std::string GetNumber()             = 0;
	virtual std::wstring GetSignerName()        = 0;
	virtual std::wstring GetIssuerName()        = 0;

	virtual std::string GetCertificateBase64()  = 0;
	virtual std::string GetCertificateHash()    = 0;

	virtual std::string GetDate()               = 0;
	virtual std::string GetId()                 = 0;
	virtual int VerifySelf()                    = 0;

public:
	virtual std::string Sign(unsigned char* pData, unsigned int nSize)                  = 0;
	virtual std::string Sign(const std::string& sXml)                                   = 0;
	virtual bool SignPKCS7(unsigned char* pData, unsigned int nSize,
						   unsigned char*& pDataDst, unsigned int& nSizeDst)            = 0;

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
	virtual std::string Print() = 0;
};

#endif // _XML_CERTIFICATE_BASE_H_
