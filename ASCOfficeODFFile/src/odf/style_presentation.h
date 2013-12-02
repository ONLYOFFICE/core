#pragma once

#include <cpdoccore/xml/attributes.h>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "office_elements_create.h"

#include "length.h"
#include "presentationclass.h"

namespace cpdoccore { 
namespace odf {

/// \class  presentation_placeholder 
///         presentation:placeholder 
class presentation_placeholder : public office_element_impl<presentation_placeholder>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStylePresentationPlaceholder;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void pptx_convert(oox::pptx_conversion_context & Context);
 
public:
	_CP_OPT(length) svg_x_;
    _CP_OPT(length) svg_y_;
    _CP_OPT(length) svg_width_;
    _CP_OPT(length) svg_height_;

	_CP_OPT(presentation_class) presentation_object_;

};

CP_REGISTER_OFFICE_ELEMENT2(presentation_placeholder);

}
}
