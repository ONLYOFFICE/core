#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include "style_ref.h"

namespace cpdoccore { 
namespace odf {


/// \brief  style:map
class style_map : public office_element_impl<style_map>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleMap;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void create_child_element( const ::std::wstring & Ns, const ::std::wstring & Name);
	virtual void add_child_element(office_element_ptr & child)
		{std::wstringstream str; str <<L"Non add child in "; str << ns; str << L":"; str <<name; _CP_LOG(error) << str.str();}
	virtual void serialize(std::wostream & strm){}

	std::wstring style_condition_;
    style_ref style_name_;
    std::wstring style_base_cell_address_;
    style_ref style_apply_style_name_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_map);

}
}
