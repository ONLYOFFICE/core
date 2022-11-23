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

#include "text_elements.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>

#include "paragraph_elements.h"
#include "list.h"

#include "odf_conversion_context.h"
#include "odf_text_context.h"

#include "style_paragraph_properties.h"
#include "style_text_properties.h"


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

using xml::xml_char_wc;

// text:h
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_h::ns		= L"text";
const wchar_t * text_h::name	= L"h";

void paragraph_attrs::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT(L"text:style-name",		text_style_name_);
    CP_XML_ATTR_OPT(L"text:cond-style-name",text_cond_style_name_);
   
	std::wstring text_class_names_all;

	for (size_t i = 0; i < text_class_names_.size(); i++)
	{
		text_class_names_all = text_class_names_[i] + std::wstring(L" ");
	}	
	if (!text_class_names_all.empty())
		CP_XML_ATTR(L"text:class-names",	text_class_names_all);
}
//////////////////////////////////////////////


text_h::text_h() : text_outline_level_(1), text_restart_numbering_(false), text_is_list_header_(false)
{}

void text_h::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_ATTR		(L"text:outline-level",		text_outline_level_);
			CP_XML_ATTR		(L"text:restart-numbering",	text_restart_numbering_);
			CP_XML_ATTR_OPT	(L"text:start-value",		text_start_value_);
			CP_XML_ATTR		(L"text:is-list-header",	text_is_list_header_);

			paragraph_attrs_.serialize(CP_GET_XML_NODE());
			
			 for (size_t i = 0; i < paragraph_content_.size(); i++)
			 {
				 paragraph_content_[i]->serialize(CP_XML_STREAM());
			 }
		}
	}
}
//
void text_h::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(paragraph_content_);
}

void text_h::add_text(const std::wstring & Text)
{
	office_element_ptr elm = text_text::create(Text);
	paragraph_content_.push_back(elm);
}
void text_h::add_child_element( const office_element_ptr & child_element)
{
	paragraph_content_.push_back(child_element);
}
// text:p
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_p::ns		= L"text";
const wchar_t * text_p::name	= L"p";

void text_p::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
	CP_CREATE_ELEMENT(paragraph_content_);
}
void text_p::add_child_element( const office_element_ptr & child_element)
{
	paragraph_content_.push_back(child_element);
}
void text_p::add_text(const std::wstring & Text)
{
	office_element_ptr elm = text_text::create(Text);
	paragraph_content_.push_back(elm);
}

void text_p::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			paragraph_attrs_.serialize(CP_GET_XML_NODE());
			 
			 for (size_t i = 0; i < paragraph_content_.size(); i++)
			 {
				 paragraph_content_[i]->serialize(CP_XML_STREAM());
			 }
		}
	}
}


// text:list
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_list::ns = L"text";
const wchar_t * text_list::name = L"list";

void text_list::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"text:style-name"			, text_style_name_);
			CP_XML_ATTR_OPT(L"text:continue-numbering"	, text_continue_numbering_ );
			
			if (text_list_header_) text_list_header_->serialize(CP_XML_STREAM());
   
			for (size_t i = 0; i < text_list_items_.size(); i++)
			{
				text_list_items_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_list::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"list-header")
    {
        CP_CREATE_ELEMENT(text_list_header_);
    }
    else
    {
        CP_CREATE_ELEMENT(text_list_items_);
    }
}
void text_list::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextListHeader)
		text_list_header_ = child_element;
	else
		text_list_items_.push_back(child_element);
}
// text:soft-page-break
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_soft_page_break::ns = L"text";
const wchar_t * text_soft_page_break::name = L"soft-page-break";

void text_soft_page_break::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}

// text-section-attr
//--------------------------------------------------------------------------------------------------------

void text_section_attr::serialize(CP_ATTR_NODE)
{
    CP_XML_ATTR_OPT	(L"text:style-name",	text_style_name_);
    CP_XML_ATTR		(L"text:name",			text_name_);
    CP_XML_ATTR_OPT	(L"text:protected",		text_protected_);
    CP_XML_ATTR_OPT	(L"text:protection-key", text_protection_key_);
    CP_XML_ATTR_OPT	(L"text:display",		text_display_);
    CP_XML_ATTR_OPT	(L"text:condition",		text_condition_);
}

