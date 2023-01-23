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
