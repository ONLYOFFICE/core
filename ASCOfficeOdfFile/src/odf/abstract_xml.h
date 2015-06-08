#ifndef _CPDOCCORE_ODF_ABSTRACT_XML_H_
#define _CPDOCCORE_ODF_ABSTRACT_XML_H_

#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf_reader {

class abstract_xml : public office_element_impl<abstract_xml>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = type_AbstractXml;
    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;
    virtual ::std::wostream & xml_to_stream(::std::wostream & _Wostream) const;

public:
    abstract_xml() {};

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    office_element_ptr_array xml_content_;
};

CP_REGISTER_OFFICE_ELEMENT2(abstract_xml);

}
}

#endif // #ifndef _CPDOCCORE_ODF_ABSTRACT_XML_H_
