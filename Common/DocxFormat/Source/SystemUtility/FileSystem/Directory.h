#pragma once

#include "Settings.h"

#ifdef _WIN32
#include <windows.h>

#include <tchar.h>
#include <atlbase.h>
#include <atlconv.h>
#include <atlstr.h>

#include <tchar.h>
#include <strsafe.h>
#else
#include "../../Base/ASCString.h"
#endif


namespace FileSystem {
	class Directory {
	public:
        static LPCTSTR GetCurrentDirectory();
		static String GetCurrentDirectoryS();
		
		static bool CreateDirectory(LPCTSTR path);
        static bool CreateDirectory(const String& path);
        static bool CreateDirectory(String strFolderPathRoot, String strFolderName);
        static bool CreateDirectories(LPCTSTR path);

		static StringArray GetFilesInDirectory(LPCTSTR path, const bool& andSubdirectories = false);
        static StringArray GetFilesInDirectory(const String& path, const bool& andSubdirectories = false);

        static int GetFilesCount(const CString& path, const bool& recursive = false);
        static CString GetFolderPath(const CString& path);
    };
}
