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

#if defined __cplusplus

#include <string>
#include <iostream>
#include <memory.h>
#include <map>
#include <boost/make_shared.hpp>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <algorithm>
#include <unordered_map>
#include <stdio.h>
#include <stack>
#include <list>
#include <boost/shared_ptr.hpp>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//#include "../../../Base/Base.h"
//#include "../../../Base/Nullable.h"
//#include "../../../Common/ComplexTypes.h"
//#include "../../../Common/SimpleTypes_Shared.h"
//#include "../../../Common/SimpleTypes_Word.h"
//#include "../../../DocxFormat/Drawing/Drawing.h"
//#include "../../../DocxFormat/External/HyperLink.h"
//#include "../../../DocxFormat/Logic/Annotations.h"
//#include "../../../DocxFormat/Logic/Hyperlink.h"
//#include "../../../DocxFormat/Logic/Paragraph.h"
//#include "../../../DocxFormat/Logic/ParagraphProperty.h"
//#include "../../../DocxFormat/Logic/Run.h"
//#include "../../../DocxFormat/Logic/RunProperty.h"
//#include "../../../DocxFormat/Logic/Sdt.h"
//#include "../../../DocxFormat/Logic/Shape.h"
//#include "../../../DocxFormat/Logic/Table.h"
//#include "../../../DocxFormat/Media/Image.h"
//#include "../../../DocxFormat/Media/Media.h"
//#include "../../../DocxFormat/Media/OleObject.h"
//#include "../../../DocxFormat/App.h"
//#include "../../../DocxFormat/Comments.h"
//#include "../../../DocxFormat/Core.h"
//#include "../../../DocxFormat/CustomXml.h"
//#include "../../../DocxFormat/Document.h"
//#include "../../../DocxFormat/Docx.h"
//#include "../../../DocxFormat/DocxFlat.h"
//#include "../../../DocxFormat/File.h"
//#include "../../../DocxFormat/FileTypes.h"
//#include "../../../DocxFormat/IFileContainer.h"
//#include "../../../DocxFormat/RId.h"
//#include "../../../DocxFormat/Styles.h"
//#include "../../../DocxFormat/WritingElement.h"
//#include "../../../SystemUtility/SystemUtility.h"
//#include "../../../Base/Unit.h"
//#include "../../../XlsxFormat/Comments/Comments.h"
//#include "../../../XlsxFormat/Drawing/Drawing.h"
//#include "../../../XlsxFormat/SharedStrings/Run.h"
//#include "../../../XlsxFormat/SharedStrings/SharedStrings.h"
//#include "../../../XlsxFormat/Styles/rPr.h"
//#include "../../../XlsxFormat/Styles/Styles.h"
//#include "../../../XlsxFormat/Styles/TableStyles.h"
//#include "../../../XlsxFormat/Table/Table.h"
//#include "../../../XlsxFormat/Workbook/Workbook.h"
//#include "../../../XlsxFormat/Worksheets/Worksheet.h"
//#include "../../../XlsxFormat/Common.h"
//#include "../../../XlsxFormat/CommonInclude.h"
//#include "../../../XlsxFormat/FileTypes_Spreadsheet.h"
//#include "../../../XlsxFormat/WritingElement.h"
//#include "../../../XlsxFormat/Xlsx.h"
//#include "../../../DocxFormat/Diagram/DiagramData.h"

#endif
