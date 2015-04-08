#include "precompiled_cpodf.h"
#include "office_scripts.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 
namespace odf {

// office:scripts
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_scripts::ns = L"office";
const wchar_t * office_scripts::name = L"scripts";

void office_scripts::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
}

void office_scripts::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if CP_CHECK_NAME(L"office", L"script")
        CP_CREATE_ELEMENT(office_script_);
    else
        CP_NOT_APPLICABLE_ELM();
}

void office_scripts::add_text(const std::wstring & Text)
{
}

// office:script
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_script::ns = L"office";
const wchar_t * office_script::name = L"script";

void office_script::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"script:language", script_language_, std::wstring(L""));
}

void office_script::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_CREATE_ELEMENT(content_);
}

void office_script::add_text(const std::wstring & Text)
{
}

}
}
