#include "precompiled_cpodf.h"

#include "mediaitems_utils.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <cpdoccore/common/boost_filesystem_version.h>

namespace cpdoccore { 
namespace odf{
namespace utils {

namespace media {

using boost::filesystem::wpath;

std::wstring get_rel_type(mediaitems::Type type)
{
    switch (type)
    {
    case mediaitems::typeImage:
        return L"";
   default:
        return L"";
    }
}

std::wstring get_default_file_name(mediaitems::Type type)
{
    switch (type)
    {
    case mediaitems::typeImage:
        return L"image";
	default:
        return L"media";
    }
}

std::wstring create_file_name(const std::wstring & uri, mediaitems::Type type, size_t Num)
{
#ifdef BOOST_FILESYSTEM_LEGACY
    return get_default_file_name(type) + boost::lexical_cast<std::wstring>(Num) + wpath(uri).extension();
#else
    return get_default_file_name(type) + boost::lexical_cast<std::wstring>(Num) + wpath(uri).extension().string<std::wstring>();
#endif
}


}

}
}
}
