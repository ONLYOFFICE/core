
#include "office_binary_data.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf_reader {



// office:binary-data
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_binary_data::ns = L"office";
const wchar_t * office_binary_data::name = L"binary-data";

::std::wostream & office_binary_data::text_to_stream(::std::wostream & _Wostream) const
{
    return _Wostream;
}

void office_binary_data::add_attributes( const xml::attributes_wc_ptr & Attributes )
{    
}

void office_binary_data::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
}

void office_binary_data::add_text(const std::wstring & Text)
{
    base64Binary_ = Text;
}

}
}
