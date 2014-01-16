#ifndef _BUILD_DIRECTORY_CROSSPLATFORM_H_
#define _BUILD_DIRECTORY_CROSSPLATFORM_H_

#include <stdio.h>
#include <string>
#include "Array.h"

#ifdef WIN32
#include "windows.h"
#include "windef.h"
#elif LINUX
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
}

#endif //_BUILD_DIRECTORY_CROSSPLATFORM_H_