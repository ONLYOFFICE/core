#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements_create.h"

#include <cpdoccore/xml/simple_xml_writer.h>

#include "bool.h"

namespace cpdoccore { 
namespace odf_writer {

//office_text_attlist
class office_text_attlist
{
public:
    _CP_OPT(odf_types::Bool)			text_use_soft_page_breaks_;

	void serialize(CP_ATTR_NODE);
};

/// \brief  office:text
class office_text : public office_element_impl<office_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeText;
    CPDOCCORE_DEFINE_VISITABLE()

	virtual void create_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( const office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);

//    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
//    virtual void add_child_element(const  xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
//    virtual void add_text(const std::wstring & Text);

	office_text_attlist office_text_attlist_;

private:
    bool text_global_;
    // TODO: office-text-content-prelude:
    //  TODO: office-forms
    //  TODO: text-tracked-changes
    //  TODO: text-decls
    //  TODO: table-decls

    office_element_ptr_array content_;
    // TODO: text-page-sequence
    // TODO: office-text-content-epilogue:
    //  TODO: table-functions

};

CP_REGISTER_OFFICE_ELEMENT2(office_text)

}
}

