#include "../PdfRenderer.h"
#include "../../PdfReader/PdfReader.h"
#include "../../DesktopEditor/common/File.h"

int main()
{
    NSFonts::IApplicationFonts* pApplicationFonts = NSFonts::NSApplication::Create();

    CPdfRenderer pdfWriter(pApplicationFonts, true);
    pdfWriter.SetTempFolder(NSFile::GetProcessDirectory() + L"/wtemp");

    IOfficeDrawingFile* pReader = new PdfReader::CPdfReader(pApplicationFonts);
    pReader->SetTempDirectory(NSFile::GetProcessDirectory() + L"/rtemp");
    std::wstring sPassword = L"";

    bool bResult = pReader->LoadFromFile(NSFile::GetProcessDirectory() + L"/test.pdf", L"", sPassword, sPassword);
    if (bResult)
    {
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
    }

    pdfWriter.SaveToFile(NSFile::GetProcessDirectory() + L"/res.pdf");
    RELEASEOBJECT(pReader);
    RELEASEOBJECT(pApplicationFonts);
    return 0;
}
