
#include "style_map.h"

#include <boost/foreach.hpp>

//#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>

#include "serialize_elements.h"

namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// style:map
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_map::ns = L"style";
const wchar_t * style_map::name = L"map";

void style_map::add_attributes( const xml::attributes_wc_ptr & Attributes )
{
    CP_APPLY_ATTR(L"style:condition", style_condition_, std::wstring(L""));
    CP_APPLY_ATTR(L"style:name", style_name_, style_ref(L""));
    CP_APPLY_ATTR(L"style:base-cell-address", style_base_cell_address_, std::wstring(L""));
    CP_APPLY_ATTR(L"style:apply-style-name", style_apply_style_name_, style_ref(L""));
}

void style_map::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

}
}
