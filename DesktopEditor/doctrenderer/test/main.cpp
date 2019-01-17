/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "../doctrenderer.h"

#ifdef USE_PDF_RENDERER
#include "../../../PdfWriter/PdfRenderer.h"
#include "../../graphics/pro/Fonts.h"
#endif

int main(int argc, char *argv[])
{
#if 1

#if 1
    std::wstring strXml = L"<Settings>\
<SrcFileType>1</SrcFileType>\
<DstFileType>1</DstFileType>\
<SrcFilePath>D:\\source</SrcFilePath>\
<DstFilePath>D:\\source\\EditorWithChanges.bin</DstFilePath>\
<FontsDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\ServerComponents\\DesktopEditor\\ChromiumBasedEditors2\\app\\test\\src\\build\\win64\\Debug\\Local\\converter\\fonts\\</FontsDirectory>\
<ImagesDirectory>D:\\source\\media</ImagesDirectory>\
<ThemesDirectory>D:\\activex\\AVS\\Sources\\TeamlabOffice\\trunk\\ServerComponents\\Test\\Applications\\TestAVSOfficeDocxFile2\\TestAVSOfficeDocxFile2\\bin\\x86\\presentationthemes</ThemesDirectory>\
<Changes TopItem=\"-1\">\
<Change>D:\\source\\changes\\changes0.json</Change>\
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

    NSDoctRenderer::CDoctrenderer oRenderer(L"C:/ProgramData/ONLYOFFICE/webdata/cloud/fonts/AllFonts.js");

    std::wstring sError;
    oRenderer.Execute(strXml, sError);

#endif

#ifdef USE_PDF_RENDERER

    CApplicationFonts oFonts;
    oFonts.Initialize();
    CPdfRenderer oPDF(&oFonts);
    oPDF.SetTempFolder(L"D:\\test_pdf");

    oPDF.OnlineWordToPdfFromBinary(L"D:\\DE_CAD4\\EditorWithChanges.bin", L"D:\\test_pdf\\123.pdf");

#endif

    return 0;
}
