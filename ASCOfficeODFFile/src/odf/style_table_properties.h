#pragma once

#include <iosfwd>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

#include "length.h"
#include "percent.h"
#include "lengthorpercent.h"
#include "fobreak.h"
#include "tablealign.h"
#include "common_attlists.h"
#include "bordermodel.h"
#include "verticalalign.h"
#include "borderwidths.h"
#include "textalignsource.h"
#include "direction.h"
#include "wrapoption.h"
#include "rotationalign.h"

namespace cpdoccore { 
namespace odf {

class table_format_properties
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    bool add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name, document_context * Context);
    void docx_convert(oox::docx_conversion_context & Context);

private:
    // 15.8.1 style:width
    optional<length>::Type style_width_;
    // style:rel-width
    optional<percent>::Type style_rel_width_;

    // 15.8.2 table:align
    optional<table_align>::Type table_align_;

    // 15.8.3 common-horizontal-margin-attlist
    // fo:margin-left
    // fo:margin-right
    common_horizontal_margin_attlist common_horizontal_margin_attlist_;

    // 15.8.4 common-vertical-margin-attlist
    // fo:margin-top
    // fo:margin-bottom
    common_vertical_margin_attlist common_vertical_margin_attlist_;

    // 15.8.5 common-margin-attlist
    // fo:margin
    common_margin_attlist common_margin_attlist_;

    // 15.8.6 common-page-number-attlist
    // style:page-number
    common_page_number_attlist common_page_number_attlist_;
    
    // 15.8.7 common-break-attlist
    // fo:break-before
    // fo:break-after
    common_break_attlist common_break_attlist_;

    // 15.8.8
    // common-background-color-attlist
    common_background_color_attlist common_background_color_attlist_;
    office_element_ptr style_background_image_;
    
    // 15.8.9
    // common-shadow-attlist
    // style:shadow
    common_shadow_attlist common_shadow_attlist_;

    // 15.8.10
    // common-keep-with-next-attlist
    common_keep_with_next_attlist common_keep_with_next_attlist_;

    // 15.8.11
    // style:may-break-between-rows
    optional<bool>::Type style_may_break_between_rows_;

    // 15.8.12
    // table:border-model
    optional<border_model>::Type table_border_model_;

    // 15.8.13
    // common-writing-mode-attlist
    common_writing_mode_attlist common_writing_mode_attlist_;

    // 15.8.14
    // table:display
    optional<bool>::Type table_display_;
       
};

/// \class  style_table_properties
///         style:table-properties
class style_table_properties : public office_element_impl<style_table_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTableProperties;

    CPDOCCORE_DEFINE_VISITABLE();
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
 
private:
    table_format_properties table_format_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_table_properties);

// style-table-column-properties-attlist
class style_table_column_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    
public:
    _CP_OPT(length) style_column_width_;
    _CP_OPT(length) style_rel_column_width_;
    _CP_OPT(bool) style_use_optimal_column_width_;
    common_break_attlist common_break_attlist_;

};

/// \class  style_table_column_properties
///         style-table-column-properties
///         style:table-column-properties
class style_table_column_properties : public office_element_impl<style_table_column_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTableColumnProperties;

    CPDOCCORE_DEFINE_VISITABLE();
    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    style_table_column_properties_attlist style_table_column_properties_attlist_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_table_column_properties);

// style-table-row-properties-attlist
class style_table_row_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void docx_convert(oox::docx_conversion_context & Context);

public:
    _CP_OPT(length) style_row_height_;
    _CP_OPT(length) style_min_row_height_;
    _CP_OPT(bool) style_use_optimal_row_height_;
    common_background_color_attlist common_background_color_attlist_;
    common_break_attlist common_break_attlist_;
    _CP_OPT(keep_together) fo_keep_together_;        

};

/// \class  style_table_row_properties
///         style-table-row-properties
///         style:table-row-properties
class style_table_row_properties : public office_element_impl<style_table_row_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTableRowProperties;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    style_table_row_properties_attlist style_table_row_properties_attlist_;
    office_element_ptr style_background_image_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_table_row_properties);

// style-table-cell-properties-attlist
class style_table_cell_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    void docx_convert(oox::docx_conversion_context & Context);

    void apply_from(const style_table_cell_properties_attlist & Other);

public:
    // 15.11.1
    _CP_OPT(vertical_align) style_vertical_align_;
    
    // 15.11.2 style:text-align-source
    _CP_OPT(text_align_source) style_text_align_source_;
    
    // 15.11.3 style:direction
    _CP_OPT(direction) style_direction_;

    // 15.11.4 style:glyph-orientation-vertical
    _CP_OPT(std::wstring) style_glyph_orientation_vertical_;
    
    // 15.11.5 
    common_shadow_attlist common_shadow_attlist_;

    // 15.11.6
    common_background_color_attlist common_background_color_attlist_;

    // 15.11.7
    common_border_attlist common_border_attlist_;

    // 15.11.8
    _CP_OPT(std::wstring) style_diagonal_tl_br_;
    _CP_OPT(border_widths) style_diagonal_tl_br_widths_;
    _CP_OPT(std::wstring) style_diagonal_bl_tr_;
    _CP_OPT(border_widths) style_diagonal_bl_tr_widths_;

    // 15.11.9
    // common-border-line-width-attlist
    common_border_line_width_attlist common_border_line_width_attlist_;
    
    // 15.11.10
    common_padding_attlist common_padding_attlist_;
    
    // 15.11.11
    // fo:wrap-option
    _CP_OPT(wrap_option) fo_wrap_option_;

    // 15.11.12
    common_rotation_angle_attlist common_rotation_angle_attlist_;
    
    // 15.11.13
    // style:rotation-align
    _CP_OPT(rotation_align) style_rotation_align_;    

    // 15.11.14
    // style:cell-protect
    _CP_OPT(std::wstring) style_cell_protect_;

    // 15.11.15 
    // style:print-content
    _CP_OPT(bool) style_print_content_;

    // 15.11.16
    // style:decimal-places
    _CP_OPT(unsigned int) style_decimal_places_;

    // 15.11.17
    // style:repeat-content
    _CP_OPT(bool) style_repeat_content_;
    
    // 15.11.18
    _CP_OPT(bool) style_shrink_to_fit_;

};

// style-table-cell-properties-elements
class style_table_cell_properties_elements
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
     // 15.11.6
    office_element_ptr style_background_image_;
    
};

/// \class  style_table_cell_properties
///         style-table-cell-properties
///         style:table-cell-properties
class style_table_cell_properties : public office_element_impl<style_table_cell_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTableCellProperties;

    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

public:
    style_table_cell_properties_attlist style_table_cell_properties_attlist_;
    office_element_ptr style_background_image_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_table_cell_properties);

}
}
