#pragma once

#include <iosfwd>

#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPNoncopyable.h>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPWeakPtr.h>
#include <cpdoccore/xml/xmlelement.h>
#include <cpdoccore/xml/nodetype.h>

#include "office_elements.h"
#include "datatypes/style_ref.h"

namespace cpdoccore { 
namespace xml {

class attributes;

typedef attributes attributes_wc;

typedef shared_ptr< attributes_wc >::Type attributes_wc_ptr;

} // namespace xml

namespace odf_reader {

namespace text {

class paragraph_attrs
{
public:
    void add_attributes( const xml::attributes_wc_ptr & Attributes );

public:
    odf_types::style_ref		text_style_name_;
    odf_types::style_ref_array	text_class_names_;
    odf_types::style_ref		text_cond_style_name_;

};


template <class ElementT> 
class text_content_impl : public office_element_impl<ElementT>
{
};


}
}
}
