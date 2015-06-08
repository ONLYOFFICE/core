
#include "header_footer.h"
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// style-header-footer-properties-attlist

void style_header_footer_properties_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"svg:height", svg_height_);
    CP_APPLY_ATTR(L"fo:min-height", fo_min_height_);
    common_horizontal_margin_attlist_.add_attributes(Attributes);
    common_vertical_margin_attlist_.add_attributes(Attributes);
    common_margin_attlist_.add_attributes(Attributes);
    common_border_attlist_.add_attributes(Attributes);
    common_border_line_width_attlist_.add_attributes(Attributes);
    common_padding_attlist_.add_attributes(Attributes);
    common_background_color_attlist_.add_attributes(Attributes);
    common_shadow_attlist_.add_attributes(Attributes);
    CP_APPLY_ATTR(L"style:dynamic-spacing", style_dynamic_spacing_);
}

/// style-header-footer-properties
const wchar_t * style_header_footer_properties::ns = L"style";
const wchar_t * style_header_footer_properties::name = L"header-footer-properties";

void style_header_footer_properties::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    style_header_footer_properties_attlist_.add_attributes(Attributes);
}

void style_header_footer_properties::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);
    }
    else
    {
        not_applicable_element(L"style-header-footer-properties", Reader, Ns, Name);
    }    
}

// common-style-header-footer-attlist

void common_style_header_footer_attlist::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:display", style_display_, true);
}

// header-footer-content
::std::wostream & header_footer_content::text_to_stream(::std::wostream & _Wostream) const
{
    serialize_elements_text(_Wostream, content_);
    return _Wostream;
}

void header_footer_content::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context)
{
    CP_CREATE_ELEMENT_SIMPLE(content_);
}

}
}
