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

#include <iostream>
#include "../../../DesktopEditor/common/File.h"

#ifdef WIN32
int wmain(int argc, wchar_t** argv)
#else
int main(int argc, char** argv)
#endif
{
	if (0 == argc)
		return 0;

#ifdef WIN32
	std::wstring sFile(argv[argc - 1]);
#else
	std::string sFileA(argv[argc - 1]);
	std::wstring sFile = UTF8_TO_U(sFileA);
#endif

	BYTE* pData = NULL;
	DWORD nSize = 0;

	NSFile::CFileBinary::ReadAllBytes(sFile, &pData, nSize);

	BYTE guidOdttf[] = {0xA0, 0x66, 0xD6, 0x20, 0x14, 0x96, 0x47, 0xfa, 0x95, 0x69, 0xB8, 0x50, 0xB0, 0x41, 0x49, 0x48};

	int nCount = nSize > 32 ? 32 : nSize;
	for (int i = 0; i < nCount; ++i)
		pData[i] ^= guidOdttf[i % 16];

	NSFile::CFileBinary oDst;
	oDst.CreateFileW(NSFile::GetProcessDirectory() + L"/font.ttf");
	oDst.WriteFile(pData, nSize);
	oDst.CloseFile();

	RELEASEARRAYOBJECTS(pData);
	return 0;
}
