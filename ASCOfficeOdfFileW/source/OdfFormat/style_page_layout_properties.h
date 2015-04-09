#pragma once

#include <string>
#include <vector>
#include <iosfwd>

#include <cpdoccore/CPNoncopyable.h>
#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPWeakPtr.h>
#include <cpdoccore/CPOptional.h>

#include "office_elements_create.h"

#include "common_attlists.h"

#include "tablecentering.h"
#include "layoutgridmode.h"
#include "direction.h"

namespace cpdoccore { 
namespace odf { 

// style:page-layout-properties-attlist
class style_page_layout_properties_attlist
{
public:
	void serialize(CP_ATTR_NODE);
   
	_CP_OPT(length)				fo_page_width_;    
    _CP_OPT(length)				fo_page_height_;    
    
	common_num_format_attlist				common_num_format_attlist_;
    common_num_format_prefix_suffix_attlist common_num_format_prefix_suffix_attlist_;

	common_horizontal_margin_attlist		common_horizontal_margin_attlist_;
    common_vertical_margin_attlist			common_vertical_margin_attlist_;
    common_margin_attlist					common_margin_attlist_;
    
    common_border_attlist				common_border_attlist_;
    common_border_line_width_attlist	common_border_line_width_attlist_;
    common_padding_attlist				common_padding_attlist_;
    common_shadow_attlist				common_shadow_attlist_;
    
    common_background_color_attlist		common_background_color_attlist_;
    common_writing_mode_attlist			common_writing_mode_attlist_;
    
	_CP_OPT(style_ref)					style_register_truth_ref_style_name_;
    _CP_OPT(std::wstring)				style_print_;
	_CP_OPT(std::wstring)				style_paper_tray_name_;
    _CP_OPT(std::wstring)				style_print_orientation_; 
    _CP_OPT(direction)					style_print_page_order_;
    _CP_OPT(std::wstring)				style_first_page_number_;
    _CP_OPT(percent)					style_scale_to_;
    _CP_OPT(unsigned int)				style_scale_to_pages_;
    _CP_OPT(table_centering)			style_table_centering_;
    _CP_OPT(length)						style_footnote_max_height_;
    _CP_OPT(layout_grid_mode)			style_layout_grid_mode_;
	_CP_OPT(length)						style_layout_grid_base_height_;
    _CP_OPT(length)						style_layout_grid_ruby_height_;
    _CP_OPT(unsigned int)				style_layout_grid_lines_;
    _CP_OPT(color)						style_layout_grid_color_;
    _CP_OPT(Bool)						style_layout_grid_ruby_below_;  
    _CP_OPT(Bool)						style_layout_grid_print_;
    _CP_OPT(Bool)						style_layout_grid_display_;

};


// style:page-layout-properties-elements
class style_page_layout_properties_elements
{
public:
    void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context);
    void add_child_element(office_element_ptr & child);
	void serialize(std::wostream & strm);

    office_element_ptr style_background_image_;
    office_element_ptr style_columns_;
    
    // 15.2.20
    office_element_ptr  style_footnote_sep_;

};

//          style:page-layout-properties
class style_page_layout_properties : public office_element_impl<style_page_layout_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePageLayout;
    CPDOCCORE_DEFINE_VISITABLE();

    style_page_layout_properties() { }

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    style_page_layout_properties_attlist	style_page_layout_properties_attlist_;
    style_page_layout_properties_elements	style_page_layout_properties_elements_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_page_layout_properties);

}
}
