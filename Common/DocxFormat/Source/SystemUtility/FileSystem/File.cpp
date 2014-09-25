#include "File.h"

namespace FileSystem {
    bool File::Exists(LPCTSTR path) {
#ifdef _WIN32
		WIN32_FIND_DATA findData;
		ZeroMemory(&findData, sizeof(findData));

		HANDLE handle = ::FindFirstFile(path, &findData);

		bool fileExists = true;
		if (handle == INVALID_HANDLE_VALUE)
			fileExists = false;
		FindClose(handle);

		return fileExists;
#else
        std::wstring path_wstring;
        std::wstring path_string;
        return (-1 == access (fname, F_OK));
#endif
	}
    bool File::Exists(const String& path) {
        return Exists(path.c_str());
    }

	void File::Create(LPCTSTR path) {
		CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
    void File::Create(const String& path) {
        Create(path.c_str());
	}
}
