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
    // TODO - надо сделать так чтобы не только с 1
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
    // TODO - надо сделать так чтобы не только с 1
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
	//заголовок это не элемент списка

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

	//заголовок это не элемент списка

	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->pptx_convert(Context);
    }

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


}
}
}