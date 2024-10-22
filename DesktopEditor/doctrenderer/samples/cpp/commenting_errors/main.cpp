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

#include "resources/utils/utils.h"

using namespace std;
using namespace NSDoctRenderer;

const wchar_t* workDir = L"C:\\Program Files\\ONLYOFFICE\\DocumentBuilder";
const wchar_t* resultPath = L"result.xlsx";

// Helper functions
void CheckCell(CValue oWorksheet, wstring cell, int row, int col)
{
	if (cell.find('#') != std::wstring::npos)
    {
		wstring commentMsg = L"Error: " + cell;
        CValue errorCell = oWorksheet.Call("GetRangeByNumber", row, col);
		errorCell.Call("AddComment", commentMsg.c_str());
    }
}

// Main function
int main()
{
    // Init DocBuilder
    CDocBuilder::Initialize(workDir);
    CDocBuilder oBuilder;
    oBuilder.SetProperty("--work-directory", workDir);

    // Open file and get context
	wstring templatePath = NSUtils::GetResourcesDirectory() + L"/docs/spreadsheet_with_errors.xlsx";
	oBuilder.OpenFile(templatePath.c_str(), L"");
    CContext oContext = oBuilder.GetContext();
    CContextScope oScope = oContext.CreateScope();
    CValue oGlobal = oContext.GetGlobal();
    CValue oApi = oGlobal["Api"];

    // Find and comment formula errors
    CValue oWorksheet = oApi.Call("GetActiveSheet");
    CValue oRange = oWorksheet.Call("GetUsedRange");
    CValue data = oRange.Call("GetValue");

    for (int row = 0; row < (int)data.GetLength(); row++)
    {
        for (int col = 0; col < (int)data[0].GetLength(); col++)
        {
			CheckCell(oWorksheet, data[row][col].ToString().c_str(), row, col);
        }
    }

    // Save and close
    oBuilder.SaveFile(OFFICESTUDIO_FILE_SPREADSHEET_XLSX, resultPath);
    oBuilder.CloseFile();
    CDocBuilder::Dispose();
    return 0;
}
