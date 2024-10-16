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

#include <map>
#include <string>
#include "common.h"
#include "docbuilder.h"

#include "resources/utils/utils.h"

using namespace std;
using namespace NSDoctRenderer;

const wchar_t* workDir = L"C:\\Program Files\\ONLYOFFICE\\DocumentBuilder";
const wchar_t* resultPath = L"result.docx";

// Helper functions
string cValueToString(CValue value)
{
    wchar_t* txt = value.ToString().c_str();
    wstring ws(txt);
    string str(ws.begin(), ws.end());
    return str;
}

// Main function
int main(int argc, char* argv[])
{
    std::map<string, string> formData;
    formData["Photo"] = "https://api.onlyoffice.com/content/img/docbuilder/examples/blue_cloud.png";
    formData["Serial"] = "A1345";
    formData["Company Name"] = "Blue Cloud";
    formData["Date"] = "25.12.2023";
    formData["Recipient"] = "Space Corporation";
    formData["Qty1"] = "25";
    formData["Description1"] = "Frame";
    formData["Qty2"] = "2";
    formData["Description2"] = "Stack";
    formData["Qty3"] = "34";
    formData["Description3"] = "Shifter";

    // Init DocBuilder
    CDocBuilder::Initialize(workDir);
    CDocBuilder oBuilder;
    oBuilder.SetProperty("--work-directory", workDir);
	wstring templatePath = NSUtils::GetResourcesDirectory() + L"/docs/form.docx";
	oBuilder.OpenFile(templatePath.c_str(), L"");

    CContext oContext = oBuilder.GetContext();
    CContextScope oScope = oContext.CreateScope();
    CValue oGlobal = oContext.GetGlobal();
    CValue oApi = oGlobal["Api"];

    // Fill form
    CValue oDocument = oApi.Call("GetDocument");
    CValue aForms = oDocument.Call("GetAllForms");

    int formNum = 0;
    while (formNum < aForms.GetLength())
    {
        CValue form = aForms[formNum];
        string type = cValueToString(aForms[formNum].Call("GetFormType"));
        string value = formData[cValueToString(aForms[formNum].Call("GetFormKey"))];
        if (type == "textForm") form.Call("SetText", value.c_str());
        if (type == "pictureForm") form.Call("SetImage", value.c_str());
        formNum++;
    }

    // Save and close
    oBuilder.SaveFile(OFFICESTUDIO_FILE_DOCUMENT_DOCX, resultPath);
    oBuilder.CloseFile();
    CDocBuilder::Dispose();
    return 0;
}
