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

using namespace std;
using namespace NSDoctRenderer;

const wchar_t* workDir = L"C:\\Program Files\\ONLYOFFICE\\DocumentBuilder";
const wchar_t* resultPath = L"result.xlsx";

// Main function
int main()
{
    string data[9][4] = {
        { "Id", "Product", "Price", "Available" },
        { "1001", "Item A", "12.2", "true" },
        { "1002", "Item B", "18.8", "true" },
        { "1003", "Item C", "70.1", "false" },
        { "1004", "Item D", "60.6", "true" },
        { "1005", "Item E", "32.6", "true" },
        { "1006", "Item F", "28.3", "false" },
        { "1007", "Item G", "11.1", "false" },
        { "1008", "Item H", "41.4", "true" }
    };

    // Init DocBuilder
    CDocBuilder::Initialize(workDir);
    CDocBuilder oBuilder;
    oBuilder.SetProperty("--work-directory", workDir);
    oBuilder.CreateFile(OFFICESTUDIO_FILE_SPREADSHEET_XLSX);

    CContext oContext = oBuilder.GetContext();
    CContextScope oScope = oContext.CreateScope();
    CValue oGlobal = oContext.GetGlobal();
    CValue oApi = oGlobal["Api"];

    // Find and comment formula errors
    CValue oWorksheet = oApi.Call("GetActiveSheet");

    // Create CValue array from data
    int rowsLen = sizeof data / sizeof data[0];
    int colsLen = sizeof data[0] / sizeof(string);
    CValue oArray = oContext.CreateArray(rowsLen);

    for (int row = 0; row < rowsLen; row++)
    {
        CValue oArrayCol = oContext.CreateArray(colsLen);

        for (int col = 0; col < colsLen; col++)
        {
            oArrayCol[col] = data[row][col].c_str();
        }
        oArray[row] = oArrayCol;
    }

    // First cell in the range (A1) is equal to (0,0)
    CValue startCell = oWorksheet.Call("GetRangeByNumber", 0, 0);

    // Last cell in the range is equal to array length -1
    CValue endCell = oWorksheet.Call("GetRangeByNumber", oArray.GetLength() - 1, oArray[0].GetLength() - 1);
    oWorksheet.Call("GetRange", startCell, endCell).Call("SetValue", oArray);

    // Save and close
    oBuilder.SaveFile(OFFICESTUDIO_FILE_SPREADSHEET_XLSX, resultPath);
    oBuilder.CloseFile();
    CDocBuilder::Dispose();
    return 0;
}
