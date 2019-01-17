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

#include "style_presentation.h"

#include <iostream>
#include <xml/simple_xml_writer.h>
#include <xml/attributes.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

//----------------------------------------------------------------------------------
const wchar_t * presentation_placeholder::ns = L"presentation";
const wchar_t * presentation_placeholder::name = L"placeholder";

void presentation_placeholder::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"presentation:object", presentation_object_);
	
			CP_XML_ATTR_OPT(L"svg:height", svg_height_);
			CP_XML_ATTR_OPT(L"svg:width", svg_width_);
			CP_XML_ATTR_OPT(L"svg:x", svg_x_);
			CP_XML_ATTR_OPT(L"svg:y", svg_y_);

		}
	}
}
//----------------------------------------------------------------------------------
const wchar_t * presentation_sound::ns = L"presentation";
const wchar_t * presentation_sound::name = L"sound";

void presentation_sound::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_xlink_attlist_.serialize(CP_GET_XML_NODE());
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------------

void drawing_page_properties::serialize(std::wostream & strm, const wchar_t * ns, const wchar_t * name )
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			common_draw_fill_attlist_.serialize(CP_GET_XML_NODE());
			anim_transition_filter_attlist_.serialize(CP_GET_XML_NODE());

			CP_XML_ATTR_OPT(L"draw:fill-image-height",	draw_fill_image_height_);
			CP_XML_ATTR_OPT(L"draw:fill-image-width",	draw_fill_image_width_);
			CP_XML_ATTR_OPT(L"draw:background-size",	draw_background_size_);

			CP_XML_ATTR_OPT(L"presentation:transition-type",	presentation_transition_type_);
			CP_XML_ATTR_OPT(L"presentation:transition-style",	presentation_transition_style_);
			CP_XML_ATTR_OPT(L"presentation:transition-speed",	presentation_transition_speed_);

			CP_XML_ATTR_OPT(L"presentation:display-footer",		presentation_display_footer_);
			CP_XML_ATTR_OPT(L"presentation:display-page-number",presentation_display_page_number_);
			CP_XML_ATTR_OPT(L"presentation:display-date-time",	presentation_display_date_time_);
			CP_XML_ATTR_OPT(L"presentation:display-header",		presentation_display_header_);

			if (presentation_sound_)
				presentation_sound_->serialize(CP_XML_STREAM());
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------
const wchar_t * style_drawing_page_properties::ns	= L"style";
const wchar_t * style_drawing_page_properties::name = L"drawing-page-properties";

void style_drawing_page_properties::serialize(std::wostream & strm)
{
	content_.serialize(strm, ns, name);
}

}
}
