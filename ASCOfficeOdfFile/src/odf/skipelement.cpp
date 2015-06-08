
#include "skipelement.h"
#include "logging.h"
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf_reader {

_skip_element _skip_element::skip_element_;

const wchar_t * _skip_element::ns = L"";
const wchar_t * _skip_element::name = L"";

void _skip_element::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    Attributes->reset_check();
    return;
}

void _skip_element::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    _CP_LOG << L"[error] : skip element " << Ns << L":" << Name << "\n";
    
    if (Reader)
        skip_element_.read_sax(Reader);        
}

void _skip_element::add_text(const std::wstring & Text)
{
     _CP_LOG << L"[error] : skip element text\n";
}

}
}
