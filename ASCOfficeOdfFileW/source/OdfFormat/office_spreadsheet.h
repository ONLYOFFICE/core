#pragma once

#include <iosfwd>

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore { 
namespace odf {


/// \brief  office:spreadsheet
class office_spreadsheet : public office_element_impl<office_spreadsheet>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeSpreadsheet;
    CPDOCCORE_DEFINE_VISITABLE();

	office_element_ptr_array & getContent(){return content_;}


	virtual void add_child_element(const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_child_element( office_element_ptr & child_element);

    virtual void serialize(std::wostream & _Wostream);
private:    

    //  TODO: office-text-content-prelude:
    //  TODO: office-forms
    //  TODO: text-tracked-changes
    //  TODO: text-decls
    //  TODO: table-decls

    office_element_ptr_array content_;
    // TODO: text-page-sequence
    // TODO: office-text-content-epilogue:
    //  TODO: table-functions

};

CP_REGISTER_OFFICE_ELEMENT2(office_spreadsheet);

}
}
