#ifndef CPDOCCORE_ODF_CONTENT_XML_H_
#define CPDOCCORE_ODF_CONTENT_XML_H_

#include <cpdoccore/common/readdocelement.h>
#include "office_elements.h"
#include "documentcontext.h"

namespace cpdoccore { 
namespace odf_reader {

class content_xml_t : public common::read_doc_element
{
public:
    content_xml_t()
    {}

public:
    office_element * get_content()
    {
        return xml_content_.get();
    }

    const office_element * get_content() const
    {
        return xml_content_.get();
    }

public:
    void docx_convert(oox::docx_conversion_context & Context);
    void xlsx_convert(oox::xlsx_conversion_context & Context);
    void pptx_convert(oox::pptx_conversion_context & Context);

    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_text(const std::wstring & Text);
    document_context * getContext();

private:
    office_element_ptr xml_content_;
    friend class odf_document;
    document_context context_;
};

}
}

#endif
