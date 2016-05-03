// DocFormatTest.cpp : Defines the entry point for the console application.
//
#include "../DocFormatLib/DocFormatLib.h"

#include "../win32/ASCOfficeCriticalSection.h"

#include "../../OfficeUtils/src/OfficeUtils.h"

#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include <string>
#include <tchar.h>

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring sSrcDoc	= argv[1];
    std::wstring sDstDocx	= argv[2];

	std::wstring outputDir		= FileSystem::Directory::GetFolderPath(sDstDocx);
	std::wstring dstTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);

	// doc->docx
	COfficeDocFile docFile;
	
	HRESULT hRes = docFile.LoadFromFile( sSrcDoc, dstTempPath, NULL);
	
	if (hRes == S_OK)
	{
		COfficeUtils oCOfficeUtils(NULL);
		hRes = oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), sDstDocx, -1);
	}
	
	FileSystem::Directory::DeleteDirectory(dstTempPath);

	return hRes;
}

