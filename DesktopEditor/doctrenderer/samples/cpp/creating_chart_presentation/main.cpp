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
#include <vector>
#include "common.h"
#include "docbuilder.h"

#include "resources/utils/utils.h"

using namespace std;
using namespace NSDoctRenderer;

const wchar_t* workDir = L"C:\\Program Files\\ONLYOFFICE\\DocumentBuilder";
const wchar_t* resultPath = L"result.pptx";

// Helper functions
string cValueToString(CValue value)
{
    wchar_t* txt = value.ToString().c_str();
    wstring ws(txt);
    string str(ws.begin(), ws.end());
    return str;
}

void addText(CValue oApi, int fontSize, string text, CValue oSlide, CValue oShape, CValue oParagraph, CValue oFill, string jc)
{
    CValue oRun = oApi.Call("CreateRun");
    CValue oTextPr = oRun.Call("GetTextPr");
    oTextPr.Call("SetFontSize", fontSize);
    oTextPr.Call("SetFill", oFill);
    oTextPr.Call("SetFontFamily", "Tahoma");
    oParagraph.Call("SetJc", jc.c_str());
    oRun.Call("AddText", text.c_str());
    oRun.Call("AddLineBreak");
    oParagraph.Call("AddElement", oRun);
    oSlide.Call("AddObject", oShape);
}

// Main function
int main(int argc, char* argv[])
{
    // Init DocBuilder
    CDocBuilder::Initialize(workDir);
    CDocBuilder oBuilder;
    oBuilder.SetProperty("--work-directory", workDir);

    // Read chart data from xlsx
	wstring templatePath = NSUtils::GetResourcesDirectory() + L"/docs/chart_data.xlsx";
	oBuilder.OpenFile(templatePath.c_str(), L"");
	CContext oContext = oBuilder.GetContext();
	CContextScope oScope = oContext.CreateScope();
    CValue oGlobal = oContext.GetGlobal();
    CValue oApi = oGlobal["Api"];
    CValue oWorksheet = oApi.Call("GetActiveSheet");
    CValue values = oWorksheet.Call("GetUsedRange").Call("GetValue");

    int sizeX = values.GetLength();
    int sizeY = values[0].GetLength();
    vector<vector<string>> data(sizeX, vector<string>(sizeY, ""));
    for (int i = 0; i < sizeX; i++)
    {
        for (int j = 0; j < sizeY; j++)
        {
            data[i][j] = cValueToString(values[i][j]);
        }
    }
    oBuilder.CloseFile();

    // Create chart presentation
    oBuilder.CreateFile(OFFICESTUDIO_FILE_PRESENTATION_PPTX);
    oContext = oBuilder.GetContext();
	oScope = oContext.CreateScope();
    oGlobal = oContext.GetGlobal();
    oApi = oGlobal["Api"];
    CValue oPresentation = oApi.Call("GetPresentation");
    CValue oSlide = oPresentation.Call("GetSlideByIndex", 0);
    oSlide.Call("RemoveAllObjects");

    CValue oRGBColor = oApi.Call("CreateRGBColor", 255, 244, 240);
    CValue oFill = oApi.Call("CreateSolidFill", oRGBColor);
    oSlide.Call("SetBackground", oFill);

    CValue oStroke = oApi.Call("CreateStroke", 0, oApi.Call("CreateNoFill"));
    CValue oShapeTitle = oApi.Call("CreateShape", "rect", 300 * 36000, 20 * 36000, oApi.Call("CreateNoFill"), oStroke);
    CValue oShapeText = oApi.Call("CreateShape", "rect", 120 * 36000, 80 * 36000, oApi.Call("CreateNoFill"), oStroke);
    oShapeTitle.Call("SetPosition", 20 * 36000, 20 * 36000);
    oShapeText.Call("SetPosition", 210 * 36000, 50 * 36000);
    CValue oParagraphTitle = oShapeTitle.Call("GetDocContent").Call("GetElement", 0);
    CValue oParagraphText = oShapeText.Call("GetDocContent").Call("GetElement", 0);
    oRGBColor = oApi.Call("CreateRGBColor", 115, 81, 68);
    oFill = oApi.Call("CreateSolidFill", oRGBColor);

    string titleContent = "Price Type Report";
    string textContent = "This is an overview of price types. As we can see, May was the price peak, but even in June the price went down, the annual upward trend persists.";
    addText(oApi, 80, titleContent, oSlide, oShapeTitle, oParagraphTitle, oFill, "center");
    addText(oApi, 42, textContent, oSlide, oShapeText, oParagraphText, oFill, "left");

    // Transform 2d array into cols names, rows names and data
    CValue cols = oContext.CreateArray(sizeY - 1);
    for (int col = 1; col < sizeY; col++)
    {
        cols[col - 1] = data[0][col].c_str();
    }

    CValue rows = oContext.CreateArray(sizeX - 1);
    for (int row = 1; row < sizeX; row++)
    {
        rows[row - 1] = data[row][0].c_str();
    }

    CValue vals = oContext.CreateArray(sizeY - 1);
    for (int row = 1; row < sizeY; row++)
    {
        CValue row_data = oContext.CreateArray(sizeX - 1);
        for (int col = 1; col < sizeX; col++)
        {
            row_data[col - 1] = data[col][row].c_str();
        }
        vals[row - 1] = row_data;
    }

    // Pass CValue data to the CreateChart method
    CValue oChart = oApi.Call("CreateChart", "lineStacked", vals, cols, rows);
    oChart.Call("SetSize", 180 * 36000, 100 * 36000);
    oChart.Call("SetPosition", 20 * 36000, 50 * 36000);
    oChart.Call("ApplyChartStyle", 24);
    oChart.Call("SetVertAxisLabelsFontSize", 16);
    oChart.Call("SetHorAxisLabelsFontSize", 16);
    oChart.Call("SetLegendFontSize", 16);
    oChart.Call("SetLegendPos", "top");
    oSlide.Call("AddObject", oChart);

    // Save and close
    oBuilder.SaveFile(OFFICESTUDIO_FILE_PRESENTATION_PPTX, resultPath);
    oBuilder.CloseFile();
    CDocBuilder::Dispose();
    return 0;
}
