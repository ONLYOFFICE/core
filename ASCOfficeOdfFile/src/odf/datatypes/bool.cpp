#include "../precompiled_cpodf.h"
#include "bool.h"

#include <ostream>
#include <sstream>

namespace cpdoccore { namespace odf { 

std::wostream & operator << (std::wostream & _Wostream, const Bool & _Val)
{
	_Wostream << _Val.string();
    return _Wostream;
}
Bool Bool::parse(const std::wstring & Str)
{
    std::wstring tmp = Str;
    boost::algorithm::to_lower(tmp);

	if (tmp == L"0" || tmp == L"false") return Bool(false);
	else return Bool(true);
}
} 
}
