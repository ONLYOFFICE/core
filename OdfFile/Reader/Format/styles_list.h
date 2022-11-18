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

#include <xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "../../DataTypes/common_attlists.h"

namespace cpdoccore { namespace odf_reader { 

class text_list_style_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

    std::wstring					style_name_;
    optional<std::wstring>::Type	style_display_name_;
    optional<bool>::Type			text_consecutive_numbering_;

};

/// text:list-style (text-list-style)
class text_list_style : public office_element_impl<text_list_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListStyle;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    text_list_style_attr			attr_;
    office_element_ptr_array		content_;
    friend class odf_document;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_style);

/// text-list-level-style-number-attr
class text_list_level_style_number_attr
{
public:
    text_list_level_style_number_attr() : text_display_levels_(1), text_start_value_(1) {}
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
    _CP_OPT(std::wstring)								text_style_name_;
    
	odf_types::common_num_format_attlist				common_num_format_attlist_;
    odf_types::common_num_format_prefix_suffix_attlist	common_num_format_prefix_suffix_attlist_;
    
	unsigned int text_display_levels_;
    unsigned int text_start_value_;

    friend class text_list_level_style_number;
};


/// text-list-level-style-attr
class text_list_level_style_attr
{
public:
    text_list_level_style_attr() : text_level_(0) {}
    void add_attributes( const xml::attributes_wc_ptr & Attributes );
    unsigned int get_text_level() const { return text_level_; }

private:
    unsigned int text_level_;

};


/// style:list-level-properties (style-list-level-properties)
class style_list_level_properties : public office_element_impl<style_list_level_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleListLevelProperties;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    friend class text_list_level_style_number;
    friend class text_list_level_style_bullet;
    
    odf_types::common_text_align			common_text_align_;
    optional<odf_types::length>::Type		text_space_before_;
    optional<odf_types::length>::Type		text_min_label_width_;
    optional<odf_types::length>::Type		text_min_label_distance_;
    optional<std::wstring>::Type			style_font_name_;
    optional<odf_types::length>::Type		fo_width_;
    optional<odf_types::length>::Type		fo_height_;
  
	odf_types::common_vertical_rel_attlist	common_vertical_rel_attlist_;
    odf_types::common_vertical_pos_attlist	common_vertical_pos_attlist_;

    office_element_ptr style_list_level_label_alignment_;
};

CP_REGISTER_OFFICE_ELEMENT2(style_list_level_properties);

/// style:list-level-label-alignment (style-list-level-label-alignment)
class style_list_level_label_alignment : public office_element_impl<style_list_level_label_alignment>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleListLevelLabelAlignment;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

public:
    _CP_OPT(std::wstring) text_label_followed_by_;

    _CP_OPT(odf_types::length) text_list_tab_stop_position_;
    _CP_OPT(odf_types::length) fo_text_indent_;
    _CP_OPT(odf_types::length) fo_margin_left_;

    friend class text_list_level_style_number;
};

CP_REGISTER_OFFICE_ELEMENT2(style_list_level_label_alignment);

/// text:list-level-style-number
class text_list_level_style_number : public office_element_impl<text_list_level_style_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListLevelStyleNumber;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    text_list_level_style_attr			text_list_level_style_attr_;
    text_list_level_style_number_attr	text_list_level_style_number_attr_;

	office_element_ptr					list_level_properties_;
	office_element_ptr					style_text_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_number);

/// text-list-level-style-bullet-attr
class text_list_level_style_bullet_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
    _CP_OPT(std::wstring)								text_style_name_;
    _CP_OPT(std::wstring)								text_bullet_char_;
    odf_types::common_num_format_prefix_suffix_attlist	common_num_format_prefix_suffix_attlist_;
    _CP_OPT(odf_types::percent)							text_bullet_relative_size_;

    friend class text_list_level_style_bullet;

};




/// text:list-level-style-bullet
class text_list_level_style_bullet : public office_element_impl<text_list_level_style_bullet>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListLevelStyleBullet;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context) ;
    void pptx_convert(oox::pptx_conversion_context & Context) ;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    text_list_level_style_attr			text_list_level_style_attr_;
    text_list_level_style_bullet_attr	text_list_level_style_bullet_attr_;
    
	office_element_ptr					list_level_properties_;
    office_element_ptr					style_text_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_bullet);

/// text-list-level-style-image-attr
class text_list_level_style_image_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

private:
	odf_types::common_xlink_attlist  xlink_attlist_;

    friend class text_list_level_style_image;

};


/// text:list-level-style-image
class text_list_level_style_image : public office_element_impl<text_list_level_style_image>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListLevelStyleImage;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context) ;
    void pptx_convert(oox::pptx_conversion_context & Context) ;
    
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);

    text_list_level_style_attr			text_list_level_style_attr_;
    text_list_level_style_image_attr	text_list_level_style_image_attr_;
    
	office_element_ptr					list_level_properties_;
    office_element_ptr					style_text_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_image);

// text:outline-style
class text_outline_style : public office_element_impl<text_outline_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextOutlineStyle;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
public:
    std::wstring					style_name_;
    office_element_ptr_array		content_;
    friend class odf_document;
};
CP_REGISTER_OFFICE_ELEMENT2(text_outline_style);

// text:outline-level-style
class text_outline_level_style : public office_element_impl<text_outline_level_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextOutlineLevelStyle;
    CPDOCCORE_DEFINE_VISITABLE();

    void docx_convert(oox::docx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name);
public:
    std::wstring										text_style_name_;
	odf_types::common_num_format_attlist				common_num_format_attlist_;
    odf_types::common_num_format_prefix_suffix_attlist	common_num_format_prefix_suffix_attlist_;
   
	unsigned int text_level_;
	unsigned int text_display_levels_;
    unsigned int text_start_value_;

	office_element_ptr	list_level_properties_;
 	office_element_ptr	text_properties_;

   friend class odf_document;
};
CP_REGISTER_OFFICE_ELEMENT2(text_outline_level_style);

} 
}
