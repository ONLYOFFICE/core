#pragma once

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore { 
namespace odf {

class office_element;
typedef shared_ptr<office_element>::Type office_element_ptr;

class styles_lite_container
{
public:
    styles_lite_container();
    ~styles_lite_container();
public:
    void add(const std::wstring & style_name, office_element_ptr content);
    office_element_ptr find_by_style_name(const std::wstring & style_name);
    
private:
    struct Impl;
    _CP_SCOPED_PTR(Impl) impl_;

};

}
}
