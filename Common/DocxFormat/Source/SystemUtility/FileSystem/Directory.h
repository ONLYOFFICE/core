#pragma once

#include "Settings.h"

#if defined(_WIN32) || defined (_WIN64)
	#include <windows.h>

	#include <tchar.h>
	#include <atlstr.h>

	#include <tchar.h>
	#include <strsafe.h>
	#include <string>
#else
	#include "../../Base/ASCString.h"
#endif


namespace FileSystem {
	class Directory {
	public:
        static LPCTSTR GetCurrentDirectory();
        static String GetCurrentDirectoryS();
		
        static bool			CreateDirectory(LPCTSTR path);
        static bool			CreateDirectory(const String& path);
        static bool			CreateDirectory(String strFolderPathRoot, String strFolderName);
        static CString                  CreateDirectoryWithUniqueName (CString & strFolderPathRoot);
        static bool			CreateDirectories(LPCTSTR path);

        static void DeleteDirectory(const CString& path, bool deleteRoot = true);

        static StringArray GetFilesInDirectory(LPCTSTR path, const bool& andSubdirectories = false);
        static StringArray GetFilesInDirectory(const String& path, const bool& andSubdirectories = false);

        static int GetFilesCount(const CString& path, const bool& recursive = false);
        
		static CString	GetFolderPath(const CString& path);
        static CString	GetLongPathName_(const CString& fileName);
        static CString	GetTempPath();
        static CString	CreateTempFileWithUniqueName (const CString & strFolderPathRoot,CString Prefix);
        static bool		PathIsDirectory(const CString& pathName);

#if defined (_WIN32) || defined(_WIN64)
		static std::wstring GetFolderPath(const std::wstring & strFolderPath);
                static void DeleteDirectory(std::wstring& path, bool deleteRoot = true);
#endif
        static std::wstring CreateDirectoryWithUniqueName (const std::wstring & strFolderPathRoot);

        static bool IsExist(const std::wstring&  strFileName);
	};
}
