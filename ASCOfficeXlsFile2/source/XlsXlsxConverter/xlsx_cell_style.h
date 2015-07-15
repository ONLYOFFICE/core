#pragma once

#include <iosfwd>
#include <string>

#include "common.h"

namespace oox {

    struct xlsx_cell_style
    {
        _CP_OPT(std::wstring) name;
        _CP_OPT(unsigned int) xfId;
        _CP_OPT(unsigned int) builtinId;

    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_cell_style & cell_style);

}

