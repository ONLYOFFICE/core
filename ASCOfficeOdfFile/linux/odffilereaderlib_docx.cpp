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
#include "../src/docx/docx_content_type.cpp"
#include "../src/docx/docx_conversion_context.cpp"
#include "../src/docx/docx_drawing.cpp"
#include "../src/docx/docx_package.cpp"
#include "../src/docx/docx_rels.cpp"
#include "../src/docx/docx_table_context.cpp"
#include "../src/docx/headers_footers.cpp"
#include "../src/docx/hyperlinks.cpp"
#include "../src/docx/measuredigits.cpp"
#include "../src/docx/mediaitems.cpp"
#include "../src/docx/mediaitems_utils.cpp"
#include "../src/docx/namespaces.cpp"
#include "../src/docx/oox_chart_axis.cpp"
#include "../src/docx/oox_chart_context.cpp"
#include "../src/docx/oox_chart_legend.cpp"
#include "../src/docx/oox_chart_series.cpp"
#include "../src/docx/oox_chart_shape.cpp"
#include "../src/docx/oox_conversion_context.cpp"
#include "../src/docx/oox_data_labels.cpp"
#include "../src/docx/oox_drawing.cpp"
#include "../src/docx/oox_drawing_fills.cpp"
#include "../src/docx/oox_layout.cpp"
#include "../src/docx/oox_package.cpp"
#include "../src/docx/oox_plot_area.cpp"
#include "../src/docx/oox_title.cpp"
#include "../src/docx/oox_types_chart.cpp"
#include "../src/odf/draw_frame_docx.cpp"
#include "../src/odf/draw_shapes_docx.cpp"
#include "../src/odf/style_paragraph_properties_docx.cpp"
#include "../src/odf/table_docx.cpp"