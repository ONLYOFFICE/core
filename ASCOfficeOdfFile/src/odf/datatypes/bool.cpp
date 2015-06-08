
#include "bool.h"
#include <boost/algorithm/string.hpp>

#include <ostream>
#include <sstream>

namespace cpdoccore { namespace odf_types { 

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
