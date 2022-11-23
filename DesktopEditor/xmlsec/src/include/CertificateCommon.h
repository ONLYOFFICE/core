#ifndef _XML_CERTIFICATE_COMMON_H_
#define _XML_CERTIFICATE_COMMON_H_

#include "./Certificate.h"

class OPENSSL_DECL CCertificateInfo
{
public:
	std::wstring Name;
	std::string Date;
	std::string Id;
	std::string RawData;

public:
	CCertificateInfo();
	~CCertificateInfo();
};

namespace NSOpenSSL
{
	OPENSSL_DECL int LoadKey(std::wstring file, std::string password);
	OPENSSL_DECL int LoadCert(std::wstring file, std::string password);
	OPENSSL_DECL int LoadKeyRaw(unsigned char* data, unsigned int len, std::string password);
	OPENSSL_DECL int LoadCertRaw(unsigned char* data, unsigned int len, std::string password);
}

namespace NSCertificate
{
	OPENSSL_DECL CCertificateInfo GetDefault();
	OPENSSL_DECL ICertificate* GetById(const std::string& id);

	OPENSSL_DECL CCertificateInfo GetInfo(ICertificate* pCert);

	OPENSSL_DECL ICertificate* FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword);
	OPENSSL_DECL ICertificate* FromFilesRaw(unsigned char* key, unsigned int keyLen, const std::string& keyPassword,
											unsigned char* cert, unsigned int certLen, const std::string& certPassword);

	OPENSSL_DECL int GetOOXMLHashAlg(const std::string& sAlg);
	OPENSSL_DECL std::string GetDigestMethodA(const int& nAlg);
	OPENSSL_DECL std::wstring GetDigestMethod(const int& nAlg);
	OPENSSL_DECL std::string GetSignatureMethodA(const int& nAlg);
	OPENSSL_DECL std::wstring GetSignatureMethod(const int& nAlg);

	OPENSSL_DECL ICertificate* CreateInstance(const int& type = CERTIFICATE_ENGINE_TYPE_DEFAULT);
};

#endif // _XML_CERTIFICATE_COMMON_H_
