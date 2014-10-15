#ifndef _BUILD_DIRECTORY_CROSSPLATFORM_H_
#define _BUILD_DIRECTORY_CROSSPLATFORM_H_

#include <stdio.h>
#include <string>
#include "Array.h"

#ifdef WIN32
#include "windows.h"
#include "windef.h"
#elif LINUX
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "File.h"
#elif MAC
#endif

namespace NSDirectory
{
	static CArray<std::wstring> GetFiles(std::wstring strDirectory)
	{
		CArray<std::wstring> oArray;

#ifdef WIN32
		WIN32_FIND_DATAW oFD; 
		
		std::wstring sSpec = strDirectory + L"\\*.*";
		HANDLE hRes = FindFirstFileW( sSpec.c_str(), &oFD );
		if( INVALID_HANDLE_VALUE == hRes )
			return oArray;
		do 
		{
			sSpec = oFD.cFileName;
			if (sSpec != L"." && sSpec != L"..")
			{
				sSpec = strDirectory + L"\\" + sSpec;
				if( !( oFD.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ) 
				{
					oArray.Add(sSpec);
				}
			}
		} while( FindNextFileW( hRes, &oFD ) );		
		FindClose( hRes );

#elif LINUX
#elif MAC
#endif

		return oArray;
	}
	static void CreateDirectory(std::wstring& strDirectory)
	{
#ifdef WIN32
		::CreateDirectoryW(strDirectory.c_str(), NULL);
#elif LINUX
		BYTE* pUtf8 = NULL;
		LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
		struct stat st;
		if (stat((char*)pUtf8, &st) == -1) {
			mkdir((char*)pUtf8, S_IRWXU | S_IRWXG | S_IRWXO);
		}
		delete [] pUtf8;
#elif MAC
#endif
	}
	static bool Exists(const std::wstring& strDirectory)
	{
#ifdef WIN32
		DWORD dwAttrib = ::GetFileAttributesW(strDirectory.c_str());
		return (dwAttrib != INVALID_FILE_ATTRIBUTES && 0 != (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
#elif LINUX
        BYTE* pUtf8 = NULL;
		LONG lLen = 0;
        NSFile::CUtf8Converter::GetUtf8StringFromUnicode(strDirectory.c_str(), strDirectory.length(), pUtf8, lLen, false);
		struct stat st;
		bool bRes = (0 == stat((char*)pUtf8, &st)) && S_ISDIR(st.st_mode);
		delete [] pUtf8;
		return bRes;
#elif MAC
		return true;
#endif
	}
}

#endif //_BUILD_DIRECTORY_CROSSPLATFORM_H_
