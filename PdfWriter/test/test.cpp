#include "../PdfRenderer.h"
#include "../../PdfReader/PdfReader.h"
#include "../../DesktopEditor/common/File.h"

int main()
{
    NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();

    CPdfRenderer pdfWriter(pApplicationFonts, true);
    pdfWriter.SetTempFolder(NSFile::GetProcessDirectory() + L"/wtemp");

    PdfReader::CPdfReader* pReader = new PdfReader::CPdfReader(pApplicationFonts);
    pReader->SetTempDirectory(NSFile::GetProcessDirectory() + L"/rtemp");
    std::wstring sPassword = L"";

    // PDFDoc монополизирует файл для чтения и не отпускает пока не деструкнится. Для дозаписи файл
    // нужно прочитать в память для ридера, а доступ к файлу отдать вритеру.
    BYTE* pData;
    DWORD nBytes;
    NSFile::CFileBinary::ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.pdf", &pData, nBytes);
    if (!pData)
    {
        RELEASEOBJECT(pReader);
        RELEASEOBJECT(pApplicationFonts);
        return 1;
    }

    bool bResult = pReader->LoadFromMemory(pData, nBytes, L"", sPassword, sPassword);
    // bool bResult = pReader->LoadFromFile(NSFile::GetProcessDirectory() + L"/test.pdf", L"", sPassword, sPassword);
    if (bResult)
    {
        /*
        for (int i = 0, nPagesCount = pReader->GetPagesCount(); i < nPagesCount; ++i)
        {
            pdfWriter.NewPage();
            pdfWriter.BeginCommand(c_nPageType);

            double dPageDpiX, dPageDpiY;
            double dWidth, dHeight;
            pReader->GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

            dWidth  *= 25.4 / dPageDpiX;
            dHeight *= 25.4 / dPageDpiY;

            pdfWriter.put_Width(dWidth);
            pdfWriter.put_Height(dHeight);

            pReader->DrawPageOnRenderer(&pdfWriter, i, NULL);

            pdfWriter.EndCommand(c_nPageType);
        }
        */
    }

    // на какую страницу, писатель PdfWriter, IRenderer с командами дозаписи
    pReader->AddToPage(0, &pdfWriter, NULL, NSFile::GetProcessDirectory() + L"/test.pdf");
    //pdfWriter.SaveToFile(NSFile::GetProcessDirectory() + L"/res.pdf");
    RELEASEOBJECT(pReader);
    RELEASEOBJECT(pApplicationFonts);
    RELEASEARRAYOBJECTS(pData);
    return 0;
}
