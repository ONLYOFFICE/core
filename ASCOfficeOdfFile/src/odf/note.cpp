
#include "note.h"

#include <boost/foreach.hpp>
#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "paragraph_elements.h"

namespace cpdoccore { 
namespace odf {
namespace text {



// text:note-citation
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * note_citation::ns = L"text";
const wchar_t * note_citation::name = L"note-citation";

::std::wostream & note_citation::text_to_stream(::std::wostream & _Wostream) const
{
    if (!text_label_.empty())
        _Wostream << text_label_;

    BOOST_FOREACH(const office_element_ptr & element, content_)
    {
        element->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void note_citation::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"text:label", text_label_, std::wstring(L""));
}

void note_citation::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void note_citation::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}

void note_citation::docx_convert(oox::docx_conversion_context & Context)
{
    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);            
    }
}

// text:note-body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * note_body::ns = L"text";
const wchar_t * note_body::name = L"note-body";

::std::wostream & note_body::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & element, content_)
    {
        element->text_to_stream(_Wostream);
    }
    return _Wostream;
}

void note_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void note_body::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void note_body::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    content_.push_back( elm );
}

void note_body::docx_convert(oox::docx_conversion_context & Context)
{
    oox::docx_conversion_context::StreamsManPtr prev = Context.get_stream_man();
    boost::shared_ptr<std::wstringstream> newXml( new std::wstringstream() );
    Context.set_stream_man( boost::shared_ptr<oox::streams_man>( new oox::streams_man(*newXml) ) );

    Context.set_process_headers_footers(true);
    bool runState = Context.get_run_state();
    Context.set_run_state(false);

	bool pState = Context.get_paragraph_state();
	Context.set_paragraph_state(false);

    BOOST_FOREACH(const office_element_ptr & elm, content_)
    {
        elm->docx_convert(Context);            
    }    
    Context.set_process_headers_footers(false);
    Context.set_run_state(runState);
	Context.set_paragraph_state(pState);

    const std::wstring & dbgStr = newXml->str();
    Context.get_notes_context().add(dbgStr, Context.get_notes_context().next_id() );

    Context.set_stream_man(prev);
}

}
}
}
