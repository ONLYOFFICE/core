
#include "../../TxtXmlFormatLib/source/TxtXmlFile.h"

#include "../../../OfficeUtils/src/OfficeUtils.h"

#include "../../../Common/DocxFormat/Source/SystemUtility/FileSystem/Directory.h"

#include <algorithm>
#include <string>

namespace BinDocxRW
{
    int g_nCurFormatVersion = 0; //extern from ...DocxSerializer
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3) return 0;

	std::wstring sXMLOptions = _T("<Options><TXTOptions><Encoding>1000</Encoding></TXTOptions></Options>");

	std::wstring srcFileName	= argv[1];
	std::wstring dstFileName	= argv[2];
	std::wstring outputDir		= FileSystem::Directory::GetFolderPath(dstFileName);

	std::wstring dstTempPath	= FileSystem::Directory::CreateDirectoryWithUniqueName(outputDir);

	int n1 = srcFileName.rfind(_T('.'));
	int n2 = dstFileName.rfind(_T('.'));

	std::wstring ext_1 = n1>=0 ? srcFileName.substr(n1+1, srcFileName.length() - n1): _T(""); //ext_1.MakeLower();
	std::wstring ext_2 = n2>=0 ? dstFileName.substr(n2+1, dstFileName.length() - n2): _T(""); //ext_2.MakeLower();

	std::transform(ext_1.begin(), ext_1.end(), ext_1.begin(), ::tolower);

	CTxtXmlFile txtFile;

	COfficeUtils oCOfficeUtils(NULL);
	if (ext_1 == _T("txt"))
	{	
		// txt->docx
		if (S_OK != txtFile.txt_LoadFromFile(srcFileName, dstTempPath, sXMLOptions))	
			return 2;
		
		if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstFileName.c_str(), -1))
			return S_FALSE;
	}
	if (ext_2 == _T("txt"))
	{	
		// docx->txt
		if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), dstTempPath.c_str(), NULL, 0))
			return S_FALSE;
		
		if (S_OK != txtFile.txt_SaveToFile( dstFileName, dstTempPath, sXMLOptions))
			return 3;
	}

	FileSystem::Directory::DeleteDirectory(dstTempPath);	
	return 0;
}

