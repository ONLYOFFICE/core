#ifndef _XML_OOXMLSIGNER_H_
#define _XML_OOXMLSIGNER_H_

#include "./Certificate.h"

class COOXMLSigner_private;
class OPENSSL_DECL COOXMLSigner
{
public:
    COOXMLSigner(const std::wstring& sFolder,               ICertificate* pContext);
    COOXMLSigner(unsigned char* data, unsigned long length, ICertificate* pContext);
    ~COOXMLSigner();

    void SetGuid        (const std::wstring& guid);
    void SetImageValid  (const std::wstring& file);
    void SetImageInvalid(const std::wstring& file);
    void SetImageValid  (unsigned char* data, unsigned long length);
    void SetImageInvalid(unsigned char* data, unsigned long length);

    int Sign(unsigned char*& pFiletoWrite, unsigned long& dwLenFiletoWrite);

    // Simle alias to Sign(data, len) for folder realization
    int Sign();

private:
    COOXMLSigner_private* m_internal;
};

#endif //_XML_OOXMLSIGNER_H_
