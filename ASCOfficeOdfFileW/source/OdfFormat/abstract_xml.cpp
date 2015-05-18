

#include "abstract_xml.h"

//#include "paragraph_elements.h"

#include <boost/foreach.hpp>

namespace cpdoccore { 
namespace odf {


// abstract-xml
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * abstract_xml::ns = L"";
const wchar_t * abstract_xml::name = L"abstract-xml";

void abstract_xml::serialize(std::wostream & strm)
{
}

//void abstract_xml::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
//{
//    CP_CREATE_ELEMENT(xml_content_);
//}
//
//void abstract_xml::add_child_element( office_element_ptr & child_element)
//{
//    xml_content_.push_back(child_element);
//}


}
}
