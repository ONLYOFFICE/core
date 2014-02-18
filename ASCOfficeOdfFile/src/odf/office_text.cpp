#include "precompiled_cpodf.h"
#include "office_text.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

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
