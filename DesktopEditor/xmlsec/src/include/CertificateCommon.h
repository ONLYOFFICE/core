#ifndef _XML_CERTIFICATE_COMMON_H_
#define _XML_CERTIFICATE_COMMON_H_

#include <string>
#include <vector>
#include "../../../common/base_export.h"
#include "./Certificate.h"

class Q_DECL_EXPORT CCertificateInfo
{
public:
    std::wstring Name;
    std::string Date;
    std::string Id;
    std::string RawData;

public:
    CCertificateInfo() {}
    ~CCertificateInfo() {}
};

namespace NSOpenSSL
{
    Q_DECL_EXPORT int LoadKey(std::wstring file, std::string password);
    Q_DECL_EXPORT int LoadCert(std::wstring file, std::string password);
    Q_DECL_EXPORT int LoadKeyRaw(unsigned char* data, unsigned int len, std::string password);
    Q_DECL_EXPORT int LoadCertRaw(unsigned char* data, unsigned int len, std::string password);
}

namespace NSCertificate
{
    Q_DECL_EXPORT CCertificateInfo GetDefault();
    Q_DECL_EXPORT ICertificate* GetById(const std::string& id);

    Q_DECL_EXPORT CCertificateInfo GetInfo(ICertificate* pCert);

    Q_DECL_EXPORT ICertificate* FromFiles(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword);
    Q_DECL_EXPORT ICertificate* FromFilesRaw(unsigned char* key, unsigned int keyLen, const std::string& keyPassword,
                                    unsigned char* cert, unsigned int certLen, const std::string& certPassword);

    Q_DECL_EXPORT int GetOOXMLHashAlg(const std::string& sAlg);
    Q_DECL_EXPORT std::string GetDigestMethodA(const int& nAlg);
    Q_DECL_EXPORT std::wstring GetDigestMethod(const int& nAlg);
    Q_DECL_EXPORT std::string GetSignatureMethodA(const int& nAlg);
    Q_DECL_EXPORT std::wstring GetSignatureMethod(const int& nAlg);

    Q_DECL_EXPORT ICertificate* CreateInstance(const int& type = CERTIFICATE_ENGINE_TYPE_DEFAULT);
};

#endif // _XML_CERTIFICATE_COMMON_H_
