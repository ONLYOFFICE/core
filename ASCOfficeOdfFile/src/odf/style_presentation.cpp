
#include "style_presentation.h"

#include <iostream>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// 

// style:chart-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * presentation_placeholder::ns = L"presentation";
const wchar_t * presentation_placeholder::name = L"placeholder";

void presentation_placeholder::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	CP_APPLY_ATTR(L"presentation:object", presentation_object_);
	
	CP_APPLY_ATTR(L"svg:height", svg_height_);
	CP_APPLY_ATTR(L"svg:width", svg_width_);
	CP_APPLY_ATTR(L"svg:x", svg_x_);
	CP_APPLY_ATTR(L"svg:y", svg_y_);

}

void presentation_placeholder::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

void presentation_placeholder::pptx_convert(oox::pptx_conversion_context & Context)
{
	double cx = svg_width_.get_value_or(length(0)).get_value_unit(length::pt);
	double cy = svg_height_.get_value_or(length(0)).get_value_unit(length::pt);

	//пока не понятно что значит отрицательная ширина ...
	cx = abs(cx);
	cy = abs(cy);
	
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

//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_drawing_page_properties::ns = L"style";
const wchar_t * style_drawing_page_properties::name = L"drawing-page-properties";

void style_drawing_page_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	drawing_page_properties_.add_attributes(Attributes);
}
void style_drawing_page_properties::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
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

}
void drawing_page_properties::apply_from(const drawing_page_properties & Other)
{
	common_draw_fill_attlist_.apply_from(Other.common_draw_fill_attlist_);

	_CP_APPLY_PROP2(draw_fill_image_height_); 
	_CP_APPLY_PROP2(draw_fill_image_width_); 
	
	_CP_APPLY_PROP2(draw_background_size_); 
}
}
}
