#ifndef _CPDOCCORE_XLSX_PROTECTION_H_INCLUDED_
#define _CPDOCCORE_XLSX_PROTECTION_H_INCLUDED_
#pragma once

#include <cpdoccore/CPOptional.h>
#include <iosfwd>
#include <string>


namespace cpdoccore {
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
}

#endif
