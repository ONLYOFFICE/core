
#include "office_text.h"

#include <boost/foreach.hpp>

#include <cpdoccore/xml/xmlchar.h>

#include <cpdoccore/xml/attributes.h>

namespace cpdoccore { 
namespace odf {

using xml::xml_char_wc;

// office:text
//////////////////////////////////////////////////////////////////////////////////////////////////
const wchar_t * office_text::ns = L"office";
const wchar_t * office_text::name = L"text";


namespace { 
bool is_text_content(const std::wstring & ns, const std::wstring & name)
{
    if (ns == L"text")
    {
        return (
            name == L"h" ||
            name == L"p" ||
            name == L"list" ||
            name == L"numbered-paragraph" ||
            name == L"section" ||
            name == L"table-of-content" ||
            name == L"illustration-index" ||
            name == L"table-index" ||
            name == L"object-index" ||
            name == L"user-index" ||
            name == L"alphabetical-index" ||
            name == L"bibliography" ||

            // change-marks
            name == L"change" ||
            name == L"change-start" ||
            name == L"change-end"

            );
    
    }
    else if (ns == L"table")
    {
        return name == L"table";
    }
    else if (ns == L"draw" || ns == L"dr3d")
    {
        return true; // all shapes // 
    }

    return false;
}
}
void office_text_attlist::serialize(CP_ATTR_NODE)	
{
	CP_XML_ATTR_OPT(L"text:use-soft-page-breaks", text_use_soft_page_breaks_);
}
void office_text::create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name)
{
    if (is_text_content(Ns, Name))
    {
        CP_CREATE_ELEMENT(content_);
    }
    else
        CP_NOT_APPLICABLE_ELM();
}

void office_text::add_child_element( const office_element_ptr & child_element)
{
    content_.push_back(child_element);
}
void office_text::serialize(std::wostream & _Wostream)
{
    CP_XML_WRITER(_Wostream)
    {
		CP_XML_NODE_SIMPLE()
        {
			office_text_attlist_.serialize(CP_GET_XML_NODE());

			BOOST_FOREACH(office_element_ptr & elm, content_)
			{
				elm->serialize(CP_XML_STREAM());
			}
		}
	}
}
}
}
