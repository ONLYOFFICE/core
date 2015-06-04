#ifndef _CPDOCCORE_XLSX_CELL_STYLE_H_
#define _CPDOCCORE_XLSX_CELL_STYLE_H_

#include <iosfwd>
#include <string>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

namespace cpdoccore {
namespace oox {

    struct xlsx_cell_style
    {
        _CP_OPT(std::wstring) name;
        _CP_OPT(unsigned int) xfId;
        _CP_OPT(unsigned int) builtinId;

    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_cell_style & cell_style);

}
}

#endif
