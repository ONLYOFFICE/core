#pragma once

#include <boost/noncopyable.hpp>
#include <cpdoccore/CPScopedPtr.h>

namespace cpdoccore {
namespace oox {

class xlsx_merge_cells
{
public:
    xlsx_merge_cells();
    xlsx_merge_cells(const xlsx_merge_cells & m);
    const xlsx_merge_cells & operator=(const xlsx_merge_cells & m);
    ~xlsx_merge_cells();
public:
    void add_merge(size_t column, size_t row, size_t colsMerged, size_t rowsMerged);
    void xlsx_serialize(std::wostream & _Wostream);
private:
    class Impl;
    _CP_SCOPED_PTR(Impl) impl_;
};

}
}
