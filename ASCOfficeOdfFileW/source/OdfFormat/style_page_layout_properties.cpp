#include "precompiled_cpodf.h"

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/simple_xml_writer.h>
#include <iostream>
#include <boost/foreach.hpp>

#include "office_elements_create.h"

#include "style_page_layout_properties.h"

namespace cpdoccore { 
namespace odf {
    
/// style:page-layout-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_page_layout_properties_attlist::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"fo:page-width", fo_page_width_);
    CP_XML_ATTR_OPT(L"fo:page-height", fo_page_height_);
    common_num_format_attlist_.serialize(CP_GET_XML_NODE());
    common_num_format_prefix_suffix_attlist_.serialize(CP_GET_XML_NODE());
    CP_XML_ATTR_OPT(L"style:paper-tray-name", style_paper_tray_name_);
    CP_XML_ATTR_OPT(L"style:print-orientation", style_print_orientation_);
    common_horizontal_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_vertical_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_margin_attlist_.serialize(CP_GET_XML_NODE());
    common_border_attlist_.serialize(CP_GET_XML_NODE());
    common_border_line_width_attlist_.serialize(CP_GET_XML_NODE());
    common_padding_attlist_.serialize(CP_GET_XML_NODE());
    common_shadow_attlist_.serialize(CP_GET_XML_NODE());
    common_background_color_attlist_.serialize(CP_GET_XML_NODE());
    CP_XML_ATTR_OPT(L"style:register-truth-ref-style-name", style_register_truth_ref_style_name_);
    CP_XML_ATTR_OPT(L"style:print", style_print_);
    CP_XML_ATTR_OPT(L"style:print-page-order", style_print_page_order_);
    CP_XML_ATTR_OPT(L"style:first-page-number", style_first_page_number_);
    CP_XML_ATTR_OPT(L"style:scale-to", style_scale_to_);
    CP_XML_ATTR_OPT(L"style:scale-to_pages", style_scale_to_pages_);
    CP_XML_ATTR_OPT(L"style:table-centering", style_table_centering_);
    CP_XML_ATTR_OPT(L"style:footnote-max-height", style_footnote_max_height_);
    common_writing_mode_attlist_.serialize(CP_GET_XML_NODE());
    CP_XML_ATTR_OPT(L"style:layout-grid-mode", style_layout_grid_mode_);
    CP_XML_ATTR_OPT(L"style:layout-grid-base-height", style_layout_grid_base_height_);
    CP_XML_ATTR_OPT(L"style:layout-grid-ruby-height", style_layout_grid_ruby_height_);
    CP_XML_ATTR_OPT(L"style:layout-grid-lines", style_layout_grid_lines_);
    CP_XML_ATTR_OPT(L"style:layout-grid-color", style_layout_grid_color_);
    CP_XML_ATTR_OPT(L"style:layout-grid-ruby-below", style_layout_grid_ruby_below_);  
    CP_XML_ATTR_OPT(L"style:layout-grid-print", style_layout_grid_print_);
    CP_XML_ATTR_OPT(L"style:layout-grid-display", style_layout_grid_display_);
}
const wchar_t * style_page_layout_properties::ns = L"style";
const wchar_t * style_page_layout_properties::name = L"page-layout-properties";

void style_page_layout_properties::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    style_page_layout_properties_elements_.create_child_element( Ns, Name, getContext());
}

void style_page_layout_properties::add_child_element( office_element_ptr & child)
{
	style_page_layout_properties_elements_.add_child_element(child);
}

void style_page_layout_properties::serialize(std::wostream & strm)
{
    CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			style_page_layout_properties_attlist_.serialize( CP_GET_XML_NODE());
			style_page_layout_properties_elements_.serialize(CP_XML_STREAM());
		}
	}
}
// style-page-layout-properties-elements
//////////////////////////////////////////////////////////////////////////////////////////////////
void style_page_layout_properties_elements::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context )
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_background_image_);            
    }
    else if (L"style" == Ns && L"columns" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_columns_);            
    }
    else if (L"style" == Ns && L"footnote-sep" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_footnote_sep_);            
    }
    else
    {
        not_applicable_element(L"style-page-layout-properties-elements", Ns, Name);
    }
}
void style_page_layout_properties_elements::add_child_element( office_element_ptr & child )
{
	if (!child)return;

	ElementType type = child->get_type();
	switch(type)
	{
	case typeStyleBackgroundImage:
        style_background_image_ = child; 
		break;
	case typeStyleColumns:
		style_columns_ = child;      
		break;
	case typeStyleFootnoteSep:
        style_footnote_sep_ = child;       
    }
}
void style_page_layout_properties_elements::serialize(std::wostream & strm)
{
    if (style_background_image_) style_background_image_->serialize(strm);
    if (style_columns_)			style_columns_->serialize(strm);
    if (style_footnote_sep_)	style_footnote_sep_->serialize(strm);
}

}
}