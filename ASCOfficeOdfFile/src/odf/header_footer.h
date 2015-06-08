#pragma once 

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "datatypes/common_attlists.h"

namespace cpdoccore { 
namespace odf_reader {

/// \class  style_header_footer_properties_attlist
///         style-header-footer-properties-attlist
class style_header_footer_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    _CP_OPT(odf_types::length)					svg_height_;
    _CP_OPT(odf_types::length)					fo_min_height_;
    odf_types::common_horizontal_margin_attlist common_horizontal_margin_attlist_;
    odf_types::common_vertical_margin_attlist	common_vertical_margin_attlist_;
    odf_types::common_margin_attlist			common_margin_attlist_;
    odf_types::common_border_attlist			common_border_attlist_;
    odf_types::common_border_line_width_attlist common_border_line_width_attlist_;
    odf_types::common_padding_attlist			common_padding_attlist_;
    odf_types::common_background_color_attlist	common_background_color_attlist_;
    odf_types::common_shadow_attlist			common_shadow_attlist_;
    _CP_OPT(bool)								style_dynamic_spacing_;
    
};

/// \class  style_header_footer_properties
///         style-header-footer-properties
class style_header_footer_properties : public office_element_impl<style_header_footer_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleHeaderFooterProperties;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    style_header_footer_properties_attlist style_header_footer_properties_attlist_;
    office_element_ptr style_background_image_;
    
};
CP_REGISTER_OFFICE_ELEMENT2(style_header_footer_properties);


/// \class  common_style_header_footer_attlist
///         common-style-header-footer-attlist
class common_style_header_footer_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
    bool style_display_; // default true
    
};

/// \class  header_footer_content
///         header-footer-content
class header_footer_content
{
public:
    ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);

public:
    office_element_ptr_array content_;
    
};

}
}
