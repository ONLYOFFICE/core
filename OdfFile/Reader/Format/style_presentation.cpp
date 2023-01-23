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

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

const wchar_t * presentation_placeholder::ns	= L"presentation";
const wchar_t * presentation_placeholder::name	= L"placeholder";

void presentation_placeholder::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"presentation:object", presentation_object_);
	
	CP_APPLY_ATTR(L"svg:height", svg_height_);
	CP_APPLY_ATTR(L"svg:width", svg_width_);
	CP_APPLY_ATTR(L"svg:x", svg_x_);
	CP_APPLY_ATTR(L"svg:y", svg_y_);

}

void presentation_placeholder::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void presentation_placeholder::pptx_convert(oox::pptx_conversion_context & Context)
{
	double cx = svg_width_.get_value_or(length(0)).get_value_unit(length::pt);
	double cy = svg_height_.get_value_or(length(0)).get_value_unit(length::pt);

	//пока не понятно что значит отрицательная ширина ...
	cx = fabs(cx);
	cy = fabs(cy);
	
	double x = svg_x_.get_value_or(length(0)).get_value_unit(length::pt);
	double y = svg_y_.get_value_or(length(0)).get_value_unit(length::pt);

	Context.get_slide_context().start_shape(2);//rect
	Context.get_slide_context().set_name(L"place_holder");

	Context.get_slide_context().set_rect(cx,cy,x,y);
	if (presentation_object_)
	{
		Context.get_slide_context().set_placeHolder_type(presentation_object_->get_type_ms());
	}

	Context.get_slide_context().end_shape();
}
//-------------------------------------------------------------------------------------------------
const wchar_t * presentation_sound::ns		= L"presentation";
const wchar_t * presentation_sound::name	= L"sound";

void presentation_sound::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	xlink_attlist_.add_attributes(Attributes);
}

void presentation_sound::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void presentation_sound::pptx_convert(oox::pptx_conversion_context & Context)
{
	Context.get_slide_context().set_link(xlink_attlist_.href_.get_value_or(L""), oox::typeAudio);
}
//-------------------------------------------------------------------------------------------------
const wchar_t * style_drawing_page_properties::ns	= L"style";
const wchar_t * style_drawing_page_properties::name = L"drawing-page-properties";

void style_drawing_page_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	drawing_page_properties_.add_attributes(Attributes);
}
void style_drawing_page_properties::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
	if CP_CHECK_NAME(L"presentation", L"sound")
        CP_CREATE_ELEMENT(drawing_page_properties_.presentation_sound_);
    else
        CP_NOT_APPLICABLE_ELM();
}

void drawing_page_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	common_draw_fill_attlist_.add_attributes(Attributes);
	anim_transition_filter_attlist_.add_attributes(Attributes);

	CP_APPLY_ATTR(L"draw:fill-image-height",	draw_fill_image_height_);
	CP_APPLY_ATTR(L"draw:fill-image-width",		draw_fill_image_width_);
	CP_APPLY_ATTR(L"draw:background-size",		draw_background_size_);

	CP_APPLY_ATTR(L"presentation:transition-type",		presentation_transition_type_);
	CP_APPLY_ATTR(L"presentation:transition-style",		presentation_transition_style_);
	CP_APPLY_ATTR(L"presentation:transition-speed",		presentation_transition_speed_);

	CP_APPLY_ATTR(L"presentation:display-footer",		presentation_display_footer_);
	CP_APPLY_ATTR(L"presentation:display-page-number",	presentation_display_page_number_);
	CP_APPLY_ATTR(L"presentation:display-date-time",	presentation_display_date_time_);
	CP_APPLY_ATTR(L"presentation:display-header",		presentation_display_header_);
	CP_APPLY_ATTR(L"presentation:page-duration",		presentation_page_duration_);
}
void drawing_page_properties::apply_from(const drawing_page_properties & Other)
{
	common_draw_fill_attlist_.apply_from(Other.common_draw_fill_attlist_);

	_CP_APPLY_PROP2(draw_fill_image_height_); 
	_CP_APPLY_PROP2(draw_fill_image_width_); 
	
	_CP_APPLY_PROP2(draw_background_size_); 

	_CP_APPLY_PROP2(presentation_transition_type_);
	_CP_APPLY_PROP2(presentation_transition_style_);
	_CP_APPLY_PROP2(presentation_transition_speed_);

	_CP_APPLY_PROP2(presentation_display_footer_);
	_CP_APPLY_PROP2(presentation_display_page_number_);
	_CP_APPLY_PROP2(presentation_display_date_time_);
	_CP_APPLY_PROP2(presentation_display_header_);
	
	_CP_APPLY_PROP2(presentation_page_duration_);

}
}
}
