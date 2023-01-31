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

#include "../../Writer/Format/office_document.cpp"
#include "../../Writer/Format/office_forms.cpp"
#include "../../Writer/Format/calcext_elements.cpp"
#include "../../Writer/Format/draw_base.cpp"
#include "../../Writer/Format/draw_frame.cpp"
#include "../../Writer/Format/draw_shapes.cpp"
#include "../../Writer/Format/header_footer.cpp"
#include "../../Writer/Format/list.cpp"
#include "../../Writer/Format/mediaitems.cpp"
#include "../../Writer/Format/number_style.cpp"
#include "../../Writer/Format/object_package.cpp"
#include "../../Writer/Format/odf_chart_context.cpp"
#include "../../Writer/Format/odf_comment_context.cpp"
#include "../../Writer/Format/odf_controls_context.cpp"
#include "../../Writer/Format/odf_notes_context.cpp"
#include "../../Writer/Format/odf_conversion_context.cpp"
#include "../../Writer/Format/odf_drawing_context.cpp"
#include "../../Writer/Format/odf_lists_styles_context.cpp"
#include "../../Writer/Format/odf_number_styles_context.cpp"
#include "../../Writer/Format/odf_page_layout_context.cpp"
#include "../../Writer/Format/odf_page_layout_state.cpp"
#include "../../Writer/Format/odf_rels.cpp"
#include "../../Writer/Format/odf_style_context.cpp"
#include "../../Writer/Format/odf_style_state.cpp"
#include "../../Writer/Format/odf_table_context.cpp"
#include "../../Writer/Format/odf_table_styles_context.cpp"
#include "../../Writer/Format/odf_text_context.cpp"
#include "../../Writer/Format/ods_conversion_context.cpp"
#include "../../Writer/Format/ods_table_context.cpp"
#include "../../Writer/Format/ods_table_state.cpp"
#include "../../Writer/Format/odt_conversion_context.cpp"
#include "../../Writer/Format/office_annotation.cpp"
#include "../../Writer/Format/office_body.cpp"
#include "../../Writer/Format/office_chart.cpp"
#include "../../Writer/Format/office_elements_create.cpp"
#include "../../Writer/Format/office_spreadsheet.cpp"
#include "../../Writer/Format/office_text.cpp"
#include "../../Writer/Format/oox_shape_defines.cpp"
#include "../../Writer/Format/paragraph_elements.cpp"
#include "../../Writer/Format/style_chart_properties.cpp"
#include "../../Writer/Format/style_graphic_properties.cpp"
#include "../../Writer/Format/style_map.cpp"
#include "../../Writer/Format/style_page_layout_properties.cpp"
#include "../../Writer/Format/style_paragraph_properties.cpp"
#include "../../Writer/Format/styles.cpp"
#include "../../Writer/Format/style_section_properties.cpp"
#include "../../Writer/Format/styles_list.cpp"
#include "../../Writer/Format/styles_lite_container.cpp"
#include "../../Writer/Format/style_table_properties.cpp"
#include "../../Writer/Format/style_text_properties.cpp"
#include "../../Writer/Format/svg_creator.cpp"
#include "../../Writer/Format/table.cpp"
#include "../../Writer/Format/table_database_ranges.cpp"
#include "../../Writer/Format/table_data_pilot_tables.cpp"
#include "../../Writer/Format/table_named_expressions.cpp"
#include "../../Writer/Format/text_elements.cpp"
#include "../../Writer/Format/odf_settings_context.cpp"
#include "../../Writer/Format/office_settings.cpp"
#include "../../Writer/Format/mediaitems_utils.cpp"
#include "../../Writer/Format/anim_elements.cpp"
#include "../../Writer/Format/draw_page.cpp"
#include "../../Writer/Format/odp_conversion_context.cpp"
#include "../../Writer/Format/odp_page_state.cpp"
#include "../../Writer/Format/odp_slide_context.cpp"
#include "../../Writer/Format/office_presentation.cpp"
#include "../../Writer/Format/style_presentation.cpp"
#include "../../Writer/Format/office_scripts.cpp"
#include "../../Writer/Format/office_event_listeners.cpp"
#include "../../Writer/Format/odf_math_context.cpp"
#include "../../Writer/Format/math_elementaries.cpp"
#include "../../Writer/Format/math_elements.cpp"
#include "../../Writer/Format/math_layout_elements.cpp"
#include "../../Writer/Format/math_limit_elements.cpp"
#include "../../Writer/Format/math_table_elements.cpp"
#include "../../Writer/Format/math_token_elements.cpp"
