#include "../doctrenderer.h"

#ifdef USE_PDF_RENDERER
#include "../../../PdfWriter/PdfRenderer.h"
#include "../../fontengine/ApplicationFonts.h"
#endif

int main(int argc, char *argv[])
{
#if 1

#if 1
    std::wstring strXml = L"<Settings>\
<SrcFileType>1</SrcFileType>\
<DstFileType>3</DstFileType>\
<SrcFilePath>D:\\build_doc</SrcFilePath>\
<DstFilePath>D:\\build_doc\\EditorWithChanges.bin</DstFilePath>\
<FontsDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\OfficeWeb\\Fonts\\native</FontsDirectory>\
<ImagesDirectory>D:\\build_doc\\media</ImagesDirectory>\
<ThemesDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\ServerComponents\\Test\\Applications\\TestAVSOfficeDocxFile2\\TestAVSOfficeDocxFile2\\bin\\x86\\presentationthemes</ThemesDirectory>\
<Changes TopItem=\"-1\">\
<Change>D:\\build_doc\\changes\\changes0.json</Change>\
</Changes>\
</Settings>";

#else

    std::wstring strXml = L"<Settings>\
<SrcFileType>0</SrcFileType>\
<DstFileType>0</DstFileType>\
<SrcFilePath>D:\\build_doc</SrcFilePath>\
<DstFilePath>D:\\build_doc\\destination</DstFilePath>\
<FontsDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\OfficeWeb\\Fonts\\native</FontsDirectory>\
<ImagesDirectory>D:\\build_doc\\media</ImagesDirectory>\
<ThemesDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\ServerComponents\\Test\\Applications\\TestAVSOfficeDocxFile2\\TestAVSOfficeDocxFile2\\bin\\x86\\presentationthemes</ThemesDirectory>\
<MailMergeData DatabasePath=\"D:\\build_doc\\mm.json\" Start=\"0\" End=\"17\" Field=\"email\" />\
</Settings>";
#endif

#else
    std::wstring strXml = L"<Settings>\
<SrcFileType>0</SrcFileType>\
<DstFileType>0</DstFileType>\
<SrcFilePath>/home/oleg/build_doc</SrcFilePath>\
<DstFilePath>/home/oleg/build_doc/EditorWithChanges.bin</DstFilePath>\
<FontsDirectory>/home/oleg/AVS/Sources/TeamlabOffice/trunk/OfficeWeb/Fonts/native</FontsDirectory>\
<ImagesDirectory>/home/oleg/build_doc/media</ImagesDirectory>\
<ThemesDirectory>/home/oleg/presentationthemes</ThemesDirectory>\
<Changes TopItem=\"-1\">\
<Change>/home/oleg/build_doc/changes/changes0.json</Change>\
</Changes>\
</Settings>";
#endif

#if 1

    NSDoctRenderer::CDoctrenderer oRenderer;

    std::wstring sError;
    oRenderer.Execute(strXml, sError);

#endif

#ifdef USE_PDF_RENDERER

    CApplicationFonts oFonts;
    oFonts.Initialize();
    CPdfRenderer oPDF(&oFonts);
    oPDF.SetTempFolder(L"D:\\test_pdf");

    oPDF.OnlineWordToPdfFromBinary(L"D:\\test_pdf\\pdf.bin", L"D:\\test_pdf\\123.pdf");

#endif

    return 0;
}
