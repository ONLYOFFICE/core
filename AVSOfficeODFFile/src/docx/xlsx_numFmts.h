#ifndef _CPDOCCORE_XLSX_NUM_FMTS_H_INCLUDED_
#define _CPDOCCORE_XLSX_NUM_FMTS_H_INCLUDED_

#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

class xlsx_num_fmts
{
public:
    xlsx_num_fmts();
    ~xlsx_num_fmts();
public:
    size_t num_format_id(const std::wstring & format_code);
    void xlsx_serialize(std::wostream & _Wostream) const;
    friend void xlsx_serialize(std::wostream & _Wostream, const xlsx_num_fmts & numFmts);

private:
    struct Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

#endif
