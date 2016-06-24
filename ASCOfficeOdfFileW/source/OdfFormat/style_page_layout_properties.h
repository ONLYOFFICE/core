/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
namespace odf_writer { 

// style:page-layout-properties-attlist
class style_page_layout_properties_attlist
{
public:
	void serialize(CP_ATTR_NODE);
   
	_CP_OPT(odf_types::length)				fo_page_width_;    
    _CP_OPT(odf_types::length)				fo_page_height_;    
    
	odf_types::common_num_format_attlist				common_num_format_attlist_;
    odf_types::common_num_format_prefix_suffix_attlist common_num_format_prefix_suffix_attlist_;

	odf_types::common_horizontal_margin_attlist		common_horizontal_margin_attlist_;
    odf_types::common_vertical_margin_attlist			common_vertical_margin_attlist_;
    odf_types::common_margin_attlist					common_margin_attlist_;
    
    odf_types::common_border_attlist				common_border_attlist_;
    odf_types::common_border_line_width_attlist	common_border_line_width_attlist_;
    odf_types::common_padding_attlist				common_padding_attlist_;
    odf_types::common_shadow_attlist				common_shadow_attlist_;
    
    odf_types::common_background_color_attlist		common_background_color_attlist_;
    odf_types::common_writing_mode_attlist			common_writing_mode_attlist_;
    
	_CP_OPT(odf_types::style_ref)					style_register_truth_ref_style_name_;
    _CP_OPT(std::wstring)				style_print_;
	_CP_OPT(std::wstring)				style_paper_tray_name_;
    _CP_OPT(std::wstring)				style_print_orientation_; 
    _CP_OPT(odf_types::direction)					style_print_page_order_;
    _CP_OPT(std::wstring)				style_first_page_number_;
    _CP_OPT(odf_types::percent)					style_scale_to_;
    _CP_OPT(unsigned int)				style_scale_to_pages_;
    _CP_OPT(odf_types::table_centering)			style_table_centering_;
    _CP_OPT(odf_types::length)						style_footnote_max_height_;
    _CP_OPT(odf_types::layout_grid_mode)			style_layout_grid_mode_;
	_CP_OPT(odf_types::length)						style_layout_grid_base_height_;
    _CP_OPT(odf_types::length)						style_layout_grid_ruby_height_;
    _CP_OPT(unsigned int)				style_layout_grid_lines_;
    _CP_OPT(odf_types::color)						style_layout_grid_color_;
    _CP_OPT(odf_types::Bool)						style_layout_grid_ruby_below_;  
    _CP_OPT(odf_types::Bool)						style_layout_grid_print_;
    _CP_OPT(odf_types::Bool)						style_layout_grid_display_;

};


// style:page-layout-properties-elements
class style_page_layout_properties_elements
{
public:
    void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context);
    void add_child_element( const office_element_ptr & child);
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
    CPDOCCORE_DEFINE_VISITABLE()

    style_page_layout_properties() { }

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    style_page_layout_properties_attlist	style_page_layout_properties_attlist_;
    style_page_layout_properties_elements	style_page_layout_properties_elements_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(style_page_layout_properties);

}
}
