#include "../odf/precompiled_cpodf.h"
#include "mediaitems_utils.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <cpdoccore/common/boost_filesystem_version.h>

namespace cpdoccore { 
namespace oox {
namespace utils {

namespace media {

using boost::filesystem::wpath;


bool is_internal(const std::wstring & uri, const std::wstring & packetRoot)
{
    std::wstring s = boost::regex_search(uri.begin(), uri.end(), boost::wregex(L"^/[A-Za-z]:")) 
        ? std::wstring(uri.begin() + 1, uri.end()) 
        : uri;

    wpath packetRootPath(packetRoot);
    wpath mediaPath(s);
    wpath resultPath = packetRoot / mediaPath;

    return boost::filesystem::exists(resultPath);
}

std::wstring get_rel_type(mediaitems::Type type)
{
    switch (type)
    {
    case mediaitems::typeImage:
        return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";
     case mediaitems::typeChart:
        return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/chart";
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
    case mediaitems::typeChart:
        return L"chart";
	default:
        return L"";
    }
}

std::wstring replace_extension(const std::wstring & ext)
{
    // TODO
    if (ext == L"jpg")
        return L"jpeg";
    else
        return ext;
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
