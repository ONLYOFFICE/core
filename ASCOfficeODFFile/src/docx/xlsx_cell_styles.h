#ifndef _CPDOCCORE_XLSX_CELL_STYLES_
#define _CPDOCCORE_XLSX_CELL_STYLES_

#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

class xlsx_cell_styles
{
public:
    xlsx_cell_styles();
    ~xlsx_cell_styles();

public:
    void xlsx_serialize(std::wostream & _Wostream) const;

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}

#endif
