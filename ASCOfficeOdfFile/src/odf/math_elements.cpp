
#include "math_elements.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>
#include <cpdoccore/xml/attributes.h>


namespace cpdoccore { 

	using namespace odf_types;

namespace odf_reader {

// math:math
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * math_math::ns = L"math";
const wchar_t * math_math::name = L"math";

void math_math::add_attributes( const xml::attributes_wc_ptr & Attributes )
{

}

void math_math::add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_NOT_APPLICABLE_ELM();
}


void math_math::docx_convert(oox::docx_conversion_context & Context) 
{

}

void math_math::xlsx_convert(oox::xlsx_conversion_context & Context)
{

}
void math_math::pptx_convert(oox::pptx_conversion_context & Context)
{

}


}
}
