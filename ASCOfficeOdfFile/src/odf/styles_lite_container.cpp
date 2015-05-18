
#include <vector>
#include <boost/unordered_map.hpp>

#include "styles_lite_container.h"

namespace cpdoccore { 
namespace odf {

struct style_ref_lite_container
{
    std::wstring style_name;
    office_element_ptr style;
};

struct styles_lite_container::Impl
{
    typedef std::vector<style_ref_lite_container> array_t;
    array_t array_;
    typedef boost::unordered_map<std::wstring, size_t> map_t;
    map_t map_;
};

styles_lite_container::styles_lite_container(): impl_(new styles_lite_container::Impl() )
{
}

styles_lite_container::~styles_lite_container()
{
}

void styles_lite_container::add(const std::wstring & style_name, office_element_ptr content)
{
    style_ref_lite_container ref = {style_name, content};
    impl_->array_.push_back(ref);
    impl_->map_[style_name] = impl_->array_.size() - 1;    
}

office_element_ptr styles_lite_container::find_by_style_name(const std::wstring & style_name)
{
    styles_lite_container::Impl::map_t::const_iterator i = impl_->map_.find(style_name);
    if (i != impl_->map_.end())
    {
        return impl_->array_[i->second].style;
    }
    else
    {
        return office_element_ptr();
    }
}

}
}