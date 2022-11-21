/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "../TxtXmlFormatLib/source/TxtXmlFile.h"

#include "../../OfficeUtils/src/OfficeUtils.h"
#include "../../DesktopEditor/common/Directory.h"

#include <algorithm>
#include <string>
#include <tchar.h>

#if defined(_WIN64)
	#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)

	#if defined(_DEBUG)
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
	#else
		#pragma comment(lib, "../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/kernel.lib")
		#pragma comment(lib, "../../build/lib/win_32/UnicodeConverter.lib")
	#endif
	#pragma comment(lib, "../../build/bin/icu/win_32/icuuc.lib")
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2) return 0;

	std::wstring sXMLOptions = _T("<Options><TXTOptions><Encoding>50</Encoding></TXTOptions></Options>");

	std::wstring srcFileName	= argv[1];
	int n1 = srcFileName.rfind(_T('.'));
	std::wstring ext_1 = n1 >= 0 ? srcFileName.substr(n1+1, srcFileName.length() - n1) : _T("");
	
	std::transform(ext_1.begin(), ext_1.end(), ext_1.begin(), ::tolower);

	std::wstring dstFileName	= argc > 2 ? argv[2] : srcFileName + L"_my." + (ext_1 == L"txt" ? L"docx" : L"txt");
	
	std::wstring outputDir		= NSDirectory::GetFolderPath(dstFileName);
	std::wstring dstTempPath	= NSDirectory::CreateDirectoryWithUniqueName(outputDir);

	CTxtXmlFile txtFile;

	COfficeUtils oCOfficeUtils(NULL);
	if (ext_1 == L"txt")
	{	
		// txt->docx
		if (S_OK != txtFile.txt_LoadFromFile(srcFileName, dstTempPath, sXMLOptions))	
			return 2;
		
		if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstFileName.c_str(), true))
			return S_FALSE;
	}
	else
	{	
		// docx->txt
		if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), dstTempPath.c_str(), NULL, 0))
			return S_FALSE;
		
		if (S_OK != txtFile.txt_SaveToFile( dstFileName, dstTempPath, sXMLOptions))
			return 3;
	}

	NSDirectory::DeleteDirectory(dstTempPath);	
	return 0;
}

