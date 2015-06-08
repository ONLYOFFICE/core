
#include "mediaitems_utils.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"


namespace cpdoccore { 
namespace oox {
namespace utils {

namespace media {


bool is_internal(const std::wstring & uri, const std::wstring & packetRoot)
{
    std::wstring mediaPath = boost::regex_search(uri.begin(), uri.end(), boost::wregex(L"^/[A-Za-z]:")) 
        ? std::wstring(uri.begin() + 1, uri.end()) 
        : uri;

	std::wstring  resultPath = packetRoot + FILE_SEPARATOR_STR + mediaPath;

	return FileSystem::Directory::IsExist(resultPath);
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



std::wstring replace_extension(const std::wstring & ext)
{
    // TODO
    if (ext == L"jpg")
        return L"jpeg";
    else
        return ext;
}

}

}
}
}