// text:section
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_section::ns	= L"text";
const wchar_t * text_section::name	= L"section";

void text_section::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"section-source" == Name)	CP_CREATE_ELEMENT(text_section_source_);    
    else											CP_CREATE_ELEMENT(content_);
}

void text_section::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextSectionSource)
		text_section_source_ = child_element;
	else
		content_.push_back(child_element);
}

void text_section::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			text_section_attr_.serialize(CP_GET_XML_NODE());

			if (text_section_source_)
				text_section_source_->serialize(CP_XML_STREAM());
 			
			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}   
		}
	}
}

// text-section-source-attr
//--------------------------------------------------------------------------------------------------------

void text_section_source_attr::serialize(CP_ATTR_NODE)
{
    common_xlink_attlist_.serialize(CP_GET_XML_NODE());

    CP_XML_ATTR_OPT(L"text:section-name", text_section_name_);
    CP_XML_ATTR_OPT(L"text:filter-name", text_filter_name_);
}

// text:section-source
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_section_source::ns		= L"text";
const wchar_t * text_section_source::name	= L"section-source";

void text_section_source::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}
void text_section_source::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			text_section_source_attr_.serialize(CP_GET_XML_NODE());
		}
	}
}
// text:index-body
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_index_body::ns		= L"text";
const wchar_t * text_index_body::name	= L"index-body";

void text_index_body::serialize(std::wostream & _Wostream)
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			for (size_t i = 0 ; i < index_content_main_.size(); i++)
			{
				index_content_main_[i]->serialize(CP_XML_STREAM());
			}
		}
	} 
}

void text_index_body::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(index_content_main_);
}

void text_index_body::add_child_element( const office_element_ptr & child_element)
{
    index_content_main_.push_back(child_element);
}

// text:index-title
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_index_title::ns	= L"text";
const wchar_t * text_index_title::name	= L"index-title";

void text_index_title::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			for (size_t i = 0 ; i < index_content_main_.size(); i++)
			{
				index_content_main_[i]->serialize(CP_XML_STREAM());
			}
		}
	} 
}
void text_index_title::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
     CP_CREATE_ELEMENT(index_content_main_);
}
void text_index_title::add_child_element( const office_element_ptr & child_element)
{
     index_content_main_.push_back(child_element);
}

// text:table-of-content
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_table_of_content::ns	= L"text";
const wchar_t * text_table_of_content::name = L"table-of-content";

void text_table_of_content::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			text_section_attr_.serialize(CP_GET_XML_NODE());

			if (table_of_content_source_)
				table_of_content_source_->serialize(CP_XML_STREAM());

			if (index_body_)
				index_body_->serialize(CP_XML_STREAM());
		}
	} 
}
void text_table_of_content::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	if (L"text" == Ns && L"table-of-content-source" == Name)	CP_CREATE_ELEMENT(table_of_content_source_);    
    else if CP_CHECK_NAME(L"text", L"index-body")				CP_CREATE_ELEMENT(index_body_);
}

void text_table_of_content::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeTextTableOfContentSource)
		table_of_content_source_= child_element;
	else if (type_ == typeTextIndexBody)
		index_body_ = child_element;
}
//--------------------------------------------------------------------------------------------------------

// text:tracked-changes
//--------------------------------------------------------------------------------------------------------
const wchar_t * text_tracked_changes::ns	= L"text";
const wchar_t * text_tracked_changes::name	= L"tracked-changes";

void text_tracked_changes::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"index-body")
    {
        CP_CREATE_ELEMENT(content_);
    }
}
void text_tracked_changes::add_child_element( const office_element_ptr & child_element)
{
    content_.push_back(child_element);
}

void text_tracked_changes::serialize(std::wostream & _Wostream)
{
	if (content_.empty()) return;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize (CP_XML_STREAM());
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------
const wchar_t * text_insertion::ns			= L"text";
const wchar_t * text_insertion::name		= L"insertion";

void text_insertion::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			if (office_change_info_)
				office_change_info_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//---------------------------------------------------------------------------------------------------
const wchar_t * text_deletion::ns			= L"text";
const wchar_t * text_deletion::name			= L"deletion";

void text_deletion::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			if (office_change_info_)
				office_change_info_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//---------------------------------------------------------------------------------------------------

const wchar_t * text_format_change::ns		= L"text";
const wchar_t * text_format_change::name	= L"format-change";

void text_format_change::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:style-name", text_style_name_);
			
			if (office_change_info_)
				office_change_info_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_unknown_change::ns		= L"text";
const wchar_t * text_unknown_change::name	= L"UnknownChange";

void text_unknown_change::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if (L"office" == Ns && L"change-info" == Name)	CP_CREATE_ELEMENT(office_change_info_);    
    else											CP_CREATE_ELEMENT(content_);
}

