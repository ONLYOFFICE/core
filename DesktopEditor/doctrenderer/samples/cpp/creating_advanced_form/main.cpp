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
#include <iostream>
#include "common.h"
#include "docbuilder.h"

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

void setTableBorders(CValue oTable, int borderColor)
{
    oTable.Call("SetTableBorderTop", "single", 4, 0, borderColor, borderColor, borderColor);
    oTable.Call("SetTableBorderBottom", "single", 4, 0, borderColor, borderColor, borderColor);
    oTable.Call("SetTableBorderLeft", "single", 4, 0, borderColor, borderColor, borderColor);
    oTable.Call("SetTableBorderRight", "single", 4, 0, borderColor, borderColor, borderColor);
    oTable.Call("SetTableBorderInsideV", "single", 4, 0, borderColor, borderColor, borderColor);
    oTable.Call("SetTableBorderInsideH", "single", 4, 0, borderColor, borderColor, borderColor);
}

CValue createFullWidthTable(CValue oApi, int rows, int cols, int borderColor)
{
    CValue oTable = oApi.Call("CreateTable", cols, rows);
    oTable.Call("SetWidth", "percent", 100);
    setTableBorders(oTable, borderColor);
    return oTable;
}

CValue getTableCellParagraph(CValue oTable, int row, int col)
{
    return oTable.Call("GetCell", row, col).Call("GetContent").Call("GetElement", 0);
}

void addTextToParagraph(CValue oParagraph, std::string text, int fontSize, bool isBold)
{
    oParagraph.Call("AddText", text.c_str());
    oParagraph.Call("SetFontSize", fontSize);
    oParagraph.Call("SetBold", isBold);
}

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

void setTextFormProperties(CValue oTextForm, string key, string tip, bool required, string placeholder, bool comb, int maxCharacters, int cellWidth, bool multiLine, bool autoFit)
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

void addTextFormToParagraph(CValue oParagraph, CValue oTextForm, int fontSize, string jc, bool hasBorder, int borderColor)
{
    if (hasBorder)
    {
        oTextForm.Call("SetBorderColor", borderColor, borderColor, borderColor);
    }
    oParagraph.Call("AddElement", oTextForm);
    oParagraph.Call("SetFontSize", fontSize);
    oParagraph.Call("SetJc", jc.c_str());
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

    // Create advanced form
    CValue oDocument = oApi.Call("GetDocument");
    CValue oTable = createFullWidthTable(oApi, 1, 2, 255);
    CValue oParagraph = getTableCellParagraph(oTable, 0, 0);
    addTextToParagraph(oParagraph, "PURCHASE ORDER", 36, true);
    oParagraph = getTableCellParagraph(oTable, 0, 1);
    addTextToParagraph(oParagraph, "Serial # ", 25, true);

    CValue oTextForm = oApi.Call("CreateTextForm");
    setTextFormProperties(oTextForm, "Serial", "Enter serial number", false, "Serial", true, 6, 1, false, false);
    addTextFormToParagraph(oParagraph, oTextForm, 25, "left", true, 255);
    oDocument.Call("Push", oTable);

    CValue oPictureForm = oApi.Call("CreatePictureForm");
    setPictureFormProperties(oPictureForm, "Photo", "Upload company logo", false, "Photo", "tooBig", false, false, 0, 0, "https://api.onlyoffice.com/content/img/docbuilder/examples/user-profile.png");
    oParagraph = oApi.Call("CreateParagraph");
    oParagraph.Call("AddElement", oPictureForm);
    oDocument.Call("Push", oParagraph);

    oTextForm = oApi.Call("CreateTextForm");
    setTextFormProperties(oTextForm, "Company Name", "Enter company name", false, "Company Name", true, 20, 1, false, false);
    oParagraph = oApi.Call("CreateParagraph");
    addTextFormToParagraph(oParagraph, oTextForm, 35, "left", false, 255);
    oDocument.Call("Push", oParagraph);

    oParagraph = oApi.Call("CreateParagraph");
    addTextToParagraph(oParagraph, "Date: ", 25, true);
    oTextForm = oApi.Call("CreateTextForm");
    setTextFormProperties(oTextForm, "Date", "Date", false, "DD.MM.YYYY", true, 10, 1, false, false);
    addTextFormToParagraph(oParagraph, oTextForm, 25, "left", true, 255);
    oDocument.Call("Push", oParagraph);

    oParagraph = oApi.Call("CreateParagraph");
    addTextToParagraph(oParagraph, "To:", 35, true);
    oDocument.Call("Push", oParagraph);

    oTable = createFullWidthTable(oApi, 1, 1, 200);
    oParagraph = getTableCellParagraph(oTable, 0, 0);
    oTextForm = oApi.Call("CreateTextForm");
    setTextFormProperties(oTextForm, "Recipient", "Recipient", false, "Recipient", true, 25, 1, false, false);
    addTextFormToParagraph(oParagraph, oTextForm, 32, "left", false, 255);
    oDocument.Call("Push", oTable);

    oTable = createFullWidthTable(oApi, 10, 2, 200);
    oTable.Call("GetRow", 0).Call("SetBackgroundColor", 245, 245, 245, false);
    CValue oCell = oTable.Call("GetCell", 0, 0);
    oCell.Call("SetWidth", "percent", 30);
    oParagraph = getTableCellParagraph(oTable, 0, 0);
    addTextToParagraph(oParagraph, "Qty.", 30, true);
    oParagraph = getTableCellParagraph(oTable, 0, 1);
    addTextToParagraph(oParagraph, "Description", 30, true);

    for (int i = 1; i < 10; i++)
    {
        CValue oTempParagraph = getTableCellParagraph(oTable, i, 0);
        CValue oTempTextForm = oApi.Call("CreateTextForm");
        setTextFormProperties(oTempTextForm, "Qty" + std::to_string(i), "Qty" + std::to_string(i), false, " ", true, 9, 1, false, false);
        addTextFormToParagraph(oTempParagraph, oTempTextForm, 30, "left", false, 255);
        oTempParagraph = getTableCellParagraph(oTable, i, 1);
        oTempTextForm = oApi.Call("CreateTextForm");
        setTextFormProperties(oTempTextForm, "Description" + std::to_string(i), "Description" + std::to_string(i), false, " ", true, 22, 1, false, false);
        addTextFormToParagraph(oTempParagraph, oTempTextForm, 30, "left", false, 255);
    }

    oDocument.Call("Push", oTable);
    oDocument.Call("RemoveElement", 0);
    oDocument.Call("RemoveElement", 1);

    // Save and close
    oBuilder.SaveFile(OFFICESTUDIO_FILE_DOCUMENT_DOCX, resultPath);
    oBuilder.CloseFile();
    CDocBuilder::Dispose();
    return 0;
}
