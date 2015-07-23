#include <QCoreApplication>

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../include/HTMLRenderer3.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //std::wstring sFile = L"\\\\KIRILLOV8\\_Office\\PDF\\Android intro(2p).pdf";
    //std::wstring sFile = L"\\\\kirillov8\\_Office\\PDF\\Main Window(15p).pdf";
    std::wstring sFile = L"D:\\knut.djvu";

    CApplicationFonts oFonts;
    oFonts.Initialize();

#if 0
    PdfReader::CPdfReader oReader(&oFonts);
    oReader.SetTempFolder(L"D:\\test\\Document");
#endif

#if 1
    CDjVuFile oReader;
#endif

    bool bResult = oReader.LoadFromFile(sFile.c_str());

    NSHtmlRenderer::CASCHTMLRenderer3 oHtmlRenderer;
    oHtmlRenderer.CreateOfficeFile(L"D:\\test\\Document");

    int nPagesCount = oReader.GetPagesCount();
    for (int i = 0; i < nPagesCount; ++i)
    {
        oReader.DrawPageOnRenderer(&oHtmlRenderer, i, NULL);
    }

    oHtmlRenderer.CloseFile();

    return a.exec();
}