void text_unknown_change::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeOfficeChangeInfo)
		office_change_info_= child_element;
	else
		content_.push_back(child_element);
}

void text_unknown_change::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			if (office_change_info_)
				office_change_info_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}

//---------------------------------------------------------------------------------------------------
const wchar_t * text_add_change::ns		= L"text";
const wchar_t * text_add_change::name	= L"unknown-change-mark";
//---------------------------------------------------------------------------------------------------
const wchar_t * text_change::ns			= L"text";
const wchar_t * text_change::name		= L"change";

void text_change::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"text:change-id", text_change_id_);
		}
	}
}
//------------------------------------------------------------------------------------------
const wchar_t * text_change_start::ns	= L"text";
const wchar_t * text_change_start::name	= L"change-start";

void text_change_start::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"text:change-id", text_change_id_);
		}
	}
}

//------------------------------------------------------------------------------------------
const wchar_t * text_change_end::ns		= L"text";
const wchar_t * text_change_end::name	= L"change-end";

void text_change_end::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"text:change-id", text_change_id_);
		}
	}
}

//---------------------------------------------------------------------------------------------------
const wchar_t * text_changed_region::ns		= L"text";
const wchar_t * text_changed_region::name	= L"changed-region";

void text_changed_region::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(element_);
}
void text_changed_region::add_child_element( const office_element_ptr & child_element)
{
    element_ = child_element;
}

void text_changed_region::serialize(std::wostream & _Wostream)
{
	if (!element_) return;

	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			CP_XML_ATTR_OPT(L"xml:id", text_id_);
			CP_XML_ATTR_OPT(L"text:id", text_id_);
			
			element_->serialize (CP_XML_STREAM());
		}
	}
}
//------------------------------------------------------------
const wchar_t * text_table_index::ns	= L"text";
const wchar_t * text_table_index::name	= L"table-index";

void text_table_index::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if		(L"text" == Ns && L"index-body" == Name)		CP_CREATE_ELEMENT(index_body_);    
    else if (L"text" == Ns && L"table-index-source" == Name)CP_CREATE_ELEMENT(table_index_source_);    
}

void text_table_index::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextIndexBody)
		index_body_= child_element;
    else if (type_ == typeTextTableIndexSource)
		table_index_source_= child_element;
}

void text_table_index::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			section_attr_.serialize(CP_GET_XML_NODE());

			if (table_index_source_)
				table_index_source_->serialize(CP_XML_STREAM());

			if (index_body_)
				index_body_->serialize(CP_XML_STREAM());
		}
	}
}
//------------------------------------------------------------
const wchar_t * text_illustration_index::ns		= L"text";
const wchar_t * text_illustration_index::name	= L"illustration-index";

void text_illustration_index::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if		(L"text" == Ns && L"index-body" == Name)				CP_CREATE_ELEMENT(index_body_);    
    else if (L"text" == Ns && L"illustration-index-source" == Name)	CP_CREATE_ELEMENT(illustration_index_source_);    
}

void text_illustration_index::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeTextIllustrationIndexSource)
		illustration_index_source_= child_element;
	else if (type_ == typeTextIndexBody)
		index_body_= child_element;
}

void text_illustration_index::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			section_attr_.serialize(CP_GET_XML_NODE());

			if (illustration_index_source_)
				illustration_index_source_->serialize(CP_XML_STREAM());

			if (index_body_)
				index_body_->serialize(CP_XML_STREAM());
		}
	}
}
//------------------------------------------------------------
const wchar_t * text_user_index::ns		= L"text";
const wchar_t * text_user_index::name	= L"user-index";

void text_user_index::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if		(L"text" == Ns && L"index-body" == Name)		CP_CREATE_ELEMENT(index_body_);    
    else if (L"text" == Ns && L"user-index-source" == Name)	CP_CREATE_ELEMENT(user_index_source_);    
}

