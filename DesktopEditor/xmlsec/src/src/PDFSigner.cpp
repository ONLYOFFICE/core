#include "./../include/PDFSigner.h"
#include "./XmlTransform.h"

class CPDFSigner_private
{
public:
    ICertificate*                           m_certificate;

public:
    CPDFSigner_private(ICertificate* pContext)
    {
        m_certificate = pContext;
    }
    CPDFSigner_private(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
    {
        m_certificate = ICertificate::CreateInstance();
        m_certificate->FromFiles(keyPath, keyPassword, certPath, certPassword);
    }
};

CPDFSigner::CPDFSigner(ICertificate* pContext)
{
    m_internal = new CPDFSigner_private(pContext);
}

CPDFSigner::CPDFSigner(const std::wstring& keyPath, const std::string& keyPassword, const std::wstring& certPath, const std::string& certPassword)
{
    m_internal = new CPDFSigner_private(keyPath, keyPassword, certPath, certPassword);
}

CPDFSigner::~CPDFSigner()
{
    RELEASEOBJECT(m_internal);
}
