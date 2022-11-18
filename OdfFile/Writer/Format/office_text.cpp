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

#include "office_text.h"

#include <xml/xmlchar.h>
#include <xml/attributes.h>

namespace cpdoccore { 
namespace odf_writer {

using xml::xml_char_wc;

// office:text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_text::ns = L"office";
const wchar_t * office_text::name = L"text";


namespace { 
bool is_text_content(const std::wstring & ns, const std::wstring & name)
{
    if (ns == L"text")
    {
        return (
            name == L"h" ||
            name == L"p" ||
            name == L"list" ||
            name == L"numbered-paragraph" ||
            name == L"section" ||
            name == L"table-of-content" ||
            name == L"illustration-index" ||
            name == L"table-index" ||
            name == L"object-index" ||
            name == L"user-index" ||
            name == L"alphabetical-index" ||
            name == L"bibliography" ||
            name == L"change" ||
            name == L"change-start" ||
            name == L"change-end"

            );
    
    }
    else if (ns == L"table")
    {
        return name == L"table";
    }
    else if (ns == L"draw" || ns == L"dr3d")
    {
        return true; // all shapes // 
    }

    return false;
}
}
void office_text_attlist::serialize(CP_ATTR_NODE)	
{
	CP_XML_ATTR_OPT(L"text:use-soft-page-breaks", text_use_soft_page_breaks_);
}
void office_text::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
 	if CP_CHECK_NAME(L"office", L"forms") 
	{
		CP_CREATE_ELEMENT(forms_);
	}
	else if CP_CHECK_NAME(L"text", L"tracked-changes") 
	{
		CP_CREATE_ELEMENT(tracked_changes_);
	}
	else if CP_CHECK_NAME(L"table", L"content-validations")
	{
		CP_CREATE_ELEMENT(table_content_validations_);
	}
	else if CP_CHECK_NAME(L"text", L"user-field-decls")
	{
		CP_CREATE_ELEMENT(user_fields_);
	}
	else if CP_CHECK_NAME(L"text", L"sequence-decls")
	{
		CP_CREATE_ELEMENT(sequences_);
	}
	else if CP_CHECK_NAME(L"text", L"variable-decls")
	{
		CP_CREATE_ELEMENT(variables_);
	}
	else if (is_text_content(Ns, Name))
    {
        CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

void office_text::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	switch(child_element->get_type())
	{
		case typeOfficeForms:
		{
			forms_ = child_element;	
		}break;
		case typeTextSequenceDecls:
		{
			sequences_ = child_element;	
		}break;
		case typeTextTrackedChanges:
		{
			tracked_changes_ = child_element;	
		}break;
		case typeTextChangedRegion:
		{
			if (!tracked_changes_)
				create_child_element(L"text", L"tracked-changes");

			tracked_changes_->add_child_element(child_element);
		}break;
		default:
			content_.push_back(child_element);
	}
}
void office_text::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			office_text_attlist_.serialize(CP_GET_XML_NODE());

			if (sequences_)
				sequences_->serialize(CP_XML_STREAM());

			if (forms_)
				forms_->serialize(CP_XML_STREAM());

			if (tracked_changes_)
				tracked_changes_->serialize(CP_XML_STREAM());

			for (int  i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}
// office:change-info
//-------------------------------------------------------------------------------------------------------------------
const wchar_t * office_change_info::ns		= L"office";
const wchar_t * office_change_info::name	= L"change-info";

void office_change_info::create_child_element( const std::wstring & Ns, const std::wstring & Name)
{
	if (Ns==L"dc" && Name == L"date")
	{
		CP_CREATE_ELEMENT(dc_date_);
	}
	else if (Ns==L"dc" && Name == L"creator")
	{
		CP_CREATE_ELEMENT(dc_creator_);
	}
}
void office_change_info::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeDcCreator)
		dc_creator_ = child_element;
	else if (type_ == typeDcDate)
		dc_date_ = child_element;
}
void office_change_info::serialize(std::wostream & _Wostream)
{
	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {   
			if (dc_creator_)dc_creator_->serialize	(CP_XML_STREAM());
			if (dc_date_)	dc_date_->serialize		(CP_XML_STREAM());
		}
	}
}
}
}
