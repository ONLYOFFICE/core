#ifndef CPDOCCORE_XLSX_HYPERLINKS_H_
#define CPDOCCORE_XLSX_HYPERLINKS_H_

#include <iosfwd>
#include <cpdoccore/CPOptional.h>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPNoncopyable.h>

namespace cpdoccore {
namespace oox {

class rels;

class xlsx_hyperlinks
{
public:
    xlsx_hyperlinks();
    ~xlsx_hyperlinks();

    std::wstring add(std::wstring const & ref, std::wstring const & target, std::wstring const & display);

    void xlsx_serialize(std::wostream & _Wostream) const;

	void dump_rels(rels & Rels) const;

private:
    class Impl;
    _CP_PTR(Impl) impl_;
    
};

}
}

#endif
