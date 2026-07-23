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

#include "note.h"
#include "paragraph_elements.h"

#include <xml/xmlchar.h>


namespace cpdoccore { 
namespace odf_reader {
namespace text {

// text:note-citation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * note_citation::ns = L"text";
const wchar_t * note_citation::name = L"note-citation";

std::wostream & note_citation::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
    if (!text_label_.empty())
        _Wostream << text_label_;

 	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

void note_citation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:label", text_label_, std::wstring(L""));
}

void note_citation::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void note_citation::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}
void note_citation::add_space(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}
void note_citation::docx_convert(oox::docx_conversion_context & Context)
{
 	//for (size_t i = 0; i < content_.size(); i++)
  //  {
  //      content_[i]->docx_convert(Context);            
  //  }
}

// text:note-body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * note_body::ns = L"text";
const wchar_t * note_body::name = L"note-body";

std::wostream & note_body::text_to_stream(std::wostream & _Wostream, bool bXmlEncode) const
{
 	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->text_to_stream(_Wostream, bXmlEncode);
    }
    return _Wostream;
}

void note_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void note_body::add_child_element( xml::sax * Reader, const std::wstring & Ns, const std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void note_body::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}

void note_body::add_space(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}
void note_body::docx_convert(oox::docx_conversion_context & Context)
{
    oox::StreamsManPtr prev = Context.get_stream_man();
    boost::shared_ptr<std::wstringstream> newXml( new std::wstringstream() );
    Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(*newXml) ) );

    Context.set_process_headers_footers(true);
	Context.reset_context_state();

	//Context.start_comment_content(); // so that ins/del doesn't work
 	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);            
    }    
	//Context.end_comment_content();

	Context.set_process_headers_footers	(false);
	
	Context.back_context_state();

    const std::wstring & dbgStr = newXml->str();
    Context.get_notes_context().add(dbgStr, Context.get_notes_context().next_id() );

    Context.set_stream_man(prev);
}

}
}
}
