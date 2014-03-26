#pragma once

#include <iosfwd>
#include <string>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "common_attlists.h"


namespace cpdoccore { 
namespace odf { 

class text_list_style_attr
{
public:

    const std::wstring & get_style_name() { return style_name_; }

    std::wstring style_name_;
    optional<std::wstring>::Type style_display_name_;
    optional<bool>::Type text_consecutive_numbering_;

};

/// \class  text_list_style
///         text:list-style (text-list-style)
class text_list_style : public office_element_impl<text_list_style>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListStyle;
    CPDOCCORE_DEFINE_VISITABLE();

    const std::wstring & get_style_name() { return text_list_style_attr_.get_style_name(); }
    office_element_ptr_array & get_content() { return text_list_style_content_; }

    virtual void create_child_element(  const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm){}

    text_list_style_attr text_list_style_attr_;
    office_element_ptr_array text_list_style_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_style);

/// \class  text_list_level_style_number_attr
///         text-list-level-style-number-attr
class text_list_level_style_number_attr
{
public:
    text_list_level_style_number_attr() : text_display_levels_(1), text_start_value_(1) {}

	_CP_OPT(style_ref) text_style_name_;
    
	common_num_format_attlist common_num_format_attlist_;
    common_num_format_prefix_suffix_attlist common_num_format_prefix_suffix_attlist_;
    
	unsigned int text_display_levels_;
    unsigned int text_start_value_;

    friend class text_list_level_style_number;
};


/// text-list-level-style-attr
class text_list_level_style_attr
{
public:
    text_list_level_style_attr() : text_level_(0) {}

	unsigned int get_text_level() const { return text_level_; }

    unsigned int text_level_;

};


/// \class  style_list_level_properties
///         style:list-level-properties (style-list-level-properties)
class style_list_level_properties : public office_element_impl<style_list_level_properties>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleListLevelProperties;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm){}

    friend class text_list_level_style_number;
    friend class text_list_level_style_bullet;
    
	const common_text_align & get_common_text_align() const { return common_text_align_; }

    common_text_align common_text_align_;
    optional<length>::Type text_space_before_;
    optional<length>::Type text_min_label_width_;
    optional<length>::Type text_min_label_distance_;
    optional<std::wstring>::Type style_font_name_;
    optional<length>::Type fo_width_;
    optional<length>::Type fo_height_;
  
	common_vertical_rel_attlist common_vertical_rel_attlist_;
    common_vertical_pos_attlist common_vertical_pos_attlist_;

    office_element_ptr style_list_level_label_alignment_;


};

CP_REGISTER_OFFICE_ELEMENT2(style_list_level_properties);

/// \class  style_list_level_label_alignment
///         style:list-level-label-alignment (style-list-level-label-alignment)
class style_list_level_label_alignment : public office_element_impl<style_list_level_label_alignment>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleListLevelLabelAlignment;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element(const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm){}

    const _CP_OPT(length) & get_fo_text_indent() const { return fo_text_indent_; }
    const _CP_OPT(length) & get_fo_margin_left() const { return fo_margin_left_; }
    const _CP_OPT(length) & get_text_list_tab_stop_position() const { return text_list_tab_stop_position_; }

    _CP_OPT(std::wstring) text_label_followed_by_;

	_CP_OPT(length) text_list_tab_stop_position_;
    _CP_OPT(length) fo_text_indent_;
    _CP_OPT(length) fo_margin_left_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_list_level_label_alignment);

/// \class  text_list_level_style_number
///         text:list-level-style-number
class text_list_level_style_number : public office_element_impl<text_list_level_style_number>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListLevelStyleNumber;
    CPDOCCORE_DEFINE_VISITABLE();

  
	office_element_ptr style_list_level_properties_;

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
    virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm){}

    text_list_level_style_attr text_list_level_style_attr_;
    text_list_level_style_number_attr text_list_level_style_number_attr_;

	office_element_ptr style_text_properties_;

};

CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_number);

/// \class  text_list_level_style_bullet_attr
///         text-list-level-style-bullet-attr
class text_list_level_style_bullet_attr
{
public:

    _CP_OPT(style_ref) text_style_name_;
    optional<wchar_t>::Type text_bullet_char_;
    common_num_format_prefix_suffix_attlist common_num_format_prefix_suffix_attlist_;
    optional<percent>::Type text_bullet_relative_size_;


};

/// \class  text_list_level_style_bullet
///         text:list-level-style-bullet
class text_list_level_style_bullet : public office_element_impl<text_list_level_style_bullet>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTextListLevelStyleBullet;
    CPDOCCORE_DEFINE_VISITABLE();

   
	office_element_ptr style_list_level_properties_;

    virtual void create_child_element( const std::wstring & Ns, const std::wstring & Name);
   
	virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm){}

    text_list_level_style_attr text_list_level_style_attr_;
    text_list_level_style_bullet_attr text_list_level_style_bullet_attr_;
    
    office_element_ptr style_text_properties_;
};

CP_REGISTER_OFFICE_ELEMENT2(text_list_level_style_bullet);


} }