void text_user_index::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeTextUserIndexSource)
		user_index_source_= child_element;
	else if (type_ == typeTextIndexBody)
		index_body_= child_element;
}

void text_user_index::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			section_attr_.serialize(CP_GET_XML_NODE());

			if (user_index_source_)
				user_index_source_->serialize(CP_XML_STREAM());

			if (index_body_)
				index_body_->serialize(CP_XML_STREAM());
		}
	}
}
//------------------------------------------------------------
const wchar_t * text_alphabetical_index::ns		= L"text";
const wchar_t * text_alphabetical_index::name	= L"alphabetical-index";

void text_alphabetical_index::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if		(L"text" == Ns && L"index-body" == Name)				CP_CREATE_ELEMENT(index_body_);    
    else if (L"text" == Ns && L"alphabetical-index-source" == Name)	CP_CREATE_ELEMENT(alphabetical_index_source_);    
}

void text_alphabetical_index::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeTextAlphabeticalIndexSource)
		alphabetical_index_source_= child_element;
	else if (type_ == typeTextIndexBody)
		index_body_= child_element;
}

void text_alphabetical_index::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			section_attr_.serialize(CP_GET_XML_NODE());

			if (alphabetical_index_source_)
				alphabetical_index_source_->serialize(CP_XML_STREAM());

			if (index_body_)
				index_body_->serialize(CP_XML_STREAM());
		}
	}
}
//------------------------------------------------------------
const wchar_t * text_bibliography::ns	= L"text";
const wchar_t * text_bibliography::name	= L"bibliography";

void text_bibliography::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if		(L"text" == Ns && L"index-body" == Name)			CP_CREATE_ELEMENT(index_body_);    
    else if (L"text" == Ns && L"bibliography-source" == Name)	CP_CREATE_ELEMENT(bibliography_source_);    
}

void text_bibliography::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

	if (type_ == typeTextBibliographySource)
		bibliography_source_= child_element;
	else if (type_ == typeTextIndexBody)
		index_body_= child_element;
}

void text_bibliography::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			section_attr_.serialize(CP_GET_XML_NODE());

			if (bibliography_source_)
				bibliography_source_->serialize(CP_XML_STREAM());

			if (index_body_)
				index_body_->serialize(CP_XML_STREAM());
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_bibliography_source::ns	= L"text";
const wchar_t * text_bibliography_source::name	= L"bibliography-source";

void text_bibliography_source::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"index-title-template" == Name)	CP_CREATE_ELEMENT(index_title_template_);    
    else													CP_CREATE_ELEMENT(entry_templates_);
}

void text_bibliography_source::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextIndexTitleTemplate)
		index_title_template_ = child_element;
	else
		entry_templates_.push_back(child_element);
}

void text_bibliography_source::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			if (index_title_template_)
				index_title_template_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < entry_templates_.size(); i++)
			{
				entry_templates_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_table_of_content_source::ns	= L"text";
const wchar_t * text_table_of_content_source::name	= L"table-of-content-source";

void text_table_of_content_source::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if		(L"text" == Ns && L"index-title-template" == Name)	CP_CREATE_ELEMENT(index_title_template_);    
	else if (L"text" == Ns && L"index-source-styles" == Name)	CP_CREATE_ELEMENT(index_source_styles_);    
	else														CP_CREATE_ELEMENT(entry_templates_);
}

void text_table_of_content_source::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextIndexTitleTemplate)
		index_title_template_ = child_element;
    else if (type_ == typeTextIndexSourceStyles)
		index_source_styles_.push_back(child_element);
	else
		entry_templates_.push_back(child_element);
}

void text_table_of_content_source::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:outline-level", outline_level_);
			CP_XML_ATTR_OPT(L"text:use-index-marks", use_index_marks_);
			CP_XML_ATTR_OPT(L"text:use-index-source-styles", use_index_source_styles_);
			CP_XML_ATTR_OPT(L"text:use-outline-level", use_outline_level_);
			CP_XML_ATTR_OPT(L"text:relative-tab-stop-position", relative_tab_stop_position_);
			CP_XML_ATTR_OPT(L"text:index-scope", index_scope_); // chapter or document:
			
			if (index_title_template_)
				index_title_template_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < index_source_styles_.size(); i++)
			{
				index_source_styles_[i]->serialize(CP_XML_STREAM());
			}  
			for (size_t i = 0 ; i < entry_templates_.size(); i++)
			{
				entry_templates_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_illustration_index_source::ns		= L"text";
const wchar_t * text_illustration_index_source::name	= L"illustration-index-source";

void text_illustration_index_source::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"index-title-template" == Name)	CP_CREATE_ELEMENT(index_title_template_);    
    else													CP_CREATE_ELEMENT(entry_templates_);
}

