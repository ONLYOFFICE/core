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


#include "ruby.h"

#include "paragraph_elements.h"
#include "serialize_elements.h"

namespace cpdoccore {
namespace odf_reader {
namespace text {



// text:ruby-base
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * ruby_base::ns = L"text";
const wchar_t * ruby_base::name = L"ruby-base";

std::wostream & ruby_base::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    serialize_elements_text(_Wostream, content_, bXmlEncode);
    return _Wostream;
}

void ruby_base::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void ruby_base::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void ruby_base::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}

// text:ruby-text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * ruby_text::ns = L"text";
const wchar_t * ruby_text::name = L"ruby-text";

std::wostream & ruby_text::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    CP_SERIALIZE_TEXT(content_, bXmlEncode);
    return _Wostream;
}

void ruby_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
	text_style_name_ =  Attributes->get_val< std::wstring >(L"text:style-name").get_value_or(L"");
}

void ruby_text::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void ruby_text::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}



}
}
}
