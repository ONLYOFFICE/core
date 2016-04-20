// PptFormatTest.cpp : Defines the entry point for the console application.
//
#include "../../PPTFormatLib/PPTFormatLib.h"
#include "../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3) return 3;

	std::wstring sSrcPpt	= argv[1];
    std::wstring sDstPptx	= argv[2];

	std::wstring outputDir		= FileSystem::Directory::GetFolderPath(sDstPptx);
	std::wstring dstTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);

	std::wstring tempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);
	// Ppt->Pptx
		COfficePPTFile pptFile;
		
		pptFile.put_TempDirectory(tempPath);

		HRESULT hRes = pptFile.LoadFromFile(sSrcPpt, dstTempPath);
	
	if (hRes != S_OK)return 2;
	
	COfficeUtils oCOfficeUtils(NULL);
	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), sDstPptx, -1))
        return 1;
	
	FileSystem::Directory::DeleteDirectory(dstTempPath);
	FileSystem::Directory::DeleteDirectory(tempPath);
	return 0;
}

