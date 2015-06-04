#pragma once

#include <iosfwd>
#include <string>

#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

#include "xlsx_alignment.h"
#include "xlsx_protection.h"

namespace cpdoccore {
namespace oox {

    struct xlsx_xf;
    typedef _CP_PTR(xlsx_xf) xlsx_xf_ptr;

    struct xlsx_xf
    {
        _CP_OPT(bool) applyAlignment;
        _CP_OPT(bool) applyBorder;
        _CP_OPT(bool) applyFill;
        _CP_OPT(bool) applyFont;
        _CP_OPT(bool) applyNumberForm;
        _CP_OPT(bool) applyProtection;
        _CP_OPT(unsigned int) borderId;
        _CP_OPT(unsigned int) fillId;
        _CP_OPT(unsigned int) fontId;
        _CP_OPT(unsigned int) numFmtId;
        _CP_OPT(bool) pivotButton;
        _CP_OPT(bool) quotePrefix;
        _CP_OPT(unsigned int) xfId;

        _CP_OPT(xlsx_alignment) alignment;
        _CP_OPT(xlsx_protection) protection;

        mutable std::size_t index;

        bool operator == (const xlsx_xf & rVal) const;
        bool operator != (const xlsx_xf & rVal) const;
        friend std::size_t hash_value(xlsx_xf const & val);
    };

    void xlsx_serialize(std::wostream & _Wostream, const xlsx_xf & xf);

}
}
