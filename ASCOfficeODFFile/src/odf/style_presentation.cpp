#include "precompiled_cpodf.h"
#include "style_presentation.h"

#include <iostream>
#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {

//using xml::xml_char_wc; 

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
}
}
