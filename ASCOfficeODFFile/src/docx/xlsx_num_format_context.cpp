#include "precompiled_cpodf.h"
#include "xlsx_num_format_context.h"
#include "xlsx_complex_number_format.h"
#include <sstream>
#include <map>

namespace cpdoccore {
namespace oox {


class num_format_context::Impl2
{
public:
    complex_number_format complex_number_format_;
};

class num_format_context::Impl
{
public:
    Impl(): in_convert_style_(false)
    {}

public:
    std::wstring current_style_name_;
    std::wstringstream stream_;

    void reset_current()
    {
        current_style_name_ = L"";
        stream_.str(std::wstring());  
        in_convert_style_ = false;
    }

    std::wstring last_format_;

    bool in_convert_style_;
};

num_format_context::num_format_context(): impl_(new Impl()), impl2_(new Impl2())
{
}

num_format_context::~num_format_context()
{
}

void num_format_context::start_format(const std::wstring & style_name)
{
    impl_->reset_current();
    impl_->current_style_name_ = style_name;
    impl_->in_convert_style_ = true;
}

std::wostream & num_format_context::output()
{
    return impl_->stream_;
}

void num_format_context::end_format()
{    
    impl_->last_format_ = impl_->stream_.str();
    impl_->reset_current();
}

std::wstring num_format_context::get_last_format() const
{
    return impl_->last_format_;
}

void num_format_context::start_complex_format()
{
    impl2_->complex_number_format_.reset();
}

void num_format_context::add_format(std::wstring const & cond, std::wstring const & format)
{
    impl2_->complex_number_format_.add_format(cond, format);
}

void num_format_context::end_complex_format()
{
    impl_->last_format_ = impl2_->complex_number_format_.result_value();
}

}
}
