// ASCOfficeXlsFileTest.cpp : Defines the entry point for the console application.
//

#include <boost/timer.hpp>
#include <iostream>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include "../source/XlsXlsxConverter/ConvertXls2Xlsx.h"
#include "../source/XlsXlsxConverter/progressCallback.h"

#include "../Common/XmlUtils.h"

#include "../../OfficeUtils/src/OfficeUtils.h"


int _tmain(int argc, _TCHAR* argv[])
{
	HRESULT hr = S_OK;
    boost::timer t1;
//////////////////////////////////////////////////////////////////////////
	std::wstring srcFileName	= argv[1];
	std::wstring dstPath		= argv[2];
	std::wstring outputDir		= FileSystem::Directory::GetFolderPath(dstPath);
	
	std::wstring dstTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);

	hr = ConvertXls2Xlsx(srcFileName, dstTempPath, L"password1", L"C:\\Windows\\Fonts", NULL);

	if (hr == S_OK) 
	{
		COfficeUtils oCOfficeUtils(NULL);
		hr = oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstPath.c_str(), -1);
	}
	
	FileSystem::Directory::DeleteDirectory(dstTempPath);

////////////////////////////////////////////////////////////////////////
    std::cout << "\n\nTime : " << t1.elapsed() << "\n";   

	return hr;
}
