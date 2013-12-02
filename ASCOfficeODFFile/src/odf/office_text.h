#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

/// \class  office_text
/// \brief  office:text
class office_text : public office_element_impl<office_text>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeText;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void docx_convert(oox::docx_conversion_context & Context) ;
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context) ;
    virtual void pptx_convert(oox::pptx_conversion_context & Context) ;

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    office_text();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    bool text_global_;
    // TODO: office-text-content-prelude:
    //  TODO: office-forms
    //  TODO: text-tracked-changes
    //  TODO: text-decls
    //  TODO: table-decls

    office_element_ptr_array text_content_;
    // TODO: text-page-sequence
    // TODO: office-text-content-epilogue:
    //  TODO: table-functions

};

CP_REGISTER_OFFICE_ELEMENT2(office_text);

}
}

