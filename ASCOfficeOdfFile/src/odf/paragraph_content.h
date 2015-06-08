#ifndef _CPDOCCORE_ODF_PARAGRAPH_CONTENT_H_
#define _CPDOCCORE_ODF_PARAGRAPH_CONTENT_H_

#include <iosfwd>
#include "office_elements.h"

namespace cpdoccore { 
namespace odf_reader {
namespace text {

template <class ElementT> 
class paragraph_content_impl : public office_element_impl<ElementT>
{
};

}
}
}

#endif
