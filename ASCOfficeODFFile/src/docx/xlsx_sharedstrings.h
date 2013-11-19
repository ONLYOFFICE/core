#ifndef _CPDOCCORE_XLSX_SHARED_STRINGS_H_INCLUDED_
#define _CPDOCCORE_XLSX_SHARED_STRINGS_H_INCLUDED_

#include <string>
#include <iosfwd>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

class xlsx_shared_strings: boost::noncopyable
{
public:
    xlsx_shared_strings();
    ~xlsx_shared_strings();

public:
    void xlsx_serialize(std::wostream & _Wostream) const;
    size_t add(const std::wstring & content);

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

#endif
