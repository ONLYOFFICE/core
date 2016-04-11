// DocFormatTest.cpp : Defines the entry point for the console application.
//
#include <tchar.h>
#include "../RtfFormatLib/source/ConvertationManager.h"

#include "../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include <algorithm>
#include <string>

int _tmain(int argc, _TCHAR* argv[])
{
	std::wstring srcFileName	= argv[1];
	std::wstring dstFileName	= argv[2];
	std::wstring outputDir		= FileSystem::Directory::GetFolderPath(dstFileName);

	std::wstring dstTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);

	int n1 = srcFileName.rfind(_T('.'));
	int n2 = dstFileName.rfind(_T('.'));

	std::wstring ext_1 = n1>=0 ? srcFileName.substr(n1+1, srcFileName.length() - n1): _T(""); //ext_1.MakeLower();
	std::wstring ext_2 = n2>=0 ? dstFileName.substr(n2+1, dstFileName.length() - n2): _T(""); //ext_2.MakeLower();

	std::transform(ext_1.begin(), ext_1.end(), ext_1.begin(), ::tolower);

	RtfConvertationManager rtfConvert;

	rtfConvert.m_sTempFolder = dstTempPath;

	COfficeUtils oCOfficeUtils(NULL);
	if (ext_1 == _T("rtf"))
	{	
		// rtf->docx
		rtfConvert.ConvertRtfToOOX(srcFileName, dstTempPath);
		
		if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstFileName.c_str(), -1))
			return S_FALSE;
	}
	if (ext_2 == _T("rtf"))
	{	
		// docx->rtf
		if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), dstTempPath.c_str(), NULL, 0))
			return S_FALSE;
		
		rtfConvert.ConvertOOXToRtf(dstTempPath, srcFileName);
	}

	FileSystem::Directory::DeleteDirectory(dstTempPath);	

	return 0;
}

