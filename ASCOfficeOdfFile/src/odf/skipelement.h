#ifndef _CPDOCCORE_ODF_OFFCIE_SKIP_ELEMENT_
#define _CPDOCCORE_ODF_OFFCIE_SKIP_ELEMENT_

#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"


namespace cpdoccore { 
namespace odf_reader {

/// \class  _skip_element
class _skip_element : public office_element_impl<_skip_element>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = type_AbstractXml;

    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

public:
    static _skip_element skip_element_;

};

}
}

#endif
