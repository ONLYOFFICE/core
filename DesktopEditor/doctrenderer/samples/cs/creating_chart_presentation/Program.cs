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

using docbuilder_net;

using OfficeFileTypes = docbuilder_net.FileTypes;
using CValue = docbuilder_net.CDocBuilderValue;
using CContext = docbuilder_net.CDocBuilderContext;
using CContextScope = docbuilder_net.CDocBuilderContextScope;

namespace Sample
{
    public class CreatingChartPresentation
    {
        public static void Main(string[] args)
        {
            string workDirectory = "C:/Program Files/ONLYOFFICE/DocumentBuilder";
            string resultPath = "../../../result.pptx";
            string filePath = "../../../../../../resources/docs/chart_data.xlsx";

            // add Docbuilder dlls in path
            System.Environment.SetEnvironmentVariable("PATH", System.Environment.GetEnvironmentVariable("PATH") + ";" + workDirectory);

            CreateChartPresentation(workDirectory, resultPath, filePath);
        }

        public static void CreateChartPresentation(string workDirectory, string resultPath, string filePath)
        {
            var doctype = (int)OfficeFileTypes.Presentation.PPTX;

            // Init DocBuilder
            CDocBuilder.Initialize(workDirectory);
            CDocBuilder oBuilder = new CDocBuilder();

            // Read chart data from xlsx
            oBuilder.OpenFile(filePath, "xlsx");
            CContext oContext = oBuilder.GetContext();
            CContextScope oScope = oContext.CreateScope();
            CValue oGlobal = oContext.GetGlobal();
            CValue oApi = oGlobal["Api"];
            CValue oWorksheet = oApi.Call("GetActiveSheet");
            CValue oRange = oWorksheet.Call("GetUsedRange").Call("GetValue");
            object[,] array = oRangeTo2dArray(oRange, oContext);
            oBuilder.CloseFile();

            // Create chart presentation
            oBuilder.CreateFile(doctype);
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
            CValue array_cols = colsFromArray(array, oContext);
            CValue array_rows = rowsFromArray(array, oContext);
            CValue array_data = dataFromArray(array, oContext);

            // Pass CValue data to the CreateChart method
            CValue oChart = oApi.Call("CreateChart", "lineStacked", array_data, array_cols, array_rows);
            oChart.Call("SetSize", 180 * 36000, 100 * 36000);
            oChart.Call("SetPosition", 20 * 36000, 50 * 36000);
            oChart.Call("ApplyChartStyle", 24);
            oChart.Call("SetVertAxisLabelsFontSize", 16);
            oChart.Call("SetHorAxisLabelsFontSize", 16);
            oChart.Call("SetLegendFontSize", 16);
            oChart.Call("SetLegendPos", "top");
            oSlide.Call("AddObject", oChart);

            // Save file and close DocBuilder
            oBuilder.SaveFile(doctype, resultPath);
            oBuilder.CloseFile();
            CDocBuilder.Destroy();
        }

        public static object[,] oRangeTo2dArray(CValue oRange, CContext oContext)
        {
            int rowsLen = (int)oRange.GetLength();
            int colsLen = (int)oRange[0].GetLength();
            object[,] oArray = new object[rowsLen, colsLen];

            for (int col = 0; col < colsLen; col++)
            {
                CValue oArrayRow = oContext.CreateArray(rowsLen);
                for (int row = 0; row < rowsLen; row++)
                {
                    oArray[row, col] = oRange[row][col].ToString();
                }
            }
            return oArray;
        }

        public static CValue colsFromArray(object[,] array, CContext oContext)
        {
            int colsLen = array.GetLength(1) - 1;
            CValue cols = oContext.CreateArray(colsLen);
            for (int col = 1; col <= colsLen; col++)
            {
                cols[col - 1] = array[0, col].ToString();
            }
            return cols;
        }

        public static CValue rowsFromArray(object[,] array, CContext oContext)
        {
            int rowsLen = array.GetLength(0) - 1;
            CValue rows = oContext.CreateArray(rowsLen);
            for (int row = 1; row <= rowsLen; row++)
            {
                rows[row - 1] = array[row, 0].ToString();
            }
            return rows;
        }

        public static CValue dataFromArray(object[,] array, CContext oContext)
        {
            int colsLen = array.GetLength(0) - 1;
            int rowsLen = array.GetLength(1) - 1;
            CValue data = oContext.CreateArray(rowsLen);
            for (int row = 1; row <= rowsLen; row++)
            {
                CValue row_data = oContext.CreateArray(colsLen);
                for (int col = 1; col <= colsLen; col++)
                {
                    row_data[col - 1] = array[col, row].ToString();
                }
                data[row - 1] = row_data;
            }
            return data;
        }

        public static void addText(CValue oApi, int fontSize, string text, CValue oSlide, CValue oShape, CValue oParagraph, CValue oFill, string jc)
        {
            CValue oRun = oApi.Call("CreateRun");
            var oTextPr = oRun.Call("GetTextPr");
            oTextPr.Call("SetFontSize", fontSize);
            oTextPr.Call("SetFill", oFill);
            oTextPr.Call("SetFontFamily", "Tahoma");
            oParagraph.Call("SetJc", jc);
            oRun.Call("AddText", text);
            oRun.Call("AddLineBreak");
            oParagraph.Call("AddElement", oRun);
            oSlide.Call("AddObject", oShape);
        }
    }
}
