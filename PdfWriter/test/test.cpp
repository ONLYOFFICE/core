#include "../PdfRenderer.h"
#include "../../PdfReader/PdfReader.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"
#include "../../DesktopEditor/xmlsec/src/include/CertificateCommon.h"

void TEST(CPdfRenderer* pRenderer)
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

void TEST2(CPdfRenderer* pRenderer)
{
    pRenderer->OnlineWordToPdf(NSFile::GetProcessDirectory() + L"/../example/pdf.bin", L"", true);
}

void TEST3(CPdfRenderer* pRenderer)
{
    pRenderer->OnlineWordToPdfFromBinary(NSFile::GetProcessDirectory() + L"/../example1/1/pdf.bin", L"", true);
}

int main()
{
    CApplicationFontsWorker oWorker;
    oWorker.m_sDirectory = NSFile::GetProcessDirectory() + L"/fonts_cache";
    oWorker.m_bIsNeedThumbnails = false;

    if (!NSDirectory::Exists(oWorker.m_sDirectory))
        NSDirectory::CreateDirectory(oWorker.m_sDirectory);

    NSFonts::IApplicationFonts* pApplicationFonts = oWorker.Check();

    CPdfRenderer pdfWriter(pApplicationFonts);
    pdfWriter.SetTempFolder(NSFile::GetProcessDirectory() + L"/wtemp");

    PdfReader::CPdfReader* pReader = new PdfReader::CPdfReader(pApplicationFonts);
    pReader->SetTempDirectory(NSFile::GetProcessDirectory() + L"/rtemp");

    // PDFDoc монополизирует доступ к файлу для чтения и не отпускает пока не деструкнится.
    // Для дозаписи файл нужно прочитать в память для ридера, а доступ к файлу отдать вритеру.
    std::wstring sSrcFile = NSFile::GetProcessDirectory() + L"/test.pdf";
    std::wstring sDstFile = NSFile::GetProcessDirectory() + L"/test2.pdf";

    if (NSFile::CFileBinary::Exists(sDstFile))
        NSFile::CFileBinary::Remove(sDstFile);

    std::wstring sPassword;
    bool bResult = pReader->LoadFromFile(sSrcFile);
    if (!bResult)
    {
        sPassword = L"123456";
        pReader->LoadFromFile(sSrcFile, L"", sPassword, sPassword);
        bResult = pReader->GetError() == 0;
    }

    double dPageDpiX, dPageDpiY;
    double dWidth, dHeight;
    pReader->GetPageInfo(0, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

    dWidth  *= 25.4 / dPageDpiX;
    dHeight *= 25.4 / dPageDpiY;

    ICertificate* pCertificate = NULL;
    if (true)
    {
        std::wstring sCertificateFile = NSFile::GetProcessDirectory() + L"/test.pfx";
        std::wstring sPrivateKeyFile = L"";
        std::string sCertificateFilePassword = "test";
        std::string sPrivateFilePassword = "";

        pCertificate = NSCertificate::FromFiles(sPrivateKeyFile, sPrivateFilePassword, sCertificateFile, sCertificateFilePassword);
    }

    if (false)
    {
        pReader->ConvertToRaster(0, NSFile::GetProcessDirectory() + L"/res.png", 4, dWidth * dPageDpiX / 25.4, dHeight * dPageDpiX / 25.4, true, pReader->GetFontManager());
        RELEASEOBJECT(pReader);
        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

    if (false && bResult)
    {
        pdfWriter.SetPassword(L"123456");
        pdfWriter.NewPage();
        pdfWriter.BeginCommand(c_nPageType);
        pdfWriter.put_Width(dWidth);
        pdfWriter.put_Height(dHeight);
        pReader->DrawPageOnRenderer(&pdfWriter, 0, NULL);

        if (pCertificate)
        {
            // Подпись будет на текущей странице
            pdfWriter.Sign(10, 10, 50, 50, NSFile::GetProcessDirectory() + L"/test.jpg", pCertificate);
            pdfWriter.Sign(10, 70, 50, 50, NSFile::GetProcessDirectory() + L"/test.png", pCertificate);
            pdfWriter.Sign(10, 130, 50, 50, NSFile::GetProcessDirectory() + L"/test.jpg", pCertificate);
            pdfWriter.Sign(0, dHeight, 0, 0, L"", pCertificate); // Невидимая подпись
        }

        pdfWriter.EndCommand(c_nPageType);
        pdfWriter.SaveToFile(sDstFile);

        RELEASEOBJECT(pReader);
        RELEASEINTERFACE(pApplicationFonts);
        RELEASEOBJECT(pCertificate);
        return 0;
    }

    bool bNewDstFile = false;
    if (bNewDstFile)
        NSFile::CFileBinary::Copy(sSrcFile, sDstFile);
    if (bResult && pReader->EditPdf(&pdfWriter, bNewDstFile ? sDstFile : sSrcFile, sPassword))
    {
        if (true && pCertificate)
        {
            // Подпись не позволяет вносить иные изменения кроме заполнения форм, подписания и комментирования документа
            if (pReader->EditPage(0))
            {
                pdfWriter.Sign(10, 70, 50, 50, NSFile::GetProcessDirectory() + L"/test.png", pCertificate);
            }
        }
        else
        {
            if (pReader->EditPage(0))
            {
                TEST(&pdfWriter);
                pdfWriter.PageRotate(90);
            }

            pReader->DeletePage(1);

            if (pReader->EditPage(1))
            {
                TEST3(&pdfWriter);
            }

            if (pReader->EditPage(2))
            {
                // При редактировании страницы нельзя изменить её пропорции
                pdfWriter.put_Width(dWidth);
                pdfWriter.put_Height(dHeight);

                TEST2(&pdfWriter);
            }

            if (pReader->AddPage(3))
            {
                // Новой странице необходимо выставить длину и ширину, иначе будут значения по умолчанию
                pdfWriter.put_Width(dWidth);
                pdfWriter.put_Height(dHeight);

                TEST(&pdfWriter);
            }

            if (pReader->AddPage(800))
            {
                // Новой странице необходимо выставить длину и ширину, иначе будут значения по умолчанию
                pdfWriter.put_Width(dWidth);
                pdfWriter.put_Height(dHeight);

                TEST3(&pdfWriter);
            }
        }

        pReader->EditClose(sPassword);
    }

    RELEASEOBJECT(pReader);
    RELEASEINTERFACE(pApplicationFonts);
    RELEASEOBJECT(pCertificate);
    return 0;
}
