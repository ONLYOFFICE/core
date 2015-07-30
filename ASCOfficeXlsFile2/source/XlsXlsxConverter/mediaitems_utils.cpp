
#include "mediaitems_utils.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

namespace oox {
namespace utils {

namespace media {


bool is_internal(const std::wstring & uri)
{
	if (uri.empty())return false;

	return FileSystem::Directory::IsExist(uri);
}

std::wstring get_rel_type(external_items::Type type)
{
    switch (type)
    {
    case external_items::typeImage:
        return L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image";
     case external_items::typeChart:
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
