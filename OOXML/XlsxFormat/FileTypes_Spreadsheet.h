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
#pragma once
#include "../DocxFormat/FileType.h"

namespace OOX
{
	namespace Spreadsheet
	{
		namespace FileTypes
		{
			extern const FileType Workbook;

			extern const FileType WorkbookMacro;

			extern const FileType SharedStrings;

			extern const FileType Styles;

			extern const FileType Worksheet;

			extern const FileType Chartsheets;

			extern const FileType CalcChain;

			extern const FileType ThreadedComments;

			extern const FileType Persons;

			extern const FileType Comments;

			extern const FileType Table;

			extern const FileType TableSingleCells;

			extern const FileType QueryTable;

			extern const FileType Connections;

			extern const FileType CtrlProp;

			extern const FileType Drawings;

			extern const FileType ExternalLinks;

			extern const FileType PivotTable;

			extern const FileType PivotCacheDefinition;

			extern const FileType PivotCacheRecords;

			extern const FileType SlicerCache;

			extern const FileType Slicer;

			extern const FileType NamedSheetView;

			extern const FileType XlBinaryIndex;

			//onlyoffice workbook comments
			extern const FileType WorkbookComments;

			extern const FileType SpreadsheetFlat;

			extern const FileType Timeline;

			extern const FileType TimelineCache;

			extern const FileType Metadata;

			extern const FileType RdRichValue;

			extern const FileType RdRichValueStructure;

			extern const FileType RdRichValueTypes;

			extern const FileType RichValueRel;
			
			extern const FileType XmlMaps;
		} // namespace FileTypes
	}
} // namespace OOX
