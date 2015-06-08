
#include "style_map.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_writer {

// style:map
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * style_map::ns = L"style";
const wchar_t * style_map::name = L"map";

void style_map::create_child_element(  const ::std::wstring & Ns, const ::std::wstring & Name)
{
    CP_NOT_APPLICABLE_ELM();
}

}
}
