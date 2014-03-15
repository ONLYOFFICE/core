#include "precompiled_cpodf.h"
#include <iostream>
#include "style_table_properties.h"
#include "borderstyle.h"

#include <cpdoccore/xml/simple_xml_writer.h>


namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

bool table_format_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT_SIMPLE(style_background_image_);
        return true;
    }
    else
    {
        not_applicable_element(L"style-table-properties", Ns, Name);
        return false;
    }
}

void table_format_properties::serialize(std::wostream & _Wostream,const wchar_t * ns, const wchar_t * name)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {

			CP_XML_ATTR_OPT(L"style:width", style_width_);
			CP_XML_ATTR_OPT(L"style:rel-width", style_rel_width_);
			CP_XML_ATTR_OPT(L"table:align", table_align_);
			CP_XML_ATTR_OPT(L"style:may-break-between-rows", style_may_break_between_rows_);
			CP_XML_ATTR_OPT(L"table:border-model", table_border_model_);
			CP_XML_ATTR_OPT(L"table:display", table_display_);

			common_writing_mode_attlist_.serialize(CP_GET_XML_NODE());
			common_horizontal_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_vertical_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_margin_attlist_.serialize(CP_GET_XML_NODE());
			common_page_number_attlist_.serialize(CP_GET_XML_NODE());
			common_break_attlist_.serialize(CP_GET_XML_NODE());
			common_background_color_attlist_.serialize(CP_GET_XML_NODE());
			common_shadow_attlist_.serialize(CP_GET_XML_NODE());
			common_keep_with_next_attlist_.serialize(CP_GET_XML_NODE());
			
			if (style_background_image_) style_background_image_->serialize(_Wostream);
		}
	}
}

/*

[ ] w:tblStyle              Referenced Table Style
[ ] w:tblpPr                Floating Table Positioning
[ ] w:tblOverlap            Floating Table Allows Other Tables to Overlap
[ ] w:bidiVisual            Visually Right to Left Table
[ ] w:tblStyleRowBandSize   Number of Rows in Row Band
[ ] w:tblStyleColBandSize   Number of Columns in Column Band
[x] w:tblW                  Preferred Table Width
[x] w:jc                    Table Alignment
[ ] w:tblCellSpacing        Table Cell Spacing Default
[ ] w:tblInd                Table Indent from Leading Margin
[ ] w:tblBorders            Table Borders
[ ] w:shd                   Table Shading
[ ] w:tblLayout             Table Layout
[ ] w:tblCellMar            Table Cell Margin Defaults
[ ] w:tblLook               Table Style Conditional Formatting Settings
[ ] w:tblPrChange           Revision Information for Table Properties

*/
// style:table-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_properties::ns = L"style";
const wchar_t * style_table_properties::name = L"table-properties";


void style_table_properties::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    table_format_properties_.create_child_element(Ns, Name, getContext());
}

void style_table_properties::serialize(std::wostream & _Wostream)
{
	table_format_properties_.serialize(_Wostream,ns,name);

}

// style-table-column-properties-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

// style:table-column-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_column_properties::ns = L"style";
const wchar_t * style_table_column_properties::name = L"table-column-properties";

void style_table_column_properties::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}


//////////////////////////////////////////////////////////////////////////////////////////////////

/// style:table-cell-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_cell_properties::ns = L"style";
const wchar_t * style_table_cell_properties::name = L"table-cell-properties";



void style_table_cell_properties::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();        
    }
}

// style-table-row-properties-attlist
//////////////////////////////////////////////////////////////////////////////////////////////////

//  style:table-row-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_table_row_properties::ns = L"style";
const wchar_t * style_table_row_properties::name = L"table-row-properties";


void style_table_row_properties::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);        
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();        
    }
}

/////

void style_table_cell_properties_attlist::apply_from(const style_table_cell_properties_attlist & Other)
{
     _CP_APPLY_PROP(style_vertical_align_, Other.style_vertical_align_);
     _CP_APPLY_PROP(style_text_align_source_, Other.style_text_align_source_);
     _CP_APPLY_PROP(style_direction_, Other.style_direction_);
     _CP_APPLY_PROP(style_glyph_orientation_vertical_, Other.style_glyph_orientation_vertical_);
    
    common_shadow_attlist_.apply_from(Other.common_shadow_attlist_);
    common_background_color_attlist_.apply_from(Other.common_background_color_attlist_);
    common_border_attlist_.apply_from(Other.common_border_attlist_);

    _CP_APPLY_PROP(style_diagonal_tl_br_, Other.style_diagonal_tl_br_);
    _CP_APPLY_PROP(style_diagonal_tl_br_widths_, Other.style_diagonal_tl_br_widths_);
    _CP_APPLY_PROP(style_diagonal_bl_tr_, Other.style_diagonal_bl_tr_);
    _CP_APPLY_PROP(style_diagonal_bl_tr_widths_, Other.style_diagonal_bl_tr_widths_);

    common_border_line_width_attlist_.apply_from(Other.common_border_line_width_attlist_);
    common_padding_attlist_.apply_from(Other.common_padding_attlist_);
    _CP_APPLY_PROP(fo_wrap_option_, Other.fo_wrap_option_);
    common_rotation_angle_attlist_.apply_from(Other.common_rotation_angle_attlist_);
    
    _CP_APPLY_PROP(style_rotation_align_, Other.style_rotation_align_);    
    _CP_APPLY_PROP(style_cell_protect_, Other.style_cell_protect_);
    _CP_APPLY_PROP(style_print_content_, Other.style_print_content_);
    _CP_APPLY_PROP(style_decimal_places_, Other.style_decimal_places_);
    _CP_APPLY_PROP(style_repeat_content_, Other.style_repeat_content_);
    _CP_APPLY_PROP(style_shrink_to_fit_, Other.style_shrink_to_fit_);    
}

}
}
