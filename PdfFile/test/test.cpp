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

    std::wstring wsSrcFile = NSFile::GetProcessDirectory() + L"/test.pdf";
    std::wstring wsDstFile = NSFile::GetProcessDirectory() + L"/test2.pdf";
    std::wstring wsTempDir = NSFile::GetProcessDirectory() + L"/pdftemp";

    if (!NSDirectory::Exists(wsTempDir))
        NSDirectory::CreateDirectory(wsTempDir);

    CPdfFile pdfFile(pApplicationFonts);
    pdfFile.SetTempDirectory(wsTempDir);

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

    if (false)
    {
        double dPageDpiX, dPageDpiY, dWidth, dHeight;
        pdfFile.GetPageInfo(0, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);
        pdfFile.ConvertToRaster(0, NSFile::GetProcessDirectory() + L"/res.png", 4, dWidth * dPageDpiX / 25.4, dHeight * dPageDpiY / 25.4, true, pdfFile.GetFontManager());

        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

    if (false)
    {
        for (int i = 0; i < pdfFile.GetPagesCount(); i++)
        {
            pdfFile.DrawPageOnRenderer(pdfFile.GetWriter(), i, NULL);
        }
        pdfFile.SaveToFile(wsDstFile);

        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

    if (bResult && pdfFile.EditPdf(wsDstFile))
    {
        if (false && pCertificate)
        {
            if (pdfFile.EditPage(0))
            {
                pdfFile.Sign(10, 70, 50, 50, NSFile::GetProcessDirectory() + L"/test.png", pCertificate);
            }
        }
        else
        {
            if (pdfFile.EditPage(0))
            {
                pdfFile.TEST(1);
                pdfFile.PageRotate(90);
            }

            pdfFile.DeletePage(1);

            if (pdfFile.EditPage(1))
            {
                pdfFile.TEST(3);
            }

            if (pdfFile.AddPage(3))
            {
                pdfFile.TEST(1);
            }
        }

        pdfFile.EditClose();
    }

    RELEASEINTERFACE(pApplicationFonts);
    RELEASEOBJECT(pCertificate);
    return 0;
}