void text_illustration_index_source::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextIndexTitleTemplate)
		index_title_template_ = child_element;
	else
		entry_templates_.push_back(child_element);
}

void text_illustration_index_source::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:caption-sequence-name", caption_sequence_name_);
			CP_XML_ATTR_OPT(L"text:caption-sequence-format", caption_sequence_format_);

			CP_XML_ATTR_OPT(L"text:relative_tab-stop-position", relative_tab_stop_position_);
			CP_XML_ATTR_OPT(L"text:use-caption", use_caption_);
			CP_XML_ATTR_OPT(L"text:index-scope", index_scope_); // chapter or document:

			if (index_title_template_)
				index_title_template_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < entry_templates_.size(); i++)
			{
				entry_templates_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_user_index_source::ns		= L"text";
const wchar_t * text_user_index_source::name	= L"user-index-source";

void text_user_index_source::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"index-title-template" == Name)	CP_CREATE_ELEMENT(index_title_template_);    
    else													CP_CREATE_ELEMENT(entry_templates_);
}

void text_user_index_source::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextIndexTitleTemplate)
		index_title_template_ = child_element;
	else
		entry_templates_.push_back(child_element);
}

void text_user_index_source::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:index-name", index_name_);
			CP_XML_ATTR_OPT(L"text:copy-outline-levels", copy_outline_levels_);

			CP_XML_ATTR_OPT(L"text:use-chart-objects", use_chart_objects_);
			CP_XML_ATTR_OPT(L"text:use-draw-objects", use_draw_objects_);
			CP_XML_ATTR_OPT(L"text:use-graphics", use_graphics_);
			CP_XML_ATTR_OPT(L"text:use-floating-frames", use_floating_frames_);
			CP_XML_ATTR_OPT(L"text:use-index-marks", use_index_marks_);
			CP_XML_ATTR_OPT(L"text:use-index_source_styles", use_index_source_styles_);
			CP_XML_ATTR_OPT(L"text:use-objects", use_objects_);
			CP_XML_ATTR_OPT(L"text:use-tables", use_tables_);			
			CP_XML_ATTR_OPT(L"text:relative_tab-stop-position", relative_tab_stop_position_);
			CP_XML_ATTR_OPT(L"text:index-scope", index_scope_); // chapter or document

			if (index_title_template_)
				index_title_template_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < index_source_styles_.size(); i++)
			{
				index_source_styles_[i]->serialize(CP_XML_STREAM());
			}  
			for (size_t i = 0 ; i < entry_templates_.size(); i++)
			{
				entry_templates_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * alphabetical_index_source::ns	= L"text";
const wchar_t * alphabetical_index_source::name	= L"alphabetical-index-source";

void alphabetical_index_source::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"index-title-template" == Name)	CP_CREATE_ELEMENT(index_title_template_);    
    else													CP_CREATE_ELEMENT(entry_templates_);
}

void alphabetical_index_source::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextIndexTitleTemplate)
		index_title_template_ = child_element;
	else
		entry_templates_.push_back(child_element);
}

void alphabetical_index_source::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:index-scope", index_scope_); // chapter or document:

			if (index_title_template_)
				index_title_template_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < entry_templates_.size(); i++)
			{
				entry_templates_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_table_index_source::ns		= L"text";
const wchar_t * text_table_index_source::name	= L"table-index-source";

void text_table_index_source::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if (L"text" == Ns && L"index-title-template" == Name)	CP_CREATE_ELEMENT(index_title_template_);    
    else													CP_CREATE_ELEMENT(entry_templates_);
}

void text_table_index_source::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextIndexTitleTemplate)
		index_title_template_ = child_element;
	else
		entry_templates_.push_back(child_element);
}

