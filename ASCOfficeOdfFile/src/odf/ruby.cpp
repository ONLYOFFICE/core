#include "precompiled_cpodf.h"

#include "ruby.h"

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/xmlchar.h>

#include <boost/foreach.hpp>

#include "paragraph_elements.h"
#include "serialize_elements.h"

namespace cpdoccore {
namespace odf {
namespace text {

using xml::xml_char_wc;

// text:ruby-base
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * ruby_base::ns = L"text";
const wchar_t * ruby_base::name = L"ruby-base";

::std::wostream & ruby_base::text_to_stream(::std::wostream & _Wostream) const
{
    serialize_elements_text(_Wostream, paragraph_content_);
    return _Wostream;
}

void ruby_base::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void ruby_base::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(paragraph_content_);
}

void ruby_base::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::create(Text) ;
    paragraph_content_.push_back( elm );
}

// text:ruby-text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * ruby_text::ns = L"text";
const wchar_t * ruby_text::name = L"ruby-text";

::std::wostream & ruby_text::text_to_stream(::std::wostream & _Wostream) const
{
    CP_SERIALIZE_TEXT(content_);
    return _Wostream;
}

void ruby_text::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    text_style_name_ = style_ref( Attributes->get_val< ::std::wstring >(L"text:style-name").get_value_or(L"") );
}

void ruby_text::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
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
