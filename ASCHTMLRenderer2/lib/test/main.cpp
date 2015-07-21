#include <QCoreApplication>

#include "../../../PdfReader/PdfReader.h"
#include "../include/HTMLRenderer3.h"
#include "../../../DesktopEditor/fontengine/ApplicationFonts.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::wstring sFile = L"\\\\KIRILLOV8\\_Office\\PDF\\Android intro(2p).pdf";

    CApplicationFonts oFonts;
    oFonts.Initialize();
    PdfReader::CPdfReader oReader(&oFonts);
    bool bResult = oReader.LoadFromFile(sFile.c_str());

    NSHtmlRenderer::CASCHTMLRenderer3 oHtmlRenderer;
    oHtmlRenderer.CreateOfficeFile(L"D:\\test\\Document");

    int nPagesCount = oReader.GetPagesCount();
    for (int i = 0; i < nPagesCount; ++i)
    {
        double dWidth = 0;
        double dHeight = 0;

        oReader.GetPageSize(i, &dWidth, &dHeight);

        oHtmlRenderer.NewPage();
        oHtmlRenderer.put_Width(dWidth);
        oHtmlRenderer.put_Height(dHeight);

        oReader.DrawPageOnRenderer(&oHtmlRenderer, i, NULL);
    }

    oHtmlRenderer.CloseFile();

    return a.exec();
}
