/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
// OdfFileTest.cpp 

#include <stdio.h>
#include <tchar.h>

#include "../../../../Common/OfficeFileFormatChecker.h"
#include "../../../../OfficeUtils/src/OfficeUtils.h"
#include "../../../../DesktopEditor/common/Directory.h"
#include "../../../Reader/Converter/ConvertOO2OOX.h"
#include "../../../Common/logging.h"


#if defined(_WIN64)
	#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)

	#if defined(_DEBUG)
		#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/UnicodeConverter.dll")
#else
		#pragma comment(lib, "../../../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/kernel.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/UnicodeConverter.dll")
#endif
	#pragma comment(lib, "../../../../build/bin/icu/win_32/icuuc.lib")
#endif

HRESULT convert_single(std::wstring srcFileName)
{
	int nResult = 0;

	COfficeFileFormatChecker fileChecker(srcFileName);

	std::wstring dstPath = srcFileName;// + ; //xlsx pptx docx
	switch(fileChecker.nFileType)
	{
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT:
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT:
	case AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT_FLAT:		dstPath += L"-my.docx"; break;
	
	case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS:
	case AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS:
	case AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS_FLAT:	dstPath += L"-my.xlsx"; break;
	
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP:
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP:
	case AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP_FLAT:	dstPath += L"-my.pptx"; break;

	default:
		return -1;
	}
//---------------------------------------------------------------------------------------------------
	COfficeUtils oCOfficeUtils(NULL);
	
	std::wstring outputDir		= NSDirectory::GetFolderPath(dstPath);
	std::wstring dstTempPath	= NSDirectory::CreateDirectoryWithUniqueName(outputDir);
	std::wstring srcTempPath;
	std::wstring srcTempPath2 = NSDirectory::CreateDirectoryWithUniqueName(outputDir);

	if (fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_ODT		||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_ODS	||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_ODP ||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_DOCUMENT_OTT		||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_SPREADSHEET_OTS	||
		fileChecker.nFileType == AVS_OFFICESTUDIO_FILE_PRESENTATION_OTP)
	{		
		srcTempPath	= NSDirectory::CreateDirectoryWithUniqueName(outputDir);

		if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), srcTempPath.c_str(), NULL, 0))
			return -2;
	}
	else // flat
	{
		srcTempPath = srcFileName;
	}
    _CP_LOG << L"[info] " << srcFileName << std::endl;
	
	nResult = ConvertODF2OOXml(srcTempPath, dstTempPath, L"C:\\Windows\\Fonts", srcTempPath2, L"");

	if (srcTempPath != srcFileName)
	{
		NSDirectory::DeleteDirectory(srcTempPath);
	}
	NSDirectory::DeleteDirectory(srcTempPath2);

	if (nResult == 0)
	{   
		if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath, dstPath, true))
			nResult = -2;
	}
	
	NSDirectory::DeleteDirectory(dstTempPath);

	return nResult;
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