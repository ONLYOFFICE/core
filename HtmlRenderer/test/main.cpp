//#include <QCoreApplication>

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../PdfWriter/PdfRenderer.h"
#include "../include/HTMLRenderer3.h"

int main(int argc, char *argv[])
{
    //QCoreApplication a(argc, argv);

#if 0
    //std::wstring sFile = L"\\\\KIRILLOV8\\_Office\\PDF\\Android intro(2p).pdf";
    std::wstring sFile = L"\\\\kirillov8\\_Office\\PDF\\Main Window(15p).pdf";
    //std::wstring sFile = L"D:\\knut.djvu";
    //std::wstring sFile = L"D:\\bankomats.xps";
    std::wstring sDst = L"D:\\test\\Document";
#endif

#if 1
    //std::wstring sFile = L"/home/oleg/activex/Android intro(2p).pdf";
    //std::wstring sFile = L"/home/oleg/activex/knut.djvu";
    std::wstring sFile = L"/home/oleg/activex/bankomats.xps";
    std::wstring sDst = L"/home/oleg/activex/1";
#endif

    CApplicationFonts oFonts;
    oFonts.Initialize();

#if 0
    PdfReader::CPdfReader oReader(&oFonts);
    oReader.SetTempFolder(sDst.c_str());
#endif

#if 0
    CDjVuFile oReader;
#endif

#if 1
    CXpsFile oReader(&oFonts);
    oReader.SetTempFolder(sDst.c_str());
#endif

    bool bResult = oReader.LoadFromFile(sFile.c_str());

#if 0
    NSHtmlRenderer::CASCHTMLRenderer3 oHtmlRenderer;
    oHtmlRenderer.CreateOfficeFile(sDst);
#else
    CPdfRenderer oHtmlRenderer(&oFonts);
    oHtmlRenderer.SetTempFolder(sDst);
#endif

    int nPagesCount = oReader.GetPagesCount();
    for (int i = 0; i < nPagesCount; ++i)
    {
        oHtmlRenderer.NewPage();
        oHtmlRenderer.BeginCommand(c_nPageType);

        double dPageDpiX, dPageDpiY;
        double dWidth, dHeight;
        oReader.GetPageInfo(i, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

        dWidth  *= 25.4 / dPageDpiX;
        dHeight *= 25.4 / dPageDpiY;

        oHtmlRenderer.put_Width(dWidth);
        oHtmlRenderer.put_Height(dHeight);

        oReader.DrawPageOnRenderer(&oHtmlRenderer, i, NULL);

        oHtmlRenderer.EndCommand(c_nPageType);
    }

#if 0
    oHtmlRenderer.CloseFile();
#else
    oHtmlRenderer.SaveToFile(L"/home/oleg/activex/1/1.pdf");
#endif

    return 0;
}
