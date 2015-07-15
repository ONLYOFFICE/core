
#include <ostream>
#include <boost/functional/hash/hash.hpp>

#include "xlsx_protection.h"

namespace oox {

bool xlsx_protection::operator == (const xlsx_protection & rVal) const
{
     const bool res = 
         hidden.get_value_or(false) == rVal.hidden.get_value_or(false) &&
         locked.get_value_or(false) == rVal.locked.get_value_or(false);
     return res;
}

bool xlsx_protection::operator != (const xlsx_protection & rVal) const
{
    return !(this->operator ==(rVal));
}

void xlsx_serialize(std::wostream & _Wostream, const xlsx_protection & protection)
{
    _Wostream << L"<protection ";
    
    if (protection.hidden)
        _Wostream << L"hidden=\"" << protection.hidden.get() << L"\" ";
    if (protection.locked)
        _Wostream << L"locked=\"" << protection.locked.get() << L"\" ";

    _Wostream << L"/>";    
}

std::size_t hash_value(xlsx_protection const & val)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, val.hidden.get_value_or(false));
    boost::hash_combine(seed, val.locked.get_value_or(false));
    return seed;
}
}

