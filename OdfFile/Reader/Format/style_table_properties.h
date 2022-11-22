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
#pragma once

#include <iosfwd>

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/length.h"
#include "../../DataTypes/percent.h"
#include "../../DataTypes/lengthorpercent.h"
#include "../../DataTypes/fobreak.h"
#include "../../DataTypes/tablealign.h"
#include "../../DataTypes/common_attlists.h"
#include "../../DataTypes/bordermodel.h"
#include "../../DataTypes/verticalalign.h"
#include "../../DataTypes/borderwidths.h"
#include "../../DataTypes/textalignsource.h"
#include "../../DataTypes/direction.h"
#include "../../DataTypes/wrapoption.h"
#include "../../DataTypes/rotationalign.h"
#include "../../DataTypes/stylecellprotect.h"

namespace cpdoccore { 
namespace odf_reader {

class table_format_properties
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    bool add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name, document_context * Context);
   
	void docx_convert(oox::docx_conversion_context & Context);

	odf_types::common_horizontal_margin_attlist common_horizontal_margin_attlist_;
    odf_types::common_vertical_margin_attlist	common_vertical_margin_attlist_;
    odf_types::common_margin_attlist			common_margin_attlist_;

    odf_types::common_page_number_attlist		common_page_number_attlist_;
    odf_types::common_break_attlist				common_break_attlist_;
    odf_types::common_background_color_attlist	common_background_color_attlist_;
    odf_types::common_shadow_attlist			common_shadow_attlist_;
    odf_types::common_keep_with_next_attlist	common_keep_with_next_attlist_;
    odf_types::common_writing_mode_attlist		common_writing_mode_attlist_;
   
    _CP_OPT(odf_types::length)			style_width_;
    _CP_OPT(odf_types::percent)			style_rel_width_;
    _CP_OPT(bool)						style_may_break_between_rows_;

    _CP_OPT(odf_types::border_model)	table_border_model_;
    _CP_OPT(odf_types::table_align)		table_align_;
    _CP_OPT(bool)						table_display_;	
	_CP_OPT(odf_types::color)			tableooo_tab_color_;
       
    _CP_OPT(odf_types::Bool)			style_use_optimal_column_width_; //not specification
	office_element_ptr					style_background_image_;
};

class style_table_properties : public office_element_impl<style_table_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTableProperties;

    CPDOCCORE_DEFINE_VISITABLE();
    virtual void docx_convert(oox::docx_conversion_context & Context) ;

	table_format_properties & content() {return content_;}

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
 
    table_format_properties content_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_table_properties);

class style_table_column_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    
	void apply_from(const style_table_column_properties_attlist & Other);
	
	_CP_OPT(odf_types::length)		style_column_width_;
    _CP_OPT(odf_types::length)		style_rel_column_width_;
    _CP_OPT(bool)					style_use_optimal_column_width_;
    odf_types::common_break_attlist common_break_attlist_;

};

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
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    style_table_column_properties_attlist attlist_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_table_column_properties);

class style_table_row_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
   
	void apply_from(const style_table_row_properties_attlist & Other);

	void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    _CP_OPT(odf_types::length)					style_row_height_;
    _CP_OPT(odf_types::length)					style_min_row_height_;
    _CP_OPT(bool)								style_use_optimal_row_height_;
    odf_types::common_background_color_attlist	common_background_color_attlist_;
    odf_types::common_break_attlist				common_break_attlist_;
    _CP_OPT(odf_types::keep_together)			fo_keep_together_;        

};

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
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    style_table_row_properties_attlist	attlist_;
    office_element_ptr					style_background_image_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_table_row_properties);

class style_table_cell_properties_attlist
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
   
	void docx_convert(oox::docx_conversion_context & Context);
    void pptx_serialize(oox::pptx_conversion_context & Context, std::wostream & strm);

    void apply_from(const style_table_cell_properties_attlist & Other);

    odf_types::common_shadow_attlist			common_shadow_attlist_;
    odf_types::common_background_color_attlist	common_background_color_attlist_;
    odf_types::common_border_attlist			common_border_attlist_;
    odf_types::common_border_line_width_attlist common_border_line_width_attlist_;
    odf_types::common_padding_attlist			common_padding_attlist_;
    odf_types::common_rotation_angle_attlist	common_rotation_angle_attlist_;

    _CP_OPT(odf_types::vertical_align)		style_vertical_align_;
    _CP_OPT(odf_types::text_align_source)	style_text_align_source_;
    _CP_OPT(odf_types::direction)			style_direction_;
    _CP_OPT(std::wstring)					style_glyph_orientation_vertical_;
    _CP_OPT(std::wstring)					style_diagonal_tl_br_;
    _CP_OPT(odf_types::border_widths)		style_diagonal_tl_br_widths_;
    _CP_OPT(std::wstring)					style_diagonal_bl_tr_;
    _CP_OPT(odf_types::border_widths)		style_diagonal_bl_tr_widths_;
    _CP_OPT(odf_types::rotation_align)		style_rotation_align_;    
    _CP_OPT(odf_types::style_cell_protect)	style_cell_protect_;
    _CP_OPT(bool)							style_print_content_;
    _CP_OPT(unsigned int)					style_decimal_places_;
    _CP_OPT(bool)							style_repeat_content_;
    _CP_OPT(bool)							style_shrink_to_fit_;

    _CP_OPT(odf_types::wrap_option)			fo_wrap_option_;
};

class style_table_cell_properties : public office_element_impl<style_table_cell_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleTableCellProperties;

    CPDOCCORE_DEFINE_VISITABLE();

	style_table_cell_properties() {}

    virtual void docx_convert(oox::docx_conversion_context & Context) ;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    style_table_cell_properties_attlist attlist_;
    office_element_ptr					style_background_image_;
        
};

CP_REGISTER_OFFICE_ELEMENT2(style_table_cell_properties);

}
}
