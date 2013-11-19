#include "precompiled_cpodf.h"
#include "odf_number_styles.h"
#include <vector>
#include <boost/unordered_map.hpp>

namespace cpdoccore { 
namespace odf {

struct number_style_ref
{
    std::wstring style_name;
    office_element_ptr style;
};

struct number_styles::Impl
{
    typedef std::vector<number_style_ref> array_t;
    array_t array_;
    typedef boost::unordered_map<std::wstring, size_t> map_t;
    map_t map_;
};

number_styles::number_styles(): impl_(new number_styles::Impl() )
{
}

number_styles::~number_styles()
{
}

void number_styles::add(const std::wstring & style_name, office_element_ptr content)
{
    number_style_ref ref = {style_name, content};
    impl_->array_.push_back(ref);
    impl_->map_[style_name] = impl_->array_.size() - 1;    
}

office_element_ptr number_styles::find_by_style_name(const std::wstring & style_name)
{
    number_styles::Impl::map_t::const_iterator i = impl_->map_.find(style_name);
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