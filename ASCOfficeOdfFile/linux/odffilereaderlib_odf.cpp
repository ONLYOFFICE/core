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
#include "../src/odf/abstract_xml.cpp"
#include "../src/odf/anim_elements.cpp"
#include "../src/odf/calcs_styles.cpp"
#include "../src/odf/chart_build_oox.cpp"
#include "../src/odf/createandread.cpp"
#include "../src/odf/documentcontext.cpp"
#include "../src/odf/draw_common.cpp"
#include "../src/odf/draw_frame.cpp"
#include "../src/odf/draw_frame_docx.cpp"
#include "../src/odf/draw_frame_pptx.cpp"
#include "../src/odf/draw_frame_xlsx.cpp"
#include "../src/odf/draw_page.cpp"
#include "../src/odf/draw_shapes.cpp"
#include "../src/odf/draw_shapes_docx.cpp"
#include "../src/odf/draw_shapes_pptx.cpp"
#include "../src/odf/draw_shapes_xlsx.cpp"
#include "../src/odf/font_face.cpp"
#include "../src/odf/header_footer.cpp"
#include "../src/odf/list.cpp"
#include "../src/odf/logging.cpp"
#include "../src/odf/note.cpp"
#include "../src/odf/number_style.cpp"
#include "../src/odf/odf_content_xml.cpp"
#include "../src/odf/odfcontext.cpp"
#include "../src/odf/odf_document.cpp"
#include "../src/odf/office_annotation.cpp"
#include "../src/odf/office_binary_data.cpp"
#include "../src/odf/office_body.cpp"
#include "../src/odf/office_chart.cpp"
#include "../src/odf/office_document.cpp"
#include "../src/odf/office_elements_create.cpp"
#include "../src/odf/office_event_listeners.cpp"
#include "../src/odf/office_presentation.cpp"
#include "../src/odf/office_scripts.cpp"
#include "../src/odf/office_forms.cpp"
#include "../src/odf/office_settings.cpp"
#include "../src/odf/office_spreadsheet.cpp"
#include "../src/odf/office_text.cpp"
#include "../src/odf/office_meta.cpp"
#include "../src/odf/paragraph_elements.cpp"
#include "../src/odf/ruby.cpp"
#include "../src/odf/search_table_cell.cpp"
#include "../src/odf/skipelement.cpp"
#include "../src/odf/style_chart_properties.cpp"
#include "../src/odf/style_graphic_properties.cpp"
#include "../src/odf/style_map.cpp"
#include "../src/odf/style_paragraph_properties.cpp"
#include "../src/odf/style_paragraph_properties_docx.cpp"
#include "../src/odf/style_paragraph_properties_pptx.cpp"
#include "../src/odf/style_presentation.cpp"
#include "../src/odf/style_regions.cpp"
#include "../src/odf/styles.cpp"
#include "../src/odf/styles_list.cpp"
#include "../src/odf/styles_lite_container.cpp"
#include "../src/odf/style_table_properties.cpp"
#include "../src/odf/style_text_properties.cpp"
#include "../src/odf/svg_parser.cpp"
#include "../src/odf/table.cpp"
#include "../src/odf/table_calculation_settings.cpp"
#include "../src/odf/table_docx.cpp"
#include "../src/odf/table_named_expressions.cpp"
#include "../src/odf/table_data_pilot_tables.cpp"
#include "../src/odf/table_pptx.cpp"
#include "../src/odf/table_xlsx.cpp"
#include "../src/odf/templates.cpp"
#include "../src/odf/text_content.cpp"
#include "../src/odf/text_elements.cpp"
#include "../src/odf/math_elementaries.cpp"
#include "../src/odf/math_layout_elements.cpp"
#include "../src/odf/math_limit_elements.cpp"
#include "../src/odf/math_table_elements.cpp"
#include "../src/odf/math_token_elements.cpp"
#include "../src/odf/datatypes/mathvariant.cpp"
#include "../src/odf/calcext_elements.cpp"
#include "../src/odf/table_database_ranges.cpp"
#include "../src/odf/math_elements.cpp"
#include "../src/odf/odf_document_impl.cpp"
