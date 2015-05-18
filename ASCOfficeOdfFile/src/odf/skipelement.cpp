
#include "skipelement.h"
#include "logging.h"
#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf {

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
    std::wstringstream ss;
    ss << L"[warning] : skip element " << Ns << L":" << Name << "\n";
    _CP_LOG(error) << ss.str();
    
    if (Reader)
        skip_element_.read_sax(Reader);        
}

void _skip_element::add_text(const std::wstring & Text)
{
    std::wstringstream ss;
    ss << L"[warning] : skip element text\n";
    _CP_LOG(error) << ss.str();
}

}
}
