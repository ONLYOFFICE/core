
#include "xlsx_complex_number_format.h"

namespace cpdoccore {
namespace oox {

class complex_number_format::Impl
{
public:
    std::wstring result_value()
    {
        std::wstring positive = (!positive_.empty() ? positive_ : default_);
        std::wstring negative = (!negative_.empty() ? negative_ : default_);
        std::wstring zero = (!zero_.empty() ? zero_ : default_);

        if (zero == positive && zero == negative)
        {
            return positive;
        }

        return positive + L";" + negative + (!zero.empty() ? (L";" + zero) : std::wstring(L""));        
    }

    void add_format(std::wstring const & condition, std::wstring const & format)
    {
        std::wstring c = condition;
        boost::algorithm::to_lower(c);
        boost::algorithm::replace_all(c, L" ", L"");
        boost::algorithm::replace_all(c, L"\t", L"");

        if (L"" == c)
        {
            default_ = format;
        }
        else if (L"value()>0" == c)
        {
            positive_ = format;               
        } 
        else if (L"value()>=0" == c)
        {
            positive_ = format;
            zero_ = format;
        }
        else if (L"value()<=0" == c)
        {
            negative_ = format;
            zero_ = format;
        }
        else if (L"value()<0" == c)
        {
            negative_ = format;
        }
        else if (L"value()=0" == c)
        {
            zero_ = format;
        }
        else 
        {
            default_ = format; // unknown
        }
    }

private:
    std::wstring default_;
    std::wstring positive_;
    std::wstring negative_;
    std::wstring zero_;
};

complex_number_format::complex_number_format() : impl_(new Impl())
{}

complex_number_format::~complex_number_format()
{
}

void complex_number_format::reset()
{
    impl_.reset(new Impl());
}

std::wstring complex_number_format::result_value()
{
    return impl_->result_value();
}

void complex_number_format::add_format(std::wstring const & condition, std::wstring const & format)
{
    return impl_->add_format(condition, format);
}

}
}
