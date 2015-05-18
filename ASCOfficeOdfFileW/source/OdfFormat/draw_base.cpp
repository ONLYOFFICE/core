
#include "draw_base.h"

#include <ostream>
#include <sstream>
#include <string>

#include <boost/foreach.hpp>
#include <boost_string.h>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>


#include "odf_conversion_context.h"

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;


const wchar_t * draw_base::ns = L"draw";
const wchar_t * draw_base::name = L"base";
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void draw_base::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
	CP_CREATE_ELEMENT(content_);
}

void draw_base::add_child_element( office_element_ptr & child_element)
{
	content_.push_back(child_element);
}
void draw_base::serialize(std::wostream & _Wostream)
{
	BOOST_FOREACH(const office_element_ptr & elm, content_)
	{
		elm->serialize(_Wostream);
	}
}
void draw_base::serialize_attlist(CP_ATTR_NODE)
{
	common_draw_attlists_.serialize(CP_GET_XML_NODE());
	common_presentation_attlist_.serialize(CP_GET_XML_NODE()); 	
}

}
}