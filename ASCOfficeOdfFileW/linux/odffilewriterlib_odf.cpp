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

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif

#include "../source/OdfFormat/office_document.cpp"
#include "../source/OdfFormat/office_forms.cpp"
#include "../source/OdfFormat/calcext_elements.cpp"
#include "../source/OdfFormat/draw_base.cpp"
#include "../source/OdfFormat/draw_frame.cpp"
#include "../source/OdfFormat/draw_shapes.cpp"
#include "../source/OdfFormat/header_footer.cpp"
#include "../source/OdfFormat/list.cpp"
#include "../source/OdfFormat/mediaitems.cpp"
#include "../source/OdfFormat/number_style.cpp"
#include "../source/OdfFormat/object_package.cpp"
#include "../source/OdfFormat/odf_chart_context.cpp"
#include "../source/OdfFormat/odf_comment_context.cpp"
#include "../source/OdfFormat/odf_controls_context.cpp"
#include "../source/OdfFormat/odf_notes_context.cpp"
#include "../source/OdfFormat/odf_conversion_context.cpp"
#include "../source/OdfFormat/odf_drawing_context.cpp"
#include "../source/OdfFormat/odf_lists_styles_context.cpp"
#include "../source/OdfFormat/odf_number_styles_context.cpp"
#include "../source/OdfFormat/odf_page_layout_context.cpp"
#include "../source/OdfFormat/odf_page_layout_state.cpp"
#include "../source/OdfFormat/odf_rels.cpp"
#include "../source/OdfFormat/odf_style_context.cpp"
#include "../source/OdfFormat/odf_style_state.cpp"
#include "../source/OdfFormat/odf_table_context.cpp"
#include "../source/OdfFormat/odf_table_styles_context.cpp"
#include "../source/OdfFormat/odf_text_context.cpp"
#include "../source/OdfFormat/ods_conversion_context.cpp"
#include "../source/OdfFormat/ods_table_context.cpp"
#include "../source/OdfFormat/ods_table_state.cpp"
#include "../source/OdfFormat/odt_conversion_context.cpp"
#include "../source/OdfFormat/office_annotation.cpp"
#include "../source/OdfFormat/office_body.cpp"
#include "../source/OdfFormat/office_chart.cpp"
#include "../source/OdfFormat/office_elements_create.cpp"
#include "../source/OdfFormat/office_spreadsheet.cpp"
#include "../source/OdfFormat/office_text.cpp"
#include "../source/OdfFormat/oox_shape_defines.cpp"
#include "../source/OdfFormat/paragraph_elements.cpp"
#include "../source/OdfFormat/style_chart_properties.cpp"
#include "../source/OdfFormat/style_graphic_properties.cpp"
#include "../source/OdfFormat/style_map.cpp"
#include "../source/OdfFormat/style_page_layout_properties.cpp"
#include "../source/OdfFormat/style_paragraph_properties.cpp"
#include "../source/OdfFormat/styles.cpp"
#include "../source/OdfFormat/style_section_properties.cpp"
#include "../source/OdfFormat/styles_list.cpp"
#include "../source/OdfFormat/styles_lite_container.cpp"
#include "../source/OdfFormat/style_table_properties.cpp"
#include "../source/OdfFormat/style_text_properties.cpp"
#include "../source/OdfFormat/svg_creator.cpp"
#include "../source/OdfFormat/table.cpp"
#include "../source/OdfFormat/table_database_ranges.cpp"
#include "../source/OdfFormat/table_data_pilot_tables.cpp"
#include "../source/OdfFormat/table_named_expressions.cpp"
#include "../source/OdfFormat/text_elements.cpp"
#include "../source/OdfFormat/odf_settings_context.cpp"
#include "../source/OdfFormat/office_settings.cpp"
#include "../source/OdfFormat/mediaitems_utils.cpp"
#include "../source/OdfFormat/anim_elements.cpp"
#include "../source/OdfFormat/draw_page.cpp"
#include "../source/OdfFormat/odp_conversion_context.cpp"
#include "../source/OdfFormat/odp_page_state.cpp"
#include "../source/OdfFormat/odp_slide_context.cpp"
#include "../source/OdfFormat/office_presentation.cpp"
#include "../source/OdfFormat/style_presentation.cpp"
#include "../source/OdfFormat/office_scripts.cpp"
#include "../source/OdfFormat/office_event_listeners.cpp"
#include "../source/utils.cpp"
