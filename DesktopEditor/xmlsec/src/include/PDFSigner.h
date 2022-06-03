#ifndef _XML_PDFSIGNER_H_
#define _XML_PDFSIGNER_H_

#include <string>
#include "../../../common/base_export.h"

class CPDFSigner_private;
class Q_DECL_EXPORT CPDFSigner
{
public:
    CPDFSigner(const std::wstring& sCertFile, const std::string& sCertPassword);
    CPDFSigner(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword);
    ~CPDFSigner();

    void Sign(unsigned char* pDataForSignature, unsigned long dwLenDataForSignature, unsigned char*& pDataForWrite, unsigned long& dwLenDataForWrite);

private:
    CPDFSigner_private* m_internal;
};

#endif //_XML_PDFSIGNER_H_
