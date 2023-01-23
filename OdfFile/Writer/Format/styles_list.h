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
#include <string>

#include <CPSharedPtr.h>
#include <CPOptional.h>

#include <xml/nodetype.h>

#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"


namespace cpdoccore { 
namespace odf_writer { 

class text_list_style_attr
{
public:

    _CP_OPT(std::wstring)	style_name_;
    _CP_OPT(std::wstring)	style_display_name_;
    _CP_OPT(odf_types::Bool)			text_consecutive_numbering_;

	void serialize(CP_ATTR_NODE);
};

/// \class  text_list_style
///         text:list-style (text-list-style)
class text_list_style : public office_element_impl<text_list_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextListStyle;
    

	virtual void create_child_element(  const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);
	virtual void serialize(std::wostream & strm);

    text_list_style_attr		text_list_style_attr_;
    office_element_ptr_array	text_list_style_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_style);

/// \class  text_list_level_style_number_attr
///         text-list-level-style-number-attr
class text_list_level_style_number_attr
{
public:
    text_list_level_style_number_attr()  {}

	_CP_OPT(std::wstring) text_style_name_;
    
	odf_types::common_num_format_attlist				common_num_format_attlist_;
    odf_types::common_num_format_prefix_suffix_attlist	common_num_format_prefix_suffix_attlist_;
    
	_CP_OPT(unsigned int)	text_display_levels_;
    _CP_OPT(unsigned int)	text_start_value_;

	void serialize(CP_ATTR_NODE);
};


/// text-list-level-style-attr
class text_list_level_style_attr
{
public:
    text_list_level_style_attr() : text_level_(0) {}

    unsigned int text_level_;
	void serialize(CP_ATTR_NODE);
};



/// \class  style_list_level_properties
///         style:list-level-properties (style-list-level-properties)
class style_list_level_properties : public office_element_impl<style_list_level_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleListLevelProperties;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);
	virtual void serialize(std::wostream & strm);

    _CP_OPT(odf_types::length)			text_space_before_;
    _CP_OPT(odf_types::length)			text_min_label_width_;
    _CP_OPT(odf_types::length)			text_min_label_distance_;

    _CP_OPT(std::wstring)	style_font_name_;
    _CP_OPT(odf_types::length)			fo_width_;
    _CP_OPT(odf_types::length)			fo_height_;
  
    odf_types::common_text_align			common_text_align_;
	odf_types::common_vertical_rel_attlist common_vertical_rel_attlist_;
    odf_types::common_vertical_pos_attlist common_vertical_pos_attlist_;

    office_element_ptr			style_list_level_label_alignment_;


};

CP_REGISTER_OFFICE_ELEMENT2(style_list_level_properties);

/// \class  style_list_level_label_alignment
///         style:list-level-label-alignment (style-list-level-label-alignment)
class style_list_level_label_alignment : public office_element_impl<style_list_level_label_alignment>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeStyleListLevelLabelAlignment;
    

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);
	virtual void serialize(std::wostream & strm);

	_CP_OPT(std::wstring) text_label_followed_by_;
	
	_CP_OPT(odf_types::length) text_list_tab_stop_position_;
    _CP_OPT(odf_types::length) fo_text_indent_;
   
	_CP_OPT(odf_types::length) fo_margin_left_;
    _CP_OPT(odf_types::length) fo_margin_right_;
	

};

CP_REGISTER_OFFICE_ELEMENT2(style_list_level_label_alignment);

/// \class  text_list_level_style_number
///         text:list-level-style-number
class text_list_level_style_number : public office_element_impl<text_list_level_style_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextListLevelStyleNumber;
    

  
	office_element_ptr style_list_level_properties_;

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child);
	virtual void serialize(std::wostream & strm);

    text_list_level_style_attr			text_list_level_style_attr_;
    text_list_level_style_number_attr	text_list_level_style_number_attr_;

	office_element_ptr style_text_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_number);

// text:list-level-style-bullet-attr
class text_list_level_style_bullet_attr
{
public:

    _CP_OPT(std::wstring)		text_style_name_;
	//_CP_OPT(wchar_t)			text_bullet_char_;
	_CP_OPT(std::wstring)		text_bullet_char_;
	_CP_OPT(odf_types::percent)	text_bullet_relative_size_;

    odf_types::common_num_format_prefix_suffix_attlist prefix_suffix_attlist_;

	void serialize(CP_ATTR_NODE);
};

// text:list-level-style-bullet
class text_list_level_style_bullet : public office_element_impl<text_list_level_style_bullet>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextListLevelStyleBullet;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);   
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    text_list_level_style_attr			text_list_level_style_attr_;
    text_list_level_style_bullet_attr	text_list_level_style_bullet_attr_;
    
    office_element_ptr					style_text_properties_;
	office_element_ptr					style_list_level_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_bullet);

///         text-list-level-style-image-attr
class text_list_level_style_image_attr
{
public:

    _CP_OPT(std::wstring)			text_style_name_;
	odf_types::common_xlink_attlist	common_xlink_attlist_;

 	void serialize(CP_ATTR_NODE);
};
///         text:list-level-style-image
class text_list_level_style_image : public office_element_impl<text_list_level_style_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;

    static const ElementType type = typeTextListLevelStyleImage;
    

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);   
	virtual void add_child_element( const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

    text_list_level_style_attr			text_list_level_style_attr_;
    text_list_level_style_image_attr	text_list_level_style_image_attr_;
    
    office_element_ptr					style_text_properties_;
	office_element_ptr					style_list_level_properties_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_image);
// text:outline-style
class text_outline_style : public office_element_impl<text_outline_style>
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;

	static const ElementType type = typeTextOutlineStyle;
	

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	std::wstring					style_name_;
	office_element_ptr_array		content_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_outline_style);

// text:outline-level-style
class text_outline_level_style : public office_element_impl<text_outline_level_style>
{
public:
	static const wchar_t * ns;
	static const wchar_t * name;

	static const ElementType type = typeTextOutlineLevelStyle;
	

	virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element(const office_element_ptr & child);

	virtual void serialize(std::wostream & strm);

	std::wstring										text_style_name_;
	odf_types::common_num_format_attlist				common_num_format_attlist_;
	odf_types::common_num_format_prefix_suffix_attlist	common_num_format_prefix_suffix_attlist_;

	unsigned int text_level_;
	unsigned int text_display_levels_;
	unsigned int text_start_value_;

	office_element_ptr	list_level_properties_;
	office_element_ptr	text_properties_;
};
CP_REGISTER_OFFICE_ELEMENT2(text_outline_level_style);

} 
}
