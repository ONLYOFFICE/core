#include "precompiled_cpodf.h"
#include "header_footer.h"
#include <cpdoccore/xml/attributes.h>



namespace cpdoccore { 
namespace odf {

void style_header_footer_properties_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"svg:height", svg_height_);
    CP_XML_ATTR_OPT(L"fo:min-height", fo_min_height_);
   
	common_horizontal_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_vertical_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_border_attlist_.serialize(CP_GET_XML_NODE());
    common_border_line_width_attlist_.serialize(CP_GET_XML_NODE());
    common_padding_attlist_.serialize(CP_GET_XML_NODE());
    common_background_color_attlist_.serialize(CP_GET_XML_NODE());
    common_shadow_attlist_.serialize(CP_GET_XML_NODE());
    CP_XML_ATTR_OPT(L"style:dynamic-spacing", style_dynamic_spacing_);
}

/// style-header-footer-properties
const wchar_t * style_header_footer_properties::ns = L"style";
const wchar_t * style_header_footer_properties::name = L"header-footer-properties";

void style_header_footer_properties::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);
    }
    else
    {
        not_applicable_element(L"style-header-footer-properties",  Ns, Name);
    }    
}
void style_header_footer_properties::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {	
			style_header_footer_properties_attlist_.serialize(CP_GET_XML_NODE());
			if (style_background_image_) style_background_image_->serialize(CP_XML_STREAM());
		}
	}
}
//////////////////////////////////////////
void header_footer_content::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    CP_CREATE_ELEMENT_SIMPLE(content_);
}
void header_footer_content::add_child_element(office_element_ptr & child)
{
    content_.push_back(child);
}
void header_footer_content::serialize(std::wostream & strm)
{
	BOOST_FOREACH(const office_element_ptr & elm, content_)
	{
		elm->serialize(strm);
	}
}
}
}
