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

#include "list.h"

#include <xml/xmlchar.h>
#include <xml/utils.h>

namespace cpdoccore { 
namespace odf_reader {
namespace text {



// text:number
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * number::ns = L"text";
const wchar_t * number::name = L"number";

std::wostream & number::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	_Wostream << (bXmlEncode ? xml::utils::replace_xml_to_text( string_ ) : string_);
    return _Wostream;
}

void number::add_text(const std::wstring & Text)
{
    string_ = Text;
}

// text:list-item
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * list_item::ns = L"text";
const wchar_t * list_item::name = L"list-item";

std::wostream & list_item::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

void list_item::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:start-value", text_start_value_);
}

void list_item::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
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

void list_item::docx_convert(oox::docx_conversion_context & Context)
{
    bool restart = false;
    // TODO - need to make it work not only from 1
    if (text_start_value_)
    {
        restart = true;                    
    }

    Context.start_list_item(restart);

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }

    Context.end_list_item();

}
void list_item::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool restart = false;
    // TODO - need to make it work not only from 1
    if (text_start_value_)
    {
        restart = true;                    
    }

    Context.get_text_context().start_list_item(restart);

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }

    Context.get_text_context().end_list_item();

}
// text:list-header
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * list_header::ns = L"text";
const wchar_t * list_header::name = L"list-header";

void list_header::docx_convert(oox::docx_conversion_context & Context)
{
	//header is not a list item

	std::wstring s = Context.current_list_style();
	Context.end_list();

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }

	Context.start_list(s, true);
}
void list_header::pptx_convert(oox::pptx_conversion_context & Context)
{
    bool restart = false;

	//header is not a list item

	Context.get_text_context().set_header(true);

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }

	Context.get_text_context().set_header(false);
}
std::wostream & list_header::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
 	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

void list_header::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void list_header::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
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
//-------------------------------------------------------------------------------------------------------------
const wchar_t* ordered_list::ns = L"text";
const wchar_t* ordered_list::name = L"ordered-list";

void ordered_list::add_attributes(const xml::attributes_wc_ptr& Attributes)
{
    CP_APPLY_ATTR(L"style:name", style_name_);
}
void ordered_list::add_child_element(xml::sax* Reader, const std::wstring& Ns, const std::wstring& Name)
{
    CP_CREATE_ELEMENT(content_);
}
std::wostream& ordered_list::text_to_stream(std::wostream& _Wostream, bool bXmlEncode) const
{
    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}
void ordered_list::docx_convert(oox::docx_conversion_context& Context)
{
    Context.start_list(style_name_.get_value_or(L""));

    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);
    }
    Context.end_list();
}
void ordered_list::pptx_convert(oox::pptx_conversion_context& Context)
{
    Context.get_text_context().start_list(style_name_.get_value_or(L""));
    for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }
    Context.get_text_context().end_list();
}
}
}
}