void text_table_index_source::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:caption-sequence-name", caption_sequence_name_);
			CP_XML_ATTR_OPT(L"text:caption-sequence-format", caption_sequence_format_);

			CP_XML_ATTR_OPT(L"text:relative_tab-stop-position", relative_tab_stop_position_);
			CP_XML_ATTR_OPT(L"text:use-caption", use_caption_);
			CP_XML_ATTR_OPT(L"text:index-scope", index_scope_); // chapter or document:

			if (index_title_template_)
				index_title_template_->serialize(CP_XML_STREAM());

			for (size_t i = 0 ; i < entry_templates_.size(); i++)
			{
				entry_templates_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_illustration_index_entry_template::ns		= L"text";
const wchar_t * text_illustration_index_entry_template::name	= L"illustration-index-entry-template";
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_alphabetical_index_entry_template::ns		= L"text";
const wchar_t * text_alphabetical_index_entry_template::name	= L"alphabetical-index-entry-template";
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_table_index_entry_template::ns				= L"text";
const wchar_t * text_table_index_entry_template::name			= L"table-index-entry-template";
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_table_of_content_entry_template::ns		= L"text";
const wchar_t * text_table_of_content_entry_template::name		= L"table-of-content-entry-template";
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_user_index_entry_template::ns				= L"text";
const wchar_t * text_user_index_entry_template::name			= L"user-index-entry-template";
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_bibliography_entry_template::ns		= L"text";
const wchar_t * text_bibliography_entry_template::name		= L"bibliography-entry-template";

void text_bibliography_entry_template::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:bibliography-type", bibliography_type_);
			CP_XML_ATTR_OPT(L"text:style-name", style_name_);
			CP_XML_ATTR_OPT(L"text:outline-level", outline_level_);

			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * common_entry_template::ns		= L"";
const wchar_t * common_entry_template::name		= L"";

void common_entry_template::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void common_entry_template::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void common_entry_template::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:style-name", style_name_);
			CP_XML_ATTR_OPT(L"text:outline-level", outline_level_);

			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}  
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_title_template::ns		= L"text";
const wchar_t * text_index_title_template::name		= L"index-title-template";

void text_index_title_template::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:style-name", style_name_);

			if (content_)
				CP_XML_CONTENT(*content_); 
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_bibliography::ns		= L"text";
const wchar_t * text_index_entry_bibliography::name		= L"index-entry-bibliography";

void text_index_entry_bibliography::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        { 
			CP_XML_ATTR_OPT(L"text:style-name", style_name_);
			CP_XML_ATTR_OPT(L"text:bibliography-data-field", bibliography_data_field_);
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_chapter::ns		= L"text";
const wchar_t * text_index_entry_chapter::name		= L"index-entry-chapter";

void text_index_entry_chapter::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_link_end::ns		= L"text";
const wchar_t * text_index_entry_link_end::name		= L"index-entry-link-end";

void text_index_entry_link_end::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_link_start::ns		= L"text";
const wchar_t * text_index_entry_link_start::name		= L"index-entry-link-start";

void text_index_entry_link_start::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_page_number::ns		= L"text";
const wchar_t * text_index_entry_page_number::name		= L"index-entry-page-number";

void text_index_entry_page_number::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_span::ns		= L"text";
const wchar_t * text_index_entry_span::name		= L"index-entry-span";

void text_index_entry_span::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_tab_stop::ns		= L"text";
const wchar_t * text_index_entry_tab_stop::name		= L"index-entry-tab_stop";

void text_index_entry_tab_stop::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_index_entry_text::ns		= L"text";
const wchar_t * text_index_entry_text::name		= L"index-entry-text";

void text_index_entry_text::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE();
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_sequence_decl::ns		= L"text";
const wchar_t * text_sequence_decl::name	= L"sequence-decl";

void text_sequence_decl::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			CP_XML_ATTR_OPT(L"text:name",					name_);
			CP_XML_ATTR_OPT(L"text:separation-character",	separation_character_); 
			CP_XML_ATTR_OPT(L"text:display-outline-level",	display_outline_level_);
		}
	}
}
//----------------------------------------------------------------------------------------------------------
const wchar_t * text_sequence_decls::ns		= L"text";
const wchar_t * text_sequence_decls::name	= L"sequence-decls";

void text_sequence_decls::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}
void text_sequence_decls::add_child_element( const office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void text_sequence_decls::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
		{
			for (size_t i = 0 ; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
}
}
