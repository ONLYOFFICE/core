/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf_reader {



// office:text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_text::ns = L"office";
const wchar_t * office_text::name = L"text";

::std::wostream & office_text::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(text_content_);
    return _Wostream;
}

office_text::office_text()
{}

void office_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:global", text_global_, false);
}

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

            // change-marks
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

void office_text::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (is_text_content(Ns, Name))
    {
        CP_CREATE_ELEMENT(text_content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

void office_text::add_text(const std::wstring & Text)
{
    // TODO : error
}

void office_text::docx_convert(oox::docx_conversion_context & Context)
{
    Context.start_office_text();
    BOOST_FOREACH(const office_element_ptr & elm, text_content_)
    {
        elm->docx_convert(Context);
    }
    Context.end_office_text();
}
void office_text::xlsx_convert(oox::xlsx_conversion_context & Context)
{
    //Context.start_office_text();
    BOOST_FOREACH(const office_element_ptr & elm, text_content_)
    {
        elm->xlsx_convert(Context);
    }
    //Context.end_office_text();
}
void office_text::pptx_convert(oox::pptx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, text_content_)
    {
        elm->pptx_convert(Context);
    }
}
}
}
