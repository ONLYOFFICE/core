#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../DesktopEditor/xmlsec/src/include/CertificateCommon.h"
#include "../PdfFile.h"

void TEST(IRenderer* pRenderer)
{
    // ТЕСТОВЫЕ КОММАНДЫ
    pRenderer->PathCommandStart();
    pRenderer->PathCommandMoveTo(10, 10);
    pRenderer->PathCommandLineTo(20, 20);
    pRenderer->PathCommandCurveTo(70, 30, 30, 20, 50, 50);
    pRenderer->PathCommandClose();
    pRenderer->put_BrushColor1(0xFF0000);
    pRenderer->put_PenColor(0x0000FF);
    pRenderer->put_PenSize(1);
    pRenderer->DrawPath(c_nStroke | c_nWindingFillMode);
    pRenderer->PathCommandEnd();
}

void TEST2(IRenderer* pRenderer)
{
    ((CPdfFile*)pRenderer)->OnlineWordToPdf(NSFile::GetProcessDirectory() + L"/../example/pdf.bin", L"");
}

void TEST3(IRenderer* pRenderer)
{
    ((CPdfFile*)pRenderer)->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/../example1/1/pdf.bin", L"");
}

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
        std::string sCertificateFilePassword = "123456";
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
        pdfFile.CreatePdf();
        double dPageDpiX, dPageDpiY, dWidth, dHeight;
        pdfFile.GetPageInfo(0, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        for (int i = 0; i < pdfFile.GetPagesCount(); i++)
        {
            pdfFile.NewPage();
            pdfFile.BeginCommand(c_nPageType);
            pdfFile.put_Width(dWidth);
            pdfFile.put_Height(dHeight);
            pdfFile.DrawPageOnRenderer(&pdfFile, i, NULL);
            pdfFile.EndCommand(c_nPageType);
        }
        if (pCertificate)
            pdfFile.Sign(10, 70, 50, 50, NSFile::GetProcessDirectory() + L"/test.png", pCertificate);
        pdfFile.SaveToFile(wsDstFile);

        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

    if (bResult && pdfFile.EditPdf(wsDstFile))
    {
        if (pCertificate)
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
                TEST(&pdfFile);
                pdfFile.RotatePage(90);
            }

            pdfFile.DeletePage(1);

            if (pdfFile.EditPage(1))
            {
                TEST2(&pdfFile);
            }

            if (pdfFile.AddPage(3))
            {
                TEST3(&pdfFile);
            }
        }

        pdfFile.Close();
    }

    RELEASEINTERFACE(pApplicationFonts);
    RELEASEOBJECT(pCertificate);
    return 0;
}
