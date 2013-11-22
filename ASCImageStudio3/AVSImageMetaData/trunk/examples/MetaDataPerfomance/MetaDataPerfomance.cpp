// MetaDataPerfomance.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include <strsafe.h>
#include "conio.h"
#include <iostream>

#pragma warning(push)
#pragma warning(disable:4192)
#import "../../bin/AVSImageMetaData.dll" rename_namespace("ImageMetaData")
#pragma warning(pop)


bool ListDir(std::wstring & dirstr, std::vector<std::wstring> & listfiles)
{
	WIN32_FIND_DATA ffd;
	LARGE_INTEGER filesize;
	wchar_t szDir[MAX_PATH];
	size_t length_of_arg;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	// Check that the input path plus 2 is not longer than MAX_PATH.

	StringCchLengthW(dirstr.c_str(), MAX_PATH, &length_of_arg);
	if (length_of_arg > (MAX_PATH - 2))
	{
		return false;
	}

	//_tprintf(TEXT("\nTarget directory is %s\n\n"), argv[1]);

	// Prepare string for use with FindFile functions.  First, copy the
	// string to a buffer, then append '\*' to the directory name.

	StringCchCopyW(szDir, MAX_PATH, dirstr.c_str());
	StringCchCatW(szDir, MAX_PATH, L"\\*");

	// Find the first file in the directory.

	hFind = FindFirstFileW(szDir, &ffd);

	if (INVALID_HANDLE_VALUE == hFind) 
	{
		return false;
	} 

	// List all the files in the directory with some info about them.

	do
	{
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//_tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
		}
		else
		{
			filesize.LowPart = ffd.nFileSizeLow;
			filesize.HighPart = ffd.nFileSizeHigh;
			//_tprintf(TEXT("  %s   %ld bytes\n"), ffd.cFileName, filesize.QuadPart);
			listfiles.push_back(std::wstring(ffd.cFileName));
		}
	}
	while (FindNextFileW(hFind, &ffd) != 0);

	dwError = GetLastError();
	if (dwError != ERROR_NO_MORE_FILES) 
	{
		return false;
	}

	FindClose(hFind);
	return true;
}

#define HR_RET(HR) if FAILED(hr = (HR)) {_ASSERTE(SUCCEEDED(hr)); std::cout << "failed at line " << __LINE__ << std::endl << std::flush; _getch(); return hr;}

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr;

	LARGE_INTEGER lpFrequency;
	if (false == QueryPerformanceFrequency(&lpFrequency))
		return 1;

	HR_RET(CoInitialize(NULL));
	std::wstring _DirStr(L"e:/pho/");
	_DirStr = L"E:/Work/AVS/ActiveX/AVS/Sources/AVSImageStudio3/AVSImageMetaData/trunk/testfiles/";
	//_DirStr = L"\\\\Fileserver\\Documents\\Photos\\2009_05_08 - НКТ - День радио\\by Yakovlev\\good\\";
	std::vector<std::wstring> _DirList;
	if (!ListDir(_DirStr,_DirList))
		return 1;

	CComPtr<ImageMetaData::IImageMetaDataParser> MetaDataParser;
	HR_RET(MetaDataParser.CoCreateInstance(__uuidof(ImageMetaData::CImageMetaDataParser)));

	LARGE_INTEGER t1,t2;
	std::vector<std::wstring>::iterator i1 = _DirList.begin();
	while(i1 != _DirList.end())
	{
		std::wstring _file = _DirStr + (*i1);
		try
		{
			QueryPerformanceCounter(&t1);
			MetaDataParser->ParseFile(_bstr_t(_file.c_str()), 0);
			long nMetaDataCount = MetaDataParser->AvailDataTypeCount;
			QueryPerformanceCounter(&t2);
			
			__int64 interval = t2.QuadPart - t1.QuadPart;
			std::wcout << "file " << (*i1) << " \ttime: " << (1.0*interval / lpFrequency.QuadPart ) << " \tmetadatacount: " << nMetaDataCount << std::endl;
		}
		catch(_com_error & er)
		{
			HR_RET(er.Error());
		}
		i1++;
	}
	
	MetaDataParser.Release();
	CoUninitialize();
	_getch();

	return 0;
}

