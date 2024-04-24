/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
