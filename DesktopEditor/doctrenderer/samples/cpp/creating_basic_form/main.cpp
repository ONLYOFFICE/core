/*
* (c) Copyright Ascensio System SIA 2010-2023
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
* You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include <string>
#include "common.h"
#include "docbuilder.h"

using namespace NSDoctRenderer;

const wchar_t* workDir = L"C:\\Program Files\\ONLYOFFICE\\DocumentBuilder";
const wchar_t* resultPath = L"result.docx";

// Helper functions
void setPictureFormProperties(CValue oPictureForm, std::string key, std::string tip, bool required, std::string placeholder, std::string scaleFlag, bool lockAspectRatio, bool respectBorders, int shiftX, int shiftY, std::string imageUrl)
{
    oPictureForm.Call("SetFormKey", key.c_str());
    oPictureForm.Call("SetTipText", tip.c_str());
    oPictureForm.Call("SetRequired", required);
    oPictureForm.Call("SetPlaceholderText", placeholder.c_str());
    oPictureForm.Call("SetScaleFlag", scaleFlag.c_str());
    oPictureForm.Call("SetLockAspectRatio", lockAspectRatio);
    oPictureForm.Call("SetRespectBorders", respectBorders);
    oPictureForm.Call("SetPicturePosition", shiftX, shiftY);
    oPictureForm.Call("SetImage", imageUrl.c_str());
}
void setTextFormProperties(CValue oTextForm, std::string key, std::string tip, bool required, std::string placeholder, bool comb, int maxCharacters, int cellWidth, bool multiLine, bool autoFit)
{
    oTextForm.Call("SetFormKey", key.c_str());
    oTextForm.Call("SetTipText", tip.c_str());
    oTextForm.Call("SetRequired", required);
    oTextForm.Call("SetPlaceholderText", placeholder.c_str());
    oTextForm.Call("SetComb", comb);
    oTextForm.Call("SetCharactersLimit", maxCharacters);
    oTextForm.Call("SetCellWidth", cellWidth);
    oTextForm.Call("SetCellWidth", multiLine);
    oTextForm.Call("SetMultiline", autoFit);
}

// Main function
int main(int argc, char* argv[])
{
    // Init DocBuilder
    CDocBuilder::Initialize(workDir);
    CDocBuilder oBuilder;
    oBuilder.SetProperty("--work-directory", workDir);
    oBuilder.CreateFile(OFFICESTUDIO_FILE_DOCUMENT_DOCX);

    CContext oContext = oBuilder.GetContext();
    CContextScope oScope = oContext.CreateScope();
    CValue oGlobal = oContext.GetGlobal();
    CValue oApi = oGlobal["Api"];

    // Create basic form
    CValue oDocument = oApi.Call("GetDocument");
    CValue oParagraph = oDocument.Call("GetElement", 0);
    CValue oHeadingStyle = oDocument.Call("GetStyle", "Heading 3");

    oParagraph.Call("AddText", "Employee pass card");
    oParagraph.Call("SetStyle", oHeadingStyle);
    oDocument.Call("Push", oParagraph);

    CValue oPictureForm = oApi.Call("CreatePictureForm");
    setPictureFormProperties(oPictureForm, "Photo", "Upload your photo", false, "Photo", "tooBig", true, false, 50, 50, "https://api.onlyoffice.com/content/img/docbuilder/examples/user-profile.png");
    oParagraph = oApi.Call("CreateParagraph");
    oParagraph.Call("AddElement", oPictureForm);
    oDocument.Call("Push", oParagraph);

    CValue oTextForm = oApi.Call("CreateTextForm");
    setTextFormProperties(oTextForm, "First name", "Enter your first name", false, "First name", true, 13, 3, false, false);
    oParagraph = oApi.Call("CreateParagraph");
    oParagraph.Call("AddElement", oTextForm);
    oDocument.Call("Push", oParagraph);

    // Save and close
    oBuilder.SaveFile(OFFICESTUDIO_FILE_DOCUMENT_DOCX, resultPath);
    oBuilder.CloseFile();
    CDocBuilder::Dispose();
    return 0;
}
