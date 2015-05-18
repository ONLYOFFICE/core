

#include "abstract_xml.h"

#include "paragraph_elements.h"

#include <boost/foreach.hpp>

namespace cpdoccore { 
namespace odf {


// abstract-xml
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * abstract_xml::ns = L"";
const wchar_t * abstract_xml::name = L"abstract-xml";

::std::wostream & abstract_xml::text_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & element, xml_content_)
    {
        element->text_to_stream(_Wostream);
    }
    return _Wostream;
}

::std::wostream & abstract_xml::xml_to_stream(::std::wostream & _Wostream) const
{
    BOOST_FOREACH(const office_element_ptr & element, xml_content_)
    {
        element->xml_to_stream(_Wostream);
    }
    return _Wostream;
}

void abstract_xml::add_attributes( const xml::attributes_wc_ptr & )
{
}

void abstract_xml::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(xml_content_);
}

void abstract_xml::add_text(const std::wstring & Text)
{
    office_element_ptr elm = text::text::create(Text) ;
    xml_content_.push_back( elm );
}

}
}
