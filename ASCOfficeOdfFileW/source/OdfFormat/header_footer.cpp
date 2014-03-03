#include "precompiled_cpodf.h"
#include "header_footer.h"
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/serialize.h>


namespace cpdoccore { 
namespace odf {


/// style-header-footer-properties
const wchar_t * style_header_footer_properties::ns = L"style";
const wchar_t * style_header_footer_properties::name = L"header-footer-properties";


void style_header_footer_properties::add_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (L"style" == Ns && L"background-image" == Name)
    {
        CP_CREATE_ELEMENT(style_background_image_);
    }
    else
    {
        not_applicable_element(L"style-header-footer-properties",  Ns, Name);
    }    
}



void header_footer_content::add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name, odf_conversion_context * Context)
{
    CP_CREATE_ELEMENT_SIMPLE(content_);
}

}
}
