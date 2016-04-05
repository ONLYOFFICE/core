// OdfFileTest.cpp 


#include <boost/timer.hpp>
#include <iostream>

#include "../../../OfficeUtils/src/OfficeUtils.h"

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/lexical_cast.hpp>

#include "../../../Common/DocxFormat/Source/Base/Base.h"
#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../../src/ConvertOO2OOX.h"

int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
    boost::timer t1;
//////////////////////////////////////////////////////////////////////////
	std::wstring srcFileName	= argv[1];
	std::wstring dstPath		= argv[2];
	std::wstring outputDir		= FileSystem::Directory::GetFolderPath(dstPath);
	
	std::wstring srcTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
	std::wstring dstTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);

    // распаковываем исходник во временную директорию
	COfficeUtils oCOfficeUtils(NULL);
    if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), srcTempPath.c_str(), NULL, 0))
		return S_FALSE;

	hr = ConvertOO2OOX(srcTempPath, dstTempPath, L"C:\\Windows\\Fonts", false, NULL);

	FileSystem::Directory::DeleteDirectory(srcTempPath);

	if (hr != S_OK)  return hr;
   
	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstPath.c_str(), -1))
        return hr;
	
	FileSystem::Directory::DeleteDirectory(dstTempPath);

////////////////////////////////////////////////////////////////////////
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";   
	return 0;
}
