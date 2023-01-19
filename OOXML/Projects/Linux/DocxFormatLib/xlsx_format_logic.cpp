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

#include "../../../XlsxFormat/CalcChain/CalcChain.cpp"
#include "../../../XlsxFormat/Chart/Chart.cpp"
#include "../../../XlsxFormat/Chart/ChartColors.cpp"
#include "../../../XlsxFormat/Chart/ChartDrawing.cpp"
#include "../../../XlsxFormat/Chart/ChartStyle.cpp"
#include "../../../XlsxFormat/Chart/ChartSerialize.cpp"
#include "../../../XlsxFormat/Chart/ChartSerializeEx.cpp"
#include "../../../XlsxFormat/Common.cpp"
#include "../../../XlsxFormat/FileFactory_Spreadsheet.cpp"
#include "../../../XlsxFormat/ComplexTypes_Spreadsheet.cpp"
#include "../../../XlsxFormat/Xlsx.cpp"
#include "../../../XlsxFormat/XlsxFlat.cpp"
#include "../../../XlsxFormat/FileTypes_Spreadsheet.cpp"
#include "../../../XlsxFormat/Worksheets/SheetData.cpp"
#include "../../../XlsxFormat/Worksheets/Worksheet.cpp"
#include "../../../XlsxFormat/Worksheets/ConditionalFormatting.cpp"
#include "../../../XlsxFormat/Worksheets/DataValidation.cpp"
#include "../../../XlsxFormat/Worksheets/Cols.cpp"
#include "../../../XlsxFormat/Worksheets/XlsxHyperlinks.cpp"
#include "../../../XlsxFormat/Worksheets/MergeCells.cpp"
#include "../../../XlsxFormat/Worksheets/Sparkline.cpp"
#include "../../../XlsxFormat/Worksheets/WorksheetChildOther.cpp"
#include "../../../XlsxFormat/Table/Tables.cpp"
#include "../../../XlsxFormat/Table/Autofilter.cpp"
#include "../../../XlsxFormat/Table/Connections.cpp"
#include "../../../XlsxFormat/Controls/Controls.cpp"
#include "../../../XlsxFormat/Styles/rPr.cpp"
#include "../../../XlsxFormat/Styles/XlsxStyles.cpp"
#include "../../../XlsxFormat/Styles/Borders.cpp"
#include "../../../XlsxFormat/Styles/CellStyles.cpp"
#include "../../../XlsxFormat/Styles/Colors.cpp"
#include "../../../XlsxFormat/Styles/dxf.cpp"
#include "../../../XlsxFormat/Styles/Fills.cpp"
#include "../../../XlsxFormat/Styles/Fonts.cpp"
#include "../../../XlsxFormat/Styles/NumFmts.cpp"
#include "../../../XlsxFormat/Styles/TableStyles.cpp"
#include "../../../XlsxFormat/Styles/Xfs.cpp"
#include "../../../XlsxFormat/SharedStrings/Si.cpp"
#include "../../../XlsxFormat/SharedStrings/Text.cpp"
#include "../../../XlsxFormat/Pivot/Pivots.cpp"
#include "../../../XlsxFormat/Slicer/SlicerCache.cpp"
#include "../../../XlsxFormat/Slicer/SlicerCacheExt.cpp"
#include "../../../XlsxFormat/Slicer/Slicer.cpp"
#include "../../../XlsxFormat/NamedSheetViews/NamedSheetViews.cpp"
#include "../../../XlsxFormat/Pivot/PivotCacheDefinitionExt.cpp"
#include "../../../XlsxFormat/Workbook/Workbook.cpp"
#include "../../../XlsxFormat/Workbook/BookViews.cpp"
#include "../../../XlsxFormat/Workbook/CalcPr.cpp"
#include "../../../XlsxFormat/Workbook/DefinedNames.cpp"
#include "../../../XlsxFormat/Workbook/ExternalReferences.cpp"
#include "../../../XlsxFormat/Workbook/Sheets.cpp"
#include "../../../XlsxFormat/Workbook/WorkbookPr.cpp"
#include "../../../XlsxFormat/Comments/XlsxComments.cpp"
#include "../../../XlsxFormat/Comments/ThreadedComments.cpp"
#include "../../../XlsxFormat/Drawing/CellAnchor.cpp"
#include "../../../XlsxFormat/Drawing/XlsxDrawing.cpp"
#include "../../../XlsxFormat/Drawing/FromTo.cpp"
#include "../../../XlsxFormat/Drawing/Pos.cpp"
#include "../../../XlsxFormat/ExternalLinks/ExternalLinkPath.cpp"
#include "../../../XlsxFormat/ExternalLinks/ExternalLinks.cpp"
#include "../../../XlsxFormat/Ole/OleObjects.cpp"
#include "../../../XlsxFormat/SharedStrings/PhoneticPr.cpp"
#include "../../../XlsxFormat/SharedStrings/XlsxRun.cpp"
#include "../../../XlsxFormat/SharedStrings/SharedStrings.cpp"
