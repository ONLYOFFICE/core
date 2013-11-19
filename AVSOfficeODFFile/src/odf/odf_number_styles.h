#ifndef _CPDOCCORE_ODF_NUMBER_STYLES_H_
#define _CPDOCCORE_ODF_NUMBER_STYLES_H_

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore { 
namespace odf {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class number_styles
{
public:
    number_styles();
    ~number_styles();
public:
    void add(const std::wstring & style_name, office_element_ptr content);
    office_element_ptr find_by_style_name(const std::wstring & style_name);
    
private:
    struct Impl;
    _CP_SCOPED_PTR(Impl) impl_;

};

}
}

#endif
