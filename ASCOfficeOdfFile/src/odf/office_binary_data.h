#ifndef _CPDOCCORE_ODF_OFFICEBINARYDATA_H_
#define _CPDOCCORE_ODF_OFFICEBINARYDATA_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"

namespace cpdoccore { 
namespace odf {

/// \class  office_binary_data
/// \brief  office:binary-data
class office_binary_data : public office_element_impl<office_binary_data>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeOfficeBinaryData;

    CPDOCCORE_DEFINE_VISITABLE();

public:
    virtual ::std::wostream & text_to_stream(::std::wostream & _Wostream) const;

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    virtual void add_text(const std::wstring & Text);

private:
    std::wstring base64Binary_;
    
};

CP_REGISTER_OFFICE_ELEMENT2(office_binary_data);

}
}

#endif
