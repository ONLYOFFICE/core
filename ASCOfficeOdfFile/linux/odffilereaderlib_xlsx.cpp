/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../src/docx/xlsx_utils.cpp"
#include "../src/odf/draw_frame_xlsx.cpp"
#include "../src/odf/draw_shapes_xlsx.cpp"
#include "../src/odf/table_xlsx.cpp"
#include "../src/docx/xlsx_alignment.cpp"
#include "../src/docx/xlsx_border.cpp"
#include "../src/docx/xlsx_borders.cpp"
#include "../src/docx/xlsx_cell_format.cpp"
#include "../src/docx/xlsx_cell_style.cpp"
#include "../src/docx/xlsx_cell_styles.cpp"
#include "../src/docx/xlsx_color.cpp"
#include "../src/docx/xlsx_comments.cpp"
#include "../src/docx/xlsx_comments_context.cpp"
#include "../src/docx/xlsx_complex_number_format.cpp"
#include "../src/docx/xlsxconversioncontext.cpp"
#include "../src/docx/xlsx_defined_names.cpp"
#include "../src/docx/xlsx_drawing.cpp"
#include "../src/docx/xlsx_drawing_context.cpp"
#include "../src/docx/xlsx_drawings.cpp"
#include "../src/docx/xlsx_fill.cpp"
#include "../src/docx/xlsx_fills.cpp"
#include "../src/docx/xlsx_font.cpp"
#include "../src/docx/xlsx_fonts.cpp"
#include "../src/docx/xlsx_hyperlinks.cpp"
#include "../src/docx/xlsx_merge_cells.cpp"
#include "../src/docx/xlsx_numFmts.cpp"
#include "../src/docx/xlsx_num_format_context.cpp"
#include "../src/docx/xlsx_output_xml.cpp"
#include "../src/docx/xlsx_package.cpp"
#include "../src/docx/xlsx_protection.cpp"
#include "../src/docx/xlsx_sharedstrings.cpp"
#include "../src/docx/xlsx_styles.cpp"
#include "../src/docx/xlsx_tablecontext.cpp"
#include "../src/docx/xlsx_table_metrics.cpp"
#include "../src/docx/xlsx_table_state.cpp"
#include "../src/docx/xlsx_textcontext.cpp"
#include "../src/docx/xlsx_xf.cpp"