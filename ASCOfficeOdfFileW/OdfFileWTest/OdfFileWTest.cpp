﻿/*
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
// ASCOfficeOdfFileWTest.cpp : Defines the entry point for the console application.
//

#include "../../Common/OfficeFileFormatChecker.h"
#include "../../OfficeUtils/src/OfficeUtils.h"

#include <iostream>
#include <string>

#include "../../Common/DocxFormat/Source/Base/Base.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../source/Oox2OdfConverter/Oox2OdfConverter.h"

#if defined(_WIN64)
	#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)
	#pragma comment(lib, "../../build/bin/icu/win_32/icuuc.lib")
#endif

HRESULT convert_single(std::wstring srcFileName)
{
	COfficeFileFormatChecker fileChecker(srcFileName);

	std::wstring dstPath = srcFileName;// + ....

	std::wstring type;
	switch(fileChecker.nFileType)
	{
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:		dstPath += L"-my.odt"; type = L"text";		break;
		
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:	dstPath += L"-my.ods"; type = L"spreadsheet";	break;
		
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:	dstPath += L"-my.odp"; type = L"presentation";	break;

		default:
			return S_FALSE;
	}

//------------------------------------------------------------------------------------------
	HRESULT hr = S_OK;

	std::wstring outputDir		= NSDirectory::GetFolderPath(dstPath);
	
	std::wstring srcTempPath	= NSDirectory::CreateDirectoryWithUniqueName(outputDir);
	std::wstring dstTempPath	= NSDirectory::CreateDirectoryWithUniqueName(outputDir);

    // распаковываем исходник во временную директорию
	COfficeUtils oCOfficeUtils(NULL);
    if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), srcTempPath.c_str(), NULL, 0))
		return S_FALSE;

	Oox2Odf::Converter converter(srcTempPath, type, L"C:\\Windows\\Fonts", NULL);

	converter.convert();
	converter.write(dstTempPath);

	NSDirectory::DeleteDirectory(srcTempPath);

	if (hr != S_OK)  return hr;
   
	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstPath.c_str(), -1))
        return hr;
	
	NSDirectory::DeleteDirectory(dstTempPath);

	return 0;
}
HRESULT convert_directory(std::wstring pathName)
{
	HRESULT hr = S_OK;

	std::vector<std::wstring> arFiles = NSDirectory::GetFiles(pathName, false);

	for (size_t i = 0; i < arFiles.size(); i++)
	{
		convert_single(arFiles[i]);
	}
	return S_OK;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 2) return 1;

	HRESULT hr = -1;
	if (NSFile::CFileBinary::Exists(argv[1]))
	{	
		hr = convert_single(argv[1]);
	}
	else if (NSDirectory::Exists(argv[1]))
	{
		hr = convert_directory(argv[1]);
	}

	return hr;
}