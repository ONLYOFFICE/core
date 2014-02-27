#include "precompiled_cpodf.h"
#include "office_body.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/serialize.h>
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// office:body
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_body::ns = L"office";
const wchar_t * office_body::name = L"body";

office_body::office_body()
{}

void office_body::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    
}

void office_body::add_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
	create_element(Ns,Name,content_,getContext());
}


void office_body::add_child_element( office_element_ptr & child_element)
{
	content_ = child_element;
}


void office_body::serialize(std::wostream & _Wostream)
{

}


}
}
