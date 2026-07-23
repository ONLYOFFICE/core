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
// ASCOfficeOdfFileWTest.cpp : Defines the entry point for the console application.
//

#include "../../../../Common/OfficeFileFormatChecker.h"
#include "../../../../OfficeUtils/src/OfficeUtils.h"

#include <iostream>
#include <string>

#include "../../../../OOXML/Base/Base.h"
#include "../../../../DesktopEditor/common/Directory.h"

#include "../../../Writer/Converter/Oox2OdfConverter.h"

#if defined(_WIN64)
	#pragma comment(lib, "../../../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)

	#if defined(_DEBUG)
		#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/kernel_network")
		#pragma comment(lib, "../../../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
	#else
		#pragma comment(lib, "../../../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/kernel.lib")
		#pragma comment(lib, "../../../../build/lib/win_32/kernel_network")
		#pragma comment(lib, "../../../../build/lib/win_32/UnicodeConverter.lib")
	#endif
	#pragma comment(lib, "../../../../build/bin/icu/win_32/icuuc.lib")
#endif

HRESULT convert_single(std::wstring srcFileName)
{
	COfficeFileFormatChecker fileChecker(srcFileName);

	std::wstring dstPath = srcFileName;// + ....

	bool bTemplate = false;
	
	std::wstring type;
	switch(fileChecker.nFileType)
	{
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCX_FLAT:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOCM:		dstPath += L"-my.odt"; type = L"text";		break;
		
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTX:
		case AVS_OFFICESTUDIO_FILE_DOCUMENT_DOTM:		dstPath += L"-my.ott"; type = L"text"; bTemplate = true; break;

		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSX_FLAT:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLSM:	dstPath += L"-my.ods"; type = L"spreadsheet";	break;
		
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTX:
		case AVS_OFFICESTUDIO_FILE_SPREADSHEET_XLTM:	dstPath += L"-my.ots"; type = L"spreadsheet"; bTemplate = true; break;

		case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTX:
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_PPTM:	dstPath += L"-my.odp"; type = L"presentation";	break;

		case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTX:
		case AVS_OFFICESTUDIO_FILE_PRESENTATION_POTM:	dstPath += L"-my.otp"; type = L"presentation"; bTemplate = true; break;
		default:
			return S_FALSE;
	}

//------------------------------------------------------------------------------------------
	HRESULT hr = S_OK;

	std::wstring outputDir		= NSDirectory::GetFolderPath(dstPath);
	
	std::wstring srcTempPath	= NSDirectory::CreateDirectoryWithUniqueName(outputDir);
	std::wstring dstTempPath	= NSDirectory::CreateDirectoryWithUniqueName(outputDir);

    // unpack source to temporary directory
	COfficeUtils oCOfficeUtils(NULL);
    if (S_OK != oCOfficeUtils.ExtractToDirectory(srcFileName.c_str(), srcTempPath.c_str(), NULL, 0))
	{
		//may be flat
		srcTempPath = srcFileName;
	}

	Oox2Odf::Converter converter(srcTempPath, type, L"C:\\Windows\\Fonts", bTemplate);

	std::wstring sPassword;// = L"password";
	
	if (false == converter.convert())
	{
		return S_FALSE;
	}
	if (false == converter.write(dstTempPath, srcTempPath, sPassword, L"hiuh56f56tfy7g"))
	{
		return S_FALSE;
	}
	if (srcFileName != srcTempPath)
	{
		NSDirectory::DeleteDirectory(srcTempPath);
	}
   
	if (S_OK != oCOfficeUtils.CompressFileOrDirectory(dstTempPath.c_str(), dstPath.c_str(), false, sPassword.empty() ? Z_DEFLATED : 0))
        return hr;
	
	NSDirectory::DeleteDirectory(dstTempPath);

	return S_OK;
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