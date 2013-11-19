#ifndef _CPDOCCORE_ODF_STYLE_MAP_H_
#define _CPDOCCORE_ODF_STYLE_MAP_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>
#include "office_elements.h"
#include "office_elements_create.h"
#include "style_ref.h"

namespace cpdoccore { 
namespace odf {

/// \class  style_map
/// \brief  style:map
///         style-map
class style_map : public office_element_impl<style_map>
{
public:
    static const wchar_t * ns;
    static const wchar_t * name;
    static const xml::NodeType xml_type = xml::typeElement;
    static const ElementType type = typeStyleMap;
    CPDOCCORE_DEFINE_VISITABLE();

private:
    virtual void add_attributes( const xml::attributes_wc_ptr & Attributes );
    virtual void add_child_element( xml::sax * Reader, const ::std::wstring & Ns, const ::std::wstring & Name);
    
public:
    std::wstring style_condition_;
    style_ref style_name_;
    std::wstring style_base_cell_address_;
    style_ref style_apply_style_name_;

};

CP_REGISTER_OFFICE_ELEMENT2(style_map);

}
}

#endif
