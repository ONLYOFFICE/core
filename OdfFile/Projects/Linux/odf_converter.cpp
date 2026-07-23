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

#include "../../Reader/Converter/ConvertOO2OOX.cpp"
#include "../../Reader/Converter/conversionelement.cpp"
#include "../../Reader/Converter/xlsx_conditionalFormatting.cpp"
#include "../../Reader/Converter/xlsx_dxfs.cpp"
#include "../../Reader/Converter/docx_content_type.cpp"
#include "../../Reader/Converter/docx_conversion_context.cpp"
#include "../../Reader/Converter/docx_drawing.cpp"
#include "../../Reader/Converter/docx_package.cpp"
#include "../../Reader/Converter/docx_table_context.cpp"
#include "../../Reader/Converter/headers_footers.cpp"
#include "../../Reader/Converter/hyperlinks.cpp"
#include "../../Reader/Converter/measuredigits.cpp"
#include "../../Reader/Converter/mediaitems.cpp"
#include "../../Reader/Converter/namespaces.cpp"
#include "../../Reader/Converter/oox_chart_axis.cpp"
#include "../../Reader/Converter/oox_chart_context.cpp"
#include "../../Reader/Converter/oox_chart_legend.cpp"
#include "../../Reader/Converter/oox_chart_series.cpp"
#include "../../Reader/Converter/oox_chart_shape.cpp"
#include "../../Reader/Converter/oox_conversion_context.cpp"
#include "../../Reader/Converter/oox_data_labels.cpp"
#include "../../Reader/Converter/oox_drawing.cpp"
#include "../../Reader/Converter/oox_drawing_fills.cpp"
#include "../../Reader/Converter/oox_layout.cpp"
#include "../../Reader/Converter/oox_package.cpp"
#include "../../Reader/Converter/oox_plot_area.cpp"
#include "../../Reader/Converter/oox_title.cpp"
#include "../../Reader/Converter/oox_types_chart.cpp"
#include "../../Reader/Converter/oox_rels.cpp"
#include "../../Reader/Converter/pptx_animation_context.cpp"
#include "../../Reader/Converter/pptx_comments.cpp"
#include "../../Reader/Converter/pptx_comments_context.cpp"
#include "../../Reader/Converter/pptx_conversion_context.cpp"
#include "../../Reader/Converter/pptx_drawing.cpp"
#include "../../Reader/Converter/pptx_drawings.cpp"
#include "../../Reader/Converter/pptx_output_xml.cpp"
#include "../../Reader/Converter/pptx_package.cpp"
#include "../../Reader/Converter/pptx_slide_context.cpp"
#include "../../Reader/Converter/pptx_table_context.cpp"
#include "../../Reader/Converter/pptx_text_context.cpp"
#include "../../Reader/Converter/xlsx_alignment.cpp"
#include "../../Reader/Converter/xlsx_border.cpp"
#include "../../Reader/Converter/xlsx_borders.cpp"
#include "../../Reader/Converter/xlsx_cell_format.cpp"
#include "../../Reader/Converter/xlsx_cell_style.cpp"
#include "../../Reader/Converter/xlsx_cell_styles.cpp"
#include "../../Reader/Converter/xlsx_color.cpp"
#include "../../Reader/Converter/xlsx_comments.cpp"
#include "../../Reader/Converter/xlsx_comments_context.cpp"
#include "../../Reader/Converter/xlsx_complex_number_format.cpp"
#include "../../Reader/Converter/xlsxconversioncontext.cpp"
#include "../../Reader/Converter/xlsx_defined_names.cpp"
#include "../../Reader/Converter/xlsx_drawing.cpp"
#include "../../Reader/Converter/xlsx_drawing_context.cpp"
#include "../../Reader/Converter/xlsx_drawings.cpp"
#include "../../Reader/Converter/xlsx_fill.cpp"
#include "../../Reader/Converter/xlsx_fills.cpp"
#include "../../Reader/Converter/xlsx_font.cpp"
#include "../../Reader/Converter/xlsx_fonts.cpp"
#include "../../Reader/Converter/xlsx_hyperlinks.cpp"
#include "../../Reader/Converter/xlsx_merge_cells.cpp"
#include "../../Reader/Converter/xlsx_numFmts.cpp"
#include "../../Reader/Converter/xlsx_num_format_context.cpp"
#include "../../Reader/Converter/xlsx_output_xml.cpp"
#include "../../Reader/Converter/xlsx_package.cpp"
#include "../../Reader/Converter/xlsx_protection.cpp"
#include "../../Reader/Converter/xlsx_sharedstrings.cpp"
#include "../../Reader/Converter/xlsx_styles.cpp"
#include "../../Reader/Converter/xlsx_tablecontext.cpp"
#include "../../Reader/Converter/xlsx_table_metrics.cpp"
#include "../../Reader/Converter/xlsx_table_state.cpp"
#include "../../Reader/Converter/xlsx_textcontext.cpp"
#include "../../Reader/Converter/xlsx_utils.cpp"
#include "../../Reader/Converter/xlsx_xf.cpp"
#include "../../Reader/Converter/xlsx_pivots_context.cpp"
#include "../../Reader/Converter/xlsx_data_validation.cpp"
#include "../../Reader/Converter/StarMath2OOXML/cooxml2odf.cpp"
#include "../../Reader/Converter/StarMath2OOXML/shakey.cpp"
