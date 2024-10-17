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
#pragma once

#include "../../DocxFormat/FileType.h"


namespace OOX
{
	namespace SpreadsheetBin
	{
		namespace FileTypes
		{

			const FileType WorkbookBin			(L"xl", L"workbook.bin",
													L"application/vnd.ms-excel.sheet.binary.macroEnabled.main",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/officeDocument");

			const FileType SharedStringsBin		(L"", L"sharedStrings.bin",
													L"application/vnd.ms-excel.sharedStrings",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sharedStrings");

			const FileType StylesBin			(L"", L"styles.bin",
													L"application/vnd.ms-excel.styles",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/styles");

			const FileType WorksheetBin			(L"worksheets", L"sheet.bin",
													L"application/vnd.ms-excel.worksheet",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/worksheet",
													L"worksheets/sheet", true);

			const FileType ChartsheetsBin		(L"chartsheets", L"sheet.bin",
													L"application/vnd.ms-excel.chartsheet",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chartsheet",
													L"chartsheets/sheet", true);

			const FileType TableBin				(L"../tables", L"table.bin",
													L"application/vnd.ms-excel.table",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/table",
													L"tables/table", true, true);

			const FileType QueryTableBin		(L"../queryTables", L"queryTable.bin",
													L"application/vnd.ms-excel.queryTable",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/queryTable",
													L"queryTables/queryTable", true, true);

			const FileType ConnectionsBin		(L"", L"connections.bin",
													L"application/vnd.ms-excel.connections",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/connections");

			const FileType CommentsBin			(L"../", L"comments.bin",
													L"application/vnd.ms-excel.comments",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/comments",
													L"comments", true, true);

			const FileType SlicerCacheBin		(L"slicerCaches", L"slicerCache.bin",
													L"application/vnd.ms-excel.slicerCache",
													L"http://schemas.microsoft.com/office/2007/relationships/slicerCache",
													L"slicerCaches/slicerCache", true, true);

			const FileType SlicerBin			(L"../slicers", L"slicer.bin",
													L"application/vnd.ms-excel.slicer",
													L"http://schemas.microsoft.com/office/2007/relationships/slicer",
													L"slicers/slicer", true, true);

			const FileType ExternalLinksBin		(L"externalLinks", L"externalLink.bin",
													L"application/vnd.ms-excel.externalLink",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/externalLink",
													L"externalLinks/externalLink", true, true);

			const FileType PivotTableBin		(L"../pivotTables", L"pivotTable.bin",
													L"application/vnd.ms-excel.pivotTable",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotTable",
													L"pivotTables/pivotTable", true, true);

			const FileType PivotCacheDefinitionBin(L"pivotCache", L"pivotCacheDefinition.bin",
														L"application/vnd.ms-excel.pivotCacheDefinition",
														L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheDefinition",
														L"pivotCache/pivotCacheDefinition", true, true);

			const FileType PivotCacheRecordsBin	(L"", L"pivotCacheRecords.bin",
													L"application/vnd.ms-excel.pivotCacheRecords",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/pivotCacheRecords",
													L"pivotCache/pivotCacheRecords", true, true);

			const FileType MetadataBin	(L"", L"metadata.bin",
													L"application/vnd.ms-excel.sheetMetadata",
													L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/sheetMetadata");

		} // namespace FileTypes
	}
} // namespace OOX
