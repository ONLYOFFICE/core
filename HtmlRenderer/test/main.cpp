#include <QCoreApplication>

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../include/HTMLRenderer3.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //std::wstring sFile = L"\\\\KIRILLOV8\\_Office\\PDF\\Android intro(2p).pdf";
    //std::wstring sFile = L"\\\\kirillov8\\_Office\\PDF\\Main Window(15p).pdf";
    //std::wstring sFile = L"D:\\knut.djvu";
    std::wstring sFile = L"D:\\bankomats.xps";

    CApplicationFonts oFonts;
    oFonts.Initialize();

#if 0
    PdfReader::CPdfReader oReader(&oFonts);
    oReader.SetTempFolder(L"D:\\test\\Document");
#endif

#if 0
    CDjVuFile oReader;
#endif

#if 1
    CXpsFile oReader(&oFonts);
    oReader.SetTempFolder(L"D:\\test\\Document");
#endif

    bool bResult = oReader.LoadFromFile(sFile.c_str());

    NSHtmlRenderer::CASCHTMLRenderer3 oHtmlRenderer;
    oHtmlRenderer.CreateOfficeFile(L"D:\\test\\Document");

    int nPagesCount = oReader.GetPagesCount();
    for (int i = 0; i < nPagesCount; ++i)
    {
#if 1
        oHtmlRenderer.NewPage();
        oHtmlRenderer.BeginCommand(c_nPageType);

        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        oReader.GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        oHtmlRenderer.put_Width(dWidth);
        oHtmlRenderer.put_Height(dHeight);
#endif

        oReader.DrawPageOnRenderer(&oHtmlRenderer, i, NULL);

#if 1
        oHtmlRenderer.EndCommand(c_nPageType);
#endif
    }

    oHtmlRenderer.CloseFile();

    return a.exec();
}
