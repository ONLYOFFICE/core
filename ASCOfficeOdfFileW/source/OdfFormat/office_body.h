#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

class office_body;
typedef shared_ptr<office_body>::Type office_body_ptr;

/// \brief  office:body
class office_body : public office_element_impl<office_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeBody;

    CPDOCCORE_DEFINE_VISITABLE()

    office_element * content() { return content_.get(); }

public:
    office_body();

	virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element( office_element_ptr & child_element);

	virtual void serialize(std::wostream & _Wostream);

private:
	
    office_element_ptr content_;
};

CP_REGISTER_OFFICE_ELEMENT2(office_body);

}
}
