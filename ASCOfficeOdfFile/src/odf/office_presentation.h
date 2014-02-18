#pragma once

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

/// \brief  office:presentation
class office_presentation : public office_element_impl<office_presentation>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficePresentation;
    CPDOCCORE_DEFINE_VISITABLE();

	virtual void docx_convert(oox::docx_conversion_context & Context);
	virtual void xlsx_convert(oox::xlsx_conversion_context & Context);
	virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

public:

    office_element_ptr_array date_time_decls_;
    office_element_ptr_array footer_decls_;
  
	office_element_ptr_array pages_;

};

CP_REGISTER_OFFICE_ELEMENT2(office_presentation);

}
}
