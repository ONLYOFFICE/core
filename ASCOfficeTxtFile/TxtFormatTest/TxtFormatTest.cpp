/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

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

