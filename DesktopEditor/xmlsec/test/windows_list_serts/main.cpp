#include "../../src/XmlCertificate.h"
#include "../../src/OOXMLSigner.h"
#include "../../src/OOXMLVerifier.h"

#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "cryptui.lib")
#pragma comment (lib, "Advapi32.lib")

void main(void)
{
    //std::wstring sFolderOOOXML = NSFile::GetProcessDirectory() + L"/ImageStamp";
    //std::wstring sSignId = L"{39B6B9C7-60AD-45A2-9F61-40C74A24042E}";

    std::wstring sFolderOOXML = L"D:\\555";

    if (false)
    {
        std::wstring sSignId = L"{9792D33F-AB37-4E5B-A465-481B9465818B}";

        CCertificate oCertificate;
        if (!oCertificate.ShowSelectDialog())
            return;

        COOXMLSigner oOOXMLSigner(sFolderOOXML, &oCertificate);

        oOOXMLSigner.SetGuid(sSignId);
        oOOXMLSigner.SetImageValid(NSFile::GetProcessDirectory() + L"/../../../resources/valid.png");
        oOOXMLSigner.SetImageInvalid(NSFile::GetProcessDirectory() + L"/../../../resources/invalid.png");

        oOOXMLSigner.Sign();
    }
    else
    {
        COOXMLVerifier oVerifier(sFolderOOXML);

        size_t nCount = oVerifier.m_arSignatures.size();
        for (std::vector<COOXMLSignature*>::iterator i = oVerifier.m_arSignatures.begin(); i != oVerifier.m_arSignatures.end(); i++)
        {
            COOXMLSignature* pSign = *i;
        }

        XML_UNUSED(nCount);
    }
}
