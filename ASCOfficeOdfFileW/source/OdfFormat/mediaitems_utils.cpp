

#include "mediaitems_utils.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

namespace cpdoccore { 
namespace odf_writer{
namespace utils {

namespace media {

std::wstring get_rel_type(_mediaitems::Type type)
{
    switch (type)
    {
    case _mediaitems::typeImage:
        return L"";
   default:
        return L"";
    }
}

std::wstring get_default_file_name(_mediaitems::Type type)
{
    switch (type)
    {
    case _mediaitems::typeImage:
        return L"image";
	default:
        return L"media";
    }
}

std::wstring create_file_name(const std::wstring & uri, _mediaitems::Type type, size_t Num)
{
 	std::wstring sExt;
	int n = uri.rfind(L".");
	if (n>=0) sExt = uri.substr(n);
	//todooo проверить
	
	return get_default_file_name(type) + boost::lexical_cast<std::wstring>(Num) + sExt;
}


}

}
}
}
