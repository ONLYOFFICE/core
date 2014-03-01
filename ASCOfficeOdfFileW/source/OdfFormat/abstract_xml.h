#pragma once

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

class abstract_xml : public office_element_impl<abstract_xml>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = type_AbstractXml;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    abstract_xml() {};

private:
	virtual void add_child_element( const std::wstring & Ns, const std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & strm);
    
private:
    //office_element_ptr_array xml_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(abstract_xml);

}
}
