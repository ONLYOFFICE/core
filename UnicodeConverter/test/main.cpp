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
#include "../UnicodeConverter.h"
#include "../../DesktopEditor/common/File.h"

int main(int argc, char *argv[])
{
    std::wstring sPathTests = NSFile::GetProcessDirectory() + L"/../testfiles/";

#if 0
    std::wstring sOriginal = sPathTests + L"0.txt";
    for (int i = 1; i < 52; i++)
    {
        std::wstring s1 = sPathTests + std::to_wstring(i) + L".txt";
        std::wstring s2 = sPathTests + std::to_wstring(i) + L"_utf8.txt";
        NSFile::CFileBinary::Copy(sOriginal, s1);
        NSFile::CFileBinary::Copy(sOriginal, s2);
    }

    return 0;
#endif

    NSUnicodeConverter::CUnicodeConverter oConverter;

    int nCountTests = 46;

    for (int i = 0; i < nCountTests; ++i)
    {
        BYTE* pDataSrc = NULL;
        DWORD nLenSrc = 0;

        if (!NSFile::CFileBinary::ReadAllBytes(sPathTests + std::to_wstring(i) + L".txt", &pDataSrc, nLenSrc))
            continue;

        std::wstring sResult = oConverter.toUnicode((const char*)pDataSrc, (unsigned int)nLenSrc, NSUnicodeConverter::Encodings[i].Name);

        std::wstring sResultCache = L"";
        NSFile::CFileBinary::ReadAllTextUtf8(sPathTests + std::to_wstring(i) + L"_utf8.txt", sResultCache);

        bool bIsEqual = (sResult == sResultCache);

        RELEASEARRAYOBJECTS(pDataSrc);

        NSFile::CFileBinary::SaveToFile(sPathTests + std::to_wstring(i) + L"_test_utf8.txt", sResult, true);
    }

#if 0
    std::string str1 = oConverter.fromUnicode(L"рус", "windows-1251");

    std::wstring str2 = oConverter.toUnicode(str1, "windows-1251");
    std::string str3 = oConverter.fromUnicode(str2, "windows-1251");
#endif

    return 0;
}
