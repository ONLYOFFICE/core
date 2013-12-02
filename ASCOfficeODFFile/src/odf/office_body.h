#ifndef _CPDOCCORE_ODF_OFFICE_BODY_H_
#define _CPDOCCORE_ODF_OFFICE_BODY_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

class office_body;
typedef shared_ptr<office_body>::Type office_body_ptr;

/// \class  office_body
/// \brief  office:body
class office_body : public office_element_impl<office_body>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeBody;

    CPDOCCORE_DEFINE_VISITABLE();

    office_element * content() { return content_.get(); }

    virtual void docx_convert(oox::docx_conversion_context & Context);
    virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

public:
    office_body();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

public:
    office_element_ptr content_;

};

CP_REGISTER_OFFICE_ELEMENT2(office_body);

}
}

#endif
