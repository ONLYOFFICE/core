#pragma once

#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

class xlsx_cell_styles
{
public:
    xlsx_cell_styles();
    ~xlsx_cell_styles();

public:
    void serialize(std::wostream & _Wostream) const;

private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}