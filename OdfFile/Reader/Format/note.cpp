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
 	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);            
    }
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

	Context.start_comment_content(); // чтоб ins/del не отрабатывало
 	for (size_t i = 0; i < content_.size(); i++)
    {
        content_[i]->docx_convert(Context);            
    }    
	Context.end_comment_content();

	Context.set_process_headers_footers	(false);
	
	Context.back_context_state();

    const std::wstring & dbgStr = newXml->str();
    Context.get_notes_context().add(dbgStr, Context.get_notes_context().next_id() );

    Context.set_stream_man(prev);
}

}
}
}
