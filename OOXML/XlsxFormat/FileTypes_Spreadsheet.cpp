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
#include "./FileTypes_Spreadsheet.h"

namespace OOX
{
	namespace Spreadsheet
	{
		namespace FileTypes
		{
			const FileType Workbook			(L"xl", L"workbook.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet.main+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

			const FileType WorkbookMacro	(L"xl", L"workbook.xml",
												L"application/vnd.ms-excel.sheet.macroEnabled.main+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

			const FileType SharedStrings	(L"", L"sharedStrings.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.sharedStrings+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings");

			const FileType Styles			(L"", L"styles.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.styles+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles");

			const FileType Worksheet		(L"worksheets", L"sheet.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.worksheet+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet",
												L"worksheets/sheet", true);

			const FileType Chartsheets		(L"chartsheets", L"sheet.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.chartsheet+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet", 
												L"chartsheets/sheet", true);

			const FileType CalcChain		(L"", L"calcChain.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.calcChain+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/calcChain");

			const FileType ThreadedComments	(L"../threadedComments", L"threadedComment.xml",
												L"application/vnd.ms-excel.threadedcomments+xml",
												L"http://schemas.microsoft.com/office/2017/10/relationships/threadedComment", 
												L"threadedComments/threadedComment", true, true);

			const FileType Persons			(L"persons", L"person.xml",
												L"application/vnd.ms-excel.person+xml",
												L"http://schemas.microsoft.com/office/2017/10/relationships/person");

			const FileType Comments			(L"../", L"comments.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.comments+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments", 
												L"comments", true, true);

			const FileType Table			(L"../tables", L"table.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.table+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/table", 
												L"tables/table", true, true);

			const FileType QueryTable		(L"../queryTables", L"queryTable.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.queryTable+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/queryTable", 
												L"queryTables/queryTable", true, true);

			const FileType Connections		(L"", L"connections.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.connections+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/connections");

			const FileType CtrlProp			(L"../ctrlProps", L"ctrlProp.xml",
												L"application/vnd.ms-excel.controlproperties+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/ctrlProp", 
												L"ctrlProps/ctrlProp", true, true);

			const FileType Drawings			(L"../drawings", L"drawing.xml",
												L"application/vnd.openxmlformats-officedocument.drawing+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/drawing", 
												L"drawings/drawing", true, true);

			const FileType ExternalLinks	(L"externalLinks", L"externalLink.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.externalLink+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLink",
												L"externalLinks/externalLink", true, true);

			const FileType PivotTable		(L"../pivotTables", L"pivotTable.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.pivotTable+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable",
												L"pivotTables/pivotTable", true, true);

			const FileType PivotCacheDefinition(L"pivotCache", L"pivotCacheDefinition.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.pivotCacheDefinition+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition",
												L"pivotCache/pivotCacheDefinition", true, true);

			const FileType PivotCacheRecords(L"", L"pivotCacheRecords.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.pivotCacheRecords+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords",
												L"pivotCache/pivotCacheRecords", true, true);

			const FileType SlicerCache		(L"slicerCaches", L"slicerCache.xml",
												L"application/vnd.ms-excel.slicerCache+xml",
												L"http://schemas.microsoft.com/office/2007/relationships/slicerCache",
												L"slicerCaches/slicerCache", true, true);

			const FileType Slicer			(L"../slicers", L"slicer.xml",
												L"application/vnd.ms-excel.slicer+xml",
												L"http://schemas.microsoft.com/office/2007/relationships/slicer",
												L"slicers/slicer", true, true);

			const FileType NamedSheetView	(L"../namedSheetViews", L"namedSheetView.xml",
												L"application/vnd.ms-excel.namedsheetviews+xml",
												L"http://schemas.microsoft.com/office/2019/04/relationships/namedSheetView",
												L"namedSheetViews/namedSheetView", true, true);

			const FileType XlBinaryIndex	(L"worksheets", L"binaryIndex.bin",
												L"application/vnd.ms-excel.binIndexW",
												L"http://schemas.microsoft.com/office/2006/relationships/xlBinaryIndex",
												L"worksheets/binaryIndex", true);

			//onlyoffice workbook comments
			const FileType WorkbookComments(L"", L"workbookComments.bin",
												L"",
												L"http://schemas.onlyoffice.com/workbookComments");


			const FileType SpreadsheetFlat(L"", L"", L"", L"");		

		} // namespace FileTypes
	}
} // namespace OOX
