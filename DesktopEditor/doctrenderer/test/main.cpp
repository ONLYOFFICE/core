#include <QCoreApplication>

#include "../doctrenderer.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::wstring strXml = L"<Settings>\
<SrcFileType>0</SrcFileType>\
<DstFileType>0</DstFileType>\
<SrcFilePath>D:\\build_doc</SrcFilePath>\
<DstFilePath>D:\\build_doc\\EditorWithChanges.bin</DstFilePath>\
<FontsDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\OfficeWeb\\Fonts\\native</FontsDirectory>\
<ImagesDirectory>D:\\build_doc\\media</ImagesDirectory>\
<ThemesDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\ServerComponents\\Test\\Applications\\TestAVSOfficeDocxFile2\\TestAVSOfficeDocxFile2\\bin\\x86\\presentationthemes</ThemesDirectory>\
<Changes TopItem=\"-1\">\
<Change>D:\\build_doc\\changes\\changes0.json</Change>\
</Changes>\
</Settings>";

    NSDoctRenderer::CDoctrenderer oRenderer;

    std::wstring sError;
    oRenderer.Execute(strXml, sError);

    return a.exec();
}
