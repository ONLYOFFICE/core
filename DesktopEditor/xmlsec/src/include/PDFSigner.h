#ifndef _XML_PDFSIGNER_H_
#define _XML_PDFSIGNER_H_

#include "./XmlCertificate.h"

class CPDFSigner_private;
class Q_DECL_EXPORT CPDFSigner
{
public:
    CPDFSigner(ICertificate* pContext);
    CPDFSigner(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword);
    ~CPDFSigner();

private:
    CPDFSigner_private* m_internal;
};

#endif //_XML_PDFSIGNER_H_
