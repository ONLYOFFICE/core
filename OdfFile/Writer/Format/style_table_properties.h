/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#pragma once

#include <iosfwd>
#include <xml/attributes.h>
#include <CPOptional.h>
#include <xml/xmlelement.h>
#include <xml/nodetype.h>

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
#include "../../DataTypes/bool.h"
#include "../../DataTypes/stylecellprotect.h"

namespace cpdoccore { 
namespace odf_writer {

class table_format_properties
{
public:
    bool create_child_element( const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
	void apply_from(const table_format_properties & Other);
	void serialize(std::wostream & strm ,const wchar_t * ns, const wchar_t * name );

    _CP_OPT(odf_types::length)			style_width_;
    _CP_OPT(odf_types::percent)			style_rel_width_;
    _CP_OPT(odf_types::Bool)			style_may_break_between_rows_;
    _CP_OPT(odf_types::border_model)	table_border_model_;
    _CP_OPT(odf_types::Bool)			table_display_;
    _CP_OPT(odf_types::table_align)		table_align_;
	_CP_OPT(odf_types::color)			table_tab_color_;
	
	odf_types::common_horizontal_margin_attlist		common_horizontal_margin_attlist_;
    odf_types::common_vertical_margin_attlist		common_vertical_margin_attlist_;
    odf_types::common_margin_attlist				common_margin_attlist_;
    odf_types::common_page_number_attlist			common_page_number_attlist_;
    odf_types::common_break_attlist					common_break_attlist_;
    odf_types::common_background_color_attlist		common_background_color_attlist_;
    odf_types::common_shadow_attlist				common_shadow_attlist_;
    odf_types::common_keep_with_next_attlist		common_keep_with_next_attlist_;
    odf_types::common_writing_mode_attlist			common_writing_mode_attlist_;
	odf_types::common_border_attlist				common_border_attlist_;
 
	office_element_ptr			style_background_image_;
    _CP_OPT(odf_types::Bool)	style_use_optimal_column_width_;
       
};

// style:table-properties
class style_table_properties : public office_element_impl<style_table_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleTableProperties;    

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( const office_element_ptr & child){}

    void apply_from(const style_table_properties * Other);

	virtual void serialize(std::wostream & strm);

    table_format_properties content_;
};
CP_REGISTER_OFFICE_ELEMENT2(style_table_properties)

// style-table-column-properties-attlist
class style_table_column_properties_attlist
{
public:
 	void serialize(std::wostream & strm ,const wchar_t * ns, const wchar_t * name );
   
    _CP_OPT(double) loext_column_width_sym_;
    _CP_OPT(odf_types::length) style_column_width_;
    _CP_OPT(odf_types::length) style_rel_column_width_;
    _CP_OPT(odf_types::Bool) style_use_optimal_column_width_;
  
	odf_types::common_break_attlist common_break_attlist_;

};

// style:table-column-properties
class style_table_column_properties : public office_element_impl<style_table_column_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleTableColumnProperties;    
  
	virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void serialize(std::wostream & strm);

    style_table_column_properties_attlist attlist_;
};
CP_REGISTER_OFFICE_ELEMENT2(style_table_column_properties)

// style-table-row-properties-attlist
class style_table_row_properties_attlist
{
public:
 	void serialize(std::wostream & strm ,const wchar_t * ns, const wchar_t * name );
  
    _CP_OPT(odf_types::length)			style_row_height_;
    _CP_OPT(odf_types::length)			style_min_row_height_;
    _CP_OPT(odf_types::Bool)			style_use_optimal_row_height_;
    _CP_OPT(odf_types::keep_together)	fo_keep_together_;        

    odf_types::common_background_color_attlist common_background_color_attlist_;
    odf_types::common_break_attlist common_break_attlist_;
};

// style:table-row-properties
class style_table_row_properties : public office_element_impl<style_table_row_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleTableRowProperties;

    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);	
	virtual void serialize(std::wostream & strm);

public:
    style_table_row_properties_attlist	style_table_row_properties_attlist_;
    office_element_ptr style_background_image_;
        
};
CP_REGISTER_OFFICE_ELEMENT2(style_table_row_properties)

// style-table-cell-properties-attlist
class style_table_cell_properties_attlist
{
public:

    void apply_from(const style_table_cell_properties_attlist & Other);
	void serialize(std::wostream & strm ,const wchar_t * ns, const wchar_t * name );

public:
    _CP_OPT(odf_types::vertical_align)		style_vertical_align_;
    _CP_OPT(odf_types::text_align_source)	style_text_align_source_;
    _CP_OPT(odf_types::direction)			style_direction_;
    _CP_OPT(std::wstring)					style_glyph_orientation_vertical_;
 
	odf_types::common_shadow_attlist			common_shadow_attlist_;
    odf_types::common_background_color_attlist	common_background_color_attlist_;
    odf_types::common_border_attlist			common_border_attlist_;
	odf_types::common_writing_mode_attlist		common_writing_mode_attlist_;

	_CP_OPT(std::wstring)				style_diagonal_tl_br_;
    _CP_OPT(odf_types::border_widths)	style_diagonal_tl_br_widths_;
    _CP_OPT(std::wstring)				style_diagonal_bl_tr_;
    _CP_OPT(odf_types::border_widths)	style_diagonal_bl_tr_widths_;

    odf_types::common_border_line_width_attlist		common_border_line_width_attlist_;
    odf_types::common_padding_attlist				common_padding_attlist_;
	odf_types::common_rotation_angle_attlist		common_rotation_angle_attlist_;

	_CP_OPT(odf_types::wrap_option)		fo_wrap_option_;

	_CP_OPT(odf_types::rotation_align)	style_rotation_align_;    
	_CP_OPT(odf_types::style_cell_protect)	style_cell_protect_;
	_CP_OPT(odf_types::Bool)			style_print_content_;
    _CP_OPT(unsigned int)				style_decimal_places_;
    _CP_OPT(odf_types::Bool)			style_repeat_content_;
    _CP_OPT(odf_types::Bool)			style_shrink_to_fit_;

};

// style-table-cell-properties-elements
class style_table_cell_properties_elements
{
public:
    office_element_ptr style_background_image_;    
};

// style:table-cell-properties
class style_table_cell_properties : public office_element_impl<style_table_cell_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleTableCellProperties;

    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);

    void apply_from(const style_table_cell_properties * Other);
	virtual void serialize(std::wostream & strm);

    style_table_cell_properties_attlist content_;
    office_element_ptr style_background_image_;
        
};
CP_REGISTER_OFFICE_ELEMENT2(style_table_cell_properties)

}
}
