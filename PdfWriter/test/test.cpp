#include "../PdfRenderer.h"
#include "../../PdfReader/PdfReader.h"
#include "../../DesktopEditor/common/File.h"
#include "../../DesktopEditor/common/Directory.h"
#include "../../DesktopEditor/fontengine/ApplicationFontsWorker.h"

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
    pRenderer->OnlineWordToPdf(NSFile::GetProcessDirectory() + L"/../example/pdf.bin", L"", true, false);
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

    BYTE* pData;
    DWORD nBytes;
    NSFile::CFileBinary::ReadAllBytes(sSrcFile, &pData, nBytes);
    if (!pData)
    {
        RELEASEOBJECT(pReader);
        RELEASEINTERFACE(pApplicationFonts);
        return 1;
    }

    bool bResult = pReader->LoadFromMemory(pData, nBytes);
    if (bResult && pReader->EditPdf(&pdfWriter))
    {
        if (pReader->EditPage(0))
        {
            TEST(&pdfWriter);
        }

        if (pdfWriter.NewPage() == S_OK)
        {
            TEST(&pdfWriter);
        }

        if (pReader->EditPage(1))
        {
            TEST2(&pdfWriter);
        }

        NSFile::CFileBinary::Copy(sSrcFile, sDstFile);
        pReader->EditClose(sDstFile);
    }

    RELEASEOBJECT(pReader);
    RELEASEINTERFACE(pApplicationFonts);
    RELEASEARRAYOBJECTS(pData);
    return 0;
}
