#ifndef _CPDOCCORE_XLSX_NUM_FORMAT_CONTEXT_H_
#define _CPDOCCORE_XLSX_NUM_FORMAT_CONTEXT_H_

#include <string>
#include <iosfwd>
#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

class num_format_context : boost::noncopyable
{
public:
    num_format_context();
    ~num_format_context();

public:
    void start_format(const std::wstring & style_name);
    std::wostream & output();
    void end_format();

    std::wstring get_last_format() const;

    void start_complex_format();
    void add_format(std::wstring const & cond, std::wstring const & format);
    void end_complex_format();

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;

    class Impl2;
    _CP_SCOPED_PTR(Impl2) impl2_;
};

}
}

#endif
