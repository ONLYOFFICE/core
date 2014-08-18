#include "precompiled_cpodf.h"
#include "styles_list.h"

#include <cpdoccore/xml/serialize.h>
//#include "style_text_properties.h"

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {
    

///////////////////////////////////////
void text_list_style_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"style:name",					style_name_);
    CP_XML_ATTR_OPT(L"style:display-name",			style_display_name_);
    CP_XML_ATTR_OPT(L"text:consecutive-numbering",	text_consecutive_numbering_);
 }

void text_list_level_style_number_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name"		,	text_style_name_);
    CP_XML_ATTR_OPT(L"text:display-levels"	,	text_display_levels_);
    CP_XML_ATTR_OPT(L"text:start-value"		,	text_start_value_);

	common_num_format_attlist_.serialize(CP_GET_XML_NODE());
    common_num_format_prefix_suffix_attlist_.serialize(CP_GET_XML_NODE());
 }
void text_list_level_style_bullet_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name",			text_style_name_);
    CP_XML_ATTR_OPT(L"text:bullet-char",		text_bullet_char_);
    CP_XML_ATTR_OPT(L"text:bullet-relative-size",	text_bullet_relative_size_);

    common_num_format_prefix_suffix_attlist_.serialize(CP_GET_XML_NODE());
 }
void text_list_level_style_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR(L"text:level",	text_level_);
 }

// text:list-style
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_style::ns = L"text";
const wchar_t * text_list_style::name = L"list-style";


void text_list_style::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"text" == Ns && L"list-level-style-number" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);
    else if (L"text" == Ns && L"list-level-style-bullet" == Name)
        CP_CREATE_ELEMENT(text_list_style_content_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_list_style::add_child_element( office_element_ptr & child)
{
 	ElementType type = child->get_type();
	
	text_list_style_content_.push_back(child);
}
void text_list_style::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			text_list_style_attr_.serialize(CP_GET_XML_NODE());

			BOOST_FOREACH(office_element_ptr & elm, text_list_style_content_)
			{
				if (elm) elm->serialize(CP_XML_STREAM());
			}	
		}
	}
}

// text:list-level-style-number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_number::ns = L"text";
const wchar_t * text_list_level_style_number::name = L"list-level-style-number";


void text_list_level_style_number::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if	(L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);    
	else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_); 
	else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_list_level_style_number::add_child_element( office_element_ptr & child)
{
 	ElementType type = child->get_type();
	
	if (type == typeStyleListLevelProperties)
		style_list_level_properties_ = child;
    else if (type == typeStyleTextProperties)
       style_text_properties_ = child;    
    else
    {
    }
}
void text_list_level_style_number::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			text_list_level_style_attr_.serialize(CP_GET_XML_NODE());
			text_list_level_style_number_attr_.serialize(CP_GET_XML_NODE());	

			if (style_list_level_properties_)	style_list_level_properties_->serialize(CP_XML_STREAM());
			if (style_text_properties_)			style_text_properties_->serialize(CP_XML_STREAM());
		}
	}
}
// style:list-level-properties
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_properties::ns = L"style";
const wchar_t * style_list_level_properties::name = L"list-level-properties";

void style_list_level_properties::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"style", L"list-level-label-alignment")
    {
        CP_CREATE_ELEMENT(style_list_level_label_alignment_);
    }
    else
    {
        CP_NOT_APPLICABLE_ELM();
    }
}
void style_list_level_properties::add_child_element( office_element_ptr & child)
{
 	ElementType type = child->get_type();
	
	if (type == typeStyleListLevelProperties)
		style_list_level_label_alignment_ = child;
    else
    {
    }
}
void style_list_level_properties::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"text:space-before",		text_space_before_);
			CP_XML_ATTR_OPT(L"text:min-label-width",	text_min_label_width_);
			CP_XML_ATTR_OPT(L"text:min-label-distance",	text_min_label_distance_);
			CP_XML_ATTR_OPT(L"style:font-name",			style_font_name_);
			CP_XML_ATTR_OPT(L"fo:width",				fo_width_);
			CP_XML_ATTR_OPT(L"fo:height",				fo_height_);
		  
			common_text_align_.serialize(CP_GET_XML_NODE());
			common_vertical_rel_attlist_.serialize(CP_GET_XML_NODE());
			common_vertical_pos_attlist_.serialize(CP_GET_XML_NODE());

			if (style_list_level_label_alignment_)
			{
				CP_XML_ATTR(L"text:list-level-position-and-space-mode", L"label-alignment");
				style_list_level_label_alignment_->serialize(CP_XML_STREAM());
			}
		}
	}
}
// style:list-level-label-alignment
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_list_level_label_alignment::ns = L"style";
const wchar_t * style_list_level_label_alignment::name = L"list-level-label-alignment";

void style_list_level_label_alignment::create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name)
{
     CP_NOT_APPLICABLE_ELM();
}
void style_list_level_label_alignment::add_child_element( office_element_ptr & child)
{
//   CP_NOT_APPLICABLE_ELM();
}

void style_list_level_label_alignment::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR_OPT(L"text:label-followed-by",		text_label_followed_by_);
			CP_XML_ATTR_OPT(L"text:list-tab-stop-position",	text_list_tab_stop_position_);
			CP_XML_ATTR_OPT(L"fo:text-indent",				fo_text_indent_);
			CP_XML_ATTR_OPT(L"fo:margin-left",				fo_margin_left_);
			CP_XML_ATTR_OPT(L"fo:margin-right",				fo_margin_right_);
		}
	}
}
// text:list-level-style-bullet
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_level_style_bullet::ns = L"text";
const wchar_t * text_list_level_style_bullet::name = L"list-level-style-bullet";


void text_list_level_style_bullet::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"list-level-properties" == Name)
        CP_CREATE_ELEMENT(style_list_level_properties_);
    else if (L"style" == Ns && L"text-properties" == Name)
        CP_CREATE_ELEMENT(style_text_properties_);    
    else
    {
         CP_NOT_APPLICABLE_ELM();
    }
}
void text_list_level_style_bullet::add_child_element( office_element_ptr & child)
{
 	ElementType type = child->get_type();
	
	if (type == typeStyleListLevelProperties)
		style_list_level_properties_ = child;
    else if (type == typeStyleTextProperties)
       style_text_properties_ = child;    
    else
    {
    }
}
void text_list_level_style_bullet::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
    {
		CP_XML_NODE_SIMPLE()
        {
			text_list_level_style_attr_.serialize(CP_GET_XML_NODE());
			text_list_level_style_bullet_attr_.serialize(CP_GET_XML_NODE());	

			if (style_list_level_properties_)	style_list_level_properties_->serialize(CP_XML_STREAM());
			if (style_text_properties_)			style_text_properties_->serialize(CP_XML_STREAM());
		}
	}
}
}
}
