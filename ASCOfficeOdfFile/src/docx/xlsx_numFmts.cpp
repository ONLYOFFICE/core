#include "../odf/precompiled_cpodf.h"
#include "xlsx_numFmts.h"

namespace cpdoccore {
namespace oox {

struct xlsx_num_fmts::Impl
{
    std::vector<std::wstring> formats_;

    static size_t transform_id(size_t id)
    {
        return id + 164;
    }

    size_t num_format_id(const std::wstring & format_code);
};

size_t xlsx_num_fmts::Impl::num_format_id(const std::wstring & format_code)
{
    for (size_t i = 0; i < formats_.size(); ++i)
    {
        if (formats_[i] == format_code)
            return transform_id(i);
    }

    formats_.push_back(format_code);
    return transform_id(formats_.size() - 1);            
}

xlsx_num_fmts::xlsx_num_fmts() : impl_(new xlsx_num_fmts::Impl())
{
}

xlsx_num_fmts::~xlsx_num_fmts()
{}

size_t xlsx_num_fmts::num_format_id(const std::wstring & format_code)
{
    return impl_->num_format_id(format_code);
}

void xlsx_num_fmts::xlsx_serialize(std::wostream & _Wostream) const
{
    _Wostream << L"<numFmts count=\"" << impl_->formats_.size() << L"\">";
    for (size_t i = 0; i < impl_->formats_.size(); ++i)
    {
        _Wostream << L"<numFmt formatCode=\"" << impl_->formats_[i] <<
            L"\" numFmtId=\"" << impl_->transform_id(i) << "\" />";
    }
        
    _Wostream << L"</numFmts>";
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_num_fmts & numFmts)
{
    return numFmts.xlsx_serialize(_Wostream);
}
   
}
}
