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
// Test.cpp : Defines the entry point for the console application.
//
#include "tchar.h"
#include "../source/ECMACryptFile.h"
#include "../../Common/OfficeFileFormatChecker.h"
#include "../../DesktopEditor/common/File.h"

#ifndef _QT

#if defined(_WIN64)
	#pragma comment(lib, "../../build/bin/icu/win_64/icuuc.lib")
#elif defined (_WIN32)

	#if defined(_DEBUG)
		//#pragma comment(lib, "../../build/lib/win_32/DEBUG/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/kernel.lib")
		#pragma comment(lib, "../../build/lib/win_32/DEBUG/UnicodeConverter.lib")
	#else
		//#pragma comment(lib, "../../build/lib/win_32/graphics.lib")
		#pragma comment(lib, "../../build/lib/win_32/kernel.lib")
	#endif
	#pragma comment(lib, "../../build/bin/icu/win_32/icuuc.lib")
#endif

#endif

int _tmain(int argc, _TCHAR* argv[])
{
#define __CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW	

	std::wstring password = L"123";
	ECMACryptFile crypt_file;
	bool result = false, bDataIntegrity = false;

	std::wstring srcFileName	= L"D:\\tests\\__67\\test.docx";
	std::wstring dstFileName	= L"D:\\tests\\__67\\test.docx-crypt.docx";
	std::wstring dstFileName2	= dstFileName + L".oox";
	
	//std::wstring dstFileName	= srcFileName + L".oox";
	//std::wstring dstFileName2	= dstFileName + L"-mycrypt.docx";

	//std::wstring srcFileName	= L"D:\\test\\_crypted\\test.docx";
	//std::wstring dstFileName3	= srcFileName + L"-mycrypt1.docx";

	//std::wstring srcFileName1	= L"D:\\test\\_crypted\\test-111.docx-ms.docx";
	//std::wstring dstFileName1	= srcFileName1 + L".oox";
	//result = crypt_file.DecryptOfficeFile(srcFileName1, dstFileName1, password, bDataIntegrity);
			

	//result = crypt_file.EncryptOfficeFile(srcFileName, dstFileName, password, L"123456789");
	////result = crypt_file.DecryptOfficeFile(dstFileName, dstFileName2, password, bDataIntegrity);
	//
	//std::wstring addit_name = L"11111111111111111111111111111";

	//for (size_t i = 0; i < 100; ++i)
	//{
	//	std::string addit_info = crypt_file.ReadAdditional(dstFileName, addit_name);

	//	std::wstring temp = NSFile::CFileBinary::CreateTempFileWithUniqueName(L"", L"asd");
	//	//
	//	addit_info += std::string(temp.begin(), temp.end());

	//	crypt_file.WriteAdditional(dstFileName, addit_name, addit_info);
	//}
	std::wstring sFile = srcFileName;
	std::wstring sPassword = L"9ddb2443-b29a-1308-a64e-12988c24ded6";
	std::wstring sDocinfo = L"9ddb2443-b29a-1308-a64e-12988c24ded6";
	bool isCrypt = false;

	COfficeFileFormatChecker oChecker;
	oChecker.isOfficeFile(sFile);

	if (AVS_OFFICESTUDIO_FILE_OTHER_MS_OFFCRYPTO != oChecker.nFileType)
	{
		ECMACryptFile file;
		if (file.EncryptOfficeFile(sFile, sFile, sPassword, sDocinfo))
		{
			for (size_t i = 0; i < 10; ++i)
			{
				file.WriteAdditional(sFile, L"DocumentID", U_TO_UTF8(sDocinfo.substr(i, sDocinfo.size() - i * 2)));
			}
			//file.WriteAdditional(sFile, L"DocumentID", "123");
			isCrypt = true;
		}
	}
	_CrtDumpMemoryLeaks();
	return 0;
}
