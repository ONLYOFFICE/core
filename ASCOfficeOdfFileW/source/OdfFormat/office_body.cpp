#include "precompiled_cpodf.h"
#include "office_body.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/xml/simple_xml_writer.h>

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// office:body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_body::ns = L"office";
const wchar_t * office_body::name = L"body";

office_body::office_body()
{}

void office_body::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}


void office_body::add_child_element( office_element_ptr & child_element)
{
	content_ = child_element;
}


void office_body::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			if (content_)content_->serialize(CP_XML_STREAM());
		}
	}
}


}
}
