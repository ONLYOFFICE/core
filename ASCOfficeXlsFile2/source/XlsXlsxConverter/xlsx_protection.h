#pragma once

#include <iosfwd>
#include <string>

#include "common.h"



namespace oox {

/// \struct xlsx_protection
/// protection 18.8.33
struct xlsx_protection
{
    _CP_OPT(bool) hidden;
    _CP_OPT(bool) locked;    

    bool operator == (const xlsx_protection & rVal) const;
    bool operator != (const xlsx_protection & rVal) const;
    
    friend void xlsx_serialize(std::wostream & _Wostream, const xlsx_protection & font);
    friend std::size_t hash_value(xlsx_protection const & val);
};

}


