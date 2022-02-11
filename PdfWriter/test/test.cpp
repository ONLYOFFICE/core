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

    // PDFDoc монополизирует доступ к файлу для чтения и не отпускает пока не деструкнится.
    // Для дозаписи файл нужно прочитать в память для ридера, а доступ к файлу отдать вритеру.
    BYTE* pData;
    DWORD nBytes;
    NSFile::CFileBinary::ReadAllBytes(NSFile::GetProcessDirectory() + L"/test.pdf", &pData, nBytes);
    if (!pData)
    {
        RELEASEOBJECT(pReader);
        RELEASEOBJECT(pApplicationFonts);
        return 1;
    }

    bool bResult = pReader->LoadFromMemory(pData, nBytes);
    if (bResult && pReader->EditPdf(&pdfWriter))
    {
        if (pReader->EditPage(0))
        {
            pdfWriter.TEST();
        }

        if (pReader->EditPage(1))
        {
            pdfWriter.TEST();
        }

        pReader->AddToFile(NSFile::GetProcessDirectory() + L"/test.pdf");
    }

    RELEASEOBJECT(pReader);
    RELEASEOBJECT(pApplicationFonts);
    RELEASEARRAYOBJECTS(pData);
    return 0;
}
