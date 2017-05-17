#include "../../src/include/XmlCertificate.h"
#include "../../src/include/OOXMLSigner.h"
#include "../../src/include/OOXMLVerifier.h"

#include "../../../common/File.h"

int main(void)
{
    //std::wstring sFolderOOOXML = NSFile::GetProcessDirectory() + L"/ImageStamp";
    //std::wstring sSignId = L"{39B6B9C7-60AD-45A2-9F61-40C74A24042E}";

    std::wstring sFolderOOXML = L"D:\\555";

    if (false)
    {
        std::wstring sSignId = L"{9792D33F-AB37-4E5B-A465-481B9465818B}";

        ICertificate* pCertificate = ICertificate::CreateInstance();
        if (!pCertificate->ShowSelectDialog())
        {
            RELEASEOBJECT(pCertificate);
            return 0;
        }

        COOXMLSigner oOOXMLSigner(sFolderOOXML, pCertificate);

        oOOXMLSigner.SetGuid(sSignId);
        oOOXMLSigner.SetImageValid(NSFile::GetProcessDirectory() + L"/../../../resources/valid.png");
        oOOXMLSigner.SetImageInvalid(NSFile::GetProcessDirectory() + L"/../../../resources/invalid.png");

        oOOXMLSigner.Sign();

        RELEASEOBJECT(pCertificate);
    }
    else
    {
        COOXMLVerifier oVerifier(sFolderOOXML);

        int nCount = oVerifier.GetSignatureCount();
        for (int i = 0; i < nCount; i++)
        {
            COOXMLSignature* pSign = oVerifier.GetSignature(i);
            int nRes = pSign->GetValid();
            pSign = pSign;
            nRes = nRes;
        }
    }
    return 0;
}
