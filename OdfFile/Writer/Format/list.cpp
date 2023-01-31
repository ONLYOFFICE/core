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

#include "list.h"

#include <xml/xmlchar.h>

#include <xml/attributes.h>
#include <xml/utils.h>
#include <xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf_writer {


using xml::xml_char_wc;

// text:number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_number::ns = L"text";
const wchar_t * text_number::name = L"number";

void text_number::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			CP_XML_STREAM() << string_;
		}
	}
}

void text_number::add_text(const std::wstring & Text)
{
    string_ = Text;
}

// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_item::ns = L"text";
const wchar_t * text_list_item::name = L"list-item";

void text_list_item::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (text_number_)text_number_->serialize(CP_XML_STREAM());

			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}


void text_list_item::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"number")
    {
        CP_CREATE_ELEMENT(text_number_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);        
    }
}
void text_list_item::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextNumber)
    {
        text_number_ = child_element;
    }
    else
    {
        content_.push_back( child_element);       
    }
}
// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * text_list_header::ns = L"text";
const wchar_t * text_list_header::name = L"list-header";


void text_list_header::serialize(std::wostream & _Wostream) 
{
 	CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (text_number_)text_number_->serialize(CP_XML_STREAM());
			
			for (size_t i = 0; i < content_.size(); i++)
			{
				content_[i]->serialize(CP_XML_STREAM());
			}
		}
	}
}

void text_list_header::create_child_element(const std::wstring & Ns, const std::wstring & Name)
{
    if CP_CHECK_NAME(L"text", L"number")
    {
        CP_CREATE_ELEMENT(text_number_);
    }
    else
    {
        CP_CREATE_ELEMENT(content_);        
    }
}
void text_list_header::add_child_element( const office_element_ptr & child_element)
{
	if (!child_element) return;

	ElementType type_ = child_element->get_type();

    if (type_ == typeTextNumber)
    {
        text_number_ = child_element;
    }
    else
    {
        content_.push_back( child_element);       
    }
}

}
}