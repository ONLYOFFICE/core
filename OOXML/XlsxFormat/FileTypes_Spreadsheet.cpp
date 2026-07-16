/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

			const FileType TableSingleCells	(L"../tables", L"tableSingleCells.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.tableSingleCells+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/tableSingleCells",
												L"tables/tableSingleCells", true, true);

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

			const FileType Timeline			(L"../timelines", L"timeline.xml",
												L"application/vnd.ms-excel.timeline+xml",
												L"http://schemas.microsoft.com/office/2011/relationships/timeline",
												L"timelines/timeline", true, true);

			const FileType TimelineCache	(L"timelineCaches", L"timelineCache.xml",
												L"application/vnd.ms-excel.timelineCache+xml",
												L"http://schemas.microsoft.com/office/2011/relationships/timelineCache",
												L"timelineCaches/timelineCache", true, true);
			//onlyoffice workbook comments
			const FileType WorkbookComments	(L"", L"workbookComments.bin",
												L"",
												L"http://schemas.onlyoffice.com/workbookComments");
			
			const FileType Metadata			(L"", L"metadata.xml",
												L"application/vnd.openxmlformats-officedocument.spreadsheetml.sheetMetadata+xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sheetMetadata");


			const FileType RdRichValue		(L"richData", L"rdrichvalue.xml",
												L"application/vnd.ms-excel.rdrichvalue+xml",
												L"http://schemas.microsoft.com/office/2017/06/relationships/rdRichValue");

			const FileType RdRichValueStructure(L"richData", L"rdrichvaluestructure.xml",
												L"application/vnd.ms-excel.rdrichvaluestructure+xml",
												L"http://schemas.microsoft.com/office/2017/06/relationships/rdRichValueStructure");

			const FileType RdRichValueTypes	(L"richData", L"rdRichValueTypes.xml",
												L"application/vnd.ms-excel.rdrichvaluetypes+xml",
												L"http://schemas.microsoft.com/office/2017/06/relationships/rdRichValueTypes");

			const FileType XmlMaps			(L"", L"xmlMaps.xml",
												L"application/xml",
												L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/xmlMaps");

			const FileType RichValueRel	(L"richData", L"richValueRel.xml",
												L"application/vnd.ms-excel.richvaluerel+xml",
												L"http://schemas.microsoft.com/office/2022/10/relationships/richValueRel");

			const FileType SpreadsheetFlat	(L"", L"", L"", L"");

		} // namespace FileTypes
	}
} // namespace OOX
