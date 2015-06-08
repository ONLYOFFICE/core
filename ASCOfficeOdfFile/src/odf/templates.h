#pragma once

#include <string>
#include <vector>
#include <iosfwd>

#include <cpdoccore/CPNoncopyable.h>
#include <cpdoccore/xml/attributes.h>

#include <cpdoccore/CPSharedPtr.h>

#include <cpdoccore/CPOptional.h>

#include "styles_list.h"
#include "style_map.h"


namespace cpdoccore { 
namespace odf_reader { 
	 
class table_body: public office_element_impl<table_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableBodyTemplate;

    CPDOCCORE_DEFINE_VISITABLE();

	
 	std::wstring		table_style_name_;

private:

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
	virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name){}

    friend class odf_document;

};
CP_REGISTER_OFFICE_ELEMENT2(table_body);


class table_table_template: public office_element_impl<table_table_template>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeTableTemplate;

    CPDOCCORE_DEFINE_VISITABLE();

	std::wstring get_text_style_name(){return text_style_name_.get_value_or(L"");}
	
	office_element_ptr			table_body_;

private:
 	_CP_OPT(std::wstring)		text_style_name_;

    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

    friend class odf_document;

};
CP_REGISTER_OFFICE_ELEMENT2(table_table_template);

}
}