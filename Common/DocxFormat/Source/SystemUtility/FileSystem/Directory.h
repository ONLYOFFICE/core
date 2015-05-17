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
        static CString		CreateDirectoryWithUniqueName (CString & strFolderPathRoot);
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
		static std::wstring CreateDirectoryWithUniqueName (std::wstring & strFolderPathRoot);
		static void			DeleteDirectory(std::wstring& path, bool deleteRoot = true);
#endif

		static bool IsExist(const std::wstring&  strFileName)
		{
#if defined(_WIN32) || defined(_WIN32_WCE) || defined(_WIN64)
			FILE* pFile = _wfopen(strFileName.c_str(), L"rb");
#else
			BYTE* pUtf8 = NULL;
			LONG lLen = 0;
			CUtf8Converter::GetUtf8StringFromUnicode(strFileName.c_str(), strFileName.length(), pUtf8, lLen, false);
			FILE* pFile = fopen((char*)pUtf8, "rb");
			delete [] pUtf8;
#endif
			if (NULL != pFile)
			{
				fclose(pFile);
				return true;
			}
			else
				return false;
		}	
	};
}
