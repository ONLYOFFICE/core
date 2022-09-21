#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../DesktopEditor/xmlsec/src/include/CertificateCommon.h"
#include "../PdfFile.h"

int main()
{
    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pApplicationFonts = oWorker.Check();

    CPdfFile pdfFile(pApplicationFonts);
    pdfFile.SetTempDirectory(NSFile::GetProcessDirectory() + L"/pdftemp");

    std::wstring wsSrcFile = NSFile::GetProcessDirectory() + L"/test.pdf";

    std::wstring wsPassword;
    bool bResult = pdfFile.LoadFromFile(wsSrcFile);
    if (!bResult)
    {
        wsPassword = L"123456";
        bResult = pdfFile.LoadFromFile(wsSrcFile, wsPassword);
    }

    ICertificate* pCertificate = NULL;
    if (false)
    {
        std::wstring wsCertificateFile = NSFile::GetProcessDirectory() + L"/test.pfx";
        std::wstring wsPrivateKeyFile = L"";
        std::string sCertificateFilePassword = "test";
        std::string sPrivateFilePassword = "";

        pCertificate = NSCertificate::FromFiles(wsPrivateKeyFile, sPrivateFilePassword, wsCertificateFile, sCertificateFilePassword);
    }

    RELEASEINTERFACE(pApplicationFonts);
    RELEASEOBJECT(pCertificate);
    return 0;
}
