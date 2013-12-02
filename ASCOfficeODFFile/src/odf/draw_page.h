#pragma once

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "style_ref.h"

namespace cpdoccore { 
namespace odf {


class draw_page_attr
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
	_CP_OPT(std::wstring)	draw_name_;
	_CP_OPT(std::wstring)		draw_style_name_;
	_CP_OPT(std::wstring)		page_layout_name_;
	_CP_OPT(std::wstring)		master_page_name_;
};
/// \class  draw_page
/// \brief  draw:page
class draw_page : public office_element_impl<draw_page>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeDrawPage;
    CPDOCCORE_DEFINE_VISITABLE();

    virtual void pptx_convert(oox::pptx_conversion_context & Context);

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);

private:
    office_element_ptr_array content_;
	draw_page_attr draw_page_attr_;
};

CP_REGISTER_OFFICE_ELEMENT2(draw_page);

}
}
