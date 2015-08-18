//#include <QCoreApplication>

#include "../../DesktopEditor/fontengine/ApplicationFonts.h"

#include "../../PdfReader/PdfReader.h"
#include "../../DjVuFile/DjVu.h"
#include "../../XpsFile/XpsFile.h"
#include "../../PdfWriter/PdfRenderer.h"
#include "../include/HTMLRenderer3.h"

#include "../include/ASCSVGWriter.h"
#include "../../DesktopEditor/raster/Metafile/MetaFile.h"

int main(int argc, char *argv[])
{
    CApplicationFonts oFonts;
    oFonts.Initialize();

#if 0

    NSHtmlRenderer::CASCSVGWriter oWriterSVG;
    oWriterSVG.SetFontManager(oFonts.GenerateFontManager());

    MetaFile::CMetaFile oMetafile(&oFonts);

    oMetafile.LoadFromFile(L"D:\\2\\ppt\\media\\image4.wmf");

    double x = 0, y = 0, w = 0, h = 0;
    oMetafile.GetBounds(&x, &y, &w, &h);

    oWriterSVG.put_Width(100000);
    oWriterSVG.put_Height(100000);
    oMetafile.DrawOnRenderer(&oWriterSVG, 0, 0, 100000, 100000);

    oWriterSVG.SaveFile(L"D:\\oleg.svg");

    return 0;

#endif


    //QCoreApplication a(argc, argv);
#ifdef WIN32
    //std::wstring sFile = L"\\\\KIRILLOV8\\_Office\\PDF\\Android intro(2p).pdf";
    //std::wstring sFile = L"D:\\activex\\Pi(1p).pdf";
    //std::wstring sFile = L"\\\\192.168.3.208\\allusers\\Files\\PDF\\AllPDF\\asia.pdf";
    //std::wstring sFile = L"D:\\knut.djvu";
    std::wstring sFile = L"D:\\bankomats.xps";
    std::wstring sDst = L"D:\\test\\Document";
#else
    //std::wstring sFile = L"/home/oleg/activex/Android intro(2p).pdf";
    //std::wstring sFile = L"/home/oleg/activex/Pi(1p).pdf";
    std::wstring sFile = L"/home/oleg/activex/knut.djvu";
    //std::wstring sFile = L"/home/oleg/activex/bankomats.xps";
    std::wstring sDst = L"/home/oleg/activex/1";
#endif    

#if 0
    PdfReader::CPdfReader oReader(&oFonts);
    oReader.SetTempFolder(sDst.c_str());
#endif

#if 1
    CDjVuFile oReader;
#endif

#if 0
    CXpsFile oReader(&oFonts);
    oReader.SetTempFolder(sDst.c_str());
#endif

    bool bResult = oReader.LoadFromFile(sFile.c_str());

#if 1
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

#if 1
    oHtmlRenderer.CloseFile();
#else
    oHtmlRenderer.SaveToFile(L"/home/oleg/activex/1/1.pdf");
#endif

    return 0;
}
