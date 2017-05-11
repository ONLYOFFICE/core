#include "../../src/XmlCanonicalizator.h"
#include "../../src/XmlSigner_mscrypto.h"
#include "../../src/OOXMLSigner.h"

#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "cryptui.lib")
#pragma comment (lib, "Advapi32.lib")

void main(void)
{
    if (false)
    {
        BYTE* pData = NULL;
        DWORD dwDataLen = 0;
        bool bRes = NSFile::CFileBinary::ReadAllBytes(L"D:\\cert2.bin", &pData, dwDataLen);

        if (!bRes)
            return;

        PCCERT_CONTEXT context = CertCreateCertificateContext(X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, pData, dwDataLen);

        if (!context)
        {
            RELEASEARRAYOBJECTS(pData);
            return;
        }

        BOOL result = CryptUIDlgViewContext(CERT_STORE_CERTIFICATE_CONTEXT, context, NULL, NULL, 0, NULL);
        result;

        if (context)
            CertFreeCertificateContext(context);

        RELEASEARRAYOBJECTS(pData);
        return;
    }

    if (false)
    {
        xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
        xmlSubstituteEntitiesDefault(1);

        std::string sXmlCan = CXmlCanonicalizator::Execute(L"D:\\1.xml", XML_C14N_1_0);
        NSFile::CFileBinary::SaveToFile(L"D:\\2.xml", UTF8_TO_U(sXmlCan));
        return;
    }

    //std::wstring sFolderOOOXML = NSFile::GetProcessDirectory() + L"/ImageStamp";
    //std::wstring sSignId = L"{39B6B9C7-60AD-45A2-9F61-40C74A24042E}";

    std::wstring sFolderOOXML = L"D:\\555";
    std::wstring sSignId = L"{9792D33F-AB37-4E5B-A465-481B9465818B}";

    CCertificate_mscrypto oCertificate;
    if (!oCertificate.ShowSelectDialog())
        return;

    COOXMLSigner oOOXMLSigner(sFolderOOXML, &oCertificate);

    oOOXMLSigner.SetGuid(sSignId);
    oOOXMLSigner.SetImageValid(NSFile::GetProcessDirectory() + L"/../../../resources/valid.png");
    oOOXMLSigner.SetImageInvalid(NSFile::GetProcessDirectory() + L"/../../../resources/invalid.png");

    oOOXMLSigner.Sign();
}
