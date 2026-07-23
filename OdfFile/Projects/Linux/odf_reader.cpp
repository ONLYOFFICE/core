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

#include "../../Reader/Format/abstract_xml.cpp"
#include "../../Reader/Format/anim_elements.cpp"
#include "../../Reader/Format/calcs_styles.cpp"
#include "../../Reader/Format/chart_build_oox.cpp"
#include "../../Reader/Format/createandread.cpp"
#include "../../Reader/Format/documentcontext.cpp"
#include "../../Reader/Format/draw_common.cpp"
#include "../../Reader/Format/draw_frame.cpp"
#include "../../Reader/Format/draw_frame_docx.cpp"
#include "../../Reader/Format/draw_frame_pptx.cpp"
#include "../../Reader/Format/draw_frame_xlsx.cpp"
#include "../../Reader/Format/draw_page.cpp"
#include "../../Reader/Format/draw_shapes.cpp"
#include "../../Reader/Format/draw_shapes_docx.cpp"
#include "../../Reader/Format/draw_shapes_pptx.cpp"
#include "../../Reader/Format/draw_shapes_xlsx.cpp"
#include "../../Reader/Format/font_face.cpp"
#include "../../Reader/Format/header_footer.cpp"
#include "../../Reader/Format/list.cpp"
#include "../../Reader/Format/note.cpp"
#include "../../Reader/Format/number_style.cpp"
#include "../../Reader/Format/odf_content_xml.cpp"
#include "../../Reader/Format/odfcontext.cpp"
#include "../../Reader/Format/odf_document.cpp"
#include "../../Reader/Format/office_annotation.cpp"
#include "../../Reader/Format/office_binary_data.cpp"
#include "../../Reader/Format/office_body.cpp"
#include "../../Reader/Format/office_chart.cpp"
#include "../../Reader/Format/office_document.cpp"
#include "../../Reader/Format/office_elements_create.cpp"
#include "../../Reader/Format/office_event_listeners.cpp"
#include "../../Reader/Format/office_presentation.cpp"
#include "../../Reader/Format/office_drawing.cpp"
#include "../../Reader/Format/office_scripts.cpp"
#include "../../Reader/Format/office_forms.cpp"
#include "../../Reader/Format/office_settings.cpp"
#include "../../Reader/Format/office_spreadsheet.cpp"
#include "../../Reader/Format/office_text.cpp"
#include "../../Reader/Format/office_meta.cpp"
#include "../../Reader/Format/paragraph_elements.cpp"
#include "../../Reader/Format/ruby.cpp"
#include "../../Reader/Format/search_table_cell.cpp"
#include "../../Reader/Format/skipelement.cpp"
#include "../../Reader/Format/style_chart_properties.cpp"
#include "../../Reader/Format/style_graphic_properties.cpp"
#include "../../Reader/Format/style_map.cpp"
#include "../../Reader/Format/style_properties.cpp"
#include "../../Reader/Format/style_paragraph_properties.cpp"
#include "../../Reader/Format/style_paragraph_properties_docx.cpp"
#include "../../Reader/Format/style_paragraph_properties_pptx.cpp"
#include "../../Reader/Format/style_presentation.cpp"
#include "../../Reader/Format/style_regions.cpp"
#include "../../Reader/Format/styles.cpp"
#include "../../Reader/Format/styles_list.cpp"
#include "../../Reader/Format/styles_lite_container.cpp"
#include "../../Reader/Format/style_table_properties.cpp"
#include "../../Reader/Format/style_text_properties.cpp"
#include "../../Reader/Format/svg_parser.cpp"
#include "../../Reader/Format/table.cpp"
#include "../../Reader/Format/table_calculation_settings.cpp"
#include "../../Reader/Format/table_docx.cpp"
#include "../../Reader/Format/table_named_expressions.cpp"
#include "../../Reader/Format/table_data_pilot_tables.cpp"
#include "../../Reader/Format/table_pptx.cpp"
#include "../../Reader/Format/table_xlsx.cpp"
#include "../../Reader/Format/templates.cpp"
#include "../../Reader/Format/text_content.cpp"
#include "../../Reader/Format/text_elements.cpp"
#include "../../Reader/Format/math_elementaries.cpp"
#include "../../Reader/Format/math_layout_elements.cpp"
#include "../../Reader/Format/math_limit_elements.cpp"
#include "../../Reader/Format/math_table_elements.cpp"
#include "../../Reader/Format/math_token_elements.cpp"
#include "../../Reader/Format/calcext_elements.cpp"
#include "../../Reader/Format/table_database_ranges.cpp"
#include "../../Reader/Format/math_elements.cpp"
#include "../../Reader/Format/odf_document_impl.cpp"
