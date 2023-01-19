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
#include "../Common/utils.cpp"

#include "../XlsXlsxConverter/xlsx_comments.cpp""
#include "../XlsXlsxConverter/xlsx_comments_context.cpp"
#include "../XlsXlsxConverter/ConvertXls2Xlsx.cpp"
#include "../XlsXlsxConverter/external_items.cpp"
#include "../XlsXlsxConverter/mediaitems_utils.cpp"
#include "../XlsXlsxConverter/namespaces.cpp"
#include "../XlsXlsxConverter/oox_content_type.cpp"
#include "../XlsXlsxConverter/oox_package.cpp"
#include "../XlsXlsxConverter/oox_rels.cpp"
#include "../XlsXlsxConverter/XlsConverter.cpp"
#include "../XlsXlsxConverter/xlsx_conversion_context.cpp"
#include "../XlsXlsxConverter/xlsx_drawing_context.cpp"
#include "../XlsXlsxConverter/xlsx_drawings.cpp"
#include "../XlsXlsxConverter/xlsx_hyperlinks.cpp"
#include "../XlsXlsxConverter/xlsx_output_xml.cpp"
#include "../XlsXlsxConverter/xlsx_package.cpp"
#include "../XlsXlsxConverter/xlsx_protection.cpp"
#include "../XlsXlsxConverter/xlsx_tablecontext.cpp"
#include "../XlsXlsxConverter/xlsx_textcontext.cpp"
#include "../XlsXlsxConverter/xlsx_chart_context.cpp"
#include "../XlsXlsxConverter/ConvertShapes/FormulaShape.cpp"
#include "../XlsXlsxConverter/ConvertShapes/CustomShape.cpp"
#include "../XlsXlsxConverter/ConvertShapes/BaseShape_1.cpp"
