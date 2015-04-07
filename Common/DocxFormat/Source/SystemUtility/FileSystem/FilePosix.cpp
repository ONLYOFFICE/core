#include "File.h"
#include <unistd.h>

namespace FileSystem {
    bool File::Exists(LPCTSTR path) {
        std::wstring path_wstring;
        path_wstring = path;
        std::string path_string = stringWstingToUtf8String(path_wstring);
        return (0 == access (path_string.c_str(), F_OK));
	}
    bool File::Exists(const String& path) {
        return Exists(path.c_str());
    }

    // wtf? same behavior has win32 version of code
	void File::Create(LPCTSTR path) {
        std::wstring path_wstring;
        path_wstring = path;
        std::string path_string = stringWstingToUtf8String(path_wstring);
        FILE * pFile = fopen (path_string.c_str(), "wb");
	}
    void File::Create(const String& path) {
        Create(path.c_str());
	}
}
