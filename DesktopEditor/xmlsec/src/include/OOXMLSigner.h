#ifndef _XML_OOXMLSIGNER_H_
#define _XML_OOXMLSIGNER_H_

#include "./XmlCertificate.h"

class COOXMLSigner_private;
class Q_DECL_EXPORT COOXMLSigner
{
public:
    COOXMLSigner(const std::wstring& sFolder, ICertificate* pContext);
    ~COOXMLSigner();

    void SetGuid(const std::wstring& guid);
    void SetImageValid(const std::wstring& file);
    void SetImageInvalid(const std::wstring& file);

    void Sign();

private:
    COOXMLSigner_private* m_internal;
};

#endif //_XML_OOXMLSIGNER_H_
