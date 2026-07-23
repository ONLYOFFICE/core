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

#include <string>
#include <vector>
#include <iosfwd>

#include <CPNoncopyable.h>
#include <xml/attributes.h>

#include <CPSharedPtr.h>
#include <CPWeakPtr.h>
#include <CPOptional.h>

#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"

#include "../../DataTypes/tablecentering.h"
#include "../../DataTypes/layoutgridmode.h"
#include "../../DataTypes/direction.h"
#include "../../DataTypes/styleprint.h"

namespace cpdoccore { 
namespace odf_writer { 

class style_page_layout_properties_attlist
{
public:
	style_page_layout_properties_attlist() : offset_page_border_(1) {}
	void serialize(CP_ATTR_NODE);
   
	_CP_OPT(odf_types::length)							fo_page_width_;    
    _CP_OPT(odf_types::length)							fo_page_height_;    
    
	odf_types::common_draw_fill_attlist					common_draw_fill_attlist_;
	odf_types::common_num_format_attlist				common_num_format_attlist_;
    odf_types::common_num_format_prefix_suffix_attlist	common_num_format_prefix_suffix_attlist_;

	odf_types::common_horizontal_margin_attlist			common_horizontal_margin_attlist_;
    odf_types::common_vertical_margin_attlist			common_vertical_margin_attlist_;
    odf_types::common_margin_attlist					common_margin_attlist_;
    _CP_OPT(odf_types::length)                          loext_margin_gutter_;

    
    odf_types::common_border_attlist					common_border_attlist_;
    odf_types::common_border_line_width_attlist			common_border_line_width_attlist_;
    odf_types::common_padding_attlist					common_padding_attlist_;
    odf_types::common_shadow_attlist					common_shadow_attlist_;    
    odf_types::common_background_color_attlist			common_background_color_attlist_;
    odf_types::common_writing_mode_attlist				common_writing_mode_attlist_;
	odf_types::common_page_number_attlist				common_page_number_attlist_;
    
	_CP_OPT(std::wstring)								style_register_truth_ref_style_name_;
    _CP_OPT(odf_types::style_print)						style_print_;
	_CP_OPT(std::wstring)								style_paper_tray_name_;
    _CP_OPT(std::wstring)								style_print_orientation_; 
    _CP_OPT(odf_types::direction)						style_print_page_order_;
    _CP_OPT(odf_types::percent)							style_scale_to_;
    _CP_OPT(unsigned int)								style_scale_to_pages_;
    _CP_OPT(odf_types::table_centering)					style_table_centering_;
    _CP_OPT(odf_types::length)							style_footnote_max_height_;
    _CP_OPT(odf_types::layout_grid_mode)				style_layout_grid_mode_;
	_CP_OPT(odf_types::length)							style_layout_grid_base_height_;
    _CP_OPT(odf_types::length)							style_layout_grid_ruby_height_;
    _CP_OPT(unsigned int)								style_layout_grid_lines_;
    _CP_OPT(odf_types::color)							style_layout_grid_color_;
    _CP_OPT(odf_types::Bool)							style_layout_grid_ruby_below_;  
    _CP_OPT(odf_types::Bool)							style_layout_grid_print_;
    _CP_OPT(odf_types::Bool)							style_layout_grid_display_;

	int													offset_page_border_;
	_CP_OPT(unsigned int)								loext_scale_to_X_;
	_CP_OPT(unsigned int)								loext_scale_to_Y_;
};

class style_page_layout_properties_elements
{
public:
    void create_child_element( const std::wstring & Ns, const std::wstring & Name, odf_conversion_context * Context);
    void add_child_element( const office_element_ptr & child);
	void serialize(std::wostream & strm);

    office_element_ptr	style_background_image_;
    office_element_ptr	style_columns_;
    
    office_element_ptr  style_footnote_sep_;

};

class style_page_layout_properties : public office_element_impl<style_page_layout_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStylePageLayout;
    

    style_page_layout_properties() { }

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    style_page_layout_properties_attlist	attlist_;
    style_page_layout_properties_elements	elements_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_page_layout_properties);

}
}
