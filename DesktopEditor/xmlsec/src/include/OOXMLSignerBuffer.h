#ifndef _XML_OOXMLSIGNERBUFFER_H_
#define _XML_OOXMLSIGNERBUFFER_H_

#include "./XmlCertificate.h"

class COOXMLSignerBuffer_private;
class Q_DECL_EXPORT COOXMLSignerBuffer
{
public:
    COOXMLSignerBuffer(unsigned char* file, ICertificate* pContext);
    ~COOXMLSignerBuffer();

    void SetGuid(const std::wstring& guid);
    void SetImageValid(unsigned char* file);
    void SetImageInvalid(unsigned char* file);

    int Sign();

private:
    COOXMLSignerBuffer_private* m_internal;
};

#endif //_XML_OOXMLSIGNERBUFFER_H_
